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
	dumpCnt int
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
	return p
}

//  func (p *dumpDec) Read(b []byte) (n int, err error) {
//  	bb := make([]byte, 1024)
//  	m, err := p.in.Read(bb)
//  	for _, x := range bb[:m] {
//  		fmt.Fprintf(p.w, "%02X ", x) // workaround
//  		p.dumpCnt++
//  		if p.dumpCnt == DumpLineByteCount {
//  			fmt.Fprintln(p.w, "") // workaround
//  			p.dumpCnt = 0
//  		}
//  	}
//  	return n, err
//  }

func (p *dumpDec) Read(b []byte) (n int, err error) {
	//bb := make([]byte, 1024)
	m, err := p.in.Read(b)
	for _, x := range b[:m] {
		fmt.Fprintf(p.w, "%02X ", x) // workaround
		p.dumpCnt++
		if p.dumpCnt == DumpLineByteCount {
			fmt.Fprintln(p.w, "") // workaround
			p.dumpCnt = 0
		}
	}
	return n, err
}
