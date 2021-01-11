// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// whitebox test for package emitter.
package emitter

import (
	"testing"

	"github.com/rokath/trice/pkg/msg"

	"github.com/stretchr/testify/assert"
)

func Test2LineComposer(t *testing.T) {
	// prepare
	lw := newCheckDisplay()
	TimestampFormat = "off"
	Prefix = "["
	Suffix = "]"
	p := newLineComposer(lw)

	_, err := p.WriteString("Hi\r\nAll\n")
	msg.InfoOnErr("", err)
	assert.Equal(t, []string{"[Hi]", "[All]"}, lw.lines)
	lw.lines = lw.lines[:0]

	_, err = p.WriteString("Hi")
	msg.InfoOnErr("", err)
	_, err = p.WriteString("Hi\n")
	msg.InfoOnErr("", err)
	assert.Equal(t, []string{"[HiHi]"}, lw.lines)
	lw.lines = lw.lines[:0]

	_, err = p.WriteString("\n\nHi\n\n")
	msg.InfoOnErr("", err)
	_, err = p.WriteString("Ho\n")
	msg.InfoOnErr("", err)
	assert.Equal(t, []string{"[]", "[]", "[Hi]", "[]", "[Ho]"}, lw.lines)
	lw.lines = lw.lines[:0]
}

func TestLineComposer(t *testing.T) {
	// prepare
	lw := newCheckDisplay()
	TimestampFormat = "zero"
	Prefix = "<<<"
	Suffix = ">>>"
	p := newLineComposer(lw)
	_, err := p.WriteString("Hi\nAll\r\n")
	msg.InfoOnErr("", err)
	assert.Equal(t, []string{"2006-01-02_1504-05 <<<Hi>>>", "2006-01-02_1504-05 <<<All>>>"}, lw.lines)
}
