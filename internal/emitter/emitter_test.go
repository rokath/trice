// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// whitebox test for package emitter.
package emitter

import (
	"bytes"
	"fmt"
	"os"
	"strings"
	"testing"
	"time"

	"github.com/rokath/trice/internal/triceemit"
	"github.com/rokath/trice/pkg/cage"
	"github.com/rokath/trice/pkg/lib"
)

// checkDisplay is an object used for displaying.
// checkDisplay implements the lineWriter interface
type checkDisplay struct {
	s []string
}

// newCheckDisplay creates a Display. It provides a lineWriter.
func newChecklDisplay() *checkDisplay {
	p := &checkDisplay{}
	return p
}

// writeLine is the implemented lineWriter interface for checkDisplay.
func (p *checkDisplay) writeLine(line []string) {
	s := strings.Join(line, "")
	p.s = append(p.s, s)
}

func compare(a, b []string) bool {
	if len(a) != len(b) {
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

func (p *checkDisplay) checkLine(t *testing.T, lines []string) {
	//if false == reflect.DeepEqual(p.s, lines) {
	if false == compare(p.s, lines) {
		t.Fail()
	}
}

func Test2LineComposer(t *testing.T) {
	// prepare
	lw := newChecklDisplay()
	pf := "[ "
	sf := " ]"
	p := newLineComposer(lw, "off", pf, sf)

	p.WriteString("Hi\r\nAll\n")
	lw.checkLine(t, []string{"[ Hi ]", "[ All ]"})
	lw.s = lw.s[:0]

	p.WriteString("Hi")
	p.WriteString("Hi\n")
	lw.checkLine(t, []string{"[ HiHi ]"})
	lw.s = lw.s[:0]

	p.WriteString("\n\nHi\n\n")
	p.WriteString("Ho\n")
	lw.checkLine(t, []string{"[  ]", "[  ]", "[ Hi ]", "[  ]", "[ Ho ]"})
	lw.s = lw.s[:0]
}

func TestLineComposer(t *testing.T) {
	// prepare
	afn := "testdata/actLineComposer.log"
	efn := "testdata/expLineComposer.log"
	os.Remove(afn)
	c := cage.Start(afn)

	lw := newLocalDisplay()
	p := newLineComposer(lw, "off", "PREFIX---", "---SUFFIX")
	n, e := p.WriteString("Hi\nAll\r\n")

	cage.Stop(c)
	lib.EqualTextFiles(t, afn, efn)
	os.Remove(afn)

	lib.Equals(t, 8, n)
	lib.Equals(t, e, nil)
}

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
	q := newLineTranslatorANSI(p, "defaultColor")
	// lineComposer r implements the io.StringWriter interface and uses the lineWriter provided.
	// The line composer scans the trice strings and composes lines out of them according to its properies.
	r := newLineComposer(q, "TIMESTAMP", "PREFIX", "SUFFIX")

	// u uses the TriceAtomsReceiver interface (x) for reception and the io.StringWriter interface (r) for writing.
	// u collects trice atoms to a complete trice, generates the appropriate string from it and writes it to the provided io.StringWriter
	u := triceemit.NewSimpleTriceInterpreter(r, x)

	time.Sleep(100 * time.Millisecond)
	//fmt.Print(u)
	u.Stop() // end of life
	//t.Fail()
}

func TestLocal(t *testing.T) {
	// prepare
	afn := "testdata/actLocal.log"
	efn := "testdata/expLocal.log"
	os.Remove(afn)
	c := cage.Start(afn)
	p := newLocalDisplay()
	l1 := []string{"This is ", "the 1st ", "line"}
	l2 := []string{"This is ", "the 2nd ", "line"}
	p.writeLine(l1)
	p.writeLine(l2)
	cage.Stop(c)
	lib.EqualTextFiles(t, afn, efn)
	os.Remove(afn)
}

func TestRemote(t *testing.T) {
	// prepare
	afn := "testdata/actRemote.log"
	efn := "testdata/expRemote.log"
	os.Remove(afn)
	p := newRemoteDisplay("localhost", "65497", "trice.exe", "-logfile "+afn)
	l1 := []string{"This is ", "the 1st ", "line"}
	l2 := []string{"This is ", "the 2nd ", "line"}
	p.writeLine(l1)
	p.writeLine(l2)
	p.stopServer(0)
	time.Sleep(100 * time.Millisecond)
	lib.EqualTextFiles(t, afn, efn)
	os.Remove(afn)
}
