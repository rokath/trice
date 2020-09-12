// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package receiver provides trice receiver functionality.
// It uses a bytes.Reader for getting bytes and provides the received trices over a trice atoms channel
package receiver

import (
	"fmt"
	"io"
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

	// syncTrice is a trice emitted regularely by the target for making sure all gets in sync again after some disruption.
	syncTrice = []byte{0x89, 0xab, 0xcd, 0xef} // idHi, idLo, daHi, daLo <- big endian is the network order, id transmitted first
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
	Err0       error        // if some error occured it is stored here
	Err1       error        // if some error occured it is stored here
	r          io.Reader    // interface embedding
	syncBuffer []byte       // valid bytes inside syncArray
	atomsCh    chan []Trice // The received and unprocessed trice atoms are sent as slices to this channel.
	ignoredCh  chan []byte  // The read bytes not usable for trice atom generation are sent as slices to this channel.
}

// String is the method for displaying the current TriceReceiver instance state.
func (p *TriceReceiver) String() string {
	s := fmt.Sprintf("syncBuffer= ")
	for _, n := range p.syncBuffer {
		s += fmt.Sprintf("%x ", n)
	}
	return s
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
