// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package bareSynchronizer reads 4 byte bare trices and syncs internally if byte stream inconsistent.
//
// The sychronisation is possible by detecting a sequence of 4 bytes equal 0x16161616.
// 0x16161616 is a sync trice: `#define TRICE_RTT_SYNC do{ TRICE16_1( Id(5654), "%d\b\b\b\b", 5654 ); }while(0)`.
// Because 5654=0x1616 is a reserved TRICE ID and the bare trice byte stream has an ID every 2 bytes
// it is impossible to have 0x16161616 inside the bare trice byte stream despite of the sync message.
// On a Read the bare bareSynchronizer tries to get a fair amount of bytes and scans them for 0x16161616.
// If the found 0x16161616 is not on a 4 byte aligned offset, the oldest 1-3 bytes are discarded to adjust the offset.
// If the bareSynchronizer does not find a 0x16161616 sequence it assumes to be in sync.
// The target should send sync trice messages cyclically.
// The bareSynchronizer does not deliver the 0x16161616 sync packages.
package bareSynchronizer

import (
	"errors"
	"io"
)

// BareSync has a read method. It reader uses an inner reader.
//
// The inner reader reads a raw byte stream.
type BareSync struct {
	r io.Reader // read from any byte source
	b []byte    // static slice for buffering
}

// New generates an BareSync instance
func New(input io.Reader) *BareSync {
	bs := &BareSync{}            // instance
	bs.b = make([]byte, 0, 1024) // static buffer with len 0 & capacity
	bs.r = input                 // inner reader to use
	return bs
}

// Read fills buf with a 4 byte bare trice and returns (4, nil) normally.
//
// It tries to find sync frames in advance and adjusts to it.
// More details in the package description.
func (p *BareSync) Read(buf []byte) (int, error) {
	var err error
	if len(buf) < 4 {
		return 0, errors.New("too short")
	}
	if 4 <= len(p.b) { // more than one bare trice in intermediate buffer
		copy(buf, p.b[:4]) // take first
		p.b = p.b[4:]      // remove
		return 4, nil      // done
	}
	leftovers := len(p.b)                           // 1,2 or 3
	p.b = p.b[:1024+leftovers]                      // extend len
	n, err := io.ReadAtLeast(p.r, p.b, 4-leftovers) // read to have at least 4 bytes
	if nil != err {
		return 0, err
	}
	p.b = p.b[:n] // limitate len to read bytes
	p.b = bareSunc(p.b)
	copy(buf, p.b[:4]) // take first
	p.b = p.b[4:]      // remove
	return 4, nil      // done
}

func bareSunc(b []byte) []byte {
	// todo: parse for 0x16161616
	return b
}
