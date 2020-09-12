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

const (
	redBalk = "error:\ne:                                                      \nerror:                                                      \n"
)

var (
	// Verbose gives more information on output if set. This variable is set outside this package.
	Verbose bool
)

// Trice is the bare Trice data type for a Trice atom.
// A Trice starts with zero or several Trice atoms with ID==0 carrying parts of the Trice data payload.
// The last Trice atom of a Trice contains the Trice ID!=0 and the last part of the data payload.
type Trice = receiver.Trice

// TriceAtomsReceiver is the interface a trice receicer has to provide for a trice interpreter.
// The provided channels are read only channels.
type TriceAtomsReceiver interface {
	TriceAtomsChannel() <-chan []Trice
	IgnoredBytesChannel() <-chan []byte
}

// TriceTranslator uses the 2 TriceReceiver channels and global settings to compose a complete log line as one string.
// The string is transferred using the io.StringWriter interface.
type TriceTranslator struct {
	// If some error occured it is stored here. This makes error handling more handy.
	Err error

	// This channel is used to stop the TriceInterpreter
	// done chan int

	// values holds so far unused values for the next trice item.
	values []uint16

	// dictionary
	list *id.List

	// item is the next trice item ready for output, if item is not 0.
	item id.Item

	// lineComplete is set when a line is complete.
	//lineComplete bool

	// sw is the interface for writing a whole line to the display.
	// When a line is complete, its []string slice is converted to one single string for output.
	// This simplifies the output interface and avoids partial lines to be mixed from several sources.
	//sw io.StringWriter
}

// String is the method for displaying the current TriceTranslator instance.
func (p *TriceTranslator) String() string {
	s := fmt.Sprintf("ID=%d=%x", p.item.ID, p.item.ID)
	s += fmt.Sprintf("values in hex: ")
	for _, n := range p.values {
		s += fmt.Sprintf("%x ", n)
	}
	return s
}

// NewSimpleTrices gets its data from the TriceAtomsReceiver interface tr.
// It uses the io.StringWriter interface sw to write the trice strings looked up in id.List
// and enhanced with the printed values. Each single trice string is written separately with sw.
// It does not evaluate the TriceInterpreter ignored bytes ('Simple'Trices).
func NewSimpleTrices(sw io.StringWriter, list *id.List, tr TriceAtomsReceiver) *TriceTranslator {
	p := &TriceTranslator{}
	//p.atomsChannel = tr.TriceAtomsChannel()
	//p.ignoredChannel = tr.IgnoredBytesChannel()
	//p.sw = sw
	//p.atoms = make([]Trice, 0, 1000)
	//p.ignored = make([]byte, 0, 1000)
	p.values = make([]uint16, 0, 100)
	//p.done = make(chan int)
	p.list = list

	go func() {
		for {
			p.ErrorFatal()
			select {
			case atoms := <-tr.TriceAtomsChannel():
				for _, trice := range atoms {
					s := p.translate(trice)
					_, p.Err = sw.WriteString(s)
				}
			case ignored := <-tr.IgnoredBytesChannel():
				// TODO: evaluate other protocols here
				if Verbose {
					s := fmt.Sprintln("error:Found", len(ignored), "ignored byte(s):", ignored)
					_, p.Err = sw.WriteString(s)
				}

				//case <-p.done:
				//	return // end of life
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

//// reSyncAtoms does remove one byte in the atoms slice.
//// f true indicates that the last 2 values are to be removed
//func (p *TriceTranslator) reSyncAtoms(f bool) {
//	if f {
//		p.values = p.values[:0] // discard accumulated values
//	}
//	//fmt.Print("?") //how to?
//
//}

// translate evaluates trice, p.values and not p.ignored and tries to generate a string.
// If an internal error state occured it discards all accumulated data and clears the error.
// On return it delivers an emty string if not enough data yet for the next string.
// This is usually the case when trice.ID is 0 and only data payload is to store.
func (p *TriceTranslator) translate(trice Trice) (s string) {
	if nil != p.Err {
		s = redBalk + fmt.Sprintln("+++++++++++++++++++++++", p.Err, p.values)
		p.values = p.values[:0]
		p.Err = nil
		return
	}
	if 0x89ab == trice.ID { // discard sync trice
		if 0xcdef != trice.Value {
			if Verbose {
				s = fmt.Sprintln("error:trice", trice, "is invalid. Must have payload [205 239].")
			}
			p.values = p.values[:0]
		}
		//s = fmt.Sprintln("debug:~")
		return
	}
	p.values = append(p.values, trice.Value) // append the uint16 date of the current trice
	if 0 == trice.ID {                       // trice atom has only data
		return
	}
	index := p.list.Index(int(trice.ID))
	if index < 0 { // unknown trice.ID
		p.Err = nil
		s = redBalk + fmt.Sprintf("error: unknown trice.ID\n")
		s += fmt.Sprintln(p)
		p.values = p.values[:0]
		return
	}
	p.item = p.list.Item(index)
	s = p.addValues()
	p.values = p.values[:0]
	return
}

// addValues wworks fine with %x, %d and %o but NOT with %u for now
// %x is emitted in Go signed!
// For %u a format string parsing is needed to perform the correct casts.
func (p *TriceTranslator) addValues() (s string) {
	f, _, _ := langCtoGoFmtStingConverter(p.item.FmtStrg) // todo
	var v0, v1, v2, v3 int16
	var w0, w1, w2, w3 int32
	var l0, l1 int64

	p.evalLen()
	if nil != p.Err {
		if Verbose {
			s = p.Err.Error() + fmt.Sprint("The accumulated data are not matching the trice.ID. Discarding: ")
			s += fmt.Sprintln(p)
		}
		p.values = p.values[:0]
		p.Err = nil
		return
	}

	switch p.item.FmtType {
	case "TRICE0":
		s = f
	case "TRICE8_1":
		if "%s" != f { // normal case, %s is for dynamic string transfer
			v0 := int8(p.values[0])
			s = fmt.Sprintf(f, v0)
		}
	case "TRICE8_2":
		v0 := int8(p.values[0])
		v1 := int8(p.values[0] >> 8)
		s = fmt.Sprintf(f, v0, v1)
	case "TRICE8_3":
		v0 := int8(p.values[0])
		v1 := int8(p.values[0] >> 8)
		v2 := int8(p.values[1])
		s = fmt.Sprintf(f, v0, v1, v2)
	case "TRICE8_4":
		v0 := int8(p.values[0])
		v1 := int8(p.values[0] >> 8)
		v2 := int8(p.values[1])
		v3 := int8(p.values[1] >> 8)
		s = fmt.Sprintf(f, v0, v1, v2, v3)
	case "TRICE8_5":
		v0 := int8(p.values[0])
		v1 := int8(p.values[0] >> 8)
		v2 := int8(p.values[1])
		v3 := int8(p.values[1] >> 8)
		v4 := int8(p.values[2])
		s = fmt.Sprintf(f, v0, v1, v2, v3, v4)
	case "TRICE8_6":
		v0 := int8(p.values[0])
		v1 := int8(p.values[0] >> 8)
		v2 := int8(p.values[1])
		v3 := int8(p.values[1] >> 8)
		v4 := int8(p.values[2])
		v5 := int8(p.values[2] >> 8)
		s = fmt.Sprintf(f, v0, v1, v2, v3, v4, v5)
	case "TRICE8_7":
		v0 := int8(p.values[0])
		v1 := int8(p.values[0] >> 8)
		v2 := int8(p.values[1])
		v3 := int8(p.values[1] >> 8)
		v4 := int8(p.values[2])
		v5 := int8(p.values[2] >> 8)
		v6 := int8(p.values[3])
		s = fmt.Sprintf(f, v0, v1, v2, v3, v4, v5, v6)
	case "TRICE8_8":
		v0 := int8(p.values[0])
		v1 := int8(p.values[0] >> 8)
		v2 := int8(p.values[1])
		v3 := int8(p.values[1] >> 8)
		v4 := int8(p.values[2])
		v5 := int8(p.values[2] >> 8)
		v6 := int8(p.values[3])
		v7 := int8(p.values[3] >> 8)
		s = fmt.Sprintf(f, v0, v1, v2, v3, v4, v5, v6, v7)
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
func (p *TriceTranslator) evalLen() {
	t := p.values
	switch p.item.FmtType {
	case "TRICE0", "TRICE8_1", "TRICE8_2", "TRICE16_1":
		if 1 != len(t) {
			p.Err = fmt.Errorf("err:len %d != 1 ", len(t))
		}
	case "TRICE8_3", "TRICE8_4", "TRICE16_2", "TRICE32_1":
		if 2 != len(t) {
			p.Err = fmt.Errorf("err:len %d != 2 ", len(t))
		}
	case "TRICE8_5", "TRICE8_6", "TRICE16_3":
		if 3 != len(t) {
			p.Err = fmt.Errorf("err:len %d != 3 ", len(t))
		}
	case "TRICE8_7", "TRICE8_8", "TRICE16_4", "TRICE32_2", "TRICE64_1":
		if 4 != len(t) {
			p.Err = fmt.Errorf("err:len %d != 4 ", len(t))
		}
	case "TRICE32_3":
		if 6 != len(t) {
			p.Err = fmt.Errorf("err:len %d != 6 ", len(t))
		}
	case "TRICE32_4", "TRICE64_2":
		if 8 != len(t) {
			p.Err = fmt.Errorf("err:len %d != 8 ", len(t))
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
// func (p *TriceTranslator) Stop() {
// 	p.done <- 0
// }
