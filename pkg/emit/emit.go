// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package emit

import (
	"encoding/binary"
	"errors"
	"fmt"
	"strings"
	"unicode"

	"github.com/fatih/color"
	"github.com/rokath/trice/pkg/id"
)

// checkValuePosition is a consistency check for positive values and their position
func checkValuePosition(l id.List, palette string) error {
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
		}
		s, err := emitter(it, b)
		if nil != err {
			return err
		}
		b = b[:0] // empty d for next trice
		err = visualize(s, palette)
		if nil != err {
			return err
		}
	}
	return nil
}

// checkNegativeValues is a consistency check for negative values
func checkNegativeValues(l id.List, palette string) error {
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
		}
		s, err := emitter(it, b)
		if nil != err {
			return err
		}
		b = b[:0] // empty d for next trice
		err = visualize(s, palette)
		if nil != err {
			return err
		}
	}
	return nil
}

func checkFix(l id.List, palette string) error {
	b := []byte{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'} // dummy data
	for i := range l {
		it := l[i]
		switch it.FmtType {
		case "TRICE0":
			fallthrough
		case "TRICE8_1":
			fallthrough
		case "TRICE8_2":
			fallthrough
		case "TRICE16_1":
			b = b[:2]
		case "TRICE8_3":
			fallthrough
		case "TRICE8_4":
			fallthrough
		case "TRICE16_2":
			fallthrough
		case "TRICE32_1":
			b = b[:4]
		case "TRICE8_5":
			fallthrough
		case "TRICE8_6":
			fallthrough
		case "TRICE16_3":
			b = b[:6]
		}
		s, err := emitter(it, b)
		if nil != err {
			return err
		}
		b = b[:0] // empty d for next trice
		err = visualize(s, palette)
		if nil != err {
			return err
		}
	}
	return nil
}

// Check evaluates the l output with specifiesd dataSet
func Check(l id.List, dataSet, palette string) error {
	if "position" == dataSet {
		return checkValuePosition(l, palette)
	}
	if "negative" == dataSet {
		return checkNegativeValues(l, palette)
	}
	return checkFix(l, palette)
}

var d = make([]byte, 1024) // param collector, usually not more than 8 bytes

// Trice emits one trice to std out. The byte slice 'b' is a trice package.
// The ID can be 0, in that case only the data payload is saved
func Trice(b []byte, l id.List, palette string) error {
	d = append(d, b[6:8]...)
	i := int(binary.LittleEndian.Uint16(b[4:6]))
	if 0 == i {
		if 0 == len(l) { // "none" as ID list or list empty
			fmt.Printf("% 20x\n", b) //  show raw data
		}
		return nil // only params
	}
	if 0 == len(l) { // "none" as ID list or list empty
		fmt.Printf("% 20x acc:", b) //  show raw data
		fmt.Println(d)              //  show acc data
		d = d[:0]                   // empty d for next trice
		return nil
	}
	x, err := id.Index(i, l)
	if nil != err {
		d = d[:0]  // empty d for next trice
		return err // i is unknown ID
	}
	it := l[x]
	s, _ := emitter(it, d)
	err = visualize(s, palette)
	d = d[:0] // empty d for next trice
	return err
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
	case "ERR":
		fallthrough
	case "err":
		c = color.New(color.FgYellow).Add(color.Bold).Add(color.BgRed)
	case "WRN":
		fallthrough
	case "wrn":
		c = color.New(color.FgRed).Add(color.BgBlack)
	case "MSG":
		fallthrough
	case "msg":
		c = color.New(color.FgGreen).Add(color.BgBlack)
	case "RD_":
		fallthrough
	case "rd_":
		c = color.New(color.FgMagenta).Add(color.BgBlack)
	case "WR_":
		fallthrough
	case "wr_":
		c = color.New(color.FgBlack).Add(color.BgMagenta)
	case "TIM":
		fallthrough
	case "tim":
		c = color.New(color.FgBlue).Add(color.BgYellow)
	case "ATT":
		fallthrough
	case "att":
		c = color.New(color.FgYellow).Add(color.Bold).Add(color.BgCyan)
	case "DBG":
		fallthrough
	case "dbg":
		c = color.New(color.FgCyan).Add(color.BgBlack)
	case "DIA":
		fallthrough
	case "dia":
		c = color.New(color.BgHiCyan).Add(color.BgWhite)
	case "ISR":
		fallthrough
	case "isr":
		c = color.New(color.FgWhite).Add(color.BgBlack)
	case "SIG":
		fallthrough
	case "sig":
		c = color.New(color.FgYellow).Add(color.Bold).Add(color.BgGreen)
	case "TST":
		fallthrough
	case "tst":
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
	case "ERR":
		fallthrough
	case "err":
		c = color.New(color.FgYellow).Add(color.Bold).Add(color.BgRed)
	case "WRN":
		fallthrough
	case "wrn":
		c = color.New(color.FgRed).Add(color.BgBlack)
	case "MSG":
		fallthrough
	case "msg":
		c = color.New(color.FgGreen).Add(color.BgBlack)
	case "RD_":
		fallthrough
	case "rd_":
		c = color.New(color.FgMagenta).Add(color.BgBlack)
	case "WR_":
		fallthrough
	case "wr_":
		c = color.New(color.FgBlack).Add(color.BgMagenta)
	case "TIM":
		fallthrough
	case "tim":
		c = color.New(color.FgBlue).Add(color.BgYellow)
	case "ATT":
		fallthrough
	case "att":
		c = color.New(color.FgYellow).Add(color.Bold).Add(color.BgCyan)
	case "DBG":
		fallthrough
	case "dbg":
		c = color.New(color.FgCyan).Add(color.BgRed)
	case "DIA":
		fallthrough
	case "dia":
		c = color.New(color.BgHiCyan).Add(color.BgHiBlack)
	case "ISR":
		fallthrough
	case "isr":
		c = color.New(color.FgWhite).Add(color.BgBlack)
	case "SIG":
		fallthrough
	case "sig":
		c = color.New(color.FgYellow).Add(color.Bold).Add(color.BgGreen)
	case "TST":
		fallthrough
	case "tst":
		c = color.New(color.FgYellow).Add(color.BgGreen)
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
func colorChannel(s, palette string) (*color.Color, string) {
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
	switch palette {
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

// set color and linebreaks
func visualize(s, palette string) error {
	var err error
	s = trimBackslashes(s)
	c, s := colorChannel(s, palette)

	// When a carriage return is executed, the whole next line gets the current background color.
	// Therefore detect this case and set the color to a default value before the carriage return.
	if strings.HasSuffix(s, "\n") {
		s := strings.TrimSuffix(s, "\n")
		printIt(s, c)
		b := color.New(color.FgWhite).Add(color.BgBlack) // assuming this as the default terminal color
		_, err = b.Println()
	} else {
		_, err = printIt(s, c)
	}
	return err
}

func printIt(s string, c *color.Color) (int, error) {
	if nil != c {
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

// emmiter wworks fine with %x, %d and %o but NOT with %u for now
// %x is emitted in Go signed!
// For %u a format string parsing is needed to perform the correct casts.
func emitterGo(it id.Item, b []byte) (string, error) {
	f, _, err := langCtoGoFmtStingConverter(it.FmtStrg)
	var s string
	var v0, v1, v2, v3 int16
	var w0, w1, w2, w3 int32
	var l0 int64
	switch it.FmtType {
	case "TRICE0":
		if 2 != len(b) { // b has 2 padding bytes
			fmt.Println(b)
			return "ERR: DATA STREAM ERROR!!!", errors.New("ERR: DATA STREAM ERROR")
		}
		s = f
	case "TRICE8_1":
		if 2 != len(b) {
			fmt.Println(b)
			return "ERR: DATA STREAM ERROR!!!", errors.New("ERR: DATA STREAM ERROR")
		}
		s = fmt.Sprintf(f, int8(b[0]))
	case "TRICE8_2":
		if 2 != len(b) {
			fmt.Println(b)
			return "ERR: DATA STREAM ERROR!!!", errors.New("ERR: DATA STREAM ERROR")
		}
		s = fmt.Sprintf(f, int8(b[0]), int8(b[1]))
	case "TRICE8_3":
		if 4 != len(b) { // b has 1 padding byte
			fmt.Println(b)
			return "ERR: DATA STREAM ERROR!!!", errors.New("ERR: DATA STREAM ERROR")
		}
		s = fmt.Sprintf(f, int8(b[0]), int8(b[1]), int8(b[2]))
	case "TRICE8_4":
		if 4 != len(b) {
			fmt.Println(b)
			return "ERR: DATA STREAM ERROR!!!", errors.New("ERR: DATA STREAM ERROR")
		}
		s = fmt.Sprintf(f, int8(b[0]), int8(b[1]), int8(b[2]), int8(b[3]))
	case "TRICE8_5":
		if 6 != len(b) { // b has 1 padding byte
			fmt.Println(b)
			return "ERR: DATA STREAM ERROR!!!", errors.New("ERR: DATA STREAM ERROR")
		}
		s = fmt.Sprintf(f, int8(b[0]), int8(b[1]), int8(b[2]), int8(b[3]), int8(b[4]))
	case "TRICE8_6":
		if 6 != len(b) {
			fmt.Println(b)
			return "ERR: DATA STREAM ERROR!!!", errors.New("ERR: DATA STREAM ERROR")
		}
		s = fmt.Sprintf(f, int8(b[0]), int8(b[1]), int8(b[2]), int8(b[3]), int8(b[4]), int8(b[5]))
	case "TRICE8_7":
		if 8 != len(b) { // b has 1 padding byte
			fmt.Println(b)
			return "ERR: DATA STREAM ERROR!!!", errors.New("ERR: DATA STREAM ERROR")
		}
		s = fmt.Sprintf(f, int8(b[0]), int8(b[1]), int8(b[2]), int8(b[3]), int8(b[4]), int8(b[5]), int8(b[6]))
	case "TRICE8_8":
		if 8 != len(b) {
			fmt.Println(b)
			return "ERR: DATA STREAM ERROR!!!", errors.New("ERR: DATA STREAM ERROR")
		}
		s = fmt.Sprintf(f, int8(b[0]), int8(b[1]), int8(b[2]), int8(b[3]), int8(b[4]), int8(b[5]), int8(b[6]), int8(b[7]))
	case "TRICE16_1":
		if 2 != len(b) {
			return "TRICE16_1", fmt.Errorf("false len %v", b)
		}
		v0 = int16(binary.LittleEndian.Uint16(b[0:2]))
		s = fmt.Sprintf(f, v0)
	case "TRICE16_2":
		if 4 != len(b) {
			return "TRICE16_2", fmt.Errorf("false len %v", b)
		}
		v0 = int16(binary.LittleEndian.Uint16(b[0:2]))
		v1 = int16(binary.LittleEndian.Uint16(b[2:4]))
		s = fmt.Sprintf(f, v0, v1)
	case "TRICE16_3":
		if 6 != len(b) {
			return "TRICE16_3", fmt.Errorf("false len %v", b)
		}
		v0 = int16(binary.LittleEndian.Uint16(b[0:2]))
		v1 = int16(binary.LittleEndian.Uint16(b[2:4]))
		v2 = int16(binary.LittleEndian.Uint16(b[4:6]))
		s = fmt.Sprintf(f, v0, v1, v2)
	case "TRICE16_4":
		if 8 != len(b) {
			return "TRICE16_4", fmt.Errorf("false len %v", b)
		}
		v0 = int16(binary.LittleEndian.Uint16(b[0:2]))
		v1 = int16(binary.LittleEndian.Uint16(b[2:4]))
		v2 = int16(binary.LittleEndian.Uint16(b[4:6]))
		v3 = int16(binary.LittleEndian.Uint16(b[6:8]))
		s = fmt.Sprintf(f, v0, v1, v2, v3)
	case "TRICE32_1":
		if 4 != len(b) {
			return "TRICE32_1", fmt.Errorf("false len %v", b)
		}
		w0 = int32(binary.LittleEndian.Uint32(b[0:4]))
		s = fmt.Sprintf(f, w0)
	case "TRICE32_2":
		if 8 != len(b) {
			return "TRICE32_2", fmt.Errorf("false len %v", b)
		}
		w0 = int32(binary.LittleEndian.Uint32(b[0:4]))
		w1 = int32(binary.LittleEndian.Uint32(b[4:8]))
		s = fmt.Sprintf(f, w0, w1)
	case "TRICE32_3":
		if 12 != len(b) {
			return "TRICE32_3", fmt.Errorf("false len %v", b)
		}
		w0 = int32(binary.LittleEndian.Uint32(b[0:4]))
		w1 = int32(binary.LittleEndian.Uint32(b[4:8]))
		w2 = int32(binary.LittleEndian.Uint32(b[8:12]))
		s = fmt.Sprintf(f, w0, w1, w2)
	case "TRICE32_4":
		if 16 != len(b) {
			return "TRICE32_4", fmt.Errorf("false len %v", b)
		}
		w0 = int32(binary.LittleEndian.Uint32(b[0:4]))
		w1 = int32(binary.LittleEndian.Uint32(b[4:8]))
		w2 = int32(binary.LittleEndian.Uint32(b[8:12]))
		w3 = int32(binary.LittleEndian.Uint32(b[12:16]))
		s = fmt.Sprintf(f, w0, w1, w2, w3)
	case "TRICE64_1":
		if 8 != len(b) {
			return "TRICE64_1", fmt.Errorf("false len %v", b)
		}
		l0 = int64(binary.LittleEndian.Uint64(b[0:8]))
		s = fmt.Sprintf(f, l0)
	default:
		return "ERR: INTERNAL ERROR!!!", errors.New("ERR: INTERNAL ERROR")
	}
	return s, err
}

func emitter(it id.Item, b []byte) (string, error) {
	return emitterGo(it, b)
}
