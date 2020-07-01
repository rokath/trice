// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package wrap reads 8 byte trice wraps and syncs internally if byte stream inconsistent
package wrap

import (
	"bytes"
	"encoding/binary"
	"errors"
	"fmt"
	"io"

	"github.com/rokath/trice/internal/receiver"
	"github.com/rokath/trice/internal/receiver/bare"
)

// encoding is used to enumerate input bytes stream encoding
type encoding int

const (
	// wrapped assumes byte stream source are in wrap format
	//
	// The wrap format are 4 control bytes followed by 4 bytes BareItem
	// start (==0xC0), dest, src, crc BareItem
	// The crc is an exOr crc over bytes 0-2&4-7
	wrapped encoding = iota

	// WrapXTEACrypted assumes byte stream source are XTEA encrypted wrapp trice data
	wrapXTEACrypted

	// bytesBufferCapacity is the internal bufferered amount for syncing
	bytesBufferCapacity int = 4096
)

// Item is the wrap trice data type
type Item struct {
	Start, Dest, Source, Crc byte
	BareV                    bare.Item
}

// Reader has a read method. Its reader uses an inner reader.
//
// The inner reader reads a raw byte stream.
type Reader struct {
	r  io.Reader // read from any byte source (inner reader)
	by []byte    // to hold bytes data for syncing
	e  encoding  // the way the byte stream is encoded
}

// NewReader generates a wrapReader instance to read from i
func NewReader(i io.Reader, enc string) (*Reader, error) {
	x := &Reader{}
	x.r = i
	x.by = make([]byte, 0, bytesBufferCapacity)
	switch enc {
	case "wrapped", "wrap":
		x.e = wrapped
	case "wrapXTEACrypted":
		x.e = wrapXTEACrypted
	default:
		return nil, errors.New("unknown encoding")
	}
	return x, nil
}

// Read uses inner reader p.r to read byte stream and encoding p.e for interpretation.
func (p *Reader) Read(b []Item) (int, error) {
	switch p.e {
	case wrapped:
		return p.readWrapped(b)
	case wrapXTEACrypted:
		return p.readwrapXTEACrypted(b)
	}
	return 0, errors.New("unknown encoding")
}

// evaluateWrap checks if the wrap in b contains valid header data.
//
// It returns true on success, otherwise false.
func evaluateWrap(b []byte) bool {
	x := 0xc0 == b[0] && // start byte - todo: cmd (|| 0xeb == b[0])
		0x60 == b[1] && // todo remAddr
		0x60 == b[2] && // todo locAddr
		b[0]^b[1]^b[2]^b[4]^b[5]^b[6]^b[7] == b[3] // crc8
	return x
}

// readWrapped uses inner reader p.r to read byte stream and assumes encoding 'wrapped' for interpretation.
func (p *Reader) readWrapped(i []Item) (int, error) {
	leftovers := len(p.by) // byte buffered in bytes buffer
	var minBytes int       // byte count making an Item
	if leftovers < 8 {
		minBytes = 8 - leftovers
	} else {
		minBytes = 8
	}
	limit := cap(p.by)

	n, err := io.ReadAtLeast(p.r, p.by[leftovers:limit], minBytes) // read to have at least 8 bytes

	le := leftovers + n

	if le < 8 {
		return 0, err
	}

	p.by = p.by[:le] // set valid length

	readCount := len(i)
	count := 0

	for len(p.by) >= 8 && count < readCount {
		if false == evaluateWrap(p.by) {
			receiver.DiscardByte(p.by[0])
			p.by = p.by[1:]
			continue
		}

		// at this point the first 8 bytes in b.by are a valid wrap
		k := i[:1] // slice for one item (going then in underlying memory)

		// now convert from bytes buffer into items buffer k
		buf := bytes.NewReader(p.by)
		err = binary.Read(buf, binary.LittleEndian, k) // target assumed to be little endian
		if err != nil {
			fmt.Println("binary.Read failed:", err)
		}

		// manage
		p.by = p.by[8:] // remove 8 read bytes
		i = i[1:]       // "remove" read item, it still is inside underlying memory
		count++
	}

	return count, nil
}

// readwrapXTEACrypted uses inner reader p.r to read byte stream and assumes encoding 'wrapXTEACrypted' for interpretation.
func (p *Reader) readwrapXTEACrypted(b []Item) (int, error) {
	return 0, nil
}
