// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package emitter

import (
	"fmt"
	"log"
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
	// notice that we're using 1, so it will actually log the where
	// the error happened, 0 = this function, we don't want that.
	pc, fn, line, _ := runtime.Caller(1)
	log.Fatalf("[error] in %s[%s:%d] %v", runtime.FuncForPC(pc).Name(), fn, line, p.Err)
}

// writeLine is the implemented lineWriter interface for localDisplay.
func (p *LocalDisplay) writeLine(line []string) {
	p.ErrorFatal()
	s := strings.Join(line, "")
	_, p.Err = fmt.Println(s)
}
