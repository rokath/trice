// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package bareSynchronizer reads 4 byte bare trices and syncs internally if byte stream inconsistent.
//
// The sychronisation is possible by detecting a sequence of 4 bytes equal 0x16161616.
// 0x16161616 is a sync trice: `#define TRICE_RTT_SYNC do{ TRICE16_1( Id(5654), "%d\b\b\b\b", 5654 ); }while(0)`.
// Because 5654=0x1616 is a reserved TRICE ID and the bare trice sequence has an ID every 2 bytes 
// it is impossible to have 0x16161616 inside the bare trice byte stream despite of the sync message.
// On a Read the bare bareSynchronizer tries to get a fair amount of bytes and scans them for 0x16161616.
// If the found 0x16161616 is not on a 4 byte aligned offset, the oldest 1-3 bytes are discarded.
// If the bareSynchronizer does not find a 0x16161616 sequence it assumes to be in sync.
// The target should send sync trice messages cyclically.
package bareSynchronizer

import (
	"errors"
	"io"
	"time"

	"github.com/rokath/trice/internal/receiver"
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
	bs := &BareSync{}         // instance
	b = make([]byte, 0, 1024) // static buffer with len 0 & capacity
	bs.r = input              // inner reader to use
	return bs
}

// Read fills buff with a 4 byte bare trice and returns 4, nil normally.
//
// In case of invalid data Read discards bytes until a valid wrap arrives.
func (p *BareSync) Read(buf []byte) (int, error) {
	var err error
	if len(buf) < 4 {
		return 0, errors.New("too short")
	}
	if 4 <= len(p.b) {
		copy(buf, p.b[:4])
		p.b = p.b[4:]
		return 4, nil
	}
	leftovers := len(p.b // 1,2 or 3
	p.b = p.b[:1024+leftovers] // extend len
	n, err := io.ReadAtLeast(p.r, p.b, max(4,leftovers))
	if io.EOF
	if( nil != err ){

	}
	for false == evaluateBare(b) {
		b, err = receiver.ReadNextByte(p.r, b)
		if io.EOF == err {
			time.Sleep(10 * time.Millisecond)
			continue
		}
		if nil != err {
			return 0, err
		}
	}
	copy(buf, b)
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
