// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// whitebox test
package id

import (
	"math/rand"
	"testing"

	"github.com/rokath/trice/pkg/tst"
)

func TestNewID(t *testing.T) {
	rand.Seed(0)
	lut := make(TriceIDLookUp)
	id := lut.newID()
	tst.AssertTrue(t, 43255 == id)
	SearchMethod = "downward"
	id = lut.newID()
	tst.AssertTrue(t, 65535 == id)
	SearchMethod = "upward"
	id = lut.newID()
	tst.AssertTrue(t, 1 == id)
	id = lut.newID()
	tst.AssertTrue(t, 1 == id)
	var i TriceFmt
	lut[id] = i
	id = lut.newID()
	tst.AssertTrue(t, 2 == id)
}

func TestNewUpwardID(t *testing.T) {
	min := TriceID(97)
	max := TriceID(100)
	var i TriceFmt
	lut := make(TriceIDLookUp, 4)
	lut[98] = i // add
	lut[99] = i // add
	id := lut.newUpwardID(min, max)
	tst.AssertTrue(t, 97 == id)
	lut[id] = i // add
	id = lut.newUpwardID(min, max)
	tst.AssertTrue(t, 100 == id)
	delete(lut, 98)
	delete(lut, 99)
	id = lut.newUpwardID(min, max)
	tst.AssertTrue(t, 98 == id)
}

func TestNewDownwardID(t *testing.T) {
	min := TriceID(97)
	max := TriceID(100)
	var i TriceFmt
	lut := make(TriceIDLookUp, 4)
	lut[98] = i // add
	lut[99] = i // add
	id := lut.newDownwardID(min, max)
	tst.AssertTrue(t, 100 == id)
	lut[id] = i // add
	id = lut.newDownwardID(min, max)
	tst.AssertTrue(t, 97 == id)
	delete(lut, 98)
	delete(lut, 99)
	id = lut.newDownwardID(min, max)
	tst.AssertTrue(t, 99 == id)
}

func TestNewRandomID(t *testing.T) {
	rand.Seed(0)
	min := TriceID(50)
	max := TriceID(100)
	lut := make(TriceIDLookUp, 4)
	id := lut.newRandomID(min, max)
	tst.AssertTrue(t, 56 == id)
	id = lut.newRandomID(min, max)
	tst.AssertTrue(t, 92 == id)
	id = lut.newRandomID(92, 92)
	tst.AssertTrue(t, 92 == id)
}
