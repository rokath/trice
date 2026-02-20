// SPDX-License-Identifier: MIT

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

func init() {
	decoder.Register("CHAR", New)
}

// char is the decoding instance for plain character streams.
type char struct {
	decoder.DecoderData
}

// New creates a character stream decoder.
//
// The returned decoder forwards input bytes as-is. The LUT and endianness
// fields are stored for API consistency with other decoders.
func New(w io.Writer, lut id.TriceIDLookUp, m *sync.RWMutex, li id.TriceIDLookUpLI, in io.Reader, endian bool) decoder.Decoder {
	p := &char{
		DecoderData: decoder.NewDecoderData(decoder.Config{
			Out:      w,
			LUT:      lut,
			LUTMutex: m,
			LI:       li,
			In:       in,
			Endian:   endian,
		}),
	}
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
