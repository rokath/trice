// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// whitebox test for package emitter.
package emitter

import (
	"testing"
)

func Test2LineComposer(t *testing.T) {
	// prepare
	lw := newCheckDisplay()
	pf := "["
	sf := "]"
	p := newLineComposer(lw, "off", pf, sf)

	p.WriteString("Hi\r\nAll\n")
	lw.checkLines(t, []string{"[ Hi ]", "[ All ]"})
	lw.lines = lw.lines[:0]

	p.WriteString("Hi")
	p.WriteString("Hi\n")
	lw.checkLines(t, []string{"[ HiHi ]"})
	lw.lines = lw.lines[:0]

	p.WriteString("\n\nHi\n\n")
	p.WriteString("Ho\n")
	lw.checkLines(t, []string{"[  ]", "[  ]", "[ Hi ]", "[  ]", "[ Ho ]"})
	lw.lines = lw.lines[:0]
}

func TestLineComposer(t *testing.T) {
	// prepare
	lw := newCheckDisplay()
	p := newLineComposer(lw, "zero", "<<<", ">>>")
	p.WriteString("Hi\nAll\r\n")

	lw.checkLines(t, []string{"2006-01-02_1504-05 <<< Hi >>>", "2006-01-02_1504-05 <<< All >>>"})
}
