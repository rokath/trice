// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package inputDummy simulates artificial trice data for testing.
//
// On creation a byte slice with mockup data is expected.
// When end of slice is reached reading starts from the beginning.
// The time.Duration value can be used to delay the Read function.
package inputDummy

import "time"

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
