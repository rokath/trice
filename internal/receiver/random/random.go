// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package random generates random trice data for testing.
package random

import (
	"math/rand"
	"time"
)

// Random delivers an endless circle of random bytes
type Random struct {
}

// New creates an instance of Random.
func New() *Random {
	rand.Seed(time.Now().UnixNano())
	r := &Random{}
	return r
}

// Read is the exported method.
func (p *Random) Read(b []byte) (int, error) {
	var i int
	for i = range b {
		b[i] = byte(rand.Intn(255))
	}
	return i, nil
}
