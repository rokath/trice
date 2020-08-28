// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package interpreter_test

/*
import (
	"testing"

	interpreter "github.com/rokath/trice/internal/Interpreter"
	"github.com/rokath/trice/internal/bare"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/pkg/display"
	"github.com/rokath/trice/pkg/lib"
)

func Test0(t *testing.T) {
	trices := make(chan []bare.Item, 4096)
	d := display.New("default")

	p, e := interpreter.New(d, trices)
	assertEqual(t, nil, e)

	x := []bare.Item{
		{
			ID:    0xC012,
			Value: [2]byte{0x09, 0x60},
		},
		{
			ID:    0xC120,
			Value: [2]byte{0x09, 0x60},
		},
	}

	trices <- x
	p.Interpret()
	//t.Fail()
}

func Test1(t *testing.T) {
	id.FnJSON = "testdata/til.json"
	id.FnJSON = lib.ConditinalFilePath(id.FnJSON)
	id.ReadListFile()
	trices := make(chan []bare.Item, 4096)
	d := display.New("default")

	p, e := interpreter.New(d, trices)
	assertEqual(t, nil, e)

	x := []bare.Item{
		//   {
		//   	"id": 46097,
		//   	"fmtType": "TRICE16_1",
		//   	"fmtStrg": "dbg:12345 as 16bit is %#016b\\n",
		//   	"created": 1590414933,
		//   	"removed": 0
		//   },
		//   {
		//   	"id": 65508,
		//   	"fmtType": "TRICE16_1",
		//   	"fmtStrg": "TST:test        message, SysTick is %6d\\n",
		//   	"created": 1590414933,
		//   	"removed": 0
		//   },
		{
			ID:    46097,
			Value: [2]byte{0x39, 0x30},
		},
		{
			ID:    65508,
			Value: [2]byte{0x09, 0x60},
		},
	}

	f := func() {
		trices <- x
		p.Interpret()
	}
	//   	exp := `dbg:12345 as 16bit is 0b0011000000111001
	//   TST:test        message, SysTick is  24585
	//   `
//
//
//
	//   	// https://stackoverflow.com/questions/19646889/why-should-i-use-log-println-instead-of-fmt-println
	//   	act := capturer.CaptureOutput(f) // caturer does not capture log output.
	//   	assertEqual(t, exp, act)          // TODO this test fails because ansiOut uses log output. But if I change that there to fmt other tests will fail.
	f()
}
*/s log output. But if I change that there to fmt other tests will fail.
	f()
}
*/
