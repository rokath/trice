// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
//                basti@blackoutcloud.de
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package com reads from COM port.
package com

import (
	"fmt"
	"io"
	"log"

	"go.bug.st/serial"
)

var (
	// Baud is the configured baudrate of the serial port. It is set as command line parameter.
	Baud int
)

///////////////////////////////////////////////////////////////////////////////////////////////////
// dynamic debug helper
//
type bytesViewer struct {
	r io.Reader
}

// newBytesViewer returns a reader `get` which is internally using reader `from`.
// Calling the `get` Read method leads to internally calling the `from` Read method
// but lets to do some additional action like logging
func newBytesViewer(from io.Reader) (get io.Reader) {
	return &bytesViewer{from}
}

func (p *bytesViewer) Read(buf []byte) (count int, err error) {
	count, err = p.r.Read(buf)

	log.Println("bytesViewer:", err, count, buf)
	return
}

//
///////////////////////////////////////////////////////////////////////////////////////////////////

// COM is a serial device trice receiver
type COM struct {
	port         string
	serialHandle serial.Port
	serialMode   serial.Mode
}

// New creates an instance of a serial device type trice receiver
func New(comPort string) *COM {
	r := &COM{
		port: comPort,
		serialMode: serial.Mode{
			BaudRate: Baud,
			DataBits: 8,
			Parity:   serial.NoParity,
			StopBits: serial.OneStopBit,
		},
	}
	return r
}

// Stores data received from the serial port into the provided byte array
// buffer. The function returns the number of bytes read.
//
// The Read function blocks until (at least) one byte is received from
// the serial port or an error occurs.
func (p *COM) Read(buf []byte) (int, error) {
	count, err := p.serialHandle.Read(buf)
	//log.Println("COM.Read:", err, count, buf[:count])
	return count, err
}

// Close releases port
func (p *COM) Close() error {
	return p.serialHandle.Close()
}

// Open initializes the serial receiver.
//
// It opens a serial port.
func (p *COM) Open() bool {
	var err error

	p.serialHandle, err = serial.Open(p.port, &p.serialMode)

	if err != nil {
		fmt.Println(err, "try 'trice s' to check for serial ports")
		return false
	}
	return true
}

// GetSerialPorts scans for serial ports
func GetSerialPorts() ([]string, error) {
	ports, err := serial.GetPortsList()

	if err != nil {
		fmt.Println(err)
		return ports, err
	}
	if len(ports) == 0 {
		fmt.Println("No serial ports found!")
		return ports, err
	}
	for _, port := range ports {
		fmt.Println("Found port: ", port)
	}

	return ports, err
}

/*
// conditionalComPortScan scans for COM ports if -port was specified as COMscan, it tries to use first found COM port.
func conditionalComPortScan() error {
	if "COMscan" != Port {
		return nil
	}
	log.Println("Scan for serial ports...")
	ports, err := GetSerialPorts()
	if err != nil {
		return err
	}
	if len(ports) > 0 {
		log.Println("Take serial port", ports[0])
		Port = ports[0]
		return nil
	}
	return errors.New("Could not find serial port on system")
}
*/
