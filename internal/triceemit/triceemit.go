// + build x
// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package emit is responsible for generating a string slice for each line.
// The substrings are optionally prefix, timestamp, several content substrings and postfix.
// Each substring can contain its own color channel as prefix ("col:").
// The colors are converted later inside the disp.Print() function.

package triceemit

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"io"
	"reflect"
	"strings"

	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/pkg/lib"
)

const (
	// triceSize is the count of bytes in a byte stream used for one Trice.
	triceSize = 4

	// triceChannelCapacity is the max possible trice slice count hold in channel
	triceChannelCapacity = 100

	// ignoredChannelCapacity is the max count of ignored bytes
	ignoredChannelCapacity = 1000
)

var (
	// bytesBufferCapacity is the internal bufferered amount for sync package search.
	bytesBufferCapacity = 4096

	// syncTrice is a trice emitted regularely by the target for making sure all gets in sync again after some disruption.
	syncTrice = []byte{0x16, 0x16, 0x16, 0x16}

	// Prefix is a (configurable) string added to each line at the beginning
	Prefix = ""

	// Postfix is a (configurable) string added to each line end
	Postfix = "\n"
)

// Trice is the bare Trice data type for a Trice atom.
// A Trice starts with zero or several Trice atoms with ID==0 carrying parts of the Trice data payload.
// The last Trice atom of a Trice contains the Trice ID!=0 and the last part of the data payload.
type Trice struct {
	ID    uint16  // 2^16 ^= more than 65500 different trice IDs possible
	Value [2]byte // max 2 byte data payload inside a TriceAtom
}

// TriceReceiver receives trices using io.Reader r and decodes them according to the expected coding.
// It provides a TriceReceiver interface.
// All recognized trice atoms as fetched are going as slices into the atoms channel.
// Not used read bytes are sent to the ignored channel. Theses bytes could be garbage after out of sync or some different protocol.
//  // TriceReceiverI is the interface a trice receicer has to provide for a trice interpreter.
//  // The provided channels are read only channels
//  type TriceReceiverI interface {
//  	TriceAtomsChannel() <-chan []trice
//  	IgnoredBytesChannel() <-chan []byte
//  }
type TriceReceiver struct {
	err        error        // if some error occured it is stored here
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
			if nil != p.err {
				return // stop in case of an error
			}
			p.readRaw()
		}
	}()
	return p
}

// TriceInterpreter uses the 2 TriceReceiver channels and global settings to compose a complete log line as one string.
// The string is transferred using the io.StringWriter interface.
type TriceInterpreter struct {
	// If some error occured it is stored here. This makes error handling more handy.
	err error

	// This is the receive channel for trice atomsChannel.
	atomsChannel <-chan []Trice

	// This is the receive channel for trice receiver ignoredChannel bytes.
	ignoredChannel <-chan []byte

	// atoms holds from channel read trice atoms not processed so far.
	atoms []Trice

	// ignored holds so far unused values for the next trice item.
	ignored []byte

	// values holds so far unused values for the next trice item.
	values []byte

	// item is the next trice item ready for output, if item is not 0.
	item id.Item

	// css is a collector for line substrings.
	// It is used only inside LineCollect() but needs to survive from call to call.
	css []string

	// lineComplete is set when a line is complete.
	lineComplete bool

	// sw is the interface for writing a whole line to the display.
	// When a line is complete, its []string slice is converted to one single string for output.
	// This simplifies the output interface and avoids partial lines to be mixed from several sources.
	sw io.StringWriter
}

// NewSimpleTriceInterpreter uses sw to write complete log lines as one composed string to a display device.
//
func NewSimpleTriceInterpreter(sw io.StringWriter, tr TriceReceiver) *TriceInterpreter {
	p := &TriceInterpreter{}
	p.atomsChannel = tr.TriceAtomsChannel()
	p.ignoredChannel = tr.IgnoredBytesChannel()
	p.sw = sw
	p.atoms = make([]Trice, 1000)
	p.ignored = make([]byte, 1000)
	p.values = make([]byte, 1000)
	p.css = make([]string, 1000)

	go func() {
		for {
			if nil != p.err {
				return // stop on any error, TODO: stop receiver too
			}
			select {
			case a, ok := <-p.atomsChannel:
				if !ok {
					return // stop when atoms channel was closed
				}
				p.atoms = append(p.atoms, a...)
			case b := <-p.ignoredChannel:
				p.ignored = append(p.ignored, b...)
			}
			s := fmt.Sprintln(p.atoms, p.ignored) // "processing"
			_, p.err = sw.WriteString(s)
			p.atoms = p.atoms[:0]
			p.ignored = p.ignored[:0]
		}
	}()
	return p
}

/*
func NewTriceInterpreter(r *TriceReceiver) {
	go func() {
		for {
			select {
			case atoms := <-r.atoms:
				p.atoms = append(p.atoms, atoms)
			case ignored := <-r.ignored:

			}
		}
	}()
	go func() {

		for {
			atomsSlice := <-r.atoms // next trice atoms slice
			// 			p.atomsBuffer = append(p.atomsBuffer, atomsSlice)
			// 			p.item.ID = int(trice.ID)
			// 			p.values = append(p.values, trice.Value[0], trice.Value[1])
			// 			//p.values = append(p.values, trice.Value...)
			fmt.Println(atomsSlice)

			if 0 < len(r.atoms) {
				atom := r.atoms[0]
				p.atoms = p.atoms[1:]
				if 0 == atom.ID {

				}
			}
		}
	}()
}
*/

// TrimBackslashes handles special chars in s
func TrimBackslashes(s string) string {
	s = strings.ReplaceAll(s, "\\a", "\a")
	s = strings.ReplaceAll(s, "\\b", "\b")
	s = strings.ReplaceAll(s, "\\t", "\t")
	s = strings.ReplaceAll(s, "\\n", "\n")
	s = strings.ReplaceAll(s, "\\f", "\f")
	s = strings.ReplaceAll(s, "\\r", "\r")
	s = strings.ReplaceAll(s, "\\v", "\v")
	// There four more formats to handle: (TODO)
	//   \x followed by exactly two hexadecimal digits,
	//   \ followed by exactly three octal digits,
	//   \u followed by exactly four hexadecimal digits,
	//   \U followed by exactly eight hexadecimal digits.
	return s
}

// LineCollect collects s into an internal line substring slice
// When s ends with a newline it is trimmed and the slice goes to Out and is discarded afterwards
func (p *TriceInterpreter) LineCollect(s string) {
	s = TrimBackslashes(s)

	a := func(su string) {
		p.css = append(p.css, su)
	}
	if 0 == len(p.css) {
		a(Prefix)
		a(lib.Timestamp())
	}
	if false == strings.HasSuffix(s, "\n") {
		a(s)
		return
	}
	s = strings.TrimSuffix(s, "\n")
	a(s)
	a(Postfix)
	//lineComplete = true
	//disp.Out(css)
	//css = css[:0] // discard slice data
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

// readRaw uses inner reader p.r to read byte stream and assumes encoding 'raw' (=='bare') for interpretation.
// It sends a number of Trice items to the internal 'atoms' channel,
// any ignored bytes to the internal 'ignored' channel and stores internally an error code.
// It looks for a sync point inside the internally read byte slice and ignores 1 to(triceSize-1) bytes
// if the sync is not on a triceSize offset. If no sync point is found sync is assumed per default.
func (p *TriceReceiver) readRaw() {
	leftovers := len(p.syncbuffer) // bytes buffered in bytes buffer from last call
	var minBytes int               // needed additional byte count making a Trice
	if leftovers < triceSize {
		minBytes = triceSize - leftovers
	} else {
		minBytes = triceSize
	}
	limit := cap(p.syncbuffer)
	var n int
	n, p.err = io.ReadAtLeast(p.r, p.syncbuffer[leftovers:limit], minBytes) // read to have at least triceSize bytes
	le := leftovers + n                                                     // the valid len inside p.by
	if le < triceSize {                                                     // got not the minimum amount of expected bytes
		if nil != p.err {
			close(p.atoms) // this is the signal for the following interpreter to stop
		}
		return // s.th. went wrong
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
	p.err = binary.Read(buf, binary.LittleEndian, atoms) // target assumed to be little endian
	p.syncbuffer = p.syncbuffer[triceSize*atomsAvail:]   // any leftover count from 1 to (triceSize-1) is possible
	p.atoms <- atoms                                     // send trices
}

// // triceItem tries to create a full trice item from the receiver data.
// func (p *TriceReceiver) triceItem() {
// 	for 0 == p.item.ID {
// 		select {
// 		case atomsSlice := <-p.atoms: // next trice atoms slice
// 			p.atomsBuffer = append(p.atomsBuffer, atomsSlice)
// 			p.item.ID = int(trice.ID)
// 			p.values = append(p.values, trice.Value[0], trice.Value[1])
// 			//p.values = append(p.values, trice.Value...)
// 		case dropped := <-p.ignored:
// 			fmt.Println(dropped)
// 		}
// 	}
// }
//
// // TriceItem tries to create a full trice item from the receiver channels.
// func (p *TriceReceiver) TriceItem() {
// 	go func() {
// 		for {
// 			if nil != p.err || io.EOF == p.err {
// 				return
// 			}
// 			p.triceItem()
// 		}
// 	}()
// }

/*func xy(){
	var t Trice // chanel 'trices'
	//var b []byte // channel 'ignored'
	item, values, err := TriceCollector(t)
	if item (
		//s, b, _ := Emitter( item, values, b)
		LineCollect(s)
	)
}*/

/* NewTriceReceiverFromWrap creates a TriceReceiver using r as internal reader.
func NewTriceReceiverFromWrap(r io.Reader) *TriceReceiver {
	p := &TriceReceiver{}
	p.r = r
	p.atoms = make(chan []Trice, triceChannelCapacity)
	p.ignored = make(chan []byte, ignoredChannelCapacity)
	go func() {

	}()
	return p
}*/

//type TriceReadFunc func Read( []Trice) (int, error)

// type LineWriter interface{
// 	func LineWrite( s []string ) (int, error)
// }

// // Receiver provides a TriceReadMethod
// type Receiver struct {
// 	inner    io.Reader // used inner byte reader
// 	encoding string    // provided outer trice reader
// 	//list id.ListT
// 	//w io.Writer // byte writer interface for strings
// 	//item Bare // trice item
// 	//data []byte // data collector
// }

// // BareRead uses the underlying byte reader and returns a count of trice atoms inside t.
// func (p *Receiver) bareRead(t []Trice) (int, error) {
// 	b := make([]byte, 1000)
// 	n, e := p.inner.Read(b)
// 	b = b[:n]
// 	return 0, e
// }

// // WrapRead uses the underlying byte reader and returns a count of trice atoms inside t.
// func (p *Receiver) wrapRead(t []Trice) (int, error) {
// 	return 0, nil
// }

// // TriceRead uses the underlying byte reader and returns a count of trice atoms inside t.
// func (p *Receiver) TriceRead(t []Trice) (int, error) {
// 	switch p.encoding {
// 	case "bare":
// 		return p.bareRead(t)
// 	case "wrap":
// 		return p.wrapRead(t)
// 	}
// 	return 0, nil
// }

// // NewTriceReceiver returns a pointer to a trice receiver instance using r as inner byte reader.
// func NewTriceReceiver(inner io.Reader, encoding string) *Receiver {
// 	p := &Receiver{}
// 	p.inner = inner
// 	p.encoding = encoding
// 	return p
// }

// func io.WriteString(w Writer, s string) (n int, err error)

// // RemoteCommand is the data type for commands.
// //
// // The data package count is given by len([]Pkg)
// type RemoteCommand struct {
// 	PackageID uint // package id (cycle counter)
// 	PackageType byte // message, command, answer
// 	FunctionType byte // type identifyer byte
// 	FunctionID byte // function identifyer byte
// 	DataPackages [][]byte // DataPackageCount is len(DataPackages)
// }
