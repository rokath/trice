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
	lut        IDLookUp  // id look-up map for translation
}

// NewInputPort returns a ReadCloser for the specified port and its args.
// err is nil on successful open.
// When port is "COMn" args can be used to be "TARM" to use a different driver for dynamic testing.
// When port is "BUFFER", args is expected to be a byte sequence in the same format as for example coming from one of the other ports.
// When port is "JLINK" args contains JLinkRTTLogger.exe specific parameters described inside UM08001_JLink.pdf.
// When port is "STLINK" args has the same format as for "JLINK"
func NewInputPort(port, args string) (r io.ReadCloser, err error) {
	switch port {
	case "JLINK", "STLINK":
		l := link.NewDevice()
		if nil != l.Open() {
			err = fmt.Errorf("can not open link device %s with args %s", port, args)
		}
		r = l
	default: // assuming serial port
		var c com.COMport   // interface type
		if "TARM" == args { // for comparing dynamic behaviour
			c = com.NewCOMPortTarm(port)
		} else {
			c = com.NewCOMPortGoBugSt(port)
		}
		if false == c.Open() {
			err = fmt.Errorf("can not open %s", port)
		}
		r = c
		return
	case "BUFFER":
		r = ioutil.NopCloser(bytes.NewBufferString(args))
	}
	return
}

// idFmt contains the ID mapped information needed for decoding.
type idFmt struct {
	Type string
	Strg string
}

// IDLookUp is the ID to format info translation map
type IDLookUp map[int]idFmt

// UnmarshalTriceIDList extracts the trice ID list byte slice to an items slice.
// til is a result of a read til.json file or is a converted test string.
// til can change during runtime, when an "trice update" occurs.
// Just in case til is not consistent the err value is not nil.
func UnmarshalTriceIDList(til []byte) (list []id.Item, err error) {
	if 0 < len(til) {
		err = json.Unmarshal(til, &list)
	}
	return
}

// MakeLut returns a trice ID lookup map.
func MakeLut(list []id.Item) (lut IDLookUp) {
	// create look-up map
	lut = make(IDLookUp)
	// to do: add timestamp evaluation
	for _, item := range list {
		key := item.ID
		value := idFmt{Type: item.FmtType, Strg: item.FmtStrg}
		lut[key] = value
	}
	return
}

// newIDLut assumes til as JSON formatted input and returns a map for trice ID to fmt string translation.
func newIDLut(til []byte) (IDLookUp, error) {
	list, err := UnmarshalTriceIDList(til)
	if nil != err {
		return nil, err
	}
	lut := MakeLut(list) // create look-up map
	return lut, nil
}
