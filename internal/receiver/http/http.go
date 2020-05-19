// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
//                basti@blackoutcloud.de
// Use of this source code is governed by a license that can be found in the LICENSE file.

package http

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
	for i = range b {
		b[i] = 0
	}
	return i, nil
}

// Close ends the connection...
func (p *HTTP) Close() error {
	return nil // clean up after closing webserver
}

// Open opens a connection.
func (p *HTTP) Open() bool {
	return true
}
