// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package display prowides a line writer.
//
// A line is a string slice type. The last string contains no newline.
// The newline is implicit defined by the end of the slice.
// Each string can start with a color specifier, like "message:".
// All characters until the first colon ':' are treated as color specifier.
// Unknown color specifiers are ignored. The string remains unchanged.
// Known color specifiers are prefixed with an ANSI code for a color.
// Known lowercase color specifiers are removed.
// The last string is postfixed with an ANSI code for default terminal color.
// Finally a newline code `\n` is appended before the line is sent to the terminal.
package display

import "github.com/rokath/trice/internal/disp"

var ()

// Line is the data struct written to Display
type Line struct {
	Ss []string
}

// IF is the provided display interface.
type IF interface {
	Writer([]Line) (int, error)
}

// T is the display type with a write method for line.
type T struct {
	colorPalette string
}

// New provides a display with color palette properties.
func New(cp string) *T {
	d := &T{
		colorPalette: cp,
	}
	return d
}

// Write is the display write method.
// It writes all lines in ls to display using the display internals settings defined on display creation with New.
func (d *T) Write(ls []Line) (int, error) {
	for _, l := range ls {
		disp.ColorPalette = d.colorPalette
		emitLine(l)
	}
	return len(ls), nil
}

// emitLine transfers one line
func emitLine(l Line) {
	disp.Out(l.Ss) // legacy implementation
}
