// SPDX-License-Identifier: MIT

package trexDecoder

import (
	"testing"

	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/id"
	"github.com/stretchr/testify/assert"
)

// TestUnsignedOrSignedOutIntegerExtremes protects the existing %u -> %d normalization
// path by verifying that unsigned values keep their magnitude while signed values are
// converted to the matching Go integer type, even at the edges of each bit width.
func TestUnsignedOrSignedOutIntegerExtremes(t *testing.T) {
	tt := []struct {
		name      string
		triceType string
		strg      string
		pFmt      string
		u         []int
		raw       []byte
		bitWidth  int
		count     int
		exp       string
	}{
		{
			name:      "8-bit extremes",
			triceType: "TRICE8_2",
			strg:      "%u %d",
			pFmt:      "%d %d",
			u:         []int{decoder.UnsignedFormatSpecifier, decoder.SignedFormatSpecifier},
			raw:       []byte{0xff, 0x80},
			bitWidth:  8,
			count:     2,
			exp:       "255 -128",
		},
		{
			name:      "16-bit extremes",
			triceType: "TRICE16_2",
			strg:      "%u %d",
			pFmt:      "%d %d",
			u:         []int{decoder.UnsignedFormatSpecifier, decoder.SignedFormatSpecifier},
			raw:       []byte{0xff, 0xff, 0x00, 0x80},
			bitWidth:  16,
			count:     2,
			exp:       "65535 -32768",
		},
		{
			name:      "32-bit extremes",
			triceType: "TRICE32_2",
			strg:      "%u %d",
			pFmt:      "%d %d",
			u:         []int{decoder.UnsignedFormatSpecifier, decoder.SignedFormatSpecifier},
			raw:       []byte{0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x80},
			bitWidth:  32,
			count:     2,
			exp:       "4294967295 -2147483648",
		},
		{
			name:      "64-bit extremes",
			triceType: "TRICE64_2",
			strg:      "%u %d",
			pFmt:      "%d %d",
			u:         []int{decoder.UnsignedFormatSpecifier, decoder.SignedFormatSpecifier},
			raw:       []byte{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80},
			bitWidth:  64,
			count:     2,
			exp:       "18446744073709551615 -9223372036854775808",
		},
	}

	for _, tc := range tt {
		t.Run(tc.name, func(t *testing.T) {
			p := &trexDec{DecoderData: decoder.NewDecoderData(decoder.Config{Endian: decoder.LittleEndian})}
			b := make([]byte, 256)

			p.Trice.Type = tc.triceType
			p.Trice.Strg = tc.strg
			p.pFmt = tc.pFmt
			p.u = tc.u
			p.B = tc.raw

			n := p.unSignedOrSignedOut(b, tc.bitWidth, tc.count)
			assert.Equal(t, tc.exp, string(b[:n]))
		})
	}
}

// TestSprintTriceLengthModifiers verifies that valid C length modifiers are accepted
// end-to-end and normalized for Go formatting during decoding.
func TestSprintTriceLengthModifiers(t *testing.T) {
	oldUnsigned := decoder.Unsigned
	decoder.Unsigned = true
	t.Cleanup(func() { decoder.Unsigned = oldUnsigned })

	tt := []struct {
		name       string
		trice      id.TriceFmt
		payload    []byte
		paramSpace int
		exp        string
	}{
		{
			name:       "size_t unsigned decimal",
			trice:      id.TriceFmt{Type: "TRICE", Strg: "count=%zu"},
			payload:    []byte{0x2a, 0x00, 0x00, 0x00},
			paramSpace: 4,
			exp:        "count=42",
		},
		{
			name:       "unsigned long with width",
			trice:      id.TriceFmt{Type: "TRICE", Strg: "count=%8lu"},
			payload:    []byte{0x2a, 0x00, 0x00, 0x00},
			paramSpace: 4,
			exp:        "count=      42",
		},
		{
			name:       "long hex uppercase",
			trice:      id.TriceFmt{Type: "TRICE", Strg: "mask=%08lX"},
			payload:    []byte{0xef, 0xbe, 0xad, 0xde},
			paramSpace: 4,
			exp:        "mask=DEADBEEF",
		},
		{
			name:       "long long hex uppercase",
			trice:      id.TriceFmt{Type: "TRICE64", Strg: "mask=%016llX"},
			payload:    []byte{0xef, 0xcd, 0xab, 0x89, 0x67, 0x45, 0x23, 0x01},
			paramSpace: 8,
			exp:        "mask=0123456789ABCDEF",
		},
	}

	for _, tc := range tt {
		t.Run(tc.name, func(t *testing.T) {
			p := &trexDec{DecoderData: decoder.NewDecoderData(decoder.Config{Endian: decoder.LittleEndian})}
			b := make([]byte, 256)

			p.Trice = tc.trice
			p.B = tc.payload
			p.ParamSpace = tc.paramSpace

			n := p.sprintTrice(b)
			assert.Equal(t, tc.exp, string(b[:n]))
		})
	}
}
