// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package translator

import (
	"encoding/binary"
	"encoding/hex"
	"fmt"
	"io"
	"log"
	"path/filepath"
	"runtime"
	"time"

	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/internal/receiver"
)

// Trice is the bare Trice data type for a Trice atom.
// A Trice starts with zero or several Trice atoms with ID==0 carrying parts of the Trice data payload.
// The last Trice atom of a Trice contains the Trice ID!=0 and the last part of the data payload.
type Trice = receiver.Trice

// TriceAtomsReceiver is the interface a trice receicer has to provide for a trice interpreter.
// The provided channels are read only channels
type TriceAtomsReceiver interface {
	TriceAtomsChannel() <-chan []Trice
	IgnoredBytesChannel() <-chan []byte
}

// TriceTranslator uses the 2 TriceReceiver channels and global settings to compose a complete log line as one string.
// The string is transferred using the io.StringWriter interface.
type TriceTranslator struct {
	// If some error occured it is stored here. This makes error handling more handy.
	Err error

	// This is the receive channel for trice atomsChannel.
	atomsChannel <-chan []Trice

	// This is the receive channel for trice receiver ignoredChannel bytes.
	ignoredChannel <-chan []byte

	// This channel is used to stop the TriceInterpreter
	done chan int

	// atoms holds read trice atoms not processed so far.
	atoms []Trice

	// ignored holds so far unused values for the next trice item.
	ignored []byte

	// values holds so far unused values for the next trice item.
	values []byte

	//
	list *id.List

	// item is the next trice item ready for output, if item is not 0.
	item id.Item

	// lineComplete is set when a line is complete.
	lineComplete bool

	// sw is the interface for writing a whole line to the display.
	// When a line is complete, its []string slice is converted to one single string for output.
	// This simplifies the output interface and avoids partial lines to be mixed from several sources.
	sw io.StringWriter
}

// NewSimpleTrices gets its data from the TriceAtomsReceiver interface tr.
// It uses the io.StringWriter interface sw to write the trice strings looked up in id.List
// and enhanced with the printed values. Each single trice string is written separately with sw.
// It does not evaluate the TriceInterpreter.ignored bytes.
func NewSimpleTrices(sw io.StringWriter, list *id.List, tr TriceAtomsReceiver) *TriceTranslator {
	p := &TriceTranslator{}
	p.atomsChannel = tr.TriceAtomsChannel()
	p.ignoredChannel = tr.IgnoredBytesChannel()
	p.sw = sw
	p.atoms = make([]Trice, 0, 1000)
	p.ignored = make([]byte, 0, 1000)
	p.values = make([]byte, 0, 100)
	p.done = make(chan int)
	p.list = list

	go func() {
		for {
			p.ErrorFatal()
			select {
			case a, _ := <-p.atomsChannel:
				//if !ok {
				//	return // stop when atoms channel was closed
				//}
				p.atoms = append(p.atoms, a...)
			case b := <-p.ignoredChannel:
				p.ignored = append(p.ignored, b...)
			case <-p.done:
				return // end of life
			}
			//fmt.Println("vales:", p.values, "atoms:", p.atoms, "ignored:", p.ignored)
			for 0 < len(p.atoms) {
				s := p.translate()
				_, p.Err = sw.WriteString(s)
			}
			if 0 < len(p.ignored) {
				// TODO: evaluate other protocols here
				s := fmt.Sprintln("ATTENTION: found ignored bytes:", p.ignored)
				time.Sleep(1 * time.Second)
				_, p.Err = sw.WriteString(s)
				p.ignored = p.ignored[:0]
			}
		}
	}()
	return p
}

// ErrorFatal ends in osExit(1) if p.err not nil.
func (p *TriceTranslator) ErrorFatal() {
	if nil == p.Err {
		return
	}
	_, file, line, _ := runtime.Caller(1)
	log.Fatal(p.Err, filepath.Base(file), line)
}

// translate evaluates p.atoms, p.values and p.ignored and tries to generate a string.
// If an internal error state occured it discards all accumulated data and clears the error.
// On return it delivers an emty string if not enough data yet for the next string.
// This is usually the case when trice.ID is 0 and only data payload is to store.
func (p *TriceTranslator) translate() (s string) {
	if nil != p.Err {
		s = fmt.Sprintln(p.Err, p.values, p.atoms, p.ignored)
		p.values = p.values[:0]
		p.atoms = p.atoms[:0]
		p.ignored = p.ignored[:0]
		p.Err = nil
		return
	}
	if 0 == len(p.atoms) {
		return
	}
	trice := p.atoms[0]
	p.atoms = p.atoms[1:]
	if 0x89ab == trice.ID { // discard sync trice
		var hi, lo int
		nativeEndian := binary.LittleEndian // TODO: put in file endian_amd64.go
		if binary.LittleEndian == nativeEndian {
			hi = 1
			lo = 0
		} else { // littleEndian
			hi = 0
			lo = 1
		}
		if 0xcd != trice.Value[hi] || 0xef != trice.Value[lo] {
			s = fmt.Sprintln("WARNING:", trice, "is invalid. Must have payload [205 239].")
			time.Sleep(1 * time.Second)
		}
		return
	}
	p.values = append(p.values, trice.Value[:]...)
	if 0 == trice.ID { // only data
		return
	}
	var index int
	index, p.Err = p.list.Index(int(trice.ID))
	if nil != p.Err {
		p.Err = nil
		s = fmt.Sprintf("WARNING: trice.ID %d not found, discarding the 2 id bytes and the data bytes %04x %s\n", trice.ID, trice.ID, hex.EncodeToString(p.values))
		time.Sleep(1 * time.Second)
		p.values = p.values[:0]
		return
	}
	p.item = id.List[index]
	s = p.emitter()
	p.values = p.values[:0]
	return
}

// emmiter wworks fine with %x, %d and %o but NOT with %u for now
// %x is emitted in Go signed!
// For %u a format string parsing is needed to perform the correct casts.
func (p *TriceTranslator) emitter() (s string) {
	it := p.item
	prm := p.values
	f, _, _ := langCtoGoFmtStingConverter(it.FmtStrg) // todo
	var v0, v1, v2, v3 int16
	var w0, w1, w2, w3 int32
	var l0, l1 int64
	p.evalLen()
	switch it.FmtType {
	case "TRICE0":
		s = f
	case "TRICE8_1":
		if "%s" != f { // normal case
			s = fmt.Sprintf(f, int8(prm[0]))
		}
	case "TRICE8_2":
		s = fmt.Sprintf(f, int8(prm[0]), int8(prm[1]))
	case "TRICE8_3":
		s = fmt.Sprintf(f, int8(prm[0]), int8(prm[1]), int8(prm[2]))
	case "TRICE8_4":
		s = fmt.Sprintf(f, int8(prm[0]), int8(prm[1]), int8(prm[2]), int8(prm[3]))
	case "TRICE8_5":
		s = fmt.Sprintf(f, int8(prm[0]), int8(prm[1]), int8(prm[2]), int8(prm[3]), int8(prm[4]))
	case "TRICE8_6":
		s = fmt.Sprintf(f, int8(prm[0]), int8(prm[1]), int8(prm[2]), int8(prm[3]), int8(prm[4]), int8(prm[5]))
	case "TRICE8_7":
		s = fmt.Sprintf(f, int8(prm[0]), int8(prm[1]), int8(prm[2]), int8(prm[3]), int8(prm[4]), int8(prm[5]), int8(prm[6]))
	case "TRICE8_8":
		s = fmt.Sprintf(f, int8(prm[0]), int8(prm[1]), int8(prm[2]), int8(prm[3]), int8(prm[4]), int8(prm[5]), int8(prm[6]), int8(prm[7]))
	case "TRICE16_1":
		v0 = int16(binary.LittleEndian.Uint16(prm[0:2]))
		s = fmt.Sprintf(f, v0)
	case "TRICE16_2":
		v0 = int16(binary.LittleEndian.Uint16(prm[0:2]))
		v1 = int16(binary.LittleEndian.Uint16(prm[2:4]))
		s = fmt.Sprintf(f, v0, v1)
	case "TRICE16_3":
		v0 = int16(binary.LittleEndian.Uint16(prm[0:2]))
		v1 = int16(binary.LittleEndian.Uint16(prm[2:4]))
		v2 = int16(binary.LittleEndian.Uint16(prm[4:6]))
		s = fmt.Sprintf(f, v0, v1, v2)
	case "TRICE16_4":
		v0 = int16(binary.LittleEndian.Uint16(prm[0:2]))
		v1 = int16(binary.LittleEndian.Uint16(prm[2:4]))
		v2 = int16(binary.LittleEndian.Uint16(prm[4:6]))
		v3 = int16(binary.LittleEndian.Uint16(prm[6:8]))
		s = fmt.Sprintf(f, v0, v1, v2, v3)
	case "TRICE32_1":
		w0 = int32(binary.LittleEndian.Uint32(prm[0:4]))
		s = fmt.Sprintf(f, w0)
	case "TRICE32_2":
		w0 = int32(binary.LittleEndian.Uint32(prm[0:4]))
		w1 = int32(binary.LittleEndian.Uint32(prm[4:8]))
		s = fmt.Sprintf(f, w0, w1)
	case "TRICE32_3":
		w0 = int32(binary.LittleEndian.Uint32(prm[0:4]))
		w1 = int32(binary.LittleEndian.Uint32(prm[4:8]))
		w2 = int32(binary.LittleEndian.Uint32(prm[8:12]))
		s = fmt.Sprintf(f, w0, w1, w2)
	case "TRICE32_4":
		w0 = int32(binary.LittleEndian.Uint32(prm[0:4]))
		w1 = int32(binary.LittleEndian.Uint32(prm[4:8]))
		w2 = int32(binary.LittleEndian.Uint32(prm[8:12]))
		w3 = int32(binary.LittleEndian.Uint32(prm[12:16]))
		s = fmt.Sprintf(f, w0, w1, w2, w3)
	case "TRICE64_1":
		l0 = int64(binary.LittleEndian.Uint64(prm[0:8]))
		s = fmt.Sprintf(f, l0)
	case "TRICE64_2":
		l0 = int64(binary.LittleEndian.Uint64(prm[0:8]))
		l1 = int64(binary.LittleEndian.Uint64(prm[8:16]))
		s = fmt.Sprintf(f, l0, l1)
	default:
		p.Err = fmt.Errorf("Unknown FmtType %s", it.FmtType)
	}
	return
}

// evalLen checks if byte buffer t has appropriate length to id.item it
func (p *TriceTranslator) evalLen() {
	it := p.item
	t := p.values
	switch it.FmtType {
	case "TRICE0", "TRICE8_1", "TRICE8_2", "TRICE16_1":
		if 2 != len(t) {
			p.Err = fmt.Errorf("len %d != 2", len(t))
		}
	case "TRICE8_3", "TRICE8_4", "TRICE16_2", "TRICE32_1":
		if 4 != len(t) {
			p.Err = fmt.Errorf("len %d != 4", len(t))
		}
	case "TRICE8_5", "TRICE8_6", "TRICE16_3":
		if 6 != len(t) {
			p.Err = fmt.Errorf("len %d != 6", len(t))
		}
	case "TRICE8_7", "TRICE8_8", "TRICE16_4", "TRICE32_2", "TRICE64_1":
		if 8 != len(t) {
			p.Err = fmt.Errorf("len %d != 8", len(t))
		}
	case "TRICE32_3":
		if 12 != len(t) {
			p.Err = fmt.Errorf("len %d != 12", len(t))
		}
	case "TRICE32_4", "TRICE64_2":
		if 16 != len(t) {
			p.Err = fmt.Errorf("len %d != 16", len(t))
		}
	}
}

// parse lang C formatstring for %u and replace them with %d and extend the
// returned slice with 0 for each %d, %c, %x and 1 for each converted %u
// http://www.cplusplus.com/reference/cstdio/printf/
//
// Changes:        C    Go
// ---------------------------
// int 	           %i   %d replace i with d
// unsigned int    %u   %d replace u with d and uint()
//
// No changes:     C    Go
// ---------------------------
// decimal         %d   %d
// hexadezimal     %x   %x
// HEXADECIMAL     %X   %X
// octal           %o   %o
// float           %f   %f
// char            %c   %c
// (void *)        %p   %p
func langCtoGoFmtStingConverter(f string) (s string, u []bool, err error) {
	s = f // TODO: parse f for %*u, replace with %*d and sett []bool values accordingly
	return
}

// Stop ends life of TriceInterpreter
func (p *TriceTranslator) Stop() {
	p.done <- 0
}
