// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package wrapSynchronizer_test is a blackbox test.
package wrapSynchronizer_test

import (
	"testing"

	"github.com/rokath/trice/internal/receiver/wrapSynchronizer"
	"github.com/rokath/trice/pkg/lib"
)

// simulator delivers an endless circle of bytes
type simulator struct {
	t      []byte
	offset int
}

// newSimulator creates an instance of simulator.
//
// It contains some false bytes at the beginning followed by a valid wrap package.
func newSimulator() *simulator {
	r := &simulator{
		t: []byte{1, 2, 3, 235, 96, 96, 235, 10, 172, 10, 172},
	}
	return r
}

// Read reads the simulation data circular.
func (p *simulator) Read(b []byte) (int, error) {
	var i int
	for i = range b {
		b[i] = p.t[(p.offset+i)%len(p.t)]
	}
	i++ // adjust to byte count
	p.offset += i
	return i, nil
}

// check if sync works
func Test1(t *testing.T) {
	s := newSimulator()
	ws := wrapSynchronizer.New(s)
	b := make([]byte, 8)
	ws.Read(b)
	c := []byte{235, 96, 96, 235, 10, 172, 10, 172}
	lib.Equals(t, b, c)
}
