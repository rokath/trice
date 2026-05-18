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

// TestIssue563EvaluateFormatAndCountMisuse covers objective macro misuse cases
// from issue #563 that do not require C argument type analysis.
func TestIssue563EvaluateFormatAndCountMisuse(t *testing.T) {
	tests := []struct {
		name       string
		tf         TriceFmt
		rest       string
		wantErrSub string
	}{
		{
			name:       "string specifier is invalid in regular macro",
			tf:         TriceFmt{Type: "TRICE", Strg: "msg=%s"},
			rest:       `, text);`,
			wantErrSub: "string format specifier outside triceS/triceN",
		},
		{
			name:       "width string specifier is invalid in regular macro",
			tf:         TriceFmt{Type: "TRICE8_1", Strg: "msg=%8s"},
			rest:       `, text);`,
			wantErrSub: "string format specifier outside triceS/triceN",
		},
		{
			name:       "string macro requires string specifier",
			tf:         TriceFmt{Type: "triceS", Strg: "value=%d"},
			rest:       `, value);`,
			wantErrSub: "should use a string format specifier",
		},
		{
			name:       "counted string macro requires string specifier",
			tf:         TriceFmt{Type: "TRICE_N", Strg: "value=%u"},
			rest:       `, value, length);`,
			wantErrSub: "should use a string format specifier",
		},
		{
			name:       "string macro rejects additional specifiers",
			tf:         TriceFmt{Type: "TRICES", Strg: "msg=%s value=%d"},
			rest:       `, text, value);`,
			wantErrSub: "exactly one format specifier",
		},
		{
			name:       "string macro rejects two string specifiers",
			tf:         TriceFmt{Type: "triceS", Strg: "%s %s"},
			rest:       `, first, second);`,
			wantErrSub: "exactly one format specifier",
		},
		{
			name:       "zero macro rejects format specifier",
			tf:         TriceFmt{Type: "TRICE_0", Strg: "value=%d"},
			rest:       `, value);`,
			wantErrSub: "does not match format specifier count",
		},
		{
			name:       "compact zero macro rejects format specifier",
			tf:         TriceFmt{Type: "TRICE0", Strg: "value=%d"},
			rest:       `, value);`,
			wantErrSub: "does not match format specifier count",
		},
		{
			name:       "explicit suffix must match format specifier count",
			tf:         TriceFmt{Type: "TRICE32_2", Strg: "value=%d"},
			rest:       `, value);`,
			wantErrSub: "does not match format specifier count",
		},
		{
			name:       "regular macro rejects more than twelve format specifiers",
			tf:         TriceFmt{Type: "TRICE", Strg: strings.Repeat("%d", 13)},
			rest:       `, a, b, c, d, e, f, g, h, i, j, k, l, m);`,
			wantErrSub: "more than 12",
		},
		{
			name:       "buffer macro rejects string specifier",
			tf:         TriceFmt{Type: "TRICE8_B", Strg: "%s"},
			rest:       `, buf, sizeof(buf));`,
			wantErrSub: "string format specifier outside triceS/triceN",
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			err := evaluateTriceParameterCount(tt.tf, 23, tt.rest)
			require.Error(t, err)
			assert.Contains(t, err.Error(), tt.wantErrSub)
		})
	}
}

// TestIssue563EvaluateFormatAndCountValidCases keeps the stricter issue #563
// checks from rejecting valid macro families in the first validation stage.
func TestIssue563EvaluateFormatAndCountValidCases(t *testing.T) {
	tests := []struct {
		name string
		tf   TriceFmt
		rest string
	}{
		{name: "regular scalar macro", tf: TriceFmt{Type: "TRICE32_2", Strg: "a=%d b=%u"}, rest: `, a, b);`},
		{name: "zero macro without parameters", tf: TriceFmt{Type: "TRICE_0", Strg: "plain"}, rest: `);`},
		{name: "string macro", tf: TriceFmt{Type: "triceS", Strg: "msg=%12s"}, rest: `, text);`},
		{name: "quoted string macro", tf: TriceFmt{Type: "triceS", Strg: "msg=%q"}, rest: `, text);`},
		{name: "hex string macro", tf: TriceFmt{Type: "triceS", Strg: "msg=% x"}, rest: `, text);`},
		{name: "counted string macro", tf: TriceFmt{Type: "TRICE_N", Strg: "msg=%s"}, rest: `, text, length);`},
		{name: "buffer macro", tf: TriceFmt{Type: "TRICE8_B", Strg: "%02x"}, rest: `, buf, sizeof(buf));`},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			assert.NoError(t, evaluateTriceParameterCount(tt.tf, 23, tt.rest))
		})
	}
}

// TestIssue563AddFmtCountValidation verifies the same format/count checks for
// existing lookup entries, where only the format string is available.
func TestIssue563AddFmtCountValidation(t *testing.T) {
	ilu := TriceIDLookUp{
		1: {Type: "TRICE_S", Strg: "value=%d"},
		2: {Type: "TRICE_N", Strg: "value=%u"},
		3: {Type: "TRICE_B", Strg: "value=%s"},
		4: {Type: "TRICE_2", Strg: "value=%d"},
		5: {Type: "TRICE0", Strg: "value=%d"},
		6: {Type: "TRICE_S", Strg: SAliasStrgPrefix + `false, "msg=%d"` + SAliasStrgSuffix},
		7: {Type: "TriceAssertTrue", Strg: "ASSERT:flag not true!\\n"},
	}

	var out bytes.Buffer
	ilu.AddFmtCount(&out)

	text := out.String()
	assert.Contains(t, text, "should use a string format specifier")
	assert.Contains(t, text, "string format specifier outside triceS/triceN")
	assert.Contains(t, text, "does not match format specifier count")
	assert.NotContains(t, text, "TriceAssertTrue")
	assert.Equal(t, "TRICE_S", ilu[6].Type)
}

func TestIssue563AliasInsertUsesRegularFormatChecks(t *testing.T) {
	defer Setup(t)()

	TriceAliases = ArrayFlag{"LOG"}
	ProcessAliases()

	var w bytes.Buffer
	p := &idData{}
	out, modified, err := p.insertTriceIDs(&w, "demo.c", "demo.c", []byte(`LOG("bad=%s", text);`), &ant.Admin{})

	require.Error(t, err)
	assert.Nil(t, out)
	assert.False(t, modified)
	assert.Contains(t, w.String(), "string format specifier outside triceS/triceN")
}
