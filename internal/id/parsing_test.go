// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

import (
	"testing"

	"github.com/tj/assert"
)

var testSet = []struct {
	text, fmt string
}{
	{`..."a"b"...`, `"a"`},
	{`..."a\"b"...`, `"a\"b"`},
	{`..."a\\"b"...`, `"a\\"`},
	{`..."a\\\"b"...`, `"a\\\"b"`},
	{`..."a\"
	\"b\"\"\"c"...`, `"a\"
	\"b\"\"\"c"`},
	{`..."a\"\"b\"\"\"c"...`, `"a\"\"b\"\"\"c"`},
	{`..."a\"b\"c"...`, `"a\"b\"c"`},
	{`..."\"b\""...`, `"\"b\""`},
	{`..."a"...`, `"a"`},
	{`..."a
		b"...`, `"a
		b"`},
}

func TestFmatchStringLiteral(t *testing.T) {
	for _, s := range testSet {
		loc := matchStringLiteral(s.text)
		assert.Equal(t, s.fmt, s.text[loc[0]:loc[1]])
	}
}

func TestFcountColonsUntilClosingBracket(t *testing.T) {
	var testTable = []struct {
		text string
		exp  int
	}{
		{`,);`, 1},
		{`, a, b);`, 2},
		{`, "noise" );`, 1},
		{`, "noise, sound" );`, 1},
		{` "noise, sound" );`, 0},
		{`, a, b, c, )`, 4},
		{`, a, M(b, c), )`, 3},
	}
	for _, x := range testTable {
		act, err := countColonsUntilClosingBracket(x.text)
		assert.Nil(t, err)
		assert.EqualValues(t, x.exp, act)
	}
}

func TestFmatchBracketLiteral(t *testing.T) {
	var testTable = []struct {
		text string
		exp  int
	}{
		{`);`, 0},
		{`,);`, 1},
		{`, "noise" );`, 10},
		{`, "noise) sound" );`, 17},
		{`, M(a,b) );`, 9},
		{`, a, b, (c) )`, 12},
		{`, a, M(b, c), )`, 14},
	}
	for _, x := range testTable {
		act, err := matchBracketLiteral(x.text)
		assert.Nil(t, err)
		assert.EqualValues(t, x.exp, act)
	}
}
