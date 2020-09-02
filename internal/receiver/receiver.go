// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package receiver provides trice receiver functionality.
// It uses a bytes.Reader for getting bytes and provides the received trices over a trice atoms channel
package receiver

import (
	"io"
)

const (
	// triceSize is the count of bytes in a byte stream used for one Trice.
	triceSize = 4

	// triceChannelCapacity is the max possible trice slice count hold in channel
	triceChannelCapacity = 1024

	// ignoredChannelCapacity is the max count of ignored bytes
	ignoredChannelCapacity = 1024

	// bytesBufferCapacity is the internal bufferered amount for sync package search.
	bytesBufferCapacity = 4096
)

var (

	// syncTrice is a trice emitted regularely by the target for making sure all gets in sync again after some disruption.
	syncTrice = []byte{0x16, 0x16, 0x16, 0x16}
)

// Trice is the bare Trice data type for a Trice atom.
// A Trice starts with zero or several Trice atoms with ID==0 carrying parts of the Trice data payload.
// The last Trice atom of a Trice contains the Trice ID!=0 and the last part of the data payload.
type Trice struct {
	ID    uint16  // 2^16 ^= more than 65500 different trice IDs possible
	Value [2]byte // max 2 byte data payload inside a TriceAtom
}

// TriceReceiver receives trices using io.Reader r and decodes them according to the expected coding.
// It provides a TriceAtomsReceiver interface.
// All recognized trice atoms as fetched are going as slices into the atoms channel.
// Not used read bytes are sent to the ignored channel. Theses bytes could be garbage after out of sync or some different protocol.
type TriceReceiver struct {
	Err0       error                     // if some error occured it is stored here
	Err1       error                     // if some error occured it is stored here
	r          io.Reader                 // interface embedding
	syncArray  [bytesBufferCapacity]byte // physical sync buffer
	syncBuffer []byte                    //  valid bytes inside syncArray
	atoms      chan []Trice              // The receivetod and unprocessed trice atoms are sent as slices to this channel.
	ignored    chan []byte               // The read bytes not usable for trice atom generation are sent as slices to this channel.
}

// TriceAtomsChannel provides a read channel for reading trice atoms.
// It is part of the TriceAtomsReceiver interface.
func (p *TriceReceiver) TriceAtomsChannel() <-chan []Trice {
	return p.atoms
}

// IgnoredBytesChannel provides a read channel for reading bytes the trice receiver ignored.
// It is part of the TriceAtomsReceiver interface.
func (p *TriceReceiver) IgnoredBytesChannel() <-chan []byte {
	return p.ignored
}
