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
	"fmt"
	"io"
	"log"
	"reflect"
	"runtime"
	"strings"

	"github.com/rokath/trice/internal/id"
)

const (
	// triceSize is the count of bytes in a byte stream used for one Trice.
	triceSize = 4

	// triceChannelCapacity is the max possible trice slice count hold in channel
	triceChannelCapacity = 10

	// ignoredChannelCapacity is the max count of ignored bytes
	ignoredChannelCapacity = 10
)

var (
	// bytesBufferCapacity is the internal bufferered amount for sync package search.
	bytesBufferCapacity = 64

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

// TriceAtomsReceiver is the interface a trice receicer has to provide for a trice interpreter.
// The provided channels are read only channels
type TriceAtomsReceiver interface {
	TriceAtomsChannel() <-chan []Trice
	IgnoredBytesChannel() <-chan []byte
}

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

// TriceInterpreter uses the 2 TriceReceiver channels and global settings to compose a complete log line as one string.
// The string is transferred using the io.StringWriter interface.
type TriceInterpreter struct {
	// If some error occured it is stored here. This makes error handling more handy.
	err error

	// This is the receive channel for trice atomsChannel.
	atomsChannel <-chan []Trice

	// This is the receive channel for trice receiver ignoredChannel bytes.
	ignoredChannel <-chan []byte

	// This channel is used to stop the TriceInterpreter
	done chan int

	// atoms holds read trice atoms not processed so far.
	atoms []Trice

	// ignored holds so far unused values for the next trice item.
	ignored []byte

	// values holds so far unused values for the next trice item.
	values []byte

	//
	list id.ListT

	// item is the next trice item ready for output, if item is not 0.
	item id.Item

	// lineComplete is set when a line is complete.
	lineComplete bool

	// sw is the interface for writing a whole line to the display.
	// When a line is complete, its []string slice is converted to one single string for output.
	// This simplifies the output interface and avoids partial lines to be mixed from several sources.
	sw io.StringWriter
}

// NewSimpleTriceInterpreter gets its data from the TriceAtomsReceiver interface tr.
// It uses the io.StringWriter interface sw to write the trice strings looked up in the trice id list
// and enhanced with the printed values. Each single trice string is written separately with sw.
// It does not evaluate the TriceInterpreter.ignored bytes.
func NewSimpleTriceInterpreter(sw io.StringWriter, l id.ListT, tr TriceAtomsReceiver) *TriceInterpreter {
	p := &TriceInterpreter{}
	p.atomsChannel = tr.TriceAtomsChannel()
	p.ignoredChannel = tr.IgnoredBytesChannel()
	p.sw = sw
	p.list = l
	p.atoms = make([]Trice, 0, 1000)
	p.ignored = make([]byte, 0, 1000)
	p.values = make([]byte, 0, 100)
	p.done = make(chan int)

	go func() {
		for {
			p.ErrorFatal()
			select {
			case a, _ := <-p.atomsChannel:
				//if !ok {
				//	return // stop when atoms channel was closed
				//}
				p.atoms = append(p.atoms, a...)
			case b := <-p.ignoredChannel:
				p.ignored = append(p.ignored, b...)
			case <-p.done:
				return // end of life
			}
			for 0 < len(p.atoms) {
				s := p.translate()
				_, p.err = sw.WriteString(s)
			}
			if 0 < len(p.ignored) {
				// TODO: evaluate other protocols here
				s := fmt.Sprintln("WARNING:ignoring bytes:", p.ignored)
				_, p.err = sw.WriteString(s)
				p.ignored = p.ignored[:0]
			}
		}
	}()
	return p
}

// ErrorFatal ends in osExit(1) if p.err not nil.
func (p *TriceInterpreter) ErrorFatal() {
	if nil == p.err {
		return
	}
	// notice that we're using 1, so it will actually log the where
	// the error happened, 0 = this function, we don't want that.
	pc, fn, line, _ := runtime.Caller(1)
	log.Fatalf("[error] in %s[%s:%d] %v", runtime.FuncForPC(pc).Name(), fn, line, p.err)
}

// translate evaluates p.atoms, p.values and p.ignored and tries to generate a string.
// If an internal error state occured it discards all accumulated data and clears the error.
// On return it delivers an emty string if
func (p *TriceInterpreter) translate() (s string) {
	if nil != p.err {
		s = fmt.Sprintln(p.err, p.values, p.atoms, p.ignored)
		p.values = p.values[:0]
		p.atoms = p.atoms[:0]
		p.ignored = p.ignored[:0]
		p.err = nil
		return
	}
	if 0 == len(p.atoms) {
		return
	}
	trice := p.atoms[0]
	p.atoms = p.atoms[1:]
	if 5654 == trice.ID { // discard sync trice
		if 22 != trice.Value[0] || 22 != trice.Value[1] {
			s = fmt.Sprintln("WARNING:", trice, "is invalid. Must have payload [22 22]. (5654 is sync trice.ID)")
		}
		return
	}
	p.values = append(p.values, trice.Value[:]...)
	if 0 == trice.ID { // only data
		return
	}
	var index int
	index, p.err = id.Index(int(trice.ID), p.list)
	if nil != p.err {
		s = fmt.Sprintln("WARNING: trice.ID", trice.ID, "not found, discarding values:", p.values)
		p.values = p.values[:0]
		return
	}
	p.item = p.list[index]
	s = p.emitter()
	p.values = p.values[:0]
	return
}

// emmiter wworks fine with %x, %d and %o but NOT with %u for now
// %x is emitted in Go signed!
// For %u a format string parsing is needed to perform the correct casts.
func (p *TriceInterpreter) emitter() (s string) {
	it := p.item
	prm := p.values
	f, _, _ := langCtoGoFmtStingConverter(it.FmtStrg) // todo
	var v0, v1, v2, v3 int16
	var w0, w1, w2, w3 int32
	var l0, l1 int64
	p.evalLen()
	switch it.FmtType {
	case "TRICE0":
		s = f
	case "TRICE8_1":
		if "%s" != f { // normal case
			s = fmt.Sprintf(f, int8(prm[0]))
		}
	case "TRICE8_2":
		s = fmt.Sprintf(f, int8(prm[0]), int8(prm[1]))
	case "TRICE8_3":
		s = fmt.Sprintf(f, int8(prm[0]), int8(prm[1]), int8(prm[2]))
	case "TRICE8_4":
		s = fmt.Sprintf(f, int8(prm[0]), int8(prm[1]), int8(prm[2]), int8(prm[3]))
	case "TRICE8_5":
		s = fmt.Sprintf(f, int8(prm[0]), int8(prm[1]), int8(prm[2]), int8(prm[3]), int8(prm[4]))
	case "TRICE8_6":
		s = fmt.Sprintf(f, int8(prm[0]), int8(prm[1]), int8(prm[2]), int8(prm[3]), int8(prm[4]), int8(prm[5]))
	case "TRICE8_7":
		s = fmt.Sprintf(f, int8(prm[0]), int8(prm[1]), int8(prm[2]), int8(prm[3]), int8(prm[4]), int8(prm[5]), int8(prm[6]))
	case "TRICE8_8":
		s = fmt.Sprintf(f, int8(prm[0]), int8(prm[1]), int8(prm[2]), int8(prm[3]), int8(prm[4]), int8(prm[5]), int8(prm[6]), int8(prm[7]))
	case "TRICE16_1":
		v0 = int16(binary.LittleEndian.Uint16(prm[0:2]))
		s = fmt.Sprintf(f, v0)
	case "TRICE16_2":
		v0 = int16(binary.LittleEndian.Uint16(prm[0:2]))
		v1 = int16(binary.LittleEndian.Uint16(prm[2:4]))
		s = fmt.Sprintf(f, v0, v1)
	case "TRICE16_3":
		v0 = int16(binary.LittleEndian.Uint16(prm[0:2]))
		v1 = int16(binary.LittleEndian.Uint16(prm[2:4]))
		v2 = int16(binary.LittleEndian.Uint16(prm[4:6]))
		s = fmt.Sprintf(f, v0, v1, v2)
	case "TRICE16_4":
		v0 = int16(binary.LittleEndian.Uint16(prm[0:2]))
		v1 = int16(binary.LittleEndian.Uint16(prm[2:4]))
		v2 = int16(binary.LittleEndian.Uint16(prm[4:6]))
		v3 = int16(binary.LittleEndian.Uint16(prm[6:8]))
		s = fmt.Sprintf(f, v0, v1, v2, v3)
	case "TRICE32_1":
		w0 = int32(binary.LittleEndian.Uint32(prm[0:4]))
		s = fmt.Sprintf(f, w0)
	case "TRICE32_2":
		w0 = int32(binary.LittleEndian.Uint32(prm[0:4]))
		w1 = int32(binary.LittleEndian.Uint32(prm[4:8]))
		s = fmt.Sprintf(f, w0, w1)
	case "TRICE32_3":
		w0 = int32(binary.LittleEndian.Uint32(prm[0:4]))
		w1 = int32(binary.LittleEndian.Uint32(prm[4:8]))
		w2 = int32(binary.LittleEndian.Uint32(prm[8:12]))
		s = fmt.Sprintf(f, w0, w1, w2)
	case "TRICE32_4":
		w0 = int32(binary.LittleEndian.Uint32(prm[0:4]))
		w1 = int32(binary.LittleEndian.Uint32(prm[4:8]))
		w2 = int32(binary.LittleEndian.Uint32(prm[8:12]))
		w3 = int32(binary.LittleEndian.Uint32(prm[12:16]))
		s = fmt.Sprintf(f, w0, w1, w2, w3)
	case "TRICE64_1":
		l0 = int64(binary.LittleEndian.Uint64(prm[0:8]))
		s = fmt.Sprintf(f, l0)
	case "TRICE64_2":
		l0 = int64(binary.LittleEndian.Uint64(prm[0:8]))
		l1 = int64(binary.LittleEndian.Uint64(prm[8:16]))
		s = fmt.Sprintf(f, l0, l1)
	default:
		p.err = fmt.Errorf("Unknown FmtType %s", it.FmtType)
	}
	return
}

// evalLen checks if byte buffer t has appropriate length to id.item it
func (p *TriceInterpreter) evalLen() {
	it := p.item
	t := p.values
	switch it.FmtType {
	case "TRICE0", "TRICE8_1", "TRICE8_2", "TRICE16_1":
		if 2 != len(t) {
			p.err = fmt.Errorf("len %d != 2", len(t))
		}
	case "TRICE8_3", "TRICE8_4", "TRICE16_2", "TRICE32_1":
		if 4 != len(t) {
			p.err = fmt.Errorf("len %d != 4", len(t))
		}
	case "TRICE8_5", "TRICE8_6", "TRICE16_3":
		if 6 != len(t) {
			p.err = fmt.Errorf("len %d != 6", len(t))
		}
	case "TRICE8_7", "TRICE8_8", "TRICE16_4", "TRICE32_2", "TRICE64_1":
		if 8 != len(t) {
			p.err = fmt.Errorf("len %d != 8", len(t))
		}
	case "TRICE32_3":
		if 12 != len(t) {
			p.err = fmt.Errorf("len %d != 12", len(t))
		}
	case "TRICE32_4", "TRICE64_2":
		if 16 != len(t) {
			p.err = fmt.Errorf("len %d != 16", len(t))
		}
	}
}

// parse lang C formatstring for %u and replace them with %d and extend the
// returned slice with 0 for each %d, %c, %x and 1 for each converted %u
// http://www.cplusplus.com/reference/cstdio/printf/
//
// Changes:        C    Go
// ---------------------------
// int 	           %i   %d replace i with d
// unsigned int    %u   %d replace u with d and uint()
//
// No changes:     C    Go
// ---------------------------
// decimal         %d   %d
// hexadezimal     %x   %x
// HEXADECIMAL     %X   %X
// octal           %o   %o
// float           %f   %f
// char            %c   %c
// (void *)        %p   %p
func langCtoGoFmtStingConverter(f string) (s string, u []bool, err error) {
	s = f // TODO: parse f for %*u, replace with %*d and sett []bool values accordingly
	return
}

// Stop ends life of TriceInterpreter
func (p *TriceInterpreter) Stop() {
	p.done <- 0
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

/*
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
*/
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
	// notice that we're using 1, so it will actually log the where
	// the error happened, 0 = this function, we don't want that.
	pc, fn, line, _ := runtime.Caller(1)
	log.Fatalf("[error] in %s[%s:%d] %v", runtime.FuncForPC(pc).Name(), fn, line, p.Err)
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
