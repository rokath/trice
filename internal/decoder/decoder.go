// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package decoder provides several decoders for differently encoded trice streams.
package decoder

import (
	"encoding/binary"
	"fmt"
	"io"
	"log"
	"os"
	"os/signal"
	"regexp"
	"strings"
	"sync"
	"syscall"
	"time"

	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/internal/receiver"
	"github.com/rokath/trice/pkg/msg"
)

const (
	// defaultSize is the beginning receive and sync buffer size.
	defaultSize = 4096

	// LittleEndian is true for little endian trice data.
	LittleEndian = true

	// flag value for TargetEndianess
	BigEndian = false

	// patNextFormatSpecifier is a regex to find next format specifier in a string (exclude %%*) and ignoring %s
	//
	// Language C plus from language Go: %b, %F, %q
	// Partial implemented: %hi, %hu, %ld, %li, %lf, %Lf, %Lu, %lli, %lld
	// Not implemented: %s
	patNextFormatSpecifier = `(?:^|[^%])(%[0-9]*(-|c|d|e|E|f|F|g|G|h|i|l|L|o|O|p|q|u|x|X|n|b))`

	// patNextFormatUSpecifier is a regex to find next format u specifier in a string
	// It does also match %%u positions! so an additional check must follow.
	patNextFormatUSpecifier = `(?:%[0-9]*u)`

	// patNextFormatXSpecifier is a regex to find next format x specifier in a string
	// It does also match %%x positions! so an additional check must follow.
	patNextFormatXSpecifier = `(?:%[0-9]*(l|o|O|x|X|b))`

	// headSize is 4; each trice message starts with a head of 4 bytes.
	headSize = 4

	// hints is the help information in case of errors.
	hints = "att:Hints:Baudrate? Overflow? Encoding? Interrupt? til.json? Password?"
)

var (
	// Verbose gives mor information on output if set. The value is injected from main packages.
	Verbose bool

	// ShowID is used as format string for displaying the first trice ID at the start of each line if not "".
	ShowID string

	// LastTriceID is last decoded ID. It is used for switch -showID.
	LastTriceID id.TriceID

	// Encoding describes the way the byte stream is coded.
	Encoding string

	// TargetEndianess, if bigEndian assumes a big endian encoded trice stream from the target.
	// To keep target load small, the encoded trice stream from the target matches the target endianess, what us usually littleEndian.
	TargetEndianess string

	// TestTableMode is a special option for easy decoder test table generation.
	TestTableMode bool

	// Unsigned if true, forces hex and in values printed as unsigned values.
	Unsigned bool

	matchNextFormatSpecifier  = regexp.MustCompile(patNextFormatSpecifier)
	matchNextFormatUSpecifier = regexp.MustCompile(patNextFormatUSpecifier)
	matchNextFormatXSpecifier = regexp.MustCompile(patNextFormatXSpecifier)

	// DebugOut enables debug information.
	DebugOut = false

	// DumpLineByteCount is the bytes per line for the DUMP decoder.
	DumpLineByteCount int

	// initialCycle is a helper for the cycle counter automatic.
	initialCycle = true

	targetTimestamp uint32

	ShowTargetTimestamp string

	targetTimestampExists bool
)

// newDecoder abstracts the function type for a new decoder.
type newDecoder func(lut id.TriceIDLookUp, m *sync.RWMutex, in io.Reader, endian bool) Decoder

// Decoder is providing a byte reader returning decoded trice's.
// setInput allows switching the input stream to a different source.
type Decoder interface {
	io.Reader
	setInput(io.Reader)
}

// decoderData is the common data struct for all decoders.
type decoderData struct {
	in                io.Reader        // in is the inner reader, which is used to get raw bytes
	iBuf              []byte           // iBuf holds unprocessed (raw) bytes for interpretation.
	b                 []byte           // read buffer holds a single decoded COBS package, which can contain several trices.
	endian            bool             // endian is LittleEndian or BigEndian
	triceSize         int              // trice head and payload size as number of bytes
	paramSpace        int              // trice payload size after head
	sLen              int              // string length for TRICE_S
	lut               id.TriceIDLookUp // id look-up map for translation
	lutMutex          *sync.RWMutex    // to avoid concurrent map read and map write during map refresh triggered by filewatcher
	trice             id.TriceFmt      // id.TriceFmt // received trice
	lastInnerRead     time.Time
	innerReadInterval time.Duration
}

// DUMP is the Decoding instance for DUMP encoded trices.
type DUMP struct {
	decoderData
	dumpCnt int
}

// NewDUMPDecoder provides a hex dump option for incoming bytes.
func NewDUMPDecoder(lut id.TriceIDLookUp, m *sync.RWMutex, in io.Reader, endian bool) Decoder {
	p := &DUMP{}
	p.in = in
	p.iBuf = make([]byte, 0, defaultSize)
	p.lut = lut
	p.lutMutex = m
	p.endian = endian
	return p
}

func (p *DUMP) Read(b []byte) (n int, err error) {
	bb := make([]byte, 1024)
	m, err := p.in.Read(bb)
	for _, x := range bb[:m] {
		//n += copy(b, fmt.Sprintf("%02x ", uint8(x))) // somehow buggy
		fmt.Printf("%02X ", uint8(x)) // workaround
		p.dumpCnt++
		if p.dumpCnt == DumpLineByteCount {
			//n += copy(b, fmt.Sprintln("")) // somehow buggy
			fmt.Println("") // workaround
			p.dumpCnt = 0
		}
	}
	//fmt.Printf("m=%d, n=%d\n\n", m, n) // for debug
	return n, err
}

// CHAR is the Decoding instance for DUMP encoded trices.
type CHAR struct {
	decoderData
}

// NewCHARDecoder provides a character terminal output option for the trice tool.
func NewCHARDecoder(lut id.TriceIDLookUp, m *sync.RWMutex, in io.Reader, endian bool) Decoder {
	p := &CHAR{}
	p.in = in
	p.iBuf = make([]byte, 0, defaultSize)
	p.lut = lut
	p.lutMutex = m
	p.endian = endian
	return p
}

func (p *CHAR) Read(b []byte) (n int, err error) {
	bb := make([]byte, 256)
	m, err := p.in.Read(bb)
	fmt.Print(string(bb[:m]))
	return n, err
}

// setInput allows switching the input stream to a different source.
//
// This function is for easier testing with cycle counters.
func (p *decoderData) setInput(r io.Reader) {
	p.in = r
}

// handleSIGTERM is called on CTRL-C shutdown.
func handleSIGTERM(rc io.ReadCloser) {
	// prepare CTRL-C shutdown reaction
	sigs := make(chan os.Signal, 1)
	signal.Notify(sigs, syscall.SIGINT, syscall.SIGTERM)
	ticker := time.NewTicker(50 * time.Millisecond)
	for {
		select {
		case sig := <-sigs: // wait for a signal
			if Verbose {
				fmt.Println("####################################", sig, "####################################")
			}
			emitter.PrintColorChannelEvents()
			msg.FatalOnErr(rc.Close())
			os.Exit(0) // end
		case <-ticker.C:
		}
	}
}

// Translate performs the trice log task.
//
// Bytes are read with rc. Then according decoder.Encoding they are translated into strings.
// Each read returns the amount of bytes for one trice. rc is called on every
// Translate returns true on io.EOF or false on hard read error or sigterm.
func Translate(sw *emitter.TriceLineComposer, lut id.TriceIDLookUp, m *sync.RWMutex, rc io.ReadCloser) error {
	var dec Decoder //io.Reader
	if Verbose {
		fmt.Println("Encoding is", Encoding)
	}
	var endian bool
	switch TargetEndianess {
	case "littleEndian":
		endian = LittleEndian
	case "bigEndian":
		endian = BigEndian
	default:
		log.Fatalf(fmt.Sprintln("unknown endianness ", TargetEndianess, "-accepting litteEndian or bigEndian."))
	}
	switch strings.ToUpper(Encoding) {
	case "COBS":
		dec = NewCOBSRDecoder(lut, m, rc, endian)
	//case "COBS/R", "COBSR":
	case "CHAR":
		dec = NewCHARDecoder(lut, m, rc, endian)
	case "DUMP":
		dec = NewDUMPDecoder(lut, m, rc, endian)
	//  case "ESC":
	//  	dec = NewEscDecoder(lut, m, rc, endian)
	//  case "FLEX":
	//  	dec = NewFlexDecoder(lut, m, rc, endian)
	default:
		log.Fatalf(fmt.Sprintln("unknown encoding ", Encoding))
	}
	go handleSIGTERM(rc)
	return decodeAndComposeLoop(sw, dec)
}

// decodeAndComposeLoop does not return.
func decodeAndComposeLoop(sw *emitter.TriceLineComposer, dec Decoder) error {
	b := make([]byte, defaultSize) // intermediate trice string buffer
	for {
		n, err := dec.Read(b) // Code to measure
		if io.EOF == err && n == 0 {
			if receiver.Port == "BUFFER" { // do not wait for a predefined buffer
				return err
			}
			if Verbose {
				fmt.Println(err, "-> WAITING...")
			}
			continue // read again
		}
		// b contains here no or several complete trice strings.
		// If several, they end with a newline, despite the last one which optionally ends with a newline.

		// Filtering is done here to suppress the id display as well for the filtered items.
		n = emitter.BanOrPickFilter(b[:n]) // to do: b can contain several trices - handle that!

		start := time.Now()

		var tts bool
		if targetTimestampExists && 0 < n && ShowTargetTimestamp != "" && len(sw.Line) == 0 {
			s := fmt.Sprintf(ShowTargetTimestamp, targetTimestamp)
			_, err := sw.Write([]byte(s))
			msg.OnErr(err)
			if ShowID != "" {
				tts = true
			}
		}

		if tts || (0 < n && ShowID != "" && len(sw.Line) == 0) {
			// dec.Read can return n=0 in some cases and then wait.
			s := fmt.Sprintf(ShowID, LastTriceID)
			_, err := sw.Write([]byte(s))
			msg.OnErr(err)
			tts = false
		}
		m, err := sw.Write(b[:n])
		duration := time.Since(start).Milliseconds()
		if duration > 100 {
			fmt.Println("TriceLineComposer.Write duration =", duration, "ms.")
		}
		msg.InfoOnErr(err, fmt.Sprintln("sw.Write wrote", m, "bytes"))
	}
}

// readU16 returns the 2 b bytes as uint16 according the specified endianness
func (p *decoderData) readU16(b []byte) uint16 {
	if p.endian == LittleEndian {
		return binary.LittleEndian.Uint16(b)
	}
	return binary.BigEndian.Uint16(b)
}

// readU32 returns the 4 b bytes as uint32 according the specified endianness
func (p *decoderData) readU32(b []byte) uint32 {
	if p.endian == LittleEndian {
		return binary.LittleEndian.Uint32(b)
	}
	return binary.BigEndian.Uint32(b)
}

// readU64 returns the 8 b bytes as uint64 according the specified endianness
func (p *decoderData) readU64(b []byte) uint64 {
	if p.endian == LittleEndian {
		return binary.LittleEndian.Uint64(b)
	}
	return binary.BigEndian.Uint64(b)
}

// uReplaceN checks all format specifier in i and replaces %nu with %nd and returns that result as o.
//
// If a replacement took place on position k u[k] is true. Afterwards len(u) is amount of found format specifiers.
// Additionall, if UnsignedHex is true, for FormatX specifiers u[k] is also true.
func uReplaceN(i string) (o string, u []bool) {
	o = i
	s := i
	var offset int
	for {
		loc := matchNextFormatSpecifier.FindStringIndex(s)
		if nil == loc { // no (more) fm found
			return
		}
		offset += loc[1] // track position
		fm := s[loc[0]:loc[1]]
		locU := matchNextFormatUSpecifier.FindStringIndex(fm)
		locX := matchNextFormatXSpecifier.FindStringIndex(fm)
		if nil != locU { // a %nu found
			o = o[:offset-1] + "d" + o[offset:] // replace %nu -> %nd
			u = append(u, true)
		} else if nil != locX && Unsigned { // a %nx, %nX or, %no, %nO or %nb found
			u = append(u, true) // no negative values
		} else { // keep sign
			u = append(u, false)
		}
		s = i[offset:] // remove processed part
	}
}
