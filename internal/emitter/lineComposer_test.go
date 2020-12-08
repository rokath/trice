// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// whitebox test for package emitter.
package emitter

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func Test2LineComposer(t *testing.T) {
	// prepare
	lw := newCheckDisplay()
	TimestampFormat = "off"
	Prefix = "["
	Suffix = "]"
	p := newLineComposer(lw)

	p.WriteString("Hi\r\nAll\n")
	assert.Equal(t, []string{"[ Hi ]", "[ All ]"}, lw.lines)
	lw.lines = lw.lines[:0]

	p.WriteString("Hi")
	p.WriteString("Hi\n")
	assert.Equal(t, []string{"[ HiHi ]"}, lw.lines)
	lw.lines = lw.lines[:0]

	p.WriteString("\n\nHi\n\n")
	p.WriteString("Ho\n")
	assert.Equal(t, []string{"[  ]", "[  ]", "[ Hi ]", "[  ]", "[ Ho ]"}, lw.lines)
	lw.lines = lw.lines[:0]
}

func TestLineComposer(t *testing.T) {
	// prepare
	lw := newCheckDisplay()
	TimestampFormat = "zero"
	Prefix = "<<<"
	Suffix = ">>>"
	p := newLineComposer(lw)
	p.WriteString("Hi\nAll\r\n")
	assert.Equal(t, []string{"2006-01-02_1504-05 <<< Hi >>>", "2006-01-02_1504-05 <<< All >>>"}, lw.lines)
}
