//
// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
//                basti@blackoutcloud.de
// All rights reserved.
// Use of this source code is governed by a
// license that can be found in the LICENSE file.

package receiver

import (
	"encoding/binary"
	"errors"
	"fmt"
	"log"
	"time"

	"go.bug.st/serial"
	"golang.org/x/crypto/xtea"
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

// NewSerialReceiver
func NewSerialReceiver(portIdentifier string, baudrate int) *SerialReceiver {
	s := &SerialReceiver{
		receiver:     receiver{"SerialReceiver", false, make(chan []byte), make(chan []byte)},
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
	p.receivingData = true
	go p.receiving()
}

// Stop stops receiving of serial data
func (p *SerialReceiver) Stop() {
	p.receivingData = false
}

// CleanUp makes clean
func (p *SerialReceiver) CleanUp() {
	p.Stop()
	p.serial_handle.Close()
}

// receiving: ReadEndless expects a pointer to a filled COM port configuration
func (p *SerialReceiver) receiving() {
	for p.receivingData == true {
		b, err := p.readHeader()

		if nil != err {
			log.Println("Could not read serial header: ", err)
			continue
		}

		if 0xeb == b[0] { // traceLog startbyte, no further data
			//fmt.Println("to trice channel:", b) // ERR: DATA STREAM BUG!!!
			p.triceChannel <- b // send to process trace log channel

		} else if 0xc0 == b[0] {
			//fmt.Print("data", b)
			switch b[6] & 0xc0 {
			case 0xc0:
				log.Println("reCal command expecting an answer")
			case 0x80:
				log.Println("reCal message (not expecting an answer)")
			case 0x40:
				log.Println("answer to a reCal command")
			case 0x00:
				if (0xff != b[4]) || (0xff != b[5]) || (1 != b[7]) {
					log.Println("ERR:wrong format")
				} else {
					// int(b[6]) contains string identificator for verification
					d, _ := p.readAtLeastBytes(2, toMs) // len of buffer (only one buffer)
					if nil != err {
						log.Println("Could not read serial len: ", err)
						continue
					}
					len := int(binary.LittleEndian.Uint16(d[:2]))
					s, _ := p.readAtLeastBytes(len+1, toMs) // len is len-1 value
					if nil != err {
						log.Println("Could not read buffer: ", err)
						continue
					}
					b = append(b, d...) // len is redundant here and usable as check
					b = append(b, s...) // the buffer (string) data
					//fmt.Println("to buffer channel:", b) // ERR: DATA STREAM BUG!!!
					p.bufferChannel <- b // send to process trace log channel
				}
			}
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

// evalHeader checks if b contains valid header data
func evalHeader(b []byte) bool {
	x := 8 == len(b) &&
		(0xc0 == b[0] || 0xeb == b[0]) && // start byte
		remAddr == b[1] &&
		locAddr == b[2] &&
		b[0]^b[1]^b[2]^b[4]^b[5]^b[6]^b[7] == b[3] // crc8
	return x
}

// Cipher is a pointer to the cryptpo struct filled during initialization
var Cipher *xtea.Cipher

// Crypto set to true if a -key other than "none" was given
var Crypto bool

//! tested with little endian embedded device
func swapBytes(b []byte) []byte {
	return []byte{b[3], b[2], b[1], b[0], b[7], b[6], b[5], b[4]}
}

func encrypt(b []byte) []byte {
	if true == Crypto {
		b = swapBytes(b)
		Cipher.Encrypt(b, b)
		b = swapBytes(b)
	}
	return b
}

func decrypt(b []byte) []byte {
	if true == Crypto {
		b = swapBytes(b)
		Cipher.Decrypt(b, b)
		b = swapBytes(b)
	}
	return b
}

// readHeader gets next header from streaming data
func (p *SerialReceiver) readHeader() ([]byte, error) {
	b, err := p.readAtLeastBytes(8, toMs)
	if nil != err {
		return b, err
	}
	for {
		b = decrypt(b)
		if true == evalHeader(b) {
			break
		}
		fmt.Print(b)
		log.Printf("discarding first byte %02x\n", b[0])
		b = encrypt(b)
		x, err := p.readAtLeastBytes(1, toMs)
		if nil != err {
			return b, err
		}
		b = append(b[1:], x...) // try to sync
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
