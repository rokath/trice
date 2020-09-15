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
)

///////////////////////////////////////////////////////////////////////////////////////////////////
// dynamic debug helper
//
type bytesViewer struct {
	r io.Reader
}

func newBytesViewer(o io.Reader) (i io.Reader) {
	return &bytesViewer{o}
}

func (p *bytesViewer) Read(b []byte) (n int, e error) {
	n, e = p.r.Read(b)
	if 0 != n {
		fmt.Println("bytesViewer:", e, n, b[:n])
	}
	return
}

//
///////////////////////////////////////////////////////////////////////////////////////////////////

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
	p.r = newBytesViewer(r) // dynamic debug helper

	p.atomsCh = make(chan []Trice)  //triceChannelCapacity)
	p.ignoredCh = make(chan []byte) //, ignoredChannelCapacity)
	go func() {
		for {
			if io.EOF != p.Err0 { // for testing and file reading, p.Err0 is cleared on file watcher event.
				p.readRaw()
			}
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
	syncID = 0x89ab // 35243
	syncDa = 0xcdef // 52719

	syncIDHi = 0x89 // 137
	syncIDLo = 0xab // 171
	syncDaHi = 0xcd // 205
	syncDaLo = 0xef // 239

	forbIddenID0  = 0xabcd // If an ID=abcd (43981) is detected -> out of sync
	forbIddenID1  = 0xcdef // If an ID=cdef (52719) is detected -> out of sync
	forbiddenIDHi = 0xef   // If an IH=ef     (239) is detected -> out of sync
	forbiddenIDLo = 0x89   // If an IL=  89   (137) is detected -> out of sync
)

// syncCheck returns -1 on success. On failure it returns the appropriate index number.
func (p *TriceReceiver) syncCheck(atoms []Trice) int {
	for i, a := range atoms {
		ih := byte(a.ID >> 8)
		il := byte(a.ID)
		if forbIddenID0 == a.ID || forbIddenID1 == a.ID || forbiddenIDHi == ih || forbiddenIDLo == il {
			return i
		}
	}
	return -1
}

// readRaw uses inner reader p.r to read byte stream and assumes encoding 'raw' (=='bare') for interpretation.
// It sends a number of Trice items to the internal 'atoms' channel,
// any ignored bytes to the internal 'ignored' channel and stores internally an error code.
// It looks for a sync point inside the internally read byte slice and ignores 1 to(triceSize-1) bytes
// if the sync is not on a triceSize offset. If no sync point is found sync is assumed per default.
func (p *TriceReceiver) readRaw() {
	p.ErrorFatal()

	leftovers := len(p.syncBuffer) // bytes buffered from last call

	// needed additional byte count making a least one trice
	var minBytes int
	if leftovers < triceSize {
		minBytes = triceSize - leftovers
	} else {
		minBytes = triceSize
	}

	// read to have at least triceSize bytes
	var n int
	receiveBuffer := make([]byte, receiveBufferCapacity)
	n, p.Err0 = io.ReadAtLeast(p.r, receiveBuffer, minBytes)
	receiveBuffer = receiveBuffer[:n] // set valid length

	p.syncBuffer = append(p.syncBuffer, receiveBuffer...) // merge
	if len(p.syncBuffer) < triceSize {                    // got not the minimum amount of expected bytes
		fmt.Println("Unexpected", p.Err0)
		// p.Err0 = nil // clear error
		return // assuming o.EOF == p.err
	}

	// look for a sync point
	o := findSubSliceOffset(p.syncBuffer, syncTrice)
	if o < 0 { // wait for more data
		return
	}
	//adjust := o % triceSize // expect to be 0, can be 0...3

	if 0 != o%triceSize { // out of sync
		if Verbose {
			fmt.Printf("############################# Out of sync: o=%d ", o)
			fmt.Println(p)
		}
		p.ignoredCh <- p.syncBuffer[:o] // send dropped bytes as slice to ignored channel
		p.syncBuffer = p.syncBuffer[o:] // drop 1 to (triceSize-1) bytes
	}

	// convert from syncBuffer into Trice slice
	atomsAvailCount := len(p.syncBuffer) / triceSize
	if 0 == atomsAvailCount {
		fmt.Println("++++++++++++++++++++++++++++ convert from syncBuffer into Trice slice, try to read more")
		fmt.Println(p)
		return // try to read more
	}
	atomsAvail := make([]Trice, atomsAvailCount) ////////////////////////////////////// TODO: avoid make here
	buf := bytes.NewReader(p.syncBuffer)
	p.Err1 = binary.Read(buf, binary.BigEndian, atomsAvail) // target received data are big endian

	// check atoms in buf for wrong sync
	i := p.syncCheck(atomsAvail)
	if 0 <= i { // out of sync
		if Verbose {
			fmt.Printf("Sync issue, trice atom %d has unexpected id=%x. ", i, atomsAvail[i].ID)
			fmt.Println(p)
		}
		ign := i * triceSize
		p.ignoredCh <- p.syncBuffer[:ign] // send dropped byte (as slice) to ignored channel
		p.syncBuffer = p.syncBuffer[ign:] // drop 1 byte
		return                            // try to read more
	}

	p.syncBuffer = p.syncBuffer[triceSize*atomsAvailCount:] // any leftover count from 1 to (triceSize-1) is possible
	p.atomsCh <- atomsAvail                                 // send trices
}

// findSubSliceOffset returns offset of slice sub inside slice b or negative len(sub) if not found.
// This is a helper function to find sync points inside readRaw
func findSubSliceOffset(b, sub []byte) int {
	s := len(sub)
	if len(b) < s { // b is too small
		return -s
	}
	for i := range b {
		if i <= len(b)-s && reflect.DeepEqual(b[i:i+s], sub) {
			return i
		}
	}
	return -s
}
