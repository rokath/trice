// Copyright 2020 basti@blackoutcloud.de
//                Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package receiver is responsible for getting and handling the bytes from the communication port.
// There ae different communication ports possible: COM, RTT, TCP...
// It is activated by the trice package and calls emit.Trice() on received bytes chunks.
package receiver

import (
	"encoding/binary"
	"fmt"
	"log"

	"github.com/rokath/trice/pkg/emit"
	"github.com/rokath/trice/pkg/id"
	"golang.org/x/crypto/xtea"
)

var (
	// Device is the trice receiver to use
	Device string
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

// newTriceReceiver creates an instance of the common trice receiver part for a new receiver device
func newTriceReceiver(r triceReceiverInterface) *triceReceiver {
	return &triceReceiver{
		triceReceiverInterface: r,
		name:                   "trice receiver",
		receivingData:          false,
		triceChannel:           make(chan []byte),
		bufferChannel:          make(chan []byte),
	}
}

// getTriceChannel returns pointer to trice receive channel
func (p *triceReceiver) getTriceChannel() *chan []byte {
	return &p.triceChannel
}

// getBufferChannel returns pointer to buffer receive channel
func (p *triceReceiver) getBufferChannel() *chan []byte {
	return &p.bufferChannel
}

// Start starts receiving of serial data
func (p *triceReceiver) Start() {
	p.receivingData = true
	go p.receiving()
}

// Stop stops receiving of serial data
func (p *triceReceiver) Stop() {
	p.receivingData = false
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

// receiving: ReadEndless expects a pointer to a filled COM port configuration
func (p *triceReceiver) receiving() {
	for p.receivingData == true {
		b, err := p.readHeader()

		if nil != err {
			fmt.Println("Could not read serial header: ", err)
			continue
		}

		if 0xeb == b[0] { // traceLog startbyte, no further data
			//fmt.Println("to trice channel:", b)
			p.triceChannel <- b // send to process trace log channel

		} else if 0xc0 == b[0] {
			switch b[6] & 0xc0 {
			case 0xc0:
				fmt.Println("reCal command expecting an answer", b)
			case 0x80:
				fmt.Println("reCal message (not expecting an answer)", b)
			case 0x40:
				fmt.Println("answer to a reCal command")
			case 0x00:
				if (0xff != b[4]) || (0xff != b[5]) || (1 != b[7]) {
					fmt.Println("ERR:wrong format")
				} else {
					// int(b[6]) contains string identificator for verification
					d, _ := p.readAtLeastBytes(2) // len of buffer (only one buffer)
					if nil != err {
						fmt.Println("Could not read serial len: ", err)
						continue
					}
					len := int(binary.LittleEndian.Uint16(d[:2]))
					s, _ := p.readAtLeastBytes(len + 1) // len is len-1 value
					if nil != err {
						fmt.Println("Could not read buffer: ", err)
						continue
					}
					b = append(b, d...) // len is redundant here and usable as check
					b = append(b, s...) // the buffer (string) data
					//log.Println("to buffer channel:", b)
					p.bufferChannel <- b // send to process trace log channel
				}
			}
		} else {
			fmt.Println("Got unknown header on serial console. Discarding...", b)
		}
	}
}
