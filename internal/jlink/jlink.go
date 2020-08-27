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
	"os"
	"os/exec"
	"runtime"

	"github.com/rokath/trice/internal/global"
)

var (
	// Verbose gives mor information on output if set. The value is injected from main packages.
	Verbose bool

	// Param contails the command line parameters for JLinkRTTLogger
	Param string

	// exe is the JLinkRTTLogger executable name
	jcmd string

	// dynLib is the JLinkRTTLogger used dynamic library name
	dynLib string

	// shell is the os specific calling environment
	shell string

	// clip is the os specific calling environment comandline beginning
	clip string

	// jlink command handle
	lcmdH *exec.Cmd
)

func init() {
	if runtime.GOOS == "windows" {
		jcmd = "JLinkRTTLogger.exe"
		dynLib = "JLinkARM.dll"
		shell = "cmd"
		clip = "/c "
	} else if runtime.GOOS == "linux" {
		jcmd = "JLinkRTTLogger"
		dynLib = "JLinkARM.so"
		shell = "gnome-terminal" // this only works for gnome based linux desktop env
		clip = "-- /bin/bash -c "
	} else {
		if Verbose {
			fmt.Println("trice is running on unknown operating system, '-source JLINK' will not work.")
		}
	}
	if Verbose {
		fmt.Println(shell, jcmd, "JLINK executable expected to be in path for usage")
	}
}

// JLINK is the Segger RealTime Transfer logger reader interface.
type JLINK struct {
	tlfN string   // tempLogFile name
	tlfH *os.File // tempLogFile handle
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
	path, err := exec.LookPath(jcmd)
	if nil == err {
		if Verbose {
			fmt.Println(path, "found")
		}
	} else {
		fmt.Println(jcmd, "not found")
		return nil
	}

	// get a temporary file name
	r.tlfH, _ = ioutil.TempFile(os.TempDir(), "trice-*.bin") // opens for read and write
	r.tlfN = r.tlfH.Name()
	r.tlfH.Close()
	clip += jcmd + " " + param + " " + r.tlfN // full parameter string

	return r
}

// Read() is part of the exported interface io.ReadCloser. It reads a slice of bytes.
func (p *JLINK) Read(b []byte) (int, error) {
	return p.tlfH.Read(b)
}

// Close is part of the exported interface io.ReadCloser. It ends the connection.
//
// See https://stackoverflow.com/questions/11886531/terminating-a-process-started-with-os-exec-in-golang
func (p *JLINK) Close() error {
	var err error
	return err
}

// Open starts the JLinkRTTLogger command with a temporary logfile
//
// THe temporary logfile is opened for reading.
func (p *JLINK) Open() error {
	var err error
	if Verbose {
		fmt.Println("Start a process:", shell, clip)
	}
	lcmdH = exec.Command(shell, clip)
	if err = lcmdH.Start(); err != nil {
		//		log.Fatal("start error", err)
		global.Check(err)
	}

	p.tlfH, err = os.Open(p.tlfN) // Open() opens a file with read only flag.
	if nil != err {
		fmt.Println(err)
		return err
	}
	if Verbose {
		fmt.Println("trice is reading from", p.tlfN)
	}
	return nil
}
