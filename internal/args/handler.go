// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package args implements the commandline interface and calls the appropriate commands.
package args

import (
	"errors"
	"fmt"
	"io"
	"os"
	"strings"
	"sync"
	"time"

	"github.com/rokath/trice/internal/com"
	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/internal/link"
	"github.com/rokath/trice/internal/receiver"
	"github.com/rokath/trice/pkg/cage"
	"github.com/rokath/trice/pkg/cipher"
	"github.com/rokath/trice/pkg/msg"
)

// Handler is called in main, evaluates args and calls the appropriate functions.
// It returns for program exit.
func Handler(w io.Writer, args []string) error {

	id.FnJSON = id.ConditionalFilePath(id.FnJSON)

	if Date == "" { // goreleaser will set Date, otherwise use file info.
		fi, err := os.Stat(os.Args[0])
		if nil == err { // On running main tests fileinfo is invalid, so do not use in that case.
			Date = fi.ModTime().String()
		}
	}
	cage.DefaultLogfileName = "2006-01-02_1504-05_trice.log"

	// Verify that a sub-command has been provided: os.Arg[0] is the main command (trice), os.Arg[1] will be the sub-command.
	if len(args) < 2 {
		m := "no args, try: 'trice help'"
		return errors.New(m)
	}

	// Switch on the sub-command. Parse the flags for appropriate FlagSet.
	// FlagSet.Parse() requires a set of arguments to parse as input.
	// os.Args[2:] will be all arguments starting after the sub-command at os.Args[1]
	subCmd := args[1]
	subArgs := args[2:]
	switch subCmd { // Check which sub-command is invoked.
	default:
		return fmt.Errorf("unknown sub-command '%s'. try: 'trice help|h'", subCmd)
	case "h", "help":
		msg.OnErr(fsScHelp.Parse(subArgs))
		distributeArgs(w)
		return scHelp(w)
	case "s", "scan":
		msg.OnErr(fsScScan.Parse(subArgs))
		distributeArgs(w)
		_, err := com.GetSerialPorts(w)
		return err
	case "ver", "version":
		msg.OnErr(fsScVersion.Parse(subArgs))
		distributeArgs(w)
		return scVersion(w)
	case "renew":
		msg.OnErr(fsScRenew.Parse(subArgs))
		distributeArgs(w)
		return id.SubCmdReNewList(w)
	case "r", "refresh":
		msg.OnErr(fsScRefresh.Parse(subArgs))
		distributeArgs(w)
		return id.SubCmdRefreshList(w)
	case "u", "update":
		msg.OnErr(fsScUpdate.Parse(subArgs))
		distributeArgs(w)
		return id.SubCmdUpdate(w)
	case "zeroSourceTreeIds":
		msg.OnErr(fsScZero.Parse(subArgs))
		distributeArgs(w)
		return id.ScZero(w, *pSrcZ, fsScZero)
	case "sd", "shutdown":
		msg.OnErr(fsScSdSv.Parse(subArgs))
		distributeArgs(w)
		return emitter.ScShutdownRemoteDisplayServer(w, 0) // 0|1: 0=no 1=with shutdown timestamp in display server
	case "ds", "displayServer":
		msg.OnErr(fsScSv.Parse(subArgs))
		distributeArgs(w)
		return emitter.ScDisplayServer(w) // endless loop
	case "l", "log":
		msg.OnErr(fsScLog.Parse(subArgs))
		distributeArgs(w)
		logLoop(w) // endless loop
		return nil
	}
}

type selector struct {
	flag bool
	info func(io.Writer) error
}

// logLoop prepares writing and lut and provides a retry mechanism for unplugged UART.
func logLoop(w io.Writer) {
	msg.FatalOnErr(cipher.SetUp()) // does nothing when -password is ""
	if decoder.TestTableMode {
		// set switches if they not set already
		// trice l -ts off -prefix " }, ``" -suffix "\n``}," -color off
		if emitter.TimestampFormat == "LOCmicro" {
			emitter.TimestampFormat = "off"
		}
		if DefaultPrefix == emitter.Prefix {
			emitter.Prefix = " }, `"
		}
		if emitter.Suffix == "" {
			emitter.Suffix = "`},"
		}
		if emitter.ColorPalette == "default" {
			emitter.ColorPalette = "off"
		}
	}
	c := cage.Start(w, cage.Name)
	defer cage.Stop(w, c)
	var lu id.TriceIDLookUp
	if id.FnJSON == "emptyFile" { // reserved name for tests only
		lu = make(id.TriceIDLookUp)
	} else {
		lu = id.NewLut(id.FnJSON) // lut is a map, that means a pointer
	}
	m := new(sync.RWMutex) // m is a pointer to a read write mutex for lu
	m.Lock()
	lu.AddFmtCount()
	m.Unlock()
	// Just in case the id list file FnJSON gets updated, the file watcher updates lut.
	// This way trice needs NOT to be restarted during development process.
	go lu.FileWatcher(w, m)

	sw := emitter.New(w)
	var interrupted bool
	var counter int

	for {
		rc, e := receiver.NewReadCloser(w, receiver.Port, receiver.PortArguments)
		if nil != e {
			fmt.Fprint(w, e)
			if !interrupted {
				//cage.Stop(c)
				return // hopeless
			}
			time.Sleep(1000 * time.Millisecond) // retry interval
			fmt.Fprintf(w, "\rsig:(re-)setup input port...%d", counter)
			counter++
			continue
		}
		defer func() { msg.OnErr(rc.Close()) }()
		interrupted = true
		if receiver.ShowInputBytes {
			rc = receiver.NewBytesViewer(rc)
		}
		e = decoder.Translate(w, sw, lu, m, rc)
		if io.EOF == e {
			return // end of predefined buffer
		}
	}
}

// scVersion is sub-command 'version'. It prints version information.
func scVersion(w io.Writer) error {
	cage.Enable(w)
	defer cage.Disable(w)
	if verbose {
		fmt.Fprintln(w, "https://github.com/rokath/trice")
	}
	if Version != "" {
		fmt.Fprintf(w, "version=%v, commit=%v, built at %v\n", Version, Commit, Date)
	} else {
		fmt.Fprintf(w, "version=devel, built %s\n", Date)
	}
	return nil
}

// distributeArgs is distributing values used in several packages.
// It must not be called before the appropriate arg parsing.
func distributeArgs(w io.Writer) {
	replaceDefaultArgs()
	com.Verbose = verbose
	id.Verbose = verbose
	link.Verbose = verbose
	cage.Verbose = verbose
	decoder.Verbose = verbose
	emitter.Verbose = verbose
	emitter.TestTableMode = decoder.TestTableMode
	evaluateColorPalette(w)
}

// replaceDefaultArgs assigns port specific default strings.
func replaceDefaultArgs() {
	if receiver.PortArguments == "" { // nothing assigned in args
		if strings.HasPrefix(receiver.Port, "COM") {
			receiver.PortArguments = defaultCOMArgs
		} else {
			switch receiver.Port {
			case "JLINK", "STLINK", "J-LINK", "ST-LINK":
				receiver.PortArguments = defaultLinkArgs
			case "BUFFER":
				receiver.PortArguments = defaultBUFFERArgs
			}
		}
	}
}

// evaluateColorPalette
func evaluateColorPalette(w io.Writer) {
	switch emitter.ColorPalette {
	case "off", "none", "default", "color":
		return
	default:
		fmt.Fprintln(w, "Ignoring unknown -color", emitter.ColorPalette, "using default.")
		emitter.ColorPalette = "default"
	}
}
