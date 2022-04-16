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

// dumpDec is the Decoding instance for dumpDec encoded trices.
type dumpDec struct {
	decoderData
	dumpCnt int // dumped bytes per line
}

// newDUMPDecoder provides a hex dump option for incoming bytes.
func newDUMPDecoder(w io.Writer, lut id.TriceIDLookUp, m *sync.RWMutex, in io.Reader, endian bool) Decoder {
	p := &dumpDec{}
	p.w = w
	p.in = in
	p.iBuf = make([]byte, 0, defaultSize)
	p.lut = lut
	p.lutMutex = m
	p.endian = endian
	p.dumpCnt = 0 // needs =0 initialization for test table tests
	return p
}

func (p *dumpDec) Read(b []byte) (n int, err error) {
	l := 3 * (len(b) >> 2) // 3rd quarter
	q := b[l:]             // used as scratch pad
	m, err := p.in.Read(q)
	for _, x := range q[:m] {
		s := fmt.Sprintf("%02x ", x)
		n += copy(b[n:], []byte(s))
		p.dumpCnt++
		if p.dumpCnt == DumpLineByteCount {
			n += copy(b[n:], []byte(`\n`))
			p.dumpCnt = 0
		}
	}
	return
}
