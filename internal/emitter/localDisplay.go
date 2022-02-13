// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package emitter

import (
	"fmt"
	"io"
	"log"
	"path/filepath"
	"runtime"
	"strings"
)

// localDisplay is an object used for displaying.
// localDisplay implements the Linewriter interface.
type localDisplay struct {
	w   io.Writer
	Err error
}

// newLocalDisplay creates a LocalDisplay. It provides a Linewriter.
// It uses internally
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

// writeLine is the implemented Linewriter interface for localDisplay.
func (p *localDisplay) writeLine(line []string) {
	p.errorFatal()
	s := strings.Join(line, "")
	_, p.Err = fmt.Fprintln(p.w, s)
}

// colorDisplay is an object used for displaying.
// It implements the Linewriter interface.
// It embeds a local display and a line transformer
type colorDisplay struct {
	display *localDisplay
	lw      lineWriter
}

// newColorDisplay creates a ColorDisplay. It provides a Linewriter.
// It uses internally a local display combined with a line transformer.
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

// writeLine is the implemented Linewriter interface for localDisplay.
func (p *colorDisplay) writeLine(line []string) {

	// calling p.lw writeLine method activates here: func (p *lineTransformerANSI) writeLine(line []string)
	p.lw.writeLine(line)
}
