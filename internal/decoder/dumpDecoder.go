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

// DUMP is the Decoding instance for DUMP encoded trices.
type DUMP struct {
	decoderData
	dumpCnt int
}

// NewDUMPDecoder provides a hex dump option for incoming bytes.
func NewDUMPDecoder(w io.Writer, lut id.TriceIDLookUp, m *sync.RWMutex, in io.Reader, endian bool) Decoder {
	p := &DUMP{}
	p.w = w
	p.in = in
	p.iBuf = make([]byte, 0, defaultSize)
	p.lut = lut
	p.lutMutex = m
	p.endian = endian
	return p
}

func (p *DUMP) Read(b []byte) (n int, err error) {
	bb := make([]byte, 1024)
	m, err := p.in.Read(bb)
	for _, x := range bb[:m] {
		fmt.Fprintf(p.w, "%02X ", uint8(x)) // workaround
		p.dumpCnt++
		if p.dumpCnt == DumpLineByteCount {
			fmt.Fprintln(p.w, "") // workaround
			p.dumpCnt = 0
		}
	}
	return n, err
}
