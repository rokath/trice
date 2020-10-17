// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package translator

import (
	"encoding/binary"
	"fmt"
	"io"

	"github.com/rokath/trice/internal/id"
)

// BareTranslator ...
type BareTranslator struct {
	translator

	// values holds so far unused values for the next trice item.
	values []uint16
}

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
		var vb []byte
		for _, v := range p.values { // convert values to bytes for displaying
			b := make([]byte, 2)
			binary.LittleEndian.PutUint16(b, v)
			vb = append(vb, b...)
		}
		s = fmt.Sprintln("error: unknown trice.ID", trice.ID, "(", trice.ID>>8, 0xff&trice.ID, "), values = ", p.values, ", as bytes: ", vb)
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
		p.savedErr = fmt.Errorf("unknown FmtType %s", p.item.FmtType)
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

// Stop ends life of TriceInterpreter.
func (p *BareTranslator) Stop() {
	p.done <- 0
}
