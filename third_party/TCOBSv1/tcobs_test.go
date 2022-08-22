// Package tcobs_test contains TCOBS whitebox tests.
package tcobs_test

import (
	"bytes"
	"fmt"
	"io"
	"math/rand"
	"testing"

	"github.com/rokath/tcobs"
	"github.com/tj/assert"
)

const (
	maxLength int = 1000000
	rounds    int = 2000
	maxEqual  int = 100000
)

// TestCEncodeCDecode12 tests on generated random byte numbers 0xFF, 0x00, 0x01 and 0x02 for random length 0-32767.
func TestRandom12CEncodeCDecode(t *testing.T) {
	datBuf := make([]byte, maxLength)

	for i := 0; i < rounds; i++ {
		length := rand.Intn(maxLength)
		j := 0
		b := uint8(rand.Intn(4)) - 1 // -1, 0, 1 2
		for k := 0; k < maxEqual && j < length; k++ {
			datBuf[j] = b
			j++
		}
		dat := datBuf[:length]
		checkSetCEncodeCDecode(t, i, dat)
	}
}

// TestCEncodeCDecode12 tests on generated random byte numbers 0xFF, 0x00, 0x01 and 0x02 for random length 0-32767.
func TestRandom12CEncodeGoDecode(t *testing.T) {
	datBuf := make([]byte, maxLength)

	for i := 0; i < rounds; i++ {
		length := rand.Intn(maxLength)
		j := 0
		b := uint8(rand.Intn(4)) - 1 // -1, 0, 1 2
		for k := 0; k < maxEqual && j < length; k++ {
			datBuf[j] = b
			j++
		}
		dat := datBuf[:length]
		checkSetCEncodeGoDecode(t, i, dat)
	}
}

// TestCEncodeCDecode1 tests on generated random byte numbers 0xFF, 0x00 and 0x01 for random length 0-32767.
func TestCEncodeCDecode1(t *testing.T) {
	datBuf := make([]byte, maxLength)
	for i := 0; i < rounds; i++ {
		length := rand.Intn(maxLength)
		for i := 0; i < length; i++ {
			b := uint8(rand.Intn(3)) - 1 // -1, 0, 1
			datBuf[i] = b
		}
		dat := datBuf[:length]
		checkSetCEncodeCDecode(t, i, dat)
	}
}

// TestCEncodeCDecode1 tests on generated random byte numbers 0xFF, 0x00 and 0x01 for random length 0-32767.
func TestCEncodeGoDecode1(t *testing.T) {
	datBuf := make([]byte, maxLength)
	for i := 0; i < rounds; i++ {
		length := rand.Intn(maxLength)
		for i := 0; i < length; i++ {
			b := uint8(rand.Intn(3)) - 1 // -1, 0, 1
			datBuf[i] = b
		}
		dat := datBuf[:length]
		checkSetCEncodeGoDecode(t, i, dat)
	}
}

// TestCEncodeCDecode256 tests on generated random byte numbers for random length 0-32767.
func TestCEncodeCDecode256(t *testing.T) {
	datBuf := make([]byte, maxLength)
	for i := 0; i < rounds; i++ {
		length := rand.Intn(maxLength)
		for i := 0; i < length; i++ {
			b := uint8(rand.Intn(256)) // 0, ..., 0xFF
			datBuf[i] = b
		}
		dat := datBuf[:length]
		checkSetCEncodeCDecode(t, i, dat)
	}
}

// TestCEncodeGoDecode256 tests on generated random byte numbers for random length 0-32767.
func TestCEncodeGoDecode256(t *testing.T) {
	datBuf := make([]byte, maxLength)
	for i := 0; i < rounds; i++ {
		length := rand.Intn(maxLength)
		for i := 0; i < length; i++ {
			b := uint8(rand.Intn(256)) // 0, ..., 0xFF
			datBuf[i] = b
		}
		dat := datBuf[:length]
		checkSetCEncodeGoDecode(t, i, dat)
	}
}

func checkSetCEncodeCDecode(t *testing.T, index int, dat []byte) {
	encBuf := make([]byte, 2*len(dat)) // max 1 + (1+1/32)*len) ~= 1.04 * len
	decBuf := make([]byte, 2*len(dat)) // max 1 + (1+1/32)*len) ~= 1.04 * len

	en := tcobs.CEncode(encBuf, dat)
	if en < 0 {
		fmt.Println(index, "dat:", len(dat), dat)
		fmt.Println(index, "encode failed with result", en)
	}
	assert.True(t, en >= 0)
	enc := encBuf[:en]
	assert.False(t, bytes.Contains(enc, []byte{0}))

	dn := tcobs.CDecode(decBuf, enc)
	if dn < 0 {
		fmt.Println(index, "dat:", len(dat), dat)
		fmt.Println(index, "enc:", en, enc)
		fmt.Println(index, "decode failed with result", dn)
	}
	assert.True(t, dn >= 0)
	dec := decBuf[len(decBuf)-dn:]
	if dn != len(dat) {
		fmt.Printf("\nindex=%d: dat:len=%d\n", index, len(dat))
		dump(dat, 32)
		fmt.Printf("\nindex=%d: enc:len=%d\n", index, len(enc))
		dump(enc, 32)
		fmt.Printf("\nindex=%d: dec:len=%d\n", index, dn)
		dump(dec, 32)
		fmt.Printf("\nindex=%d: decode failed with length %d, expected is %d\n", index, dn, len(dat))
	}
	assert.True(t, dn == len(dat))
	assert.Equal(t, dat, dec)
}

func checkSetCEncodeGoDecode(t *testing.T, index int, dat []byte) {
	encBuf := make([]byte, 2*len(dat)) // max 1 + (1+1/32)*len) ~= 1.04 * len
	decBuf := make([]byte, 2*len(dat)) // max 1 + (1+1/32)*len) ~= 1.04 * len

	en := tcobs.CEncode(encBuf, dat)
	if en < 0 {
		fmt.Println(index, "dat:", len(dat), dat)
		fmt.Println(index, "encode failed with result", en)
	}
	assert.True(t, en >= 0)
	enc := encBuf[:en]
	assert.False(t, bytes.Contains(enc, []byte{0}))

	dn, e := tcobs.Decode(decBuf, enc)
	if e != nil && e != io.EOF {
		fmt.Println(index, "decode error:", e)
	}
	assert.True(t, e == nil || e == io.EOF)
	assert.True(t, dn >= 0)
	dec := decBuf[len(decBuf)-dn:]
	if dn != len(dat) {
		fmt.Printf("\nindex=%d: dat:len=%d\n", index, len(dat))
		dump(dat, 32)
		fmt.Printf("\nindex=%d: enc:len=%d\n", index, len(enc))
		dump(enc, 32)
		fmt.Printf("\nindex=%d: dec:len=%d\n", index, dn)
		dump(dec, 32)
		fmt.Printf("\nindex=%d: decode failed with length %d, expected is %d\n", index, dn, len(dat))
	}
	assert.True(t, dn == len(dat))
	assert.Equal(t, dat, dec)
}

func dump(b []byte, width int) {
	for i := 0; i < len(b); i++ {
		if i%width == 0 {
			fmt.Printf("\t//%6d\n", i)
		}
		fmt.Printf("0x%02x,", b[i])
	}
}
