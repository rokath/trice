// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package jlink reads from SeggerRTT with the SEGGER app JLinkRTTLogger.
//
// It provides a ReadCloser interface and makes no assumptiona about the delivered data.
// It is also agnostic concerning the RTT channel and other setup parameters.
package jlink

import (
	"fmt"
	"io/ioutil"
	"log"
	"os"
	"os/exec"
	"runtime"

	"github.com/fsnotify/fsnotify"
)

var (
	// Verbose gives mor information on output if set. The value is injected from main packages.
	Verbose bool

	// Param contails the command line parameters for JLinkRTTLogger
	Param string

	// jlinkBinary is the JLinkRTTLogger executable .
	jlinkBinary string

	// jlinkDynLib is the JLinkRTTLogger used dynamic library name.
	jlinkDynLib string

	// shell is the os specific calling environment.
	shell string

	// jlinkCmdLine is the os specific JLINK commandline.
	jlinkCmdLine string

	// jlink command handle
	lcmdH *exec.Cmd
)

func init() {
	if runtime.GOOS == "windows" {
		jlinkBinary = "JLinkRTTLogger.exe"
		jlinkDynLib = "JLinkARM.dll"
		shell = "cmd"
		jlinkCmdLine = "/c "
	} else if runtime.GOOS == "linux" {
		jlinkBinary = "JLinkRTTLogger"
		jlinkDynLib = "JLinkARM.so"
		shell = "gnome-terminal" // this only works for gnome based linux desktop env
		jlinkCmdLine = "-- /bin/bash -c "
	} else {
		if Verbose {
			fmt.Println("trice is running on unknown operating system, '-source JLINK' will not work.")
		}
	}
	if Verbose {
		fmt.Println(shell, jlinkBinary, "JLINK executable expected to be in path for usage")
	}
}

// JLINK is the Segger RealTime Transfer logger reader interface.
type JLINK struct {
	tempLogFileName   string
	tempLogFileHandle *os.File
	Err               error
}

// exists returns whether the given file or directory exists
// https://stackoverflow.com/questions/10510691/how-to-check-whether-a-file-or-directory-exists
func exists(path string) (bool, error) {
	_, err := os.Stat(path)
	if err == nil {
		return true, nil
	}
	if os.IsNotExist(err) {
		return false, nil
	}
	return false, err
}

// New creates an instance of RTT ReadCloser.
//
// It is intended to be used by receiver.New() which embeds its interface.
// The param string is used as JLinkRTTLogger parameter string. See SEGGER UM08001_JLink.pdf for details.
func New(param string) *JLINK {
	r := &JLINK{} // create SeggerRTT instance

	// check environment
	path, err := exec.LookPath(jlinkBinary)
	if nil == err {
		if Verbose {
			fmt.Println(path, "found")
		}
	} else {
		fmt.Println(jlinkBinary, "not found")
		return nil
	}

	// get a temporary file name
	r.tempLogFileHandle, _ = ioutil.TempFile(os.TempDir(), "trice-*.bin") // opens for read and write
	r.tempLogFileName = r.tempLogFileHandle.Name()
	r.tempLogFileHandle.Close()
	jlinkCmdLine += jlinkBinary + " " + param + " " + r.tempLogFileName // full parameter string

	return r
}

// ErrorFatal ends in osExit(1) if p.Err not nil.
func (p *JLINK) ErrorFatal() {
	if nil == p.Err {
		return
	}
	log.Panic("jlinkCmdLine =", jlinkCmdLine, "jlinkDynLib =", jlinkDynLib, "PATH ok?")
}

// Read() is part of the exported interface io.ReadCloser. It reads a slice of bytes.
func (p *JLINK) Read(b []byte) (int, error) {
	return p.tempLogFileHandle.Read(b)
}

// Close is part of the exported interface io.ReadCloser. It ends the connection.
//
// See https://stackoverflow.com/questions/11886531/terminating-a-process-started-with-os-exec-in-golang
func (p *JLINK) Close() error {
	p.Err = p.tempLogFileHandle.Close()
	p.ErrorFatal()
	return os.Remove(p.tempLogFileName) // clean up
}

// Open starts the JLinkRTTLogger command with a temporary logfile
//
// The temporary logfile is opened for reading.
func (p *JLINK) Open() error {
	if Verbose {
		fmt.Println("Start a process:", shell, jlinkCmdLine)
	}
	lcmdH = exec.Command(shell, jlinkCmdLine)
	p.Err = lcmdH.Start()
	p.ErrorFatal()

	p.tempLogFileHandle, p.Err = os.Open(p.tempLogFileName) // Open() opens a file with read only flag.
	p.ErrorFatal()

	p.watchLogfile()
	if Verbose {
		fmt.Println("trice is watching and reading from", p.tempLogFileName)
	}
	return nil
}

// watchLogfile creates a new file watcher.
func (p *JLINK) watchLogfile() {
	var watcher *fsnotify.Watcher
	watcher, p.Err = fsnotify.NewWatcher()
	defer watcher.Close()
	go func() {
		for {
			p.ErrorFatal()
			select {
			case event := <-watcher.Events: // watch for events
				fmt.Printf("EVENT! %#v\n", event)

			case p.Err = <-watcher.Errors: // watch for errors
				fmt.Print("E")
			}
		}
	}()
	// out of the box fsnotify can watch a single file, or a single directory
	p.Err = watcher.Add("xxxx")
}
