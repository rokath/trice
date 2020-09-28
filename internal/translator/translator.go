// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package translator converts...
package translator

import (
	"encoding/binary"
	"fmt"
	"io"
	"log"
	"path/filepath"
	"runtime"
	"time"

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

// translator is the common data cantainmer with the common methods for BareTranslator, EscTranslator and the like.
type translator struct {
	list     *id.List
	item     id.Item // item is the next trice item ready for output.
	savedErr error
	done     chan int // This channel is used to stop the TriceInterpreter

	//////////////////////////////////////////////////////////////////////
	// DEBUG
	prev     int64
	errCount int
	//
	//////////////////////////////////////////////////////////////////////
}

// Translator is the common interface for BareTranslator, EscTranslator and the like.
type Translator interface {
	SavedError() error
	Done() chan int
}

func (p *translator) SavedError() error {
	return p.savedErr
}

func (p *translator) Done() chan int {
	return p.done
}

// ErrorFatal ends in osExit(1) if p.err not nil.
func (p *translator) ErrorFatal() {
	if nil == p.savedErr {
		return
	}
	_, file, line, _ := runtime.Caller(1)
	log.Fatal(p.savedErr, filepath.Base(file), line)
}

// BareTranslator ...
type BareTranslator struct {
	translator

	// values holds so far unused values for the next trice item.
	values []uint16
}

// EscTranslator ...
type EscTranslator struct {
	translator

	// inner bytes reader
	in io.Reader

	// inner string writer
	sw io.StringWriter

	syncBuffer []byte
}

// NewEscTrices uses in for reception and sw for writing.
// It collects trice bytes to a complete esc trice message, generates the appropriate string using list and writes it to sw.
// EC LC IH IL ...
func NewEscTrices(sw io.StringWriter, list *id.List, in io.ReadCloser) *EscTranslator {
	p := new(EscTranslator)
	p.sw = sw
	p.list = list
	p.in = in
	p.syncBuffer = make([]byte, 0, 1000)
	p.done = make(chan int)
	go func() {
		for {
			select {
			case <-p.done:
				if Verbose {
					fmt.Println("end of esc translator life")
				}
				return

			case <-time.After(1 * time.Millisecond): // todo: trigger from fileWatcher
				if io.EOF == p.savedErr {
					p.savedErr = nil
				}
				s := p.readEsc()
				_, p.savedErr = sw.WriteString(s)
			}
		}

	}()
	return p
}

func (p *EscTranslator) readEsc() (s string) {
	p.ErrorFatal()
	var n int
	rb := make([]byte, 100)
	n, p.savedErr = p.in.Read(rb)
	p.syncBuffer = append(p.syncBuffer, rb[:n]...) // merge with leftovers
parse:
	if len(p.syncBuffer) < 4 {
		return
	}
	for _, c := range p.syncBuffer {
		if 0xec != c {
			p.syncBuffer = p.syncBuffer[1:] // remove 1st char
			goto parse                      // no start byte
		}
		if 0xde == p.syncBuffer[1] {
			p.syncBuffer = p.syncBuffer[1:] // remove 1st char
			goto parse                      // no start byte: `0xec 0xde` is no valid esc packet start.
		}

		// p.syncBuffer[0] is esc, p.syncBuffer[1] is length code
		id := (int(p.syncBuffer[2]) << 8) | int(p.syncBuffer[3])
		index := p.list.Index(id)
		if index < 0 { // unknown id
			s = redBalk + fmt.Sprintln("error: unknown id", id, "syncBuffer = ", p.syncBuffer)
			p.syncBuffer = p.syncBuffer[1:] // remove 1st char
			goto parse
		}
		p.item = p.list.Item(index)

		switch p.syncBuffer[1] { // length code
		case 0xdf: // no params
			if "TRICE0" == p.item.FmtType {
				s = fmt.Sprintf(p.item.FmtStrg)
				p.syncBuffer = p.syncBuffer[4:]
				return
			}
			p.syncBuffer = p.syncBuffer[1:] // remove 1st char
			goto parse
		case 0xe0: // 1 byte param
			if len(p.syncBuffer) < 5 {
				return // wait
			}
			if "TRICE8_1" == p.item.FmtType {
				s = fmt.Sprintf(p.item.FmtStrg, int8(p.syncBuffer[4]))
				p.syncBuffer = p.syncBuffer[5:]
				return
			}
			s = redBalk + fmt.Sprintln("error: ", p.syncBuffer)
			p.syncBuffer = p.syncBuffer[1:] // remove 1st char
			goto parse
		case 0xe1: // 2 bytes param
			if len(p.syncBuffer) < 6 {
				return // wait
			}
			if "TRICE8_2" == p.item.FmtType {
				s = fmt.Sprintf(p.item.FmtStrg,
					int8(p.syncBuffer[4]),
					int8(p.syncBuffer[5]))
				p.syncBuffer = p.syncBuffer[6:]
				return
			}
			if "TRICE16_1" == p.item.FmtType {
				s = fmt.Sprintf(p.item.FmtStrg, int16(binary.BigEndian.Uint16(p.syncBuffer[4:6])))
				p.syncBuffer = p.syncBuffer[6:]
				return
			}
			s = redBalk + fmt.Sprintln("error: ", p.syncBuffer)
			p.syncBuffer = p.syncBuffer[1:] // remove 1st char
			goto parse
		case 0xe2: // 4 bytes param
			if len(p.syncBuffer) < 7 {
				return // wait
			}
			if "TRICE8_3" == p.item.FmtType {
				s = fmt.Sprintf(p.item.FmtStrg,
					int8(p.syncBuffer[4]),
					int8(p.syncBuffer[5]),
					int8(p.syncBuffer[6]))
				p.syncBuffer = p.syncBuffer[7:]
				return
			}
			if len(p.syncBuffer) < 8 {
				return // wait
			}
			if "TRICE8_4" == p.item.FmtType {
				s = fmt.Sprintf(p.item.FmtStrg,
					int8(p.syncBuffer[4]),
					int8(p.syncBuffer[5]),
					int8(p.syncBuffer[6]),
					int8(p.syncBuffer[7]))
				p.syncBuffer = p.syncBuffer[8:]
				return
			}
			if "TRICE16_2" == p.item.FmtType {
				s = fmt.Sprintf(p.item.FmtStrg,
					int16(binary.BigEndian.Uint16(p.syncBuffer[4:6])),
					int16(binary.BigEndian.Uint16(p.syncBuffer[6:8])))
				p.syncBuffer = p.syncBuffer[8:]
				return
			}
			if "TRICE32_1" == p.item.FmtType {
				s = fmt.Sprintf(p.item.FmtStrg, int32(binary.BigEndian.Uint32(p.syncBuffer[4:8])))
				p.syncBuffer = p.syncBuffer[8:]
				return
			}
			s = redBalk + fmt.Sprintln("error: ", p.syncBuffer)
			p.syncBuffer = p.syncBuffer[1:] // remove 1st char
			goto parse
		case 0xe3: // 8 bytes param
			if "TRICE8_5" == p.item.FmtType {
				if len(p.syncBuffer) < 9 {
					return // wait
				}
				s = fmt.Sprintf(p.item.FmtStrg,
					int8(p.syncBuffer[4]),
					int8(p.syncBuffer[5]),
					int8(p.syncBuffer[6]),
					int8(p.syncBuffer[7]),
					int8(p.syncBuffer[8]))
				p.syncBuffer = p.syncBuffer[9:]
				return
			}
			if len(p.syncBuffer) < 10 {
				return // wait
			}
			if "TRICE8_6" == p.item.FmtType {
				s = fmt.Sprintf(p.item.FmtStrg,
					int8(p.syncBuffer[4]),
					int8(p.syncBuffer[5]),
					int8(p.syncBuffer[6]),
					int8(p.syncBuffer[7]),
					int8(p.syncBuffer[8]),
					int8(p.syncBuffer[9]))
				p.syncBuffer = p.syncBuffer[10:]
				return
			}
			if "TRICE16_3" == p.item.FmtType {
				s = fmt.Sprintf(p.item.FmtStrg,
					int16(binary.BigEndian.Uint16(p.syncBuffer[4:6])),
					int16(binary.BigEndian.Uint16(p.syncBuffer[6:8])),
					int16(binary.BigEndian.Uint16(p.syncBuffer[8:10])))
				p.syncBuffer = p.syncBuffer[10:]
				return
			}
			if len(p.syncBuffer) < 11 {
				return // wait
			}
			if "TRICE8_7" == p.item.FmtType {
				s = fmt.Sprintf(p.item.FmtStrg,
					int8(p.syncBuffer[4]),
					int8(p.syncBuffer[5]),
					int8(p.syncBuffer[6]),
					int8(p.syncBuffer[7]),
					int8(p.syncBuffer[8]),
					int8(p.syncBuffer[9]),
					int8(p.syncBuffer[10]))
				p.syncBuffer = p.syncBuffer[11:]
				return
			}
			if len(p.syncBuffer) < 12 {
				return // wait
			}
			if "TRICE8_8" == p.item.FmtType {
				s = fmt.Sprintf(p.item.FmtStrg,
					int8(p.syncBuffer[4]),
					int8(p.syncBuffer[5]),
					int8(p.syncBuffer[6]),
					int8(p.syncBuffer[7]),
					int8(p.syncBuffer[8]),
					int8(p.syncBuffer[9]),
					int8(p.syncBuffer[10]),
					int8(p.syncBuffer[11]))
				p.syncBuffer = p.syncBuffer[12:]
				return
			}
			if "TRICE16_4" == p.item.FmtType {
				s = fmt.Sprintf(p.item.FmtStrg,
					int16(binary.BigEndian.Uint16(p.syncBuffer[4:6])),
					int16(binary.BigEndian.Uint16(p.syncBuffer[6:8])),
					int16(binary.BigEndian.Uint16(p.syncBuffer[8:10])),
					int16(binary.BigEndian.Uint16(p.syncBuffer[10:12])))
				p.syncBuffer = p.syncBuffer[12:]
				return
			}
			if "TRICE32_2" == p.item.FmtType {
				s = fmt.Sprintf(p.item.FmtStrg,
					int32(binary.BigEndian.Uint32(p.syncBuffer[4:8])),
					int32(binary.BigEndian.Uint32(p.syncBuffer[8:12])))
				p.syncBuffer = p.syncBuffer[12:]
				return
			}
			if "TRICE64_1" == p.item.FmtType {
				s = fmt.Sprintf(p.item.FmtStrg,
					int64(binary.BigEndian.Uint64(p.syncBuffer[4:12])))
				p.syncBuffer = p.syncBuffer[12:]
				return
			}
			s = redBalk + fmt.Sprintln("error: ", p.syncBuffer)
			p.syncBuffer = p.syncBuffer[1:] // remove 1st char
			goto parse
		case 0xe4: // 16 bytes param
			if len(p.syncBuffer) < 16 {
				return // wait
			}
			if "TRICE32_3" == p.item.FmtType {
				s = fmt.Sprintf(p.item.FmtStrg,
					int32(binary.BigEndian.Uint32(p.syncBuffer[4:8])),
					int32(binary.BigEndian.Uint32(p.syncBuffer[8:12])),
					int32(binary.BigEndian.Uint32(p.syncBuffer[12:16])))
				p.syncBuffer = p.syncBuffer[16:]
				return
			}
			if len(p.syncBuffer) < 20 {
				return // wait
			}
			if "TRICE32_4" == p.item.FmtType {
				s = fmt.Sprintf(p.item.FmtStrg,
					int32(binary.BigEndian.Uint32(p.syncBuffer[4:8])),
					int32(binary.BigEndian.Uint32(p.syncBuffer[8:12])),
					int32(binary.BigEndian.Uint32(p.syncBuffer[12:16])),
					int32(binary.BigEndian.Uint32(p.syncBuffer[16:20])))
				p.syncBuffer = p.syncBuffer[20:]
				return
			}
			if "TRICE64_2" == p.item.FmtType {
				if 0 < p.errCount {
					s = fmt.Sprintf("err:Failures:%d, ", p.errCount) // DEBUG
				} else {
					s = fmt.Sprintf("tst:Failures:%d, ", p.errCount) // DEBUG
				}
				s += fmt.Sprintf(p.item.FmtStrg,
					int64(binary.BigEndian.Uint64(p.syncBuffer[4:12])),
					int64(binary.BigEndian.Uint64(p.syncBuffer[12:20])))
				/*
					//////////////////////////////////////////////////////////////////////
					// DEBUG for MDK-ARM_LL_UART_ESC_NUCLEO-F070RB
					a := int64(binary.BigEndian.Uint64(p.syncBuffer[4:12]))
					b := int64(binary.BigEndian.Uint64(p.syncBuffer[12:20]))
					if (0x1020304050607080 != a || b != p.prev+1) && 0 != p.prev {
						s = redBalk + s + redBalk
						p.errCount++
					}

					p.prev = b
					//
					//////////////////////////////////////////////////////////////////////
				*/
				p.syncBuffer = p.syncBuffer[20:]
				return
			}
			s = redBalk + fmt.Sprintln("error: ", p.syncBuffer)
			p.syncBuffer = p.syncBuffer[1:] // remove 1st char
			goto parse
		default:
			s = redBalk + fmt.Sprintln("error: ", p.syncBuffer)
			p.syncBuffer = p.syncBuffer[1:]
			goto parse // invalid length code
		}
	}
	return
}

// // String is the method for displaying the current TriceTranslator instance.
// func (p *TriceTranslator) String() string {
// 	s := fmt.Sprintf("ID=%d=%x", p.item.ID, p.item.ID)
// 	s += fmt.Sprintf("values in hex: ")
// 	for _, n := range p.values {
// 		s += fmt.Sprintf("%x ", n)
// 	}
// 	return s
// }

// NewSimpleTrices gets its data from the TriceAtomsReceiver interface tr.
// It uses the io.StringWriter interface sw to write the trice strings looked up in id.List
// and enhanced with the printed values. Each single trice string is written separately with sw.
// It does not evaluate the TriceInterpreter ignored bytes ('Simple'Trices).
func NewSimpleTrices(sw io.StringWriter, list *id.List, tr TriceAtomsReceiver) *BareTranslator {
	p := &BareTranslator{}
	p.values = make([]uint16, 0, 100)
	p.done = make(chan int)
	p.list = list

	go func() {
		for {
			p.ErrorFatal()
			select {
			case atoms := <-tr.TriceAtomsChannel():
				for _, trice := range atoms {
					s := p.translate(trice)
					_, p.savedErr = sw.WriteString(s)
				}
			case ignored := <-tr.IgnoredBytesChannel():
				// TODO: evaluate other protocols here
				if Verbose {
					s := fmt.Sprintln("error:Found", len(ignored), "ignored byte(s):", ignored)
					_, p.savedErr = sw.WriteString(s)
				}

			case <-p.done:
				if Verbose {
					fmt.Println("end of bare translator life")
				}
				return
			}
		}
	}()
	return p
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
func (p *BareTranslator) translate(trice Trice) (s string) {
	if nil != p.savedErr {
		s = redBalk + fmt.Sprintln(p.savedErr, p.values)
		p.values = p.values[:0]
		p.savedErr = nil
		return
	}
	if 0x89ab == trice.ID { // discard sync trice
		if 0xcdef != trice.Value {
			if Verbose {
				s = fmt.Sprintln("error:trice", trice, "is invalid. Must have payload [205 239].")
			}
			p.values = p.values[:0]
		}
		return
	}
	p.values = append(p.values, trice.Value) // append the uint16 date of the current trice
	if 0 == trice.ID {                       // trice atom has only data
		return
	}
	index := p.list.Index(int(trice.ID))
	if index < 0 { // unknown trice.ID
		if Verbose {
			var vb []byte
			for _, v := range p.values { // convert values to bytes for displaying
				b := make([]byte, 2)
				binary.LittleEndian.PutUint16(b, v)
				vb = append(vb, b...)
			}
			s = redBalk + fmt.Sprintln("error: unknown trice.ID", trice.ID, "(", trice.ID>>8, 0xff&trice.ID, "), values = ", p.values, ", as bytes: ", vb)
			s += fmt.Sprintln(p)
		}
		p.values = p.values[:0] // clear values
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
func (p *BareTranslator) addValues() (s string) {
	f, _, _ := langCtoGoFmtStingConverter(p.item.FmtStrg) // todo
	var v0, v1, v2, v3 int16
	var w0, w1, w2, w3 int32
	var l0, l1 int64

	p.evalLen()
	if nil != p.savedErr {
		if Verbose {
			s = p.savedErr.Error() + fmt.Sprint("The accumulated data are not matching the trice.ID. Discarding: ")
			s += fmt.Sprintln(p)
		}
		p.values = p.values[:0]
		p.savedErr = nil
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
		p.savedErr = fmt.Errorf("Unknown FmtType %s", p.item.FmtType)
	}
	return
}

// evalLen checks if byte buffer t has appropriate length to id.item it
func (p *BareTranslator) evalLen() {
	t := p.values
	switch p.item.FmtType {
	case "TRICE0", "TRICE8_1", "TRICE8_2", "TRICE16_1":
		if 1 != len(t) {
			p.savedErr = fmt.Errorf("err:len %d != 1 ", len(t))
		}
	case "TRICE8_3", "TRICE8_4", "TRICE16_2", "TRICE32_1":
		if 2 != len(t) {
			p.savedErr = fmt.Errorf("err:len %d != 2 ", len(t))
		}
	case "TRICE8_5", "TRICE8_6", "TRICE16_3":
		if 3 != len(t) {
			p.savedErr = fmt.Errorf("err:len %d != 3 ", len(t))
		}
	case "TRICE8_7", "TRICE8_8", "TRICE16_4", "TRICE32_2", "TRICE64_1":
		if 4 != len(t) {
			p.savedErr = fmt.Errorf("err:len %d != 4 ", len(t))
		}
	case "TRICE32_3":
		if 6 != len(t) {
			p.savedErr = fmt.Errorf("err:len %d != 6 ", len(t))
		}
	case "TRICE32_4", "TRICE64_2":
		if 8 != len(t) {
			p.savedErr = fmt.Errorf("err:len %d != 8 ", len(t))
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

// Stop ends life of TriceInterpreter.
func (p *BareTranslator) Stop() {
	p.done <- 0
}
