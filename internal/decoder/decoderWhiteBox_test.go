// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// whitebox test for decoder
package decoder

import (
	"bufio"
	"bytes"
	"fmt"
	"strings"
	"testing"

	"github.com/rokath/trice/pkg/display"
	"github.com/rokath/trice/pkg/lib"
)

// strin is an empty struct to provide a Write method for strings
type strin struct{}

// newStringWriter creates an object with a write method for strings
func newStringWriter() *strin {
	p := &strin{}
	return p
}

var (
	act []string
)

// Write is the provided write method for strin
func (p *strin) Write(s []string) (int, error) {
	act = make([]string, len(s))
	copy(act, s)
	return len(s), nil
}

func TestAsciiOneLine(t *testing.T) {
	//by := bytes.NewReader([]byte{'m', ':', 'H', 'e', 'l', 'l', 'o', ' ', 'G', 'o', 'p', 'h', 'e', 'r', '!', '\n'})
	by := bytes.NewReader([]byte("m:Hello Gopher!\n"))
	enc := []string{"ascii"}
	s := newStringWriter()
	d, err := New(by, enc, s, nil)
	lib.Equals(t, nil, err)
	d.decode()
	//exp := []string{"PREXIX", "TS", "m:Hello Gopher!"}
	//lib.Equals(t, exp, act)

}

func TestAscii2(t *testing.T) {
	//by := bytes.NewReader([]byte{'m', ':', 'H', 'e', 'l', 'l', 'o', ' ', 'G', 'o', 'p', 'h', 'e', 'r', '!', '\n'})
	by := bytes.NewReader([]byte("m:Hello Gopher!\n"))
	enc := []string{"ascii"}
	s := newStringWriter()
	d, err := New(by, enc, s, nil)
	lib.Equals(t, nil, err)
	d.decode()
	//exp := []string{"PREXIX", "TS", "m:Hello \aatt:Gopher!"}
	//lib.Equals(t, exp, act)

}

/*
// asciiDecode uses
func myAsciiDecoder(r io.Reader, w StringWriter)( bc, sc int, error) {
return 0,0,nil
}
*/

// asciiDecode converts buffer to string and splits at delimiters and returns segments without delimiter
func asciiDecode(buffer []byte, delimiter string) []string {
	s := string(buffer)
	return strings.Split(s, delimiter)
}

func ExampleScanner() {
	const input = "msg:1234 5678\r\natt:123\ndebug:456790123456789\nt:0"
	scanner := bufio.NewScanner(strings.NewReader(input))
	for scanner.Scan() {

		fmt.Printf("%s\n", scanner.Text())

	}
	if err := scanner.Err(); err != nil {

		fmt.Printf("Invalid input: %s", err)

	}
	// Output:
	// msg:1234 5678
	// att:123
	// debug:456790123456789
	// t:0
}

func Test_asciiDecode(t *testing.T) {
	b := []byte("m:Hello\nyou\t\a\a\asig:Gophers!\n")
	act := asciiDecode(b, "\a")

	exp := []string{"m:Hello\nyou\t", "", "", "sig:Gophers!\n"}
	lib.Equals(t, exp, act)
}

// buildLines scans all stings in s for newlines ans splits them.
// All strings and substrings until next newline build one line which starts with prefic and timestamp and ends with postfix.
func buildLines(ss []string, prefix, timestamp, postfix string) []display.Line {
	var li []display.Line
	if 0 == len(ss) {
		return li
	}

	var line display.Line

	for _, s := range ss {
		scanner := bufio.NewScanner(strings.NewReader(s))
		line.Segments = append(line.Segments, prefix, timestamp)

		// Validate the input

		for scanner.Scan() {
			line.Segments = append(line.Segments, scanner.Text(), postfix)
		}
		line.Segments = append(line.Segments, postfix)
		li = append(li, line)
	}
	return li
}

/*
func Test_buildLines(t *testing.T) {
	input := []string{"m:one", "a:two\nthree", "sig:four\r\n"}
	pre := "PREFIX:"
	ts := "time.stamp"
	post := ""
	act := buildLines(input, pre, ts, post)
	exp := []display.Line{
		{[]string{pre, ts, "m:one", "a:two", post}},
		{[]string{pre, ts, "sig:three", post}},
	}
	lib.Equals(t, exp, act)
}
*/
