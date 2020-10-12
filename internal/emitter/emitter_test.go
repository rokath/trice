// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// whitebox test for package emitter.
package emitter

import (
	"bytes"
	"fmt"
	"testing"
	"time"

	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/internal/receiver"
	"github.com/rokath/trice/internal/translator"
	"github.com/stretchr/testify/assert"
)

// TestNewSimpleTriceInterpreterWithAnsiOff
// There is a small chance this test fails because of unexpected ordering of 'ignoring bytes' message.
// This is no error.
func TestNewSimpleTriceInterpreterWithAnsiOff(t *testing.T) {

	// rd implements the io.Reader interface needed by TriceReceiver.
	// It is the input source.
	rd := bytes.NewReader([]byte{1, 2, 3, 4, 0x89, 0xab, 0xcd, 0xef, 2, 3, 0, 0xc, 1, 3, 0, 0, 3, 3, 3, 3, 4, 4})
	// tai uses the io.Reader interface from s and implements the TriceAtomsReceiver interface.
	// It scans the raw input byte stream and decodes the trice atoms it transmits to the TriceAtomsReceiver interface.
	hardReadError := make(chan bool)
	tai := receiver.NewTricesfromBare(rd, hardReadError)

	// display lwD implements the Linewriter interface needed by lineComposer.
	// It interprets the lines written to it according to its properties.
	lwD := newCheckDisplay()
	// lwT uses the Linewriter lwD internally.
	// It provides a Linewriter.
	lwT := NewLineTransformerANSI(lwD, "off")
	// lineComposer r implements the io.StringWriter interface and uses the Linewriter provided.
	// The line composer scans the trice strings and composes lines out of them according to its properies.
	sw := NewLineComposer(lwT, "zero", "PREFIX", "")

	dict := id.NewList("none")
	dict.List = []id.Item{
		{ID: 258, FmtType: "TRICE8_2", FmtStrg: "att:Hello, %d*%d=", Created: 0, Removed: 0}, // 1, 2, 3, 4,
		{ID: 515, FmtType: "TRICE16_1", FmtStrg: "att:%d, ok", Created: 0, Removed: 0},       // 2, 3, 0, 0xc,
		{ID: 259, FmtType: "TRICE0", FmtStrg: "?\n", Created: 0, Removed: 0},                 // 1, 3, 0, 0,
		{ID: 771, FmtType: "TRICE0", FmtStrg: "msg:Yes!\n", Created: 0, Removed: 0},          // 3, 3, 3, 3,
	}

	// sti uses the TriceAtomsReceiver interface tai for reception and the io.StringWriter interface sw for writing.
	// sti collects trice atoms to a complete trice, generates the appropriate string using list and writes it to the provided io.StringWriter
	sti := translator.NewSimpleTrices(sw, dict, tai)

	lines := []string{
		"2006-01-02_1504-05 PREFIX att:Hello, 4*3=att:12, ok?",
		"2006-01-02_1504-05 PREFIX msg:Yes!",
	}
	for len(lwD.lines) < 2 {
		time.Sleep(10 * time.Millisecond)
	}
	assert.Equal(t, lines, lwD.lines)
	sti.Stop() // end of life
}

// There is a small chance this test fails because of unexpected ordering of 'ignoring bytes' message.
// This is no error.
func TestNewSimpleTriceInterpreterWithAnsiNone(t *testing.T) {
	// rd implements the io.Reader interface needed by TriceReceiver.
	// It is the input source.
	rd := bytes.NewReader([]byte{1, 1, 1, 1, 0x89, 0xab, 0xcd, 0xef, 2, 4, 0, 2, 3, 3, 3, 3, 4, 4})
	// tai uses the io.Reader interface from s and implements the TriceAtomsReceiver interface.
	// It scans the raw input byte stream and decodes the trice atoms it transmits to the TriceAtomsReceiver interface.
	hardReadError := make(chan bool)
	tai := receiver.NewTricesfromBare(rd, hardReadError)

	// display lwD implements the Linewriter interface needed by lineComposer.
	// It interprets the lines written to it according to its properties.
	lwD := newCheckDisplay()
	// lwT uses the Linewriter lwD internally.
	// It provides a Linewriter.
	lwT := NewLineTransformerANSI(lwD, "none")
	// lineComposer r implements the io.StringWriter interface and uses the Linewriter provided.
	// The line composer scans the trice strings and composes lines out of them according to its properies.
	sw := NewLineComposer(lwT, "zero", "PREFIX", "")

	dict := id.NewList("none")
	dict.List = []id.Item{
		{ID: 257, FmtType: "TRICE8_2", FmtStrg: "att:Hello, %d+%d=", Created: 0, Removed: 0},
		{ID: 516, FmtType: "TRICE16_1", FmtStrg: "att:%d, ok?\n", Created: 0, Removed: 0},
		{ID: 771, FmtType: "TRICE0", FmtStrg: "msg:Yes!\n", Created: 0, Removed: 0},
		{ID: 5654, FmtType: "TRICE0", FmtStrg: "%s", Created: 0, Removed: 0},
	}

	// sti uses the TriceAtomsReceiver interface tai for reception and the io.StringWriter interface (r) for writing.
	// sti collects trice atoms to a complete trice, generates the appropriate string with list and writes it to the provided io.StringWriter
	sti := translator.NewSimpleTrices(sw, dict, tai)

	lines := []string{
		"2006-01-02_1504-05 PREFIX Hello, 1+1=2, ok?",
		"2006-01-02_1504-05 PREFIX Yes!",
	}
	for len(lwD.lines) < 2 {
		time.Sleep(100 * time.Millisecond)
	}
	assert.Equal(t, lines, lwD.lines)
	// lwD.checkLines(t, lines)
	sti.Stop() // end of life
}

// There is a small chance this test fails because of unexpected ordering of 'ignoring bytes' message.
// This is no error.
func TestNewSimpleTriceInterpreterResync(t *testing.T) {
	// rd implements the io.Reader interface needed by TriceReceiver.
	// It is the input source.
	rd := bytes.NewReader([]byte{'j', 'a', 'r', 0x89, 0xab, 0xcd, 0xef, 4, 4})
	// tai uses the io.Reader interface from s and implements the TriceAtomsReceiver interface.
	// It scans the raw input byte stream and decodes the trice atoms it transmits to the TriceAtomsReceiver interface.
	hardReadError := make(chan bool)
	tai := receiver.NewTricesfromBare(rd, hardReadError)

	// display lwD implements the Linewriter interface needed by lineComposer.
	// It interprets the lines written to it according to its properties.
	lwD := newCheckDisplay()
	// lwT uses the Linewriter p internally.
	// It provides a Linewriter.
	lwT := NewLineTransformerANSI(lwD, "none")
	// lineComposer r implements the io.StringWriter interface and uses the Linewriter provided.
	// The line composer scans the trice strings and composes lines out of them according to its properies.
	sw := NewLineComposer(lwT, "zero", "PREFIX", "")

	dict := id.NewList("none")

	// sti uses the TriceAtomsReceiver interface tai for reception and the io.StringWriter interface (r) for writing.
	// sti collects trice atoms to a complete trice, generates the appropriate string with list and writes it to the provided io.StringWriter
	translator.Verbose = true
	sti := translator.NewSimpleTrices(sw, dict, tai)

	for len(lwD.lines) < 1 {
		time.Sleep(100 * time.Millisecond)
	}
	assert.Equal(t, []string{"2006-01-02_1504-05 PREFIX Found 7 ignored byte(s): [106 97 114 137 171 205 239]"}, lwD.lines)
	sti.Stop() // end of life
}

func TestScCheck(t *testing.T) {
	// display lwD implements the Linewriter interface needed by lineComposer.
	// It interprets the lines written to it according to its properties.
	lwD := newCheckDisplay()
	// lwT uses the Linewriter p internally.
	// It provides a Linewriter.
	lwT := NewLineTransformerANSI(lwD, "none")
	// lineComposer r implements the io.StringWriter interface and uses the Linewriter provided.
	// The line composer scans the trice strings and composes lines out of them according to its properies.
	sw := NewLineComposer(lwT, "zero", "PREFIX", "")
	fmt.Print(sw)
}
