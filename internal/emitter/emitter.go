// SPDX-License-Identifier: MIT

// Package emitter emits the translated trice strings.
package emitter

import (
	"fmt"
	"io"
	"os"
	"slices"
	"strings"

	"github.com/rokath/trice/internal/receiver"
	"github.com/rokath/trice/pkg/msg"
)

var (
	// Verbose enables optional diagnostic output.
	Verbose bool

	// HostStamp configures line timestamps.
	// off/none = no timestamp
	// LOCmicro = local time with microseconds
	// UTCmicro = UTC time with microseconds
	// zero = fixed "2006-01-02_1504-05" timestamp (for tests)
	HostStamp string

	// Prefix is written before the first payload fragment of each line.
	Prefix string

	// Suffix is written when a line is completed. Usually empty.
	Suffix string

	// ColorPalette controls tag-prefix handling.
	// off = no color handling at all. Lower case color prefixes are not removed. Use with care.
	// none = no colors. Lower case color prefixes are removed.
	// default = color codes added (TODO: change to ANSI)
	ColorPalette string

	// IPAddr is the remote display server IP address.
	IPAddr string

	// IPPort is the remote display server TCP port.
	IPPort string

	// DisplayRemote sends lines to a remote RPC display server when true.
	DisplayRemote bool

	// TestTableMode avoids Prefix rewrites in New.
	TestTableMode bool

	// NextLine is set when a line is completed.
	// It is used by decoder table-mode output coordination.
	NextLine bool

	// Ban contains channel tags to suppress.
	Ban channelArrayFlag

	// Pick contains channel tags to display exclusively.
	Pick channelArrayFlag

	// UserLabel adds user-defined tag names at runtime.
	UserLabel ArrayFlag
)

// ArrayFlag supports repeated CLI values.
type ArrayFlag []string

// String implements flag.Value.
func (i *ArrayFlag) String() string {
	return ""
}

// Set implements flag.Value.
func (i *ArrayFlag) Set(value string) error {
	*i = append(*i, value)
	return nil
}

// channelArrayFlag stores canonical and alias tag names used by -ban/-pick.
type channelArrayFlag []string

// String implements flag.Value.
func (i *channelArrayFlag) String() string {
	return fmt.Sprintf("%v", *i)
}

// appendIfMissing appends item only when not already present.
func appendIfMissing(slice []string, item string) []string {
	for _, ele := range slice {
		if ele == item {
			return slice
		}
	}
	return append(slice, item)
}

// Set expands known tag aliases and appends unique entries.
func (i *channelArrayFlag) Set(value string) error {
	ss := strings.Split(value, ":")
	for _, s := range ss {
		cv := tagVariants(s)
		for _, c := range cv {
			*i = appendIfMissing(*i, c)
		}
	}
	return nil
}

// LineWriter is the common interface for output devices.
// The string slice `line` contains all string parts of one line including prefix and suffix.
// The last string part is without newline char and must be handled by the output device.
type LineWriter interface {
	WriteLine([]string)
}

// newLineWriter creates either a remote or local line writer.
func newLineWriter(w io.Writer) (lwD LineWriter) {
	if DisplayRemote {
		p := newRemoteDisplay(w, os.Args)
		msg.FatalOnErr(p.Err)
		lwD = p
	} else {
		lwD = newColorDisplay(w, ColorPalette)
	}
	return
}

// New creates the emitter instance and returns a string writer to be used for emitting.
func New(w io.Writer) *TriceLineComposer {
	if !TestTableMode { // do not change Prefix in TestTableMode
		setPrefix()
	}
	// lineComposer implements the io.StringWriter interface and uses the line writer provided.
	// The line composer scans the trice strings and composes lines out of them according to its properties.
	return newLineComposer(newLineWriter(w))
}

// setPrefix rewrites the default "source:" prefix to "<receiver.Port>:".
func setPrefix() {
	defaultPrefix := "source:"
	if strings.HasPrefix(Prefix, defaultPrefix) {
		Prefix = receiver.Port + ":" + Prefix[len(defaultPrefix):]
	} else if Prefix == "off" || Prefix == "none" {
		Prefix = ""
	}
}

// BanOrPickFilter returns len(b) when b passes filtering, otherwise 0.
// If Ban and Pick are nil nothing is filtered out.
// If Ban and Pick are both not nil this is a fatal error (os.Exit).
// If b starts with a known channel specifier existent in Ban 0, is returned.
// If b starts with a known channel specifier existent in Pick len of b, is returned.
func BanOrPickFilter(b []byte) (n int) {
	return banOrPickFilter(Ban, Pick, b)
}

// banOrPickFilter applies the -ban/-pick channel rules.
func banOrPickFilter(ban, pick channelArrayFlag, b []byte) int {
	if ban == nil && pick == nil {
		return len(b) // nothing to filter
	}
	msg.FatalInfoOnTrue(ban != nil && pick != nil, "switches -ban and -pick cannot be used together")
	s := string(b)
	sc := strings.SplitN(s, ":", 2) // example: "deb" -> []string{ "deb"} "deb:" -> []string{ "deb", "" }
	if ban != nil {
		if len(sc) < 2 { // no color separator
			return len(b) // nothing to filter
		}
		if slices.Contains(ban, sc[0]) {
			return 0 // filter match
		}
		return len(b) // no filter match
	} else { // Pick is set
		if len(sc) < 2 { // no color separator
			return 0 // filter out
		}
		if slices.Contains(pick, sc[0]) {
			return len(b) // filter match
		}
		return 0 // no filter match
	}
}
