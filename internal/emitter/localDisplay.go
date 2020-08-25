// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package emitter

import (
	"fmt"
	"strings"

	"github.com/rokath/trice/internal/global"
)

// localDisplay is an object used for displaying.
// localDisplay implements the lineWriter interface
type localDisplay struct {
	err error
}

// newLocalDisplay creates a localDisplay. It provides a lineWriter
func newLocalDisplay() *localDisplay {
	p := &localDisplay{}
	return p
}

// writeLine is the implemented lineWriter interface for localDisplay.
func (p *localDisplay) writeLine(line []string) {
	global.Fatal(p.err)
	s := strings.Join(line, "")
	_, p.err = fmt.Println(s)
}
