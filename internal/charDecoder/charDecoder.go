// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package charDecoder provides a decoder char encoded trice streams.
package charDecoder

import (
	"io"
	"sync"

	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/id"
)

// char is the Decoding instance for dumpDec encoded *Trices*.
type char struct {
	decoder.DecoderData
}

// New provides a character terminal output option for the trice tool.
func New(w io.Writer, lut id.TriceIDLookUp, m *sync.RWMutex, li id.TriceIDLookUpLI, in io.Reader, endian bool) *char {
	p := &char{}
	p.W = w
	p.In = in
	p.IBuf = make([]byte, 0, decoder.DefaultSize)
	p.Lut = lut
	p.LutMutex = m
	p.Endian = endian
	return p
}

func (p *char) Read(b []byte) (n int, err error) {
	return p.In.Read(b)
}
