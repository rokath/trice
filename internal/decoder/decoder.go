// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package decoder provides several decoders for differently encoded trice streams.
package decoder

import (
	"bytes"
	"encoding/json"
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

// decoding is the common data struct for all decoders
type decoding struct {
	in         io.Reader // inner reader
	syncBuffer []byte    // unprocessed bytes hold for next cycle
	lut        idLookUp  // id look-up map for translation
}

// NewInputPort is the common action taken by the decoder specific methods NewInputPort.
// port and pargs are the input port specific descriptors.
// r is returned for input abstraction.
// err is nil on successful open.
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

// idFmt contains the ID mapped information needed for decoding
type idFmt struct {
	Type string
	Strg string
}

// idLookUp is the translation map ID to format info
type idLookUp map[int]idFmt

// newIDLut assumes til as JSON formatted input and returns a map for trice ID to fmt string translation.
func newIDLut(til []byte) (lut idLookUp, err error) {

	// l is an item list for up to 65536 items (2^16)
	//l := make([]id.Item, 0, 65536)
	var list []id.Item

	if 0 < len(til) {
		err = json.Unmarshal(til, &list)
	}

	if nil != err {
		return lut, err
	}

	// create look-up map
	lut = make(idLookUp)
	// to do: add timestamp evaluation
	for _, item := range list {
		key := item.ID
		value := idFmt{Type: item.FmtType, Strg: item.FmtStrg}
		lut[key] = value
	}

	//fmt.Println(til)
	//fmt.Println(list)
	//fmt.Println(lut)

	return lut, nil
}
