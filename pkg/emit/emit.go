// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package emit

import (
	"encoding/binary"
	"errors"
	"fmt"
	"runtime"
	"strings"
	"time"
	"unicode"

	"github.com/fatih/color"
	"github.com/rokath/trice/pkg/id"
)

var ColorPalette = "default"
var TimeStampFormat string = "LOCmicro"

// checkValuePosition is a consistency check for positive values and their position
func checkValuePosition(l id.List, s []byte) error {
	var b []byte // check data
	for i := range l {
		it := l[i]
		switch it.FmtType {
		case "TRICE0":
			b = append(b, 0, 0) // 2 bytes padding like inside Trace()
		case "TRICE8_1":
			b = append(b, 1, 0) // 1 byte padding like inside Trace()
		case "TRICE8_2":
			b = append(b, 1, 2)
		case "TRICE8_3":
			b = append(b, 1, 2, 3, 0) // 1 byte padding like inside Trace()
		case "TRICE8_4":
			b = append(b, 1, 2, 3, 4)
		case "TRICE8_5":
			b = append(b, 1, 2, 3, 4, 5, 0) // 1 byte padding like inside Trace()
		case "TRICE8_6":
			b = append(b, 1, 2, 3, 4, 5, 6)
		case "TRICE8_7":
			b = append(b, 1, 2, 3, 4, 5, 6, 7, 0) // 1 byte padding like inside Trace()
		case "TRICE8_8":
			b = append(b, 1, 2, 3, 4, 5, 6, 7, 8)
		case "TRICE16_1":
			b = append(b, 1, 0)
		case "TRICE16_2":
			b = append(b, 1, 0, 2, 0)
		case "TRICE16_3":
			b = append(b, 1, 0, 2, 0, 3, 0)
		case "TRICE16_4":
			b = append(b, 1, 0, 2, 0, 3, 0, 4, 0)
		case "TRICE32_1":
			b = append(b, 1, 0, 0, 0)
		case "TRICE32_2":
			b = append(b, 1, 0, 0, 0, 2, 0, 0, 0)
		case "TRICE32_3":
			b = append(b, 1, 0, 0, 0, 2, 0, 0, 0, 3, 0, 0, 0)
		case "TRICE32_4":
			b = append(b, 1, 0, 0, 0, 2, 0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0)
		case "TRICE64_1":
			b = append(b, 1, 0, 0, 0, 0, 0, 0, 0)
		case "TRICE64_2":
			b = append(b, 1, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0)
		}
		if "%s" == it.FmtStrg {
			b[0] = s[6] // set simulated pix value equal
		}
		s, _, err := emitter(it, b, s)
		if nil != err {
			return err
		}
		b = b[:0] // empty d for next trice
		err = Visualize(s)
		if nil != err {
			return err
		}
	}
	return nil
}

// checkNegativeValues is a consistency check for negative values
func checkNegativeValues(l id.List, s []byte) error {
	var b []byte // check data
	for i := range l {
		it := l[i]
		switch it.FmtType {
		case "TRICE0":
			b = append(b, 0, 0) // 2 bytes padding like inside Trace()
		case "TRICE8_1":
			b = append(b, 0x80, 0) // 1 byte padding like inside Trace()
		case "TRICE8_2":
			b = append(b, 0x80, 0x80)
		case "TRICE8_3":
			b = append(b, 0x80, 0x80, 0x80, 0) // 1 byte padding like inside Trace()
		case "TRICE8_4":
			b = append(b, 0x80, 0x80, 0x80, 0x80)
		case "TRICE8_5":
			b = append(b, 0x80, 0x80, 0x80, 0x80, 0x80, 0) // 1 byte padding like inside Trace()
		case "TRICE8_6":
			b = append(b, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80)
		case "TRICE8_7":
			b = append(b, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0) // 1 byte padding like inside Trace()
		case "TRICE8_8":
			b = append(b, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80)
		case "TRICE16_1":
			b = append(b, 0, 0x80)
		case "TRICE16_2":
			b = append(b, 0, 0x80, 0x00, 0x80)
		case "TRICE16_3":
			b = append(b, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80)
		case "TRICE16_4":
			b = append(b, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80)
		case "TRICE32_1":
			b = append(b, 0, 0, 0, 0x80)
		case "TRICE32_2":
			b = append(b, 0, 0, 0, 0x80, 0, 0, 0, 0x80)
		case "TRICE32_3":
			b = append(b, 0, 0, 0, 0x80, 0, 0, 0, 0x80, 0, 0, 0, 0x80)
		case "TRICE32_4":
			b = append(b, 0, 0, 0, 0x80, 0, 0, 0, 0x80, 0, 0, 0, 0x80, 0, 0, 0, 0x80)
		case "TRICE64_1":
			b = append(b, 0, 0, 0, 0, 0, 0, 0, 0x80)
		case "TRICE64_2":
			b = append(b, 0, 0, 0, 0, 0, 0, 0, 0x80, 0, 0, 0, 0, 0, 0, 0, 0x80)
		}
		if "%s" == it.FmtStrg {
			b[0] = s[6] // set simulated pix value equal
		}
		s, _, err := emitter(it, b, s)
		if nil != err {
			return err
		}
		b = b[:0] // empty d for next trice
		err = Visualize(s)
		if nil != err {
			return err
		}
	}
	return nil
}

func checkFix(l id.List, s []byte) error {
	b := []byte{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'} // dummy data
	for i := range l {
		it := l[i]
		switch it.FmtType {
		case "TRICE0", "TRICE8_1", "TRICE8_2", "TRICE16_1":
			b = b[:2]
		case "TRICE8_3", "TRICE8_4", "TRICE16_2", "TRICE32_1":
			b = b[:4]
		case "TRICE8_5", "TRICE8_6", "TRICE16_3":
			b = b[:6]
		case "TRICE8_7", "TRICE8_8", "TRICE16_4", "TRICE32_2", "TRICE64_1":
			b = b[:8]
		case "TRICE32_3":
			b = b[:12]
		case "TRICE32_4", "TRICE64_2":
			b = b[:16]
		}
		if "%s" == it.FmtStrg {
			b[0] = s[6] // set simulated pix value equal
		}
		s, _, err := emitter(it, b, s)
		if nil != err {
			return err
		}
		b = b[:0] // empty d for next trice
		err = Visualize(s)
		if nil != err {
			return err
		}
	}
	return nil
}

// Check evaluates the l output with specifiesd dataSet
func Check(l id.List, dataSet string) error {
	s := make([]byte, 0, 12)
	// this is a dummy buffer for a simulated runtime string "RS" with pix = 1 (for easy crc8)
	s = append(s, 0xc0, 0, 0, 0xc0, 0xff, 0xff, 1, 1, 1, 0, 'R', 'S')
	if "position" == dataSet {
		return checkValuePosition(l, s)
	}
	if "negative" == dataSet {
		return checkNegativeValues(l, s)
	}
	return checkFix(l, s)
}

var d = make([]byte, 0, 32) // param collector, usually not more than 16 bytes
var buffer []byte           // container for buffer data (strings)

// Trice emits one trice to std out. The byte slice 't' is a trice package.
// The start byte is 0xeb.
// The ID can be 0, in that case only the data payload is saved
// If b is not empty, the start byte in buffer b is 0xc0. The byte slice b contains a 8 byte header, a 2 byte len-1 value and and len buffer bytes.
// b is used for long runtime strings, when the format string is "%s".
func Trice(t, b []byte, l id.List) ([]byte, error) {
	d = append(d, t[6:8]...)
	i := int(binary.LittleEndian.Uint16(t[4:6]))
	if 0 == i {
		if 0 == len(l) { // "none" as ID list or list empty
			fmt.Printf("% 20x\n", t) //  show raw data
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
	s, b, _ := emitter(it, d, b)
	err = Visualize(s)
	d = d[:0] // empty d for next trice
	return b, err
}

func isLower(s string) bool {
	for _, r := range s {
		if !unicode.IsLower(r) && unicode.IsLetter(r) {
			return false
		}
	}
	return true
}

func colorSetDefault(channel string) (*color.Color, error) {
	var err error
	var c *color.Color
	switch channel {
	case "ERR", "err":
		c = color.New(color.FgYellow).Add(color.Bold).Add(color.BgRed)
	case "WRN", "wrn":
		c = color.New(color.FgRed).Add(color.BgBlack)
	case "MSG", "msg":
		c = color.New(color.FgGreen).Add(color.BgBlack)
	case "RD_", "rd_":
		c = color.New(color.FgMagenta).Add(color.BgBlack)
	case "WR_", "wr_":
		c = color.New(color.FgBlack).Add(color.BgMagenta)
	case "TIM", "tim":
		c = color.New(color.FgBlue).Add(color.BgYellow)
	case "ATT", "att":
		c = color.New(color.FgYellow).Add(color.Bold).Add(color.BgCyan)
	case "DBG", "dbg":
		c = color.New(color.FgCyan).Add(color.BgBlack)
	case "DIA", "dia":
		c = color.New(color.BgHiCyan).Add(color.BgWhite)
	case "ISR", "isr":
		c = color.New(color.FgYellow).Add(color.BgHiBlue)
	case "SIG", "sig":
		c = color.New(color.FgYellow).Add(color.Bold).Add(color.BgGreen)
	case "TST", "tst":
		c = color.New(color.FgYellow).Add(color.BgBlack)
	default:
		c = color.New(color.FgWhite).Add(color.BgBlack)
		err = errors.New("unknown channel info")
	}
	return c, err
}

func colorSetAlternate(channel string) (*color.Color, error) {
	var err error
	var c *color.Color
	switch channel {
	case "ERR", "err":
		c = color.New(color.FgRed).Add(color.Bold).Add(color.BgYellow)
	case "WRN", "wrn":
		c = color.New(color.FgBlack).Add(color.BgRed)
	case "MSG", "msg":
		c = color.New(color.FgBlack).Add(color.BgGreen)
	case "RD_", "rd_":
		c = color.New(color.FgBlack).Add(color.BgMagenta)
	case "WR_", "wr_":
		c = color.New(color.FgMagenta).Add(color.BgBlack)
	case "TIM", "tim":
		c = color.New(color.FgYellow).Add(color.BgBlue)
	case "ATT", "att":
		c = color.New(color.FgCyan).Add(color.Bold).Add(color.BgYellow)
	case "DBG", "dbg":
		c = color.New(color.FgRed).Add(color.BgCyan)
	case "DIA", "dia":
		c = color.New(color.BgHiBlack).Add(color.BgHiCyan)
	case "ISR", "isr":
		c = color.New(color.FgBlack).Add(color.BgYellow)
	case "SIG", "sig":
		c = color.New(color.FgGreen).Add(color.Bold).Add(color.BgYellow)
	case "TST", "tst":
		c = color.New(color.FgRed).Add(color.BgGreen)
	default:
		c = color.New(color.FgWhite).Add(color.BgBlack)
		err = errors.New("unknown channel info")
	}
	return c, err
}

// check for color match and remove color info
// expects s starting with "col:" or "COL:" and returns color and (modified) s
// If no match occuured it returns nil and unchanged s
// If upper case match it returns *color.Color and unchanged s
// If lower case match it returns *color.Color and s without starting pattern "col:"
// col options are: err, wrn, msg, ...
// COL options are: ERR, WRN, MSG, ...
func colorChannel(s string) (*color.Color, string) {
	var c *color.Color
	var err error
	sc := strings.SplitN(s, ":", 2)
	if 2 != len(sc) {
		return c, s
	}
	var r string
	if isLower(sc[0]) {
		r = sc[1] // remove channel info
	} else {
		r = s // keep channel info
	}
	switch ColorPalette {
	case "off":
		color.NoColor = true // disables colorized output
	case "default":
		c, err = colorSetDefault(sc[0])
		if nil != err {
			r = s // keep channel info
		}
	case "alternate":
		c, err = colorSetAlternate(sc[0])
		if nil != err {
			r = s // keep channel info
		}
	}
	return c, r
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

// Visualize is an exported function pointer, which can be redirected for example to a client call
var Visualize = visualize

// visualize displays s and sets color and linebreaks
// The timestamp is printed only and only after \n and with the next s
// todo: The timestamp should be created on client side for more accurate timings
func visualize(s string) error {
	var err error
	s = trimBackslashes(s)
	c, s := colorChannel(s)
	b := color.New(color.FgWhite).Add(color.BgBlack) // assuming this as the default terminal background color
	if true == tsFlag {
		tsFlag = false
		switch TimeStampFormat {
		case "LOCmicro":
			_, err = b.Print(time.Now().Format(time.StampMicro), ": ")
		case "UTCmicro":
			_, err = b.Print(time.Now().UTC().Format(time.StampMicro), ": ")
		case "off": // do nothing
		}
	}
	// When a carriage return is executed, the whole next line gets the current background color.
	// Therefore detect this case and set the color to a default value before the carriage return.
	if strings.HasSuffix(s, "\n") {
		s := strings.TrimSuffix(s, "\n")
		printIt(s, c)
		_, _ = b.Println()
		tsFlag = true
	} else {
		_, err = printIt(s, c)
	}
	return err
}

func printIt(s string, c *color.Color) (int, error) {
	if nil != c {
		//log.Print(s)
		//return 0, nil
		return c.Print(s)
	}
	return fmt.Print(s)
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
