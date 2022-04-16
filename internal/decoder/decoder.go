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

	// littleEndian is true for little endian trice data.
	littleEndian = true

	// bigEndian is the flag value for target endianness.
	bigEndian = false

	// patNextFormatSpecifier is a regex to find next format specifier in a string (exclude %%*) and ignoring %s
	//
	// https://regex101.com/r/BjiD5M/1
	// Language C plus from language Go: %b, %F, %q
	// Partial implemented: %hi, %hu, %ld, %li, %lf, %Lf, %Lu, %lli, %lld
	// Not implemented: %s
	//patNextFormatSpecifier = `(?:^|[^%])(%[0-9]*(-|c|d|e|E|f|F|g|G|h|i|l|L|o|O|p|q|u|x|X|n|b))`
	//patNextFormatSpecifier = `%([+\-#'0-9\.0-9])*(c|d|e|E|f|F|g|G|h|i|l|L|o|O|p|q|u|x|X|n|b|t)` // assumes no `%%` inside string!
	patNextFormatSpecifier = `%([+\-#'0-9\.0-9])*(b|c|d|e|f|g|E|F|G|h|i|l|L|n|o|O|p|q|t|u|x|X)` // assumes no `%%` inside string!

	// patNextFormatUSpecifier is a regex to find next format u specifier in a string
	// It does also match %%u positions!
	//patNextFormatUSpecifier = `(?:%[0-9]*u)`
	patNextFormatUSpecifier = `%[0-9]*u` // assumes no `%%` inside string!

	// patNextFormatISpecifier is a regex to find next format i specifier in a string
	// It does also match %%i positions!
	patNextFormatISpecifier = `%[0-9]*i` // assumes no `%%` inside string!

	// patNextFormatXSpecifier is a regex to find next format x specifier in a string
	// It does also match %%x positions!
	// patNextFormatXSpecifier = `(?:%[0-9]*(l|o|O|x|X|b))`
	patNextFormatXSpecifier = `%[0-9]*(l|o|O|x|X|b|p|t)` // assumes no `%%` inside string!

	// patNextFormatFSpecifier is a regex to find next format f specifier in a string
	// It does also match %%f positions!
	patNextFormatFSpecifier = `%[(+\-0-9\.0-9#]*(e|E|f|F|g|G)` // assumes no `%%` inside string!

	// patNextFormatBoolSpecifier is a regex to find next format f specifier in a string
	// It does also match %%t positions!
	patNextFormatBoolSpecifier = `%t` // assumes no `%%` inside string!

	// patNextFormatPointerSpecifier is a regex to find next format f specifier in a string
	// It does also match %%t positions!
	patNextFormatPointerSpecifier = `%p` // assumes no `%%` inside string!

	// headSize is 4; each trice message starts with a head of 4 bytes.
	headSize = 4

	// hints is the help information in case of errors.
	hints = "att:Hints:Baudrate? Encoding? Interrupt? Overflow? Parameter count? Password? til.json? Version?"
)

var (
	// Verbose gives more information on output if set. The value is injected from main packages.
	Verbose bool

	// ShowID is used as format string for displaying the first trice ID at the start of each line if not "".
	ShowID string

	//// ShowLoc is used as format string for displaying the first trice ID at the start of each line if not "".
	//ShowLoc string

	// lastTriceID is last decoded ID. It is used for switch -showID.
	lastTriceID id.TriceID

	// Encoding describes the way the byte stream is coded.
	Encoding string

	// TargetEndianness if bigEndian assumes a big endian encoded trice stream from the target.
	// To keep target load small, the encoded trice stream from the target matches the target endianess, what us usually littleEndian.
	TargetEndianness string

	// TestTableMode is a special option for easy decoder test table generation.
	TestTableMode bool

	// Unsigned if true, forces hex and in values printed as unsigned values.
	Unsigned bool

	matchNextFormatSpecifier        = regexp.MustCompile(patNextFormatSpecifier)
	matchNextFormatUSpecifier       = regexp.MustCompile(patNextFormatUSpecifier)
	matchNextFormatISpecifier       = regexp.MustCompile(patNextFormatISpecifier)
	matchNextFormatXSpecifier       = regexp.MustCompile(patNextFormatXSpecifier)
	matchNextFormatFSpecifier       = regexp.MustCompile(patNextFormatFSpecifier)
	matchNextFormatBoolSpecifier    = regexp.MustCompile(patNextFormatBoolSpecifier)
	matchNextFormatPointerSpecifier = regexp.MustCompile(patNextFormatPointerSpecifier)

	DebugOut              = false // DebugOut enables debug information.
	DumpLineByteCount     int     // DumpLineByteCount is the bytes per line for the dumpDec decoder.
	initialCycle          = true  // initialCycle is a helper for the cycle counter automatic.
	targetTimestamp       uint32  // targetTimestamp contains target specific timestamp value.
	targetLocation        uint32  // targetLocation contains 16 bit file id in high and 16 bit line number in low part.
	ShowTargetTimestamp   string  // ShowTargetTimestamp is the format string for target timestamps.
	ShowTargetLocation    string  // ShowTargetLocation is the format string for target location: line numer and file name.
	targetTimestampExists bool    // targetTimestampExists is set in dependence of p.COBSModeDescriptor.
	targetLocationExists  bool    // targetLocationExists is set in dependence of p.COBSModeDescriptor.
)

// newDecoder abstracts the function type for a new decoder.
type newDecoder func(out io.Writer, lut id.TriceIDLookUp, m *sync.RWMutex, in io.Reader, endian bool) Decoder

// Decoder is providing a byte reader returning decoded trice's.
// setInput allows switching the input stream to a different source.
type Decoder interface {
	io.Reader
	setInput(io.Reader)
}

// decoderData is the common data struct for all decoders.
type decoderData struct {
	w          io.Writer        // io.Stdout or the like
	in         io.Reader        // in is the inner reader, which is used to get raw bytes
	iBuf       []byte           // iBuf holds unprocessed (raw) bytes for interpretation.
	b          []byte           // read buffer holds a single decoded COBS package, which can contain several trices.
	endian     bool             // endian is true for LittleEndian and false for BigEndian
	triceSize  int              // trice head and payload size as number of bytes
	paramSpace int              // trice payload size after head
	sLen       int              // string length for TRICE_S
	lut        id.TriceIDLookUp // id look-up map for translation
	lutMutex   *sync.RWMutex    // to avoid concurrent map read and map write during map refresh triggered by filewatcher
	trice      id.TriceFmt      // id.TriceFmt // received trice
	//lastInnerRead     time.Time
	//innerReadInterval time.Duration
}

// setInput allows switching the input stream to a different source.
//
// This function is for easier testing with cycle counters.
func (p *decoderData) setInput(r io.Reader) {
	p.in = r
}

// handleSIGTERM is called on CTRL-C shutdown.
func handleSIGTERM(w io.Writer, rc io.ReadCloser) {
	// prepare CTRL-C shutdown reaction
	sigs := make(chan os.Signal, 1)
	signal.Notify(sigs, syscall.SIGINT, syscall.SIGTERM)
	ticker := time.NewTicker(50 * time.Millisecond)
	for {
		select {
		case sig := <-sigs: // wait for a signal
			if Verbose {
				fmt.Fprintln(w, "####################################", sig, "####################################")
			}
			emitter.PrintColorChannelEvents(w)
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
func Translate(w io.Writer, sw *emitter.TriceLineComposer, lut id.TriceIDLookUp, m *sync.RWMutex, rc io.ReadCloser) error {
	var dec Decoder //io.Reader
	if Verbose {
		fmt.Fprintln(w, "Encoding is", Encoding)
	}
	var endian bool
	switch TargetEndianness {
	case "littleEndian":
		endian = littleEndian
	case "bigEndian":
		endian = bigEndian
	default:
		log.Fatalf(fmt.Sprintln("unknown endianness ", TargetEndianness, "-accepting litteEndian or bigEndian."))
	}
	switch strings.ToUpper(Encoding) {
	case "COBS":
		dec = newCOBSDecoder(w, lut, m, rc, endian)
		//cobsVariantDecode = cobs.Decode
	//  case "COBSFF":
	//  	dec = newCOBSDecoder(w, lut, m, rc, endian)
	//  	cobsVariantDecode = cobsFFDecode
	case "CHAR":
		dec = newCHARDecoder(w, lut, m, rc, endian)
	case "DUMP":
		dec = newDUMPDecoder(w, lut, m, rc, endian)
	default:
		log.Fatalf(fmt.Sprintln("unknown encoding ", Encoding))
	}
	go handleSIGTERM(w, rc)
	return decodeAndComposeLoop(w, sw, dec, lut)
}

// decodeAndComposeLoop does not return.
func decodeAndComposeLoop(w io.Writer, sw *emitter.TriceLineComposer, dec Decoder, lut id.TriceIDLookUp) error {
	b := make([]byte, defaultSize) // intermediate trice string buffer
	for {
		n, err := dec.Read(b) // Code to measure, dec.Read can return n=0 in some cases and then wait.

		if err != io.EOF && err != nil {
			log.Fatal(err)
		}

		if n == 0 {
			if receiver.Port == "FILEBUFFER" && err == io.EOF { // do not wait if a predefined buffer
				_, err := sw.Write([]byte(`\n`)) // add newline as line end to display any started line
				msg.OnErr(err)
				return io.EOF
			}
			if Verbose {
				fmt.Fprintln(w, err, "-> WAITING...")
			}
			if receiver.Port == "FILE" {
				time.Sleep(100 * time.Millisecond)
			}
			continue // read again
		}

		// b contains here none or several complete trice strings.
		// If several, they end with a newline each, despite the last one which optionally ends with a newline.
		start := time.Now()

		// Filtering is done here to suppress the loc, timestamp and id display as well for the filtered items.
		n = emitter.BanOrPickFilter(b[:n]) // todo: b can contain several trices - handle that!

		if n > 0 { // s.th. to write out
			var logLineStart bool // logLineStart is a helper flag for log line start detection
			if len(sw.Line) == 0 {
				logLineStart = true
			}

			// If target location & enabled and line start, write target location.
			if logLineStart && targetLocationExists && ShowTargetLocation != "" {
				targetFileID := id.TriceID(targetLocation >> 16)
				t := lut[targetFileID]
				targetFile := t.Strg
				s := fmt.Sprintf(ShowTargetLocation, targetFile, 0xffff&targetLocation)
				_, err := sw.Write([]byte(s))
				msg.OnErr(err)
			}

			// If target timestamp & enabled and line start, write target timestamp.
			if logLineStart && targetTimestampExists && ShowTargetTimestamp != "" {
				s := fmt.Sprintf(ShowTargetTimestamp, targetTimestamp)
				_, err := sw.Write([]byte(s))
				msg.OnErr(err)
			}

			// write ID only if enabled and line start.
			if logLineStart && ShowID != "" {
				s := fmt.Sprintf(ShowID, lastTriceID)
				_, err := sw.Write([]byte(s))
				msg.OnErr(err)
			}
			_, err := sw.Write(b[:n])
			msg.OnErr(err)
		}

		duration := time.Since(start).Milliseconds()
		if duration > 100 {
			fmt.Fprintln(w, "TriceLineComposer.Write duration =", duration, "ms.")
		}
		//msg.InfoOnErr(err, fmt.Sprintln("sw.Write wrote", m, "bytes"))
	}
}

// readU16 returns the 2 b bytes as uint16 according the specified endianness
func (p *decoderData) readU16(b []byte) uint16 {
	if p.endian {
		return binary.LittleEndian.Uint16(b)
	}
	return binary.BigEndian.Uint16(b)
}

// readU32 returns the 4 b bytes as uint32 according the specified endianness
func (p *decoderData) readU32(b []byte) uint32 {
	if p.endian {
		return binary.LittleEndian.Uint32(b)
	}
	return binary.BigEndian.Uint32(b)
}

// readU64 returns the 8 b bytes as uint64 according the specified endianness
func (p *decoderData) readU64(b []byte) uint64 {
	if p.endian {
		return binary.LittleEndian.Uint64(b)
	}
	return binary.BigEndian.Uint64(b)
}

// uReplaceN checks all format specifier in i and replaces %nu with %nd and returns that result as o.
//
// If a replacement took place on position k u[k] is 1. Afterwards len(u) is amount of found format specifiers.
// Additional, if UnsignedHex is true, for FormatX specifiers u[k] is also 1.
// If a float format specifier was found at position k, u[k] is 2,
// http://www.cplusplus.com/reference/cstdio/printf/
// https://www.codingunit.com/printf-format-specifiers-format-conversions-and-formatted-output
func uReplaceN(i string) (o string, u []int) {
	o = i
	i = strings.ReplaceAll(i, "%%", "__") // this makes regex easier and faster
	var offset int
	for {
		s := i[offset:] // remove processed part
		loc := matchNextFormatSpecifier.FindStringIndex(s)
		if nil == loc { // no (more) fm found
			return
		}
		offset += loc[1] // track position
		fm := s[loc[0]:loc[1]]
		locPointer := matchNextFormatPointerSpecifier.FindStringIndex(fm)
		if nil != locPointer { // a %p found
			u = append(u, 4) // pointer value
			continue
		}
		locBool := matchNextFormatBoolSpecifier.FindStringIndex(fm)
		if nil != locBool { // a %t found
			u = append(u, 3) // bool value
			continue
		}
		locF := matchNextFormatFSpecifier.FindStringIndex(fm)
		if nil != locF { // a %nf found
			u = append(u, 2) // float value
			continue
		}
		locU := matchNextFormatUSpecifier.FindStringIndex(fm)
		if nil != locU { // a %nu found
			o = o[:offset-1] + "d" + o[offset:] // replace %nu -> %nd
			u = append(u, 0)                    // no negative values
			continue
		}
		locI := matchNextFormatISpecifier.FindStringIndex(fm)
		if nil != locI { // a %ni found
			o = o[:offset-1] + "d" + o[offset:] // replace %ni -> %nd
			u = append(u, 1)                    // also negative values
			continue
		}
		locX := matchNextFormatXSpecifier.FindStringIndex(fm)
		if nil != locX { // a %nx, %nX or, %no, %nO or %nb found
			if Unsigned {
				u = append(u, 0) // no negative values
			} else {
				u = append(u, 1) // also negative values
			}
			continue
		}
		u = append(u, 1) // keep sign in all other cases(also negative values)
	}
}
