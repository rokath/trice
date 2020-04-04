// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package emit

import (
	"strings"
	"sync"
	"time"

	"github.com/fatih/color"
)

// timestamp returns local time as string according var TimeStampFormat
func timestamp() string {
	var s string
	switch TimeStampFormat {
	case "LOCmicro":
		s = time.Now().Format(time.StampMicro) + "  "
	case "UTCmicro":
		s = "UTC " + time.Now().UTC().Format(time.StampMicro) + "  "
	case "off":
		s = ""
	}
	return s
}

// LineCollect collects s into an internal line substring slice
// When s ends with a newline it is trimmed and the slice goes to Visualize and is discarded afterwards
func LineCollect(s string) {
	s = trimBackslashes(s)
	var ss []string
	a := func(su string) { // this closure is needed to treat ss as surviving slice
		ss = append(ss, su)
	}
	if 0 == len(ss) {
		a(Prefix)
		a(timestamp())
	}
	if !strings.HasSuffix(s, "\n") {
		a(s)
		return
	}
	s = strings.TrimSuffix(s, "\n")
	a(s)
	a(Postfix)
	Visualize(ss)
	ss = nil
}

var mux sync.Mutex

// visualize displays ss and sets color.
// ss is a slice containing substring parts of one line.
// Each substring inside ss is result of Trice or contains prefix,
// timestamp or postfix and can have its own color prefix.
// The last substring inside the slice is definitly the last in
// the line and has already trimmed its newline.
// Linebreaks inside the substrings are not handled separately (yet).
func visualize(ss []string) error {
	var c *color.Color
	var line string

	mux.Lock()
	for _, s := range ss {
		c, s = colorChannel(s)
		line += c.Sprint(s)
	}
	o := color.NoColor
	color.NoColor = true
	c.Print(line)
	color.NoColor = o
	mux.Unlock()
	return nil
}
