// Copyright 2022 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package decoder

import (
	"io"
	"sync"

	"github.com/rokath/trice/internal/id"
)

// newTREXDecoder provides a TREX decoder instance.
//
// l is the trice id list in slice of struct format.
// in is the usable reader for the input bytes.
func newTREXDecoder(w io.Writer, lut id.TriceIDLookUp, m *sync.RWMutex, in io.Reader, endian bool) Decoder {
	p := &cobsDec{}
	p.cycle = 0xc0 // start value
	p.w = w
	p.in = in
	p.iBuf = make([]byte, 0, defaultSize)
	p.b = make([]byte, 0, defaultSize)
	p.lut = lut
	p.lutMutex = m
	p.endian = endian
	return p
}
