// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package decoder provides several decoders for differently encoded trice streams.
package decoder

import (
	"fmt"
	"io"
)

var (
	wrapStartByte   byte = 0xEB
	senderAddress   byte = 0x80 // remote device
	receiverAddress byte = 0x81 // this device
)

// BareReaderFromWrap provides a Read method for bare data from wrapped data
type BareReaderFromWrap struct {
	in            io.Reader
	wrapStartByte byte
	localAddr     byte
	remoteAddr    byte
	nextData      []byte
	holdBuf       []byte
	bareBuf       []byte
	savedErr      error
}

// NewBareReaderFromWrap returns a pointer to a BareReaderFromWrap instance
// which is satisfying the io.Reader interface and using `in` as internal reader.
// It assumes wrap coded trices in the input byte stream.
// It uses the wrapper bytes for syncing and removes them silently.
func NewBareReaderFromWrap(in io.Reader) *BareReaderFromWrap {
	p := &BareReaderFromWrap{}
	p.in = in
	p.wrapStartByte = wrapStartByte
	p.localAddr = receiverAddress
	p.remoteAddr = senderAddress
	p.nextData = make([]byte, 4096)
	p.holdBuf = make([]byte, 0, 4096)
	p.bareBuf = make([]byte, 0, 4096)
	return p
}

// Read is the provided method for interface satisfying.
func (p *BareReaderFromWrap) Read(buf []byte) (int, error) {
	n, err := p.in.Read(p.nextData)
	p.holdBuf = append(p.holdBuf, p.nextData[:n]...)
	p.bareBuf = p.bareBuf[:0]
	for 8 <= len(p.holdBuf) {
		if false == p.evaluateWrap(p.holdBuf[:8]) {
			fmt.Println("ignoring", p.holdBuf[0])
			p.holdBuf = p.holdBuf[1:]
			continue // try to re-sync
		}
		p.bareBuf = append(p.bareBuf, p.holdBuf[4:8]...)
		p.holdBuf = p.holdBuf[8:]
	}
	count := copy(buf, p.bareBuf)

	// keep remaining bytes for next turn
	return count, err
}

// evaluateWrap checks if the wrap in b contains valid trice header data.
//
// It returns true on success, otherwise false.
func (p *BareReaderFromWrap) evaluateWrap(b []byte) (x bool) {
	if 8 == len(b) {
		x = p.wrapStartByte == b[0] && // start byte 0xEB
			b[1] == p.remoteAddr &&
			b[2] == p.localAddr &&
			b[3] == b[0]^b[1]^b[2]^b[4]^b[5]^b[6]^b[7] // crc8
	}
	return
}
