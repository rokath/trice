// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package decoder provides several decoders for differently encoded trice streams.
package decoder

import (
	"encoding/json"
	"fmt"
	"io"
	"log"
	"path/filepath"
	"runtime"
	"time"

	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/pkg/cipher"
)

const (
	// receive and sync buffer size
	buffSize = 1024
)

var (
	// Verbose gives mor information on output if set. The value is injected from main packages.
	Verbose bool

	// Encoding describes the way the byte stream is coded.
	Encoding string
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

// NewEscFormat provides an EscDecoder instance.
// l is the trice id list in slice of struct format.
// in is the usable reader for the input bytes.
func NewEscFormat(l []id.Item, in io.Reader) (p *Esc) {
	p = &Esc{}
	p.in = in
	p.syncBuffer = make([]byte, 0, 2*buffSize)
	p.lut = MakeLut(l)
	return
}

// Translate performs the trice log task.
// Bytes are read with rc, according decoder.Encoding are translated into strings
// returns false on program end signal or true on hard read error.
func Translate(sw *emitter.TriceLineComposer, list *id.List, rc io.ReadCloser /*, hardReadError chan bool*/) bool {

	// activate selected encoding
	// var p translator.Translator // interface type
	switch Encoding {
	case "esc":
		dec := NewEscFormat(list.ItemList, rc)
		for {
			err := run(sw, dec)
			if nil != err {
				time.Sleep(2 * time.Second)
				dec = NewEscFormat(list.ItemList, rc) // read list again - it could have changed
			}
		}
	case "bare":
		dec := NewBareFormat(list.ItemList, rc)
		for {
			err := run(sw, dec)
			if nil != err {
				time.Sleep(2 * time.Second)
				dec = NewBareFormat(list.ItemList, rc) // read list again - it could have changed
			}
		}
	case "wrap":
		dec := NewBareFormat(list.ItemList, NewBareReaderFromWrap(rc))
		for {
			err := run(sw, dec)
			if nil != err {
				time.Sleep(2 * time.Second)
				dec = NewBareFormat(list.ItemList, NewBareReaderFromWrap(rc)) // read list again - it could have changed
			}
		}

	//case "bare":
	//	p = receiveBareSimpleTricesAndDisplayAnsiColor(sw, rc, list, hardReadError)
	//case "esc":
	//p = receiveEscTricesAndDisplayAnsiColor(sw, rc, list, hardReadError)
	//case "wrap", "wrapped":
	//p = receiveWrapSimpleTricesAndDisplayAnsiColor(rc, fnJSON)
	//	case "sim":
	//		p = simNewSimpleTriceInterpreterWithAnsi(r)
	case "bareXTEACrypted", "wrapXTEACrypted":
		errorFatal(cipher.SetUp())
		fallthrough
	case "ascii":
		fallthrough
	default:
		fmt.Println("unknown encoding ", Encoding)
		return false
	}

	//// prepare CTRL-C shutdown reaction
	//sigs := make(chan os.Signal, 1)
	//signal.Notify(sigs, syscall.SIGINT, syscall.SIGTERM)
	//select {
	//case <-hardReadError:
	//	if verbose {
	//		//fmt.Println("####################################", p.SavedError(), "####################################")
	//	}
	//	// p.Done() <- 0 // end translator
	//	return true
	//case sig := <-sigs: // wait for a signal
	//	if verbose {
	//		fmt.Println("####################################", sig, "####################################")
	//	}
	//	//p.Done() <- 0 // end translator
	//	return false // back to main
	//}
}

func run(sw *emitter.TriceLineComposer, sr StringsReader) error {
	ss := make([]string, 100)
	n, err := sr.StringsRead(ss)
	if nil != err && io.EOF != err {
		return err
	}
	for i := range ss[:n] {
		sw.WriteString(ss[i])
	}
	return nil
}

// errorFatal ends in osExit(1) if err not nil.
func errorFatal(err error) {
	if nil == err {
		return
	}
	if Verbose {
		_, file, line, _ := runtime.Caller(1)
		log.Fatal(err, " "+filepath.Base(file)+" ", line)
	}
	log.Fatal(err)
}
