// SPDX-License-Identifier: MIT

package fmtspec

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

// TestNormalizeSupportedRemainingCases covers the supported C format variants that
// were not yet exercised by the decoder/id level regression tests.
func TestNormalizeSupportedRemainingCases(t *testing.T) {
	tt := []struct {
		name  string
		in    string
		out   string
		kinds []Kind
	}{
		{name: "short signed", in: "%hd", out: "%d", kinds: []Kind{KindSigned}},
		{name: "long signed i", in: "%li", out: "%d", kinds: []Kind{KindSigned}},
		{name: "char unsigned", in: "%hhu", out: "%u", kinds: []Kind{KindUnsigned}},
		{name: "intmax signed", in: "%jd", out: "%d", kinds: []Kind{KindSigned}},
		{name: "ptrdiff signed", in: "%td", out: "%d", kinds: []Kind{KindSigned}},
		{name: "long long unsigned", in: "%llu", out: "%u", kinds: []Kind{KindUnsigned}},
		{name: "long octal upper", in: "%#lo", out: "%#o", kinds: []Kind{KindBasedInteger}},
		{name: "long long uppercase octal", in: "%#llO", out: "%#O", kinds: []Kind{KindBasedInteger}},
		{name: "intmax unicode", in: "%jU", out: "%U", kinds: []Kind{KindBasedInteger}},
		{name: "long long quoted", in: "%llq", out: "%q", kinds: []Kind{KindBasedInteger}},
		{name: "ptrdiff binary", in: "%tb", out: "%b", kinds: []Kind{KindBasedInteger}},
		{name: "long float", in: "%0.3lf", out: "%0.3f", kinds: []Kind{KindFloat}},
		{name: "long double scientific", in: "%+08Le", out: "%+08e", kinds: []Kind{KindFloat}},
		{name: "plain bool", in: "%t", out: "%t", kinds: []Kind{KindBool}},
		{name: "plain pointer", in: "%p", out: "%p", kinds: []Kind{KindPointer}},
		{name: "plain string", in: "%12s", out: "%12s", kinds: []Kind{KindString}},
		{name: "plain char", in: "%c", out: "%c", kinds: []Kind{KindSigned}},
		{name: "mixed sequence", in: "a=%td b=%#llX c=%02hhu d=%t e=%s", out: "a=%d b=%#X c=%02u d=%t e=%s", kinds: []Kind{KindSigned, KindBasedInteger, KindUnsigned, KindBool, KindString}},
		{name: "escaped percent stays escaped", in: "%% %zu", out: "%% %u", kinds: []Kind{KindUnsigned}},
	}

	for _, tc := range tt {
		t.Run(tc.name, func(t *testing.T) {
			got, specs := Normalize(tc.in)
			assert.Equal(t, tc.out, got)
			assert.Len(t, specs, len(tc.kinds))
			for i, kind := range tc.kinds {
				assert.Equal(t, kind, specs[i].Kind)
			}
		})
	}
}

// TestNormalizeRejectsInvalidLengthModifierCombinations verifies that obviously
// invalid combinations are preserved as-is and not counted as supported Trice
// format specifiers.
func TestNormalizeRejectsInvalidLengthModifierCombinations(t *testing.T) {
	tt := []string{
		"%LX",
		"%Ls",
		"%lp",
		"%llf",
		"%zh",
	}
	for _, in := range tt {
		t.Run(in, func(t *testing.T) {
			got, specs := Normalize(in)
			assert.Equal(t, in, got)
			assert.Empty(t, specs)
		})
	}
}

// TestNormalizeReportsLocalFormatterFeatures verifies the metadata used by
// the generated target table. Keeping this in the shared parser prevents the
// generator from growing a second, subtly different printf grammar.
func TestNormalizeReportsLocalFormatterFeatures(t *testing.T) {
	_, specs := Normalize("plain=%d padded=%#08x precise=%.3f empty=%.s")
	assert.Len(t, specs, 4)
	assert.False(t, specs[0].HasFlags)
	assert.False(t, specs[0].HasFieldWidth)
	assert.False(t, specs[0].HasPrecision)
	assert.False(t, specs[0].HasAltForm)
	assert.True(t, specs[1].HasFlags)
	assert.Equal(t, "#0", specs[1].Flags)
	assert.True(t, specs[1].HasFieldWidth)
	assert.False(t, specs[1].HasPrecision)
	assert.True(t, specs[1].HasAltForm)
	assert.True(t, specs[2].HasPrecision)
	assert.False(t, specs[3].HasFieldWidth)
	assert.True(t, specs[3].HasPrecision)
}
