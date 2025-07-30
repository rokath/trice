// Copyright 2022 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package trexDecoder

import (
	"bytes"
	"encoding/hex"
	"fmt"
	"io"
	"log"
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
	tyIdSize              = 2 // tySize is what each trice message starts with: 2 bytes
	ncSize                = 2 // countSize is what each regular trice message contains after an optional target timestamp
	typeS0                = 1 // regular trice format without stamp    : 01iiiiiiI NC ...
	typeS2                = 2 // regular trice format with 16-bit stamp: 10iiiiiiI TT NC ... or with -doubled16BitID 10iiiiiiI 10iiiiiiI TT NC ...
	typeS4                = 3 // regular trice format with 32-bit stamp: 11iiiiiiI TT TT NC ...
	typeX0                = 0 // unspecified, usually a counted string : 00nnnnnnN... with TypeX0Handler = "countedString"
	packageFramingNone    = iota
	packageFramingNone0   // For compability, detection, if compact or aligned buffers come in, is not done yet.
	packageFramingNone8   // -pf=none8   Compact unframed buffer, no spaces between Trices. Use case is TRICE_DIRECT_SEGGER_RTT_8BIT_WRITE without framing.
	packageFramingNone32  // -pf=none32  Aligned unframed buffer, after each Trice 0-3 padding zeroes to make the length a multiple of 4 bytes. Use case is TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE without framing.
	packageFramingNone64  // -pf=none64  Aligned unframed buffer, after each Trice 0-7 padding zeroes to make the length a multiple of 8 bytes. Use case is with encryption and no framing.
	packageFramingCOBS    // -pf=COBS    Compact buffer, no spaces between Trices, but at buffer end 0-3(7 with encryption) padding zeroes allowed.
	packageFramingTCOBS   // -pf=TCOBS   Compact buffer, no spaces between Trices, but at buffer end 0-3(7 with encryption) padding zeroes allowed.
	packageFramingTCOBSv2 // -pf=TCOBSv2 Compact buffer, no spaces between Trices, but at buffer end 0-3(7 with encryption) padding zeroes allowed.
)

var (
	Doubled16BitID               bool              // CLI, needed for rtt32
	AddNewlineToEachTriceMessage bool              // CLI
	SingleFraming                bool              // CLI SingleFraming demands, that each received package contains not more than a singe Trice message.
	DisableCycleErrors           bool              // CLI, needed when routing is enabled
	TypeX0Handler                = "countedString" // CLI, needed when other typeX0 handlers are needed.
)

// trexDec is the Decoding instance for trex encoded trices.
type trexDec struct {
	decoder.DecoderData
	expectedCycle  uint8      // cycle date: c0...bf
	pFmt           string     // modified trice format string: %u -> %d
	u              []int      // 1: modified format string positions:  %u -> %d, 2: float (%f)
	packageFraming int        // CLI, range packageFramingNone...packageFramingTCOBSv2
	triceType      id.TriceID // range 0-3
	triceID        id.TriceID // range 0-16383
	receivedCycle  uint8      // range 0-255
	ntlen          int        // computed len of next Trice in p.B
}

// New provides a TREX decoder instance.
//
// l is the trice id list in slice of struct format.
// in is the usable reader for the input bytes.
func New(w io.Writer, lut id.TriceIDLookUp, m *sync.RWMutex, li id.TriceIDLookUpLI, in io.Reader, endian bool) decoder.Decoder {
	// Todo: rewrite using the TCOBS Reader. The provided in io.Reader provides a raw data stream.
	// https://github.com/rokath/tcobs/blob/master/TCOBSv1/read.go -> use NewDecoder ...

	p := &trexDec{}
	p.expectedCycle = 0xc0 // start value
	p.W = w
	p.In = in
	//p.ScratchPad = make([]byte, 0, decoder.DefaultSize) // len 0
	p.InnerBuffer = make([]byte, decoder.DefaultSize) // len max
	p.Last = make([]byte, 0, decoder.DefaultSize)     // len 0
	p.B = make([]byte, decoder.DefaultSize)           // len max
	//p.B = p.B0           // len max
	p.InnerBuffer = make([]byte, decoder.DefaultSize) // len max
	p.Lut = lut
	p.LutMutex = m
	p.Endian = endian
	p.Li = li
	decoder.PackageFraming = strings.ToLower(decoder.PackageFraming)
	switch decoder.PackageFraming {
	case "cobs":
		p.packageFraming = packageFramingCOBS
	case "tcobs", "tcobsv1":
		p.packageFraming = packageFramingTCOBS
	case "tcobsv2":
		p.packageFraming = packageFramingTCOBSv2
	case "none", "none0":
		if cipher.Password != "" { // encrypted
			p.packageFraming = packageFramingNone64
		} else {
			p.packageFraming = packageFramingNone0
		}
	case "none8":
		p.packageFraming = packageFramingNone8
	case "none32":
		p.packageFraming = packageFramingNone32
	case "none64":
		p.packageFraming = packageFramingNone64
	default:
		log.Fatal("Invalid framing switch:\a", decoder.PackageFraming)
	}
	return p
}

// Read is the provided read method for TREX decoding and provides next string as byte slice.
func (p *trexDec) Read(b []byte) (n int, err error) {
	switch p.packageFraming {
	case packageFramingNone, packageFramingNone0:
		p.nextData() // returns all unprocessed data inside p.B
		return       //p.InterpretUnframedData0(b)
	case packageFramingNone8:
		p.nextData() // returns all unprocessed data inside p.B
		return       //p.InterpretUnframedData8(b)
	case packageFramingNone32:
		p.nextData() // returns all unprocessed data inside p.B
		return       //p.InterpretUnframedData32(b)
	case packageFramingNone64: // XTEA is active
		p.nextData() // returns all unprocessed data inside p.B
		return       //p.InterpretUnframedData64(b)
	default:
		p.nextFrame()
		return p.InterpretDecodedFrame(b), nil
	}
}

// nextBytes reads with an inner reader a raw byte stream and appends it to p.Last.
func (p *trexDec) nextBytes() {
	m, err := p.In.Read(p.InnerBuffer) // use p.InnerBuffer as destination read buffer
	if err != nil && err != io.EOF {   // some serious error
		log.Fatal("ERROR:internal reader error\a", err) // exit
	}
	p.Last = append(p.Last, p.InnerBuffer[:m]...) // merge with leftovers
}

// nextData reads with an inner reader a raw byte stream and appends it to p.B.
func (p *trexDec) nextData() {
	p.nextBytes()
	p.I = p.Last
	p.Last = p.Last[:0]
}

// nextFrame reads with an inner reader a raw byte stream, appends it to p.Last and returns in p.B a complete decoded frame or p.B stays empty.
func (p *trexDec) nextFrame()(err error) {
	p.nextBytes()
	index := bytes.IndexByte(p.Last, 0) // find terminating 0
	if index == -1 {                    // p.Last has no complete (T)COBS data
		p.I = p.I[:0]
		return
	}
	err = p.decodeFrame(p.Last[:index]) // exclude terminating 0, put frame into p.B
	p.Last = p.Last[index:]              // exclude terminating 0, keep rest for next package
	return 
}

// decodeFrame expects a full frame without 0-delimiter in src, and decodes it into p.I.
func (p *trexDec) decodeFrame(src []byte) ( err error) {
	var n int
	p.I = p.B
	switch p.packageFraming {
	case packageFramingCOBS:
		n, err = cobs.Decode(p.I, src)
		p.I = p.I[:n]
	case packageFramingTCOBS:
		n, err = tcobs.Decode(p.I, src)
		p.I = p.I[len(p.I)-n:]
	case packageFramingTCOBSv2:
		fallthrough
	default:
		log.Fatalln("unexpected p.packageFraming", p.packageFraming)
	}
	if err != nil {
		return err
	}
	return p.DecryptIfEncrypted()
}

// DecryptIfEncrypted decrypts p.I in place, when a Cipher.Password is set.
func (p *trexDec) DecryptIfEncrypted() (err error) {
	if decoder.DebugOut { // Debug output
		//fmt.Print("->TRICE: ")
		//decoder.Dump(os.Stdout, p.B)
		fmt.Fprint(p.W, "->TRICE: ")
		decoder.Dump(p.W, p.I)
	}
	if cipher.Password != "" { // encrypted
		if len(p.I)%8 != 0 {
			fmt.Fprint(p.W, "-> ERR:  ")
			decoder.Dump(p.W, p.I)
			err = fmt.Errorf("decoded frame length %d not a multiple of 8, discarding.", len(p.I))
			p.I = p.I[:0]
			return err
		}
		cipher.Decrypt(p.I, p.I)
		if decoder.DebugOut { // Debug output
			//fmt.Print("->DEC: ")
			//decoder.Dump(os.Stdout, p.B)
			fmt.Fprint(p.W, "-> DEC:  ")
			decoder.Dump(p.W, p.I)
		}
	}
	return nil
}

// InterpretDecodedFrame expects in p.I a decoded frame for interpretation.
func (p *trexDec) InterpretDecodedFrame(b []byte) (n int) {
	for p.TriceComplete() {
		n += p.printTrice(b[n:])
		p.I = p.I[p.ntlen:] // remove used data
	}
	if len(p.I) < 4 && allZero(p.B) {
		p.I = p.I[:0] // discard padding bytes
		return
	}
	if len(p.B) < 7 && allZero(p.B) && cipher.Password != "" {
		p.I = p.I[:0] // discard padding bytes
		return
	}
	n += copy(b[n:], fmt.Sprintln("ERR:incomplete Trice in frame:\a"))
	n += copy(b[n:], fmt.Sprintln(hex.Dump(p.B)))
	n += copy(b[n:], fmt.Sprintln(decoder.Hints))
	p.I = p.I[:0] // discard
	return
}

// allZero returns true, when all values in bytes are 0.
// If bytes is empty, true is returned as well.
func allZero(bytes []byte) bool {
	b := byte(0)
	for _, s := range bytes {
		b |= s
	}
	return b == 0
}

// checkReceivedCycle evaluates p.receivedCycle and updates p.receivedCycle.
// On error it write a message into and returns in n the message length.
// It uses and updates decoder.InitialCycle.
func (p *trexDec) checkReceivedCycle(b []byte) (n int) {
	// cycle counter automatic & check
	if p.receivedCycle == 0xc0 && p.expectedCycle != 0xc0 && decoder.InitialCycle { // with cycle counter and seems to be a target reset
		n += copy(b[n:], fmt.Sprintln("warning:\a   Target Reset?   "))
		p.expectedCycle = p.receivedCycle + 1 // adjust cycle
		decoder.InitialCycle = false
	}
	if p.receivedCycle == 0xc0 && p.expectedCycle != 0xc0 && !decoder.InitialCycle { // with cycle counter and seems to be a target reset
		//n += copy(b[n:], fmt.Sprintln("info:   Target Reset?   ")) // todo: This line is ok with cycle counter but not without cycle counter
		p.expectedCycle = p.receivedCycle + 1 // adjust cycle
	}
	if p.receivedCycle == 0xc0 && p.expectedCycle == 0xc0 && decoder.InitialCycle { // with or without cycle counter and seems to be a target reset
		//n += copy(b[n:], fmt.Sprintln("warning:   Restart?   "))
		p.expectedCycle = p.receivedCycle + 1 // adjust cycle
		decoder.InitialCycle = false
	}
	if p.receivedCycle == 0xc0 && p.expectedCycle == 0xc0 && !decoder.InitialCycle { // with or without cycle counter and seems to be a normal case
		p.expectedCycle = p.receivedCycle + 1 // adjust cycle
	}
	if p.receivedCycle != 0xc0 && !DisableCycleErrors { // with cycle counter and s.th. lost
		if p.receivedCycle != p.expectedCycle { // no cycle check for 0xc0 to avoid messages on every target reset and when no cycle counter is active
			n += copy(b[n:], fmt.Sprintln("CYCLE_ERROR:\a", p.receivedCycle, "!=", p.expectedCycle, " (count=", emitter.TagEvents("CYCLE_ERROR")+1, ")"))
			n += copy(b[n:], "                                         ") // len of location information plus stamp: 41 spaces - see NewlineIndent below - todo: make it generic
			p.expectedCycle = p.receivedCycle                             // adjust cycle
		}
		decoder.InitialCycle = false
		p.expectedCycle++
	}
}

// printTrice expects in p.B enough data for printing a trice.
// p.ntlen, p.triceType, p.triceID==decoder.LastTriceID, p.receivedCycle, p.ParamSpace, decoder.TargetTimestampSize, decoder.TargetTimestamp are valid already
func (p *trexDec) printTrice(b []byte) (n int) {
	n = p.checkReceivedCycle(b)
	var ok bool
	//p.LutMutex.RLock()
	p.Trice, ok = p.Lut[p.triceID]
	//p.LutMutex.RUnlock()
	if !ok {
		// if p.packageFraming == packageFramingNone {
		// 	if decoder.Verbose {
		// 		n += copy(b[n:], fmt.Sprintln("wrn:\adiscarding first byte", p.B0[0], "from:"))
		// 		n += copy(b[n:], fmt.Sprintln(hex.Dump(p.B0)))
		// 	}
		// 	p.B0 = p.B0[1:] // discard first byte and try again
		// 	p.B = p.B0
		// } else {
		n += copy(b[n:], fmt.Sprintln("WARNING:\aunknown ID ", p.triceID, "- ignoring trice:"))
		n += copy(b[n:], fmt.Sprintln(hex.Dump(p.I[:p.ntlen])))
		n += copy(b[n:], fmt.Sprintln(decoder.Hints))
		//p.I = p.I[p.ntlen:] // discard
		//}
		return
	}
	if AddNewlineToEachTriceMessage {
		p.Trice.Strg += `\n` // this adds a newline to each single Trice message
	}

	n += p.sprintTrice(b[n:]) // use param info

	return
}

// TriceComplete tries to interpret next data in p.I. If not enough bytes false is returned. p.I is not altered.
// On success true is returned and these values are valid:
// p.triceType, p.triceID==decoder.LastTriceID, p.receivedCycle, p.ParamSpace, decoder.TargetTimestampSize, decoder.TargetTimestamp
func (p *trexDec) TriceComplete() bool {
	p.ntlen = 2
	if len(p.I) < p.ntlen { // not enough data
		return false
	}
	if cipher.Password != "" {
		p.ntlen = 8
		if len(p.I) < p.ntlen { // not enough data
			return false
		}
		// todo:
		// Decrypt 8 bytes and optionally next 8 bytes and so on, until the first Trice is complete.
		// We may need to wait for more data
	}
	tyId := id.TriceID(p.ReadU16(p.I))
	if tyId == 0 { // No padding bytes expected here.
		for true {
		}
	}
	p.triceType = tyId >> decoder.IDBits // 2 most significant bits are the triceType
	p.triceID = 0x3FFF & tyId            // 14 least significant bits are the ID
	decoder.LastTriceID = p.triceID      // used for showID

	switch p.triceType {
	case typeS0: // no stamp
		return p.TriceTypeS0Complete()
	case typeS2: // 16-bit stamp
		return p.TriceTypeS2Complete()
	case typeS4: // 32-bit stamp
		return p.TriceTypeS4Complete()
	case typeX0:
		return p.TriceTypeX0Complete()
	}
	return false
}

func (p *trexDec) GetCycleAndParamSize(b []byte) {
	nc := p.ReadU16(b) // n = number of data bytes (without timestamp), most significant bit is the count encoding, c = cycle
	if nc>>15 == 1 {   // special case: more than data 127 bytes
		// C code: #define TRICE_LCNT(count) TRICE_PUT16( (0x8000 | (count)) );
		p.receivedCycle = p.expectedCycle // cycle is not transmitted, so set expected value
		p.ParamSpace = int(0x7FFF & nc)   // 15 bit for data byte count excluding timestamp
	} else {
		// C code: #define TRICE_CNTC(count) TRICE_PUT16( ((count)<<8) | TRICE_CYCLE )
		p.receivedCycle = uint8(nc) // low byte is cycle
		p.ParamSpace = int(nc >> 8) // high byte is 7 bit number of bytes for data count excluding timestamp
	}
}

func (p *trexDec) TriceTypeS0Complete() bool {
	decoder.TargetTimestampSize = 0
	p.ntlen = tyIdSize + ncSize
	if len(p.I) < p.ntlen {
		return false
	}
	p.GetCycleAndParamSize(p.I[tyIdSize:])
	p.ntlen += p.ParamSpace
	if len(p.I) < p.ntlen {
		return false
	}
	decoder.RecordForStatistics(p.triceID) // This is for the "trice log -stat" flag
	return true
}

func (p *trexDec) TriceTypeS2Complete() bool {
	decoder.TargetTimestampSize = 2
	if Doubled16BitID { // p.packageFraming == packageFramingNone || cipher.Password != "" {
		p.ntlen = tyIdSize + tyIdSize + 2 + ncSize
	} else {
		p.ntlen = tyIdSize + 2 + ncSize
	}
	if len(p.I) < p.ntlen {
		return false
	}
	decoder.TargetTimestamp = uint64(p.ReadU16(p.I[p.ntlen-4:]))
	p.GetCycleAndParamSize(p.I[p.ntlen-2:])
	p.ntlen += p.ParamSpace
	if len(p.I) < p.ntlen {
		return false
	}
	decoder.RecordForStatistics(p.triceID) // This is for the "trice log -stat" flag
	return true
}

func (p *trexDec) TriceTypeS4Complete() bool {
	decoder.TargetTimestampSize = 4
	p.ntlen = tyIdSize + 4 + ncSize
	if len(p.I) < p.ntlen {
		return false
	}
	decoder.TargetTimestamp = uint64(p.ReadU32(p.I[tyIdSize:]))
	p.GetCycleAndParamSize(p.I[tyIdSize+4:])
	p.ntlen += p.ParamSpace
	if len(p.I) < p.ntlen {
		return false
	}
	decoder.RecordForStatistics(p.triceID) // This is for the "trice log -stat" flag
	return true
}

func (p *trexDec) TriceTypeX0Complete() bool {
	switch TypeX0Handler {
	case "countedString": // The first 2 bytes are the count.
		p.ntlen = tyIdSize + int(p.triceID)
		if len(p.I) < p.ntlen {
			return false // not enough data
		}
	default:
		return false
	}
	return true
}
