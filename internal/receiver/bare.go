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
	"time"
)

// BareTriceReceiver is the TriceReceiver data struct for receiving bare trice data.
type BareTriceReceiver struct {
	TriceReceiver // common components
	syncBuffer    []byte
}

// NewTricesfromBare creates a TriceReceiver using r as internal reader.
// It assumes bare coded trices in the byte stream.
// It creates a trices channel and and sends the received trices to it.
// If an out of sync condition is detected some bytes from the beginning are ignored.
// The ignored bytes are send to an also created ignored channel.
// The sync condition is assumed generally. From time to time (aka every second)
// a sync trice should be inside the byte stream. This sync trice must be on a
// multiple of triceSice offset. If not, the appropriate count of bytes is ignored.
func NewTricesfromBare(r io.Reader) *BareTriceReceiver {
	p := &BareTriceReceiver{}
	p.r = r
	p.atomsCh = make(chan []Trice)
	p.ignoredCh = make(chan []byte)
	p.syncBuffer = make([]byte, 0, 10000)
	go func() {
		for {
			time.Sleep(1 * time.Millisecond) // todo: trigger from fileWatcher
			p.readBare()
		}
	}()
	return p
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
// const (
// 	syncID = 0x89ab // 35243
// 	syncDa = 0xcdef // 52719
//
// 	syncIDHi = 0x89 // 137
// 	syncIDLo = 0xab // 171
// 	syncDaHi = 0xcd // 205
// 	syncDaLo = 0xef // 239
//
// 	forbIddenID0  = 0xabcd // If an ID=abcd (43981) is detected -> out of sync
// 	forbIddenID1  = 0xcdef // If an ID=cdef (52719) is detected -> out of sync
// 	forbiddenIDHi = 0xef   // If an IH=ef     (239) is detected -> out of sync
// 	forbiddenIDLo = 0x89   // If an IL=  89   (137) is detected -> out of sync
// )

// readBare uses inner reader p.r to read the byte stream and assumes encoding 'raw' (=='bare') for interpretation.
// It sends a number of Trice items to the internal 'atoms' channel, any ignored bytes to
// the internal 'ignored' channel and stores internally an error code in that case.
// If no sync trice (sync point) is visible the sync condition is assumed and no bytes are ignored.
// It looks for the first sync point inside the internally read byte slice and ignores ALL bytes
// if the sync is not on a triceSize offset.
func (p *BareTriceReceiver) readBare() {
	p.ErrorFatal()
	var n int
	rb := make([]byte, receiveBufferCapacity)
	n, p.savedErr = p.r.Read(rb)
	p.syncBuffer = append(p.syncBuffer, rb[:n]...) // merge with leftovers

	for len(p.syncBuffer) >= triceSize {
		var syncLen int
		o := findSyncTriceOffset(p.syncBuffer)
		if o < 0 { // not found
			//return // wait
			syncLen = (len(p.syncBuffer) >> 2) << 2 // assume sync state
		} else { // found
			syncLen = o + 4
			adjust := o % triceSize // expect to be 0, but can be 0...3
			if 0 != adjust {        // out of sync
				if Verbose {
					fmt.Printf("############################# Out of sync at sync point offset %d ", o)
					fmt.Println("ignoring", p.syncBuffer[:syncLen])
				}
				// Even there are possibly some full trices between 0 and o ignore them all to generate less garbage.
				ig := make([]byte, syncLen)
				copy(ig, p.syncBuffer[:syncLen])
				p.syncBuffer = p.syncBuffer[syncLen:] // drop 1 to o bytes plus 4 bytes of the detected sync trice
				p.ignoredCh <- ig                     // send dropped bytes as slice to ignored channel and include the detected sync trice
				continue
			}
		}

		// convert into Trice slice
		cv := make([]byte, syncLen)
		copy(cv, p.syncBuffer)
		//log.Println("syncbuffer:", syncLen, cv)
		r := bytes.NewReader(cv)
		atomsAvailCount := len(cv) / triceSize
		atomsAvail := make([]Trice, atomsAvailCount)              // needs to be created on each loop fresh because it is send away thru a channel
		p.savedErr = binary.Read(r, binary.BigEndian, atomsAvail) // received data are big endian

		// check atoms in buf for wrong sync
		//i := p.syncCheck(atomsAvail)
		//if 0 <= i { // out of sync
		//	if Verbose {
		//		fmt.Printf("Sync issue, trice atom %d has unexpected id=%x. ", i, atomsAvail[i].ID)
		//		fmt.Println(p)
		//	}
		//	ign := (i + 1) * triceSize
		//	p.ignoredCh <- p.syncBuffer[:ign] // send dropped byte (as slice) to ignored channel
		//	p.syncBuffer = p.syncBuffer[ign:] // drop 1 byte
		//	return                            // try to read more
		//}

		p.atomsCh <- atomsAvail               // send trices
		p.syncBuffer = p.syncBuffer[syncLen:] // remove processed bytes
	}
}

// findSyncTriceOffset returns offset of syncTrice inside slice b or -1 if not found.
func findSyncTriceOffset(b []byte) int {
	for i := range b { // network order is big endian:   idHi, idLo, daHi, daLo
		if i <= len(b)-4 && bytes.Equal(b[i:i+4], []byte{0x89, 0xab, 0xcd, 0xef}) {
			return i
		}
	}
	return -1
}

// // syncCheck returns -1 on success. On failure it returns the appropriate index number.
// func (p *BareTriceReceiver) syncCheck(atoms []Trice) int {
// 	for i, a := range atoms {
// 		ih := byte(a.ID >> 8)
// 		il := byte(a.ID)
// 		if forbIddenID0 == a.ID || forbIddenID1 == a.ID || forbiddenIDHi == ih || forbiddenIDLo == il {
// 			return i
// 		}
// 	}
// 	return -1
// }
