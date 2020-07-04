// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package display_test is a blackbox test for package display.
package display_test

import (
	"os"
	"testing"

	"github.com/rokath/trice/pkg/cage"
	"github.com/rokath/trice/pkg/display"
	"github.com/rokath/trice/pkg/lib"
)

var (
	d0 *display.T
	d1 *display.T
	d2 *display.T
)

func init() {
	d0 = display.New("off")
	d1 = display.New("none")
	d2 = display.New("default")
}

// show is called to see ls on d
func show(d *display.T, ls []display.Line) {
	d.Write(ls)
}

// visualize sends l to displays d0, d1, d2
func visualize(ls []display.Line) {
	show(d0, ls)
	show(d1, ls)
	show(d2, ls)
}

func Test1(t *testing.T) {
	afn := "testdata/act.log"
	efn := "testdata/exp.log"
	os.Remove(afn)
	c := cage.Start(afn)
	var l1, l2 display.Line
	l1.Ss = []string{"a", " + ", "msg:a", " + ", "MESSAGE:a", " + ", "unknown:a"}
	l2.Ss = []string{"b", " + ", "MSG:b", " + ", "debug:b", " + ", "Unknown:b"}
	ls := []display.Line{l1, l2}
	visualize(ls)
	cage.Stop(c)
	lib.EqualTextFiles(t, afn, efn)
	os.Remove(afn)
}
