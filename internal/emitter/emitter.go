// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package emitter emits the translated trice strings.
package emitter

import (
	"os"

	"github.com/rokath/trice/pkg/cage"
)

var (
	// Verbose gives mor information on output if set. The value is injected from main packages.
	Verbose bool

	// TimestampFormat is used tor line timestamps.
	// off = no timestamp
	// none = no timestamp
	// LOCmicro = local time with microseconds
	// UTCmicro = universal time with microseconds
	// zero = fixed "2006-01-02_1504-05" timestamp (for tests)
	TimestampFormat string

	// Prefix starts lines. It follows line timestamp, if any.
	Prefix string

	// Suffix lollows lines. Usually empty.
	Suffix string

	// ColorPalette determines the way color is handled.
	// off = no color handling at all. Lower case color prefixes are not removed. Use with care.
	// none = no colors. Lower case color prefixes are removed.
	// default = color codes added (TODO: change to ANSI)
	ColorPalette string

	// IPAddr ist the remote display IP address.
	IPAddr string

	// IPPort ist the remote display port number.
	IPPort string
)

// LineWriter is the common interface for output devices.
// The string slice `line` contains all string parts of one line including prefix and suffix.
// The last string part is without newline char and must be handled by the output device.
type LineWriter interface {
	writeLine([]string)
}

// NewLineWriter provides a LineWriter which can be a remote Display or the local console.
func NewLineWriter(displayRemote, autostart bool) (lwD LineWriter) {
	if true == displayRemote {
		var p *RemoteDisplay
		if true == autostart {
			p = NewRemoteDisplay(os.Args[0], "-logfile "+cage.Name)
		} else {
			p = NewRemoteDisplay()
		}
		p.ErrorFatal()
		lwD = p
		//keybcmd.ReadInput()
	} else {
		// NewColorDisplay creates a ColorlDisplay. It provides a Linewriter.
		// It uses internally a local display combined with a line transformer.
		lwD = NewColorDisplay(ColorPalette)
	}
	return
}
