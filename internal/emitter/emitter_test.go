// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// whitebox test for package emitter.
package emitter

import (
	"bytes"
	"fmt"
	"os"
	"testing"

	"github.com/rokath/trice/internal/triceemit"
	"github.com/rokath/trice/pkg/cage"
	"github.com/rokath/trice/pkg/lib"
)

func TestAA(t *testing.T) {
	// s implements the io.Reader interface needed by TriceReceiver.
	// It is the input source.
	s := bytes.NewReader([]byte{'j', 'a', 'r', 1, 1, 1, 1, 0x16, 0x16, 0x16, 0x16, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4})
	// x uses the io.Reader interface from s and implements the TriceAtomsReceiver interface.
	// It scans the raw input byte stream and decodes the trice atoms it transmits to the TriceAtomsReceiver interface.
	x := triceemit.NewTriceReceiverfromBare(s)

	// display p implements the lineWriter interface needed by lineComposer.
	// It interprets the lines written to it according to its properties.
	p := newLocalDisplay()
	// q uses the lineWriter p internally.
	// It provides a lineWriter.
	q := newLineTranslator(p, "defaultColor")
	// lineComposer r implements the io.StringWriter interface and uses the lineWriter provided.
	// The line composer scans the trice strings and composes lines out of them according to its properies.
	r := newLineComposer("TIMESTAMP", "PREFIX", "SUFFIX", q)

	// u uses the TriceAtomsReceiver interface (x) for reception and the io.StringWriter interface (r) for writing.
	// u collects trice atoms to a complete trice, generates the appropriate string from it and writes it to the provided io.StringWriter
	u := triceemit.NewSimpleTriceInterpreter(r, x)

	fmt.Print(u)
	u.Stop() // end of life
}

func TestLocal(t *testing.T) {
	// prepare
	afn := "testdata/act.log"
	efn := "testdata/exp.log"
	os.Remove(afn)
	c := cage.Start(afn)

	// do
	p := newLocalDisplay()
	l1 := []string{"This is ", "the 1st ", "line"}
	l2 := []string{"This is ", "the 2nd ", "line"}
	p.writeLine(l1)
	p.writeLine(l2)

	// check output
	cage.Stop(c)
	lib.EqualTextFiles(t, afn, efn)
	os.Remove(afn)
}

func TestRemote(t *testing.T) {
	// prepare
	afn := "testdata/act.log"
	efn := "testdata/exp.log"
	os.Remove(afn)

	// do
	p := newRemoteDisplay("localhost", "65497", "trice.exe", afn)
	l1 := []string{"This is ", "the 1st ", "line"}
	l2 := []string{"This is ", "the 2nd ", "line"}
	p.writeLine(l1)
	p.writeLine(l2)

	// check output
	lib.EqualTextFiles(t, afn, efn)
	os.Remove(afn)
}
