// Copyright 2020 basti@blackoutcloud.de
//                Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package receiver is responsible for getting and handling the bytes from the communication port.
// There ae different communication ports possible: COM, RTT, TCP...
// It is activated by the trice package and calls emit.Trice() on received bytes chunks.
//
// Implementation
// ==============
// - trice.DoReceive() is called when trice ist started in log mode
// - In dependence on receiverDevice DoSerial() or DoSeggerRTT() or ... is activated (named DoDevice() from now)
// - DoDevice()
//   - performs device specific initialization
//   - performs start() which starts the go routine receiving(), responsible for raw data (see below)
//   - and ends in device.doReceive()
// - device is a composed type from triceReceiver (common functionality) and deviceReceiver (device specific functionality)
// - The newDevice(...) function creates the device specific instance and links to it the triceReceiver instance created by
// device.triceReceiver = newTriceReceiver(device). The device parameter for newTriceReceiver() is the linked device interface
// into the triceReceiver instance so that the this way composed device instance can use the common triceReceiver parts as well.
// - receiving()
//  - scans the incoming data stream for valid headers,
//  - and puts these into the triceChannel, when trice haeders
//  - or puts them together with the following package data into the buffer channel if it is a valid package
//  - It is up to the trice implementation to add more channels here for other protocols
// - doReceive() is the endless loop waiting for data in the channels filled in by receiving()
//  - All incoming buffer packets are collected into a byte slice for later usage (case `TRICE_RUNTIME_GENERATED_STRINGS_SUPPORT_EXPERIMENTAL`)
//  - If a trice header comes out of the triceChannel emit.Trice() is called

package receiver

import (
	"encoding/binary"
	"fmt"
	"io"

	"github.com/rokath/trice/internal/disp"
	"github.com/rokath/trice/internal/emit"
	"github.com/rokath/trice/internal/id"
	"golang.org/x/crypto/xtea"
)

var (
	// Source is the trice receiver to use
	Source string

	// local trice address, used for routing in distributed systems
	locAddr = byte(0x60)

	// remote trice address, used for routing in distributed systems
	remAddr = byte(0x60)

	// DiscardByte is the function to execute if a data stream byte is to be discarded
	// type DicardFunc func(byte)
	DiscardByte = DiscardWithMessage
)

// DiscardWithMessage discards a byte with a verbose message
func DiscardWithMessage(b byte) {
	var level string
	if "off" == disp.ColorPalette {
		level = ""
	} else {
		level = "wrn:"
	}
	c := byte(' ')
	if 32 <= b && b <= 127 {
		c = b
	}
	emit.LineCollect(fmt.Sprintf("%strice:discarding byte 0x%02x (dez %d, char '%c')\n", level, b, b, c))
}

// DiscardASCII discards a byte assuming to be printable and prints it.
func DiscardASCII(c byte) {
	emit.LineCollect(fmt.Sprintf("%c", c))
}

// DiscardSilent discards a byte silently
func DiscardSilent(c byte) {
}

// ReadNextByte discards first byte in b and appends next read byte to b
func ReadNextByte(r io.Reader, b []byte) ([]byte, error) {
	DiscardByte(b[0])
	c := make([]byte, 1)
	n, err := io.ReadFull(r, c)
	if 1 == n {
		b = append(b[1:], c...)
	}
	return b, err
}

type tricer interface {
	io.ReadCloser
}

// TriceReceiver is a type definition
type TriceReceiver struct {
	tricer  // interface embedding
	active  bool
	trices  chan []byte
	buffers chan []byte
}

// NewTriceReceiver creates an instance of the common trice receiver part for a new receiver device
//
// After creation the embedded io.ReadCloser interface is in an opened state.
// The created Trice receiver is in inactive state.
func New(r io.ReadCloser) *TriceReceiver {
	p := &TriceReceiver{
		tricer:  r,
		active:  false,
		trices:  make(chan []byte),
		buffers: make(chan []byte),
	}
	return p
}

// start starts receiving of serial data
func (p *TriceReceiver) Start() {
	p.active = true
	go p.receiveBytes()
	p.receiveTrices()
}

// stop stops receiving of serial data
func (p *TriceReceiver) Stop() {
	p.active = false
}

func (p *TriceReceiver) receiveTrices() {
	defer p.Close()
	var err error
	var t, b []byte
	for true == p.active {
		select {
		case c := <-p.buffers:
			if len(c) > 0 {
				b = append(b, c...)
			}
		case t = <-p.trices:
			b, err = emit.Trice(t, b, id.List)
			if nil != err {
				fmt.Println("trice.Log error", err, t, b)
			}
		}
	}
}

/*
// readAtLeastBytes returns not when end of file is reached and no more bytes are written
func (p *TriceReceiver) readAtLeastBytes(count int) ([]byte, error) {
	buf := make([]byte, count) // the buffer size limits the read count
	var b []byte
	var n int
	var err error
	//start := time.Now()
	for len(b) < count {
		n, err = p.Read(buf)
		if nil != err {
			log.Fatal(err)
		}
		b = append(b, buf[:n]...)
		if count == len(b) { // https://stackoverflow.com/questions/45791241/correctly-measure-time-duration-in-go
			//fmt.Println("\tincoming:", b, "\t\t", time.Now().Sub(start).Nanoseconds(), "nanoseconds")
			return b, nil
		}
		buf = buf[n:]
		time.Sleep(100 * time.Millisecond) // needed for file reader
	}
	return b, nil
}
*/

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
func (p *TriceReceiver) readHeader() ([]byte, error) {
	//b, err := p.readAtLeastBytes(8)
	var n int
	var err error
	b := make([]byte, 8)
	n, err = io.ReadFull(p, b)

	if nil != err {
		b := b[:n]
		return b, err
	}
	for {
		b = decrypt(b)
		if true == evalHeader(b) {
			break
		}
		DiscardByte(b[0])
		b = encrypt(b)
		//x, err := p.readAtLeastBytes(1)
		c := make([]byte, 1)
		_, err = io.ReadFull(p, c)
		if nil != err {
			return b[1:], err
		}
		b = append(b[1:], c...) // try to sync
	}
	return b, nil
}

// receiving: ReadEndless expects a pointer to a filled COM port configuration
func (p *TriceReceiver) receiveBytes() {
	for true == p.active {
		b, err := p.readHeader()

		if nil != err {
			//fmt.Println("Could not read serial header: ", err)
			//log.Fatal(err)
			continue
		}

		if 0xeb == b[0] { // traceLog startbyte, no further data
			//fmt.Println("to trice channel:", b)
			p.trices <- b // send to process trace log channel

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
					//d, _ := p.readAtLeastBytes(2) // len of buffer (only one buffer)
					d := make([]byte, 2)
					n, err := io.ReadFull(p, d)
					if nil != err {
						fmt.Println("Could not read serial len: ", n, err)
						continue
					}
					len := int(binary.LittleEndian.Uint16(d[:2]))
					//s, _ := p.readAtLeastBytes(len + 1) // len is len-1 value
					s := make([]byte, len+1) // len is len-1 value
					m, err := io.ReadFull(p, s)
					if nil != err {
						fmt.Println("Could not read buffer: ", m, err)
						continue
					}
					b = append(b, d...) // len is redundant here and usable as check
					b = append(b, s...) // the buffer (string) data
					//log.Println("to buffer channel:", b)
					p.buffers <- b // send to process trace log channel
				}
			}
		} else {
			fmt.Println("Got unknown header on serial console. Discarding...", b)
		}
	}
}
