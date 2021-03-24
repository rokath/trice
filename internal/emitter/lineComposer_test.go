// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// whitebox test for package emitter.
package emitter

import (
	"strings"
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
	msg.OnErr(err)
	assert.Equal(t, []string{"[Hi]", "[All]"}, lw.lines)
	lw.lines = lw.lines[:0]

	_, err = p.WriteString("Hi")
	msg.OnErr(err)
	_, err = p.WriteString("Hi\n")
	msg.OnErr(err)
	assert.Equal(t, []string{"[HiHi]"}, lw.lines)
	lw.lines = lw.lines[:0]

	_, err = p.WriteString("\n\nHi\n\n")
	msg.OnErr(err)
	_, err = p.WriteString("Ho\n")
	msg.OnErr(err)
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
	msg.OnErr(err)
	assert.Equal(t, []string{"2006-01-02_1504-05 <<<Hi>>>", "2006-01-02_1504-05 <<<All>>>"}, lw.lines)
}

// checkDisplay is an object used for testing.
// It implements the Linewriter interface.
type checkDisplay struct {
	lines []string
}

// newCheckDisplay creates a Display. It provides a Linewriter.
func newCheckDisplay() *checkDisplay {
	p := &checkDisplay{}
	return p
}

// writeLine is the implemented Linewriter interface for checkDisplay.
// It appends written lines to the internal data.
func (p *checkDisplay) writeLine(line []string) {
	s := strings.Join(line, "")
	p.lines = append(p.lines, s)
}
