// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package link reads from SeggerRTT with the SEGGER app JLinkRTTLogger or with the open source app stRttLogger.exe.
//
// It provides a ReadCloser interface and makes no assumptiona about the delivered data.
// It is also agnostic concerning the RTT channel and other setup parameters.
package link

import (
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"os"
	"os/exec"
	"strings"

	"github.com/pkg/errors"
	"github.com/rokath/trice/pkg/msg"
)

var (
	// Verbose gives mor information on output if set. The value is injected from main packages.
	Verbose bool
)

// Device is the RTT logger reader interface.
type Device struct {
	w         io.Writer // os.Stdout
	Exec      string    // linkBinary is the RTT logger executable .
	Lib       string    // linkDynLib is the RTT used dynamic library name.
	args      []string  //  contains the command line parameters for JLinkRTTLogger
	arguments string    // needed only for error message

	cmd               *exec.Cmd // link command handle
	tempLogFileName   string
	tempLogFileHandle *os.File
	Err               error
	Done              chan bool
}

// NewDevice creates an instance of RTT ReadCloser of type Port.
// The Args string is used as parameter string. See SEGGER UM08001_JLink.pdf for details.
func NewDevice(w io.Writer, port, arguments string) *Device {
	p := &Device{} // create link instance
	p.w = w
	switch port {
	case "JLINK", "J-LINK":
		p.Exec = "JLinkRTTLogger"
		p.Lib = "JLinkARM"
	case "STLINK", "ST-LINK":
		p.Exec = "stRttLogger"
		p.Lib = "libusb-1.0"
	default:
		log.Panic("Unknown port:", port)
	}
	if Verbose {
		fmt.Fprintln(w, "port:", port, "arguments:", arguments)
		fmt.Fprintln(w, "LINK executable", p.Exec, "and dynamic lib", p.Lib, "expected to be in path for usage.")
	}
	// get a temporary file name
	var e error
	p.tempLogFileHandle, e = ioutil.TempFile(os.TempDir(), "trice-*.bin") // opens for read and write
	msg.OnErr(e)
	p.tempLogFileName = p.tempLogFileHandle.Name()
	msg.OnErr(p.tempLogFileHandle.Close())

	p.arguments = arguments
	p.args = strings.Split(arguments, " ")
	// The -RTTSearchRanges "..." need to be written without "" and with _ instead of space.
	for i := range p.args { // 0x20000000_0x1800 -> 0x20000000 0x1800
		p.args[i] = strings.ReplaceAll(p.args[i], "_0x", " 0x")
	}
	p.args = append(p.args, p.tempLogFileName) // to do: check if slice could be passed directly.
	return p
}

// ErrorFatal ends in osExit(1) if p.Err not nil.
func (p *Device) ErrorFatal() {
	if nil == p.Err {
		return
	}
	log.Panic(p.Err, ": linkCmd =", p.Exec, "linkLib =", p.Lib, " <--- PATH ok? error:")
}

// Read is part of the exported interface io.ReadCloser. It reads a slice of bytes.
func (p *Device) Read(b []byte) (int, error) {
	return p.tempLogFileHandle.Read(b)
}

// Close is part of the exported interface io.ReadCloser. It ends the connection.
func (p *Device) Close() error {
	if Verbose {
		fmt.Fprintln(p.w, "Closing link device.")
	}
	// CTRL-C sends SIGTERM also to the started command. It closes the temporary file and terminates itself.
	// Todo: If trice is terminated not with CTRL-C kill automatically.
	// p.Err = errors.Wrap(p.Err, p.cmd.Process.Kill().Error())
	// p.Err = errors.Wrap(p.Err, p.tempLogFileHandle.Close().Error())
	p.Err = errors.Wrap(p.Err, os.Remove(p.tempLogFileName).Error())
	return p.Err
}

// Open starts the RTT logger command with a temporary logfile.
// The temporary logfile is opened for reading.
func (p *Device) Open() error {
	if Verbose {
		fmt.Fprintln(p.w, "Start a process:", p.Exec, "with needed lib", p.Lib, "and args:")
		for i, a := range p.args {
			fmt.Fprintln(p.w, i, a)
		}
	}
	p.cmd = exec.Command(p.Exec, p.args...)

	if Verbose {
		p.cmd.Stdout = os.Stdout
		p.cmd.Stderr = os.Stderr
	}
	p.Err = p.cmd.Start()
	p.ErrorFatal()

	p.tempLogFileHandle, p.Err = os.Open(p.tempLogFileName) // Open() opens a file with read only flag.
	p.ErrorFatal()

	// p.watchLogfile() // todo: make it working well
	if Verbose {
		fmt.Fprintln(p.w, "trice is watching and reading from", p.tempLogFileName)
	}
	return nil
}

// watchLogfile creates a new file watcher.
//  func (p *Device) watchLogfile() {
//  	var watcher *fsnotify.Watcher
//  	watcher, p.Err = fsnotify.NewWatcher()
//  	defer func() { msg.OnErr(watcher.Close()) }()
//
//  	go func() {
//  		for {
//  			var ok bool
//  			var event fsnotify.Event
//  			p.ErrorFatal()
//  			select {
//  			case event, ok = <-watcher.Events: // watch for events
//  				if !ok {
//  					continue // return
//  				}
//  				fmt.Printf("EVENT! %#v\n", event)
//  				if event.Op&fsnotify.Write == fsnotify.Write {
//  					log.Println("modified file:", event.Name)
//  				}
//  			case p.Err, ok = <-watcher.Errors: // watch for errors
//  				if !ok {
//  					continue // return
//  				}
//  			}
//  		}
//  	}()
//  	// out of the box fsnotify can watch a single file, or a single directory
//  	p.Err = watcher.Add(p.tempLogFileName)
//  }
