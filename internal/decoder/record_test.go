// SPDX-License-Identifier: MIT

package decoder

import (
	"testing"

	"github.com/rokath/trice/internal/fmtspec"
	"github.com/stretchr/testify/assert"
)

// TestStructuredScalarTypes checks semantic values before any display rounding,
// padding, alternate-base spelling, or signed text compatibility can intervene.
func TestStructuredScalarTypes(t *testing.T) {
	for _, tt := range []struct {
		name, format string
		bits         int
		raw          uint64
		want         any
		address      bool
	}{
		{"signed byte masks unused high bits", "%i", 8, 0x1ff, int64(-1), false},
		{"octal remains a number", "%#06o", 16, 64, uint64(64), false},
		{"capital octal remains a number", "%O", 16, 64, uint64(64), false},
		{"binary remains a number", "%b", 16, 7, uint64(7), false},
		{"hexadecimal remains a number", "%08X", 32, 0xffffffff, uint64(0xffffffff), false},
		{"character is a string", "%c", 32, 'ä', "ä", false},
		{"quoted character has no display quotes in its value", "%q", 8, 'A', "A", false},
		{"invalid unicode uses replacement character", "%c", 32, 0xd800, "�", false},
		{"oversized code point does not wrap", "%c", 64, 0x100000041, "�", false},
		{"zero boolean", "%t", 8, 0, false, false},
		{"nonzero boolean", "%t", 8, 2, true, false},
		{"null address", "%p", 64, 0, "0x0", true},
		{"full width address", "%p", 64, 0xffffffffffffffff, "0xffffffffffffffff", true},
	} {
		t.Run(tt.name, func(t *testing.T) {
			template, err := fmtspec.ParseTemplate("{value:"+tt.format+"}", nil)
			if !assert.NoError(t, err) {
				return
			}
			got, err := ScalarField(template.Fields[0], tt.bits, tt.raw)
			assert.NoError(t, err)
			assert.Equal(t, FieldValue{Name: "value", Value: tt.want, Address: tt.address}, got)
		})
	}
}

// TestStructuredScalarRejectsInvalidTypes prevents caller mistakes from being
// represented as seemingly valid values in a machine record.
func TestStructuredScalarRejectsInvalidTypes(t *testing.T) {
	for _, tt := range []struct {
		name, format, errorText string
		bits                    int
	}{
		{"unsupported width", "%d", "unsupported bit width", 24},
		{"float cannot use sixteen bits", "%f", "requires 32 or 64 bits", 16},
		{"string is not scalar bits", "%s", "not a numeric scalar", 32},
	} {
		t.Run(tt.name, func(t *testing.T) {
			template, err := fmtspec.ParseTemplate("{value:"+tt.format+"}", nil)
			if !assert.NoError(t, err) {
				return
			}
			_, err = ScalarField(template.Fields[0], tt.bits, 0)
			assert.ErrorContains(t, err, tt.errorText)
		})
	}
}
