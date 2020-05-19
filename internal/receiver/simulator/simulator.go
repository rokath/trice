// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package simulator simulates artificial trice data for testing.
package simulator

// Simulator delivers an endless circle of bytes
type Simulator struct {
	t []byte
}

// New creates an instance of simulator.
func New() *Simulator {
	r := &Simulator{
		t: []byte{235, 96, 96, 182, 0, 0, 232, 181},
	}
	return r
}

// Read is the exported method.
func (p *Simulator) Read(b []byte) (int, error) {
	var i int
	for i = range b {
		b[i] = p.t[i%len(p.t)]
	}
	return i + 1, nil
}
