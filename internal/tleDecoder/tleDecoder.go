// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package tleDecoder

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
	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/pkg/cipher"
)

const (
	// headSize is 4; each trice message starts with a head of 4 bytes.
	headSize = 4
)

// cobsDec is the Decoding instance for cobsDec encoded trices.
type cobsDec struct {
	decoder.DecoderData
	cycle              uint8  // cycle date: c0...bf
	COBSModeDescriptor uint32 // 0: no target timestamps, 1: target timestamps exist
	pFmt               string // modified trice format string: %u -> %d
	u                  []int  // 1: modified format string positions:  %u -> %d, 2: float (%f)
}

// New provides a COBS decoder instance.
//
// l is the trice id list in slice of struct format.
// in is the usable reader for the input bytes.
func New(w io.Writer, lut id.TriceIDLookUp, m *sync.RWMutex, li id.TriceIDLookUpLI, in io.Reader, endian bool) decoder.Decoder {
	p := &cobsDec{}
	p.cycle = 0xc0 // start value
	p.W = w
	p.In = in
	p.IBuf = make([]byte, 0, decoder.DefaultSize)
	p.B = make([]byte, 0, decoder.DefaultSize)
	p.Lut = lut
	p.LutMutex = m
	p.Li = li
	p.Endian = endian
	return p
}

// nextPackage reads with an inner reader a COBS encoded byte stream.
//
// When no terminating 0 is found in the incoming bytes nextPackage returns without action.
// That means the incoming data stream is exhausted and a next try should be started a bit later.
// Some arrived bytes are kept internally and concatenated with the following bytes in a next Read.
// When a terminating 0 is found in the incoming bytes ReadFromCOBS decodes the COBS package
// and returns it in b and its len in n. If more data arrived after the first terminating 0,
// these are kept internally and concatenated with the following bytes in a next Read.
func (p *cobsDec) nextPackage() {
	// Here p.IBuf contains none or available bytes, what can be several trice messages.
	// So first try to process p.IBuf.
	index := bytes.IndexByte(p.IBuf, 0) // find terminating 0
	if index == -1 {                    // p.IBuf has no complete COBS data, so try to read more input
		bb := make([]byte, 1024)           // intermediate buffer
		m, err := p.In.Read(bb)            // use bb as bytes read buffer
		p.IBuf = append(p.IBuf, bb[:m]...) // merge with leftovers
		if err != nil && err != io.EOF {   // some serious error
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
	// here a complete COBS package exists
	if decoder.DebugOut { // Debug output
		fmt.Fprint(p.W, "COBS: ")
		decoder.Dump(p.W, p.IBuf[:index+1])
	}

	p.B = make([]byte, decoder.DefaultSize)
	n, e := cobs.Decode(p.B, p.IBuf[:index])
	if e != nil {
		fmt.Println("inconsistent COBS buffer:", p.IBuf[:index+1])
	}
	p.IBuf = p.IBuf[index+1:] // step forward (next package data in p.IBuf now, if any)
	p.B = p.B[:n]             // decoded trice COBS packages have a multiple of 4 len
	if n&3 != 0 {
		decoder.Dump(p.W, p.B)
		fmt.Fprintln(p.W, "ERROR:Decoded trice COBS package has not expected  multiple of 4 len. The len is", n) // exit
		n = 0
		p.B = p.B[:0]
		return
	}

	if decoder.DebugOut { // Debug output
		fmt.Fprint(p.W, "-> PKG:  ")
		decoder.Dump(p.W, p.B)
	}

	if cipher.Password != "" { // encrypted
		cipher.Decrypt(p.B, p.B)
		if decoder.DebugOut { // Debug output
			fmt.Fprint(p.W, "-> DEC:  ")
			decoder.Dump(p.W, p.B)
		}
	}

	if n >= 4 {
		p.COBSModeDescriptor = p.ReadU32(p.B)
		p.B = p.B[4:] // drop COBS package descriptor
	}
}

func (p *cobsDec) handleCOBSModeDescriptor() error {
	switch p.COBSModeDescriptor {
	case 0: // nothing to do
		decoder.TargetTimestampSize = 0
		decoder.TargetLocationExists = false
		return nil
	case 1:
		decoder.TargetTimestamp = uint64(p.ReadU32(p.B))
		decoder.TargetTimestampSize = 4
		decoder.TargetLocationExists = false
		p.B = p.B[4:] // drop target timestamp
		return nil
	case 2:
		decoder.TargetLocation = p.ReadU32(p.B)
		decoder.TargetTimestampSize = 0
		decoder.TargetLocationExists = true
		p.B = p.B[4:] // drop target location
		return nil
	case 3:
		decoder.TargetLocation = p.ReadU32(p.B)
		decoder.TargetTimestamp = uint64(p.ReadU32(p.B[4:]))
		decoder.TargetTimestampSize = 4
		decoder.TargetLocationExists = true
		p.B = p.B[8:] // drop target location & timestamp
		return nil
	}
	err := fmt.Errorf("Info:Unknown COBS packet with descriptor 0x%08x and len %d \n(((as ASCII:\"%s\")))", p.COBSModeDescriptor, len(p.B), string(p.B))
	p.B = p.B[:0] // clear buffer
	return err
}

// Read is the provided read method for COBS decoding and provides next string as byte slice.
//
// It uses inner reader p.in and internal id look-up table to fill b with a string.
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
func (p *cobsDec) Read(b []byte) (n int, err error) {
	minPkgSize := headSize
	if decoder.TargetTimestampSize > 0 {
		minPkgSize += 4
	}
	if decoder.TargetLocationExists {
		minPkgSize += 4
	}
	if len(p.B) < minPkgSize { // last decoded COBS package exhausted
		p.nextPackage()
	}
	if len(p.B) < minPkgSize { // not enough data for a next package
		return
	}

	// Inside p.pkg is here one or a partial package, what means one or more trice messages.
	if len(p.B) < 4 {
		n += copy(b[n:], fmt.Sprintln("ERROR:package len", len(p.B), "is too short - ignoring package", p.B))
		n += copy(b[n:], fmt.Sprintln(decoder.Hints))
		return
	}
	err = p.handleCOBSModeDescriptor()
	if err != nil {
		n += copy(b[n:], fmt.Sprintln(err))
		return // ignore package
	}
	head := p.ReadU32(p.B)

	// cycle counter automatic & check
	cycle := uint8(head)
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

	p.ParamSpace = int((0x0000FF00 & head) >> 6)
	p.TriceSize = headSize + p.ParamSpace
	triceID := id.TriceID(uint16(head >> 16))
	decoder.LastTriceID = triceID // used for showID
	if len(p.B) < p.TriceSize {
		n += copy(b[n:], fmt.Sprintln("ERROR:package len", len(p.B), "is <", p.TriceSize, " - ignoring package", p.B))
		n += copy(b[n:], fmt.Sprintln(decoder.Hints))
		p.B = p.B[len(p.B):]
		return
	}
	if decoder.DebugOut {
		fmt.Fprint(p.W, "TRICE -> ")
		decoder.Dump(p.W, p.B[:p.TriceSize])
	}
	var ok bool
	p.LutMutex.RLock()
	p.Trice, ok = p.Lut[triceID]
	p.LutMutex.RUnlock()
	if !ok {
		n += copy(b[n:], fmt.Sprintln("WARNING:unknown ID ", triceID, "- ignoring trice", p.B[:p.TriceSize]))
		n += copy(b[n:], fmt.Sprintln(decoder.Hints))
		p.B = p.B[p.TriceSize:]
		return
	}
	p.B = p.B[headSize:] // drop used head info

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
func (p *cobsDec) sprintTrice(b []byte) (n int) {

	if p.Trice.Type == "TRICE_S" { // patch table ParamSpace in that case
		p.SLen = int(p.ReadU32(p.B))
		cobsFunctionPtrList[0].ParamSpace = (p.SLen + 7) & ^3 // +4 for 4 bytes sLen, +3^3 is alignment to 4
	}
	if p.Trice.Type == "TRICE_N" { // patch table ParamSpace in that case
		p.SLen = int(p.ReadU32(p.B))
		cobsFunctionPtrList[1].ParamSpace = (p.SLen + 7) & ^3 // +4 for 4 bytes sLen, +3^3 is alignment to 4
	}
	if p.Trice.Type == "TRICE_B" { // patch table ParamSpace in that case
		p.SLen = int(p.ReadU32(p.B))
		cobsFunctionPtrList[2].ParamSpace = (p.SLen + 7) & ^3 // +4 for 4 bytes sLen, +3^3 is alignment to 4
	}
	if p.Trice.Type == "TRICE8_B" { // patch table ParamSpace in that case
		p.SLen = int(p.ReadU32(p.B))
		cobsFunctionPtrList[3].ParamSpace = (p.SLen + 7) & ^3 // +4 for 4 bytes sLen, +3^3 is alignment to 4
	}
	if p.Trice.Type == "TRICE16_B" { // patch table ParamSpace in that case
		p.SLen = int(p.ReadU32(p.B))
		cobsFunctionPtrList[4].ParamSpace = (p.SLen + 7) & ^3 // +4 for 4 bytes sLen, +3^3 is alignment to 4
	}
	if p.Trice.Type == "TRICE32_B" { // patch table ParamSpace in that case
		p.SLen = int(p.ReadU32(p.B))
		cobsFunctionPtrList[5].ParamSpace = (p.SLen + 7) & ^3 // +4 for 4 bytes sLen, +3^3 is alignment to 4
	}
	if p.Trice.Type == "TRICE64_B" { // patch table ParamSpace in that case
		p.SLen = int(p.ReadU32(p.B))
		cobsFunctionPtrList[6].ParamSpace = (p.SLen + 7) & ^3 // +4 for 4 bytes sLen, +3^3 is alignment to 4
	}
	if p.Trice.Type == "TRICE_F" { // patch table ParamSpace in that case
		p.SLen = int(p.ReadU32(p.B))
		cobsFunctionPtrList[7].ParamSpace = (p.SLen + 7) & ^3 // +4 for 4 bytes sLen, +3^3 is alignment to 4
	}
	if p.Trice.Type == "TRICE8_F" { // patch table ParamSpace in that case
		p.SLen = int(p.ReadU32(p.B))
		cobsFunctionPtrList[8].ParamSpace = (p.SLen + 7) & ^3 // +4 for 4 bytes sLen, +3^3 is alignment to 4
	}
	if p.Trice.Type == "TRICE16_F" { // patch table ParamSpace in that case
		p.SLen = int(p.ReadU32(p.B))
		cobsFunctionPtrList[9].ParamSpace = (p.SLen + 7) & ^3 // +4 for 4 bytes sLen, +3^3 is alignment to 4
	}
	if p.Trice.Type == "TRICE32_F" { // patch table ParamSpace in that case
		p.SLen = int(p.ReadU32(p.B))
		cobsFunctionPtrList[10].ParamSpace = (p.SLen + 7) & ^3 // +4 for 4 bytes sLen, +3^3 is alignment to 4
	}
	if p.Trice.Type == "TRICE64_F" { // patch table ParamSpace in that case
		p.SLen = int(p.ReadU32(p.B))
		cobsFunctionPtrList[11].ParamSpace = (p.SLen + 7) & ^3 // +4 for 4 bytes sLen, +3^3 is alignment to 4
	}

	p.pFmt, p.u = decoder.UReplaceN(p.Trice.Strg)

	triceType := "unknownTriceType" // need to reconstruct full TRICE info, if not exist in type string
	for _, name := range []string{"TRICE", "TRice", "Trice", "trice"} {
		if strings.HasPrefix(p.Trice.Type, name+"_") { // when no bitwidth, insert it
			triceType = name + id.DefaultTriceBitWidth + "_" + p.Trice.Type[6:]
		}
		if p.Trice.Type == name { // when nothing
			triceType = fmt.Sprintf("TRICE"+id.DefaultTriceBitWidth+"_%d", len(p.u)) // append bitwidth and count
		}
		//  if p.Trice.Type == name && len(p.u) == 0 { // special case, overwrite
		//  	triceType = name + "0"
		//  }
		if p.Trice.Type == name+"8" || p.Trice.Type == name+"16" || p.Trice.Type == name+"32" || p.Trice.Type == name+"64" { // when no count
			triceType = fmt.Sprintf(p.Trice.Type+"_%d", len(p.u)) // append count
		}
	}
	for _, s := range cobsFunctionPtrList { // walk through the list and try to find a match for execution
		if s.triceType == p.Trice.Type || s.triceType == triceType { // match list entry "TRICE..."
			if s.ParamSpace == p.ParamSpace { // size ok
				if len(p.B) < p.ParamSpace {
					n += copy(b[n:], fmt.Sprintln("err:len(p.B) =", len(p.B), "< p.ParamSpace = ", p.ParamSpace, "- ignoring package", p.B[:len(p.B)]))
					n += copy(b[n:], fmt.Sprintln(decoder.Hints))
					return
				}
				n += s.triceFn(p, b, s.bitWidth, s.paramCount) // match found, call handler
				return
			} else { // size error
				n += copy(b[n:], fmt.Sprintln("err:trice.Type", p.Trice.Type, ": s.ParamSpace", s.ParamSpace, "!= p.ParamSpace", p.ParamSpace, "- ignoring data", p.B[:p.ParamSpace]))
				n += copy(b[n:], fmt.Sprintln(decoder.Hints))
				return
			}
		}
	}
	n += copy(b[n:], fmt.Sprintln("err:Unknown trice.Type:", p.Trice.Type, "and", triceType, "not matching - ignoring trice data", p.B[:p.ParamSpace]))
	n += copy(b[n:], fmt.Sprintln(decoder.Hints))
	return
}

// triceTypeFn is the type for cobsFunctionPtrList elements.
type triceTypeFn struct {
	triceType  string                                              // triceType describes if parameters, the parameter bit width or if the parameter is a string.
	triceFn    func(p *cobsDec, b []byte, bitwidth, count int) int // triceFn performs the conversion to the output string.
	ParamSpace int                                                 // ParamSpace is the count of bytes allocated for the parameters.
	bitWidth   int                                                 // bitWidth is the individual parameter width.
	paramCount int                                                 // paramCount is the amount pf parameters for the format string, which must match the count of format specifiers.
}

// cobsFunctionPtrList is a function pointer list.
var cobsFunctionPtrList = [...]triceTypeFn{
	{"TRICE_S", (*cobsDec).triceS, -1, 0, 0},     // do not remove from first position, see cobsFunctionPtrList[0].ParamSpace = ...
	{"TRICE_N", (*cobsDec).triceN, -1, 0, 0},     // do not remove from 2nd position, see cobsFunctionPtrList[1].ParamSpace = ...
	{"TRICE_B", (*cobsDec).trice8B, -1, 0, 0},    // do not remove from 3rd position, see cobsFunctionPtrList[2].ParamSpace = ...
	{"TRICE8_B", (*cobsDec).trice8B, -1, 0, 0},   // do not remove from 4th position, see cobsFunctionPtrList[3].ParamSpace = ...
	{"TRICE16_B", (*cobsDec).trice16B, -1, 0, 0}, // do not remove from 4th position, see cobsFunctionPtrList[4].ParamSpace = ...
	{"TRICE32_B", (*cobsDec).trice32B, -1, 0, 0}, // do not remove from 4th position, see cobsFunctionPtrList[5].ParamSpace = ...
	{"TRICE64_B", (*cobsDec).trice64B, -1, 0, 0}, // do not remove from 4th position, see cobsFunctionPtrList[6].ParamSpace = ...

	{"TRICE_F", (*cobsDec).trice8F, -1, 0, 0},    // do not remove from 4th position, see cobsFunctionPtrList[7].ParamSpace = ...
	{"TRICE8_F", (*cobsDec).trice8F, -1, 0, 0},   // do not remove from 4th position, see cobsFunctionPtrList[8].ParamSpace = ...
	{"TRICE16_F", (*cobsDec).trice16F, -1, 0, 0}, // do not remove from 4th position, see cobsFunctionPtrList[9].ParamSpace = ...
	{"TRICE32_F", (*cobsDec).trice32F, -1, 0, 0}, // do not remove from 4th position, see cobsFunctionPtrList[10].ParamSpace = ...
	{"TRICE64_F", (*cobsDec).trice64F, -1, 0, 0}, // do not remove from 4th position, see cobsFunctionPtrList[11].ParamSpace = ...

	{"TRICE32_0", (*cobsDec).trice0, 0, 0, 0},
	{"TRICE0", (*cobsDec).trice0, 0, 0, 0},
	{"TRICE8_1", (*cobsDec).unSignedOrSignedOut, 4, 8, 1},
	{"TRICE8_2", (*cobsDec).unSignedOrSignedOut, 4, 8, 2},
	{"TRICE8_3", (*cobsDec).unSignedOrSignedOut, 4, 8, 3},
	{"TRICE8_4", (*cobsDec).unSignedOrSignedOut, 4, 8, 4},
	{"TRICE8_5", (*cobsDec).unSignedOrSignedOut, 8, 8, 5},
	{"TRICE8_6", (*cobsDec).unSignedOrSignedOut, 8, 8, 6},
	{"TRICE8_7", (*cobsDec).unSignedOrSignedOut, 8, 8, 7},
	{"TRICE8_8", (*cobsDec).unSignedOrSignedOut, 8, 8, 8},
	{"TRICE8_9", (*cobsDec).unSignedOrSignedOut, 12, 8, 9},
	{"TRICE8_10", (*cobsDec).unSignedOrSignedOut, 12, 8, 10},
	{"TRICE8_11", (*cobsDec).unSignedOrSignedOut, 12, 8, 11},
	{"TRICE8_12", (*cobsDec).unSignedOrSignedOut, 12, 8, 12},
	{"TRICE16_1", (*cobsDec).unSignedOrSignedOut, 4, 16, 1},
	{"TRICE16_2", (*cobsDec).unSignedOrSignedOut, 4, 16, 2},
	{"TRICE16_3", (*cobsDec).unSignedOrSignedOut, 8, 16, 3},
	{"TRICE16_4", (*cobsDec).unSignedOrSignedOut, 8, 16, 4},
	{"TRICE16_5", (*cobsDec).unSignedOrSignedOut, 12, 16, 5},
	{"TRICE16_6", (*cobsDec).unSignedOrSignedOut, 12, 16, 6},
	{"TRICE16_7", (*cobsDec).unSignedOrSignedOut, 16, 16, 7},
	{"TRICE16_8", (*cobsDec).unSignedOrSignedOut, 16, 16, 8},
	{"TRICE16_9", (*cobsDec).unSignedOrSignedOut, 20, 16, 9},
	{"TRICE16_10", (*cobsDec).unSignedOrSignedOut, 20, 16, 10},
	{"TRICE16_11", (*cobsDec).unSignedOrSignedOut, 24, 16, 11},
	{"TRICE16_12", (*cobsDec).unSignedOrSignedOut, 24, 16, 12},
	{"TRICE32_1", (*cobsDec).unSignedOrSignedOut, 4, 32, 1},
	{"TRICE32_2", (*cobsDec).unSignedOrSignedOut, 8, 32, 2},
	{"TRICE32_3", (*cobsDec).unSignedOrSignedOut, 12, 32, 3},
	{"TRICE32_4", (*cobsDec).unSignedOrSignedOut, 16, 32, 4},
	{"TRICE32_5", (*cobsDec).unSignedOrSignedOut, 20, 32, 5},
	{"TRICE32_6", (*cobsDec).unSignedOrSignedOut, 24, 32, 6},
	{"TRICE32_7", (*cobsDec).unSignedOrSignedOut, 28, 32, 7},
	{"TRICE32_8", (*cobsDec).unSignedOrSignedOut, 32, 32, 8},
	{"TRICE32_9", (*cobsDec).unSignedOrSignedOut, 36, 32, 9},
	{"TRICE32_10", (*cobsDec).unSignedOrSignedOut, 40, 32, 10},
	{"TRICE32_11", (*cobsDec).unSignedOrSignedOut, 44, 32, 11},
	{"TRICE32_12", (*cobsDec).unSignedOrSignedOut, 48, 32, 12},
	{"TRICE64_1", (*cobsDec).unSignedOrSignedOut, 8, 64, 1},
	{"TRICE64_2", (*cobsDec).unSignedOrSignedOut, 16, 64, 2},
	{"TRICE64_3", (*cobsDec).unSignedOrSignedOut, 24, 64, 3},
	{"TRICE64_4", (*cobsDec).unSignedOrSignedOut, 32, 64, 4},
	{"TRICE64_5", (*cobsDec).unSignedOrSignedOut, 40, 64, 5},
	{"TRICE64_6", (*cobsDec).unSignedOrSignedOut, 48, 64, 6},
	{"TRICE64_7", (*cobsDec).unSignedOrSignedOut, 56, 64, 7},
	{"TRICE64_8", (*cobsDec).unSignedOrSignedOut, 64, 64, 8},
	{"TRICE64_9", (*cobsDec).unSignedOrSignedOut, 72, 64, 9},
	{"TRICE64_10", (*cobsDec).unSignedOrSignedOut, 80, 64, 10},
	{"TRICE64_11", (*cobsDec).unSignedOrSignedOut, 88, 64, 11},
	{"TRICE64_12", (*cobsDec).unSignedOrSignedOut, 96, 64, 12},
}

// triceS converts dynamic strings.
func (p *cobsDec) triceS(b []byte, _ int, _ int) int {
	if decoder.DebugOut {
		fmt.Fprintln(p.W, p.B)
	}
	s := p.B[4 : 4+p.SLen]
	return copy(b, fmt.Sprintf(p.Trice.Strg, string(s)))
}

// triceB converts dynamic buffers.
func (p *cobsDec) trice8B(b []byte, _ int, _ int) (n int) {
	if decoder.DebugOut {
		fmt.Fprintln(p.W, p.B)
	}
	s := p.B[4 : 4+p.SLen]
	for i := 0; i < len(s); i++ {
		n += copy(b[n:], fmt.Sprintf(p.Trice.Strg, s[i]))
	}
	return
}

// trice16B converts dynamic buffers.
func (p *cobsDec) trice16B(b []byte, _ int, _ int) (n int) {
	if decoder.DebugOut {
		fmt.Fprintln(p.W, p.B)
	}
	s := p.B[4 : 4+p.SLen]
	for i := 0; i < len(s); i += 2 {
		n += copy(b[n:], fmt.Sprintf(p.Trice.Strg, binary.LittleEndian.Uint16(s[i:])))
	}
	return
}

// trice32B converts dynamic buffers.
func (p *cobsDec) trice32B(b []byte, _ int, _ int) (n int) {
	if decoder.DebugOut {
		fmt.Fprintln(p.W, p.B)
	}
	s := p.B[4 : 4+p.SLen]
	for i := 0; i < len(s); i += 4 {
		n += copy(b[n:], fmt.Sprintf(p.Trice.Strg, binary.LittleEndian.Uint32(s[i:])))
	}
	return
}

// trice64B converts dynamic buffers.
func (p *cobsDec) trice64B(b []byte, _ int, _ int) (n int) {
	if decoder.DebugOut {
		fmt.Fprintln(p.W, p.B)
	}
	s := p.B[4 : 4+p.SLen]
	for i := 0; i < len(s); i += 8 {
		n += copy(b[n:], fmt.Sprintf(p.Trice.Strg, binary.LittleEndian.Uint64(s[i:])))
	}
	return
}

// trice8F display function call with 8-bit parameters.
func (p *cobsDec) trice8F(b []byte, _ int, _ int) (n int) {
	if decoder.DebugOut {
		fmt.Fprintln(p.W, p.B)
	}
	s := p.B[4 : 4+p.SLen]
	n += copy(b[n:], fmt.Sprintf(p.Trice.Strg))
	for i := 0; i < len(s); i++ {
		n += copy(b[n:], fmt.Sprintf("(%02x)", s[i]))
	}
	n += copy(b[n:], fmt.Sprintf("\n"))
	return
}

// trice16F display function call with 16-bit parameters.
func (p *cobsDec) trice16F(b []byte, _ int, _ int) (n int) {
	if decoder.DebugOut {
		fmt.Fprintln(p.W, p.B)
	}
	s := p.B[4 : 4+p.SLen]
	n += copy(b[n:], fmt.Sprintf(p.Trice.Strg))
	for i := 0; i < len(s); i += 2 {
		n += copy(b[n:], fmt.Sprintf("(%04x)", binary.LittleEndian.Uint16(s[i:])))
	}
	n += copy(b[n:], fmt.Sprintf("\n"))
	return
}

// trice32F display function call with 32-bit parameters.
func (p *cobsDec) trice32F(b []byte, _ int, _ int) (n int) {
	if decoder.DebugOut {
		fmt.Fprintln(p.W, p.B)
	}
	s := p.B[4 : 4+p.SLen]
	n += copy(b[n:], fmt.Sprintf(p.Trice.Strg))
	for i := 0; i < len(s); i += 4 {
		n += copy(b[n:], fmt.Sprintf("(%08x)", binary.LittleEndian.Uint32(s[i:])))
	}
	n += copy(b[n:], fmt.Sprintf("\n"))
	return
}

// trice64F display function call with 64-bit parameters.
func (p *cobsDec) trice64F(b []byte, _ int, _ int) (n int) {
	if decoder.DebugOut {
		fmt.Fprintln(p.W, p.B)
	}
	s := p.B[4 : 4+p.SLen]
	n += copy(b[n:], fmt.Sprintf(p.Trice.Strg))
	for i := 0; i < len(s); i += 8 {
		n += copy(b[n:], fmt.Sprintf("(%016x)", binary.LittleEndian.Uint64(s[i:])))
	}
	n += copy(b[n:], fmt.Sprintf("\n"))
	return
}

// triceN converts dynamic strings.
func (p *cobsDec) triceN(b []byte, _ int, _ int) int {
	if decoder.DebugOut {
		fmt.Fprintln(p.W, p.B)
	}
	s := p.B[4 : 4+p.SLen]
	// todo: evaluate p.Trice.Strg, use p.SLen and do whatever should be done
	return copy(b, fmt.Sprintf(p.Trice.Strg, string(s)))
}

// trice0 prints the trice format string.
func (p *cobsDec) trice0(b []byte, _ int, _ int) int {
	return copy(b, fmt.Sprintf(p.Trice.Strg))
}

// unSignedOrSignedOut prints p.B according to the format string.
func (p *cobsDec) unSignedOrSignedOut(b []byte, bitwidth, count int) int {
	if len(p.u) != count {
		return copy(b, fmt.Sprintln("ERROR: Invalid format specifier count inside", p.Trice.Type, p.Trice.Strg))
	}
	v := make([]interface{}, 1024) // theoretical 1008 bytes could arrive
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
func (p *cobsDec) printTestTableLine(n int) {
	if emitter.NextLine || testTableVirgin {
		emitter.NextLine = false
		testTableVirgin = false
		fmt.Printf("{ []byte{ ")
	}
	for _, b := range p.IBuf[0:n] { // just to see trice bytes per trice
		fmt.Printf("%3d,", b)
	}
}
