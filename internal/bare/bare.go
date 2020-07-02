// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package bare reads bare items from differently encoded byte streams.
//
// To use it you need a to provide an inner byte reader like COM or FILE and information about the byte stream encoding.
// See baretrice_test.go for examples.
package bare

import (
	"bytes"
	"encoding/binary"
	"errors"
	"fmt"
	"io"
	"reflect"

	"github.com/rokath/trice/internal/emit"
	"github.com/rokath/trice/internal/global"
)

// encoding is used to enumerate input bytes stream encoding
type encoding int

const (
	// raw assumes byte stream source are raw trice data
	raw encoding = iota

	// wrapped assumes byte stream source are in wrap format
	//
	// The wrap format are 4 control bytes followed by 4 bytes BareItem
	// start (==0xC0), dest, src, crc BareItem
	// The crc is an exOr crc over bytes 0-2&4-7
	wrapped

	// RawXTEACrypted assumes byte stream source are XTEA encrypted raw trice data
	rawXTEACrypted

	// WrapXTEACrypted assumes byte stream source are XTEA encrypted wrapp trice data
	wrapXTEACrypted

	// bytesBufferCapacity is the internal bufferered amount for sync package search
	bytesBufferCapacity int = 4096
)

// Item is the raw trice data type
type Item struct {
	ID    uint16
	Value uint16
}

// Reader uses an inner Reader to read a raw byte stream.
type Reader struct {
	r  io.Reader // read from any byte source (inner reader)
	by []byte    // to hold bytes data for syncing
	e  encoding  // the way the byte stream is encoded
}

// NewReader generates a Bare instance to read from i
func NewReader(i io.Reader, enc string) (*Reader, error) {
	x := &Reader{}
	x.r = i
	x.by = make([]byte, 0, bytesBufferCapacity)
	switch enc {
	case "raw", "bare":
		x.e = raw
	case "wrapped", "wrap":
		x.e = wrapped
	case "rawXTEACrypted":
		x.e = rawXTEACrypted
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
	case raw:
		return p.readRaw(b)
	case wrapped:
		return p.readWrapped(b)
	case rawXTEACrypted:
		return p.readRawXTEACrypted(b)
	case wrapXTEACrypted:
		return p.readwrapXTEACrypted(b)
	}
	return 0, errors.New("unknown encoding")
}

// findSubSliceOffset returns offset of sub inside b or negative len(sub) if not found
func findSubSliceOffset(b, sub []byte) int {
	s := len(sub)
	if len(b) < s {
		return -s
	}
	for i := range b {
		if i <= len(b)-s && reflect.DeepEqual(b[i:i+s], sub) {
			return i
		}
	}
	return -s
}

func min(a, b int) int {
	if a < b {
		return a
	}
	return b
}

// readRaw uses inner reader p.r to read byte stream and assumes encoding 'raw' for interpretation.
func (p *Reader) readRaw(i []Item) (int, error) {
	leftovers := len(p.by) // byte buffered in bytes buffer
	var minBytes int       // byte count making an Item
	if leftovers < 4 {
		minBytes = 4 - leftovers
	} else {
		minBytes = 4
	}
	limit := cap(p.by)

	n, err := io.ReadAtLeast(p.r, p.by[leftovers:limit], minBytes) // read to have at least 4 bytes

	le := leftovers + n

	if le < 4 {
		return 0, err
	}

	p.by = p.by[:le] // set valid length
	o := findSubSliceOffset(p.by, []byte{0x16, 0x16, 0x16, 0x16})
	adjust := o % 4
	p.by = p.by[adjust:] // drop 1-3 bytes if out of
	itemsAvail := len(p.by) / 4
	itemCount := min(itemsAvail, len(i))
	i = i[:itemCount] // adjust destination size

	// now convert from bytes buffer into itemsmbuffer i
	buf := bytes.NewReader(p.by)
	err = binary.Read(buf, binary.LittleEndian, i) // target assumed to be little endian
	if err != nil {
		fmt.Println("binary.Read failed:", err)
	}
	p.by = p.by[4*itemCount:] // any leftovers possible

	return itemCount, nil

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
		if false == global.EvaluateWrap(p.by) {
			emit.DiscardByte(p.by[0])
			p.by = p.by[1:]
			continue
		}

		// at this point the first 8 bytes in b.by are a valid wrap
		p.by = p.by[4:] // discard control information
		k := i[:1]      // slice for one item (going then in underlying memory)

		// now convert from bytes buffer into item buffer k
		buf := bytes.NewReader(p.by)
		err = binary.Read(buf, binary.LittleEndian, k) // target assumed to be little endian
		if err != nil {
			fmt.Println("binary.Read failed:", err)
		}

		// manage
		p.by = p.by[4:] // remove 4 read bytes
		i = i[1:]       // "remove" read item, it still is inside underlying memory
		count++
	}

	return count, nil
}

// readRawXTEACrypted uses inner reader p.r to read byte stream and assumes encoding 'bareXTEACrypted' for interpretation.
func (p *Reader) readRawXTEACrypted(b []Item) (int, error) {
	return 0, nil
}

// readwrapXTEACrypted uses inner reader p.r to read byte stream and assumes encoding 'wrapXTEACrypted' for interpretation.
func (p *Reader) readwrapXTEACrypted(b []Item) (int, error) {
	return 0, nil
}
