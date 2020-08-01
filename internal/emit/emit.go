// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package emit is responsible for generating a string slice for each line.
// The substrings are optionally prefix, timestamp, several content substrings and postfix.
// Each substring can contain its own color channel as prefix ("col:").
// The colors are converted later inside the disp.Print() function.
package emit

import (
	"encoding/binary"
	"errors"
	"fmt"
	"strings"

	"github.com/rokath/trice/internal/disp"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/pkg/lib"
)

var (
	// Prefix is a (configurable) string added to each line end
	Prefix = ""

	// Postfix is a (configurable) string added to each line end
	Postfix = "\n"

	// param collector, usually not more than 16 bytes
	d = make([]byte, 0, 32)

	// container for buffer data (strings)
	buffer []byte

	// css is a collector for line substrings
	// it is used only inside LineCollect() but needs to survive from call to call
	css []string

	// DiscardByte is the function to execute if a data stream byte is to be discarded
	// type DicardFunc func(byte)
	DiscardByte = DiscardWithMessage
)

// Clear removes data garbage from inner structs, accumulated during RTTD sync process
func Clear() {
	d = d[:0]
	buffer = buffer[:0]
	css = css[:0]
}

// Trice emits one trice. The byte slice 't' is a trice package.
// The start byte is 0xeb.
// The ID can be 0, in that case only the data payload is saved
// If b is not empty, the start byte in buffer b is 0xc0.
// The byte slice b contains a 8 byte header, a 2 byte len-1 value and and len buffer bytes.
// b is used for long runtime strings, when the format string is "%s".
// Trice returns b unchanged usually, but when the format string is "%s" it removes the
// first long runtime string from there and returns the remaining data in b.
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
		fmt.Println(d)              //  show acc data
		d = d[:0]                   // empty d for next trice
		return b, nil
	}
	x, err := id.Index(i, l)
	if nil != err {
		d = d[:0]     // empty d for next trice
		return b, err // i is unknown ID
	}
	it := l[x]
	s, b, _ := Emitter(it, d, b)
	LineCollect(s)
	d = d[:0] // empty d for next trice
	return b, nil
}

// TrimBackslashes handles special chars in s
func TrimBackslashes(s string) string {
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
func LangCtoGoFmtStingConverter(f string) (string, []bool, error) {
	var err error
	var u []bool
	s := f // TODO: parse f for %*u, replace with %*d and sett []bool values accordingly
	return s, u, err
}

// errFalseLen reports an error in case of false len, what could be caused by data garbage
func errFalseLen(it id.Item, t, b []byte) (string, []byte, error) {
	s := fmt.Sprintln("ERR:", it, "false len", len(t), "in data stream", t, b)
	return s, b, errors.New("data stream error")
}

// EvalLen checks if byte buffer t has appropriate length to id.item it
func EvalLen(it id.Item, t []byte) error {
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

// emmiter works fine with %x, %d and %o but NOT with %u for now
// %x is emitted in Go signed!
// For %u a format string parsing is needed to perform the correct casts.
func Emitter(it id.Item, t, b []byte) (string, []byte, error) {
	f, _, err := LangCtoGoFmtStingConverter(it.FmtStrg)
	var s string
	var v0, v1, v2, v3 int16
	var w0, w1, w2, w3 int32
	var l0, l1 int64
	err = EvalLen(it, t)
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

// LineCollect collects s into an internal line substring slice
// When s ends with a newline it is trimmed and the slice goes to Out and is discarded afterwards
func LineCollect(s string) {
	s = TrimBackslashes(s)

	a := func(su string) {
		css = append(css, su)
	}
	if 0 == len(css) {
		a(Prefix)
		a(lib.Timestamp())
	}
	if false == strings.HasSuffix(s, "\n") {
		a(s)
		return
	}
	s = strings.TrimSuffix(s, "\n")
	a(s)
	a(Postfix)
	disp.Out(css)
	css = css[:0] // discard slice data
}

// DiscardWithMessage discards a byte with a verbose message
func DiscardWithMessage(b byte) {
	c := byte(' ')
	if 32 <= b && b <= 127 {
		c = b
	}
	LineCollect(fmt.Sprintf("wrn:discarding byte 0x%02x (dez %d, char '%c')\n", b, b, c))
}

// DiscardASCII discards a byte assuming to be printable and prints it.
func DiscardASCII(c byte) {
	LineCollect(fmt.Sprintf("%c", c))
}

// DiscardSilent discards a byte silently
func DiscardSilent(c byte) {
}
