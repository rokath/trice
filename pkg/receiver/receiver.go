// Copyright 2020 basti@blackoutcloud.de
//                Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package receiver is responsible for getting and handling the bytes from the communication port.
// Currently only serial port is supported.
// It is activated by the trice package and call emit.Trice() on received bytes chunks.
package receiver

import (
	"fmt"
	"log"

	"github.com/rokath/trice/pkg/emit"
	"github.com/rokath/trice/pkg/id"
	"golang.org/x/crypto/xtea"
)

var (
	// Device is the trice receiver to use
	Device string

	// Port is the COMport name like COM1
	Port string

	// Baud is the configured baudrate of the serial port
	Baud int
)

type triceReceiverInterface interface { // Daemon
	getTriceChannel() *chan []byte
	getBufferChannel() *chan []byte
	Start()
	Stop()
	Read([]byte) (int, error)
	doReceive()
	readBytes(count int) (int, []byte)
	readAtLeastBytes(count int) ([]byte, error)
	readHeader() ([]byte, error)
}

// abstractReceiver is an abstrace type
type triceReceiver struct { // AbstractDaemon
	triceReceiverInterface // interface
	name                   string
	receivingData          bool
	triceChannel           chan []byte
	bufferChannel          chan []byte
}

func (p *triceReceiver) getTriceChannel() *chan []byte {
	return &p.triceChannel
}

func (p *triceReceiver) getBufferChannel() *chan []byte {
	return &p.bufferChannel
}

func (p *triceReceiver) doReceive() {
	var err error
	var t, b []byte
	for {
		select {
		case c := <-(*p.getBufferChannel()):
			if len(c) > 0 {
				b = append(b, c...)
			}
		case t = <-(*p.getTriceChannel()):
			b, err = emit.Trice(t, b, id.List)
			if nil != err {
				log.Println("trice.Log error", err, t, b)
			}
		}
	}
}

// export readBytes
func (p *triceReceiver) readBytes(count int) (int, []byte) {
	b := make([]byte, count) // the buffer size limits the read count
	n, err := p.Read(b)

	if err != nil {
		log.Fatal(err)
	}

	return n, b
}

// export readAtLeastBytes
func (p *triceReceiver) readAtLeastBytes(count int) ([]byte, error) {
	buf := make([]byte, count) // the buffer size limits the read count
	var b []byte
	var n int
	var err error
	//start := time.Now()
	for len(b) < count {
		n, err = p.Read(buf)
		if err != nil {
			log.Println("Port read error")
			log.Fatal(err)
		}
		b = append(b, buf[:n]...)
		if count == len(b) { // https://stackoverflow.com/questions/45791241/correctly-measure-time-duration-in-go
			//fmt.Println("\tincoming:", b, "\t\t", time.Now().Sub(start).Nanoseconds(), "nanoseconds")
			return b, nil
		}
		buf = buf[n:]
	}
	return b, nil
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
func (p *triceReceiver) readHeader() ([]byte, error) {
	b, err := p.readAtLeastBytes(8)
	if nil != err {
		return b, err
	}
	for {
		b = decrypt(b)
		if true == evalHeader(b) {
			break
		}
		emit.LineCollect(fmt.Sprintf("wrn:trice:discarding byte 0x%02x (dez %d)\n", b[0], b[0]))
		b = encrypt(b)
		x, err := p.readAtLeastBytes(1)
		if nil != err {
			return b, err
		}
		b = append(b[1:], x...) // try to sync
	}
	return b, nil
}
