// Package tcobs_test performs TCOBS whitebox tests.
package tcobs_test

import (
	"fmt"
	"math/rand"
	"testing"

	"github.com/rokath/trice/pkg/tcobs"
	"github.com/tj/assert"
)

type testTable []struct {
	dec []byte
	enc []byte
}

var testData = testTable{

	{[]byte{}, []byte{}},

	{[]byte{0}, []byte{0x20}},
	{[]byte{0, 0xAA}, []byte{0x20, 0xAA, 0xA1}},
	{[]byte{0, 0xFF}, []byte{0x20, 0xFF, 0xA1}},

	{[]byte{0, 0}, []byte{0x40}},
	{[]byte{0, 0, 0xAA}, []byte{0x40, 0xAA, 0xA1}},
	{[]byte{0, 0, 0xFF}, []byte{0x40, 0xFF, 0xA1}},

	{[]byte{0, 0, 0}, []byte{0x60}},
	{[]byte{0, 0, 0, 0xAA}, []byte{0x60, 0xAA, 0xA1}},
	{[]byte{0, 0, 0, 0xFF}, []byte{0x60, 0xFF, 0xA1}},

	{[]byte{0, 0, 0, 0}, []byte{0x60, 0x20}},

	{[]byte{0, 0, 0, 0, 0}, []byte{0x60, 0x40}},
	{[]byte{0, 0, 0, 0, 0, 0xFF}, []byte{0x60, 0x40, 0xFF, 0xA1}},
	{[]byte{0, 0, 0, 0, 0, 0xAA}, []byte{0x60, 0x40, 0xAA, 0xA1}},

	{[]byte{0, 0, 0, 0, 0, 0, 0xFF}, []byte{0x60, 0x60, 0xFF, 0xA1}},
	{[]byte{0, 0, 0, 0, 0, 0xAA, 0xFF, 0xFF, 0xAA}, []byte{0x60, 0x40, 0xAA, 0xC1, 0xAA, 0xA1}},

	{[]byte{0xAA, 0xAA}, []byte{0xAA, 0xAA, 0xA2}},
	{[]byte{0xAA, 0xBB}, []byte{0xAA, 0xBB, 0xA2}},

	{[]byte{0xFF}, []byte{0xFF, 0xA1}},
	{[]byte{0xFF, 0x00}, []byte{0xFF, 0x21}},
	{[]byte{0xFF, 0xAA}, []byte{0xFF, 0xAA, 0xA2}},

	{[]byte{0xFF, 0xFF}, []byte{0xC0}},
	{[]byte{0xFF, 0xFF, 0x00}, []byte{0xC0, 0x20}},
	{[]byte{0xFF, 0xFF, 0xAA}, []byte{0xC0, 0xAA, 0xA1}},

	{[]byte{0xFF, 0xFF, 0xFF}, []byte{0xE0}},
	{[]byte{0xFF, 0xFF, 0xFF, 0x00}, []byte{0xE0, 0x20}},
	{[]byte{0xFF, 0xFF, 0xFF, 0xAA}, []byte{0xE0, 0xAA, 0xA1}},

	{[]byte{0xFF, 0xFF, 0xFF, 0xFF}, []byte{0x80}},
	{[]byte{0xFF, 0xFF, 0xFF, 0xFF, 0x00}, []byte{0x80, 0x20}},
	{[]byte{0xFF, 0xFF, 0xFF, 0xFF, 0xAA}, []byte{0x80, 0xAA, 0xA1}},

	{[]byte{0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, []byte{0x80, 0xFF, 0xA1}},
	{[]byte{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00}, []byte{0x80, 0xFF, 0x21}},
	{[]byte{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xAA}, []byte{0x80, 0xFF, 0xAA, 0xA2}},

	{[]byte{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, []byte{0x80, 0xC0}},
	{[]byte{0xFF, 0xFF, 0xFF, 0xFF, 0xAA, 0xFF, 0xFF}, []byte{0x80, 0xAA, 0xC1}},
	{[]byte{0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF}, []byte{0x80, 0x20, 0xC0}},

	{[]byte{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xAA, 0xFF, 0xFF}, []byte{0x80, 0xFF, 0xAA, 0xC2}},

	{[]byte{0xAA}, []byte{0xAA, 0xA1}},
	{[]byte{0xAA, 0xAA}, []byte{0xAA, 0xAA, 0xA2}},
	{[]byte{0xAA, 0xAA, 0xAA}, []byte{0xAA, 0x09}},             // AA R2
	{[]byte{0xAA, 0xAA, 0xAA, 0xAA}, []byte{0xAA, 0x11}},       // AA R3
	{[]byte{0xAA, 0xAA, 0xAA, 0xAA, 0xAA}, []byte{0xAA, 0x19}}, // AA R4
	{[]byte{0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA}, []byte{0xAA, 0x19, 0xAA, 0xA1}},
	{[]byte{0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA}, []byte{0xAA, 0x19, 0xAA, 0xAA, 0xA2}},

	{[]byte{0xAA, 0x00}, []byte{0xAA, 0x21}},
	{[]byte{0xAA, 0xAA, 0x00}, []byte{0xAA, 0xAA, 0x22}},

	{[]byte{0, 2, 2, 2, 0, 1, 0, 255, 255, 255, 1, 2, 1, 0, 255, 1, 2, 0, 0, 1, 2, 1, 255, 1, 2, 2, 2, 255, 1, 2, 0, 255, 2, 2, 0},
		[]byte{0x20, 0x2, 0x9, 0x20, 0x1, 0x21, 0xe0, 0x1, 0x2, 0x1, 0x23, 0xff, 0x1, 0x2, 0x43, 0x1, 0x2, 0x1, 0xff, 0x1, 0x2, 0xe, 0xff, 0x1, 0x2, 0x23, 0xff, 0x2, 0x2, 0x23}},

	{[]byte{0xFF, 0x02, 0x02, 0x00}, []byte{0xFF, 0x02, 0x02, 0x23}},
	{[]byte{0xFF, 0x02, 0x02, 0x00, 0xFF}, []byte{0xFF, 0x02, 0x02, 0x23, 0xFF, 0xA1}},

	{[]byte{0, 2, 2, 2, 0, 1, 0, 255, 255, 255, 1, 2, 1, 0, 255, 1, 2, 0, 0, 1, 2, 1, 255, 1, 2, 2, 2, 255, 1, 2, 0, 255, 2, 2, 0, 255},
		[]byte{0x20, 0x2, 0x9, 0x20, 0x1, 0x21, 0xe0, 0x1, 0x2, 0x1, 0x23, 0xff, 0x1, 0x2, 0x43, 0x1, 0x2, 0x1, 0xff, 0x1, 0x2, 0xe, 0xff, 0x1, 0x2, 0x23, 0xff, 0x2, 0x2, 0x23, 0xFF, 0xA1}},
}

// TestTCOBSEncode checks if decoded testData lead to encoded testData.
func TestEncode(t *testing.T) {
	for _, k := range testData {
		enc := make([]byte, 40000)
		n := tcobs.Encode(enc, k.dec)
		enc = enc[:n]
		assert.Equal(t, k.enc, enc)
	}
}

// TestTCOBSDecode checks if encoded testData lead to unencoded testData.
func TestDecode(t *testing.T) { // fails
	b := make([]byte, 40000)
	for _, k := range testData {
		n, e := tcobs.Decode(b, k.enc) // func TCOBSDecode(d, in []byte) (n int, e error) {
		assert.True(t, e == nil)
		assert.True(t, n <= len(b))
		dec := b[len(b)-n:]
		assert.Equal(t, k.dec, dec)
	}
}

// TestDecodertests the repeated usage of Decoder.
func TestDecoder(t *testing.T) {
	i := []byte{0xAA, 0xBB, 0xA2, 0, 4, 0xA1, 0, 6}
	before, after, err := tcobs.Decoder(i) // panic
	assert.True(t, err == nil)
	assert.Equal(t, i[:2], before)
	assert.Equal(t, i[4:], after)

	before, after, err = tcobs.Decoder(after)
	assert.True(t, err == nil)
	assert.Equal(t, i[4:5], before)
	assert.Equal(t, i[7:], after)

	before, after, err = tcobs.Decoder(after)
	assert.True(t, err == nil)
	var exp []uint8
	assert.Equal(t, exp, before)
	assert.Equal(t, i[7:], after)
}

func PrintAsGoCode(x []byte) {
	fmt.Print("[]byte{")
	for _, b := range x {
		fmt.Printf("0x%02x, ", b)
	}
	fmt.Println("}")
}

func TestEncodeDecode12(t *testing.T) { // fails
	max := 32768
	for i := 0; i < 100; i++ {
		length := rand.Intn(max)
		datBuf := make([]byte, max)
		encBuf := make([]byte, 2*max) // max 1 + (1+1/32)*len) ~= 1.04 * len
		decBuf := make([]byte, 2*max) // max 1 + (1+1/32)*len) ~= 1.04 * len
		for i := 0; i < length; i++ {
			b := uint8(rand.Intn(4)) - 1 // -1, 0, 1 2
			datBuf[i] = b
		}
		dat := datBuf[:length]
		fmt.Println()
		n := tcobs.Encode(encBuf, dat)
		enc := encBuf[:n]
		n, e := tcobs.Decode(decBuf, enc)
		assert.True(t, e == nil)
		dec := decBuf[len(decBuf)-n:]

		PrintAsGoCode(dat)
		PrintAsGoCode(enc)
		PrintAsGoCode(dec)

		assert.Equal(t, dat, dec)
	}
}

func TestEncodeDecode1(t *testing.T) { // fails
	max := 32768
	for i := 0; i < 100; i++ {
		length := rand.Intn(max)
		datBuf := make([]byte, max)
		encBuf := make([]byte, 2*max) // max 1 + (1+1/32)*len) ~= 1.04 * len
		decBuf := make([]byte, 2*max) // max 1 + (1+1/32)*len) ~= 1.04 * len
		for i := 0; i < length; i++ {
			b := uint8(rand.Intn(3)) - 1 // -1, 0, 1
			datBuf[i] = b
		}
		dat := datBuf[:length]
		fmt.Println()
		n := tcobs.Encode(encBuf, dat)
		enc := encBuf[:n]
		n, e := tcobs.Decode(decBuf, enc)
		assert.True(t, e == nil)
		dec := decBuf[len(decBuf)-n:]

		PrintAsGoCode(dat)
		PrintAsGoCode(enc)
		PrintAsGoCode(dec)

		assert.Equal(t, dat, dec)
	}
}

func TestEncodeDecode256(t *testing.T) { // fails
	max := 32768
	for i := 0; i < 100; i++ {
		length := rand.Intn(max)
		datBuf := make([]byte, max)
		encBuf := make([]byte, 2*max) // max 1 + (1+1/32)*len) ~= 1.04 * len
		decBuf := make([]byte, 2*max) // max 1 + (1+1/32)*len) ~= 1.04 * len
		for i := 0; i < length; i++ {
			b := uint8(rand.Intn(256)) // 0, ..., 0xFF
			datBuf[i] = b
		}
		dat := datBuf[:length]
		fmt.Println()
		n := tcobs.Encode(encBuf, dat)
		enc := encBuf[:n]
		n, e := tcobs.Decode(decBuf, enc)
		assert.True(t, e == nil)
		dec := decBuf[len(decBuf)-n:]

		PrintAsGoCode(dat)
		PrintAsGoCode(enc)
		PrintAsGoCode(dec)

		assert.Equal(t, dat, dec)
	}
}
