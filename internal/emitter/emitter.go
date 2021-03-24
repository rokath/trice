// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package emitter emits the translated trice strings.
package emitter

import (
	"runtime"

	"github.com/rokath/trice/internal/receiver"
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

	// DisplayRemote if set, sends trice lines over TCP.
	DisplayRemote bool

	// Autostart if set, starts an additional trice instance as displayserver.
	Autostart bool

	// TestTableMode is set externally to avoid Prefix overwrite
	TestTableMode bool

	// NextLine is set true as help for decoder.TestTableMode, where it is clreared at line start.
	NextLine bool
)

// LineWriter is the common interface for output devices.
// The string slice `line` contains all string parts of one line including prefix and suffix.
// The last string part is without newline char and must be handled by the output device.
type LineWriter interface {
	writeLine([]string)
}

// baseName returns basic filename of program without extension
func baseName() string {
	// a0 := os.Args[0]
	// b0 := filepath.Base(a0)
	// e0 := filepath.Ext(a0)
	// s := strings.TrimSuffix(b0, e0)
	// return strings.TrimSuffix(s, ".test") // for Example tests only
	if runtime.GOOS == "windows" {
		return "trice.exe"
	}
	return "trice"
}

// newLineWriter provides a LineWriter which can be a remote Display or the local console.
func newLineWriter() (lwD LineWriter) {
	if true == DisplayRemote {
		var p *RemoteDisplay
		if true == Autostart {
			p = NewRemoteDisplay(baseName(), "-logfile "+cage.Name)
		} else {
			p = NewRemoteDisplay()
		}
		p.ErrorFatal()
		lwD = p
		// keybcmd.ReadInput()
	} else {
		lwD = NewColorDisplay(ColorPalette)
	}
	return
}

// New creates the emitter instance and returns a string writer to be used for emitting.
func New() *TriceLineComposer {
	if !DisplayRemote {
		cage.Enable()
		defer cage.Disable()
	}
	if !TestTableMode { // do not change Prefix in TestTableMode
		SetPrefix()
	}
	// lineComposer implements the io.StringWriter interface and uses the line writer provided.
	// The line composer scans the trice strings and composes lines out of them according to its properties.
	return newLineComposer(newLineWriter())
}

// SetPrefix changes "source:" to e.g., "JLINK:".
// to do: better implementation for example with strings.Split
func SetPrefix() {
	port := receiver.Port
	switch Prefix {
	case "source:":
		Prefix = port + ":"
	case "source: ":
		Prefix = port + ": "
	case "source:  ":
		Prefix = port + ":  "
	case "source:   ":
		Prefix = port + ":   "
	case "source:    ":
		Prefix = port + ":    "
	case "source:     ":
		Prefix = port + ":     "
	case "source:      ":
		Prefix = port + ":      "
	case "source:       ":
		Prefix = port + ":       "
	case "source:        ":
		Prefix = port + ":        "
	case "source:         ":
		Prefix = port + ":         "
	case "source:          ":
		Prefix = port + ":          "
	case "source:           ":
		Prefix = port + ":           "
	case "source:            ":
		Prefix = port + ":            "
	case "off", "none":
		Prefix = ""
	}
}
