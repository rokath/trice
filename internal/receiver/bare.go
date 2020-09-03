// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package receiver provides trice receiver functionality.
// It uses a bytes.Reader for getting bytes and provides the received trices over a trice atoms channel

package receiver

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"io"
	"reflect"
	"time"
)

type bytesViewer struct {
	r io.Reader
}

func newBytesViewer(o io.Reader) (i io.Reader) {
	return &bytesViewer{o}
}

func (p *bytesViewer) Read(b []byte) (n int, e error) {
	n, e = p.r.Read(b)
	fmt.Println("bytesViewer:", e, n, b[:n])
	return
}

// NewTricesfromBare creates a TriceReceiver using r as internal reader.
// It assumes bare coded trices in the byte stream.
// It creates a trices channel and and sends the received trices to it.
// If an out of sync condition is detected 1 to (triceSize-1) bytes are ignored.
// The ignored bytes are send to an also created ignored channel.
// The sync condition is assumed generally. From time to time (aka every second)
// a sync trice should be inside the byte stream. This sync trice must be on a
// multiple of triceSice offset. If not, the appropriate count of bytes is ignored.
func NewTricesfromBare(r io.Reader) *TriceReceiver {
	p := &TriceReceiver{}
	p.r = r // newBytesViewer(r)

	p.atoms = make(chan []Trice, triceChannelCapacity)
	p.ignored = make(chan []byte, ignoredChannelCapacity)
	go func() {
		for {
			if io.EOF == p.Err0 {
				return
			}
			p.readRaw()
		}
	}()
	return p
}

// ErrorFatal ends in osExit(1) if p.Err not nil.
func (p *TriceReceiver) ErrorFatal() {
	if nil != p.Err0 {
		panic(p.Err0)
	}
	if nil != p.Err1 {
		panic(p.Err1)
	}
}

// syncSyncBuffer checks p.syncBuffer for invalid patterns:
// Sync package is IDDA=89abcdef
//
// To avoid wrong syncing these ID's are excluded: nn89, abcd, cdef, efnn (514 pieces)
//
// Possible:    IH IL DH DL IH IL DH DL IH IL DH DL (1 right)
//              xx xx xx xx nn 89 ab cd ef nn xx xx -> avoid with ID!=nn89, ID!=efnn
//
// Possible:    IH IL DH DL IH IL DH DL IH IL DH DL (2 right)
//              xx xx xx xx xx nn 89 ab cd ef nn xx -> avoid with ID!=cdef
//
// Possible:    IH IL DH DL IH IL DH DL IH IL DH DL (3 right)
//              xx xx xx xx xx xx nn 89 ab cd ef nn -> avoid with ID!=abcd
//
// Sync packet: IH IL DH DL IH IL DH DL IH IL DH DL
//              xx xx xx nn 89 ab cd ef nn xx xx xx -> use ID=89ab with DA=cdef as sync packet
//
//  Possible:   IH IL DH DL IH IL DH DL IH IL DH DL (1 left)
//              xx xx xx 89 ab cd ef xx xx xx xx xx -> avoid with ID!=abcd
//
//  Possible:   IH IL DH DL IH IL DH DL IH IL DH DL (2 left)
//              xx xx 89 ab cd ef xx xx xx xx xx xx -> avoid with ID!=cdef
//
//  Possible:   IH IL DH DL IH IL DH DL IH IL DH DL (3 left)
//              xx 89 ab cd ef xx xx xx xx xx xx xx ->  avoid with ID!=nn89, ID!=efnn
//
// If an ID=89ab with DA!=cdef is detected -> out of sync!
// If an IH=ef is detected -> out of sync
// If an IL=89 is detected -> out of sync
// If an ID=abcd is detected -> out of sync
// If an ID=cdef is detected -> out of sync
const (
	syncID = 0x89ab
	syncDa = 0xcdef

	syncIDHi = 0x89
	syncIDLo = 0xab
	syncDaHi = 0xcd
	syncDaLo = 0xef

	forbIddenID0  = 0xabcd // If an ID=abcd is detected -> out of sync
	forbIddenID1  = 0xcdef // If an ID=cdef is detected -> out of sync
	forbiddenIDHi = 0xef   // If an IH=ef is detected -> out of sync
	forbiddenIDLo = 0x89   // If an IL=89 is detected -> out of sync
)

func (p *TriceReceiver) syncCheck(atoms []Trice) bool {
	for _, a := range atoms {
		ih := byte(a.ID >> 8)
		il := byte(a.ID)
		if forbIddenID0 == a.ID || forbIddenID1 == a.ID || forbiddenIDHi == ih || forbiddenIDLo == il {
			fmt.Println("sync issue, unexpected id, ih, il, ignoring first byte and retrying...: ", a.ID, ih, il)
			p.ignored <- p.syncBuffer[:1]   // send dropped byte to ignored channel
			p.syncBuffer = p.syncBuffer[1:] // drop 1 to (triceSize-1) bytes
			return false
		}
	}
	return true
}

// readRaw uses inner reader p.r to read byte stream and assumes encoding 'raw' (=='bare') for interpretation.
// It sends a number of Trice items to the internal 'atoms' channel,
// any ignored bytes to the internal 'ignored' channel and stores internally an error code.
// It looks for a sync point inside the internally read byte slice and ignores 1 to(triceSize-1) bytes
// if the sync is not on a triceSize offset. If no sync point is found sync is assumed per default.
func (p *TriceReceiver) readRaw() {
	p.ErrorFatal()

	// move any leftovers (1-3 bytes) to start of syncArray
	leftovers := len(p.syncBuffer) // bytes buffered in bytes buffer from last call
	copy(p.syncArray[:], p.syncBuffer)

	// set reception size
	p.syncBuffer = p.syncArray[leftovers:bytesBufferCapacity]

	// needed additional byte count making a least one Trice
	var minBytes int
	if leftovers < triceSize {
		minBytes = triceSize - leftovers
	} else {
		minBytes = triceSize
	}

	// read to have at least triceSize bytes
	var n int
	n, p.Err0 = io.ReadAtLeast(p.r, p.syncBuffer, minBytes)

	// the valid len inside syncBuffer
	le := leftovers + n
	if le < triceSize { // got not the minimum amount of expected bytes
		return // assuming o.EOF == p.err
	}
	p.syncBuffer = p.syncArray[:le] // set valid length

	// look for a sync point
	o := findSubSliceOffset(p.syncBuffer, syncTrice)
	adjust := o % triceSize // expect to be 0
	if 0 != adjust {        // out of sync
		p.ignored <- p.syncBuffer[:adjust]   // send dropped bytes to ignored channel
		p.syncBuffer = p.syncBuffer[adjust:] // drop 1 to (triceSize-1) bytes
	}

retrySync:
	// convert from syncBuffer into Trice slice
	atomsAvail := len(p.syncBuffer) / triceSize
	atoms := make([]Trice, atomsAvail) ////////////////////////////////////// TODO: avoid make here
	buf := bytes.NewReader(p.syncBuffer)

	// consider Big.Endian
	//p.Err1 = binary.Read(buf, binary.LittleEndian, atoms) // target assumed to be little endian
	p.Err1 = binary.Read(buf, binary.BigEndian, atoms) // target assumed to be big endian

	// check atoms in buf for wrong sync
	if false == p.syncCheck(atoms) {
		time.Sleep(3 * time.Second)
		goto retrySync
	}

	p.syncBuffer = p.syncBuffer[triceSize*atomsAvail:] // any leftover count from 1 to (triceSize-1) is possible
	p.atoms <- atoms                                   // send trices
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
