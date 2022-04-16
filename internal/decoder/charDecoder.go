// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package decoder provides several decoders for differently encoded trice streams.
package decoder

import (
	"io"
	"sync"

	"github.com/rokath/trice/internal/id"
)

// char is the Decoding instance for dumpDec encoded *Trices*.
type char struct {
	decoderData
}

// newCHARDecoder provides a character terminal output option for the trice tool.
func newCHARDecoder(w io.Writer, lut id.TriceIDLookUp, m *sync.RWMutex, in io.Reader, endian bool) Decoder {
	p := &char{}
	p.w = w
	p.in = in
	p.iBuf = make([]byte, 0, defaultSize)
	p.lut = lut
	p.lutMutex = m
	p.endian = endian
	return p
}

func (p *char) Read(b []byte) (n int, err error) {
	return p.in.Read(b)
}
