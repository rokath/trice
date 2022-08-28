// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package dumpDecoder provides a decoder dumping encoded trice stream.
package dumpDecoder

import (
	"fmt"
	"io"
	"sync"

	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/id"
)

// dumpDec is the Decoding instance for dumpDec encoded trices.
type dumpDec struct {
	decoder.DecoderData
	dumpCnt int // dumped bytes per line
}

// New provides a hex dump option for incoming bytes.
func New(w io.Writer, lut id.TriceIDLookUp, m *sync.RWMutex, li id.TriceIDLookUpLI, in io.Reader, endian bool) decoder.Decoder {
	p := &dumpDec{}
	p.W = w
	p.In = in
	p.IBuf = make([]byte, 0, decoder.DefaultSize)
	p.Lut = lut
	p.LutMutex = m
	p.Endian = endian
	p.dumpCnt = 0 // needs =0 initialization for test table tests
	return p
}

func (p *dumpDec) Read(b []byte) (n int, err error) {
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
