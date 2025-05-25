// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// white-box test
package id

import (
	"fmt"
	"github.com/rokath/trice/pkg/tst"
	"testing"

	"github.com/tj/assert"
)

// sampleLut0 is a test helper function, which returns a lookup map.
func sampleLut0() (lu TriceIDLookUp) {
	lu = make(TriceIDLookUp)
	lu[11] = TriceFmt{Type: "t11", Strg: "s11"}
	lu[12] = TriceFmt{Type: "t12", Strg: "s12"}
	lu[12] = TriceFmt{Type: "t12", Strg: "s12a"}
	return
}

const sampleLutJSON0 = `{
	"11": {
		"Type": "t11",
		"Strg": "s11"
	},
	"12": {
		"Type": "t12",
		"Strg": "s12a"
	}
}`

const sampleLutMap0 = "map[11:{t11 s11} 12:{t12 s12a}]"

// TestLutToJSONFilledByteSlice checks normal initial case.
func TestLutToJSON(t *testing.T) {
	lut := sampleLut0()
	exp := sampleLutJSON0
	b, e := lut.toJSON()
	assert.Nil(t, e)
	act := string(b)
	assert.Equal(t, exp, act)
}

// TestJSONToLutEmptyByteSlice checks empty case.
func TestJSONToLutEmptyByteSlice(t *testing.T) {
	var b []byte
	exp := "map[]"
	lut := make(TriceIDLookUp)
	assert.Nil(t, lut.FromJSON(b))
	act := fmt.Sprint(lut)
	assert.Equal(t, exp, act)
}

// TestJSONToLutMapUpdate checks if a prefilled map is extended and updated.
func TestJSONToLutMapUpdate(t *testing.T) {
	b := []byte(sampleLutJSON0)
	exp := "map[11:{t11 s11} 12:{t12 s12} 33:{t33 s33}]"
	lut := make(TriceIDLookUp)
	assert.Nil(t, lut.FromJSON(b))
	const sampleLutJSON1 = `{
	"33": {
		"Type": "t33",
		"Strg": "s33"
	},
	"12": {
		"Type": "t12",
		"Strg": "s12"
	}
}`
	b1 := []byte(sampleLutJSON1)
	err := lut.FromJSON(b1)
	assert.Nil(t, err)
	act := fmt.Sprint(lut)
	act = tst.NormalizeMapString(act)
	assert.Equal(t, exp, act)
}

// TestJSONToLut checks if JSON to lut works.
func TestJSONToLut(t *testing.T) {
	b := []byte(sampleLutJSON0)
	exp := sampleLutMap0
	lut := make(TriceIDLookUp)
	assert.Nil(t, lut.FromJSON(b))
	act := fmt.Sprint(lut)
	act = tst.NormalizeMapString(act)
	assert.Equal(t, exp, act)
}

const sampleLutJSON1 = `{
	"15": {
		"Type": "t11",
		"Strg": "s11"
	},
	"15": {
		"Type": "t12",
		"Strg": "s12a"
	}
}`

const sampleLutMap1 = "map[15:{t12 s12a}]"

// TestJSONToLut checks if JSON to lut with IDs used twice works.
func TestJSONWithDoubleIDToLut(t *testing.T) {
	b := []byte(sampleLutJSON1)
	exp := sampleLutMap1
	lut := make(TriceIDLookUp)
	assert.Nil(t, lut.FromJSON(b))
	act := fmt.Sprint(lut)
	act = tst.NormalizeMapString(act)
	assert.Equal(t, exp, act)
}
