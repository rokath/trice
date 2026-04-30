// SPDX-License-Identifier: MIT

// white-box test
package id

import (
	"os"
	"testing"

	"github.com/stretchr/testify/assert"
)

// TestFformatSpecifierCount verifies the expected behavior.
func TestFformatSpecifierCount(t *testing.T) {
	type testVector struct {
		fmt   string
		count int
	}

	testTable := []testVector{
		{"%%d", 0},
		{"%d", 1},
		{"%4u", 1},
		{"some%4utext", 1},
		{"some%4utext%9.3f %Gustaf", 3},
	}
	for _, item := range testTable {
		act := formatSpecifierCount(item.fmt)
		assert.Equal(t, item.count, act)
	}
}

// TestFormatSpecifierCountLengthModifiers verifies that valid C length modifiers
// are still counted as exactly one format specifier by the source scanner.
func TestFormatSpecifierCountLengthModifiers(t *testing.T) {
	tt := []struct {
		name  string
		fmt   string
		count int
	}{
		{name: "size_t unsigned", fmt: "count=%zu", count: 1},
		{name: "unsigned long width", fmt: "count=%8lu", count: 1},
		{name: "size_t signed", fmt: "count=%zd", count: 1},
		{name: "size_t hex upper", fmt: "mask=%zX", count: 1},
		{name: "long hex upper", fmt: "mask=%08lX", count: 1},
		{name: "long long hex upper", fmt: "mask=%016llX", count: 1},
		{name: "long double float", fmt: "value=%04Lf", count: 1},
		{name: "escaped size_t unsigned", fmt: "%%zu", count: 0},
	}
	for _, tc := range tt {
		t.Run(tc.name, func(t *testing.T) {
			assert.Equal(t, tc.count, formatSpecifierCount(tc.fmt))
		})
	}
}

// TestNewID verifies the expected behavior.
func TestNewID(t *testing.T) {
	var id, mi, ma TriceID
	lut := make(TriceIDLookUp)
	w := os.Stdout
	mi = 32768
	ma = 65535
	id = lut.newID(w, 1, ma, "downward")
	assert.True(t, id == ma)
	id = lut.newID(w, mi, ma, "upward")
	assert.True(t, id == mi)
	id = lut.newID(w, mi, ma, "upward")
	assert.True(t, id == mi)
	var i TriceFmt
	lut[id] = i
	id = lut.newID(w, mi, ma, "upward")
	assert.True(t, id == 32769)
	id = lut.newID(w, mi, ma, "random")
	assert.True(t, mi <= id && id <= ma)
}

// TestNewUpwardID verifies the expected behavior.
func TestNewUpwardID(t *testing.T) {
	min := TriceID(97)
	max := TriceID(100)
	var i TriceFmt
	lut := make(TriceIDLookUp, 4)
	lut[98] = i // add
	lut[99] = i // add
	id := lut.newUpwardID(min, max)
	assert.True(t, id == 97)
	lut[id] = i // add
	id = lut.newUpwardID(min, max)
	assert.True(t, id == 100)
	delete(lut, 98)
	delete(lut, 99)
	id = lut.newUpwardID(min, max)
	assert.True(t, id == 98)
}

// TestNewDownwardID verifies the expected behavior.
func TestNewDownwardID(t *testing.T) {
	min := TriceID(97)
	max := TriceID(100)
	var i TriceFmt
	lut := make(TriceIDLookUp, 4)
	lut[98] = i // add
	lut[99] = i // add
	id := lut.newDownwardID(min, max)
	assert.True(t, id == 100)
	lut[id] = i // add
	id = lut.newDownwardID(min, max)
	assert.True(t, id == 97)
	delete(lut, 98)
	delete(lut, 99)
	id = lut.newDownwardID(min, max)
	assert.True(t, id == 99)
}

// TestNewRandomID verifies the expected behavior.
func TestNewRandomID(t *testing.T) {
	mi := TriceID(50)
	ma := TriceID(100)
	lut := make(TriceIDLookUp, 4)
	w := os.Stdout
	id := lut.newRandomID(w, mi, ma)
	assert.True(t, mi <= id && id <= ma)
	id = lut.newRandomID(w, 92, 92)
	assert.True(t, id == 92)
}
