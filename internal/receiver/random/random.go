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
	//rand.Seed(time.Now().UnixNano())
	rand.Seed(0)
	r := &Random{}
	return r
}

// Read is the exported method.
func (p *Random) Read(b []byte) (int, error) {
	var i int
	time.Sleep(10 * time.Millisecond)
	for i = range b {
		b[i] = byte(rand.Intn(255))
	}
	//if i >= 7 {
	//	b[0] = 235 // valid start byte
	//	b[1] = 96                                             // valid addr
	//	b[2] = 96                                             // valid addr
	//	b[3] = b[0] ^ b[1] ^ b[2] ^ b[4] ^ b[5] ^ b[6] ^ b[7] // compute crc
	//}
	return i + 1, nil
}
