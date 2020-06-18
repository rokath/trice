// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package inputDummy_test is a blsckbox test.
package inputDummy_test

import (
	"testing"
	"time"

	"github.com/rokath/trice/internal/receiver/inputDummy"
	"github.com/rokath/trice/pkg/lib"
)

// Source delivers an endless circle of bytes.
//
//
type Source struct {
	b []byte        // internal byte buffer to read from
	t time.Duration // tinternal delay
}

// New creates an instance of simulator.
// b contains bytes it reads cyclically
func New(b []byte, t time.Duration) *Source {
	s := &Source{
		//t: []byte{'g', 'a', 'r', 'b', 'a', 'g', 'e', 235, 96, 96, 0, 10, 172, 0, 0},
	}
	s.b = b
	s.t = t
	return s
}

// Read is the exported method.
func (p *Source) Read(b []byte) (int, error) {
	var i int
	time.Sleep(p.t)
	//p.b[6]++                                                              // increment par
	//p.b[3] = p.b[0] ^ p.b[1] ^ p.b[2] ^ p.b[4] ^ p.b[5] ^ p.b[6] ^ p.b[7] // compute crc
	for i = range b {
		b[i] = p.b[i%len(p.b)]
	}
	return i + 1, nil
}

func Test1(t *testing.T) {
	i := []byte{'g', 'a', 'r', 'b', 'a', 'g', 'e', 235, 96, 96, 0, 10, 172, 0, 0}
	s := inputDummy.New(b, 0)
	o := make([]byte, 7)
	r := []byte{'g', 'a', 'r', 'b', 'a', 'g', 'e'}
	s.Read(o)
	lib.Equals(t, i, o)
}
