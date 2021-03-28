// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package args implemets the commandline interface and calls the appropriate commands.
package args

import (
	"errors"
	"fmt"
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
func Handler(args []string) error {

	id.FnJSON = id.ConditionalFilePath(id.FnJSON)

	if "" == Date { // goreleaser will set Date, otherwise use file info.
		fi, err := os.Stat(os.Args[0])
		if nil == err { // On running main tests fileinfo is invalid, so do not use in that case.
			Date = fi.ModTime().String()
		}
	}
	cage.DefaultLogfileName = "2006-01-02_1504-05_trice.log"

	// Verify that a subcommand has been provided: os.Arg[0] is the main command (trice), os.Arg[1] will be the subcommand.
	if len(args) < 2 {
		m := "no args, try: 'trice help'"
		return errors.New(m)
	}

	// Switch on the subcommand. Parse the flags for appropriate FlagSet.
	// FlagSet.Parse() requires a set of arguments to parse as input.
	// os.Args[2:] will be all arguments starting after the subcommand at os.Args[1]
	subCmd := args[1]
	subArgs := args[2:]
	switch subCmd { // Check which subcommand is invoked.
	default:
		return fmt.Errorf("unknown subcommand '%s'. try: 'trice help|h'", subCmd)
	case "h", "help":
		msg.OnErr(fsScHelp.Parse(subArgs))
		distributeArgs()
		return scHelp()
	case "s", "scan":
		msg.OnErr(fsScScan.Parse(subArgs))
		distributeArgs()
		_, err := com.GetSerialPorts()
		return err
	case "ver", "version":
		msg.OnErr(fsScVersion.Parse(subArgs))
		distributeArgs()
		return scVersion()
	case "renew":
		msg.OnErr(fsScRenew.Parse(subArgs))
		distributeArgs()
		return id.SubCmdReNewList()
	case "r", "refresh":
		msg.OnErr(fsScRefresh.Parse(subArgs))
		distributeArgs()
		return id.SubCmdRefreshList()
	case "u", "update":
		msg.OnErr(fsScUpdate.Parse(subArgs))
		distributeArgs()
		return id.SubCmdUpdate()
	case "zeroSourceTreeIds":
		msg.OnErr(fsScZero.Parse(subArgs))
		distributeArgs()
		return id.ScZero(*pSrcZ, fsScZero)
	case "sd", "shutdown":
		msg.OnErr(fsScSdSv.Parse(subArgs))
		distributeArgs()
		return emitter.ScShutdownRemoteDisplayServer(1)
	case "ds", "displayServer":
		msg.OnErr(fsScSv.Parse(subArgs))
		distributeArgs()
		return emitter.ScDisplayServer() // endless loop
	case "l", "log":
		msg.OnErr(fsScLog.Parse(subArgs))
		distributeArgs()
		logLoop() // endless loop
		return nil
	}
}

type selector struct {
	flag bool
	info func() error
}

// logLoop prepares writing and lut and provides a retry mechanism for unplugged UART.
func logLoop() {
	msg.FatalOnErr(cipher.SetUp()) // does nothing when -password is ""
	if decoder.TestTableMode {
		// set switches if they not set already
		// trice l -ts off -prefix " }, ``" -suffix "\n``}," -color off
		if "LOCmicro" == emitter.TimestampFormat {
			emitter.TimestampFormat = "off"
		}
		if defaultPrefix == emitter.Prefix {
			//emitter.Prefix = " }, \""
			emitter.Prefix = " }, `"
		}
		if "" == emitter.Suffix {
			//emitter.Suffix = "\\\\n\"},"
			emitter.Suffix = "`},"
		}
		if "default" == emitter.ColorPalette {
			emitter.ColorPalette = "off"
		}
	}
	c := cage.Start(cage.Name)
	defer cage.Stop(c)
	lu := id.NewLut(id.FnJSON) // lut is a map, that means a pointer
	m := new(sync.RWMutex)     // m is a pointer to a read write mutex for lu
	m.Lock()
	lu.AddFmtCount()
	m.Unlock()
	// Just in case the id list file FnJSON gets updated, the file watcher updates lut.
	// This way trice needs NOT to be restarted during development process.
	go lu.FileWatcher(m)

	sw := emitter.New()
	var interrupted bool
	var counter int

	for {
		rc, e := receiver.NewReadCloser(receiver.Port, receiver.PortArguments)
		if nil != e {
			fmt.Println(e)
			if !interrupted {
				cage.Stop(c)
				return // hopeless
			}
			time.Sleep(1000 * time.Millisecond) // retry interval
			fmt.Printf("\rsig:(re-)setup input port...%d", counter)
			counter++
			continue
		}
		defer func() { msg.OnErr(rc.Close()) }()
		if receiver.ShowInputBytes {
			rc = receiver.NewBytesViewer(rc)
		}

		decoder.Translate(sw, lu, m, rc)
		interrupted = true
	}
}

// scVersion is subcommand 'version'. It prints version information.
func scVersion() error {
	cage.Enable()
	defer cage.Disable()
	if verbose {
		fmt.Println("https://github.com/rokath/trice")
	}
	if "" != Version {
		fmt.Printf("version=%v, commit=%v, built at %v\n", Version, Commit, Date)
	} else {
		fmt.Printf("version=devel, built %s\n", Date)
	}
	return nil
}

// distributeArgs is distibuting values used in several packages.
// It must not be called before the appropriate arg parsing.
func distributeArgs() {
	replaceDefaultArgs()
	com.Verbose = verbose
	id.Verbose = verbose
	link.Verbose = verbose
	cage.Verbose = verbose
	decoder.Verbose = verbose
	emitter.Verbose = verbose
	emitter.TestTableMode = decoder.TestTableMode
}

// replaceDefaultArgs assigns port specific default strings.
func replaceDefaultArgs() {
	if strings.HasPrefix(receiver.Port, "COM") {
		receiver.PortArguments = defaultCOMArgs
	} else {
		// switch receiver.Port {
		// case "JLINK", "STLINK", "J-LINK", "ST-LINK":
		receiver.PortArguments = defaultLinkArgs
		//case "BUFFER":
		//	receiver.PortArguments = defaultBUFFERArgs
		//}
	}
}
