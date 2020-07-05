// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package decoder_test is a blackbox test for decoder
package decoder_test

import (
	"bytes"
	"fmt"
	"testing"
	"time"

	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/pkg/display"
	"github.com/rokath/trice/pkg/lib"
)

var (
	di *display.T
	
)

// init creates a default display for some tests
func init() {
	di = display.New("default")
}

// strin is an empty struct to provide a Write method for strings
type strin struct{}

// newStringWriter creates an object with a write method for strings
func newStringWriter() *strin {
	p := &strin{}
	return p
}

// Write is the provided write method for strin
func (p *strin) Write(s []string) (int, error) {
	var i int
	for i = range s {
		fmt.Println(s[i]) // just print out
	}
	i++
	return i, nil
}

// dis is an empty struct to provide a Write method for dis
type dis struct{}

// newStringWriter creates an object with a write method for strings
func newStringWriter() *dis {
	p := &dis{}
	return p
}

// Write is the provided write method for dis
// ss contains any number of strings ending with newline or not.
// All strings are checked if a string ends with newline.
// The newline is removed and the checked strings copied as string slice to a display line.
// 
func (p *dis) Write(ss []string) (int, error) {
	var i int
	for i, s = range ss {	
		var l display.Line 
		l.Ss = append( l.Ss, s) // simply only one string per line here

		if "\n" != s[len(s)-1:]{ // no newline at end
		continue
		}
		s = s[len(s)-1:] // remove newline char
		ls []display.Line
		ls = append( ls, l)

		di.Write(ls)
	}
	i++
	return i, nil
}

// commands is an empty struct to provide a Write method for commands
type commands struct{}

// newCommandWriter creates an object with a write method for commands
func newCommandWriter() *commands {
	p := &commands{}
	return p
}

// Write is the provided write method for commands
func (p *commands) Write(c []decoder.Command) (int, error) {
	var i int
	for i = range c {
		fmt.Println(c[i]) // just print out
	}
	i++
	return i, nil
}

func TestAsciiOneLine(t *testing.T) {
	by := bytes.NewReader([]byte{'m', ':', 'H', 'e', 'l', 'l', 'o', ' ', 'G', 'o', 'p', 'h', 'e', 'r', '!', '\n'})
	enc := []string{"ascii"}
	s := newStringWriter()
	d, err := decoder.New(by, enc, s, nil)
	lib.Equals(t, nil, err)
	d.Start()
	time.Sleep(time.Millisecond)
	//t.Fail()
}

func Test3(t *testing.T) {
	by := bytes.NewReader([]byte{'m', ':', 'H', 'e', 'l', 'l', 'o', ' ', 'G', 'o', 'p', 'h', 'e', 'r', '!', '\n'})
	enc := []string{"ascii"}
	c := newCommandWriter()
	d, err := decoder.New(by, enc, nil, c)
	lib.Equals(t, nil, err)
	d.Start()
	time.Sleep(time.Millisecond)
	//t.Fail()
}

func Test2(t *testing.T) {
	by := bytes.NewReader([]byte{'m', ':', 'H', 'e', 'l', 'l', 'o', ' ', 'G', 'o', 'p', 'h', 'e', 'r', '!', '\n'})
	enc := []string{"ascii"}
	s := newStringWriter()
	c := newCommandWriter()
	d, err := decoder.New(by, enc, s, c)
	lib.Equals(t, nil, err)
	d.Start()
	time.Sleep(time.Millisecond)
	//t.Fail()
}
