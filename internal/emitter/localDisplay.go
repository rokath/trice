// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package emitter

import (
	"fmt"
	"log"
	"path/filepath"
	"runtime"
	"strings"
)

// LocalDisplay is an object used for displaying.
// LocalDisplay implements the Linewriter interface.
type LocalDisplay struct {
	Err error
}

// NewLocalDisplay creates a LocalDisplay. It provides a Linewriter.
// It uses internally
func NewLocalDisplay() *LocalDisplay {

	// display lwD implements the Linewriter interface needed by lineTransformer.
	// It interprets the lines written to it according to its properties.
	lwD := &LocalDisplay{}
	return lwD
}

// ErrorFatal ends in osExit(1) if err not nil.
func (p *LocalDisplay) ErrorFatal() {
	if nil == p.Err {
		return
	}
	_, file, line, _ := runtime.Caller(1)
	log.Fatal(p.Err, filepath.Base(file), line)
}

// writeLine is the implemented Linewriter interface for localDisplay.
func (p *LocalDisplay) writeLine(line []string) {
	p.ErrorFatal()
	s := strings.Join(line, "")
	_, p.Err = fmt.Println(s)
}

// ColorDisplay is an object used for displaying.
// It implements the Linewriter interface.
// It embeds a local display and a line transformer
type ColorDisplay struct {
	display *LocalDisplay
	lw      LineWriter
}

// NewColorDisplay creates a ColorDisplay. It provides a Linewriter.
// It uses internally a local display combined with a line transformer.
func NewColorDisplay(colorPalette string) *ColorDisplay {

	// display lD implements the Linewriter interface needed by lineTransformer.
	// It interprets the lines written to it according to its properties.
	lD := NewLocalDisplay()
	// lwT uses the Linewriter lD internally.
	// It provides a Linewriter.
	lwT := NewLineTransformerANSI(lD, colorPalette)

	cD := &ColorDisplay{lD, lwT}
	return cD
}

// writeLine is the implemented Linewriter interface for localDisplay.
func (p *ColorDisplay) writeLine(line []string) {

	// calling p.lw writeLine method activates here: func (p *lineTransformerANSI) writeLine(line []string)
	p.lw.writeLine(line)
}
