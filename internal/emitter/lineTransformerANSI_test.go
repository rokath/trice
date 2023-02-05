// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// white-box test for package emitter.
package emitter

import (
	"fmt"
	"strings"
	"testing"

	"github.com/stretchr/testify/assert"
)

func Test1colorize(t *testing.T) {
	lw := newCheckDisplay()
	p := newLineTransformerANSI(lw, "none")
	s := "abc:de"
	c := p.colorize(s)
	if c != s {
		t.Fail()
	}
}

func Test2colorize(t *testing.T) {
	lw := newCheckDisplay()
	p := newLineTransformerANSI(lw, "none")
	s := "msg:de"
	c := p.colorize(s)
	assert.Equal(t, "de", c)
}

func Test3colorize(t *testing.T) {
	lw := newCheckDisplay()
	p := newLineTransformerANSI(lw, "off")
	s := "msg:de"
	assert.Equal(t, s, p.colorize(s))
}

func _Test4colorize(t *testing.T) {
	lw := newCheckDisplay()
	p := newLineTransformerANSI(lw, "default")
	s := "msg:de"
	c := p.colorize(s)
	act := []byte(c)
	exp := []byte{27, 91, 57, 50, 59, 52, 48, 109, 100, 101, 27, 91, 48, 109}
	fmt.Println("exp:", string(exp))
	fmt.Println("act:", string(act))
	assert.Equal(t, exp, act)
}

func _Test5colorize(t *testing.T) {
	lw := newCheckDisplay()
	p := newLineTransformerANSI(lw, "default")
	s := "MESSAGE:de"
	c := p.colorize(s)
	b := []byte{27, 91, 57, 50, 59, 52, 48, 109, 77, 69, 83, 83, 65, 71, 69, 58, 100, 101, 27, 91, 48, 109}
	assert.Equal(t, b, []byte(c))
}

func Test1WriteLine(t *testing.T) {
	lw := newCheckDisplay()
	p := newLineTransformerANSI(lw, "none")
	q := newLineTransformerANSI(lw, "off")
	l := []string{"M:msg", "I:Info", "wrn:End"}
	p.WriteLine(l)
	q.WriteLine(l)
	ep := strings.Join([]string{"M:msg", "I:Info", "End"}, "")
	eq := strings.Join([]string{"M:msg", "I:Info", "wrn:End"}, "")
	assert.Equal(t, []string{ep, eq}, lw.lines)
}
