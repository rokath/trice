// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package emitter does...
package emitter

// lineWriter is the common interface for output devices.
// string slice `line` contains all string parts of one line.
// The last string part is without newline char and must be handled by the output device.
type lineWriter interface {
	writeLine([]string)
}
