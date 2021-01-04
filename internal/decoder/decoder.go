// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package decoder provides several decoders for differently encoded trice streams.
package decoder

import (
	"encoding/binary"
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
	buffSize     = 4096
	littleEndian = true
	bigEndian    = false
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
	endian     bool      // littleEndian or bigEndian
	trice      idFmt     // received trice
	b          []byte    // read buffer
	bc         int       // trice specific bytes count
}

// readU16 returns the 2 b bytes as uint16 according the specified endianess
func (p *decoding) readU16(b []byte) uint16 {
	if littleEndian == p.endian {
		return binary.LittleEndian.Uint16(b)
	}
	return binary.BigEndian.Uint16(b)
}

// readU32 returns the 4 b bytes as uint32 according the specified endianess
func (p *decoding) readU32(b []byte) uint32 {
	if littleEndian == p.endian {
		return binary.LittleEndian.Uint32(b)
	}
	return binary.BigEndian.Uint32(b)
}

// readU64 returns the 8 b bytes as uint64 according the specified endianess
func (p *decoding) readU64(b []byte) uint64 {
	if littleEndian == p.endian {
		return binary.LittleEndian.Uint64(b)
	}
	return binary.BigEndian.Uint64(b)
}

// rub removes leading bytes from sync buffer
func (p *decoding) rub(n int) {
	p.syncBuffer = p.syncBuffer[n:]
}

func (p *decoding) outOfSync(msg string) (n int, e error) {
	n = copy(p.b, fmt.Sprintln("error:", msg, "ignoring first byte", p.syncBuffer[0:p.bc]))
	p.rub(1)
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

// Translate performs the trice log task.
// Bytes are read with rc, according decoder.Encoding are translated into strings
// returns false on program end signal or true on hard read error.
func Translate(sw *emitter.TriceLineComposer, list *id.List, rc io.ReadCloser /*, hardReadError chan bool*/) bool {

	// activate selected encoding
	// var p translator.Translator // interface type
	switch Encoding {
	case "leg":
		dec := NewEscLegacyDecoder(list.ItemList, rc)
		for {
			err := run(sw, dec)
			if nil != err {
				time.Sleep(2 * time.Second)
				dec = NewEscLegacyDecoder(list.ItemList, rc) // read list again - it could have changed
			}
		}
	case "esc":
		dec := NewEscDecoder(list.ItemList, rc, bigEndian)
		for {
			err := run(sw, dec)
			if nil != err {
				time.Sleep(2 * time.Second)
				dec = NewEscDecoder(list.ItemList, rc, bigEndian) // read list again - it could have changed
			}
		}
	case "pack":
		dec := NewPackDecoder(list.ItemList, rc, bigEndian)
		for {
			err := run(sw, dec)
			if nil != err {
				time.Sleep(2 * time.Second)
				dec = NewPackDecoder(list.ItemList, rc, bigEndian) // read list again - it could have changed
			}
		}
	case "packl", "packL":
		dec := NewPackDecoder(list.ItemList, rc, littleEndian)
		for {
			err := run(sw, dec)
			if nil != err {
				time.Sleep(2 * time.Second)
				dec = NewPackDecoder(list.ItemList, rc, littleEndian) // read list again - it could have changed
			}
		}
	case "bare":
		dec := NewBareDecoder(list.ItemList, rc, bigEndian)
		for {
			err := run(sw, dec)
			if nil != err {
				time.Sleep(2 * time.Second)
				dec = NewBareDecoder(list.ItemList, rc, bigEndian) // read list again - it could have changed
			}
		}
	case "barel", "bareL":
		dec := NewBareDecoder(list.ItemList, rc, littleEndian)
		for {
			err := run(sw, dec)
			if nil != err {
				time.Sleep(2 * time.Second)
				dec = NewBareDecoder(list.ItemList, rc, littleEndian) // read list again - it could have changed
			}
		}
	case "wrap":
		dec := NewBareDecoder(list.ItemList, NewBareReaderFromWrap(rc), bigEndian)
		for {
			err := run(sw, dec)
			if nil != err {
				time.Sleep(2 * time.Second)
				dec = NewBareDecoder(list.ItemList, NewBareReaderFromWrap(rc), bigEndian) // read list again - it could have changed
			}
		}
	case "wrapl", "wrapL":
		dec := NewBareDecoder(list.ItemList, NewBareReaderFromWrap(rc), littleEndian)
		for {
			err := run(sw, dec)
			if nil != err {
				time.Sleep(2 * time.Second)
				dec = NewBareDecoder(list.ItemList, NewBareReaderFromWrap(rc), littleEndian) // read list again - it could have changed
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
	var sssiz int // to do: 1 for pack, 100 for esc
	if Encoding == "pack" {
		sssiz = 1
	} else {
		sssiz = 100
	}
	ss := make([]string, sssiz)
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

/*
func (p *decoding) triceS(cnt int) (n int, e error) { return }
func (p *decoding) trice0() (n int, e error)        { return }
func (p *decoding) trice81() (n int, e error)       { return }
func (p *decoding) trice82() (n int, e error)       { return }
func (p *decoding) trice83() (n int, e error)       { return }
func (p *decoding) trice84() (n int, e error)       { return }
func (p *decoding) trice85() (n int, e error)       { return }
func (p *decoding) trice86() (n int, e error)       { return }
func (p *decoding) trice87() (n int, e error)       { return }
func (p *decoding) trice88() (n int, e error)       { return }
func (p *decoding) trice161() (n int, e error)      { return }
func (p *decoding) trice162() (n int, e error)      { return }
func (p *decoding) trice163() (n int, e error)      { return }
func (p *decoding) trice164() (n int, e error)      { return }
func (p *decoding) trice321() (n int, e error)      { return }
func (p *decoding) trice322() (n int, e error)      { return }
func (p *decoding) trice323() (n int, e error)      { return }
func (p *decoding) trice324() (n int, e error)      { return }
func (p *decoding) trice641() (n int, e error)      { return }
func (p *decoding) trice642() (n int, e error)      { return }

func (d *decoding) sprintTrice(p *decoding, cnt int) (n int, e error) {
	// ID and count are ok
	switch p.trice.Type {
	case "TRICE0":
		return p.trice0()
	case "TRICE8_1":
		return p.trice81()
	case "TRICE8_2":
		return p.trice82()
	case "TRICE8_3":
		return p.trice83()
	case "TRICE8_4":
		return p.trice84()
	case "TRICE8_5":
		return p.trice85()
	case "TRICE8_6":
		return p.trice86()
	case "TRICE8_7":
		return p.trice87()
	case "TRICE8_8":
		return p.trice88()
	case "TRICE16_1":
		return p.trice161()
	case "TRICE16_2":
		return p.trice162()
	case "TRICE16_3":
		return p.trice163()
	case "TRICE16_4":
		return p.trice164()
	case "TRICE32_1":
		return p.trice321()
	case "TRICE32_2":
		return p.trice322()
	case "TRICE32_3":
		return p.trice323()
	case "TRICE32_4":
		return p.trice324()
	case "TRICE64_1":
		return p.trice641()
	case "TRICE64_2":
		return p.trice642()
	case "TRICE_S":
		return p.triceS(cnt)
	default:
		return p.outOfSync(fmt.Sprintf("Unexpected trice.Type %s", p.trice.Type))
	}
}
*/
