// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package translator

import (
	"fmt"
	"io"
	"log"
	"path/filepath"
	"runtime"

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

	// ignored holds so far unused bytes for the next trice item.
	ignored []byte

	// values holds so far unused values for the next trice item.
	values []uint16

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
	p.values = make([]uint16, 0, 100)
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
				s := fmt.Sprintln("e:\ne:                           \ne: found ignored byte:", p.ignored, "\ne:                           ")
				//time.Sleep(1 * time.Second)
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

// reSyncAtoms does remove one byte in the atoms slice.
// f true indicates that the last 2 values are to be removed
func (p *TriceTranslator) reSyncAtoms(f bool) {
	if f {
		p.values = p.values[:0] // discard accumulated values
	}
	//fmt.Print("?") //how to?

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
	if 0 == len(p.atoms) { // nothing to do
		return
	}
	defer func() { // remove one atom after successully processing
		p.atoms = p.atoms[1:]
	}()
	trice := p.atoms[0]
reSyncAtomsDone:
	if 0 == len(p.atoms) { // nothing to do
		return // This can be the case after removing some bytes
	}

	// discard sync trice
	if 0x89ab == trice.ID {
		if 0xcdef != trice.Value {
			s = fmt.Sprintln("e:                                   \ne: ", trice, "is invalid. Must have payload [205 239]. \ne:                                     ")
		}
		s = fmt.Sprintln("att:~")
		return
	}
	p.values = append(p.values, trice.Value) // append the uint16 date of the current trice
	if 0 == trice.ID {                       // trice atom has only data
		return
	}
	var index int
	index = p.list.Index(int(trice.ID))
	if index < 0 { // unknown trice.ID
		p.Err = nil
		s = fmt.Sprintf("e:                                                                       \ne: trice.ID %d not found, trying reSync... \ne:                                                                     \n", trice.ID)
		p.reSyncAtoms(true)
		goto reSyncAtomsDone
	}

	if err := p.evalLen(); nil != err { // The accumulated data are not matching the trice.ID
		s = err.Error()
		s += fmt.Sprintf("e:                                                                       \ne trice.ID %d does not match values %x - ignoring both \ne:                                                         \n", trice.ID, p.values)
		p.values = p.values[:0]
		return
	}
	p.item = p.list.Item(index)
	s = p.emitter()
	p.values = p.values[:0]
	return
}

// emmiter wworks fine with %x, %d and %o but NOT with %u for now
// %x is emitted in Go signed!
// For %u a format string parsing is needed to perform the correct casts.
func (p *TriceTranslator) emitter() (s string) {
	f, _, _ := langCtoGoFmtStingConverter(p.item.FmtStrg) // todo
	var v0, v1, v2, v3 int16
	var w0, w1, w2, w3 int32
	var l0, l1 int64

	prm := p.values
	switch p.item.FmtType {
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
		v0 = int16(p.values[0])
		s = fmt.Sprintf(f, v0)
	case "TRICE16_2":
		v0 = int16(p.values[0])
		v1 = int16(p.values[1])
		s = fmt.Sprintf(f, v0, v1)
	case "TRICE16_3":
		v0 = int16(p.values[0])
		v1 = int16(p.values[1])
		v2 = int16(p.values[2])
		s = fmt.Sprintf(f, v0, v1, v2)
	case "TRICE16_4":
		v0 = int16(p.values[0])
		v1 = int16(p.values[1])
		v2 = int16(p.values[2])
		v3 = int16(p.values[3])
		s = fmt.Sprintf(f, v0, v1, v2, v3)
	case "TRICE32_1":
		w0 = (int32(p.values[1]) << 16) | int32(p.values[0]) // daLo ist tranmitted first -> idx 0
		s = fmt.Sprintf(f, w0)
	case "TRICE32_2":
		w0 = (int32(p.values[1]) << 16) | int32(p.values[0]) // first param is transmitted first
		w1 = (int32(p.values[3]) << 16) | int32(p.values[2])
		s = fmt.Sprintf(f, w0, w1)
	case "TRICE32_3":
		w0 = (int32(p.values[1]) << 16) | int32(p.values[0])
		w1 = (int32(p.values[3]) << 16) | int32(p.values[2])
		w2 = (int32(p.values[5]) << 16) | int32(p.values[4])
		s = fmt.Sprintf(f, w0, w1, w2)
	case "TRICE32_4":
		w0 = (int32(p.values[1]) << 16) | int32(p.values[0])
		w1 = (int32(p.values[3]) << 16) | int32(p.values[2])
		w2 = (int32(p.values[5]) << 16) | int32(p.values[4])
		w3 = (int32(p.values[7]) << 16) | int32(p.values[6])
		s = fmt.Sprintf(f, w0, w1, w2, w3)
	case "TRICE64_1":
		l0 = (int64(p.values[3]) << 48) | (int64(p.values[2]) << 32) | (int64(p.values[1]) << 16) | int64(p.values[0]) // hh hl lh ll
		s = fmt.Sprintf(f, l0)
	case "TRICE64_2":
		l0 = (int64(p.values[3]) << 48) | (int64(p.values[2]) << 32) | (int64(p.values[1]) << 16) | int64(p.values[0]) // hh hl lh ll
		l1 = (int64(p.values[7]) << 48) | (int64(p.values[6]) << 32) | (int64(p.values[5]) << 16) | int64(p.values[4]) // hh hl lh ll
		s = fmt.Sprintf(f, l0, l1)
	default:
		p.Err = fmt.Errorf("Unknown FmtType %s", p.item.FmtType)
	}
	return
}

// evalLen checks if byte buffer t has appropriate length to id.item it
func (p *TriceTranslator) evalLen() (err error) {
	t := p.values
	switch p.item.FmtType {
	case "TRICE0", "TRICE8_1", "TRICE8_2", "TRICE16_1":
		if 2 != len(t) {
			err = fmt.Errorf("len %d != 2", len(t))
		}
	case "TRICE8_3", "TRICE8_4", "TRICE16_2", "TRICE32_1":
		if 4 != len(t) {
			err = fmt.Errorf("len %d != 4", len(t))
		}
	case "TRICE8_5", "TRICE8_6", "TRICE16_3":
		if 6 != len(t) {
			err = fmt.Errorf("len %d != 6", len(t))
		}
	case "TRICE8_7", "TRICE8_8", "TRICE16_4", "TRICE32_2", "TRICE64_1":
		if 8 != len(t) {
			err = fmt.Errorf("len %d != 8", len(t))
		}
	case "TRICE32_3":
		if 12 != len(t) {
			err = fmt.Errorf("len %d != 12", len(t))
		}
	case "TRICE32_4", "TRICE64_2":
		if 16 != len(t) {
			err = fmt.Errorf("len %d != 16", len(t))
		}
	}
	err = nil
	return
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
