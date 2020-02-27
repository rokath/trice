//
// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
//                basti@blackoutcloud.de
// All rights reserved.
// Use of this source code is governed by a
// license that can be found in the LICENSE file.

package receiver

import (
	"errors"
	"log"
	"time"

	"github.com/rokath/trice/pkg/treyfer"
	"go.bug.st/serial"
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

func NewSerialReceiver(port_identifier string, baudrate int) *SerialReceiver {
	s := &SerialReceiver{
		receiver:     receiver{"SerialReceiver", false, make(chan []byte)},
		port_name:    port_identifier,
		read_timeout: 1,
		serial_mode: serial.Mode{BaudRate: baudrate,
			DataBits: 8,
			Parity:   serial.NoParity,
			StopBits: serial.OneStopBit},
	}

	return s
}

func (self *SerialReceiver) SetReadTimeOut(timeout int) {
	self.read_timeout = timeout
}

func (self *SerialReceiver) SetParity(parity serial.Parity) {
	self.serial_mode.Parity = parity
}

func (self *SerialReceiver) SetDataBits(databits int) {
	self.serial_mode.DataBits = databits
}

func (self *SerialReceiver) SetBaudrate(baudrate int) {
	self.serial_mode.BaudRate = baudrate
}

func (self *SerialReceiver) SetStopBits(stopbits serial.StopBits) {
	self.serial_mode.StopBits = stopbits
}

func (self *SerialReceiver) SetUp() bool {
	var err error

	self.serial_handle, err = serial.Open(self.port_name, &self.serial_mode)

	if err != nil {
		log.Println("Error: Could not open serial port:", err)
		return false
	} else {
		return true
	}
}

// starts receiving of serial data
func (self *SerialReceiver) Start() {
	self.receiving_data = true
	go self.receiving()
}

// Stops receiving of serial data
func (self *SerialReceiver) Stop() {
	self.receiving_data = false
}

func (self *SerialReceiver) CleanUp() {
	self.Stop()
	self.serial_handle.Close()
}

// ReadEndless expects a pointer to a filled COM port configuration
func (self *SerialReceiver) receiving() {
	for self.receiving_data == true {
		b, err := self.readHeader()

		if nil != err {
			log.Println("Could not read serial header: ", err)
			continue
		}

		if 0xeb == b[0] { // traceLog startbyte, no further data
			self.bytes_channel <- b // send to process trace log channel

		} else {
			log.Println("Got unknown header on serial console. Discarding...", b)
		}
	}
}

func (self *SerialReceiver) ClosePort() {
	self.serial_handle.Close()
}

// export readBytes
func (self *SerialReceiver) readBytes(count int) (int, []byte) {
	b := make([]byte, count) // the buffer size limits the read count
	n, err := self.serial_handle.Read(b)

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

	var key = [...]uint8{0x11, 0x22, 0x33, 0x44, 0x44, 0x66, 0x77, 0x88}
	b = treyfer.Decrypt(b, key)

	for b[1] != remAddr || b[2] != locAddr ||
		b[0]^b[1]^b[2]^b[4]^b[5]^b[6]^b[7] != b[3] { // crc8 check

		log.Println("Discarding package", b[0])

		x, err := p.readAtLeastBytes(1, toMs)

		if nil != err {
			return b, err
		}

		b = append(b[1:], x...) // try to sync
		b = treyfer.Decrypt(b, key)
	}
	return b, nil
}

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
