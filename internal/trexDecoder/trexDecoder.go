// Copyright 2022 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package trexDecoder

import (
	"io"
	"log"
	"strings"
	"sync"

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
	packageFramingNone0   // -pf=none    For compability. Detection, if compact or aligned buffers come in.
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
	SingleFraming                bool              // CLI SingleFraming demands, that each received frame contains not more than a singe Trice message.
	DisableCycleErrors           bool              // CLI, needed when routing is enabled
	TypeX0Handler                = "countedString" // CLI, needed when other typeX0 handlers are needed.
)

// trexDec is the Decoding instance for trex encoded trices.
type trexDec struct {
	decoder.DecoderData
	trices         []triceR // Contains parsed but not printed yet Trices.
	t              *triceR  // t points to actual triceR inside trices slice.
	tcount         int      // tcount is the number of parsed Trices. It is the next writable TriceR struct index in Trices.
	packageFraming int      // CLI, range packageFramingNone...packageFramingTCOBSv2
	paddingSpace   int      // Needed in packageFramingNone0 to help detection of packageFramingNone8 or packageFramingNone32
}

// triceR contains all parsed individual Trice specific read data for a single Trice.
type triceR struct {
	ilk           id.TriceID // range 0-3
	id            id.TriceID // range 0-16383
	stampSize     int        // 0 or 2 or 4 bytes
	stamp         uint64     // targetTimestamp contains target specific (time) stamp value.
	expectedCycle uint8      // range 0-255: c0...
	receivedCycle uint8      // range 0-255
	pFmt          string     // modified trice format string: %u -> %d
	u             []int      // 1: modified format string positions:  %u -> %d, 2: float (%f)
	headerSize    int        // Trice header size as number of bytes
	paramSize     int        // Trice net payload size after header, equals len(v), but we need it during parsing.
	v             []byte     // Trice values without padding space

	Trice id.TriceFmt // id.TriceFmt (lut format string)
	//SLen       int // string length for TRICE_S
}

// New provides a TREX decoder instance.
//
// l is the trice id list in slice of struct format.
// in is the usable reader for the input bytes.
func New(w io.Writer, sw *emitter.TriceLineComposer, lut id.TriceIDLookUp, m *sync.RWMutex, li id.TriceIDLookUpLI, in io.Reader, endian bool) decoder.Decoder {
	// Todo: rewrite using the TCOBS Reader. The provided in io.Reader provides a raw data stream.
	// https://github.com/rokath/tcobs/blob/master/TCOBSv1/read.go -> use NewDecoder ...

	p := &trexDec{}
	p.Sw = sw
	p.W = w
	p.In = in
	p.InnerBuffer = make([]byte, decoder.DefaultSize) // len max
	p.Last = make([]byte, 0, decoder.DefaultSize)     // len 0
	p.B = make([]byte, decoder.DefaultSize)           // len max
	p.trices = make([]triceR, decoder.DefaultSize)    // len max
	p.trices[0].expectedCycle = 0xc0                  // start value

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
	err = p.readAndParseTrices()
	if err != nil {
		return 0, err
	}
	return p.interpretTrices(b)
}
