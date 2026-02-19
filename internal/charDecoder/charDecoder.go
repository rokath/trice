// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package charDecoder provides a pass-through decoder for character streams.
//
// It is used for transport modes where incoming data is already plain text and
// should be forwarded unchanged.
package charDecoder

import (
	"io"
	"sync"

	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/id"
)

// char is the decoding instance for plain character streams.
type char struct {
	decoder.DecoderData
}

// New creates a character stream decoder.
//
// The returned decoder forwards input bytes as-is. The LUT and endianness
// fields are stored for API consistency with other decoders.
func New(w io.Writer, lut id.TriceIDLookUp, m *sync.RWMutex, li id.TriceIDLookUpLI, in io.Reader, endian bool) decoder.Decoder {
	p := &char{}
	p.W = w
	p.In = in
	p.IBuf = make([]byte, 0, decoder.DefaultSize)
	p.Lut = lut
	p.LutMutex = m
	p.Endian = endian
	return p
}

// Read forwards bytes from the current input stream to b.
//
// If no input stream is configured, Read returns io.EOF.
func (p *char) Read(b []byte) (n int, err error) {
	if p.In == nil {
		return 0, io.EOF
	}
	return p.In.Read(b)
}
