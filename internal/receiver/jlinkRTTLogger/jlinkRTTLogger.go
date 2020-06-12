// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package jlinkRTTLogger reads from SeggerRTT.
package jlinkRTTLogger

import (
	"fmt"
	"io/ioutil"
	"log"
	"os"
	"os/exec"
	"os/signal"
	"runtime"
	"syscall"
)

// RTTL is the Segger RealTime Transfer logger reader interface.
type RTTL struct {
	tlfN  string    // tempLogFile name
	tlfH  *os.File  // tempLogFile handle
	ch    string    // used RTT channel
	lcmdN string    // JLinkRTTLogger command name
	lcmdH *exec.Cmd // JLinkRTTLogger command handle
}

// New creates an instance of RTT ReadCloser.
//
// It is intended to be used by receiver.New() which embeds its interface.
func New() *RTTL {
	r := &RTTL{} // create SeggerRTT instance
	r.ch = "-RTTChannel 0"
	r.tlfH, _ = ioutil.TempFile(os.TempDir(), "trice-*.bin") // opens for read and write
	r.tlfN = r.tlfH.Name()

	//r.tlfN = "C:\\Users\\ms\\AppData\\Local\\Temp\\trice-428975731.bin"

	r.tlfH.Close()
	//r.cN = "/c/Program Files x86/SEGGER/JLink/JLinkRTTLogger.exe"
	//r.lcmdN = "C:\\Program Files (x86)\\SEGGER\\JLink\\JLinkRTTLogger.exe"
	r.lcmdN = "C:\\repos\\trice\\third_party\\JLinkRTTLogger.exe"
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
	// Start a process:
	if runtime.GOOS == "windows" {
		//prog := p.lcmdN
		//clip := "-Device STM32F030R8 -if SWD -Speed 4000 " + p.ch + " " + p.tlfN

		prog := "cmd"
		clip := " /c " + p.lcmdN + " -Device STM32F030R8 -if SWD -Speed 4000 " + p.ch + " " + p.tlfN
		fmt.Println(prog, clip)
		p.lcmdH = exec.Command(prog, clip)
		if err = p.lcmdH.Start(); err != nil {
			log.Fatal("start error", err)
		}
	}
	fmt.Println(p.lcmdN, "writing to", p.tlfN)
	p.tlfH, err = os.Open(p.tlfN) // Open() opens a file with read only flag.
	if nil != err {
		return err
	}
	fmt.Println("trice is reading from", p.tlfN)
	return nil
}

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
