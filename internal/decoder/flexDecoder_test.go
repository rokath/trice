// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package decoder

import (
	"testing"

	"github.com/tj/assert"
)

type u1Table struct {
	fmtStrg string
	value   uint32
	expStrg string
}

var uTable1 = []u1Table{
	{"%u", 200, "200"},
	{"%d", 127, "127"},
	{"%d", 128, "-128"},
	{"%u", 128, "128"},
	{"%u", 255, "255"},
	{"%d", 255, "-1"},
	{"%05u", 200, "00200"},
	{"aaa%5u", 200, "aaa  200"},
	{"aaa%%7u.%u", 200, "aaa%7u.200"},
	{"%%%%7u.%u", 200, "%%7u.200"},
}

func Test_trice81(t *testing.T) {
	p := &Flex{}
	p.b = make([]byte, defaultSize)
	p.iBuf = make([]byte, defaultSize)
	for _, v := range uTable1 {
		p.b = p.b[:defaultSize]
		p.trice.Strg = v.fmtStrg
		p.d0 = v.value
		exp := v.expStrg
		n, e := p.trice81()
		assert.Nil(t, e)
		p.b = p.b[:n]
		act := string(p.b)
		assert.Equal(t, exp, act)
	}
}

type u2Table struct {
	fmtStrg string
	v0, v1  uint32
	expStrg string
}

var uTable2 = []u2Table{
	{"%c%c", 'a', 'b', "ab"},
	{"%u%u", 200, 200, "200200"},
	{"%d%u", 127, 128, "127128"},
	{"%%u.%%x%u%d", 128, 128, "%u.%x128-128"}, // to do: need to fix: when x == % error
	//{"%%%u%d", 128, 128, "%128-128"},          // to do: need to fix: when more than 2 %% come together -> error
}

func Test_trice82(t *testing.T) {
	p := &Flex{}
	p.b = make([]byte, defaultSize)
	p.iBuf = make([]byte, defaultSize)
	for _, v := range uTable2 {
		p.b = p.b[:defaultSize]
		p.trice.Strg = v.fmtStrg
		p.d0 = (v.v0 << 8) | v.v1
		exp := v.expStrg
		n, e := p.trice82()
		assert.Nil(t, e)
		p.b = p.b[:n]
		act := string(p.b)
		assert.Equal(t, exp, act)
	}
}

type uNTable struct {
	fmtStrg string
	v       []uint32
	expStrg string
}

var uTable4 = []uNTable{
	{"%c%c%c%c", []uint32{'a', 'b', 'c', 'd'}, "abcd"},
	{"%d%u%d%u", []uint32{127, 128, 127, 128}, "127128127128"},
}

func Test_trice84(t *testing.T) {
	p := &Flex{}
	p.b = make([]byte, defaultSize)
	p.iBuf = make([]byte, defaultSize)
	for _, v := range uTable4 {
		p.b = p.b[:defaultSize]
		p.trice.Strg = v.fmtStrg
		p.d0 = (v.v[0] << 24) | (v.v[1] << 16) | (v.v[2] << 8) | v.v[3]
		exp := v.expStrg
		n, e := p.trice84()
		assert.Nil(t, e)
		p.b = p.b[:n]
		act := string(p.b)
		assert.Equal(t, exp, act)
	}
}

func TestFlexL(t *testing.T) {
	var table = testTable{ // little endian
		{[]byte{0, 192, 248, 135}, `--------------------------------------------------`},
		{[]byte{1, 192, 248, 135}, `--------------------------------------------------`},
	}
	doTableTest(t, NewFlexDecoder, littleEndian, table, "unwrapped")
}

func TestFlex(t *testing.T) {
	var table = testTable{ // little endian

	}
	doTableTest(t, NewFlexDecoder, bigEndian, table, "unwrapped")
}
