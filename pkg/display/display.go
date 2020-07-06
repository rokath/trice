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
// Known lowercase color specifiers are removed after prefixed with an ANSI code for a color.
// The last string is postfixed with an ANSI code for default terminal color.
// Finally a newline code `\n` is appended before the line is sent to the terminal.
package display

import "github.com/rokath/trice/internal/disp"

// Line is the data struct written to Display.
// It contains segments which are exist as string slice.
type Line struct {
	Segments []string
}

// // IF is the provided display interface.
// type IF interface {
// 	Writer([]Line) (int, error)
// }

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
// It writes lines to display using the display internals settings defined on display creation with New.
func (d *T) Write(lines []Line) (int, error) {
	for i, line := range lines {
		disp.ColorPalette = d.colorPalette
		err := emit(line)
		if nil != err {
			return i, err
		}
	}
	return len(lines), nil
}

// emit transfers one line to display
//
// todo: replace disp legacy implementation
func emit(l Line) error {
	return disp.Out(l.Segments)
}
