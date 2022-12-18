// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package com reads from and writes to COM port.
package stimcom

import (
	"fmt"
	"io"
	"log"
	"strings"

	"go.bug.st/serial"
)

var (
	// SerialPort is the OS specific serial port name used
	SerialPort string

	// BaudRate is the configured baud rate of the serial port. It is set as command line parameter.
	BaudRate int

	// Parity is the configured baud parity of the serial port. It is set as command line parameter.
	Parity string

	// Verbose shows additional information if set true.
	Verbose = false
)

// COMport is the comport interface type to use different COMports.
type COMport interface {
	Open() bool
	Read(buf []byte) (int, error)
	Write(buf []byte) (int, error)
	Close() error
}

// PortGoBugSt is a serial device trice receiver
type port struct {
	verbose      bool
	port         string
	serialHandle serial.Port
	serialMode   serial.Mode
	w            io.Writer
}

// NewPort creates an instance of a serial device type trice receiver
func NewPort(w io.Writer, comPort string, verbose bool) *port {
	var parity serial.Parity
	switch strings.ToLower(Parity) {
	case "n", "no", "none":
		parity = serial.NoParity
	case "e", "ev", "even":
		parity = serial.EvenParity
	case "o", "odd":
		parity = serial.OddParity
	default:
		log.Fatal("invalid parity value: ", Parity, " Accepting case insensitive: n|no|none|e|even||o|odd.")
	}

	r := &port{
		port: comPort,
		serialMode: serial.Mode{
			BaudRate: BaudRate,
			DataBits: 8,
			Parity:   parity,            // serial.NoParity, serial.EvenParity, serial.OddParity
			StopBits: serial.OneStopBit, // serial.OnePointFiveStopBits, serial.TwoStopBits
		},
	}
	r.w = w
	r.verbose = verbose
	if verbose {
		fmt.Fprintln(w, "New COM port:", r)
	}
	return r
}

// Read blocks until (at least) one byte is received from
// the serial port or an error occurs.
// It stores data received from the serial port into the provided byte array
// buffer. The function returns the number of bytes read.
func (p *port) Read(buf []byte) (int, error) {
	return p.serialHandle.Read(buf)
}

func (p *port) Write(buf []byte) (int, error) {
	return p.serialHandle.Write(buf)
}

// Close releases port.
func (p *port) Close() error {
	if p.verbose {
		fmt.Fprintln(p.w, "Closing COM port")
	}
	return p.serialHandle.Close()
}

// Open initializes the serial receiver.
//
// It opens a serial port.
func (p *port) Open() bool {
	var err error
	p.serialHandle, err = serial.Open(p.port, &p.serialMode)
	if err != nil {
		if p.verbose {
			fmt.Fprintln(p.w, err, "try 'stim s' to check for serial ports")
		}
		return false
	}
	return true
}

// GetSerialPorts scans for serial ports.
func GetSerialPorts(w io.Writer) ([]string, error) {
	ports, err := serial.GetPortsList()

	if err != nil {
		fmt.Fprintln(w, err)
		return ports, err
	}
	if len(ports) == 0 {
		if Verbose {
			fmt.Fprintln(w, "No serial ports found!")
		}
		return ports, err
	}
	for _, port := range ports {
		pS := NewPort(w, port, false)
		if pS.Open() {
			pS.Close()
			fmt.Fprintln(w, "Found port: ", port)
		} else {
			fmt.Fprintln(w, "Found port: ", port, "(used)")
		}
	}
	return ports, err
}
