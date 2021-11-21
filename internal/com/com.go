// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
//                basti@blackoutcloud.de
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package com reads from COM port.
package com

import (
	"fmt"
	"io"
	"time"

	serialtarm "github.com/tarm/serial"
	serialgobugst "go.bug.st/serial"
)

var (
	// Baud is the configured baudrate of the serial port. It is set as command line parameter.
	Baud int

	// Verbose shows additional information if set true.
	Verbose bool
)

// COMport is the comport interface type to use different COMports.
type COMport interface {
	Open() bool
	Read(buf []byte) (int, error)
	Close() error
}

// PortGoBugSt is a serial device trice receiver
type PortGoBugSt struct {
	port         string
	serialHandle serialgobugst.Port
	serialMode   serialgobugst.Mode
	w            io.Writer
}

// NewCOMPortGoBugSt creates an instance of a serial device type trice receiver
func NewCOMPortGoBugSt(w io.Writer, comPortName string) *PortGoBugSt {
	r := &PortGoBugSt{
		port: comPortName,
		serialMode: serialgobugst.Mode{
			BaudRate: Baud,
			DataBits: 8,
			Parity:   serialgobugst.NoParity,
			StopBits: serialgobugst.OneStopBit,
		},
	}
	r.w = w
	if Verbose {
		fmt.Fprintln(w, "NewCOMPortGoBugSt:", r)
	}
	return r
}

// Read blocks until (at least) one byte is received from
// the serial port or an error occurs.
// It stores data received from the serial port into the provided byte array
// buffer. The function returns the number of bytes read.
func (p *PortGoBugSt) Read(buf []byte) (int, error) {
	return p.serialHandle.Read(buf)
}

// Close releases port.
func (p *PortGoBugSt) Close() error {
	if Verbose {
		fmt.Fprintln(p.w, "Closing GoBugSt COM port")
	}
	return p.serialHandle.Close()
}

// Open initializes the serial receiver.
//
// It opens a serial port.
func (p *PortGoBugSt) Open() bool {
	var err error
	p.serialHandle, err = serialgobugst.Open(p.port, &p.serialMode)
	if err != nil {
		if Verbose {
			fmt.Fprintln(p.w, err, "try 'trice s' to check for serial ports")
		}
		return false
	}
	return true
}

// GetSerialPorts scans for serial ports.
func GetSerialPorts(w io.Writer) ([]string, error) {
	ports, err := serialgobugst.GetPortsList()

	if err != nil {
		fmt.Fprintln(w, err)
		return ports, err
	}
	if len(ports) == 0 {
		fmt.Fprintln(w, "No serial ports found!")
		return ports, err
	}
	for _, port := range ports {
		fmt.Fprintln(w, "Found port: ", port)
	}
	return ports, err
}

// PortTarm is a serial device trice receiver.
type PortTarm struct {
	config serialtarm.Config
	stream *serialtarm.Port
	w      io.Writer
}

// NewCOMPortTarm creates an instance of a serial device type trice receiver.
func NewCOMPortTarm(w io.Writer, comPortName string) *PortTarm {
	var p = new(PortTarm)
	p.w = w
	p.config.Name = comPortName
	p.config.Baud = Baud
	p.config.ReadTimeout = 100 * time.Millisecond
	p.config.Size = 8
	if Verbose {
		fmt.Fprintln(w, "NewCOMPortTarm:", p.config)
	}
	return p
}

// Open returns true on successful operation.
func (p *PortTarm) Open() bool {
	var err error
	p.stream, err = serialtarm.OpenPort(&p.config)
	if err != nil {
		if Verbose {
			fmt.Fprintln(p.w, p.config.Name, "not found")
			fmt.Fprintln(p.w, "try 'trice scan'")
		}
		return false
	}
	return true
}

// Close returns an error in case of failure.
func (p *PortTarm) Close() error {
	if Verbose {
		fmt.Fprintln(p.w, "Closing Tarm COM port")
	}
	return p.stream.Close()
}

// Read blocks until (at least) one byte is received from
// the serial port or an error occurs.
// It stores data received from the serial port into the provided byte array
// buffer. The function returns the number of bytes read.
func (p *PortTarm) Read(buf []byte) (int, error) {
	return p.stream.Read(buf)
}
