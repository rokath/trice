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
)

// TestNewSimpleTriceInterpreterWithAnsiOff
// There is a small chance this test fails because of unexpected ordering of 'ignoring bytes' message.
// This is no error.
func TestNewSimpleTriceInterpreterWithAnsiOff(t *testing.T) {

	// rd implements the io.Reader interface needed by TriceReceiver.
	// It is the input source.
	rd := bytes.NewReader([]byte{1, 1, 1, 1, 0x16, 0x16, 0x16, 0x16, 2, 2, 2, 0, 3, 2, 0, 0, 3, 3, 3, 3, 4, 4})
	// tai uses the io.Reader interface from s and implements the TriceAtomsReceiver interface.
	// It scans the raw input byte stream and decodes the trice atoms it transmits to the TriceAtomsReceiver interface.
	tai := receiver.NewTricesfromBare(rd)

	// display lwD implements the Linewriter interface needed by lineComposer.
	// It interprets the lines written to it according to its properties.
	lwD := newCheckDisplay()
	// lwT uses the Linewriter lwD internally.
	// It provides a Linewriter.
	lwT := NewLineTransformerANSI(lwD, "off")
	// lineComposer r implements the io.StringWriter interface and uses the Linewriter provided.
	// The line composer scans the trice strings and composes lines out of them according to its properies.
	sw := NewLineComposer(lwT, "zero", "PREFIX", "")

	var list id.ListT = []id.Item{
		{ID: 257, FmtType: "TRICE8_2", FmtStrg: "att:Hello, %d+%d=", Created: 0, Removed: 0},
		{ID: 514, FmtType: "TRICE16_1", FmtStrg: "att:%d, ok", Created: 0, Removed: 0},
		{ID: 515, FmtType: "TRICE0", FmtStrg: "?\n", Created: 0, Removed: 0},
		{ID: 771, FmtType: "TRICE0", FmtStrg: "msg:Yes!\n", Created: 0, Removed: 0},
		{ID: 5654, FmtType: "TRICE0", FmtStrg: "%s", Created: 0, Removed: 0},
	}

	// sti uses the TriceAtomsReceiver interface tai for reception and the io.StringWriter interface sw for writing.
	// sti collects trice atoms to a complete trice, generates the appropriate string using list and writes it to the provided io.StringWriter
	sti := translator.NewSimpleTrices(sw, list, tai)

	lines := []string{
		"2006-01-02_1504-05 PREFIX att:Hello, 1+1=att:2, ok?",
		"2006-01-02_1504-05 PREFIX msg:Yes!",
	}
	for len(lwD.lines) < 2 {
		time.Sleep(10 * time.Millisecond)
	}
	lwD.checkLines(t, lines)
	sti.Stop() // end of life
}

// There is a small chance this test fails because of unexpected ordering of 'ignoring bytes' message.
// This is no error.
func TestNewSimpleTriceInterpreterWithAnsiNone(t *testing.T) {
	// rd implements the io.Reader interface needed by TriceReceiver.
	// It is the input source.
	rd := bytes.NewReader([]byte{1, 1, 1, 1, 0x16, 0x16, 0x16, 0x16, 2, 2, 2, 0, 3, 3, 3, 3, 4, 4})
	// tai uses the io.Reader interface from s and implements the TriceAtomsReceiver interface.
	// It scans the raw input byte stream and decodes the trice atoms it transmits to the TriceAtomsReceiver interface.
	tai := receiver.NewTricesfromBare(rd)

	// display lwD implements the Linewriter interface needed by lineComposer.
	// It interprets the lines written to it according to its properties.
	lwD := newCheckDisplay()
	// lwT uses the Linewriter lwD internally.
	// It provides a Linewriter.
	lwT := NewLineTransformerANSI(lwD, "none")
	// lineComposer r implements the io.StringWriter interface and uses the Linewriter provided.
	// The line composer scans the trice strings and composes lines out of them according to its properies.
	sw := NewLineComposer(lwT, "zero", "PREFIX", "")

	var list id.ListT = []id.Item{
		{ID: 257, FmtType: "TRICE8_2", FmtStrg: "att:Hello, %d+%d=", Created: 0, Removed: 0},
		{ID: 514, FmtType: "TRICE16_1", FmtStrg: "att:%d, ok?\n", Created: 0, Removed: 0},
		{ID: 771, FmtType: "TRICE0", FmtStrg: "msg:Yes!\n", Created: 0, Removed: 0},
		{ID: 5654, FmtType: "TRICE0", FmtStrg: "%s", Created: 0, Removed: 0},
	}

	// sti uses the TriceAtomsReceiver interface tai for reception and the io.StringWriter interface (r) for writing.
	// sti collects trice atoms to a complete trice, generates the appropriate string with list and writes it to the provided io.StringWriter
	sti := translator.NewSimpleTrices(sw, list, tai)

	lines := []string{
		"2006-01-02_1504-05 PREFIX Hello, 1+1=2, ok?",
		"2006-01-02_1504-05 PREFIX Yes!",
	}
	for len(lwD.lines) < 2 {
		time.Sleep(100 * time.Millisecond)
	}
	lwD.checkLines(t, lines)
	sti.Stop() // end of life
}

// There is a small chance this test fails because of unexpected ordering of 'ignoring bytes' message.
// This is no error.
func TestNewSimpleTriceInterpreterResync(t *testing.T) {
	// rd implements the io.Reader interface needed by TriceReceiver.
	// It is the input source.
	rd := bytes.NewReader([]byte{'j', 'a', 'r', 0x16, 0x16, 0x16, 0x16, 4, 4})
	// tai uses the io.Reader interface from s and implements the TriceAtomsReceiver interface.
	// It scans the raw input byte stream and decodes the trice atoms it transmits to the TriceAtomsReceiver interface.
	tai := receiver.NewTricesfromBare(rd)

	// display lwD implements the Linewriter interface needed by lineComposer.
	// It interprets the lines written to it according to its properties.
	lwD := newCheckDisplay()
	// lwT uses the Linewriter p internally.
	// It provides a Linewriter.
	lwT := NewLineTransformerANSI(lwD, "none")
	// lineComposer r implements the io.StringWriter interface and uses the Linewriter provided.
	// The line composer scans the trice strings and composes lines out of them according to its properies.
	sw := NewLineComposer(lwT, "zero", "PREFIX", "")

	var list id.ListT = []id.Item{
		{ID: 257, FmtType: "TRICE8_2", FmtStrg: "att:Hello, %d+%d=", Created: 0, Removed: 0},
		{ID: 514, FmtType: "TRICE16_1", FmtStrg: "att:%d, ok?\n", Created: 0, Removed: 0},
		{ID: 771, FmtType: "TRICE0", FmtStrg: "msg:Yes!\n", Created: 0, Removed: 0},
		{ID: 5654, FmtType: "TRICE0", FmtStrg: "%s", Created: 0, Removed: 0},
	}

	// sti uses the TriceAtomsReceiver interface tai for reception and the io.StringWriter interface (r) for writing.
	// sti collects trice atoms to a complete trice, generates the appropriate string with list and writes it to the provided io.StringWriter
	sti := translator.NewSimpleTrices(sw, list, tai)

	lines := []string{
		"2006-01-02_1504-05 PREFIX WARNING:ignoring bytes: [106 97 114]",
	}
	for len(lwD.lines) < 1 {
		time.Sleep(100 * time.Millisecond)
	}
	lwD.checkLines(t, lines)
	sti.Stop() // end of life
}

func _TestScCheck(t *testing.T) {
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
