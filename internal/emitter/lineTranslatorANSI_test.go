// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// whitebox test for package emitter.
package emitter

import (
	"fmt"
	"reflect"
	"strings"
	"testing"

	"github.com/mgutz/ansi"
)

func Test1colorize(t *testing.T) {
	lw := newCheckDisplay()
	p := newLineTranslatorANSI(lw, "none")
	s := "abc:de"
	c := p.colorize(s)
	if c != s {
		t.Fail()
	}
}

func Test2colorize(t *testing.T) {
	lw := newCheckDisplay()
	p := newLineTranslatorANSI(lw, "none")
	s := "msg:de"
	c := p.colorize(s)
	if "de" != c {
		t.Fail()
	}
}

func Test3colorize(t *testing.T) {
	lw := newCheckDisplay()
	p := newLineTranslatorANSI(lw, "off")
	s := "msg:de"
	c := p.colorize(s)
	if s != c {
		fmt.Println(s, c)
		t.Fail()
	}
}

func Test4colorize(t *testing.T) {
	lw := newCheckDisplay()
	p := newLineTranslatorANSI(lw, "default")
	s := "msg:de"
	c := p.colorize(s)
	b := []byte{27, 91, 57, 50, 59, 52, 48, 109, 100, 101, 27, 91, 48, 109}
	if false == reflect.DeepEqual(b, []byte(c)) {
		t.Fail()
	}
}

func Test5colorize(t *testing.T) {
	lw := newCheckDisplay()
	p := newLineTranslatorANSI(lw, "default")
	s := "MESSAGE:de"
	c := p.colorize(s)
	b := []byte{27, 91, 57, 50, 59, 52, 48, 109, 77, 69, 83, 83, 65, 71, 69, 58, 100, 101, 27, 91, 48, 109}
	if false == reflect.DeepEqual(b, []byte(c)) {
		fmt.Println([]byte(c))
		t.Fail()
	}
}

func Test1writeLine(t *testing.T) {
	lw := newCheckDisplay()
	p := newLineTranslatorANSI(lw, "none")
	q := newLineTranslatorANSI(lw, "off")
	l := []string{"M:msg", "I:info", "wrn:end"}
	p.writeLine(l)
	q.writeLine(l)
	ep := strings.Join([]string{"M:msg", "I:info", "end", ansi.Reset}, "")
	eq := strings.Join([]string{"M:msg", "I:info", "wrn:end", ansi.Reset}, "")
	lw.checkLines(t, []string{ep, eq})
}
