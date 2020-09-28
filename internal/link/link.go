// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package link reads from SeggerRTT with the SEGGER app JLinkRTTLogger.
//
// It provides a ReadCloser interface and makes no assumptiona about the delivered data.
// It is also agnostic concerning the RTT channel and other setup parameters.
package link

import (
	"fmt"
	"io/ioutil"
	"log"
	"os"
	"os/exec"
	"strings"

	"github.com/fsnotify/fsnotify"
	"github.com/pkg/errors"
)

var (
	// Verbose gives mor information on output if set. The value is injected from main packages.
	Verbose bool

	// Port is the LINK used.
	Port string

	// Args contails the command line parameters for JLinkRTTLogger
	Args string
)

// Device is the RTT logger reader interface.
type Device struct {
	Exec              string    // linkBinary is the RTT logger executable .
	Lib               string    // linkDynLib is the RTT used dynamic library name.
	cmd               *exec.Cmd // link command handle
	tempLogFileName   string
	tempLogFileHandle *os.File
	Err               error
	Done              chan bool
}

// SetLinkCommandName evaluates port and sets the appropriate link command name
func (p *Device) SetLinkCommandName() {
	switch Port {
	case "JLINK":
		p.Exec = "JLinkRTTLogger"
		p.Lib = "JLinkARM"
	case "STLINK":
		p.Exec = "stRttLogger"
		p.Lib = "libusb-1.0"
	}
	if Verbose {
		fmt.Println("LINK executable", p.Exec, "and dynamic lib", p.Lib, "expected to be in path for usage.")
	}
}

// NewDevice creates an instance of RTT ReadCloser of type Port.
// The Args string is used as parameter string. See SEGGER UM08001_JLink.pdf for details.
func NewDevice() *Device {
	p := &Device{} // create link instance
	p.SetLinkCommandName()

	// get a temporary file name
	p.tempLogFileHandle, _ = ioutil.TempFile(os.TempDir(), "trice-*.bin") // opens for read and write
	p.tempLogFileName = p.tempLogFileHandle.Name()
	p.tempLogFileHandle.Close()
	return p
}

// ErrorFatal ends in osExit(1) if p.Err not nil.
func (p *Device) ErrorFatal() {
	if nil == p.Err {
		return
	}
	log.Panic("linkCmd =", p.Exec, "linkLib =", p.Exec, " <--- PATH ok?")
}

// Read() is part of the exported interface io.ReadCloser. It reads a slice of bytes.
func (p *Device) Read(b []byte) (int, error) {
	return p.tempLogFileHandle.Read(b)
}

// Close is part of the exported interface io.ReadCloser. It ends the connection.
func (p *Device) Close() error {
	if Verbose {
		fmt.Println("Closing link device.")
	}
	// CTRL-C sends SIGTERM also to the started command. It closes the temporary file and terminates itself.
	// Todo: If trice is terminated not with CTRL-C kill automatically.
	//p.Err = errors.Wrap(p.Err, p.cmd.Process.Kill().Error())
	//p.Err = errors.Wrap(p.Err, p.tempLogFileHandle.Close().Error())
	p.Err = errors.Wrap(p.Err, os.Remove(p.tempLogFileName).Error())
	return p.Err
}

// Open starts the RTT logger command with a temporary logfile.
// The temporary logfile is opened for reading.
func (p *Device) Open() error {
	if Verbose {
		fmt.Println("Start a process:", p.Exec, "with needed lib", p.Lib, "and args:", Args, p.tempLogFileName)
	}
	args := strings.Split(Args, " ")
	args = append(args, p.tempLogFileName) // Todo: check if slice could be passed directly.
	switch len(args) {
	case 0:
		p.cmd = exec.Command(p.Exec)
	case 1:
		p.cmd = exec.Command(p.Exec, args[0])
	case 2:
		p.cmd = exec.Command(p.Exec, args[0], args[1])
	case 3:
		p.cmd = exec.Command(p.Exec, args[0], args[1], args[2])
	case 4:
		p.cmd = exec.Command(p.Exec, args[0], args[1], args[2], args[3])
	case 5:
		p.cmd = exec.Command(p.Exec, args[0], args[1], args[2], args[3], args[4])
	case 6:
		p.cmd = exec.Command(p.Exec, args[0], args[1], args[2], args[3], args[4], args[5])
	case 7:
		p.cmd = exec.Command(p.Exec, args[0], args[1], args[2], args[3], args[4], args[5], args[6])
	case 8:
		p.cmd = exec.Command(p.Exec, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7])
	case 9:
		p.cmd = exec.Command(p.Exec, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8])
	case 10:
		p.cmd = exec.Command(p.Exec, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9])
	case 11:
		p.cmd = exec.Command(p.Exec, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9], args[10])
	case 12:
		p.cmd = exec.Command(p.Exec, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9], args[10], args[11])
	case 13:
		p.cmd = exec.Command(p.Exec, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9], args[10], args[11], args[12])
	case 14:
		p.cmd = exec.Command(p.Exec, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9], args[10], args[11], args[12], args[13])
	case 15:
		p.cmd = exec.Command(p.Exec, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9], args[10], args[11], args[12], args[13], args[14])
	case 16:
		p.cmd = exec.Command(p.Exec, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9], args[10], args[11], args[12], args[13], args[14], args[15])
	default:
		p.Err = fmt.Errorf("Cannot handle \"%s\"as parameter string - too much separators: %v", Args, args)
	}

	if Verbose {
		p.cmd.Stdout = os.Stdout
		p.cmd.Stderr = os.Stderr
	}
	p.Err = p.cmd.Start()
	p.ErrorFatal()

	p.tempLogFileHandle, p.Err = os.Open(p.tempLogFileName) // Open() opens a file with read only flag.
	p.ErrorFatal()

	//p.watchLogfile() // todo: make it working well
	if Verbose {
		fmt.Println("trice is watching and reading from", p.tempLogFileName)
	}
	return nil
}

// watchLogfile creates a new file watcher.
func (p *Device) watchLogfile() {
	var watcher *fsnotify.Watcher
	watcher, p.Err = fsnotify.NewWatcher()
	defer watcher.Close()

	go func() {
		for {
			var ok bool
			var event fsnotify.Event
			p.ErrorFatal()
			select {
			case event, ok = <-watcher.Events: // watch for events
				if !ok {
					continue //return
				}
				fmt.Printf("EVENT! %#v\n", event)
				if event.Op&fsnotify.Write == fsnotify.Write {
					log.Println("modified file:", event.Name)
				}
			case p.Err, ok = <-watcher.Errors: // watch for errors
				if !ok {
					continue // return
				}
			}
		}
	}()
	// out of the box fsnotify can watch a single file, or a single directory
	p.Err = watcher.Add(p.tempLogFileName)
}
