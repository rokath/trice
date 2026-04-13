// SPDX-License-Identifier: MIT

package id

import (
	"bytes"
	"errors"
	"testing"

	"github.com/rokath/trice/pkg/ant"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
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

// TestFmatchStringLiteral verifies the expected behavior.
func TestFmatchStringLiteral(t *testing.T) {
	for _, s := range testSet {
		loc := matchStringLiteral(s.text)
		assert.Equal(t, s.fmt, s.text[loc[0]:loc[1]])
	}
}

// TestFcountColonsUntilClosingBracket verifies the expected behavior.
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

// TestFmatchBracketLiteral verifies the expected behavior.
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

// TestEvaluateTriceParameterCount verifies the expected behavior.
func TestEvaluateTriceParameterCount(t *testing.T) {
	tt := []struct {
		name string
		tf   TriceFmt
		rest string
		ok   bool
	}{
		{name: "regular ok", tf: TriceFmt{Type: "TRICE", Strg: "%d"}, rest: `, value);`, ok: true},
		{name: "regular mismatch", tf: TriceFmt{Type: "TRICE", Strg: "%d%d"}, rest: `, value);`, ok: false},
		{name: "string ok", tf: TriceFmt{Type: "TRICES", Strg: "%s"}, rest: `, "abc");`, ok: true},
		{name: "buffer ok", tf: TriceFmt{Type: "TRICE8B", Strg: "%d"}, rest: `, buf, sizeof(buf));`, ok: true},
		{name: "function ok", tf: TriceFmt{Type: "TRICE16F", Strg: "rpc:Call"}, rest: `, buf, sizeof(buf));`, ok: true},
		{name: "assert ok", tf: TriceFmt{Type: "TRICEAssertTrue", Strg: ""}, rest: `, value);`, ok: true},
		{name: "assert mismatch", tf: TriceFmt{Type: "TRICEAssertTrue", Strg: "%d"}, rest: `, value);`, ok: false},
	}

	for _, tc := range tt {
		t.Run(tc.name, func(t *testing.T) {
			err := evaluateTriceParameterCount(tc.tf, 7, tc.rest)
			if tc.ok {
				assert.NoError(t, err)
			} else {
				assert.Error(t, err)
			}
		})
	}
}

// TestHelperUtilities verifies the expected behavior.
func TestHelperUtilities(t *testing.T) {
	defer Setup(t)()

	require.NoError(t, FSys.WriteFile(SFName, []byte("demo"), 0o777))
	assert.True(t, fileExists(FSys, SFName))
	assert.False(t, fileExists(FSys, Proj+"missing.c"))

	p := &idData{}
	func() {
		p.join(errors.New("boom"))
	}()
	require.Error(t, p.err)
	assert.Contains(t, p.err.Error(), "boom")
	assert.Contains(t, p.err.Error(), "TestHelperUtilities")
}

// TestWriteAndCleanIDHelpers verifies the low-level ID rewrite helpers directly.
func TestWriteAndCleanIDHelpers(t *testing.T) {
	defer Setup(t)()

	input := `TRICE("msg");`
	loc := matchTrice(input)
	require.NotNil(t, loc)

	oldSpace := SpaceInsideParenthesis
	oldStamp := DefaultStampSize
	t.Cleanup(func() {
		SpaceInsideParenthesis = oldSpace
		DefaultStampSize = oldStamp
	})

	DefaultStampSize = 16
	SpaceInsideParenthesis = false
	got, delta := writeID(input, 0, loc, TriceFmt{Type: "TRICE", Strg: "msg"}, 77)
	assert.Equal(t, `TRICE(Id(77), "msg");`, got)
	assert.NotZero(t, delta)

	got, delta = cleanID(got, 0, matchTrice(got), TriceFmt{Type: "TRICE", Strg: "msg"})
	assert.Equal(t, `TRICE(Id(0), "msg");`, got)
	assert.NotZero(t, delta)

	lower := `trice(iD(15), "msg");`
	SpaceInsideParenthesis = true
	got, delta = cleanID(lower, 0, matchTrice(lower), TriceFmt{Type: "trice", Strg: "msg"})
	assert.Equal(t, `trice( "msg");`, got)
	assert.NotZero(t, delta)
}

// TestRemoveIDFromSlice verifies both the removal and unchanged branches.
func TestRemoveIDFromSlice(t *testing.T) {
	ids := []TriceID{11, 22, 33}
	removed := removeIDFromSlice(append([]TriceID(nil), ids...), 22)
	assert.ElementsMatch(t, []TriceID{11, 33}, removed)

	unchanged := removeIDFromSlice(append([]TriceID(nil), ids...), 99)
	assert.Equal(t, ids, unchanged)
}

// TestInsertAndProcessEarlyErrorReturns keeps the explicit short-circuit
// branches covered so later refactors do not accidentally drop them.
func TestInsertAndProcessEarlyErrorReturns(t *testing.T) {
	defer Setup(t)()

	expected := errors.New("stop early")
	p := &idData{err: expected}

	out, modified, err := p.insertTriceIDs(&bytes.Buffer{}, "demo.c", []byte(`TRICE("x");`), &ant.Admin{})
	require.NoError(t, err)
	assert.Nil(t, out)
	assert.False(t, modified)
	require.ErrorIs(t, p.err, expected)

	info, statErr := FSys.Stat(FnJSON)
	require.NoError(t, statErr)
	require.ErrorIs(t, p.processTriceIDInsertion(&bytes.Buffer{}, FSys, FnJSON, info, &ant.Admin{}), expected)
	require.ErrorIs(t, p.processTriceIDCleaning(&bytes.Buffer{}, FSys, FnJSON, info, &ant.Admin{}), expected)
}
