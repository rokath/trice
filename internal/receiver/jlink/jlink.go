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
	"path/filepath"
	"runtime"

	"github.com/rokath/trice/internal/global"
	//"time"
)

var (
	// Param contails the command line parameters for JLinkRTTLogger
	Param string
)

// RTTL is the Segger RealTime Transfer logger reader interface.
type RTTL struct {
	tlfN string   // tempLogFile name
	tlfH *os.File // tempLogFile handle

	lcmdN string    // jlink command name
	lcmdH *exec.Cmd // jlink command handle

	jlinkEx  string // name of JLinkRTTLogger executable
	jlinkLib string // name of JLinkRTTLogger dynamic library

	shell string // os calling environment
	clip  string // full shell parameter string including JLinkRTTLogger and its parameters.
}

//var pointerToInstance *RTTL

// New creates an instance of RTT ReadCloser.
//
// It is intended to be used by receiver.New() which embeds its interface.
// The param string is used as JLinkRTTLogger parameter string. See SEGGER UM08001_JLink.pdf for details.
func New(param string) *RTTL {
	r := &RTTL{} // create SeggerRTT instance
	//pointerToInstance = r // for cleanup

	// get a temporary file name
	r.tlfH, _ = ioutil.TempFile(os.TempDir(), "trice-*.bin") // opens for read and write
	r.tlfN = r.tlfH.Name()
	r.tlfH.Close()
	r.clip = " " + param + " " + r.tlfN // full parameter string

	// get path of trice command, because JLinkRTTLogger exewcutable and library are expected there
	ex, err := os.Executable()
	if err != nil {
		panic(err)
	}
	exPath := filepath.Dir(ex)

	if runtime.GOOS == "windows" {
		r.jlinkEx = exPath + "\\JLinkRTTLogger.exe"
		r.jlinkLib = exPath + "\\JLinkARM.dll"
		r.shell = "cmd"
		r.clip = "/c " + r.jlinkEx + r.clip // for shell

	} else if runtime.GOOS == "linux" {
		r.jlinkEx = "/JLinkRTTLogger"                    // todo: check
		r.jlinkLib = "/JLinkARM.so"                      // todo: check
		r.shell = "gnome-terminal"                       // this only works for gnome based linux desktop env
		r.clip = "-- /bin/bash -c " + r.jlinkEx + r.clip // for shell

	} else {
		log.Fatal("trice is running on unknown operating system")
	}

	// check environment
	if _, err := os.Stat(r.jlinkEx); os.IsNotExist(err) {
		log.Fatal(r.jlinkEx, " does not exist")
	}
	if _, err := os.Stat(r.jlinkLib); os.IsNotExist(err) {
		log.Fatal(r.jlinkLib, " does not exist")
	}
	return r
}

// Read() is part of the exported interface io.ReadCloser. It reads a slice of bytes.
func (p *RTTL) Read(b []byte) (int, error) {
	return p.tlfH.Read(b)
}

// Close is part of the exported interface io.ReadCloser. It ends the connection.
//
// See https://stackoverflow.com/questions/11886531/terminating-a-process-started-with-os-exec-in-golang
func (p *RTTL) Close() error {
	var err error
	// if err = p.lcmdH.Process.Kill(); nil != err {
	// 	fmt.Print(err)
	// }
	// if err = os.Remove(p.tlfH.Name()); nil != err {
	// 	fmt.Print(err) // remove C:\Users\ms\AppData\Local\Temp\trice-458250371.bin: The process cannot access the file because it is being used by another process.
	// }
	return err
}

// Open starts the JLinkRTTLogger command with a temporary logfile
//
// THe temporary logfile is opened for reading.
func (p *RTTL) Open() error {
	var err error
	if global.Verbose {
		fmt.Println("Start a process:", p.shell, p.clip)
	}
	p.lcmdH = exec.Command(p.shell, p.clip)
	if err = p.lcmdH.Start(); err != nil {
		log.Fatal("tart error", err)
	}

	p.tlfH, err = os.Open(p.tlfN) // Open() opens a file with read only flag.
	if nil != err {
		return err
	}
	if global.Verbose {
		fmt.Println("trice is reading from", p.tlfN)
	}
	return nil
}

// TODO This code works but does not delete the temp file.
// SetupCloseHandler creates a 'listener' on a new goroutine which will notify the
// program if it receives an interrupt from the OS. We then handle this by calling
// our clean up procedure and exiting the program.
// func init() {
// 	c := make(chan os.Signal)
// 	signal.Notify(c, os.Interrupt, syscall.SIGTERM)
// 	go func() {
// 		<-c
// 		fmt.Println("\n\r- Ctrl+C pressed in Terminal")
// 		pointerToInstance.Close()
// 		time.Sleep(3 * time.Second)
// 		os.Exit(0)
// 	}()
// }
