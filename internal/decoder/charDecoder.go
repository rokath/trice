// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package decoder provides several decoders for differently encoded trice streams.
package decoder

import (
	"fmt"
	"io"
	"sync"

	"github.com/rokath/trice/internal/id"
)

// CHAR is the Decoding instance for DUMP encoded trices.
type CHAR struct {
	decoderData
}

// NewCHARDecoder provides a character terminal output option for the trice tool.
func NewCHARDecoder(w io.Writer, lut id.TriceIDLookUp, m *sync.RWMutex, in io.Reader, endian bool) Decoder {
	p := &CHAR{}
	p.w = w
	p.in = in
	p.iBuf = make([]byte, 0, defaultSize)
	p.lut = lut
	p.lutMutex = m
	p.endian = endian
	return p
}

func (p *CHAR) Read(b []byte) (n int, err error) {
	bb := make([]byte, 256)
	m, err := p.in.Read(bb)
	fmt.Fprint(p.w, string(bb[:m]))
	return n, err
}
