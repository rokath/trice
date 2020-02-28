//
// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
//                basti@blackoutcloud.de
// All rights reserved.
// Use of this source code is governed by a
// license that can be found in the LICENSE file.

package receiver

import (
	"errors"
	"fmt"
	"log"
	"time"

	"go.bug.st/serial"
	"golang.org/x/crypto/tea"
)

var locAddr = byte(0x60) // local address
var remAddr = byte(0x60) // remote address
var toMs = 1000

type SerialReceiver struct {
	receiver

	port_name    string
	read_timeout int

	// serial port configuration
	serial_handle serial.Port
	serial_mode   serial.Mode
}

var password string

// NewSerialReceiver
func NewSerialReceiver(portIdentifier string, baudrate int, pwd string) *SerialReceiver {
	password = pwd
	s := &SerialReceiver{
		receiver:     receiver{"SerialReceiver", false, make(chan []byte)},
		port_name:    portIdentifier,
		read_timeout: 1,
		serial_mode: serial.Mode{BaudRate: baudrate,
			DataBits: 8,
			Parity:   serial.NoParity,
			StopBits: serial.OneStopBit},
	}

	return s
}

// SetReadTimeOut sets timeout
func (p *SerialReceiver) SetReadTimeOut(timeout int) {
	p.read_timeout = timeout
}

// SetParity sets transmit parity
func (p *SerialReceiver) SetParity(parity serial.Parity) {
	p.serial_mode.Parity = parity
}

// SetDataBits sets bit count
func (p *SerialReceiver) SetDataBits(databits int) {
	p.serial_mode.DataBits = databits
}

// SetBaudrate sets speed
func (p *SerialReceiver) SetBaudrate(baudrate int) {
	p.serial_mode.BaudRate = baudrate
}

// SetStopBits sets stop condition
func (p *SerialReceiver) SetStopBits(stopbits serial.StopBits) {
	p.serial_mode.StopBits = stopbits
}

// SetUp opens a serial port
func (p *SerialReceiver) SetUp() bool {
	var err error

	p.serial_handle, err = serial.Open(p.port_name, &p.serial_mode)

	if err != nil {
		fmt.Println("Error: Could not open serial port:", err)
		return false
	} else {
		return true
	}
}

// Start starts receiving of serial data
func (p *SerialReceiver) Start() {
	p.receiving_data = true
	go p.receiving()
}

// Stop stops receiving of serial data
func (p *SerialReceiver) Stop() {
	p.receiving_data = false
}

// CleanUp makes clean
func (p *SerialReceiver) CleanUp() {
	p.Stop()
	p.serial_handle.Close()
}

// receiving: ReadEndless expects a pointer to a filled COM port configuration
func (p *SerialReceiver) receiving() {
	for p.receiving_data == true {
		b, err := p.readHeader()

		if nil != err {
			log.Println("Could not read serial header: ", err)
			continue
		}

		if 0xeb == b[0] { // traceLog startbyte, no further data
			p.bytes_channel <- b // send to process trace log channel

		} else {
			log.Println("Got unknown header on serial console. Discarding...", b)
		}
	}
}

// ClosePort releases port
func (p *SerialReceiver) ClosePort() {
	p.serial_handle.Close()
}

// export readBytes
func (p *SerialReceiver) readBytes(count int) (int, []byte) {
	b := make([]byte, count) // the buffer size limits the read count
	n, err := p.serial_handle.Read(b)

	if err != nil {
		log.Fatal(err)
	}

	return n, b
}

// export readBytes
func (p *SerialReceiver) readAtLeastBytes(count, msTimeout int) ([]byte, error) {
	buf := make([]byte, count) // the buffer size limits the read count

	var b []byte
	var n, ms int
	var err error

	for len(b) < count && ms < msTimeout {
		n, err = p.serial_handle.Read(buf)

		if err != nil {
			log.Println("Port read error")
			log.Fatal(err)
		}

		b = append(b, buf[:n]...)
		if count == len(b) {
			return b, nil
		}

		buf = buf[n:]
		time.Sleep(50 * time.Millisecond) // has slight influence on cpu load

		ms += 50
	}
	return b, errors.New("read timeout")
}

func (p *SerialReceiver) readHeader() ([]byte, error) {
	b, err := p.readAtLeastBytes(8, toMs)

	if nil != err {
		return b, err
	}

	// https://gobyexample.com/sha1-hashes
	if "" == password {
		for b[1] != remAddr || b[2] != locAddr ||
			b[0]^b[1]^b[2]^b[4]^b[5]^b[6]^b[7] != b[3] { // crc8 check
			log.Printf("discarding byte %02x\n", b[0])
			x, err := p.readAtLeastBytes(1, toMs)
			if nil != err {
				return b, err
			}
			b = append(b[1:], x...) // try to sync
		}
	} else {
		var key = []byte{0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF}
		c, err := tea.NewCipherWithRounds(key, 64)
		if err != nil {
			return nil, errors.New("NewCipher returned error")
		}
		c.Decrypt(b, b)
		for b[1] != remAddr || b[2] != locAddr ||
			b[0]^b[1]^b[2]^b[4]^b[5]^b[6]^b[7] != b[3] { // crc8 check
			c.Encrypt(b, b) // restore received data
			log.Printf("discarding byte %02x\n", b[0])

			x, err := p.readAtLeastBytes(1, toMs)

			if nil != err {
				return b, err
			}
			b = append(b[1:], x...) // try to sync
			c.Decrypt(b, b)
		}
	}
	return b, nil
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
