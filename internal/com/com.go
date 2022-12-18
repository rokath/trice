// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
//                basti@blackoutcloud.de
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package com reads from COM port.
package com

import (
	"fmt"
	"io"
	"log"
	"os"
	"strings"

	"go.bug.st/serial"
)

var (
	// SerialPortName is the OS specific serial port name used
	SerialPortName string

	// BaudRate is the configured baud rate of the serial port. It is set as command line parameter.
	BaudRate int

	// DataBits is the serial port bit count for one "byte".
	DataBits int

	// Parity is the transmitted bit parity: "even", "odd", "none"
	Parity string

	// StopBits is the number of stop bits: "1", "1.5", "2"
	StopBits string

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

// port is a serial device trice receiver
type port struct {
	verbose      bool
	port         string
	serialHandle serial.Port
	serialMode   serial.Mode
	w            io.Writer
}

// NewPort creates an instance of a serial device type trice receiver
func NewPort(w io.Writer, comPortName string, verbose bool) *port {
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

	var stopBits serial.StopBits
	switch strings.ToLower(StopBits) {
	case "1", "one":
		stopBits = serial.OneStopBit
	case "1.5":
		stopBits = serial.OnePointFiveStopBits
	case "2", "two":
		stopBits = serial.TwoStopBits
	default:
		log.Fatalf(" Unknown stop bits value \"%s\". Valid are \"1\", \"1.5\". \"2\"\n", StopBits)
	}

	if !(5 <= DataBits && DataBits <= 9) {
		log.Fatalf("Invalid dataBits value %d. Valid are 5-9\n", DataBits)
	}

	r := &port{
		port: comPortName,
		serialMode: serial.Mode{
			BaudRate: BaudRate,
			DataBits: DataBits,
			Parity:   parity,
			StopBits: stopBits,
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
			fmt.Fprintln(p.w, err, "try '", os.Args[0], "s' to check for serial ports")
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

//  // PortTarm is a serial device trice receiver.
//  type PortTarm struct {
//  	config  serialtarm.Config
//  	stream  *serialtarm.Port
//  	w       io.Writer
//  	verbose bool
//  }
//
//  // NewCOMPortTarm creates an instance of a serial device type trice receiver.
//  func NewCOMPortTarm(w io.Writer, verbose bool, comPortName string) *PortTarm {
//  	var p = new(PortTarm)
//  	p.w = w
//  	p.verbose = verbose
//  	p.config.Name = comPortName
//  	p.config.Baud = Baud
//  	p.config.ReadTimeout = 100 * time.Millisecond
//  	p.config.Size = 8
//  	if p.verbose {
//  		fmt.Fprintln(w, "NewCOMPortTarm:", p.config)
//  	}
//  	if 5 <= Databits && Databits <= 9 {
//  		p.config.Size = byte(Databits)
//  	} else {
//  		log.Fatalf("Invalid databits value %d. Valid are 5-9\n", Databits)
//  	}
//
//  	switch strings.ToLower(Parity) {
//  	case "o", "odd":
//  		p.config.Parity = serialtarm.ParityOdd
//  	case "e", "even":
//  		p.config.Parity = serialtarm.ParityEven
//  	case "n", "none":
//  		p.config.Parity = serialtarm.ParityNone
//  	default:
//  		log.Fatalf(" Unknown parity value \"%s\". Valid are \"odd\", \"even\". \"none\"\n", Parity)
//  	}
//  	switch strings.ToLower(Stopbits) {
//  	case "1", "one":
//  		p.config.StopBits = serialtarm.Stop1
//  	case "1.5":
//  		p.config.StopBits = serialtarm.Stop1Half
//  	case "2", "two":
//  		p.config.StopBits = serialtarm.Stop2
//  	default:
//  		log.Fatalf(" Unknown stop bits value \"%s\". Valid are \"1\", \"1.5\". \"2\"\n", Parity)
//  	}
//
//  	return p
//  }
//
//  // Open returns true on successful operation.
//  func (p *PortTarm) Open() bool {
//  	var err error
//  	p.stream, err = serialtarm.OpenPort(&p.config)
//  	if err != nil {
//  		if p.verbose {
//  			fmt.Fprintln(p.w, p.config.Name, "not found")
//  			fmt.Fprintln(p.w, "try 'trice scan'")
//  		}
//  		return false
//  	}
//  	return true
//  }
//
//  // Close returns an error in case of failure.
//  func (p *PortTarm) Close() error {
//  	if p.verbose {
//  		fmt.Fprintln(p.w, "Closing Tarm COM port")
//  	}
//  	return p.stream.Close()
//  }
//
//  // Read blocks until (at least) one byte is received from
//  // the serial port or an error occurs.
//  // It stores data received from the serial port into the provided byte array
//  // buffer. The function returns the number of bytes read.
//  func (p *PortTarm) Read(buf []byte) (int, error) {
//  	return p.stream.Read(buf)
//  }
