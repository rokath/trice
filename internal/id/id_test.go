// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// whitebox test
package id

import (
	"math/rand"
	"os"
	"testing"

	"github.com/tj/assert"
)

func TestNewID(t *testing.T) {
	rand.Seed(0)
	lut := make(TriceIDLookUp)
	w := os.Stdout
	id := lut.newID(w, 32768, 65535, "random")
	assert.True(t, id == 45050)
	id = lut.newID(w, 1, 65535, "downward")
	assert.True(t, id == 65535)
	id = lut.newID(w, 32768, 65535, "upward")
	assert.True(t, id == 32768)
	id = lut.newID(w, 32768, 65535, "upward")
	assert.True(t, id == 32768)
	var i TriceFmt
	lut[id] = i
	id = lut.newID(w, 32768, 65535, "upward")
	assert.True(t, id == 32769)
}

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

func TestNewRandomID(t *testing.T) {
	rand.Seed(0)
	min := TriceID(50)
	max := TriceID(100)
	lut := make(TriceIDLookUp, 4)
	w := os.Stdout
	id := lut.newRandomID(w, min, max)
	assert.True(t, id == 56)
	id = lut.newRandomID(w, min, max)
	assert.True(t, id == 92)
	id = lut.newRandomID(w, 92, 92)
	assert.True(t, id == 92)
}
