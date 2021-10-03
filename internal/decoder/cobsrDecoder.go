// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package decoder

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"io"
	"strings"
	"sync"

	"github.com/dim13/cobs"
	"github.com/rokath/trice/internal/id"
)

var (
	debugOut         = false
	MinPackageLength int
)

// COBSR is the Decoding instance for COBSR encoded trices.
type COBSR struct {
	decoderData
	cycle uint8
	bc    int // trice specific bytes count
}

// NewEscDecoder provides an EscDecoder instance.
// l is the trice id list in slice of struct format.
// in is the usable reader for the input bytes.
func NewCOBSRDecoder(lut id.TriceIDLookUp, m *sync.RWMutex, in io.Reader, endian bool) Decoder {
	p := &COBSR{}
	p.cycle = 0xc0 // start value
	p.in = in
	p.iBuf = make([]byte, 0, defaultSize)
	p.lut = lut
	p.lutMutex = m
	p.endian = endian
	return p
}

// Read is the provided read method for COBS decoding of next string as byte slice.
// It uses inner reader p.in and internal id look-up table to fill b with a string.
// b is a slice of bytes with a len for the max expected string size.
// n is the count of read bytes inside b.
// Read returns usually one trice string or nothing.
// In case of a not matching cycle, a warning message in trice format is prefixed.
// In case of invalid package data, error messages in trice format are returned and the package is dropped.
func (p *COBSR) Read(b []byte) (n int, err error) {
	sizeMsg := fmt.Sprintln("e:buf too small, expecting", defaultSize, "bytes.")
	if len(b) < len(sizeMsg) {
		return
	}
	if len(b) < defaultSize {
		n = copy(b, sizeMsg)
		return
	}

	// use b as intermediate read buffer to avoid allocation
	n, err = p.in.Read(b)
	// p.iBuf can contain unprocessed bytes from last call.
	p.iBuf = append(p.iBuf, b[:n]...) // merge with leftovers

	// SEEMS NOT TO WORK
	//  buf := make([]byte, 0, defaultSize)
	//  n, err = p.in.Read(buf)
	//  // p.iBuf can contain unprocessed bytes from last call.
	//  p.iBuf = append(p.iBuf, buf[:n]...) // merge with leftovers

	n = 0
	if nil != err && io.EOF != err {
		n = copy(b, fmt.Sprintln("error:internal reader error ", err))
		return
	}

	// Even err could be io.EOF, some valid data possibly in p.iBUf.
	// In case of file input (J-LINK usage) a plug off is not detectable here.

	index := bytes.IndexByte(p.iBuf, 0) // find terminating 0
	if index == -1 {
		return 0, io.EOF // no terminating 0
	}

	d := cobs.Decode(p.iBuf[:index+1])
	if len(d) < MinPackageLength {
		p.iBuf = p.iBuf[index+1:] // step forward
		n += copy(b[n:], fmt.Sprintln("err:package len", len(d), "is too short - ignoring package."))
		n += copy(b[n:], fmt.Sprintln("att:Hint:Target buffer overflow?"))
		return
	}

	if debugOut { // Debug output
		for _, x := range p.iBuf[:index+1] {
			fmt.Printf("%02x ", x)
		}
		fmt.Print("\n-> ")
		for _, x := range d {
			fmt.Printf("%02x ", x)
		}
		fmt.Println("")
	}

	p.iBuf = p.iBuf[index+1:] // step forward

	var triceID id.TriceID
	if CycleCounter {
		cycle := d[0] // little endian target
		if cycle != p.cycle {
			n += copy(b, fmt.Sprintln("wrn:Cycle", cycle, "not equal expected value", p.cycle, "- adjusting cycle."))
			p.cycle = cycle // adjust cycle
		}
		if debugOut { // Debug output
			n += copy(b[n:], fmt.Sprintln("dbg:-> cycle", cycle))
		}
		p.cycle++
		d = d[1:] // drop cycle count
	}
	triceID = id.TriceID(binary.LittleEndian.Uint16(d[:2]))
	p.b = d[2:] // drop id and transfer values
	p.bc = len(p.b)
	if debugOut { // Debug output
		n += copy(b[n:], fmt.Sprintln("dbg:-> id", triceID, "byteCount", p.bc))
	}

	var ok bool
	p.lutMutex.RLock()
	p.trice, ok = p.lut[triceID]
	p.lutMutex.RUnlock()
	if !ok { // unknown id
		n += copy(b[n:], fmt.Sprintln("wrn:unknown ID ", triceID, "- ignoring package."))
		n += copy(b[n:], fmt.Sprintln("att:Hint:Target buffer overflow?"))
		return
	}

	p.upperCaseTriceType = strings.ToUpper(p.trice.Type) // for trice* too

	if p.expectedByteCount() != p.bc {
		n += copy(b[n:], fmt.Sprintln("err:trice.Type ", p.trice.Type, " with not matching parameter byte count ", p.bc, "- ignoring package"))
		n += copy(b[n:], fmt.Sprintln("att:Hint:Target buffer overflow?"))
		return
	}

	o := n
	n, err = p.sprintTrice(b[o:])
	n += o
	return
}

// byteCount returns expected byte count for triceType.
// It returns -1 for an unknown value an -2 for unknown triceType.
func (p *COBSR) expectedByteCount() int {
	switch p.upperCaseTriceType {
	case "TRICE0":
		return 0
	case "TRICE8_1":
		return 1
	case "TRICE8_2", "TRICE16_1":
		return 2
	case "TRICE8_3":
		return 3
	case "TRICE8_4", "TRICE16_2", "TRICE32_1":
		return 4
	case "TRICE8_5":
		return 5
	case "TRICE8_6", "TRICE16_3":
		return 6
	case "TRICE8_7":
		return 7
	case "TRICE8_8", "TRICE16_4", "TRICE32_2", "TRICE64_1":
		return 8
	case "TRICE32_3":
		return 12
	case "TRICE32_4", "TRICE64_2":
		return 16
	case "TRICE_S":
		return p.bc
	default:
		return -1 // unknown trice type
	}
}

type triceTypeFn struct {
	triceType string
	triceFn   func(p *COBSR, b []byte) (int, error)
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

func (p *COBSR) sprintTrice(b []byte) (n int, e error) {
	if "TRICE_S" == p.trice.Type { // special case
		return p.triceS(b)
	}
	// p.rub(4) // remove header

	for _, s := range cobsFunctionPtrList {
		if s.triceType == p.upperCaseTriceType {
			return s.triceFn(p, b)
		}
	}
	n = copy(b, fmt.Sprintf("err:Unexpected trice.Type %s\n", p.trice.Type))
	return
}

func (p *COBSR) triceS(b []byte) (n int, e error) {
	n = copy(b, fmt.Sprintf(p.trice.Strg, string(p.b)))
	return
}

func (p *COBSR) trice0(b []byte) (n int, e error) {
	n = copy(b, fmt.Sprintf(p.trice.Strg))
	return
}

func (p *COBSR) trice81(b []byte) (n int, e error) {
	b0 := int8(p.b[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(b, fmt.Sprintf(p.trice.Strg, b0))
	return
}

func (p *COBSR) trice82(b []byte) (n int, e error) {
	b0 := int8(p.b[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b1 := int8(p.b[1]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(b, fmt.Sprintf(p.trice.Strg, b0, b1))
	return
}

func (p *COBSR) trice83(b []byte) (n int, e error) {
	b0 := int8(p.b[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b1 := int8(p.b[1]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b2 := int8(p.b[2]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(b, fmt.Sprintf(p.trice.Strg, b0, b1, b2))
	return
}

func (p *COBSR) trice84(b []byte) (n int, e error) {
	b0 := int8(p.b[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b1 := int8(p.b[1]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b2 := int8(p.b[2]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b3 := int8(p.b[3]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(b, fmt.Sprintf(p.trice.Strg, b0, b1, b2, b3))
	return
}

func (p *COBSR) trice85(b []byte) (n int, e error) {
	b0 := int8(p.b[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b1 := int8(p.b[1]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b2 := int8(p.b[2]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b3 := int8(p.b[3]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b4 := int8(p.b[4]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(b, fmt.Sprintf(p.trice.Strg, b0, b1, b2, b3, b4))
	return
}

func (p *COBSR) trice86(b []byte) (n int, e error) {
	b0 := int8(p.b[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b1 := int8(p.b[1]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b2 := int8(p.b[2]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b3 := int8(p.b[3]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b4 := int8(p.b[4]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b5 := int8(p.b[5]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(b, fmt.Sprintf(p.trice.Strg, b0, b1, b2, b3, b4, b5))
	return
}

func (p *COBSR) trice87(b []byte) (n int, e error) {
	b0 := int8(p.b[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b1 := int8(p.b[1]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b2 := int8(p.b[2]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b3 := int8(p.b[3]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b4 := int8(p.b[4]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b5 := int8(p.b[5]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b6 := int8(p.b[6]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(b, fmt.Sprintf(p.trice.Strg, b0, b1, b2, b3, b4, b5, b6))
	return
}

func (p *COBSR) trice88(b []byte) (n int, e error) {
	b0 := int8(p.b[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b1 := int8(p.b[1]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b2 := int8(p.b[2]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b3 := int8(p.b[3]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b4 := int8(p.b[4]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b5 := int8(p.b[5]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b6 := int8(p.b[6]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b7 := int8(p.b[7]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, b0, b1, b2, b3, b4, b5, b6, b7))
	return
}

func (p *COBSR) trice161(b []byte) (n int, e error) {
	d0 := int16(p.readU16(p.b[0:2])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(b, fmt.Sprintf(p.trice.Strg, d0))
	return
}

func (p *COBSR) trice162(b []byte) (n int, e error) {
	d0 := int16(p.readU16(p.b[0:2])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int16(p.readU16(p.b[2:4])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(b, fmt.Sprintf(p.trice.Strg, d0, d1))
	return
}

func (p *COBSR) trice163(b []byte) (n int, e error) {
	d0 := int16(p.readU16(p.b[0:2])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int16(p.readU16(p.b[2:4])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d2 := int16(p.readU16(p.b[4:6])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(b, fmt.Sprintf(p.trice.Strg, d0, d1, d2))
	return
}

func (p *COBSR) trice164(b []byte) (n int, e error) {
	d0 := int16(p.readU16(p.b[0:2])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int16(p.readU16(p.b[2:4])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d2 := int16(p.readU16(p.b[4:6])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d3 := int16(p.readU16(p.b[6:8])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0, d1, d2, d3))
	return
}

func (p *COBSR) trice321(b []byte) (n int, e error) {
	d0 := int32(p.readU32(p.b[0:4])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(b, fmt.Sprintf(p.trice.Strg, d0))
	return
}

func (p *COBSR) trice322(b []byte) (n int, e error) {
	d0 := int32(p.readU32(p.b[0:4])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int32(p.readU32(p.b[4:8])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(b, fmt.Sprintf(p.trice.Strg, d0, d1))
	return
}

func (p *COBSR) trice323(b []byte) (n int, e error) {
	d0 := int32(p.readU32(p.b[0:4]))  // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int32(p.readU32(p.b[4:8]))  // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d2 := int32(p.readU32(p.b[8:12])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(b, fmt.Sprintf(p.trice.Strg, d0, d1, d2))
	return
}

func (p *COBSR) trice324(b []byte) (n int, e error) {
	d0 := int32(p.readU32(p.b[0:4]))   // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int32(p.readU32(p.b[4:8]))   // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d2 := int32(p.readU32(p.b[8:12]))  // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d3 := int32(p.readU32(p.b[12:16])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(b, fmt.Sprintf(p.trice.Strg, d0, d1, d2, d3))
	return
}

func (p *COBSR) trice641(b []byte) (n int, e error) {
	d0 := int64(p.readU64(p.b[0:8])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(b, fmt.Sprintf(p.trice.Strg, d0))
	return
}

func (p *COBSR) trice642(b []byte) (n int, e error) {
	d0 := int64(p.readU64(p.b[0:8]))  // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int64(p.readU64(p.b[8:16])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(b, fmt.Sprintf(p.trice.Strg, d0, d1))
	return
}
