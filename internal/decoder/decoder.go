// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package decoder provides several decoders for differently encoded trice streams.
package decoder

import (
	"io"

	"github.com/rokath/trice/internal/id"
)

// StringsReader as Decoder method uses an underlying (byte) Reader for reading and returns max len(p) strings inside p.
// n is the received anmount of strings. err is the underlying Reader error or an internal error.
type StringsReader interface {
	StringsRead(p []string) (n int, err error)
}

// Decoder is the common data struct for all decoders
type Decoder struct {
	in         io.Reader // inner reader
	syncBuffer []byte    // unprocessed bytes hold for next cycle
	list       *id.List
	StringsReader
	savedErr error
	//item     id.Item // item is the next trice item ready for output.
	//done     chan int // This channel is used to stop the TriceInterpreter
}
