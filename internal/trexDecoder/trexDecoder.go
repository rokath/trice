// Copyright 2022 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package trexDecoder

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"io"
	"log"
	"math"
	"strings"
	"sync"

	cobs "github.com/rokath/cobs/go"
	"github.com/rokath/tcobs/v1"
	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/pkg/cipher"
)

const (
	tyIdSize = 2 // tySize is what each trice message starts with: 2 bytes
	ncSize   = 2 // countSize is what each regular trice message contains after an optional target timestamp
	//headSize = tyIdSize + ncSize // headSize is what each regular trice message starts with: 2-bit msb + 14-bit ID + 16-bit nc

	typeS0 = 1 // regular trice format without stamp     : 011iiiiiI NC ...
	typeS2 = 2 // regular trice format with 16-bit stamp : 101iiiiiI TT NC ...
	typeS4 = 3 // regular trice format with 32-bit stamp : 111iiiiiI TT TT NC ...
	typeX0 = 0 // regular trice format with 32-bit stamp : 001iiiiiI TT TT TT TT NC ...
	//IDMask = 0x03FFF

)

/*
var (
	IDMask int
	typeS0 int
	typeS2 int
	typeS4 int
	typeS8 int
	typeX0 int
	typeX1 int
	typeX2 int
	typeX3 int
)

func init() {
	if decoder.IDBits == 13 {
		typeS0 = 3 // regular trice format without stamp     : 011iiiiiI NC ...
		typeS2 = 5 // regular trice format with 16-bit stamp : 101iiiiiI TT NC ...
		typeS4 = 7 // regular trice format with 32-bit stamp : 111iiiiiI TT TT NC ...
		typeS8 = 1 // regular trice format with 64-bit stamp : 001iiiiiI TT TT TT TT NC ...
		typeX0 = 0 // extended trice format or user data     : 000...... ...
		typeX1 = 2 // extended trice format or user data     : 010...... ...
		typeX2 = 4 // extended trice format or user data     : 100...... ...
		typeX3 = 6 // extended trice format or user data     : 110...... ...
		IDMask = 0x01FFF
	} else if decoder.IDBits == 14 {
		typeS0 = 1 // regular trice format without stamp     : 011iiiiiI NC ...
		typeS2 = 2 // regular trice format with 16-bit stamp : 101iiiiiI TT NC ...
		typeS4 = 3 // regular trice format with 32-bit stamp : 111iiiiiI TT TT NC ...
		typeX0 = 0 // regular trice format with 32-bit stamp : 001iiiiiI TT TT TT TT NC ...
		IDMask = 0x03FFF
	} else {
		log.Fatal("TREX works with 14 (legacy) or 13 (actual) bits for ID encoding.")
	}
}
*/
// trexDec is the Decoding instance for trex encoded trices.
type trexDec struct {
	decoder.DecoderData
	cycle uint8  // cycle date: c0...bf
	pFmt  string // modified trice format string: %u -> %d
	u     []int  // 1: modified format string positions:  %u -> %d, 2: float (%f)
}

// New provides a TREX decoder instance.
//
// l is the trice id list in slice of struct format.
// in is the usable reader for the input bytes.
func New(w io.Writer, lut id.TriceIDLookUp, m *sync.RWMutex, li id.TriceIDLookUpLI, in io.Reader, endian bool) decoder.Decoder {
	// Todo: rewrite using the TCOBS Reader. The provided in io.Reader provides a raw data stream.
	// https://github.com/rokath/tcobs/blob/master/TCOBSv1/read.go -> use NewDecoder ...

	p := &trexDec{}
	p.cycle = 0xc0 // start value
	p.W = w
	p.In = in
	p.IBuf = make([]byte, 0, decoder.DefaultSize)
	p.B = make([]byte, 0, decoder.DefaultSize)
	p.InnerBuffer = make([]byte, decoder.DefaultSize)
	p.Lut = lut
	p.LutMutex = m
	p.Endian = endian
	p.Li = li
	return p
}

// nextPackage reads with an inner reader a TCOBSv1 encoded byte stream.
//
// When no terminating 0 is found in the incoming bytes nextPackage returns without action.
// That means the incoming data stream is exhausted and a next try should be started a bit later.
// Some arrived bytes are kept internally and concatenated with the following bytes in a next Read.
// When a terminating 0 is found in the incoming bytes ReadFromCOBS decodes the COBS package
// and returns it in b and its len in n. If more data arrived after the first terminating 0,
// these are kept internally and concatenated with the following bytes in a next Read.
func (p *trexDec) nextPackage() {
	// Here p.IBuf contains none or available bytes, what can be several trice messages.
	// So first try to process p.IBuf.
	index := bytes.IndexByte(p.IBuf, 0) // find terminating 0
	if index == -1 {                    // p.IBuf has no complete COBS data, so try to read more input
		m, err := p.In.Read(p.InnerBuffer)            // use p.InnerBuffer as bytes read buffer
		p.IBuf = append(p.IBuf, p.InnerBuffer[:m]...) // merge with leftovers
		if err != nil && err != io.EOF {              // some serious error
			log.Fatal("ERROR:internal reader error", err) // exit
		}
		index = bytes.IndexByte(p.IBuf, 0) // find terminating 0
		if index == -1 {                   // p.IBuf has no complete COBS data, so leave
			// Even err could be io.EOF, some valid data possibly in p.iBUf.
			// In case of file input (J-LINK usage) a plug off is not detectable here.
			return // no terminating 0, nothing to do
		}
	}
	if decoder.TestTableMode {
		p.printTestTableLine(index + 1)
	}
	// here a complete TCOBS package exists
	if decoder.DebugOut { // Debug output
		fmt.Fprintf(p.W, "%s: ", decoder.PackageFraming)
		decoder.Dump(p.W, p.IBuf[:index+1])
	}

	p.B = make([]byte, decoder.DefaultSize) // todo: avoid allocation

	var n int
	var e error
	// todo: automatically set default decoder.PackageFraming value to COBS if XTEA is active.
	switch strings.ToLower(decoder.PackageFraming) {
	case "cobs":
		n, e = cobs.Decode(p.B, p.IBuf[:index]) // if index is 0, an empty buffer is decoded
		if e != nil {
			fmt.Println("inconsistent COBS buffer:", p.IBuf[:index+1]) // show also terminating 0
		}
	case "tcobs", "tcobsv1":
		n, e = tcobs.Decode(p.B, p.IBuf[:index]) // if index is 0, an empty buffer is decoded
		if e != nil {
			fmt.Println("inconsistent TCOBSv1 buffer:", p.IBuf[:index+1]) // show also terminating 0
			p.B = p.B[:0]
		} else {
			p.B = p.B[len(p.B)-n:]
		}
	case "none":
		p.B = p.IBuf[:index]
		n, e = index, nil
	//  case "TCOBSv2", "TCOBSV2", "tcobsv2":
	//  	n := tcobs.CDecode(p.B, p.IBuf[:index]) // if index is 0, an empty buffer is decoded
	//  	if n < 0 {
	//  		fmt.Println("inconsistent TCOBSv2 buffer:", p.IBuf[:index+1]) // show also terminating 0
	//  		p.B = p.B[:0]
	//  	} else {
	//  		p.B = p.B[len(p.B)-n:]
	//  	}
	default:
		fmt.Println("Invalid framing switch:", decoder.PackageFraming) // show also terminating 0
		p.B = p.B[:0]

	}

	p.IBuf = p.IBuf[index+1:] // step forward (next package data in p.IBuf now, if any)
	p.B = p.B[:n]

	if decoder.DebugOut { // Debug output
		fmt.Fprint(p.W, "->TRICE: ")
		decoder.Dump(p.W, p.B)
	}

	if cipher.Password != "" { // encrypted
		cipher.Decrypt(p.B, p.B)
		if decoder.DebugOut { // Debug output
			fmt.Fprint(p.W, "-> DEC:  ")
			decoder.Dump(p.W, p.B)
		}
	}
}

// Read is the provided read method for TREX decoding and provides next string as byte slice.
//
// It uses inner reader p.In and internal id look-up table to fill b with a string.
// b is a slice of bytes with a len for the max expected string size.
// n is the count of read bytes inside b.
// Read returns usually one complete trice string or nothing but can return concatenated
// trice strings, each ending with a newline despite the last one, when messages added.
// Read does not process all internally read complete trice packages to be able later to
// separate Trices within one line to keep them separated for color processing.
// Therefore, Read needs to be called cyclically even after returning io.EOF to process internal data.
// When Read returns n=0, all processable complete trice packages are done,
// but the start of a following trice package can be already inside the internal buffer.
// In case of a not matching cycle, a warning message in trice format is prefixed.
// In case of invalid package data, error messages in trice format are returned and the package is dropped.
func (p *trexDec) Read(b []byte) (n int, err error) {
	if len(p.B) == 0 { // last decoded package exhausted
		p.nextPackage()
	}
	packageSize := len(p.B)
	if packageSize < tyIdSize { // not enough data for a next package
		return
	}
	tyId := p.ReadU16(p.B)
	p.B = p.B[tyIdSize:]

	triceType := int(tyId >> decoder.IDBits) // most significant bit are the triceType
	triceID := id.TriceID(0x3FFF & tyId)     // 14 least significant bits are the ID
	decoder.LastTriceID = triceID            // used for showID

	switch triceType {
	case typeS0: // no timestamp
		decoder.TargetTimestampSize = 0
	case typeS2: // 16-bit stamp
		decoder.TargetTimestampSize = 2
	case typeS4: // 32-bit stamp
		decoder.TargetTimestampSize = 4
	//case typeS8: // 64-bit stamp
	//	decoder.TargetTimestampSize = 8
	case typeX0: // extended trice type X0
		// todo: implement special case here
		//case typeX1: // extended trice type X0
		// todo: implement special case here
		//case typeX2: // extended trice type X0
		// todo: implement special case here
		//case typeX3: // extended trice type X0
		// todo: implement special case here
		return
	}

	if packageSize < tyIdSize+decoder.TargetTimestampSize+ncSize { // non typeEX trices
		return
	}

	if triceType == typeS0 {
		decoder.TargetTimestamp = 0
	} else if triceType == typeS2 { // 16-bit stamp
		decoder.TargetTimestamp = uint64(p.ReadU16(p.B))
	} else if triceType == typeS4 { // 32-bit stamp
		decoder.TargetTimestamp = uint64(p.ReadU32(p.B))
		//} else if triceType == typeS8 { // 64-bit stamp
		//	decoder.TargetTimestamp = uint64(p.ReadU64(p.B))
	} else {
		log.Fatal("triceType ", triceType, " not implemented (hint: IDBits value?)")
	}
	p.B = p.B[decoder.TargetTimestampSize:]

	nc := p.ReadU16(p.B) // n = number of data bytes (without timestamp), most significant bit is the count encoding, c = cycle
	if len(p.B) == 2 {
		p.B = p.B[:0]
	} else {
		p.B = p.B[ncSize:]
	}
	var cycle uint8
	if nc>>15 == 1 { // special case: more than data 127 bytes
		// C code: #define LCNT(count) TRICE_PUT16( (0x8000 | (count)) );
		cycle = p.cycle                 // cycle is not transmitted, so set expected value
		p.ParamSpace = int(0x7FFF & nc) // 15 bit for data byte count excluding timestamp
	} else {
		// C code: #define CNTC(count) TRICE_PUT16( ((count)<<8) | TRICE_CYCLE )
		cycle = uint8(nc)           // low byte is cycle
		p.ParamSpace = int(nc >> 8) // high byte is 7 bit number of bytes for data count excluding timestamp
	}

	p.TriceSize = tyIdSize + decoder.TargetTimestampSize + ncSize + p.ParamSpace
	if p.TriceSize > packageSize { // todo: change to '>' for multiple trices in one package (TriceOutMultiPackMode instead of TriceOutMultiSafeMode)
		n += copy(b[n:], fmt.Sprintln("ERROR:package size", packageSize, "is <", p.TriceSize, " - ignoring package", p.B))
		n += copy(b[n:], fmt.Sprintln(tyIdSize, decoder.TargetTimestampSize, ncSize, p.ParamSpace))
		n += copy(b[n:], fmt.Sprintln(decoder.Hints))
		p.B = p.B[len(p.B):]
		return
	}

	// cycle counter automatic & check
	if cycle == 0xc0 && p.cycle != 0xc0 && decoder.InitialCycle { // with cycle counter and seems to be a target reset
		n += copy(b[n:], fmt.Sprintln("warning:   Target Reset?   "))
		p.cycle = cycle + 1 // adjust cycle
		decoder.InitialCycle = false
	}
	if cycle == 0xc0 && p.cycle != 0xc0 && !decoder.InitialCycle { // with cycle counter and seems to be a target reset
		//n += copy(b[n:], fmt.Sprintln("info:   Target Reset?   ")) // todo: This line is ok with cycle counter but not without cycle counter
		p.cycle = cycle + 1 // adjust cycle
	}
	if cycle == 0xc0 && p.cycle == 0xc0 && decoder.InitialCycle { // with or without cycle counter and seems to be a target reset
		//n += copy(b[n:], fmt.Sprintln("warning:   Restart?   "))
		p.cycle = cycle + 1 // adjust cycle
		decoder.InitialCycle = false
	}
	if cycle == 0xc0 && p.cycle == 0xc0 && !decoder.InitialCycle { // with or without cycle counter and seems to be a normal case
		p.cycle = cycle + 1 // adjust cycle
	}
	if cycle != 0xc0 { // with cycle counter and s.th. lost
		if cycle != p.cycle { // no cycle check for 0xc0 to avoid messages on every target reset and when no cycle counter is active
			n += copy(b[n:], fmt.Sprintln("CYCLE:", cycle, "not equal expected value", p.cycle, "- adjusting. Now", emitter.ColorChannelEvents("CYCLE")+1, "CycleEvents"))
			p.cycle = cycle // adjust cycle
		}
		decoder.InitialCycle = false
		p.cycle++
	}

	var ok bool
	p.LutMutex.RLock()
	p.Trice, ok = p.Lut[triceID]
	p.LutMutex.RUnlock()
	if !ok {
		n += copy(b[n:], fmt.Sprintln("WARNING:unknown ID ", triceID, "- ignoring trice ending with", p.B))
		n += copy(b[n:], fmt.Sprintln(decoder.Hints))
		p.B = p.B[:0]
		return
	}

	n += p.sprintTrice(b[n:]) // use param info
	if len(p.B) < p.ParamSpace {
		n += copy(b[n:], fmt.Sprintln("ERROR:ignoring data garbage"))
		n += copy(b[n:], fmt.Sprintln(decoder.Hints))
		p.B = p.B[:0]
	} else {
		p.B = p.B[p.ParamSpace:] // drop param info
	}
	return

}

// sprintTrice writes a trice string or appropriate message into b and returns that len.
func (p *trexDec) sprintTrice(b []byte) (n int) {
	p.pFmt, p.u = decoder.UReplaceN(p.Trice.Strg)

	p.Trice.Type = strings.ToUpper(p.Trice.Type) // do not distinguish upper and lower case

	var triceType string                           // need to reconstruct full TRICE info, if not exist in type string
	if strings.HasPrefix(p.Trice.Type, "TRICE_") { // when no bitwidth, insert it
		triceType = "TRICE" + id.DefaultTriceBitWidth + "_" + p.Trice.Type[6:]
	}
	if p.Trice.Type == "TRICE" { // when nothing
		triceType = fmt.Sprintf("TRICE"+id.DefaultTriceBitWidth+"_%d", len(p.u)) // append bitwidth and count
	}
	if p.Trice.Type == "TRICE" && len(p.u) == 0 { // special case, overwrite
		triceType = "TRICE0"
	}
	if p.Trice.Type == "TRICE8" || p.Trice.Type == "TRICE16" || p.Trice.Type == "TRICE32" || p.Trice.Type == "TRICE64" { // when no count
		triceType = fmt.Sprintf(p.Trice.Type+"_%d", len(p.u)) // append count
	}

	for _, s := range cobsFunctionPtrList { // walk through the list and try to find a match for execution
		if s.triceType == p.Trice.Type || s.triceType == triceType { // match list entry "TRICE..."
			if len(p.B) < p.ParamSpace {
				n += copy(b[n:], fmt.Sprintln("err:len(p.B) =", len(p.B), "< p.ParamSpace = ", p.ParamSpace, "- ignoring package", p.B[:len(p.B)]))
				n += copy(b[n:], fmt.Sprintln(decoder.Hints))
				return
			}
			n += s.triceFn(p, b, s.bitWidth, s.paramCount) // match found, call handler
			return
		}
	}
	n += copy(b[n:], fmt.Sprintln("err:Unknown trice.Type:", p.Trice.Type, "and", triceType, "not matching - ignoring trice data", p.B[:p.ParamSpace]))
	n += copy(b[n:], fmt.Sprintln(decoder.Hints))
	return
}

// triceTypeFn is the type for cobsFunctionPtrList elements.
type triceTypeFn struct {
	triceType  string                                              // triceType describes if parameters, the parameter bit width or if the parameter is a string.
	triceFn    func(p *trexDec, b []byte, bitwidth, count int) int // triceFn performs the conversion to the output string.
	ParamSpace int                                                 // ParamSpace is the count of bytes allocated for the parameters.
	bitWidth   int                                                 // bitWidth is the individual parameter width.
	paramCount int                                                 // paramCount is the amount pf parameters for the format string, which must match the count of format specifiers.
}

// cobsFunctionPtrList is a function pointer list.
var cobsFunctionPtrList = [...]triceTypeFn{
	{"TRICE_S", (*trexDec).triceS, -1, 0, 0},     // do not remove from first position, see cobsFunctionPtrList[0].ParamSpace = ...
	{"TRICE_N", (*trexDec).triceN, -1, 0, 0},     // do not remove from 2nd position, see cobsFunctionPtrList[1].ParamSpace = ...
	{"TRICE_B", (*trexDec).trice8B, -1, 0, 0},    // do not remove from 3rd position, see cobsFunctionPtrList[2].ParamSpace = ...
	{"TRICE8_B", (*trexDec).trice8B, -1, 0, 0},   // do not remove from 4th position, see cobsFunctionPtrList[3].ParamSpace = ...
	{"TRICE16_B", (*trexDec).trice16B, -1, 0, 0}, // do not remove from 4th position, see cobsFunctionPtrList[4].ParamSpace = ...
	{"TRICE32_B", (*trexDec).trice32B, -1, 0, 0}, // do not remove from 4th position, see cobsFunctionPtrList[5].ParamSpace = ...
	{"TRICE64_B", (*trexDec).trice64B, -1, 0, 0}, // do not remove from 4th position, see cobsFunctionPtrList[6].ParamSpace = ...

	{"TRICE_F", (*trexDec).trice8F, -1, 0, 0},    // do not remove from 4th position, see cobsFunctionPtrList[7].ParamSpace = ...
	{"TRICE8_F", (*trexDec).trice8F, -1, 0, 0},   // do not remove from 4th position, see cobsFunctionPtrList[8].ParamSpace = ...
	{"TRICE16_F", (*trexDec).trice16F, -1, 0, 0}, // do not remove from 4th position, see cobsFunctionPtrList[9].ParamSpace = ...
	{"TRICE32_F", (*trexDec).trice32F, -1, 0, 0}, // do not remove from 4th position, see cobsFunctionPtrList[10].ParamSpace = ...
	{"TRICE64_F", (*trexDec).trice64F, -1, 0, 0}, // do not remove from 4th position, see cobsFunctionPtrList[11].ParamSpace = ...

	{"TRICE32_0", (*trexDec).trice0, 0, 0, 0},
	{"TRICE0", (*trexDec).trice0, 0, 0, 0},
	{"TRICE8_1", (*trexDec).unSignedOrSignedOut, 1, 8, 1},
	{"TRICE8_2", (*trexDec).unSignedOrSignedOut, 2, 8, 2},
	{"TRICE8_3", (*trexDec).unSignedOrSignedOut, 3, 8, 3},
	{"TRICE8_4", (*trexDec).unSignedOrSignedOut, 4, 8, 4},
	{"TRICE8_5", (*trexDec).unSignedOrSignedOut, 5, 8, 5},
	{"TRICE8_6", (*trexDec).unSignedOrSignedOut, 6, 8, 6},
	{"TRICE8_7", (*trexDec).unSignedOrSignedOut, 7, 8, 7},
	{"TRICE8_8", (*trexDec).unSignedOrSignedOut, 8, 8, 8},
	{"TRICE8_9", (*trexDec).unSignedOrSignedOut, 9, 8, 9},
	{"TRICE8_10", (*trexDec).unSignedOrSignedOut, 10, 8, 10},
	{"TRICE8_11", (*trexDec).unSignedOrSignedOut, 11, 8, 11},
	{"TRICE8_12", (*trexDec).unSignedOrSignedOut, 12, 8, 12},
	{"TRICE16_1", (*trexDec).unSignedOrSignedOut, 2, 16, 1},
	{"TRICE16_2", (*trexDec).unSignedOrSignedOut, 4, 16, 2},
	{"TRICE16_3", (*trexDec).unSignedOrSignedOut, 6, 16, 3},
	{"TRICE16_4", (*trexDec).unSignedOrSignedOut, 8, 16, 4},
	{"TRICE16_5", (*trexDec).unSignedOrSignedOut, 10, 16, 5},
	{"TRICE16_6", (*trexDec).unSignedOrSignedOut, 12, 16, 6},
	{"TRICE16_7", (*trexDec).unSignedOrSignedOut, 14, 16, 7},
	{"TRICE16_8", (*trexDec).unSignedOrSignedOut, 16, 16, 8},
	{"TRICE16_9", (*trexDec).unSignedOrSignedOut, 18, 16, 9},
	{"TRICE16_10", (*trexDec).unSignedOrSignedOut, 20, 16, 10},
	{"TRICE16_11", (*trexDec).unSignedOrSignedOut, 22, 16, 11},
	{"TRICE16_12", (*trexDec).unSignedOrSignedOut, 24, 16, 12},
	{"TRICE32_1", (*trexDec).unSignedOrSignedOut, 4, 32, 1},
	{"TRICE32_2", (*trexDec).unSignedOrSignedOut, 8, 32, 2},
	{"TRICE32_3", (*trexDec).unSignedOrSignedOut, 12, 32, 3},
	{"TRICE32_4", (*trexDec).unSignedOrSignedOut, 16, 32, 4},
	{"TRICE32_5", (*trexDec).unSignedOrSignedOut, 20, 32, 5},
	{"TRICE32_6", (*trexDec).unSignedOrSignedOut, 24, 32, 6},
	{"TRICE32_7", (*trexDec).unSignedOrSignedOut, 28, 32, 7},
	{"TRICE32_8", (*trexDec).unSignedOrSignedOut, 32, 32, 8},
	{"TRICE32_9", (*trexDec).unSignedOrSignedOut, 36, 32, 9},
	{"TRICE32_10", (*trexDec).unSignedOrSignedOut, 40, 32, 10},
	{"TRICE32_11", (*trexDec).unSignedOrSignedOut, 44, 32, 11},
	{"TRICE32_12", (*trexDec).unSignedOrSignedOut, 48, 32, 12},
	{"TRICE64_1", (*trexDec).unSignedOrSignedOut, 8, 64, 1},
	{"TRICE64_2", (*trexDec).unSignedOrSignedOut, 16, 64, 2},
	{"TRICE64_3", (*trexDec).unSignedOrSignedOut, 24, 64, 3},
	{"TRICE64_4", (*trexDec).unSignedOrSignedOut, 32, 64, 4},
	{"TRICE64_5", (*trexDec).unSignedOrSignedOut, 40, 64, 5},
	{"TRICE64_6", (*trexDec).unSignedOrSignedOut, 48, 64, 6},
	{"TRICE64_7", (*trexDec).unSignedOrSignedOut, 56, 64, 7},
	{"TRICE64_8", (*trexDec).unSignedOrSignedOut, 64, 64, 8},
	{"TRICE64_9", (*trexDec).unSignedOrSignedOut, 72, 64, 9},
	{"TRICE64_10", (*trexDec).unSignedOrSignedOut, 80, 64, 10},
	{"TRICE64_11", (*trexDec).unSignedOrSignedOut, 88, 64, 11},
	{"TRICE64_12", (*trexDec).unSignedOrSignedOut, 96, 64, 12},
}

// triceN converts dynamic strings.
func (p *trexDec) triceN(b []byte, _ int, _ int) int {
	s := string(p.B[:p.ParamSpace])
	// todo: evaluate p.Trice.Strg, use p.SLen and do whatever should be done
	return copy(b, fmt.Sprintf(p.Trice.Strg, s))
}

// triceS converts dynamic strings.
func (p *trexDec) triceS(b []byte, _ int, _ int) int {
	s := string(p.B[:p.ParamSpace])
	return copy(b, fmt.Sprintf(p.Trice.Strg, s))
}

// triceB converts dynamic buffers.
func (p *trexDec) trice8B(b []byte, _ int, _ int) (n int) {
	if decoder.DebugOut {
		fmt.Fprintln(p.W, p.B)
	}
	s := p.B[:p.ParamSpace]
	for i := 0; i < len(s); i++ {
		n += copy(b[n:], fmt.Sprintf(p.Trice.Strg, s[i]))
	}
	return
}

// trice16B converts dynamic buffers.
func (p *trexDec) trice16B(b []byte, _ int, _ int) (n int) {
	if decoder.DebugOut {
		fmt.Fprintln(p.W, p.B)
	}
	s := p.B[:p.ParamSpace]
	for i := 0; i < len(s); i += 2 {
		n += copy(b[n:], fmt.Sprintf(p.Trice.Strg, binary.LittleEndian.Uint16(s[i:])))
	}
	return
}

// trice32B converts dynamic buffers.
func (p *trexDec) trice32B(b []byte, _ int, _ int) (n int) {
	if decoder.DebugOut {
		fmt.Fprintln(p.W, p.B)
	}
	s := p.B[:p.ParamSpace]
	for i := 0; i < len(s); i += 4 {
		n += copy(b[n:], fmt.Sprintf(p.Trice.Strg, binary.LittleEndian.Uint32(s[i:])))
	}
	return
}

// trice64B converts dynamic buffers.
func (p *trexDec) trice64B(b []byte, _ int, _ int) (n int) {
	if decoder.DebugOut {
		fmt.Fprintln(p.W, p.B)
	}
	s := p.B[:p.ParamSpace]
	for i := 0; i < len(s); i += 8 {
		n += copy(b[n:], fmt.Sprintf(p.Trice.Strg, binary.LittleEndian.Uint64(s[i:])))
	}
	return
}

// trice8F display function call with 8-bit parameters.
func (p *trexDec) trice8F(b []byte, _ int, _ int) (n int) {
	if decoder.DebugOut {
		fmt.Fprintln(p.W, p.B)
	}
	s := p.B[:p.ParamSpace]
	n += copy(b[n:], fmt.Sprintf(p.Trice.Strg))
	for i := 0; i < len(s); i++ {
		n += copy(b[n:], fmt.Sprintf("(%02x)", s[i]))
	}
	n += copy(b[n:], fmt.Sprintf("\n"))
	return
}

// trice16F display function call with 16-bit parameters.
func (p *trexDec) trice16F(b []byte, _ int, _ int) (n int) {
	if decoder.DebugOut {
		fmt.Fprintln(p.W, p.B)
	}
	s := p.B[:p.ParamSpace]
	n += copy(b[n:], fmt.Sprintf(p.Trice.Strg))
	for i := 0; i < len(s); i += 2 {
		n += copy(b[n:], fmt.Sprintf("(%04x)", binary.LittleEndian.Uint16(s[i:])))
	}
	n += copy(b[n:], fmt.Sprintf("\n"))
	return
}

// trice32F display function call with 32-bit parameters.
func (p *trexDec) trice32F(b []byte, _ int, _ int) (n int) {
	if decoder.DebugOut {
		fmt.Fprintln(p.W, p.B)
	}
	s := p.B[:p.ParamSpace]
	n += copy(b[n:], fmt.Sprintf(p.Trice.Strg))
	for i := 0; i < len(s); i += 4 {
		n += copy(b[n:], fmt.Sprintf("(%08x)", binary.LittleEndian.Uint32(s[i:])))
	}
	n += copy(b[n:], fmt.Sprintf("\n"))
	return
}

// trice64F display function call with 64-bit parameters.
func (p *trexDec) trice64F(b []byte, _ int, _ int) (n int) {
	if decoder.DebugOut {
		fmt.Fprintln(p.W, p.B)
	}
	s := p.B[:p.ParamSpace]
	n += copy(b[n:], fmt.Sprintf(p.Trice.Strg))
	for i := 0; i < len(s); i += 8 {
		n += copy(b[n:], fmt.Sprintf("(%016x)", binary.LittleEndian.Uint64(s[i:])))
	}
	n += copy(b[n:], fmt.Sprintf("\n"))
	return
}

// trice0 prints the trice format string.
func (p *trexDec) trice0(b []byte, _ int, _ int) int {
	return copy(b, fmt.Sprintf(p.Trice.Strg))
}

// unSignedOrSignedOut prints p.B according to the format string.
func (p *trexDec) unSignedOrSignedOut(b []byte, bitwidth, count int) int {
	if len(p.u) != count {
		return copy(b, fmt.Sprintln("ERROR: Invalid format specifier count inside", p.Trice.Type, p.Trice.Strg))
	}
	v := make([]interface{}, 32768) // theoretical 2^15 bytes could arrive
	switch bitwidth {
	case 8:
		for i, f := range p.u {
			switch f {
			//case decoder.PointerFormatSpecifier:
			//	v[i] = unsafe.Pointer(uintptr(p.B[i]))
			case decoder.UnsignedFormatSpecifier, decoder.PointerFormatSpecifier: // see comment inside decoder.UReplaceN
				v[i] = p.B[i]
			case decoder.SignedFormatSpecifier:
				v[i] = int8(p.B[i])
			case decoder.BooleanFormatSpecifier:
				v[i] = p.B[i] != 0
			default:
				return copy(b, fmt.Sprintln("ERROR: Invalid format specifier (float?) inside", p.Trice.Type, p.Trice.Strg))
			}
		}
	case 16:
		for i, f := range p.u {
			n := p.ReadU16(p.B[2*i:])
			switch f {
			//case decoder.PointerFormatSpecifier:
			//	v[i] = unsafe.Pointer(uintptr(n))
			case decoder.UnsignedFormatSpecifier, decoder.PointerFormatSpecifier: // see comment inside decoder.UReplaceN
				v[i] = n
			case decoder.SignedFormatSpecifier:
				v[i] = int16(n)
			case decoder.BooleanFormatSpecifier:
				v[i] = n != 0
			default:
				return copy(b, fmt.Sprintln("ERROR: Invalid format specifier (float?) inside", p.Trice.Type, p.Trice.Strg))
			}
		}
	case 32:
		for i, f := range p.u {
			n := p.ReadU32(p.B[4*i:])
			switch f {
			//case decoder.PointerFormatSpecifier:
			//	v[i] = unsafe.Pointer(uintptr(n))
			case decoder.UnsignedFormatSpecifier, decoder.PointerFormatSpecifier: // see comment inside decoder.UReplaceN
				v[i] = n
			case decoder.SignedFormatSpecifier:
				v[i] = int32(n)
			case decoder.FloatFormatSpecifier:
				v[i] = math.Float32frombits(n)
			case decoder.BooleanFormatSpecifier:
				v[i] = n != 0
			default:
				return copy(b, fmt.Sprintln("ERROR: Invalid format specifier inside", p.Trice.Type, p.Trice.Strg))
			}
		}
	case 64:
		for i, f := range p.u {
			n := p.ReadU64(p.B[8*i:])
			switch f {
			//case decoder.PointerFormatSpecifier:
			//	v[i] = unsafe.Pointer(uintptr(n))
			case decoder.UnsignedFormatSpecifier, decoder.PointerFormatSpecifier: // see comment inside decoder.UReplaceN
				v[i] = n
			case decoder.SignedFormatSpecifier:
				v[i] = int64(n)
			case decoder.FloatFormatSpecifier:
				v[i] = math.Float64frombits(n)
			case decoder.BooleanFormatSpecifier:
				v[i] = n != 0
			default:
				return copy(b, fmt.Sprintln("ERROR: Invalid format specifier inside", p.Trice.Type, p.Trice.Strg))
			}
		}
	}
	return copy(b, fmt.Sprintf(p.pFmt, v[:len(p.u)]...))
}

var testTableVirgin = true

// printTestTableLine is used to generate testdata
func (p *trexDec) printTestTableLine(n int) {
	if emitter.NextLine || testTableVirgin {
		emitter.NextLine = false
		testTableVirgin = false
		fmt.Printf("{ []byte{ ")
	}
	for _, b := range p.IBuf[0:n] { // just to see trice bytes per trice
		fmt.Printf("%3d,", b)
	}
}
