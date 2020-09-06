// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package triceFile reads trice data from file.
package triceFile

import "os"

// File delivers
type file struct {
	fh *os.File
}

// New creates an instance of trice file reader.
func NewReader() *file {
	r := &file{}
	return r
}

// Read is the exported read method.
//
// Read is part of the exported interface io.ReadCloser.
func (p *file) Read(b []byte) (int, error) {
	return p.fh.Read(b)
}

// Close ends the connection to the opened file.
//
// Close is part of the exported interface io.ReadCloser.
func (p *file) Close() error {
	return p.fh.Close()
}

// Open prepares for reding from fn.
//
// Open is not done inside New() to be able to get the filename later. It is not part of the exported interface.
func (p *file) Open(fn string) error {
	var err error
	p.fh, err = os.Open(fn)
	return err
}
