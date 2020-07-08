// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package interpreter_test

import (
	"testing"

	interpreter "github.com/rokath/trice/internal/Interpreter"
	"github.com/rokath/trice/internal/bare"
	"github.com/rokath/trice/pkg/display"
	"github.com/rokath/trice/pkg/lib"
)

func Test0(t *testing.T) {
	trices := make(chan []bare.Item, 4096)
	d := display.New("default")

	p, e := interpreter.New(d, trices)
	lib.Equals(t, nil, e)

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
