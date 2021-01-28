// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// whitebox test
package id

import (
	"fmt"
	"testing"

	"github.com/rokath/trice/pkg/tst"
)

func sampeLut() (lut LookUp) {
	lut = make(LookUp)
	lut[111] = Item{FmtType: "t111", FmtStrg: "ss"}
	lut[12] = Item{FmtType: "t12", FmtStrg: "s12"}
	lut[12] = Item{FmtType: "tt", FmtStrg: "ss"}
	return
}

const reverseSampleLut = "map[{t111 ss}:111 {tt ss}:12]"

const sampleLutJSON = `{
	"111": {
		"fmtType": "t111",
		"fmtStrg": "ss"
	},
	"12": {
		"fmtType": "tt",
		"fmtStrg": "ss"
	}
}`

const sampleLutJSON1 = `{
	"3": {
		"fmtType": "t3",
		"fmtStrg": "s3"
	},
	"12": {
		"fmtType": "t12",
		"fmtStrg": "ss"
	}
}`

const sampleLutMap = "map[12:{tt ss} 111:{t111 ss}]"
const sampleLutMap1 = "map[12:{tt ss} 333:{t33 ss3}]"

func TestLutReverse(t *testing.T) {
	lut := sampeLut()
	exp := reverseSampleLut
	ilut := lut.reverse()
	act := fmt.Sprint(ilut)
	tst.Equal(t, exp, act)
}

// TestLutToJSONFilledByteSlice checks normal initial case.
func TestLutToJSON(t *testing.T) {
	lut := sampeLut()
	exp := sampleLutJSON
	b, e := lut.toJSON()
	tst.AssertNoErr(t, e)
	act := string(b)
	tst.Equal(t, exp, act)
}

// TestJSONToLutEmptyByteSlice checks empty case.
func TestJSONToLutEmptyByteSlice(t *testing.T) {
	var b []byte
	exp := "map[]"
	try := make(map[Item]int)
	fmt.Println(try)
	lut := make(LookUp)
	tst.AssertNoErr(t, lut.FromJSON(b))
	act := fmt.Sprint(lut)
	tst.Equal(t, exp, act)
}

// TestJSONToLutMapUpdate checks if a prefilled map is extended and updated.
func TestJSONToLutMapUpdate(t *testing.T) {
	b := []byte(sampleLutJSON)
	exp := "map[3:{t3 s3} 12:{t12 ss} 111:{t111 ss}]"
	lut := make(LookUp)
	tst.AssertNoErr(t, lut.FromJSON(b))
	b1 := []byte(sampleLutJSON1)
	lut.FromJSON(b1)
	act := fmt.Sprint(lut)
	tst.Equal(t, exp, act)
}

// TestJSONToLut checks if JSON to lut works.
func TestJSONToLut(t *testing.T) {
	b := []byte(sampleLutJSON)
	exp := sampleLutMap
	lut := make(LookUp)
	tst.AssertNoErr(t, lut.FromJSON(b))
	act := fmt.Sprint(lut)
	tst.Equal(t, exp, act)
}

// TestLutFileTransfer checks lut file transfer.
func TestLutFileTransfer(t *testing.T) {
	wr := sampeLut()
	exp := sampleLutMap
	fn := tst.TempFileName("TestWriteLutToFile*.JSON")
	tst.AssertNoErr(t, wr.toFile(fn))
	rd := make(LookUp)
	tst.AssertNoErr(t, rd.fromFile(fn))
	act := fmt.Sprint(rd)
	tst.Equal(t, exp, act)
}
