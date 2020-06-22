// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package jlinkrttlogger reads from SeggerRTT with the SEGGER app JLinkRTTLogger.
//
// It provides a ReadCloser interface and makes no assumptiona about the delivered data.
// It is also agnostic concerning the RTT channel and other setup parameters.
package jlinkrttlogger

import (
	"fmt"
	"io/ioutil"
	"log"
	"os"
	"os/exec"
	"path/filepath"
	"runtime"
)

// RTTL is the Segger RealTime Transfer logger reader interface.
type RTTL struct {
	tlfN string   // tempLogFile name
	tlfH *os.File // tempLogFile handle

	lcmdN string    // jlinkrttlogger command name
	lcmdH *exec.Cmd // jlinkrttlogger command handle

	jlinkEx  string // name of JLinkRTTLogger executable
	jlinkLib string // name of JLinkRTTLogger dynamic library

	shell string // os calling environment
	clip  string // full shell parameter string including JLinkRTTLogger and its parameters - see SEGGER UM08001_JLink.pdf
}

// New creates an instance of RTT ReadCloser.
//
// It is intended to be used by receiver.New() which embeds its interface.
func New(param string) *RTTL {
	r := &RTTL{} // create SeggerRTT instance

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
	/*
		r.cmd = exec.Command(r.shell, r.clip...)
		err = r.cmd.Run()
		if err != nil {
			log.Println(clip)
			log.Fatal(err)
		}
	*/
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
	fmt.Print("CLOSE")
	var err error
	if err = p.lcmdH.Process.Kill(); nil != err {
		fmt.Print(err)
	}

	if err = os.Remove(p.tlfH.Name()); nil != err {
		fmt.Print(err)
	}

	return err
}

// Open starts the JLinkRTTLogger command with a temporary logfile
//
// THe temporary logfile is opened for reading.
func (p *RTTL) Open() error {
	var err error
	fmt.Println("Start a process:", p.shell, p.clip)
	p.lcmdH = exec.Command(p.shell, p.clip)
	if err = p.lcmdH.Start(); err != nil {
		log.Fatal("tart error", err)
	}

	//fmt.Println(p.lcmdN, "writing to", p.tlfN)
	p.tlfH, err = os.Open(p.tlfN) // Open() opens a file with read only flag.
	if nil != err {
		return err
	}
	fmt.Println("trice is reading from", p.tlfN)
	return nil
}

/*
// SetupCloseHandler creates a 'listener' on a new goroutine which will notify the
// program if it receives an interrupt from the OS. We then handle this by calling
// our clean up procedure and exiting the program.
func init() {
	fmt.Println("SetupCloseHandler")
	c := make(chan os.Signal)
	signal.Notify(c, os.Interrupt, syscall.SIGTERM)
	go func() {
		fmt.Println("SetupCloseHandler 22222222222")
		<-c
		fmt.Println("\n\r- Ctrl+C pressed in Terminal")
		//DeleteFiles()
		os.Exit(0)
	}()
}
*/
