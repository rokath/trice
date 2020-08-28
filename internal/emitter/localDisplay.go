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
// LocalDisplay implements the lineWriter interface
type LocalDisplay struct {
	Err error
}

// NewLocalDisplay creates a localDisplay. It provides a lineWriter
func NewLocalDisplay() *LocalDisplay {
	p := &LocalDisplay{}
	return p
}

// ErrorFatal ends in osExit(1) if err not nil.
func (p *LocalDisplay) ErrorFatal() {
	if nil == p.Err {
		return
	}
	_, file, line, _ := runtime.Caller(1)
	log.Fatal(p.Err, filepath.Base(file), line)
}

// writeLine is the implemented lineWriter interface for localDisplay.
func (p *LocalDisplay) writeLine(line []string) {
	p.ErrorFatal()
	s := strings.Join(line, "")
	_, p.Err = fmt.Println(s)
}
