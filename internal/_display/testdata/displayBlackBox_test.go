// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package display_test is a blackbox test for package display.
package display_test

import (
	"os"
	"testing"

	"github.com/rokath/trice/pkg/cage"
	"github.com/rokath/trice/pkg/display"
)

var (

	// d0 is a display, it provides a
	d0 *display.T
	d1 *display.T
	d2 *display.T
)

// create 3 displays
func init() {
	d0 = display.New("off")
	d1 = display.New("none")
	d2 = display.New("default")
}

// show is called to see lines on d
func show(d *display.T, lines []display.Line) {
	d.Write(lines)
}

// visualize sends lines to displays d0, d1, d2
func visualize(lines []display.Line) {
	show(d0, lines)
	show(d1, lines)
	show(d2, lines)
}

func Test1(t *testing.T) {

	// prepare
	afn := "testdata/act.log"
	efn := "testdata/exp.log"
	os.Remove(afn)
	c := cage.Start(afn)

	// create 2 test lines from segments
	var l1, l2 display.Line
	l1.Segments = []string{"a", " + ", "msg:a", " + ", "MESSAGE:a", " + ", "unknown:a"}
	l2.Segments = []string{"b", " + ", "MSG:b", " + ", "debug:b", " + ", "Unknown:b"}

	// create lines slice for displaying
	lines := []display.Line{l1, l2}

	// display the 2 lines on the 3 displays
	visualize(lines)
	cage.Stop(c)

	// check output
	notEqualTextFilesFail(t, afn, efn)
	os.Remove(afn)
}
