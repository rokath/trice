// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package emitter

import (
	"strings"
	"time"
)

// SyncPacketPattern is used if a sync packet arrives
const SyncPacketPattern = "inf:[TRICE_SYNC_PACKET 0x89abcdef]"

// TriceLineComposer collects all partial strings forming one line.
type TriceLineComposer struct {
	lw              lineWriter // internal interface
	timestampFormat string
	prefix          string
	suffix          string
	Line            []string // line collector
	err             error
}

// newLineComposer constructs log lines according to these rules:...
// It provides an io.StringWriter interface which is used for the reception of (trice) strings.
// It uses lw for writing the generated lines.
func newLineComposer(lw lineWriter) *TriceLineComposer {
	p := &TriceLineComposer{lw, TimestampFormat, Prefix, Suffix, make([]string, 0, 4096), nil} // not more than 4096 strings per line expected
	return p
}

// timestamp returns local time as string according var p.timeStampFormat
func (p *TriceLineComposer) timestamp() string {
	var s string
	switch p.timestampFormat {
	case "LOCmicro":
		s = time.Now().Format(time.StampMicro) + "  "
	case "UTCmicro":
		s = "UTC " + time.Now().UTC().Format(time.StampMicro) + "  "
	case "off", "none":
		s = ""
	case "zero":
		s = "2006-01-02_1504-05 "
	default:
		s = p.timestampFormat + " "
	}
	return s
}

// Write treats received buffer as a string.
func (p *TriceLineComposer) Write(b []byte) (n int, err error) {
	s := string(b)
	return p.WriteString(s)
}

// WriteString implements the io.StringWriter interface. The triceLineComposer can use it.
// WriteString uses the internal line writer p.lw for writing out full lines.
// If s is empty, WriteString returns 0, nil.
// If p.line is empty, the actual timestamp and the prefix are added to p.line.
// If s is without newline it is added to p.line and WriteString returns.
// If s ends with newline it is added to p.line and also the suffix is added to p.line and pline is written to p.lw.
// If s contains several newlines it is split there and the substrings are handled accordingly.
// That means it writes internally a separate line for each substring (in s) ending with a newline.
func (p *TriceLineComposer) WriteString(s string) (n int, err error) {
	n = len(s)
	if n == 0 {
		return
	}
	var emptyLine bool
	s0 := strings.ReplaceAll(s, "\\r\\n", "\n")
	s1 := strings.ReplaceAll(s0, "\\n", "\n")
	sn := strings.ReplaceAll(s1, "\r\n", "\n")
	ss := strings.Split(sn, "\n")

	// play ground results:
	// fmt.Printf("%q\n", strings.Split("a", "\n"))           // ["s"]                // 0 new line
	// fmt.Printf("%q\n", strings.Split("a\n", "\n"))         // ["a" ""]             // 1 new line
	// fmt.Printf("%q\n", strings.Split("", "\n"))            // [""]                 // 0 new line
	// fmt.Printf("%q\n", strings.Split("\n", "\n"))          // ["" ""]              // 1 new lne
	// fmt.Printf("%q\n", strings.Split("\n\n", "\n"))        // ["" "" ""]           // 2 new lines
	// fmt.Printf("%q\n", strings.Split("a b c", "\n"))       // ["a b c"]            // 0 new line
	// fmt.Printf("%q\n", strings.Split("a\nb\nc\n", "\n"))   // ["a" "b" "c" ""]     // 3 new lines
	// fmt.Printf("%q\n", strings.Split("a\n\nb\nc\n", "\n")) // ["a" "" "b" "c" ""]  // 4 new lines
	// fmt.Printf("%q\n", strings.Split("a\nb\nc", "\n"))     // ["a" "b" "c"]        // 2 new lines
	// derived rule: len(ss)-1 is count of new lines
	lineEndCount := len(ss) - 1

	// One string with several newlines gets the identical timestamp.
	// If a string was already started and gets completed with a following WriteString call,
	// it keeps its original timestamp, but if following lines inside s they get a new timestamp.
	ts := p.timestamp()
	for _, sx := range ss {
		if len(p.Line) == 0 && 0 < lineEndCount { // start new line && and complete line
			p.Line = append(p.Line, ts, p.prefix, sx, p.suffix)
			p.completeLine()
			lineEndCount--
		} else if len(p.Line) == 0 && lineEndCount == 0 { // start new line
			p.Line = append(p.Line, ts, p.prefix, sx)
			if len(sx) == 0 { // A new line with an empty string was started.
				// This could cause unwanted timestamp offsets if the next line is significantly delayed.
				emptyLine = true
			}
		} else if 0 < len(p.Line) && 0 < lineEndCount { // complete line
			p.Line = append(p.Line, sx, p.suffix)
			p.completeLine()
			lineEndCount--
		} else if 0 < len(p.Line) && 0 == lineEndCount { // extend line
			p.Line = append(p.Line, sx)
		}
	}
	if emptyLine { // clean up
		p.Line = p.Line[:0]
	}
	return
}

func (p *TriceLineComposer) completeLine() {
	p.lw.writeLine(p.Line)
	p.Line = p.Line[:0]
	NextLine = true
}
