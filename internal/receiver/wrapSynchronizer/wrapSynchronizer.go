// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package wrapsynchronizer reads 8 byte trice wraps and syncs internally if byte stream inconsistent
package wrapsynchronizer

import (
	"errors"
	"io"

	"github.com/rokath/trice/internal/receiver"
)

// WrapSync has a read method. It reader uses an inner reader.
//
// The inner reader reads a raw byte stream.
type WrapSync struct {
	r io.Reader // read from any byte source
}

// New generates an wrapSync instance
func New(input io.Reader) *WrapSync {
	ws := &WrapSync{} // instance
	ws.r = input      // inner reader
	return ws
}

// Read fills buff with an 8 byte wrap and returns 8, nil
//
// In case of invalid data Read discards bytes until a valid wrap arrives.
func (p *WrapSync) Read(b []byte) (int, error) {
	var err error
	if len(b) < 8 {
		return 0, errors.New("too short for wrap")
	}
	b = b[:8] // wrap size is max
	n, err := p.r.Read(b)
	if nil != err {
		return n, err
	}
	if true == evaluateWrap(b) {
		return 8, nil
	}

	// out of sync handling
	// The primary slice b we can only use to fill the underlying memory.
	// It is passed by value, so any changes to it will not reach the caller.
	// Therefore receiver.ReadNextByte cannot use b directly.
	buf := make([]byte, 8) // need to work on a separate slice
	copy(buf, b)
	for false == evaluateWrap(buf) {
		receiver.DiscardByte(buf[0])
		buf, err = receiver.ReadNextByte(p.r, buf)
		if nil != err {
			return 0, err
		}
	}
	copy(b, buf) // bring it to the primary slice
	return 8, err
}

// evaluateWrap checks if the wrap in b contains valid header data.
//
// In returns true on success, otherwise false.
func evaluateWrap(b []byte) bool {
	x := 8 == len(b) &&
		(0xc0 == b[0] || 0xeb == b[0]) && // start byte
		0x60 == b[1] && // todo remAddr
		0x60 == b[2] && // todo locAddr
		b[0]^b[1]^b[2]^b[4]^b[5]^b[6]^b[7] == b[3] // crc8
	return x
}
