// +build x
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

	"github.com/rokath/trice/pkg/display"
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
	notEqualFail(t, exp, act)

	act = strings.Split("att:12345\rdbg:\rtime:12:03\rsig:679012", "\r")
	exp = []string{"att:12345", "dbg:", "time:12:03", "sig:679012"}
	notEqualFail(t, exp, act)

	act = stringsSplit("att:12345\rdbg:\rtime:12:03\rsig:679012\r", '\r')
	exp = []string{"att:12345", "dbg:", "time:12:03", "sig:679012"}
	notEqualFail(t, exp, act)

	act = strings.Split("att:12345\rdbg:\rtime:12:03\rsig:679012\r", "\r")
	exp = []string{"att:12345", "dbg:", "time:12:03", "sig:679012", ""}
	notEqualFail(t, exp, act)

	act = stringsSplit("att:12345\rdbg:\rtime:12:03\rsig:679012\r\r", '\r')
	exp = []string{"att:12345", "dbg:", "time:12:03", "sig:679012", ""}
	notEqualFail(t, exp, act)

	act = strings.Split("att:12345\rdbg:\rtime:12:03\rsig:679012\r\r", "\r")
	exp = []string{"att:12345", "dbg:", "time:12:03", "sig:679012", "", ""}
	notEqualFail(t, exp, act)
}
