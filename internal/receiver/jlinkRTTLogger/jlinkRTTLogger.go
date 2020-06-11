// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package jlinkRTTLogger reads from SeggerRTT.
package jlinkRTTLogger

import (
	"fmt"
	"io/ioutil"
	"os"
	"os/exec"
)

// RTTL is the Segger RealTime Transfer logger reader interface.
type RTTL struct {
	fN string    // tempLogFile name
	fH *os.File  // tempLogFile handle
	ch string    // used RTT channel
	cN string    // JLinkRTTLogger command name
	cH *exec.Cmd // JLinkRTTLogger command handle
}

// New creates an instance of RTT ReadCloser.
//
// It is intended to be used by receiver.New() which embeds its interface.
func New() *RTTL {
	r := &RTTL{} // create SeggerRTT instance
	r.ch = "-RTTChannel 0"
	r.fH, _ = ioutil.TempFile(os.TempDir(), "trice-*.bin") // opens for read and write
	r.fN = r.fH.Name()
	r.fH.Close()
	//r.cN = "/c/Program Files x86/SEGGER/JLink/JLinkRTTLogger.exe"
	r.cN = "C:\\Program Files (x86)\\SEGGER\\JLink\\JLinkRTTLogger.exe"
	return r
}

// Read() is part of the exported interface io.ReadCloser. It reads a slice of bytes.
func (p *RTTL) Read(b []byte) (int, error) {
	return p.fH.Read(b)
}

// Close is part of the exported interface io.ReadCloser. It ends the connection.
//
// See https://stackoverflow.com/questions/11886531/terminating-a-process-started-with-os-exec-in-golang
func (p *RTTL) Close() error {
	if err := p.cH.Process.Kill(); err != nil {
		return err
	}
	return os.Remove(p.fH.Name())
}

// Open starts the JLinkRTTLogger command with a temporary logfile
//
// THe temporary logfile is opened for reading.
func (p *RTTL) Open() error {
	var err error
	// Start a process:
	p.cH = exec.Command(p.cN, "-Device STM32F030R8 -if SWD -Speed 4000 "+p.ch+" "+p.fN)
	if err = p.cH.Start(); err != nil {
		return err
	}
	fmt.Println(p.cN, "writing to", p.fN)
	p.fH, err = os.Open(p.fN) // Open() opens a file with read only flag.
	if nil != err {
		return err
	}
	fmt.Println("trice is reading from", p.fN)
	return nil
}
