// whitebox tests
package disp

import (
	"testing"
)

func Test_isLower(t *testing.T) {
	f := isLower("lower:")
	assertEqual(t, true, f)
	f = isLower("Lower:")
	assertEqual(t, false, f)
	f = isLower("uppeR:")
	assertEqual(t, false, f)
	f = isLower("0123456789:")
	assertEqual(t, true, f)
}

func TestDefault1_colorize(t *testing.T) {
	ColorPalette = "default"
	var s, c, e string

	s = "err:string\n"
	e = "\x1b[38;5;11;41mstring\n\x1b[0m"
	c = colorize(s)
	assertEqual(t, e, c)
	s = "ERR:string"
	e = "\x1b[38;5;11;41mERR:string\x1b[0m"
	c = colorize(s)
	assertEqual(t, e, c)
}

func TestNone_colorize(t *testing.T) {
	ColorPalette = "none"
	var s, c, e string

	s = "warning:string\n"
	e = "string\n"
	c = colorize(s)
	assertEqual(t, e, c)
	s = "MSG:string"
	e = "MSG:string"
	c = colorize(s)
	assertEqual(t, e, c)
}

func TestOff_colorize(t *testing.T) {
	ColorPalette = "off"
	var s, c, e string

	s = "dbg:string\n"
	e = "dbg:string\n"
	c = colorize(s)
	assertEqual(t, e, c)
	s = "D:string"
	e = "D:string"
	c = colorize(s)
	assertEqual(t, e, c)
}

func TestNoneNo_colorize(t *testing.T) {
	ColorPalette = "none"
	var s, c, e string

	s = "msg :string\n"
	e = "msg :string\n"
	c = colorize(s)
	assertEqual(t, e, c)
	s = "MSG string"
	e = "MSG string"
	c = colorize(s)
	assertEqual(t, e, c)
}

func TestDefault2_colorize(t *testing.T) {
	ColorPalette = "default"
	var s, c, e string

	s = "msg: string\n"
	e = "\x1b[92;40m string\n\x1b[0m"
	c = colorize(s)
	assertEqual(t, e, c)
	s = "attention:string"
	e = "\x1b[38;5;11;42mstring\x1b[0m"
	c = colorize(s)
	assertEqual(t, e, c)
}
