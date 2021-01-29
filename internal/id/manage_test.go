// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// whitebox test
package id

import (
	"fmt"
	"testing"

	"github.com/rokath/trice/pkg/tst"
)

func sampleLut0() (lu TriceIDLookUp) {
	lu = make(TriceIDLookUp)
	lu[11] = TriceFmt{Type: "t11", Strg: "s11"}
	lu[12] = TriceFmt{Type: "t12", Strg: "s12"}
	lu[12] = TriceFmt{Type: "t12", Strg: "s12a"}
	return
}

const reverseSampleLut0 = "map[{t11 s11}:11 {t12 s12a}:12]"

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

const sampleLutMap0 = "map[11:{t11 s11} 12:{t12 s12a}]"
const sampleLutMap1 = "map[12:{t12 s12} 33:{t33 s33}]"

// TestLutToJSONFilledByteSlice checks normal initial case.
func TestLutToJSON(t *testing.T) {
	lut := sampleLut0()
	exp := sampleLutJSON0
	b, e := lut.toJSON()
	tst.AssertNoErr(t, e)
	act := string(b)
	tst.Equal(t, exp, act)
}

// TestJSONToLutEmptyByteSlice checks empty case.
func TestJSONToLutEmptyByteSlice(t *testing.T) {
	var b []byte
	exp := "map[]"
	lut := make(TriceIDLookUp)
	tst.AssertNoErr(t, lut.FromJSON(b))
	act := fmt.Sprint(lut)
	tst.Equal(t, exp, act)
}

// TestJSONToLutMapUpdate checks if a prefilled map is extended and updated.
func TestJSONToLutMapUpdate(t *testing.T) {
	b := []byte(sampleLutJSON0)
	exp := "map[11:{t11 s11} 12:{t12 s12} 33:{t33 s33}]"
	lut := make(TriceIDLookUp)
	tst.AssertNoErr(t, lut.FromJSON(b))
	b1 := []byte(sampleLutJSON1)
	lut.FromJSON(b1)
	act := fmt.Sprint(lut)
	tst.Equal(t, exp, act)
}

// TestJSONToLut checks if JSON to lut works.
func TestJSONToLut(t *testing.T) {
	b := []byte(sampleLutJSON0)
	exp := sampleLutMap0
	lut := make(TriceIDLookUp)
	tst.AssertNoErr(t, lut.FromJSON(b))
	act := fmt.Sprint(lut)
	tst.Equal(t, exp, act)
}

// TestLutFileTransfer checks lut file transfer.
func TestLutFileTransfer(t *testing.T) {
	wr := sampleLut0()
	exp := sampleLutMap0
	fn := tst.TempFileName("TestWriteLutToFile*.JSON")
	tst.AssertNoErr(t, wr.toFile(fn))
	rd := make(TriceIDLookUp)
	tst.AssertNoErr(t, rd.fromFile(fn))
	act := fmt.Sprint(rd)
	tst.Equal(t, exp, act)
}
