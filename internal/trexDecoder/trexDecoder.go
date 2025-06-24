// Copyright 2022 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package trexDecoder

import (
	"bytes"
	"encoding/binary"
	"encoding/hex"
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
	typeS0   = 1 // regular trice format without stamp     : 011iiiiiI NC ...
	typeS2   = 2 // regular trice format with 16-bit stamp : 101iiiiiI TT NC ...
	typeS4   = 3 // regular trice format with 32-bit stamp : 111iiiiiI TT TT NC ...
	typeX0   = 0 // regular trice format with 32-bit stamp : 001iiiiiI TT TT TT TT NC ...

	packageFramingNone = iota
	packageFramingCOBS
	packageFramingTCOBS   //v1
	packageFramingTCOBSv2 //v2
)

var (
	Doubled16BitID               bool
	AddNewlineToEachTriceMessage bool
	SingleFraming                bool // SingleFraming demands, that each received package contains not more than a singe Trice message.
)

// trexDec is the Decoding instance for trex encoded trices.
type trexDec struct {
	decoder.DecoderData
	cycle          uint8  // cycle date: c0...bf
	pFmt           string // modified trice format string: %u -> %d
	u              []int  // 1: modified format string positions:  %u -> %d, 2: float (%f)
	packageFraming int
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
	p.IBuf = make([]byte, 0, decoder.DefaultSize)     // len 0
	p.B = make([]byte, 0, decoder.DefaultSize)        // len 0
	p.B0 = make([]byte, decoder.DefaultSize)          // len max
	p.InnerBuffer = make([]byte, decoder.DefaultSize) // len max
	p.Lut = lut
	p.LutMutex = m
	p.Endian = endian
	p.Li = li

	switch strings.ToLower(decoder.PackageFraming) {
	case "cobs":
		p.packageFraming = packageFramingCOBS
	case "tcobs", "tcobsv1":
		p.packageFraming = packageFramingTCOBS
	case "tcobsv2":
		p.packageFraming = packageFramingTCOBSv2
	case "none":
		p.packageFraming = packageFramingNone
	default:
		log.Fatal("Invalid framing switch:\a", decoder.PackageFraming)
	}
	return p
}

// nextData reads with an inner reader a raw byte stream.
//
// When less 4 bytes found in the incoming bytes nextPackage nextData without action.
// That means the incoming data stream is exhausted and a next try should be started a bit later.
// Some arrived bytes are kept internally and concatenated with the following bytes in a next Read.
// Afterwards 0 or at least 4 bytes are inside p.B
func (p *trexDec) nextData() {
	m, err := p.In.Read(p.InnerBuffer)      // use p.InnerBuffer as destination read buffer
	p.B = append(p.B, p.InnerBuffer[:m]...) // merge with leftovers
	if err != nil && err != io.EOF {        // some serious error
		log.Fatal("ERROR:internal reader error\a", err) // exit
	}
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
			log.Fatal("ERROR:internal reader error\a", err) // exit
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
	// here a complete COBS or TCOBS package exists
	if decoder.DebugOut { // Debug output
		fmt.Fprintf(p.W, "%s: ", decoder.PackageFraming)
		decoder.Dump(p.W, p.IBuf[:index+1])
	}

	frame := p.IBuf[:index]

	switch p.packageFraming {

	case packageFramingCOBS:
		p.B = p.B0                      // make([]byte, decoder.DefaultSize) // todo: avoid allocation
		n, e := cobs.Decode(p.B, frame) // if index is 0, an empty buffer is decoded
		p.IBuf = p.IBuf[index+1:]       // step forward (next package data in p.IBuf now, if any)
		if e != nil {
			if decoder.Verbose {
				fmt.Println("\ainconsistent COBS buffer!") // show also terminating 0
			}
		}
		p.B = p.B[:n]

	case packageFramingTCOBS:
	repeat:
		p.B = p.B0                       // make([]byte, decoder.DefaultSize) // todo: avoid allocation
		n, e := tcobs.Decode(p.B, frame) // if index is 0, an empty buffer is decoded
		// from merging: p.IBuf = p.IBuf[index+1:]        // step forward (next package data in p.IBuf now, if any)
		if e != nil {
			fmt.Println("\ainconsistent TCOBSv1 buffer!")

			// remove 3 lines if they exist
			s := strings.SplitN(strings.ReplaceAll(string(frame), "\r\n", "\n"), "\n", 4)
			var bytesCount int
			if len(s) >= 3 {
				var newLines int
				fmt.Println(s[0])
				fmt.Println(s[1])
				fmt.Println(s[2])
				for _, b := range frame {
					frame = frame[1:]
					bytesCount++
					if b == 10 {
						newLines++
						if newLines == 3 {
							break
						}
					}
					continue
				}
				index -= bytesCount
				goto repeat
			}
			if decoder.Verbose {
				fmt.Println(e, "\ainconsistent TCOBSv1 buffer:")
				fmt.Println(e, hex.Dump(frame)) // show also terminating 0
			}
			e = nil
			p.B = p.B[:0]
			p.IBuf = p.IBuf[index+1:] // step forward (next package data in p.IBuf now, if any) // from merging:
		} else {
			p.B = p.B[len(p.B)-n:]    // buffer is filled from the end
			p.IBuf = p.IBuf[index+1:] // step forward (next package data in p.IBuf now, if any) // from merging:
		}
	default:
		log.Fatalln("unexpected execution path", p.packageFraming)
	}

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

func isZero(bytes []byte) bool {
	b := byte(0)
	for _, s := range bytes {
		b |= s
	}
	return b == 0
}

func (p *trexDec) removeZeroHiByte(s []byte) (r []byte) {
	// The package interpreter does not know the number of padding zeroes, so it needs to discard them one by one.
	// If they are not zero, this is an error.
	if p.Endian == decoder.BigEndian {
		// Big endian case: 00 00 AA AA C0 00 -> 00 AA AA C0 00 -> still typeX0 -> AA AA C0 00 -> ok next package
		if s[0] != 0 {
			fmt.Println("unexpected case in line 273", s)
		}
		r = s[1:]
	} else if p.Endian == decoder.LittleEndian {
		// Little endian case: 00 00 AA AA C0 00 -> 00 AA AA C0 00 -> AA00 signals a valid Trice, but it is not! -> We need to remove the HI byte!
		if s[1] != 0 {
			//log.Fatal("unexpected case", s)
			// todo: This needs to be disabled for successfully running all test cases.
			// BUT: deferred package framing NONE does not work
		}
		r = append(s[:1], s[2:]...)
	} else {
		fmt.Println("unexpected case 927346193377", s)
	}
	return
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
	if p.packageFraming == packageFramingNone {
		p.nextData() // returns all unprocessed data inside p.B
		p.B0 = p.B   // keep data for re-sync
	} else {
		if cipher.Password != "" && len(p.B) < 8 && isZero(p.B) {
			p.B = p.B[:0] // Discard trailing zeroes. ATTENTION: incomplete trice messages containing many zeroes could be problematic here!
		}
		if len(p.B) == 1 { // last decoded package exhausted
			if decoder.Verbose {
				fmt.Println("Inconsistent data, discarding last single byte", p.B[0], "from:")
				fmt.Println(hex.Dump(p.B))
			}
			p.B = p.B[:0]
		}
		if len(p.B) == 0 { // last decoded package exhausted
			p.nextPackage() // returns one decoded package inside p.B
		}
	}
	packageSize := len(p.B)
	if packageSize < tyIdSize { // not enough data for a next package
		return
	}
	packed := p.B
	tyId := p.ReadU16(p.B)
	p.B = p.B[tyIdSize:]

	triceType := int(tyId >> decoder.IDBits) // most significant bit are the triceType
	triceID := id.TriceID(0x3FFF & tyId)     // 14 least significant bits are the ID
	decoder.LastTriceID = triceID            // used for showID
	decoder.RecordForStatistics(triceID)     // This is for the "trice log -stat" flag

	switch triceType {
	case typeS0: // no timestamp
		decoder.TargetTimestampSize = 0
	case typeS2: // 16-bit stamp
		decoder.TargetTimestampSize = 2
		if Doubled16BitID { // p.packageFraming == packageFramingNone || cipher.Password != "" {
			if len(p.B) < 2 {
				return // wait for more data
			}

			// Without encoding it needs to be done here.
			// Also encrypted trice messages carry a double 16-bit ID.
			p.B = p.B[tyIdSize:] // When target encoding is done, it removes the double 16-bit ID at the 16-bit timestamp trices.
		}
	case typeS4: // 32-bit stamp
		decoder.TargetTimestampSize = 4
	case typeX0: // extended trice type X0
		if p.packageFraming == packageFramingNone {
			// typeX0 is not supported (yet)
			if decoder.Verbose {
				n += copy(b[n:], fmt.Sprintln("wrn:\aTo try to resync removing zero HI byte from:"))
				n += copy(b[n:], fmt.Sprintln(hex.Dump(p.B0)))
			}
			p.B = p.removeZeroHiByte(p.B0)
			return
		}
		// We can reach here in target TRICE_MULTI_PACK_MODE, when a trice message is followed by several zeroes (up to 7 possible with encryption).
		p.B = p.removeZeroHiByte(packed)
	}

	if packageSize < tyIdSize+decoder.TargetTimestampSize+ncSize { // for non typeEX trices
		return // not enough data
	}

	// try to interpret
	if triceType == typeS0 {
		decoder.TargetTimestamp = 0
	} else if triceType == typeS2 { // 16-bit stamp
		decoder.TargetTimestamp = uint64(p.ReadU16(p.B))
	} else if triceType == typeS4 { // 32-bit stamp
		decoder.TargetTimestamp = uint64(p.ReadU32(p.B))
	} else {
		n += copy(b[n:], fmt.Sprintln("ERROR:\atriceType typeX0 not implemented (hint: IDBits value?)"))
		return n, nil
	}
	p.B = p.B[decoder.TargetTimestampSize:]

	if len(p.B) < 2 {
		return // wait for more data
	}
	nc := p.ReadU16(p.B) // n = number of data bytes (without timestamp), most significant bit is the count encoding, c = cycle
	p.B = p.B[ncSize:]

	var cycle uint8
	if nc>>15 == 1 { // special case: more than data 127 bytes
		// C code: #define TRICE_LCNT(count) TRICE_PUT16( (0x8000 | (count)) );
		cycle = p.cycle                 // cycle is not transmitted, so set expected value
		p.ParamSpace = int(0x7FFF & nc) // 15 bit for data byte count excluding timestamp
	} else {
		// C code: #define TRICE_CNTC(count) TRICE_PUT16( ((count)<<8) | TRICE_CYCLE )
		cycle = uint8(nc)           // low byte is cycle
		p.ParamSpace = int(nc >> 8) // high byte is 7 bit number of bytes for data count excluding timestamp
	}

	p.TriceSize = tyIdSize + decoder.TargetTimestampSize + ncSize + p.ParamSpace
	if p.TriceSize > packageSize { //  '>' for multiple trices in one package (case TriceOutMultiPackMode), todo: discuss all possible variants
		if p.packageFraming == packageFramingNone {
			if decoder.Verbose {
				n += copy(b[n:], fmt.Sprintln("wrn:\adiscarding first byte", p.B0[0], "from:"))
				n += copy(b[n:], fmt.Sprintln(hex.Dump(p.B0)))
			}
			p.B0 = p.B0[1:] // discard first byte and try again
			p.B = p.B0
			return
		}
		if decoder.Verbose {
			n += copy(b[n:], fmt.Sprintln("ERROR:\apackage size", packageSize, "is <", p.TriceSize, " - ignoring package:"))
			n += copy(b[n:], fmt.Sprintln(hex.Dump(p.B)))
			n += copy(b[n:], fmt.Sprintln("tyIdSize=", tyIdSize, "tsSize=", decoder.TargetTimestampSize, "ncSize=", ncSize, "ParamSpae=", p.ParamSpace))
			n += copy(b[n:], fmt.Sprintln(decoder.Hints))
		}
		p.B = p.B[len(p.B):] // discard buffer
	}
	if SingleFraming && p.TriceSize != packageSize {
		if decoder.Verbose {
			n += copy(b[n:], fmt.Sprintln("ERROR:\asingle framed package size", packageSize, "is !=", p.TriceSize, " - ignoring package:"))
			n += copy(b[n:], fmt.Sprintln(hex.Dump(p.B)))
			n += copy(b[n:], fmt.Sprintln("tyIdSize=", tyIdSize, "tsSize=", decoder.TargetTimestampSize, "ncSize=", ncSize, "ParamSpae=", p.ParamSpace))
			n += copy(b[n:], fmt.Sprintln(decoder.Hints))
		}
		p.B = p.B[len(p.B):] // discard buffer
	}

	// cycle counter automatic & check
	if cycle == 0xc0 && p.cycle != 0xc0 && decoder.InitialCycle { // with cycle counter and seems to be a target reset
		n += copy(b[n:], fmt.Sprintln("warning:\a   Target Reset?   "))
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
			n += copy(b[n:], fmt.Sprintln("CYCLE_ERROR:\a", cycle, "!=", p.cycle, " (count=", emitter.TagEvents("CYCLE_ERROR")+1, ")"))
			n += copy(b[n:], "                                         ") // len of location information plus stamp: 41 spaces - see NewlineIndent below - todo: make it generic
			p.cycle = cycle                                               // adjust cycle
		}
		decoder.InitialCycle = false
		p.cycle++
	}

	var ok bool
	p.LutMutex.RLock()
	p.Trice, ok = p.Lut[triceID]
	if AddNewlineToEachTriceMessage {
		p.Trice.Strg += `\n` // this adds a newline to each single Trice message
	}
	p.LutMutex.RUnlock()
	if !ok {
		if p.packageFraming == packageFramingNone {
			if decoder.Verbose {
				n += copy(b[n:], fmt.Sprintln("wrn:\adiscarding first byte", p.B0[0], "from:"))
				n += copy(b[n:], fmt.Sprintln(hex.Dump(p.B0)))
			}
			p.B0 = p.B0[1:] // discard first byte and try again
			p.B = p.B0
		} else {
			n += copy(b[n:], fmt.Sprintln("WARNING:\aunknown ID ", triceID, "- ignoring trice ending with:"))
			n += copy(b[n:], fmt.Sprintln(hex.Dump(p.B)))
			n += copy(b[n:], fmt.Sprintln(decoder.Hints))
			p.B = p.B[:0] // discard all
		}
		return
	}

	n += p.sprintTrice(b[n:]) // use param info
	if len(p.B) < p.ParamSpace {
		if p.packageFraming == packageFramingNone {
			if decoder.Verbose {
				n += copy(b[n:], fmt.Sprintln("wrn:discarding first byte", p.B0[0], "from:"))
				n += copy(b[n:], fmt.Sprintln(hex.Dump(p.B0)))
			}
			p.B0 = p.B0[1:] // discard first byte and try again
			p.B = p.B0
		} else {
			n += copy(b[n:], fmt.Sprintln("ERROR:ignoring data garbage:"))
			n += copy(b[n:], fmt.Sprintln(hex.Dump(p.B)))
			n += copy(b[n:], fmt.Sprintln(decoder.Hints))
			p.B = p.B[:0] // discard all
		}
	} else {
		if p.packageFraming != packageFramingNone { // COBS | TCOBS are exact
			p.B = p.B[p.ParamSpace:] // drop param info
		} else { // no package framing
			padding := (p.ParamSpace + 3) & ^3
			if padding <= len(p.B) {
				p.B = p.B[padding:]
			} else {
				// n += copy(b[n:], fmt.Sprintln("wrn: cannot discard padding bytes", ))
			}
		}
	}
	return
}

// sprintTrice writes a trice string or appropriate message into b and returns that len.
//
// p.Trice.Type is the received trice, in fact the name from til.json.
func (p *trexDec) sprintTrice(b []byte) (n int) {

	isSAlias := strings.HasPrefix( p.Trice.Strg, id.SAliasStrgPrefix ) && strings.HasSuffix( p.Trice.Strg, id.SAliasStrgSuffix )
	if isSAlias { // A SAlias Strg is covered with id.SAliasStrgPrefix and id.SAliasStrgSuffix in til.json and it needs to be replaced with "%s" here. 
		p.Trice.Strg = "%s" // See appropriate comment inside insertTriceIDs().
	} 

	p.pFmt, p.u = decoder.UReplaceN(p.Trice.Strg)
	p.Trice.Type = strings.TrimSuffix(p.Trice.Type, "AssertTrue")
	p.Trice.Type = strings.TrimSuffix(p.Trice.Type, "AssertFalse")
	triceType, err := id.ConstructFullTriceInfo(p.Trice.Type, len(p.u))

	if err != nil {
		n += copy(b[n:], fmt.Sprintln("err:ConstructFullTriceInfo failed with:", p.Trice.Type, len(p.B), "- ignoring package:"))
		return
	}
	ucTriceTypeReceived := strings.ToUpper(p.Trice.Type)   // examples: TRICE_S,   TRICE,   TRICE32,   TRICE16_2
	ucTriceTypeReconstructed := strings.ToUpper(triceType) // examples: TRICE32_S, TRICE0,  TRICE32_4, TRICE16_2
	for _, s := range cobsFunctionPtrList {                // walk through the list and try to find a match for execution
		if s.triceType == ucTriceTypeReconstructed || s.triceType == ucTriceTypeReceived { // match list entry "TRICE..."
			if len(p.B) < p.ParamSpace {
				n += copy(b[n:], fmt.Sprintln("err:len(p.B) =", len(p.B), "< p.ParamSpace = ", p.ParamSpace, "- ignoring package:"))
				n += copy(b[n:], fmt.Sprintln(hex.Dump(p.B[:len(p.B)])))
				n += copy(b[n:], fmt.Sprintln(decoder.Hints))
				return
			}
			if p.ParamSpace != (s.bitWidth>>3)*s.paramCount {
				specialCases := []string{
					"TRICES", "TRICEN", "TRICEB", "TRICEF",
					"TRICE8B", "TRICE16B", "TRICE32B", "TRICE64B",
					"TRICE8F", "TRICE16F", "TRICE32F", "TRICE64F",

					"TRICE_S", "TRICE_N", "TRICE_B", "TRICE_F",
					"TRICE8_B", "TRICE16_B", "TRICE32_B", "TRICE64_B",
					"TRICE8_F", "TRICE16_F", "TRICE32_F", "TRICE64_F",
				}
				tt := strings.ToUpper(s.triceType)
				for _, casus := range specialCases {
					if tt == casus {
						goto ignoreSpecialCase
					}
				}
				n += copy(b[n:], fmt.Sprintln("err:s.triceType =", s.triceType, "ParamSpace =", p.ParamSpace, "not matching with bitWidth ", s.bitWidth, "and paramCount", s.paramCount, "- ignoring package:"))
				n += copy(b[n:], fmt.Sprintln(hex.Dump(p.B[:len(p.B)])))
				n += copy(b[n:], fmt.Sprintln(decoder.Hints))
				return
			}
		ignoreSpecialCase:
			ss := strings.Split(p.pFmt, `\n`)
			if len(ss) >= 3 { // at least one "\n" before "\n" line end
				if decoder.NewlineIndent == -1 { // auto sense
					decoder.NewlineIndent = 12 + 1 // todo: strings.SplitN & len(decoder.TargetStamp0) // 12
					if !(id.LIFnJSON == "off" || id.LIFnJSON == "none") {
						decoder.NewlineIndent += 28 /* todo: length(decoder.LocationInformationFormatString), see https://stackoverflow.com/questions/32987215/find-numbers-in-string-using-golang-regexp*/
						// todo: split channel info with format specifiers too, example: ["msg:%d\nsignal:%x %u\n", p0, p1, p2] -> ["msg:%d\n", p0] && ["signal:%x %u\n", p1, p2]
					}
					if decoder.ShowID != "" {
						decoder.NewlineIndent += 5 // todo: automatic
					}
				}
				skip := `\n`
				spaces := decoder.NewlineIndent
				for spaces > 0 {
					skip += " "
					spaces--
				}
				p.pFmt = strings.Join(ss[:], skip)
				p.pFmt = strings.TrimRight(p.pFmt, " ")
			}

			n += s.triceFn(p, b, s.bitWidth, s.paramCount) // match found, call handler
			return
		}
	}
	n += copy(b[n:], fmt.Sprintln("err:Unknown trice.Type:", p.Trice.Type, "and", triceType, "not matching - ignoring trice data:"))
	n += copy(b[n:], fmt.Sprintln(hex.Dump(p.B[:p.ParamSpace])))
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
	{"TRICE_0", (*trexDec).trice0, 0, 0, 0},
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

	{"TRICES", (*trexDec).triceS, -1, 0, 0},
	{"TRICEN", (*trexDec).triceN, -1, 0, 0},

	{"TRICE8F", (*trexDec).trice8F, -1, 0, 0},
	{"TRICE16F", (*trexDec).trice16F, -1, 0, 0},
	{"TRICE32F", (*trexDec).trice32F, -1, 0, 0},
	{"TRICE64F", (*trexDec).trice64F, -1, 0, 0},

	{"TRICE8B", (*trexDec).trice8B, -1, 0, 0},
	{"TRICE16B", (*trexDec).trice16B, -1, 0, 0},
	{"TRICE32B", (*trexDec).trice32B, -1, 0, 0},
	{"TRICE64B", (*trexDec).trice64B, -1, 0, 0},
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
	before, after, found := strings.Cut(p.Trice.Strg, ":")
	if found {
		n += copy(b[n:], fmt.Sprint(before+":")) // print channel
	} else {
		after = p.Trice.Strg
	}

	noNewline := strings.TrimSuffix(after, `\n`)

	for i := 0; i < len(s); i++ {
		n += copy(b[n:], fmt.Sprintf(noNewline, s[i]))
	}
	if len(noNewline) < len(after) { // strings.TrimSuffix removed a newline
		n += copy(b[n:], fmt.Sprintln()) // so add it finally
	}
	return
}

// trice16B converts dynamic buffers.
func (p *trexDec) trice16B(b []byte, _ int, _ int) (n int) {
	if decoder.DebugOut {
		fmt.Fprintln(p.W, p.B)
	}
	s := p.B[:p.ParamSpace]

	before, after, found := strings.Cut(p.Trice.Strg, ":")
	if found {
		n += copy(b[n:], fmt.Sprint(before+":")) // print channel
	} else {
		after = p.Trice.Strg
	}

	noNewline := strings.TrimSuffix(after, `\n`)

	for i := 0; i < len(s); i += 2 {
		n += copy(b[n:], fmt.Sprintf(noNewline, binary.LittleEndian.Uint16(s[i:])))
	}
	if len(noNewline) < len(after) { // strings.TrimSuffix removed a newline
		n += copy(b[n:], fmt.Sprintln()) // so add it finally
	}

	return
}

// trice32B converts dynamic buffers.
func (p *trexDec) trice32B(b []byte, _ int, _ int) (n int) {
	if decoder.DebugOut {
		fmt.Fprintln(p.W, p.B)
	}
	s := p.B[:p.ParamSpace]

	before, after, found := strings.Cut(p.Trice.Strg, ":")
	if found {
		n += copy(b[n:], fmt.Sprint(before+":")) // print channel
	} else {
		after = p.Trice.Strg
	}

	noNewline := strings.TrimSuffix(after, `\n`)

	for i := 0; i < len(s); i += 4 {
		n += copy(b[n:], fmt.Sprintf(noNewline, binary.LittleEndian.Uint32(s[i:])))
	}
	if len(noNewline) < len(after) { // strings.TrimSuffix removed a newline
		n += copy(b[n:], fmt.Sprintln()) // so add it finally
	}
	return
}

// trice64B converts dynamic buffers.
func (p *trexDec) trice64B(b []byte, _ int, _ int) (n int) {
	if decoder.DebugOut {
		fmt.Fprintln(p.W, p.B)
	}
	s := p.B[:p.ParamSpace]

	before, after, found := strings.Cut(p.Trice.Strg, ":")
	if found {
		n += copy(b[n:], fmt.Sprint(before+":")) // print channel
	} else {
		after = p.Trice.Strg
	}

	noNewline := strings.TrimSuffix(after, `\n`)

	for i := 0; i < len(s); i += 8 {
		n += copy(b[n:], fmt.Sprintf(noNewline, binary.LittleEndian.Uint64(s[i:])))
	}
	if len(noNewline) < len(after) { // strings.TrimSuffix removed a newline
		n += copy(b[n:], fmt.Sprintln()) // so add it finally
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
	n += copy(b[n:], fmt.Sprintln())
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
	n += copy(b[n:], fmt.Sprintln())
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
	n += copy(b[n:], fmt.Sprintln())
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
	n += copy(b[n:], fmt.Sprintln())
	return
}

// trice0 prints the trice format string.
func (p *trexDec) trice0(b []byte, _ int, _ int) int {
	return copy(b, fmt.Sprintf(p.pFmt))
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
