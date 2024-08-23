// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

import (
	"testing"

	"github.com/tj/assert"
)

func TestMatchTrice(t *testing.T) {
	var testSet = []struct {
		text, triceType, triceID, triceFmts string
	}{
		{`...TRice( "%d+%3d=\n%u", 
		(3), 4, 
		(3+4) ); ,...`, `TRice`, ``, `"%d+%3d=\n%u"`},
		{`...TRice( "%d+%3d=%u", 
		(3), 4, (3+4) ); ,...`, `TRice`, ``, `"%d+%3d=%u"`},
		{`...TRice( "%d", 
		(3+4) ); ,...`, `TRice`, ``, `"%d"`},
		{`...TRice( "%d", (3+4) ); ,...`, `TRice`, ``, `"%d"`},
		{`...TRice( "a" ); ,...`, `TRice`, ``, `"a"`},
		{`......TRice( iD(0), "a" ); ,...`, `TRice`, `iD(0)`, `"a"`}, // 6 11 12 13 18 20 23
		{`."x"..TRice( iD(0), "a" ); ,...`, `TRice`, `iD(0)`, `"a"`}, // 6 11 12  0  0 20 23
		{`... ttt ... TRice( "a" ); ,...`, `TRice`, ``, `"a"`},       // 12 17 18 0 0 19 22
		{`... "t" ... TRice(        "a" ); ,...`, `TRice`, ``, `"a"`},
		{`... "a" ... TRice( iD(99),"a" ); ,...`, `TRice`, `iD(99)`, `"a"`},
		{`. "a" "b".. TRice( iD(99),"a" ); ,...`, `TRice`, `iD(99)`, `"a"`},
		{`...TRice( "a\"b" ); ,...`, `TRice`, ``, `"a\"b"`},
		{`...// "trice" ... "aa" ... TRice(        "a" ); ,...`, `TRice`, ``, `"a"`},
		{`...// "trice" ... "aa" ... TRice( iD(7), "a" ); ,...`, `TRice`, `iD(7)`, `"a"`},
		{`...// "trice" ... "aa" ... TRice( iD( 77 ), "a" ); ,...`, `TRice`, `iD( 77 )`, `"a"`},
		{`...a "trice(" .b. "aa" ... TRice( "a" ); ,...`, `TRice`, ``, `"a"`},
		{`...a "trice( id(8() \"hi\" )" .b. "aa" ... TRice( "a" ); ,...`, `TRice`, ``, `"a"`},
		{`...a "trice( id(8() "hi" )" .b. "aa" ... TRice( "a" ); ,...`, `TRice`, ``, `"a"`},
		{`... "a" ... TRice( iD(99),"a
		b" ); ,...`, `TRice`, `iD(99)`, `"a
		b"`},
		{`... ttt ... TRice( "a", "b" ); ,...`, `TRice`, ``, `"a"`}, // 12 17 18 0 0 19 22
		{`... ttt ... TRice( 
			"a", "b" ); ,...`, `TRice`, ``, `"a"`}, // 12 17 18 0 0 19 22
		{`... ttt ... TRice
			( "a", "b" ); ,...`, `TRice`, ``, `"a"`}, // 12 17 18 0 0 19 22
		{`... ttt ... TRice
			( iD(5), "a", "b" ); ,...`, `TRice`, `iD(5)`, `"a"`}, // 12 17 18 0 0 19 22
	}
	for _, s := range testSet {
		loc := matchTrice(s.text)
		assert.Equal(t, s.triceType, s.text[loc[0]:loc[1]])
		assert.Equal(t, s.triceID, s.text[loc[3]:loc[4]])
		assert.Equal(t, s.triceFmts, s.text[loc[5]:loc[6]])
	}
}
