// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package args implements the commandline interface and calls the appropriate commands.
package args

import (
	"errors"
	"flag"
	"fmt"
	"io"
	"os"
	"runtime/debug"
	"sync"
	"time"

	"github.com/kr/pretty"
	"github.com/rokath/trice/internal/com"
	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/do"
	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/internal/receiver"
	"github.com/rokath/trice/internal/translator"
	"github.com/rokath/trice/pkg/cipher"
	"github.com/rokath/trice/pkg/msg"
	"github.com/spf13/afero"
)

// Handler is called in main, evaluates args and calls the appropriate functions.
// It returns for program exit.
func Handler(w io.Writer, fSys *afero.Afero, args []string) error {

	if Date == "" { // goreleaser will set Date, otherwise use file info.
		path, err := os.Executable()
		if err != nil {
			Date = fmt.Sprint(err) // just in case, simply show the error
		} else {
			fi, err := fSys.Stat(path)
			if err == nil { // On running main tests file-info is invalid, so do not use it in that case.
				Date = fi.ModTime().String()
			} else {
				Date = fmt.Sprint(err) // just in case, simply show the error
			}
		}
	}

	// Verify that a sub-command has been provided: os.Arg[0] is the main command (trice), os.Arg[1] will be the sub-command.
	if len(args) < 2 {
		m := "no args, try: '" + args[0] + " help'"
		return errors.New(m)
	}

	// Switch on the sub-command. Parse the flags for appropriate FlagSet.
	// FlagSet.Parse() requires a set of arguments to parse as input.
	// os.Args[2:] will be all arguments starting after the sub-command at os.Args[1]
	subCmd := args[1]
	subArgs := args[2:]
	switch subCmd { // Check which sub-command is invoked.
	default:
		return fmt.Errorf("unknown sub-command '%s'. try: '%s help|h'", subCmd, args[0])
	case "h", "help":
		msg.OnErr(fsScHelp.Parse(subArgs))
		w = do.DistributeArgs(w, fSys, LogfileName, Verbose)
		return scHelp(w)
	case "s", "scan":
		msg.OnErr(fsScScan.Parse(subArgs))
		w = do.DistributeArgs(w, fSys, LogfileName, Verbose)
		_, err := com.GetSerialPorts(w)
		return err
	case "ver", "version":
		msg.OnErr(fsScVersion.Parse(subArgs))
		w = do.DistributeArgs(w, fSys, LogfileName, Verbose)
		if Verbose {
			bi, ok := debug.ReadBuildInfo()
			if !ok {
				fmt.Println("buildInfo not ok")
			} else {
				pretty.Println(bi)
			}
		}
		return scVersion(w)
	case "a", "add":
		msg.OnErr(fsScAdd.Parse(subArgs))
		id.CompactSrcs()
		w = do.DistributeArgs(w, fSys, LogfileName, Verbose)
		return id.SubCmdIdAdd(w, fSys)
	case "g", "gen", "generate":
		msg.OnErr(fsScGenerate.Parse(subArgs))
		id.CompactSrcs()
		w = do.DistributeArgs(w, fSys, LogfileName, Verbose)
		return id.SubCmdGenerate(w, fSys)
	case "i", "insert":
		msg.OnErr(fsScInsert.Parse(subArgs))
		id.CompactSrcs()
		err := id.EvaluateIDRangeStrings()
		if err != nil {
			return err
		}
		w = do.DistributeArgs(w, fSys, LogfileName, Verbose)
		return id.SubCmdIdInsert(w, fSys)
	case "c", "clean":
		msg.OnErr(fsScClean.Parse(subArgs))
		id.CompactSrcs()
		w = do.DistributeArgs(w, fSys, LogfileName, Verbose)
		return id.SubCmdIdClean(w, fSys)
	case "sd", "shutdown":
		msg.OnErr(fsScSdSv.Parse(subArgs))
		w = do.DistributeArgs(w, fSys, LogfileName, Verbose)
		return emitter.ScShutdownRemoteDisplayServer(w, 0) // 0|1: 0=no 1=with shutdown timestamp in display server
	case "ds", "displayServer":
		msg.OnErr(fsScSv.Parse(subArgs))
		w = do.DistributeArgs(w, fSys, LogfileName, Verbose)
		return emitter.ScDisplayServer(w) // endless loop
	case "l", "log":
		id.Logging = true
		msg.OnErr(fsScLog.Parse(subArgs))
		decoder.TargetTimeStampUnitPassed = isLogFlagPassed("ts")
		decoder.ShowTargetStamp32Passed = isLogFlagPassed("ts32")
		decoder.ShowTargetStamp16Passed = isLogFlagPassed("ts16")
		decoder.ShowTargetStamp0Passed = isLogFlagPassed("ts0")
		w = do.DistributeArgs(w, fSys, LogfileName, Verbose)
		logLoop(w, fSys) // endless loop
		return nil
	}
}

// https://stackoverflow.com/questions/35809252/check-if-flag-was-provided-in-go
func isLogFlagPassed(name string) bool {
	found := false
	fsScLog.Visit(func(f *flag.Flag) {
		if f.Name == name {
			found = true
		}
	})
	return found
}

type selector struct {
	flag bool
	info func(io.Writer) error
}

// logLoop prepares writing and lut and provides a retry mechanism for unplugged UART.
func logLoop(w io.Writer, fSys *afero.Afero) {
	msg.FatalOnErr(cipher.SetUp(w)) // does nothing when -password is ""
	if decoder.TestTableMode {
		// set switches if they not set already
		// trice l -ts off -prefix " }, ``" -suffix "\n``}," -color off
		if emitter.HostStamp == "LOCmicro" {
			emitter.HostStamp = "off"
		}
		if defaultPrefix == emitter.Prefix {
			emitter.Prefix = " }, `"
		}
		if emitter.Suffix == "" {
			emitter.Suffix = "`},"
		}
		if emitter.ColorPalette == "default" {
			emitter.ColorPalette = "off"
		}
		decoder.TargetStamp32 = "" // todo: justify this line
	}

	var ilu id.TriceIDLookUp
	if id.FnJSON == "emptyFile" { // reserved name for tests only
		ilu = make(id.TriceIDLookUp)
	} else {
		ilu = id.NewLut(w, fSys, id.FnJSON) // lut is a map, that means a pointer
		decoder.IDLUT = ilu
	}
	m := new(sync.RWMutex) // m is a pointer to a read write mutex for lu
	m.Lock()
	ilu.AddFmtCount(w)
	m.Unlock()
	// Just in case the id list file FnJSON gets updated, the file watcher updates lut.
	// This way trice needs NOT to be restarted during development process.
	//go ilu.FileWatcher(w, fSys, m)

	var li id.TriceIDLookUpLI // nil

	if id.LIFnJSON == "emptyFile" { // reserved name for tests only
		li = make(id.TriceIDLookUpLI) // li is not nil, but empty
	} else {
		if _, err := fSys.Stat(id.LIFnJSON); errors.Is(err, os.ErrNotExist) {
			if id.LIFnJSON != "off" && id.LIFnJSON != "none" && id.LIFnJSON != "no" {
				if Verbose {
					fmt.Fprintf(w, "path/to/ %s does not exist: li is nil\n", id.LIFnJSON)
				}
			}
		} else {
			li = id.NewLutLI(w, fSys, id.LIFnJSON) // lut is a map, that means a pointer
			decoder.LILUT = li
			// Just in case the id location information file LIFnJSON gets updated, the file watcher updates li.
			// This way trice needs NOT to be restarted during development process.
			//go li.FileWatcher(w, fSys)
		}
	}

	sw := emitter.New(w)
	var interrupted bool
	var counter int

	for {
		rwc, e := receiver.NewReadWriteCloser(w, fSys, Verbose, receiver.Port, receiver.PortArguments)
		if e != nil {
			fmt.Fprintln(w, e)
			if !interrupted {
				return // hopeless
			}
			time.Sleep(1000 * time.Millisecond) // retry interval
			fmt.Fprintf(w, "\rsig:(re-)setup input port...%d", counter)
			counter++
			continue
		}
		defer func() { msg.OnErr(rwc.Close()) }()
		interrupted = true
		if receiver.ShowInputBytes {
			rwc = receiver.NewBytesViewer(w, rwc)
		}
		if receiver.BinaryLogfileName != "off" && receiver.BinaryLogfileName != "none" {
			rwc = receiver.NewBinaryLogger(w, fSys, rwc)
		}
		e = translator.Translate(w, sw, ilu, m, li, rwc)
		if io.EOF == e {
			return // end of predefined buffer
		}
	}
}

// scVersion is sub-command 'version'. It prints version information.
func scVersion(w io.Writer) error {
	if Verbose {
		fmt.Fprintln(w, "https://github.com/rokath/trice")
		emitter.ShowAllColors()
	}
	if Version == "" {
		fmt.Fprintf(w, "version=devel, built %s\n", Date)
	} else {
		fmt.Fprintf(w, "version=%v, commit=%v, built at %v\n", Version, Commit, Date)
	}
	return nil
}
