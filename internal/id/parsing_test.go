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

func TestMatchFormatString(t *testing.T) {
	for _, s := range testSet {
		loc := matchFormatString(s.text)
		assert.Equal(t, s.fmt, s.text[loc[0]:loc[1]])
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
