// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package decoder provides several decoders for differently encoded trice streams.
package decoder

import (
	"bytes"
	"fmt"
	"io"
	"io/ioutil"

	"github.com/rokath/trice/internal/com"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/internal/link"
)

// StringsReader as Decoder method uses an underlying (byte) Reader for reading and returns max len(p) strings inside p.
// n is the received amount of strings. err is the underlying Reader error or an internal error.
type StringsReader interface {
	StringsRead(p []string) (n int, err error)
}

// Decoder is the interface (method set) a decoder needs to provide
type Decoder interface {
	StringsReader
}

// Decoder is the common data struct for all decoders
type decoding struct {
	in         io.Reader // inner reader
	syncBuffer []byte    // unprocessed bytes hold for next cycle
	list       *id.List
}

// NewInputPort is the common action taken by the decoder specific methods NewInputPort.
func NewInputPort(port, pargs string) (r io.ReadCloser, err error) {
	switch port {
	case "JLINK", "STLINK":
		l := link.NewDevice()
		if nil != l.Open() {
			err = fmt.Errorf("can not open link device %s with args %s", port, pargs)
		}
		r = l
	default: // assuming serial port
		var c com.COMport // interface type
		c = com.NewCOMPortGoBugSt(port)
		//c = com.NewCOMPortTarm(Port)
		if false == c.Open() {
			err = fmt.Errorf("can not open %s", port)
		}
		r = c
		return
	case "BUFFER": // pargs is expected to be a byte sequence in the same format as for example coming from one of the other ports
		r = ioutil.NopCloser(bytes.NewBufferString(pargs))
	}

	return
}
