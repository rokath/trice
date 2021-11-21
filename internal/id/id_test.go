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
	id := lut.newID(os.Stdout, 32768, 65535, "random")
	assert.True(t, 45050 == id)
	id = lut.newID(os.Stdout, 1, 65535, "downward")
	assert.True(t, 65535 == id)
	id = lut.newID(os.Stdout, 32768, 65535, "upward")
	assert.True(t, 32768 == id)
	id = lut.newID(os.Stdout, 32768, 65535, "upward")
	assert.True(t, 32768 == id)
	var i TriceFmt
	lut[id] = i
	id = lut.newID(os.Stdout, 32768, 65535, "upward")
	assert.True(t, 32769 == id)
}

func TestNewUpwardID(t *testing.T) {
	min := TriceID(97)
	max := TriceID(100)
	var i TriceFmt
	lut := make(TriceIDLookUp, 4)
	lut[98] = i // add
	lut[99] = i // add
	id := lut.newUpwardID(min, max)
	assert.True(t, 97 == id)
	lut[id] = i // add
	id = lut.newUpwardID(min, max)
	assert.True(t, 100 == id)
	delete(lut, 98)
	delete(lut, 99)
	id = lut.newUpwardID(min, max)
	assert.True(t, 98 == id)
}

func TestNewDownwardID(t *testing.T) {
	min := TriceID(97)
	max := TriceID(100)
	var i TriceFmt
	lut := make(TriceIDLookUp, 4)
	lut[98] = i // add
	lut[99] = i // add
	id := lut.newDownwardID(min, max)
	assert.True(t, 100 == id)
	lut[id] = i // add
	id = lut.newDownwardID(min, max)
	assert.True(t, 97 == id)
	delete(lut, 98)
	delete(lut, 99)
	id = lut.newDownwardID(min, max)
	assert.True(t, 99 == id)
}

func TestNewRandomID(t *testing.T) {
	rand.Seed(0)
	min := TriceID(50)
	max := TriceID(100)
	lut := make(TriceIDLookUp, 4)
	id := lut.newRandomID(os.Stdout, min, max)
	assert.True(t, 56 == id)
	id = lut.newRandomID(os.Stdout, min, max)
	assert.True(t, 92 == id)
	id = lut.newRandomID(os.Stdout, 92, 92)
	assert.True(t, 92 == id)
}
