// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package args implements the commandline interface and calls the appropriate commands.
package args

import (
	"errors"
	"fmt"
	"io"
	"io/ioutil"
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
	"github.com/rokath/trice/internal/translator"
	"github.com/rokath/trice/pkg/cipher"
	"github.com/rokath/trice/pkg/msg"
)

// Handler is called in main, evaluates args and calls the appropriate functions.
// It returns for program exit.
func Handler(w io.Writer, args []string) error {

	id.FnJSON = id.ConditionalFilePath(id.FnJSON)

	if Date == "" { // goreleaser will set Date, otherwise use file info.
		fi, err := os.Stat(args[0])
		if nil == err { // On running main tests file-info is invalid, so do not use in that case.
			Date = fi.ModTime().String()
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
		w = distributeArgs(w)
		return scHelp(w)
	case "s", "scan":
		msg.OnErr(fsScScan.Parse(subArgs))
		w = distributeArgs(w)
		_, err := com.GetSerialPorts(w)
		return err
	case "ver", "version":
		msg.OnErr(fsScVersion.Parse(subArgs))
		w = distributeArgs(w)
		return scVersion(w)
	case "renew":
		msg.OnErr(fsScRenew.Parse(subArgs))
		w = distributeArgs(w)
		return id.SubCmdReNewList(w)
	case "r", "refresh":
		msg.OnErr(fsScRefresh.Parse(subArgs))
		w = distributeArgs(w)
		return id.SubCmdRefreshList(w)
	case "u", "update":
		msg.OnErr(fsScUpdate.Parse(subArgs))
		w = distributeArgs(w)
		return id.SubCmdUpdate(w)
	case "z", "zeroSourceTreeIds":
		msg.OnErr(fsScZero.Parse(subArgs))
		w = distributeArgs(w)
		//  return id.ScZero(w, *pSrcZ, fsScZero)
		return id.ScZeroMulti(w, fsScZero)
	case "sd", "shutdown":
		msg.OnErr(fsScSdSv.Parse(subArgs))
		w = distributeArgs(w)
		return emitter.ScShutdownRemoteDisplayServer(w, 0) // 0|1: 0=no 1=with shutdown timestamp in display server
	case "ds", "displayServer":
		msg.OnErr(fsScSv.Parse(subArgs))
		w = distributeArgs(w)
		return emitter.ScDisplayServer(w) // endless loop
	case "l", "log":
		msg.OnErr(fsScLog.Parse(subArgs))
		w = distributeArgs(w)
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

	var li id.TriceIDLookUpLI // nil

	if id.LIFnJSON == "emptyFile" { // reserved name for tests only
		li = make(id.TriceIDLookUpLI) // li is not nil, but empty
	} else {
		if _, err := os.Stat(id.LIFnJSON); errors.Is(err, os.ErrNotExist) {
			if id.LIFnJSON != "off" && id.LIFnJSON != "none" && id.LIFnJSON != "no" {
				fmt.Fprintf(w, "path/to/ %s does not exist: li is nil\n", id.LIFnJSON)
			}
		} else {
			li = id.NewLutLI(w, id.LIFnJSON) // lut is a map, that means a pointer

			// Just in case the id location information file LIFnJSON gets updated, the file watcher updates li.
			// This way trice needs NOT to be restarted during development process.
			go li.FileWatcher(w)
		}
	}

	sw := emitter.New(w)
	var interrupted bool
	var counter int

	for {
		rwc, e := receiver.NewReadWriteCloser(w, verbose, receiver.Port, receiver.PortArguments)
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
			rwc = receiver.NewBinaryLogger(w, rwc)
		}
		e = translator.Translate(w, sw, lu, m, li, rwc)
		if io.EOF == e {
			return // end of predefined buffer
		}
	}
}

// scVersion is sub-command 'version'. It prints version information.
func scVersion(w io.Writer) error {
	if verbose {
		fmt.Fprintln(w, "https://github.com/rokath/trice")
		//emitter.ShowAllColors()
	}
	if Version == "" {
		fmt.Fprintf(w, "version=devel, built %s\n", Date)
	} else {
		fmt.Fprintf(w, "version=%v, commit=%v, built at %v\n", Version, Commit, Date)
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

// distributeArgs is distributing values used in several packages.
// It must not be called before the appropriate arg parsing.
func distributeArgs(w io.Writer) io.Writer {

	id.Verbose = verbose
	link.Verbose = verbose
	decoder.Verbose = verbose
	emitter.Verbose = verbose
	receiver.Verbose = verbose
	translator.Verbose = verbose
	emitter.TestTableMode = decoder.TestTableMode

	w = triceOutput(w, LogfileName)
	evaluateColorPalette(w)
	return w
}

// triceOutput returns w as a a optional combined io.Writer. If fileName is given the returned io.Writer write a copy into the given file.
func triceOutput(w io.Writer, fileName string) io.Writer {
	tcpWriter := TCPWriter()

	// start logging only if fn not "none" or "off"
	if fileName == "none" || fileName == "off" {
		if verbose {
			fmt.Println("No logfile writing...")
		}
		return io.MultiWriter(w, tcpWriter)
	}

	// defaultLogfileName is the pattern for default logfile name. The timestamp is replaced with the actual time.
	defaultLogfileName := "2006-01-02_1504-05_trice.log"
	if fileName == "auto" {
		fileName = defaultLogfileName
	}
	// open logfile
	if fileName == defaultLogfileName {
		fileName = time.Now().Format(fileName) // Replace timestamp in default log filename.
	} // Otherwise, use cli defined log filename.

	lfHandle, err := os.OpenFile(fileName, os.O_RDWR|os.O_CREATE|os.O_APPEND, 0666)
	msg.FatalOnErr(err)
	if verbose {
		fmt.Printf("Writing to logfile %s...\n", fileName)
	}

	return io.MultiWriter(w, tcpWriter, lfHandle)
}

var TCPOutAddr = ""

func TCPWriter() io.Writer {
	if TCPOutAddr == "" {
		return ioutil.Discard
	}
	// The net.Listen() function makes the program a TCP server. This functions returns a Listener variable, which is a generic network listener for stream-oriented protocols.
	fmt.Println("Listening on " + TCPOutAddr + "...")
	listen, err := net.Listen("tcp", TCPOutAddr)
	if err != nil {
		log.Fatal(err)
	}
	defer listen.Close()

	// t is only after a successful call to Accept() that the TCP server can begin to interact with TCP clients.
	TCPConn, err := listen.Accept()
	fmt.Println("Accepting connection:", TCPConn)
	if err != nil {
		log.Fatal(err)
	}
	// Make a buffer to hold incoming data.
	buf := make([]byte, 1024)
	reqLen, err := TCPConn.Read(buf)
	if err != nil {
		fmt.Println("Error reading:", err.Error())
	}
	fmt.Println(string(buf[:reqLen]))
	TCPConn.Write([]byte("Trice connected...\r\n"))

	//defer TCPConn.Close()
	return TCPConn

}
