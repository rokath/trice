// SPDX-License-Identifier: MIT

package id

import (
	"bytes"
	"strings"
	"testing"

	"github.com/rokath/trice/pkg/ant"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestIssue632InsertErrorReportsFileLineAndContext(t *testing.T) {
	defer Setup(t)()

	source := strings.Join([]string{
		"void demo(void) {",
		"    int value = 1;",
		"    TRICE(\"value=%d %d\",",
		"          value);",
		"    int apparentContinuation = 0;",
		"    int sameContext = 0;",
		"    int stillContext = 1;",
		"    int lastContext = 2;",
		"    int finalContext = 3;",
		"    int outsideContext = 3;",
		"}",
	}, "\n")

	var w bytes.Buffer
	p := &idData{}
	out, modified, err := p.insertTriceIDs(&w, "project/source/big_file.c", "big_file.c", []byte(source), &ant.Admin{})

	require.Error(t, err)
	assert.Nil(t, out)
	assert.False(t, modified)

	text := w.String()
	assert.Contains(t, text, "project/source/big_file.c:3: line 3 format specifier count 2 != parameter count 1")
	assert.Contains(t, text, "project/source/big_file.c:3-8:")
	assert.Contains(t, text, `3 |     TRICE("value=%d %d",`)
	assert.Contains(t, text, "4 |           value);")
	assert.Contains(t, text, "8 |     int lastContext = 2;")
	assert.NotContains(t, text, "outsideContext")
}

func TestIssue632ParserErrorDiagnosticDoesNotDumpRemainingFile(t *testing.T) {
	source := strings.Join([]string{
		"void demo(void) {",
		"    int value = 1;",
		"    TRICE(\"value=%d\",",
		"          call(value);",
		"    int sameContext = 0;",
		"    int stillContext = 1;",
		"    int lastContext = 2;",
		"    int finalContext = 3;",
		"    int outsideContext = 3;",
		"}",
	}, "\n")

	err := evaluateTriceParameterCount(TriceFmt{Type: "TRICE", Strg: "%d"}, 3, `, call(value);`)
	require.Error(t, err)

	text := formatTriceInsertParseError("project/source/big_file.c", source, 3, 3, err)

	assert.Contains(t, text, "project/source/big_file.c:3: malformed Trice parameter list after format string: no matching closing bracket found")
	assert.Contains(t, text, "project/source/big_file.c:3-8:")
	assert.Contains(t, text, `3 |     TRICE("value=%d",`)
	assert.Contains(t, text, "8 |     int finalContext = 3;")
	assert.NotContains(t, text, "outsideContext")
}

func TestIssue632SourceLineContext(t *testing.T) {
	longLine := strings.Repeat("a", triceDiagnosticMaxLineRunes+10)
	tests := []struct {
		name          string
		source        string
		startLine     int
		maxLines      int
		wantFrom      int
		wantTo        int
		wantContains  []string
		wantNotInText []string
	}{
		{
			name:         "multi-line range",
			source:       "one\ntwo\nthree\nfour",
			startLine:    2,
			maxLines:     2,
			wantFrom:     2,
			wantTo:       3,
			wantContains: []string{"2 | two", "3 | three"},
		},
		{
			name:         "line beyond file clamps to last line",
			source:       "one\ntwo\nthree",
			startLine:    20,
			maxLines:     3,
			wantFrom:     3,
			wantTo:       3,
			wantContains: []string{"3 | three"},
		},
		{
			name:          "long line is shortened",
			source:        longLine + "\nnext",
			startLine:     1,
			maxLines:      1,
			wantFrom:      1,
			wantTo:        1,
			wantContains:  []string{strings.Repeat("a", triceDiagnosticMaxLineRunes) + " ..."},
			wantNotInText: []string{strings.Repeat("a", triceDiagnosticMaxLineRunes+1)},
		},
	}

	for _, tc := range tests {
		t.Run(tc.name, func(t *testing.T) {
			from, to, context := sourceLineContext(tc.source, tc.startLine, tc.maxLines)

			assert.Equal(t, tc.wantFrom, from)
			assert.Equal(t, tc.wantTo, to)
			for _, want := range tc.wantContains {
				assert.Contains(t, context, want)
			}
			for _, unwanted := range tc.wantNotInText {
				assert.NotContains(t, context, unwanted)
			}
		})
	}
}
