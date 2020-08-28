// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package triceemit is responsible for generating a string slice for each line.
// The substrings are optionally prefix, timestamp, several content substrings and postfix.
// Each substring can contain its own color channel as prefix ("col:").
// The colors are converted later inside the disp.Print() function.

package triceemit

import (
	"bytes"
	"encoding/binary"
	"io"
	"log"
	"path/filepath"
	"reflect"
	"runtime"
)

var (
	// bytesBufferCapacity is the internal bufferered amount for sync package search.
	bytesBufferCapacity = 64

	// syncTrice is a trice emitted regularely by the target for making sure all gets in sync again after some disruption.
	syncTrice = []byte{0x16, 0x16, 0x16, 0x16}
)

// TriceReceiver receives trices using io.Reader r and decodes them according to the expected coding.
// It provides a TriceAtomsReceiver interface.
// All recognized trice atoms as fetched are going as slices into the atoms channel.
// Not used read bytes are sent to the ignored channel. Theses bytes could be garbage after out of sync or some different protocol.
type TriceReceiver struct {
	Err        error        // if some error occured it is stored here
	r          io.Reader    // interface embedding
	syncbuffer []byte       // to hold read bytes for syncing
	atoms      chan []Trice // The received and unprocessed trice atoms are sent as slices to this channel.
	ignored    chan []byte  // The read bytes not usable for trice atom generation are sent as slices to this channel.
}

// TriceAtomsChannel provides a read channel for reading trice atoms.
func (p *TriceReceiver) TriceAtomsChannel() <-chan []Trice {
	return p.atoms
}

// IgnoredBytesChannel provides a read channel for reading bytes the trice receiver ignored.
func (p *TriceReceiver) IgnoredBytesChannel() <-chan []byte {
	return p.ignored
}

// NewTriceReceiverfromBare creates a TriceReceiver using r as internal reader.
// It assumes bare coded trices in the byte stream.
// It creates a trices channel and and sends the received trices to it.
// If an out of sync condition is detected 1 to (triceSize-1) bytes are ignored.
// The ignored bytes are send to an also created ignored channel.
// The sync condition is assumed generally. From time to time (aka every second)
// a sync trice should be inside the byte stream. This sync trice must be on a
// multiple of triceSice offset. If not, the appropriate count of bytes is ignored.
func NewTriceReceiverfromBare(r io.Reader) *TriceReceiver {
	p := &TriceReceiver{}
	p.r = r
	p.syncbuffer = make([]byte, 0, bytesBufferCapacity)
	p.atoms = make(chan []Trice, triceChannelCapacity)
	p.ignored = make(chan []byte, ignoredChannelCapacity)
	go func() {
		for {
			if io.EOF == p.Err {
				return
			}
			p.readRaw()
		}
	}()
	return p
}

// findSubSliceOffset returns offset of slice sub inside slice b or negative len(sub) if not found.
// This is a helper function to find sync points inside readRaw
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

// min returns the smaller int from a and b
func min(a, b int) int {
	if a < b {
		return a
	}
	return b
}

// ErrorFatal ends in osExit(1) if p.Err not nil.
func (p *TriceReceiver) ErrorFatal() {
	if nil == p.Err {
		return
	}
	_, file, line, _ := runtime.Caller(1)
	log.Fatal(p.Err, filepath.Base(file), line)
}

// readRaw uses inner reader p.r to read byte stream and assumes encoding 'raw' (=='bare') for interpretation.
// It sends a number of Trice items to the internal 'atoms' channel,
// any ignored bytes to the internal 'ignored' channel and stores internally an error code.
// It looks for a sync point inside the internally read byte slice and ignores 1 to(triceSize-1) bytes
// if the sync is not on a triceSize offset. If no sync point is found sync is assumed per default.
func (p *TriceReceiver) readRaw() {
	p.ErrorFatal()
	leftovers := len(p.syncbuffer) // bytes buffered in bytes buffer from last call
	var minBytes int               // needed additional byte count making a Trice
	if leftovers < triceSize {
		minBytes = triceSize - leftovers
	} else {
		minBytes = triceSize
	}
	limit := cap(p.syncbuffer)
	var n int
	n, p.Err = io.ReadAtLeast(p.r, p.syncbuffer[leftovers:limit], minBytes) // read to have at least triceSize bytes
	le := leftovers + n                                                     // the valid len inside p.by
	if le < triceSize {                                                     // got not the minimum amount of expected bytes
		return // assuming o.EOF == p.err
	}
	p.syncbuffer = p.syncbuffer[:le]                 // set valid length
	o := findSubSliceOffset(p.syncbuffer, syncTrice) // look for a sync point
	adjust := o % triceSize                          // expect to be 0
	if 0 != adjust {                                 // out of sync
		p.ignored <- p.syncbuffer[:adjust]   // send dropped bytes to ignored channel
		p.syncbuffer = p.syncbuffer[adjust:] // drop 1 to (triceSize-1) bytes
	}
	atomsAvail := len(p.syncbuffer) / triceSize
	atoms := make([]Trice, atomsAvail)
	// now convert from bytes slice into Trice slice t
	buf := bytes.NewReader(p.syncbuffer)
	p.Err = binary.Read(buf, binary.LittleEndian, atoms) // target assumed to be little endian
	p.syncbuffer = p.syncbuffer[triceSize*atomsAvail:]   // any leftover count from 1 to (triceSize-1) is possible
	p.atoms <- atoms                                     // send trices
}
