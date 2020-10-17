// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package decoder provides several decoders for differently encoded trice streams.
package decoder

import (
	"io"

	"github.com/rokath/trice/internal/id"
)

// EscDecoder is the Decoder instance for esc encoded trices
type Esc struct {
	Decoder
}

// NewEscaped provides an EscDecoder instance.
func NewEsc(list *id.List, in io.Reader) *Esc {
	p := &Esc{}
	p.in = in
	p.list = list
	return p
}

// StringsRead is the provided read method for esc decoding.
func (p *Esc) StringsRead(ss []string) (n int, err error) {
	return
}
