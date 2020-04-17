// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package disp is for dispatching and displaying trice log lines
// This file contains the common code.
package disp

import (
	"fmt"

	"github.com/fatih/color"
)

var (
	// Out is an exported function pointer, which can be redirected for example to a client call
	Out = out
)

// out displays ss and sets color.
// ss is a slice containing substring parts of one line.
// Each substring inside ss is result of Trice or contains prefix,
// timestamp or postfix and can have its own color prefix.
// The last substring inside the slice is definitly the last in
// the line and has already trimmed its newline.
// Linebreaks inside the substrings are not handled separately (yet).
func out(ss []string) error {
	var c *color.Color
	var line string

	mux.Lock()
	for _, s := range ss {
		c, s = colorChannel(s)
		if true == color.NoColor {
			line += fmt.Sprint(s)
		} else {
			line += c.Sprint(s)
		}
	}
	o := color.NoColor
	color.NoColor = true
	c.Print(line) // here better use simply fmt.Print, but then the io.Writer to os.file assignment issue must be solved
	color.NoColor = o
	mux.Unlock()
	return nil
}
