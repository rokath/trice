// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
//                basti@blackoutcloud.de
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package com reads from COM port.
package com

import (
	"errors"
	"fmt"
	"log"

	"go.bug.st/serial"
)

var (
	// Port is the COMport name like COM1
	Port string

	// Baud is the configured baudrate of the serial port
	Baud int
)

// COM is a serial device trice receiver
type COM struct {
	serialHandle serial.Port
	serialMode   serial.Mode
}

// New creates an instance of a serial device type trice receiver
func New() *COM {
	r := &COM{
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
	return p.serialHandle.Read(buf)
}

// Close releases port
func (p *COM) Close() error {
	return p.serialHandle.Close()
}

// open() initializes the serial receiver.
//
// It opens a serial port.
func (p *COM) Open() bool {
	var err error

	p.serialHandle, err = serial.Open(Port, &p.serialMode)

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
		log.Println(err)
		return ports, err
	}
	if len(ports) == 0 {
		log.Println("No serial ports found!")
		return ports, err
	}
	for _, port := range ports {
		log.Println("Found port: ", port)
	}

	return ports, err
}

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
