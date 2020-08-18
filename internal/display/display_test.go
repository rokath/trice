// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package display_test is whitebox test for package display.
package display_test

import (
	"os"
	"testing"

	"github.com/rokath/trice/internal/display"
	"github.com/rokath/trice/pkg/cage"
	"github.com/rokath/trice/pkg/lib"
)

func TestLocal(t *testing.T) {
	// prepare
	afn := "testdata/act.log"
	efn := "testdata/exp.log"
	os.Remove(afn)
	c := cage.Start(afn)

	// do
	p := display.NewLocal()
	l1 := []string{"This is ", "the 1st ", "line"}
	l2 := []string{"This is ", "the 2nd ", "line"}
	p.LineWrite(l1)
	p.LineWrite(l2)

	// check output
	cage.Stop(c)
	lib.EqualTextFiles(t, afn, efn)
	os.Remove(afn)
}

func TestRemote(t *testing.T) {
	// prepare
	afn := "testdata/act.log"
	efn := "testdata/exp.log"
	os.Remove(afn)

	// do
	p := display.NewRemote("localhost", "65497", "trice.exe", afn)
	l1 := []string{"This is ", "the 1st ", "line"}
	l2 := []string{"This is ", "the 2nd ", "line"}
	p.LineWrite(l1)
	p.LineWrite(l2)

	// check output
	lib.EqualTextFiles(t, afn, efn)
	os.Remove(afn)
}
