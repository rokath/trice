// SPDX-License-Identifier: MIT

// Package dumpDecoder provides a decoder that hex-dumps an encoded trice stream.
package dumpDecoder

import (
	"errors"
	"fmt"
	"io"
	"sync"

	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/id"
)

func init() {
	decoder.Register("DUMP", New)
}

// dumpDec is the decoder instance for dump-formatted output.
type dumpDec struct {
	decoder.DecoderData
	dumpCnt int // dumped bytes per line
}

// New creates a decoder that converts incoming bytes to a hexadecimal dump.
func New(w io.Writer, lut id.TriceIDLookUp, m *sync.RWMutex, li id.TriceIDLookUpLI, in io.Reader, endian bool) decoder.Decoder {
	p := &dumpDec{
		DecoderData: decoder.NewDecoderData(decoder.Config{
			Out:      w,
			LUT:      lut,
			LUTMutex: m,
			LI:       li,
			In:       in,
			Endian:   endian,
		}),
		dumpCnt: 0, // needs =0 initialization for test table tests
	}
	return p
}

// Read converts raw input bytes to hex dump text and writes it into b.
//
// A minimum output buffer size of 4 bytes is required, because Read uses the
// last quarter of b as a temporary read buffer.
func (p *dumpDec) Read(b []byte) (n int, err error) {
	if p.In == nil {
		return 0, io.EOF
	}
	if len(b) < 4 {
		return 0, errors.New("buffer too small for dump decoder")
	}
	l := 3 * (len(b) >> 2) // 3rd quarter
	q := b[l:]             // used as scratch pad
	m, err := p.In.Read(q)
	for _, x := range q[:m] {
		s := fmt.Sprintf("%02x ", x)
		n += copy(b[n:], []byte(s))
		p.dumpCnt++
		if p.dumpCnt == decoder.DumpLineByteCount {
			n += copy(b[n:], []byte(`\n`))
			p.dumpCnt = 0
		}
	}
	return
}
