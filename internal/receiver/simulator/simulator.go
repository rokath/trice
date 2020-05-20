// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package simulator simulates artificial trice data for testing.
package simulator

import "time"

// Simulator delivers an endless circle of bytes
type Simulator struct {
	t []byte
}

// New creates an instance of simulator.
func New() *Simulator {
	r := &Simulator{
		t: []byte{235, 96, 96, 0, 10, 172, 0, 0},
	}
	return r
}

// Read is the exported method.
func (p *Simulator) Read(b []byte) (int, error) {
	var i int
	time.Sleep(100 * time.Millisecond)
	p.t[6]++                                                              // increment par
	p.t[3] = p.t[0] ^ p.t[1] ^ p.t[2] ^ p.t[4] ^ p.t[5] ^ p.t[6] ^ p.t[7] // compute crc
	for i = range b {
		b[i] = p.t[i%len(p.t)]
	}
	return i + 1, nil
}
