// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// whitebox test for package emitter.
package emitter

import (
	"io/ioutil"

	"runtime"
	"strings"
)

// test helper ///////////////////////////////////////////////////////////////////////
//
//

func readLines(filename string) (lines []string, err error) {
	content, err := ioutil.ReadFile(filename)
	stringContent := string(content)
	if runtime.GOOS == "windows" {
		stringContent = strings.ReplaceAll(stringContent, "\r\n", "\n")
	}
	lines = strings.Split(stringContent, "\n")
	return
}

// checkDisplay is an object used for testing.
// It implements the Linewriter interface.
type checkDisplay struct {
	lines []string
}

// newCheckDisplay creates a Display. It provides a Linewriter.
func newCheckDisplay() *checkDisplay {
	p := &checkDisplay{}
	return p
}

// writeLine is the implemented Linewriter interface for checkDisplay.
// It appends written lines to the internal data.
func (p *checkDisplay) writeLine(line []string) {
	s := strings.Join(line, "")
	p.lines = append(p.lines, s)
}

//
// test helper ///////////////////////////////////////////////////////////////////////
