// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package args implements the commandline interface and calls the appropriate commands.
package args

import (
	"errors"
	"fmt"
	"io"
	"log"
	"net"
	"os"
	"sync"
	"time"

	"github.com/rokath/trice/internal/com"
	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/internal/link"
	"github.com/rokath/trice/internal/receiver"
	"github.com/rokath/trice/pkg/cipher"
	"github.com/rokath/trice/pkg/msg"
)

// Handler is called in main, evaluates args and calls the appropriate functions.
// It returns for program exit.
func Handler(args []string) error {

	id.FnJSON = id.ConditionalFilePath(id.FnJSON)

	if Date == "" { // goreleaser will set Date, otherwise use file info.
		fi, err := os.Stat(os.Args[0])
		if nil == err { // On running main tests file-info is invalid, so do not use in that case.
			Date = fi.ModTime().String()
		}
	}

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
		w := distributeArgs()
		return scHelp(w)
	case "s", "scan":
		msg.OnErr(fsScScan.Parse(subArgs))
		w := distributeArgs()
		_, err := com.GetSerialPorts(w)
		return err
	case "ver", "version":
		msg.OnErr(fsScVersion.Parse(subArgs))
		w := distributeArgs()
		return scVersion(w)
	case "renew":
		msg.OnErr(fsScRenew.Parse(subArgs))
		w := distributeArgs()
		return id.SubCmdReNewList(w)
	case "r", "refresh":
		msg.OnErr(fsScRefresh.Parse(subArgs))
		w := distributeArgs()
		return id.SubCmdRefreshList(w)
	case "u", "update":
		msg.OnErr(fsScUpdate.Parse(subArgs))
		w := distributeArgs()
		return id.SubCmdUpdate(w)
	case "zeroSourceTreeIds":
		msg.OnErr(fsScZero.Parse(subArgs))
		w := distributeArgs()
		return id.ScZero(w, *pSrcZ, fsScZero)
	case "sd", "shutdown":
		msg.OnErr(fsScSdSv.Parse(subArgs))
		w := distributeArgs()
		return emitter.ScShutdownRemoteDisplayServer(w, 0) // 0|1: 0=no 1=with shutdown timestamp in display server
	case "ds", "displayServer":
		msg.OnErr(fsScSv.Parse(subArgs))
		w := distributeArgs()
		return emitter.ScDisplayServer(w) // endless loop
	case "l", "log":
		msg.OnErr(fsScLog.Parse(subArgs))
		w := distributeArgs()
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
	msg.FatalOnErr(cipher.SetUp(w)) // does nothing when -password is ""
	if decoder.TestTableMode {
		// set switches if they not set already
		// trice l -ts off -prefix " }, ``" -suffix "\n``}," -color off
		if emitter.TimestampFormat == "LOCmicro" {
			emitter.TimestampFormat = "off"
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
		decoder.ShowTargetTimestamp = "" // todo: justify this line
	}

	var lu id.TriceIDLookUp
	if id.FnJSON == "emptyFile" { // reserved name for tests only
		lu = make(id.TriceIDLookUp)
	} else {
		lu = id.NewLut(w, id.FnJSON) // lut is a map, that means a pointer
	}
	m := new(sync.RWMutex) // m is a pointer to a read write mutex for lu
	m.Lock()
	lu.AddFmtCount(w)
	m.Unlock()
	// Just in case the id list file FnJSON gets updated, the file watcher updates lut.
	// This way trice needs NOT to be restarted during development process.
	go lu.FileWatcher(w, m)

	sw := emitter.New(w)
	var interrupted bool
	var counter int

	for {
		rc, e := receiver.NewReadCloser(w, verbose, receiver.Port, receiver.PortArguments)
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
		defer func() { msg.OnErr(rc.Close()) }()
		interrupted = true
		if receiver.ShowInputBytes {
			rc = receiver.NewBytesViewer(w, rc)
		}
		e = decoder.Translate(w, sw, lu, m, rc)
		if io.EOF == e {
			return // end of predefined buffer
		}
	}
}

// scVersion is sub-command 'version'. It prints version information.
func scVersion(w io.Writer) error {
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

var TriceOutWriter io.Writer

// distributeArgs is distributing values used in several packages.
// It must not be called before the appropriate arg parsing.
func distributeArgs() io.Writer {

	id.Verbose = verbose
	link.Verbose = verbose
	decoder.Verbose = verbose
	emitter.Verbose = verbose
	receiver.Verbose = verbose
	emitter.TestTableMode = decoder.TestTableMode

	TriceOutWriter = os.Stdout
	w := triceOutput(LogfileName)
	evaluateColorPalette(w)
	return w
}

// triceOutput returns a io.Writer to log to. If fn is given the returned io.Writer write a copy into the given file.
func triceOutput(fn string) io.Writer {
	// start logging only if fn not "none" or "off"
	if fn == "none" || fn == "off" {
		if verbose {
			fmt.Println("No logfile writing...")
		}
		return os.Stdout
	}

	// defaultLogfileName is the pattern for default logfile name. The timestamp is replaced with the actual time.
	defaultLogfileName := "2006-01-02_1504-05_trice.log"
	if fn == "auto" {
		fn = defaultLogfileName
	}
	// open logfile
	if fn == defaultLogfileName {
		fn = time.Now().Format(fn) // Replace timestamp in default log filename.
	} // Otherwise, use cli defined log filename.

	lfHandle, err := os.OpenFile(fn, os.O_RDWR|os.O_CREATE|os.O_APPEND, 0666)
	msg.FatalOnErr(err)
	if verbose {
		fmt.Printf("Writing to logfile %s...\n", fn)
	}

	w := io.MultiWriter(TriceOutWriter, lfHandle)
	return w
}

func CreateTCP4Server(addr string) {
	// The net.Listen() function makes the program a TCP server. This functions returns a Listener variable, which is a generic network listener for stream-oriented protocols.
	l, err := net.Listen("tcp", addr)
	if err != nil {
		log.Fatal(err)
	}
	defer l.Close()
	for { // wait for a connection
		// t is only after a successful call to Accept() that the TCP server can begin to interact with TCP clients.
		conn, err := l.Accept()
		if err != nil {
			fmt.Println(err)
			continue // try again
		}
		// a new connection exist, add it to the write list
		TriceOutWriter = io.MultiWriter(TriceOutWriter, conn)
		defer conn.Close()
	}
}

/*
// handleIncomingRequest is activated after an external connection
func handleIncomingRequest(conn net.Conn) {
	// store incoming data
	buffer := make([]byte, 1024)
	_, err := conn.Read(buffer)
	if err != nil {
		log.Fatal(err)
	}
	// respond
	time := time.Now().Format("Monday, 02-Jan-06 15:04:05 MST")
	conn.Write([]byte("Hi back!\n"))
	conn.Write([]byte(time))

	// close conn
	conn.Close()
}
*/
