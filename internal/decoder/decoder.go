// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package decoder provides several decoders for differently encoded trice streams.
package decoder

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"io"
	"os"
	"os/signal"
	"syscall"
	"time"

	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/pkg/msg"
)

const (
	// receive and sync buffer size
	defaultSize  = 4096
	littleEndian = true
	bigEndian    = false
)

var (
	// Verbose gives mor information on output if set. The value is injected from main packages.
	Verbose bool

	// Encoding describes the way the byte stream is coded.
	Encoding string

	// TestTableMode is a special option for easy decoder test table generation.
	TestTableMode bool
)

// newDecoder abstracts the function type for a new decoder.
type newDecoder func(lut id.TriceIDLookUp, in io.Reader, endian bool) Decoder

// Decoder is providing a byte reader returning decoded trice's.
// setInput allows switching the input stream to a different source.
type Decoder interface {
	io.Reader
	setInput(io.Reader)
}

// decoderData is the common data struct for all decoders.
type decoderData struct {
	in                io.Reader        // inner reader
	syncBuffer        []byte           // unprocessed bytes hold for next cycle
	endian            bool             // littleEndian or bigEndian
	lut               id.TriceIDLookUp // id look-up map for translation
	trice             id.TriceFmt      //id.TriceFmt // received trice
	b                 []byte           // read buffer
	bc                int              // trice specific bytes count
	lastInnerRead     time.Time
	innerReadInterval time.Duration
}

// type intervalReader struct {
// 	r io.ReadCloser
// }
//
// // NewBytesViewer returns a ReadCloser `in` which is internally using reader `from`.
// // Calling the `in` Read method leads to internally calling the `from` Read method
// // but lets to do some additional action like logging
// func newIntervalReader(from io.ReadCloser) (in io.ReadCloser) {
// 	return &intervalReader{from}
// }
//
// func (p *intervalReader) Read(buf []byte) (count int, err error) {
// 	count, err = p.r.Read(buf)
// 	return
// }
//
// // Close is needed to satisfy the ReadCloser interface.
// func (p *intervalReader) Close() error { return nil }
//
// // setInput allows switching the input stream to a different source.
// //
// // This function is for easier testing with cycle counters.
// func (p *decoderData) setInput(r io.ReadCloser) {
// 	//	p.in = newIntervalReader(r)
// 	p.in = r
// }

// setInput allows switching the input stream to a different source.
//
// This function is for easier testing with cycle counters.
func (p *decoderData) setInput(r io.Reader) {
	p.in = r
}

// Translate performs the trice log task.
// Bytes are read with rc. Then according decoder.Encoding they are translated into strings.
// Each read returns the amount of bytes for one trice. rc is called on every
// Translate returns true on io.EOF or false on hard read error or sigterm.
func Translate(sw *emitter.TriceLineComposer, lut id.TriceIDLookUp, rc io.ReadCloser) bool {

	var dec Decoder //io.Reader
	switch Encoding {
	case "esc":
		dec = NewEscDecoder(lut, rc, bigEndian)
	case "pack":
		dec = NewPackDecoder(lut, rc, bigEndian)
	case "packl", "packL":
		dec = NewPackDecoder(lut, rc, littleEndian)
	case "pack2":
		dec = NewPack2Decoder(lut, rc, bigEndian)
	case "pack2l", "pack2L":
		dec = NewPack2Decoder(lut, rc, littleEndian)
	case "bare":
		dec = NewBareDecoder(lut, rc, bigEndian)
	case "barel", "bareL":
		dec = NewBareDecoder(lut, rc, littleEndian)
	case "wrap":
		dec = NewBareDecoder(lut, NewBareReaderFromWrap(rc), bigEndian)
	case "wrapl", "wrapL":
		dec = NewBareDecoder(lut, NewBareReaderFromWrap(rc), littleEndian)
	//case "bareXTEAEncrypted", "wrapXTEAEncrypted":
	//	msg.FatalErr(cipher.SetUp())
	//	fallthrough
	default:
		fmt.Println("unknown encoding ", Encoding)
		//return false // stop
	}

	// prepare CTRL-C shutdown reaction
	sigs := make(chan os.Signal, 1)
	signal.Notify(sigs, syscall.SIGINT, syscall.SIGTERM)

	// intermediate trice string buffer for a single trice
	b := make([]byte, defaultSize)
outer:
	for {
		select {
		case sig := <-sigs: // wait for a signal
			if Verbose {
				fmt.Println("####################################", sig, "####################################")
			}
			return false // end
		default:

			n, err := dec.Read(b) // Code to measure

			if io.EOF == err {
				if Verbose {
					fmt.Println(err)
				}
				if Verbose {
					fmt.Println("WAITING...")
				}
				time.Sleep(100 * time.Millisecond) // limit try again speed
				continue outer                     // read again
			}
			if nil != err {
				if Verbose {
					fmt.Println(err)
				}
				return true // try again
			}
			start := time.Now()
			m, err := sw.Write(b[:n])
			duration := time.Since(start).Milliseconds()
			if duration > 100 {
				fmt.Println("TriceLineComposer.Write duration =", duration, "ms.")
			}
			msg.InfoOnErr(fmt.Sprintln("sw.Write wrote", m, "bytes"), err)
		}
	}
}

/*
func run0(sw *emitter.TriceLineComposer, sr StringsReader) error {
	var size int // to do: 1 for pack, 100 for esc
	if Encoding == "pack" {
		size = 1
	} else {
		size = 100
	}
	ss := make([]string, size)
	n, err := sr.StringsRead(ss)
	if nil != err && io.EOF != err {
		return err
	}
	for i := range ss[:n] {
		sw.WriteString(ss[i])
	}
	return nil
}
*/

// readU16 returns the 2 b bytes as uint16 according the specified endianness
func (p *decoderData) readU16(b []byte) uint16 {
	if littleEndian == p.endian {
		return binary.LittleEndian.Uint16(b)
	}
	return binary.BigEndian.Uint16(b)
}

// readU32 returns the 4 b bytes as uint32 according the specified endianness
func (p *decoderData) readU32(b []byte) uint32 {
	if littleEndian == p.endian {
		return binary.LittleEndian.Uint32(b)
	}
	return binary.BigEndian.Uint32(b)
}

// writeU32 returns the 4 bytes as uint32 in b according the specified endianness
func (p *decoderData) writeU32(v uint32) (b *bytes.Buffer) {
	var err error
	b = new(bytes.Buffer)
	if littleEndian == p.endian {
		err = binary.Write(b, binary.LittleEndian, v)
	} else {
		err = binary.Write(b, binary.BigEndian, v)
	}
	msg.InfoOnErr("binary.Write failed:", err)
	return
}

// readU64 returns the 8 b bytes as uint64 according the specified endianness
func (p *decoderData) readU64(b []byte) uint64 {
	if littleEndian == p.endian {
		return binary.LittleEndian.Uint64(b)
	}
	return binary.BigEndian.Uint64(b)
}

// rub removes leading bytes from sync buffer
func (p *decoderData) rub(n int) {
	if TestTableMode {
		if emitter.NextLine {
			emitter.NextLine = false
			fmt.Printf("{ []byte{ ")
		}
		for _, b := range p.syncBuffer[0:n] { // just to see trice bytes per trice
			fmt.Printf("%3d,", b)
		}
	}
	p.syncBuffer = p.syncBuffer[n:]
}

// rubWithLongCount removes leading bytes from sync buffer but considers long count bytes
// in case of TestTableMode to generate correct test table data.
func (p *decoderData) rubWithLongCount(n, count int) {
	if TestTableMode {
		if emitter.NextLine {
			emitter.NextLine = false
			fmt.Printf("{ []byte{ ")
		}
		for _, b := range p.syncBuffer[0:4] { // just to see trice bytes per trice
			fmt.Printf("%3d,", b)
		}
		if count > 12 { // restore long count transfer bytes
			hi := uint32(count << 16)
			lo := uint16(^count)
			countTransfer := hi | uint32(lo)
			buf := p.writeU32(countTransfer)
			for _, b := range buf.Bytes() {
				fmt.Printf("%d,", b)
			}
		}
		for _, b := range p.syncBuffer[4:n] { // just to see trice bytes per trice
			fmt.Printf("%3d,", b)
		}
	}
	p.syncBuffer = p.syncBuffer[n:]
}

func (p *decoderData) outOfSync(msg string) (n int, e error) {
	cnt := p.bc
	if cnt > 20 {
		cnt = 20
	}
	n = copy(p.b, fmt.Sprintln("error:", msg, "ignoring first byte", p.syncBuffer[0:cnt]))
	p.rub(1)
	return
}
