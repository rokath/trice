// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package link reads from SeggerRTT with the SEGGER app JLinkRTTLogger or with the open source app stRttLogger.exe.
//
// It provides a ReadCloser interface and makes no assumptions about the delivered data.
// It is also agnostic concerning the RTT channel and other setup parameters.
package link

import (
	"fmt"
	"io"
	"log"
	"os"
	"os/exec"
	"path/filepath"
	"strings"
	"time"

	"github.com/pkg/errors"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/pkg/msg"
	"github.com/spf13/afero"
)

var (
	// Verbose gives more information on output if set. The value is injected from main packages.
	Verbose bool
)

// Device is the RTT logger reader interface.
type Device struct {
	w    io.Writer // os.Stdout
	fSys *afero.Afero
	Exec string   // linkBinary is the RTT logger executable .
	Lib  string   // linkDynLib is the RTT used dynamic library name.
	args []string //  contains the command line parameters for JLinkRTTLogger
	//arguments string   // needed only for error message

	cmd               *exec.Cmd // link command handle
	tempLogFileName   string
	tempLogFileHandle afero.File
	Err               error
	Done              chan bool
}

// NewDevice creates an instance of RTT ReadCloser of type Port.
// The Args string is used as parameter string. See SEGGER UM08001_JLink.pdf for details.
func NewDevice(w io.Writer, fSys *afero.Afero, port, arguments string) *Device {
	p := &Device{} // create link instance
	p.fSys = fSys
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
	//p.arguments = arguments
	p.args = strings.Split(arguments, " ")
	// The -RTTSearchRanges "..." need to be written without "" and with _ instead of space.
	for i := range p.args { // 0x20000000_0x1800 -> 0x20000000 0x1800
		p.args[i] = strings.ReplaceAll(p.args[i], "_0x", " 0x")
	}

	lastArg := p.args[len(p.args)-1]
	lastArgExt := filepath.Ext(lastArg)

	if lastArgExt == ".bin" {
		if Verbose {
			fmt.Printf("An intermediate log file name \"%s\" is specified inside p.args, so use that.\n", lastArg)
			p.tempLogFileName = lastArg
		}
	} else {
		// get a temporary file name in a writable folder temp
		dir := filepath.Dir(id.FnJSON) // the id list folder is assumed to be writable and readable

		// create temp folder if not exists
		tempDir := filepath.Join(dir, "temp")
		e := os.MkdirAll(tempDir, os.ModePerm)
		msg.OnErr(e)

		// create a new file
		fh, e := os.CreateTemp(tempDir, "trice-*.bin") // opens for read and write
		msg.OnErr(e)
		p.tempLogFileName = fh.Name() // p.tempLogFileName is trice needed to know where to read from
		msg.OnErr(fh.Close())

		p.args = append(p.args, p.tempLogFileName) // p.tempLogFileName is passed here for JLinkRTTLogger
	}
	return p
}

// errorFatal ends in osExit(1) if p.Err not nil.
func (p *Device) errorFatal() {
	if nil == p.Err {
		return
	}
	fmt.Printf("p.err=%v\n", p.Err)
	fmt.Printf("p.Exec=%s\n", p.Exec)
	fmt.Printf("p.Lib=%s\n", p.Lib)
	fmt.Printf("p.args=%s\n", p.args)
	fmt.Printf("p.tempLogFileName=%s\n", p.tempLogFileName)
	os.Exit(1)
}

// Read is part of the exported interface io.ReadCloser. It reads a slice of bytes.
func (p *Device) Read(b []byte) (int, error) {
	return p.tempLogFileHandle.Read(b)
}

func (p *Device) Write(b []byte) (int, error) {
	return p.tempLogFileHandle.Write(b)
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
	p.Err = errors.Wrap(p.Err, p.fSys.Remove(p.tempLogFileName).Error())
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
		fmt.Println(p.cmd)
		p.cmd.Stdout = os.Stdout
		p.cmd.Stderr = os.Stderr
	}
	p.Err = p.cmd.Start()
	p.errorFatal()

	go func() {
		e := p.cmd.Wait()
		if e != nil {
			fmt.Println(e)
		}
	}()

	// todo: check if file exists in a loop for more speed
	time.Sleep(1000 * time.Millisecond)                         // to be sure, log fie is created
	p.tempLogFileHandle, p.Err = p.fSys.Open(p.tempLogFileName) // Open() opens a file with read only flag.
	p.errorFatal()

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
