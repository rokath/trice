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

//  // matchFormatString0 returns a two-element slice of integers defining the location of the leftmost match in s of the matchFmtString regular expression.
//  // The match itself is at s[loc[0]:loc[1]]. A return value of nil indicates no match.
//  // If the format string contains `\"` elements, the found sub strings are concatenated to the returned result.
//  func matchFormatString0(s string) (loc []int) {
//  	ss := s
//  	for {
//  		fmtLoc := matchFmtString.FindStringIndex(ss)
//  		if fmtLoc == nil {
//  			return loc // done
//  		}
//  		if loc == nil {
//  			loc = fmtLoc // Keep start position and current ent position.
//  		}
//  		fs := ss[fmtLoc[0]:fmtLoc[1]]       // fs is the found string.
//  		if ss[len(fs)-2:len(fs)-1] != `\` { // Check for "a\"b" cases
//  			return loc // done
//  		}
//  		// s ends with `\"`, so we need to continue parsing
//  		ss = ss[fmtLoc[1]:]             // cut off ss start for next parsing round
//  		loc[1] += fmtLoc[1] - fmtLoc[0] // set new value for loc[1]
//  	}
//  }

//  func TestMatchFormatString0(t *testing.T) {
//  	for _, s := range testSet {
//  		loc := matchFormatString0(s.text)
//  		assert.Equal(t, s.fmt, s.text[loc[0]:loc[1]])
//  	}
//  }
