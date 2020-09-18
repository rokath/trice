// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package receiver provides trice receiver functionality.
// It uses a bytes.Reader for getting bytes and provides the received trices over a trice atoms channel
package receiver

import (
	"io"
	"time"
)

const (
	// triceSize is the count of bytes in a byte stream used for one Trice.
	triceSize = 4

	// bytesBufferCapacity is the internal bufferered amount for sync package search.
	receiveBufferCapacity = 64 * 1024
)

var (
	// Verbose gives more information on output if set. This variable is set outside this package.
	Verbose bool
)

// Trice is the bare Trice data type for a Trice atom.
// A trice starts with zero or several trice atoms with ID==0 carrying parts of the trice data payload.
// The last trice atom of a trice contains the trice ID!=0 and the last part of the data payload.
type Trice struct {
	ID    uint16 // 2^16 ^= more than 65500 different trice IDs possible
	Value uint16 // max 16 bit data payload inside a TriceAtom
}

// TriceReceiver receives trices using io.Reader r and decodes them according to the expected coding.
// It provides a TriceAtomsReceiver interface.
// All recognized trice atoms as fetched are going as slices into the atoms channel.
// Not used read bytes are sent to the ignored channel. Theses bytes could be garbage after out of sync or some different protocol.
type TriceReceiver struct {
	r         io.Reader    // interface embedding
	atomsCh   chan []Trice // The received and unprocessed trice atoms are sent as slices to this channel.
	ignoredCh chan []byte  // The read bytes not usable for trice atom generation are sent as slices to this channel.
	savedErr  error        // if some error occured it is stored here
}

// TriceAtomsChannel provides a read channel for reading trice atoms.
// It is part of the TriceAtomsReceiver interface.
func (p *TriceReceiver) TriceAtomsChannel() <-chan []Trice {
	return p.atomsCh
}

// IgnoredBytesChannel provides a read channel for reading bytes the trice receiver ignored.
// It is part of the TriceAtomsReceiver interface.
func (p *TriceReceiver) IgnoredBytesChannel() <-chan []byte {
	return p.ignoredCh
}

// ErrorFatal ends in osExit(1) if p.Err not nil.
func (p *TriceReceiver) ErrorFatal() {
	if io.EOF == p.savedErr {
		time.Sleep(200 * time.Millisecond)
		p.savedErr = nil
		return
	}
	if nil != p.savedErr {
		panic(p.savedErr)
	}
}
