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
	"github.com/rokath/trice/pkg/lib"
)

// strings is an empty struct to provide a Write method for strings
type strings struct{}

// newStringWriter creates an object with a write method for strings
func newStringWriter() *strings {
	p := &strings{}
	return p
}

// Write is the provided write method for strings
func (p *strings) Write(s []string) (int, error) {
	var i int
	for i = range s {
		fmt.Println(s[i]) // just print out
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

func Test1(t *testing.T) {
	by := bytes.NewReader([]byte{'g', '1', '2', 'g', 'g', 'g', 'a', 'b', 'g', 'g', 'g', 'g', 'g', '0', '\n'})
	enc := []string{"ascii"}
	s := newStringWriter()
	c := newCommandWriter()
	d, err := decoder.New(by, enc, s, c)
	lib.Equals(t, nil, err)
	d.Start()
	time.Sleep(time.Millisecond)
	//t.Fail()
}
