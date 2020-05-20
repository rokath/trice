// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
//                basti@blackoutcloud.de
// Use of this source code is governed by a license that can be found in the LICENSE file.

package http

import "time"

// HTTP defines ...
type HTTP struct {
	IPaddress []int
	port      int
}

// New creates an instance of HTTP receiver.
func New() *HTTP {
	r := &HTTP{
		IPaddress: []int{127, 0, 0, 1},
		port:      8888,
	}
	return r
}

// Read() is part of the exported interface io.ReadCloser. It reads a slice of bytes.
func (p *HTTP) Read(b []byte) (int, error) {
	var i int
	time.Sleep(100 * time.Millisecond)
	for i = range b {
		b[i] = 'A'
	}
	if i >= 7 {
		b[0] = 235                                            // valid start byte
		b[1] = 96                                             // valid addr
		b[2] = 96                                             // valid addr
		b[3] = b[0] ^ b[1] ^ b[2] ^ b[4] ^ b[5] ^ b[6] ^ b[7] // compute crc
	}
	return i + 1, nil
}

// Close ends the connection...
func (p *HTTP) Close() error {
	return nil // clean up after closing webserver
}

// Open opens a connection.
func (p *HTTP) Open() bool {
	return true
}
