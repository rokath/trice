// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package decoder

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"io"
	"log"
	"sync"

	"github.com/dim13/cobs"
	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
)

// COBS is the Decoding instance for COBS encoded trices.
type COBS struct {
	decoderData
	cycle uint8
}

// NewCOBSRDecoder provides an EscDecoder instance.
//
// l is the trice id list in slice of struct format.
// in is the usable reader for the input bytes.
func NewCOBSRDecoder(lut id.TriceIDLookUp, m *sync.RWMutex, in io.Reader, endian bool) Decoder {
	p := &COBS{}
	p.cycle = 0xc0 // start value
	p.in = in
	p.iBuf = make([]byte, 0, defaultSize)
	p.b = make([]byte, 0, defaultSize)
	p.lut = lut
	p.lutMutex = m
	p.endian = endian
	return p
}

// decodeCOBS expects in slice rd a byte sequence ending with a 0, writes the COBS decoded data to wr and returns len(wr).
//
// If rd contains more bytes after the first 0 byte, these are ignored.
// Needs to be written in a better way.
func decodeCOBS(wr, rd []byte) int {
	if len(wr) < len(rd) {
		log.Fatalf("ERROR: len(wr) = %d < len(rd) = %d\n", len(wr), len(rd))
	}
	d := cobs.Decode(rd) // to do: avoid allocation
	if d == nil {
		return 0
	}
	return copy(wr, d)
}

// nextCOBSpackage reads with an inner reader a COBS encoded byte stream.
//
// When no terminating 0 is found in the incoming bytes nextCOBSpackage returns without action.
// That means the incoming data stream is exhausted and a next try should be started a bit later.
// Some arrived bytes are kept internally and concatenated with the following bytes in a next Read.
// When a terminating 0 is found in the incoming bytes ReadFromCOBS decodes the COBS package
// and returns it in b and its len in n. If more data arrived after the first terminating 0,
// these are kept internally and concatenated with the following bytes in a next Read.
func (p *COBS) nextCOBSpackage() {
	// Here p.iBuf contains no or available bytes, what can be several trice messages.
	// So first try to process p.iBuf
	index := bytes.IndexByte(p.iBuf, 0) // find terminating 0
	if index == -1 {                    // p.iBuf has no complete COBS data, so try to read more input
		bb := make([]byte, 1024)           // intermediate buffer
		m, err := p.in.Read(bb)            // use bb as bytes read buffer
		p.iBuf = append(p.iBuf, bb[:m]...) // merge with leftovers
		if err != nil && err != io.EOF {   // some serious error
			log.Fatal("ERROR:internal reader error", err) // exit
		}
		index = bytes.IndexByte(p.iBuf, 0) // find terminating 0
		if index == -1 {                   // p.iBuf has no complete COBS data, so leave
			// Even err could be io.EOF, some valid data possibly in p.iBUf.
			// In case of file input (J-LINK usage) a plug off is not detectable here.
			return // no terminating 0, nothing to do
		}
	}
	// here a complete COBS package exists
	p.b = make([]byte, defaultSize)
	n := decodeCOBS(p.b, p.iBuf[:index+1])
	p.b = p.b[:n]
	if DebugOut { // Debug output
		for _, x := range p.iBuf[:index+1] {
			fmt.Printf("%02x ", x)
		}
		fmt.Print("\n-> ")
		for _, x := range p.b {
			fmt.Printf("%02x ", x)
		}
		fmt.Println("")
	}
	p.iBuf = p.iBuf[index+1:] // step forward (next package data in p.iBuf now, if any)
	return
}

// Read is the provided read method for COBS decoding and provides next string as byte slice.
//
// It uses inner reader p.in and internal id look-up table to fill b with a string.
// b is a slice of bytes with a len for the max expected string size.
// n is the count of read bytes inside b.
// Read returns usually one complete trice string or nothing but can return concatenated
// trice strings, each ending with a newline despite the last one, when messages added.
// Read does not process all internally read complete trice packages to be able later to
// separate trices within one line to keep them separated for color processing.
// Therefore Read needs to be called cyclically even after returning io.EOF to process internal data.
// When Read returns n=0, all processable complete trice packages are done,
// but the start of a following trice package can be already inside the internal buffer.
// In case of a not matching cycle, a warning message in trice format is prefixed.
// In case of invalid package data, error messages in trice format are returned and the package is dropped.
func (p *COBS) Read(b []byte) (n int, err error) {
	if len(p.b) == 0 { // last decoded COBS package exhausted
		p.nextCOBSpackage()
	}
	if len(p.b) == 0 { // not enough data for a next package
		return
	}

	// Inside p.pkg is here one or a partial package, what means one or more trice messages.
	if len(p.b) < 4 {
		n += copy(b[n:], fmt.Sprintln("ERROR:package len", len(p.b), "is too short - ignoring package", p.b))
		n += copy(b[n:], fmt.Sprintln(hints))
		return
	}
	var head uint32 // 16 bit ID in upper 2 bytes, 8 bit parameter size as u32Count, 8 bit cycle counter least significant byte
	if p.endian == LittleEndian {
		head = binary.LittleEndian.Uint32(p.b[0:4])
	} else {
		head = binary.BigEndian.Uint32(p.b[0:4])
	}

	// cycle counter automatic & check
	cycle := uint8(head)
	if cycle == 0xc0 && p.cycle != 0xc0 && initialCycle == true { // with cycle counter and seems to be a target reset
		n += copy(b[n:], fmt.Sprintln("warning:   Target Reset?   "))
		p.cycle = cycle + 1 // adjust cycle
		initialCycle = false
	}
	if cycle == 0xc0 && p.cycle != 0xc0 && initialCycle == false { // with cycle counter and seems to be a target reset
		//n += copy(b[n:], fmt.Sprintln("info:   Target Reset?   ")) // to do: This line is ok with cycle counter but not without cycle counter
		p.cycle = cycle + 1 // adjust cycle
	}
	if cycle == 0xc0 && p.cycle == 0xc0 && initialCycle == true { // with or without cycle counter and seems to be a target reset
		n += copy(b[n:], fmt.Sprintln("warning:   Restart?   "))
		p.cycle = cycle + 1 // adjust cycle
		initialCycle = false
	}
	if cycle == 0xc0 && p.cycle == 0xc0 && initialCycle == false { // with or without cycle counter and seems to be a normal case
		p.cycle = cycle + 1 // adjust cycle
	}
	if cycle != 0xc0 { // with cycle counter and s.th. lost
		if cycle != p.cycle { // no cycle check for 0xc0 to avoid messages on every target reset and when no cycle counter is active
			n += copy(b[n:], fmt.Sprintln("CYCLE:", cycle, "not equal expected value", p.cycle, "- adjusting. Now", emitter.ColorChannelEvents("CYCLE")+1, "CycleEvents"))
			p.cycle = cycle // adjust cycle
		}
		initialCycle = false
		p.cycle++
	}

	p.paramSpace = int((0x0000FF00 & head) >> 6)
	p.triceSize = headSize + p.paramSpace
	triceID := id.TriceID(uint16(head >> 16))
	LastTriceID = triceID // used for showID
	if len(p.b) < p.triceSize {
		n += copy(b[n:], fmt.Sprintln("ERROR:package len", len(p.b), "is too short - ignoring package", p.b))
		n += copy(b[n:], fmt.Sprintln(hints))
		p.b = p.b[p.triceSize:]
		return
	}
	var ok bool
	p.lutMutex.RLock()
	p.trice, ok = p.lut[triceID]
	p.lutMutex.RUnlock()
	if !ok {
		n += copy(b[n:], fmt.Sprintln("WARNING:unknown ID ", triceID, "- ignoring trice", p.b[:p.triceSize]))
		n += copy(b[n:], fmt.Sprintln(hints))
		p.b = p.b[p.triceSize:]
		return
	}
	p.b = p.b[headSize:]      // drop used head info
	n += p.sprintTrice(b[n:]) // use param info
	p.b = p.b[p.paramSpace:]  // drop param info
	return
}

// sprintTrice writes a trice string or appropriate message into b and returns that len.
func (p *COBS) sprintTrice(b []byte) (n int) {

	if p.trice.Type == "TRICE_S" { // patch table paramSpace in that case
		if p.endian == LittleEndian {
			p.sLen = int(binary.LittleEndian.Uint32(p.b[0:4]))
		} else {
			p.sLen = int(binary.BigEndian.Uint32(p.b[0:4]))
		}
		cobsFunctionPtrList[0].paramSpace = (p.sLen + 7) & ^3 // +4 for 4 bytes sLen, +3^3 is alignment to 4
	}

	for _, s := range cobsFunctionPtrList {
		if s.triceType == p.trice.Type {
			if s.paramSpace == p.paramSpace {
				if len(p.b) < p.paramSpace {
					n += copy(b[n:], fmt.Sprintln("err:len(p.b) =", len(p.b), "< p.paramSpace = ", p.paramSpace, "- ignoring package", p.b[:len(p.b)]))
					n += copy(b[n:], fmt.Sprintln(hints))
					return
				}
				n += s.triceFn(p, b)
				return
			} else {
				n += copy(b[n:], fmt.Sprintln("err:trice.Type", p.trice.Type, ": s.paramSpace", s.paramSpace, "!= p.paramSpace", p.paramSpace, "- ignoring data", p.b[:p.paramSpace]))
				n += copy(b[n:], fmt.Sprintln(hints))
				return
			}
		}
	}
	n += copy(b[n:], fmt.Sprintln("err:Unknown trice.Type ", p.trice.Type, "- ignoring trice data", p.b[:p.paramSpace]))
	n += copy(b[n:], fmt.Sprintln(hints))
	p.b = p.b[p.paramSpace:]
	return
}

// triceTypeFn is the type for cobsFunctionPtrList elements.
type triceTypeFn struct {
	triceType  string
	triceFn    func(p *COBS, b []byte) int
	paramSpace int
}

// cobsFunctionPtrList is a function pointer list.
var cobsFunctionPtrList = [...]triceTypeFn{
	{"TRICE_S", (*COBS).triceS, -1}, // do not remove from first position, see cobsFunctionPtrList[0].paramSpace = ...
	{"TRICE0", (*COBS).trice0, 0},
	{"TRICE8_1", (*COBS).trice81, 4},
	{"TRICE8_2", (*COBS).trice82, 4},
	{"TRICE8_3", (*COBS).trice83, 4},
	{"TRICE8_4", (*COBS).trice84, 4},
	{"TRICE8_5", (*COBS).trice85, 8},
	{"TRICE8_6", (*COBS).trice86, 8},
	{"TRICE8_7", (*COBS).trice87, 8},
	{"TRICE8_8", (*COBS).trice88, 8},
	{"TRICE16_1", (*COBS).trice161, 4},
	{"TRICE16_2", (*COBS).trice162, 4},
	{"TRICE16_3", (*COBS).trice163, 8},
	{"TRICE16_4", (*COBS).trice164, 8},
	{"TRICE32_1", (*COBS).trice321, 4},
	{"TRICE32_2", (*COBS).trice322, 8},
	{"TRICE32_3", (*COBS).trice323, 12},
	{"TRICE32_4", (*COBS).trice324, 16},
	{"TRICE64_1", (*COBS).trice641, 8},
	{"TRICE64_2", (*COBS).trice642, 16},
}

func (p *COBS) triceS(b []byte) int {
	if DebugOut {
		fmt.Println(p.b)
	}
	s := p.b[4 : 4+p.sLen]
	return copy(b, fmt.Sprintf(p.trice.Strg, string(s)))
}

func (p *COBS) trice0(b []byte) int {
	return copy(b, fmt.Sprintf(p.trice.Strg))
}

func (p *COBS) trice81(b []byte) int {
	b0 := int8(p.b[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, b0))
}

func (p *COBS) trice82(b []byte) int {
	b0 := int8(p.b[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b1 := int8(p.b[1]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, b0, b1))
}

func (p *COBS) trice83(b []byte) int {
	b0 := int8(p.b[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b1 := int8(p.b[1]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b2 := int8(p.b[2]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, b0, b1, b2))
}

func (p *COBS) trice84(b []byte) int {
	b0 := int8(p.b[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b1 := int8(p.b[1]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b2 := int8(p.b[2]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b3 := int8(p.b[3]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, b0, b1, b2, b3))
}

func (p *COBS) trice85(b []byte) int {
	b0 := int8(p.b[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b1 := int8(p.b[1]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b2 := int8(p.b[2]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b3 := int8(p.b[3]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b4 := int8(p.b[4]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, b0, b1, b2, b3, b4))
}

func (p *COBS) trice86(b []byte) int {
	b0 := int8(p.b[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b1 := int8(p.b[1]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b2 := int8(p.b[2]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b3 := int8(p.b[3]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b4 := int8(p.b[4]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b5 := int8(p.b[5]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, b0, b1, b2, b3, b4, b5))
}

func (p *COBS) trice87(b []byte) int {
	b0 := int8(p.b[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b1 := int8(p.b[1]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b2 := int8(p.b[2]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b3 := int8(p.b[3]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b4 := int8(p.b[4]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b5 := int8(p.b[5]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b6 := int8(p.b[6]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, b0, b1, b2, b3, b4, b5, b6))
}

func (p *COBS) trice88(b []byte) int {
	b0 := int8(p.b[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b1 := int8(p.b[1]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b2 := int8(p.b[2]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b3 := int8(p.b[3]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b4 := int8(p.b[4]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b5 := int8(p.b[5]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b6 := int8(p.b[6]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b7 := int8(p.b[7]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, b0, b1, b2, b3, b4, b5, b6, b7))
}

func (p *COBS) trice161(b []byte) int {
	d0 := int16(p.readU16(p.b[0:2])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, d0))
}

func (p *COBS) trice162(b []byte) int {
	d0 := int16(p.readU16(p.b[0:2])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int16(p.readU16(p.b[2:4])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, d0, d1))
}

func (p *COBS) trice163(b []byte) int {
	d0 := int16(p.readU16(p.b[0:2])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int16(p.readU16(p.b[2:4])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d2 := int16(p.readU16(p.b[4:6])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, d0, d1, d2))
}

func (p *COBS) trice164(b []byte) int {
	d0 := int16(p.readU16(p.b[0:2])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int16(p.readU16(p.b[2:4])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d2 := int16(p.readU16(p.b[4:6])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d3 := int16(p.readU16(p.b[6:8])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, d0, d1, d2, d3))
}

func (p *COBS) trice321(b []byte) int {
	d0 := int32(p.readU32(p.b[0:4])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, d0))
}

func (p *COBS) trice322(b []byte) int {
	d0 := int32(p.readU32(p.b[0:4])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int32(p.readU32(p.b[4:8])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, d0, d1))
}

func (p *COBS) trice323(b []byte) int {
	d0 := int32(p.readU32(p.b[0:4]))  // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int32(p.readU32(p.b[4:8]))  // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d2 := int32(p.readU32(p.b[8:12])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, d0, d1, d2))
}

func (p *COBS) trice324(b []byte) int {
	d0 := int32(p.readU32(p.b[0:4]))   // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int32(p.readU32(p.b[4:8]))   // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d2 := int32(p.readU32(p.b[8:12]))  // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d3 := int32(p.readU32(p.b[12:16])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, d0, d1, d2, d3))
}

func (p *COBS) trice641(b []byte) int {
	d0 := int64(p.readU64(p.b[0:8])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, d0))
}

func (p *COBS) trice642(b []byte) int {
	d0 := int64(p.readU64(p.b[0:8]))  // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int64(p.readU64(p.b[8:16])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, d0, d1))
}
