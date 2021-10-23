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

var (
	DumpLineByteCount int
)

// COBSR is the Decoding instance for COBSR encoded trices.
type COBSR struct {
	decoderData
	cycle uint8
}

// NewCOBSRDecoder provides an EscDecoder instance.
// l is the trice id list in slice of struct format.
// in is the usable reader for the input bytes.
func NewCOBSRDecoder(lut id.TriceIDLookUp, m *sync.RWMutex, in io.Reader, endian bool) Decoder {
	p := &COBSR{}
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
	// fmt.Printf("################## len(d)=%d, len(rd)=%d\n", len(d), len(rd) )
	// fmt.Printf("################## len(d)=%d, len(rd)=%d\n", len(d), len(rd) )
	// fmt.Printf("################## len(d)=%d, len(rd)=%d\n", len(d), len(rd) )
	// fmt.Printf("################## len(d)=%d, len(rd)=%d\n", len(d), len(rd) )
	// fmt.Printf("################## len(d)=%d, len(rd)=%d\n", len(d), len(rd) )
	// fmt.Printf("################## len(d)=%d, len(rd)=%d\n", len(d), len(rd) )
	// fmt.Printf("################## len(d)=%d, len(rd)=%d\n", len(d), len(rd) )
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
func (p *COBSR) nextCOBSpackage() {
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
func (p *COBSR) Read(b []byte) (n int, err error) {
	if len(p.b) == 0 { // last decoded COBS package exhausted
		p.nextCOBSpackage()
	}
	if len(p.b) == 0 { // not enough data for a next package
		return
	}
	// Inside p.pkg is here one or a partial package, what means one or more trice messages.
	hints := "att:Hints:Baudrate? Overflow? Encoding? Interrupt? CycleCounter? til.json?"
	if len(p.b) < 4 {
		n += copy(b[n:], fmt.Sprintln("ERROR:package len", len(p.b), "is too short - ignoring package", p.b))
		n += copy(b[n:], fmt.Sprintln(hints))
		return
	}
	var head uint32
	if p.endian == LittleEndian {
		head = binary.LittleEndian.Uint32(p.b[0:4])
	} else {
		head = binary.BigEndian.Uint32(p.b[0:4])
	}
	if CycleCounter {
		cycle := uint8(head)
		if cycle != p.cycle { // no cycle check for 0xc0 to avoid messages on every target reset and when no cycle counter is active
			n += copy(b, fmt.Sprintln("CYCLE:", cycle, "not equal expected value", p.cycle, "- adjusting. Now", emitter.ColorChannelEvents("CYCLE")+1, "CycleEvents"))
			p.cycle = cycle // adjust cycle
		}
		p.cycle++
	}
	u32Cnt := int(uint8(head >> 8))
	triceID := id.TriceID(uint16(head >> 16))
	LastTriceID = triceID // used for showID
	triceSize := 4 + 4*u32Cnt
	if len(p.b) < triceSize {
		n += copy(b[n:], fmt.Sprintln("ERROR:package len", len(p.b), "is too short - ignoring package", p.b))
		n += copy(b[n:], fmt.Sprintln(hints))
		return
	}
	var ok bool
	p.lutMutex.RLock()
	p.trice, ok = p.lut[triceID]
	p.lutMutex.RUnlock()
	if !ok { // unknown id
		n += copy(b[n:], fmt.Sprintln("WARNING:unknown ID ", triceID, "- ignoring trice", p.b[:triceSize]))
		n += copy(b[n:], fmt.Sprintln(hints))
		p.b = p.b[triceSize:]
		return
	}
	expU32Cnt := p.expectedU32Count()
	if expU32Cnt == -1 {
		n += copy(b[n:], fmt.Sprintln("err:Unknown trice.Type ", p.trice.Type, "- ignoring trice", p.b[:triceSize]))
		n += copy(b[n:], fmt.Sprintln(hints))
		p.b = p.b[triceSize:]
		return
	}
	if expU32Cnt == -2 { // no check for strings here
		expU32Cnt = u32Cnt
	}
	if expU32Cnt != u32Cnt {
		n += copy(b[n:], fmt.Sprintln("err:trice.Type ", p.trice.Type, " with not matching parameter uint32 count ", u32Cnt, "- ignoring trice", p.b[:triceSize]))
		n += copy(b[n:], fmt.Sprintln(hints))
		p.b = p.b[triceSize:]
		return
	}
	p.b = p.b[4:]
	n += p.sprintTrice(b[n:])
	p.b = p.b[triceSize-4:]
	return
}

// expectedU32Count returns expected uint32 count for triceType.
// It returns -2 for an unknown value (strings) and -1 for unknown triceType.
func (p *COBSR) expectedU32Count() int {
	switch p.trice.Type {
	case "TRICE0":
		return 0
	case "TRICE8_1":
		return 1
	case "TRICE8_2", "TRICE16_1":
		return 1
	case "TRICE8_3":
		return 1
	case "TRICE8_4", "TRICE16_2", "TRICE32_1":
		return 1
	case "TRICE8_5":
		return 2
	case "TRICE8_6", "TRICE16_3":
		return 2
	case "TRICE8_7":
		return 2
	case "TRICE8_8", "TRICE16_4", "TRICE32_2", "TRICE64_1":
		return 2
	case "TRICE32_3":
		return 3
	case "TRICE32_4", "TRICE64_2":
		return 4
	case "TRICE_S":
		return -2 // fake value for the check. To do: Check len with transmitted length.
	default:
		return -1 // unknown trice type
	}
}

type triceTypeFn struct {
	triceType string
	triceFn   func(p *COBSR, b []byte) int
}

// cobsFunctionPtrList is a function pointer list.
var cobsFunctionPtrList = []triceTypeFn{
	{"TRICE0", (*COBSR).trice0},
	{"TRICE8_1", (*COBSR).trice81},
	{"TRICE8_2", (*COBSR).trice82},
	{"TRICE8_3", (*COBSR).trice83},
	{"TRICE8_4", (*COBSR).trice84},
	{"TRICE8_5", (*COBSR).trice85},
	{"TRICE8_6", (*COBSR).trice86},
	{"TRICE8_7", (*COBSR).trice87},
	{"TRICE8_8", (*COBSR).trice88},
	{"TRICE16_1", (*COBSR).trice161},
	{"TRICE16_2", (*COBSR).trice162},
	{"TRICE16_3", (*COBSR).trice163},
	{"TRICE16_4", (*COBSR).trice164},
	{"TRICE32_1", (*COBSR).trice321},
	{"TRICE32_2", (*COBSR).trice322},
	{"TRICE32_3", (*COBSR).trice323},
	{"TRICE32_4", (*COBSR).trice324},
	{"TRICE64_1", (*COBSR).trice641},
	{"TRICE64_2", (*COBSR).trice642},
}

func (p *COBSR) sprintTrice(b []byte) int {
	if p.trice.Type == "TRICE_S" { // special case
		return p.triceS(b)
	}
	for _, s := range cobsFunctionPtrList {
		if s.triceType == p.trice.Type {
			return s.triceFn(p, b)
		}
	}
	return copy(b, fmt.Sprintf("err:Unexpected trice.Type %s\n", p.trice.Type))
}

func (p *COBSR) triceS(b []byte) int {
	if DebugOut {
		fmt.Println(p.b)
	}
	var len uint32
	if p.endian == LittleEndian {
		len = binary.LittleEndian.Uint32(p.b[0:4])
	} else {
		len = binary.BigEndian.Uint32(p.b[0:4])
	}
	//p.b = p.b[4:] // The first 4 bytes are the payload length, drop length info.
	s := p.b[4:len]
	return copy(b, fmt.Sprintf(p.trice.Strg, string(s)))
}

func (p *COBSR) trice0(b []byte) int {
	return copy(b, fmt.Sprintf(p.trice.Strg))
}

func (p *COBSR) trice81(b []byte) int {
	b0 := int8(p.b[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, b0))
}

func (p *COBSR) trice82(b []byte) int {
	b0 := int8(p.b[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b1 := int8(p.b[1]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, b0, b1))
}

func (p *COBSR) trice83(b []byte) int {
	b0 := int8(p.b[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b1 := int8(p.b[1]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b2 := int8(p.b[2]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, b0, b1, b2))
}

func (p *COBSR) trice84(b []byte) int {
	b0 := int8(p.b[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b1 := int8(p.b[1]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b2 := int8(p.b[2]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b3 := int8(p.b[3]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, b0, b1, b2, b3))
}

func (p *COBSR) trice85(b []byte) int {
	b0 := int8(p.b[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b1 := int8(p.b[1]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b2 := int8(p.b[2]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b3 := int8(p.b[3]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b4 := int8(p.b[4]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, b0, b1, b2, b3, b4))
}

func (p *COBSR) trice86(b []byte) int {
	b0 := int8(p.b[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b1 := int8(p.b[1]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b2 := int8(p.b[2]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b3 := int8(p.b[3]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b4 := int8(p.b[4]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b5 := int8(p.b[5]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, b0, b1, b2, b3, b4, b5))
}

func (p *COBSR) trice87(b []byte) int {
	b0 := int8(p.b[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b1 := int8(p.b[1]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b2 := int8(p.b[2]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b3 := int8(p.b[3]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b4 := int8(p.b[4]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b5 := int8(p.b[5]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b6 := int8(p.b[6]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, b0, b1, b2, b3, b4, b5, b6))
}

func (p *COBSR) trice88(b []byte) int {
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

func (p *COBSR) trice161(b []byte) int {
	d0 := int16(p.readU16(p.b[0:2])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, d0))
}

func (p *COBSR) trice162(b []byte) int {
	d0 := int16(p.readU16(p.b[0:2])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int16(p.readU16(p.b[2:4])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, d0, d1))
}

func (p *COBSR) trice163(b []byte) int {
	d0 := int16(p.readU16(p.b[0:2])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int16(p.readU16(p.b[2:4])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d2 := int16(p.readU16(p.b[4:6])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, d0, d1, d2))
}

func (p *COBSR) trice164(b []byte) int {
	d0 := int16(p.readU16(p.b[0:2])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int16(p.readU16(p.b[2:4])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d2 := int16(p.readU16(p.b[4:6])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d3 := int16(p.readU16(p.b[6:8])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, d0, d1, d2, d3))
}

func (p *COBSR) trice321(b []byte) int {
	d0 := int32(p.readU32(p.b[0:4])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, d0))
}

func (p *COBSR) trice322(b []byte) int {
	d0 := int32(p.readU32(p.b[0:4])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int32(p.readU32(p.b[4:8])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, d0, d1))
}

func (p *COBSR) trice323(b []byte) int {
	d0 := int32(p.readU32(p.b[0:4]))  // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int32(p.readU32(p.b[4:8]))  // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d2 := int32(p.readU32(p.b[8:12])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, d0, d1, d2))
}

func (p *COBSR) trice324(b []byte) int {
	d0 := int32(p.readU32(p.b[0:4]))   // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int32(p.readU32(p.b[4:8]))   // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d2 := int32(p.readU32(p.b[8:12]))  // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d3 := int32(p.readU32(p.b[12:16])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, d0, d1, d2, d3))
}

func (p *COBSR) trice641(b []byte) int {
	d0 := int64(p.readU64(p.b[0:8])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, d0))
}

func (p *COBSR) trice642(b []byte) int {
	d0 := int64(p.readU64(p.b[0:8]))  // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int64(p.readU64(p.b[8:16])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	return copy(b, fmt.Sprintf(p.trice.Strg, d0, d1))
}
