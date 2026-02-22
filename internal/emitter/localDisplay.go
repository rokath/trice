// SPDX-License-Identifier: MIT

package emitter

import (
	"fmt"
	"io"
	"log"
	"path/filepath"
	"runtime"
	"strings"
)

// localDisplay writes completed lines to an io.Writer.
type localDisplay struct {
	w   io.Writer
	Err error
}

// newLocalDisplay creates a local LineWriter implementation.
func newLocalDisplay(w io.Writer) *localDisplay {

	// display lwD implements the Linewriter interface needed by lineTransformer.
	// It interprets the lines written to it according to its properties.
	lwD := &localDisplay{}
	lwD.w = w
	return lwD
}

// errorFatal ends in osExit(1) if err not nil.
func (p *localDisplay) errorFatal() {
	if nil == p.Err {
		return
	}
	_, file, line, _ := runtime.Caller(1)
	log.Fatal(p.Err, filepath.Base(file), line)
}

// WriteLine implements LineWriter.
func (p *localDisplay) WriteLine(line []string) {
	p.errorFatal()
	s := strings.Join(line, "")
	_, p.Err = fmt.Fprintln(p.w, s)
}

// colorDisplay composes local display output with optional ANSI transformation.
type colorDisplay struct {
	display *localDisplay
	lw      LineWriter
}

// newColorDisplay creates a local display with tag-aware ANSI transformation.
func newColorDisplay(w io.Writer, colorPalette string) *colorDisplay {

	// display lD implements the Linewriter interface needed by lineTransformer.
	// It interprets the lines written to it according to its properties.
	lD := newLocalDisplay(w)
	// lwT uses the Linewriter lD internally.
	// It provides a Linewriter.
	lwT := newLineTransformerANSI(lD, colorPalette)

	cD := &colorDisplay{lD, lwT}
	return cD
}

// WriteLine implements LineWriter.
func (p *colorDisplay) WriteLine(line []string) {

	// Delegates to lineTransformerANSI.WriteLine.
	p.lw.WriteLine(line)
}
