// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// whitebox test for package emitter.
package emitter

import (
	"bytes"
	"fmt"
	"strings"
	"testing"
	"time"

	"github.com/rokath/trice/internal/id"

	"github.com/rokath/trice/internal/triceemit"
)

// checkDisplay is an object used for testing.
// It implements the lineWriter interface.
type checkDisplay struct {
	s []string
}

// newCheckDisplay creates a Display. It provides a lineWriter.
func newCheckDisplay() *checkDisplay {
	p := &checkDisplay{}
	return p
}

// writeLine is the implemented lineWriter interface for checkDisplay.
// It appends written lines to the internal data.
func (p *checkDisplay) writeLine(line []string) {
	s := strings.Join(line, "")
	p.s = append(p.s, s)
}

// equalStringSlices returns true when a == b
func equalStringSlices(a, b []string) bool {
	if len(a) != len(b) {
		fmt.Println(len(a), len(b))
		return false
	}
	for i := range a {
		if a[i] != b[i] {
			fmt.Println(a[i])
			fmt.Println(b[i])
			return false
		}
	}
	return true
}

func (p *checkDisplay) checkLines(t *testing.T, lines []string) {
	//if false == reflect.DeepEqual(p.s, lines) {
	if false == equalStringSlices(p.s, lines) {
		t.Fail()
	}
}

func TestAA(t *testing.T) {
	// s implements the io.Reader interface needed by TriceReceiver.
	// It is the input source.
	s := bytes.NewReader([]byte{'j', 'a', 'r', 1, 1, 1, 1, 0x16, 0x16, 0x16, 0x16, 2, 2, 2, 0, 3, 3, 3, 3, 4, 4})
	// x uses the io.Reader interface from s and implements the TriceAtomsReceiver interface.
	// It scans the raw input byte stream and decodes the trice atoms it transmits to the TriceAtomsReceiver interface.
	x := triceemit.NewTriceReceiverfromBare(s)

	// display p implements the lineWriter interface needed by lineComposer.
	// It interprets the lines written to it according to its properties.
	p := newCheckDisplay()
	// q uses the lineWriter p internally.
	// It provides a lineWriter.
	q := newLineTranslatorANSI(p, "none")
	// lineComposer r implements the io.StringWriter interface and uses the lineWriter provided.
	// The line composer scans the trice strings and composes lines out of them according to its properies.
	r := newLineComposer(q, "zero", "PREFIX", "")

	var l id.ListT = []id.Item{
		{ID: 257, FmtType: "TRICE8_2", FmtStrg: "att:Hello, %d+%d=", Created: 0, Removed: 0},
		{ID: 514, FmtType: "TRICE16_1", FmtStrg: "att:%d, ok?\n", Created: 0, Removed: 0},
		{ID: 771, FmtType: "TRICE0", FmtStrg: "msg:Yes!\n", Created: 0, Removed: 0},
		{ID: 5654, FmtType: "TRICE0", FmtStrg: "%s", Created: 0, Removed: 0},
	}

	// u uses the TriceAtomsReceiver interface (x) for reception and the io.StringWriter interface (r) for writing.
	// u collects trice atoms to a complete trice, generates the appropriate string from it and writes it to the provided io.StringWriter
	u := triceemit.NewSimpleTriceInterpreter(r, l, x)

	lines := []string{
		"Hello, 1+1=2, ok?\n",
		"Yes!\n",
	}
	for len(p.s) < 3 {
		time.Sleep(10 * time.Millisecond)
	}
	p.checkLines(t, lines)
	//fmt.Print(u)
	u.Stop() // end of life
	//t.Fail()
}
