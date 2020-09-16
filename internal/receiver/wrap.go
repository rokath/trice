// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package wrap provides a Reader for wrapped trice data
//
// It uses any byte reader and additional encoding information.
// It reads trice wraps and syncs internally if byte stream inconsistent.
// Discarded Bytes are transferred to function emit.DiscardByte().
package receiver

import (
	"io"
	"time"
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

// WrapTriceReceiver is the TriceReceiver data struct for receiving wrapped trice data.
type WrapTriceReceiver struct {
	TriceReceiver
}

// NewTricesfromWrap creates a TriceReceiver using r as internal reader.
// It assumes wrap coded trices in the byte stream.
// It creates a trices channel and and sends the received trices to it.
// If an out of sync condition is detected some bytes from the beginning are ignored.
// The ignored bytes are send to an also created ignored channel.
func NewTricesfromWrap(r io.Reader) *WrapTriceReceiver {
	p := &WrapTriceReceiver{}
	p.r = r // newBytesViewer(r) // dynamic debug helper

	p.atomsCh = make(chan []Trice)  //triceChannelCapacity)
	p.ignoredCh = make(chan []byte) //, ignoredChannelCapacity)
	go func() {
		for {
			time.Sleep(100 * time.Millisecond) // todo: trigger from fileWatcher
			p.readWrap()
		}
	}()
	return p
}

func (p *WrapTriceReceiver) readWrap() {

}

/*
// Item is the wrap trice data type
type Item struct {
	Start, Dest, Source, Crc byte
	BareV                    bare.Item
}

// readWrapped uses inner reader p.r to read byte stream and assumes encoding 'wrapped' for interpretation.
func (p *TriceReceiver) readWrapped(i []Item) (int, error) {
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

// readwrapXTEACrypted uses inner reader p.r to read byte stream and assumes encoding 'wrapXTEACrypted' for interpretation.
func (p *Reader) readwrapXTEACrypted(b []Item) (int, error) {
	return 0, nil
}
*/
