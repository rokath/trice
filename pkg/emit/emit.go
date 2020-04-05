// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// emit package is responsible for generating a string slice for each line.
// The substrings are optionally prefix, timestamp, several content substrings and postfix.
// Each substring can contain its own color channel as prefix ("col:").
// The colors are converted later inside the disp.Print() function.
package emit

import (
	"encoding/binary"
	"errors"
	"fmt"
	"runtime"
	"strings"
	"time"

	"github.com/rokath/trice/pkg/disp"
	"github.com/rokath/trice/pkg/id"
)

var (
	// TimeStampFormat is the PC timestamp format
	TimeStampFormat = "LOCmicro"

	// Prefix is a (configurable) string added to each line end
	Prefix = ""

	// Postfix is a (configurable) string added to each line end
	Postfix = "\n"
)

var d = make([]byte, 0, 32) // param collector, usually not more than 16 bytes
var buffer []byte           // container for buffer data (strings)

// Trice emits one trice to std out. The byte slice 't' is a trice package.
// The start byte is 0xeb.
// The ID can be 0, in that case only the data payload is saved
// If b is not empty, the start byte in buffer b is 0xc0. The byte slice b contains a 8 byte header, a 2 byte len-1 value and and len buffer bytes.
// b is used for long runtime strings, when the format string is "%s".
func Trice(t, b []byte, l id.ListT) ([]byte, error) {
	d = append(d, t[6:8]...)
	i := int(binary.LittleEndian.Uint16(t[4:6]))
	if 0 == i {
		if 0 == len(l) { // "none" as ID list or list empty
			fmt.Printf("% 20x\n", t) //  show raw data
			//mt.Fprintf(Tee, "% 20x\n", t) //  show raw data
		}
		return b, nil // only params
	}
	if 0 == len(l) { // "none" as ID list or list empty
		fmt.Printf("% 20x acc:", t) //  show raw data
		//fmt.Fprintf(Tee, "% 20x acc:", t) //  show raw data
		fmt.Println(d) //  show acc data
		//fmt.Fprintln(Tee, d)              //  show acc data
		d = d[:0] // empty d for next trice
		return b, nil
	}
	x, err := id.Index(i, l)
	if nil != err {
		d = d[:0]     // empty d for next trice
		return b, err // i is unknown ID
	}
	it := l[x]
	s, b, _ := emitter(it, d, b)
	LineCollect(s)
	d = d[:0] // empty d for next trice
	return b, nil
}

func trimBackslashes(s string) string {
	s = strings.ReplaceAll(s, "\\a", "\a")
	s = strings.ReplaceAll(s, "\\b", "\b")
	s = strings.ReplaceAll(s, "\\t", "\t")
	s = strings.ReplaceAll(s, "\\n", "\n")
	s = strings.ReplaceAll(s, "\\f", "\f")
	s = strings.ReplaceAll(s, "\\r", "\r")
	s = strings.ReplaceAll(s, "\\v", "\v")
	// There four more formats to handle: (TODO)
	//   \x followed by exactly two hexadecimal digits,
	//   \ followed by exactly three octal digits,
	//   \u followed by exactly four hexadecimal digits,
	//   \U followed by exactly eight hexadecimal digits.
	return s
}

var tsFlag = true

// parse lang C formatstring for %u and replace them with %d and extend the
// returned slice with 0 for each %d, %c, %x and 1 for each converted %u
// http://www.cplusplus.com/reference/cstdio/printf/
//
// Changes:        C    Go
// ---------------------------
// int 	           %i   %d replace i with d
// unsigned int    %u   %d replace u with d and uint()
//
// No changes:     C    Go
// ---------------------------
// decimal         %d   %d
// hexadezimal     %x   %x
// HEXADECIMAL     %X   %X
// octal           %o   %o
// float           %f   %f
// char            %c   %c
// (void *)        %p   %p
func langCtoGoFmtStingConverter(f string) (string, []bool, error) {
	var err error
	var u []bool
	s := f // TODO: parse f for %*u, replace with %*d and sett []bool values accordingly
	return s, u, err
}

func fileLine() string {
	_, fileName, fileLine, ok := runtime.Caller(1)
	var s string
	if ok {
		s = fmt.Sprintf("%s:%d", fileName, fileLine)
	} else {
		s = ""
	}
	return s
}

// errFalseLen reports an error becvause of false len
func errFalseLen(it id.Item, t, b []byte) (string, []byte, error) {
	s := fmt.Sprintln("ERR:", it, "false len", len(t), "in data stream", t, b)
	return s, b, errors.New("data stream error")
}

// evalLen checks if byte buffer t has appropriate length to id.item it
func evalLen(it id.Item, t []byte) error {
	err := errors.New("false len")
	switch it.FmtType {
	case "TRICE0", "TRICE8_1", "TRICE8_2", "TRICE16_1":
		if 2 != len(t) {
			return err
		}
	case "TRICE8_3", "TRICE8_4", "TRICE16_2", "TRICE32_1":
		if 4 != len(t) {
			return err
		}
	case "TRICE8_5", "TRICE8_6", "TRICE16_3":
		if 6 != len(t) {
			return err
		}
	case "TRICE8_7", "TRICE8_8", "TRICE16_4", "TRICE32_2", "TRICE64_1":
		if 8 != len(t) {
			return err
		}
	case "TRICE32_3":
		if 12 != len(t) {
			return err
		}
	case "TRICE32_4", "TRICE64_2":
		if 16 != len(t) {
			return err
		}
	}
	return nil
}

// emmiter wworks fine with %x, %d and %o but NOT with %u for now
// %x is emitted in Go signed!
// For %u a format string parsing is needed to perform the correct casts.
func emitter(it id.Item, t, b []byte) (string, []byte, error) {
	f, _, err := langCtoGoFmtStingConverter(it.FmtStrg)
	var s string
	var v0, v1, v2, v3 int16
	var w0, w1, w2, w3 int32
	var l0, l1 int64
	err = evalLen(it, t)
	if nil != err {
		return errFalseLen(it, t, b)
	}
	switch it.FmtType {
	case "TRICE0":
		s = f
	case "TRICE8_1":
		if "%s" != f { // normal case
			s = fmt.Sprintf(f, int8(t[0]))
		} else if 11 > len(b) || // 8 byte header + 2 byte count + at least 1 byte data
			t[0] != b[6] { // t[0] is parameter and b[6] pix and they must be equal
			b = nil
			return errFalseLen(it, t, b)
		} else { // ok
			sLen := 1 + int(binary.LittleEndian.Uint16(b[8:10])) // sLen is -1 value
			b = b[10:]                                           // discard old header & sLen
			s = string(b[:sLen])                                 // use string
			b = b[sLen:]                                         // discard old string
		}
	case "TRICE8_2":
		s = fmt.Sprintf(f, int8(t[0]), int8(t[1]))
	case "TRICE8_3":
		s = fmt.Sprintf(f, int8(t[0]), int8(t[1]), int8(t[2]))
	case "TRICE8_4":
		s = fmt.Sprintf(f, int8(t[0]), int8(t[1]), int8(t[2]), int8(t[3]))
	case "TRICE8_5":
		s = fmt.Sprintf(f, int8(t[0]), int8(t[1]), int8(t[2]), int8(t[3]), int8(t[4]))
	case "TRICE8_6":
		s = fmt.Sprintf(f, int8(t[0]), int8(t[1]), int8(t[2]), int8(t[3]), int8(t[4]), int8(t[5]))
	case "TRICE8_7":
		s = fmt.Sprintf(f, int8(t[0]), int8(t[1]), int8(t[2]), int8(t[3]), int8(t[4]), int8(t[5]), int8(t[6]))
	case "TRICE8_8":
		s = fmt.Sprintf(f, int8(t[0]), int8(t[1]), int8(t[2]), int8(t[3]), int8(t[4]), int8(t[5]), int8(t[6]), int8(t[7]))
	case "TRICE16_1":
		v0 = int16(binary.LittleEndian.Uint16(t[0:2]))
		s = fmt.Sprintf(f, v0)
	case "TRICE16_2":
		v0 = int16(binary.LittleEndian.Uint16(t[0:2]))
		v1 = int16(binary.LittleEndian.Uint16(t[2:4]))
		s = fmt.Sprintf(f, v0, v1)
	case "TRICE16_3":
		v0 = int16(binary.LittleEndian.Uint16(t[0:2]))
		v1 = int16(binary.LittleEndian.Uint16(t[2:4]))
		v2 = int16(binary.LittleEndian.Uint16(t[4:6]))
		s = fmt.Sprintf(f, v0, v1, v2)
	case "TRICE16_4":
		v0 = int16(binary.LittleEndian.Uint16(t[0:2]))
		v1 = int16(binary.LittleEndian.Uint16(t[2:4]))
		v2 = int16(binary.LittleEndian.Uint16(t[4:6]))
		v3 = int16(binary.LittleEndian.Uint16(t[6:8]))
		s = fmt.Sprintf(f, v0, v1, v2, v3)
	case "TRICE32_1":
		w0 = int32(binary.LittleEndian.Uint32(t[0:4]))
		s = fmt.Sprintf(f, w0)
	case "TRICE32_2":
		w0 = int32(binary.LittleEndian.Uint32(t[0:4]))
		w1 = int32(binary.LittleEndian.Uint32(t[4:8]))
		s = fmt.Sprintf(f, w0, w1)
	case "TRICE32_3":
		w0 = int32(binary.LittleEndian.Uint32(t[0:4]))
		w1 = int32(binary.LittleEndian.Uint32(t[4:8]))
		w2 = int32(binary.LittleEndian.Uint32(t[8:12]))
		s = fmt.Sprintf(f, w0, w1, w2)
	case "TRICE32_4":
		w0 = int32(binary.LittleEndian.Uint32(t[0:4]))
		w1 = int32(binary.LittleEndian.Uint32(t[4:8]))
		w2 = int32(binary.LittleEndian.Uint32(t[8:12]))
		w3 = int32(binary.LittleEndian.Uint32(t[12:16]))
		s = fmt.Sprintf(f, w0, w1, w2, w3)
	case "TRICE64_1":
		l0 = int64(binary.LittleEndian.Uint64(t[0:8]))
		s = fmt.Sprintf(f, l0)
	case "TRICE64_2":
		l0 = int64(binary.LittleEndian.Uint64(t[0:8]))
		l1 = int64(binary.LittleEndian.Uint64(t[8:16]))
		s = fmt.Sprintf(f, l0, l1)
	default:
		return "ERR: INTERNAL ERROR!!!", b, errors.New("ERR: INTERNAL ERROR")
	}
	return s, b, err
}

// timestamp returns local time as string according var TimeStampFormat
func timestamp() string {
	var s string
	switch TimeStampFormat {
	case "LOCmicro":
		s = time.Now().Format(time.StampMicro) + "  "
	case "UTCmicro":
		s = "UTC " + time.Now().UTC().Format(time.StampMicro) + "  "
	case "off":
		s = ""
	}
	return s
}

// LineCollect collects s into an internal line substring slice
// When s ends with a newline it is trimmed and the slice goes to Visualize and is discarded afterwards
func LineCollect(s string) {
	s = trimBackslashes(s)
	var ss []string
	a := func(su string) { // this closure is needed to treat ss as surviving slice
		ss = append(ss, su)
	}
	if 0 == len(ss) {
		a(Prefix)
		a(timestamp())
	}
	if !strings.HasSuffix(s, "\n") {
		a(s)
		return
	}
	s = strings.TrimSuffix(s, "\n")
	a(s)
	a(Postfix)
	disp.Visualize(ss)
	ss = nil
}
