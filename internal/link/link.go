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
	"runtime"

	"github.com/fsnotify/fsnotify"
)

var (
	// Verbose gives mor information on output if set. The value is injected from main packages.
	Verbose bool

	// Port is the LINK used.
	Port string

	// Args contails the command line parameters for JLinkRTTLogger
	Args string

	// linkBinary is the JLinkRTTLogger executable .
	linkCmd string

	// linkDynLib is the JLinkRTTLogger used dynamic library name.
	linkLib string

	// shell is the os specific calling environment.
	//shell string

	// linkCmdLine is the os specific LINK commandline.
	//linkCmdLine string

	// link command handle
	cmd *exec.Cmd
)

func setEnvironment() {
	if "windows" != runtime.GOOS && "linux" != runtime.GOOS {
		fmt.Println("trice needs windows or linux as operating system.")
	}
	switch Port {
	case "JLINK":
		if runtime.GOOS == "windows" {
			linkCmd = "JLinkRTTLogger.exe"
			linkLib = "JLinkARM.dll"
			//shell = "cmd"
			//linkCmdLine = "/c "
		} else if runtime.GOOS == "linux" {
			linkCmd = "JLinkRTTLogger"
			linkLib = "JLinkARM.so"
			//shell = "gnome-terminal" // this only works for gnome based linux desktop env
			//linkCmdLine = "-- /bin/bash -c "
		}
	case "STLINK":
		if runtime.GOOS == "windows" {
			linkCmd = "rttLogger.exe"
			linkLib = "libusb-1.0.dll"
			//shell = "cmd"
			//linkCmdLine = "/c "
		} else if runtime.GOOS == "linux" {
			linkCmd = "rttLogger"
			linkLib = "libusb-1.0.so"
			//shell = "gnome-terminal" // this only works for gnome based linux desktop env
			//linkCmdLine = "-- /bin/bash -c "
		}
	}
	if Verbose {
		fmt.Println("LINK executable", linkCmd, "and lib", linkLib, "expected to be in path for usage.")
	}

}

// LINK is the Segger RealTime Transfer logger reader interface.
type LINK struct {
	tempLogFileName   string
	tempLogFileHandle *os.File
	Err               error
}

/*
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
*/

// NewReadCloser creates an instance of RTT ReadCloser.
//
// It is intended to be used by receiver.New() which embeds its interface.
// The param string is used as JLinkRTTLogger parameter string. See SEGGER UM08001_JLink.pdf for details.
func NewReadCloser(param string) *LINK {
	p := &LINK{} // create link instance

	setEnvironment()
	// check environment
	path, err := exec.LookPath(linkCmd)
	if nil == err {
		if Verbose {
			fmt.Println(path, "found")
		}
	} else {
		fmt.Println(linkCmd, "not found")
		return nil
	}

	// get a temporary file name
	p.tempLogFileHandle, _ = ioutil.TempFile(os.TempDir(), "trice-*.bin") // opens for read and write
	p.tempLogFileName = p.tempLogFileHandle.Name()
	p.tempLogFileHandle.Close()
	//linkCmdLine += linkCmd + " " + Args + " " + r.tempLogFileName // full parameter string

	return p
}

// ErrorFatal ends in osExit(1) if p.Err not nil.
func (p *LINK) ErrorFatal() {
	if nil == p.Err {
		return
	}
	log.Panic("linkCmd =", linkCmd, "linkLib =", linkLib, " <--- PATH ok?")
}

// Read() is part of the exported interface io.ReadCloser. It reads a slice of bytes.
func (p *LINK) Read(b []byte) (int, error) {
	return p.tempLogFileHandle.Read(b)
}

// Close is part of the exported interface io.ReadCloser. It ends the connection.
//
// See https://stackoverflow.com/questions/11886531/terminating-a-process-started-with-os-exec-in-golang
func (p *LINK) Close() error {
	p.Err = p.tempLogFileHandle.Close()
	p.ErrorFatal()
	return os.Remove(p.tempLogFileName) // clean up
}

// Open starts the JLinkRTTLogger command with a temporary logfile
//
// The temporary logfile is opened for reading.
func (p *LINK) Open() error {
	if Verbose {
		fmt.Println("Start a process:", linkCmd, "with needed lib", linkLib, "and args:", Args, p.tempLogFileName)
	}
	//cmd = exec.Command(linkCmd, Args, p.tempLogFileName)
	cmd = exec.Command(linkCmd, "-Device", "STM32F070RB", "-if", "SWD", "-Speed", "4000", "-RTTChannel", "0", p.tempLogFileName)

	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr

	p.Err = cmd.Start()
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
func (p *LINK) watchLogfile() {
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

/*
func xxxmain() {
	watcher, err := fsnotify.NewWatcher()
	if err != nil {
		log.Fatal(err)
	}
	defer watcher.Close()

	done := make(chan bool)
	go func() {
		for {
			select {
			case event, ok := <-watcher.Events:
				if !ok {
					return
				}
				log.Println("event:", event)
				if event.Op&fsnotify.Write == fsnotify.Write {
					log.Println("modified file:", event.Name)
				}
			case err, ok := <-watcher.Errors:
				if !ok {
					return
				}
				log.Println("error:", err)
			}
		}
	}()

	err = watcher.Add("/tmp/foo")
	if err != nil {
		log.Fatal(err)
	}
	<-done
}
*/
