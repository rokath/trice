// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package decoder_test is a blackbox test for decoder
package decoder_test

import (
	"bufio"
	"bytes"
	"fmt"
	"strings"
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
	if 0 == len(s) {
		return 0, nil
	}
	var i int
	for i = range s {
		fmt.Println(s[i]) // just print out
	}
	i++
	return i, nil
}

/*
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
		l.Segments = append( l.Segments, s) // simply only one string per line here

		if "\n" != s[len(s)-1:]{ // no newline at end
		continue
		}
		s = s[len(s)-1:] // remove newline char
		var ls []display.Line
		ls = append( ls, l)

		di.Write(ls)
	}
	i++
	return i, nil
}
*/
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
	//by := bytes.NewReader([]byte{'m', ':', 'H', 'e', 'l', 'l', 'o', ' ', 'G', 'o', 'p', 'h', 'e', 'r', '!', '\n'})
	by := bytes.NewReader([]byte("m:Hello\rdebug: \ratt:Gopher!\n"))
	enc := []string{"ascii"}
	s := newStringWriter()
	d, err := decoder.New(by, enc, s, nil)
	lib.Equals(t, nil, err)
	d.Start()
	time.Sleep(time.Millisecond)
	//t.Fail()
}

/*
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
*/

func Test2(t *testing.T) {
	//by := bytes.NewReader([]byte{'m', ':', 'H', 'e', 'l', 'l', 'o', ' ', 'G', 'o', 'p', 'h', 'e', 'r', '!', '\n'})
	by := bytes.NewReader([]byte("m:Hello Gopher!\n"))
	enc := []string{"ascii"}
	s := newStringWriter()
	c := newCommandWriter()
	d, err := decoder.New(by, enc, s, c)
	lib.Equals(t, nil, err)
	d.Start()
	time.Sleep(time.Millisecond)
	//t.Fail()
}

func TestX(t *testing.T) {
	// An artificial input source.
	const input = "msg:1234 5678\natt:12345\rdbg:\rtime:12:03\rsig:679012\r\n\nMSG:34567890"
	scanner := bufio.NewScanner(strings.NewReader(input))

	exp := []string{"msg:1234 5678", "att:12345\rdbg:\rtime:12:03\rsig:679012", "", "MSG:34567890"}
	var act []string
	for scanner.Scan() {
		act = append(act, scanner.Text())
	}
	lib.Equals(t, exp, act)
}

// stringsSplit splits s on each delimiter, removes d and returns all substrings
func stringsSplit(s string, delimiter byte) []string {
	scanner := bufio.NewScanner(strings.NewReader(s))

	// func is a split function for a Scanner that returns each line of
	// text, stripped of any trailing '\r' The returned line may
	// be empty. The end-of-line marker is one carriage return.
	// The last non-empty line of input will be returned even if it has  '\r'
	split := func(data []byte, atEOF bool) (advance int, token []byte, err error) {
		if atEOF && len(data) == 0 {
			return 0, nil, nil
		}
		if i := bytes.IndexByte(data, delimiter); i >= 0 {
			// We have a full newline-terminated line.
			return i + 1, data[0:i], nil
		}
		// If we're at EOF, we have a final, non-terminated line. Return it.
		if atEOF {
			return len(data), data, nil
		}
		// Request more data.
		return 0, nil, nil
	}

	// Set the split function for the scanning operation.
	scanner.Split(split)

	var result []string
	for scanner.Scan() {
		result = append(result, scanner.Text())
	}
	return result
}

func TestZ(t *testing.T) {
	act := stringsSplit("att:12345\rdbg:\rtime:12:03\rsig:679012", '\r')
	exp := []string{"att:12345", "dbg:", "time:12:03", "sig:679012"}
	lib.Equals(t, exp, act)

	act = strings.Split("att:12345\rdbg:\rtime:12:03\rsig:679012", "\r")
	exp = []string{"att:12345", "dbg:", "time:12:03", "sig:679012"}
	lib.Equals(t, exp, act)

	act = stringsSplit("att:12345\rdbg:\rtime:12:03\rsig:679012\r", '\r')
	exp = []string{"att:12345", "dbg:", "time:12:03", "sig:679012"}
	lib.Equals(t, exp, act)

	act = strings.Split("att:12345\rdbg:\rtime:12:03\rsig:679012\r", "\r")
	exp = []string{"att:12345", "dbg:", "time:12:03", "sig:679012", ""}
	lib.Equals(t, exp, act)

	act = stringsSplit("att:12345\rdbg:\rtime:12:03\rsig:679012\r\r", '\r')
	exp = []string{"att:12345", "dbg:", "time:12:03", "sig:679012", ""}
	lib.Equals(t, exp, act)

	act = strings.Split("att:12345\rdbg:\rtime:12:03\rsig:679012\r\r", "\r")
	exp = []string{"att:12345", "dbg:", "time:12:03", "sig:679012", "", ""}
	lib.Equals(t, exp, act)
}
