// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package interpreter listens on several channels and converts the data to Lines.
// It expects a Line writer
package interpreter

/*
import (
	"encoding/binary"
	"errors"
	"fmt"
	"strings"

	"github.com/rokath/trice/internal/bare"
	"github.com/rokath/trice/internal/disp"
	"github.com/rokath/trice/internal/emit"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/pkg/display"
	"github.com/rokath/trice/pkg/lib"
)


var (
	// TimeStampFormat is the PC timestamp format.
	TimeStampFormat = "off"
)

// Timestamp returns local time as string according var TimeStampFormat
// https://www.alexedwards.net/blog/an-overview-of-go-tooling#managing-dependencies
func timestamp() string {
	var s string
	switch TimeStampFormat {
	case "LOCmicro":
		s = time.Now().Format(time.StampMicro) + "  "
	case "UTCmicro":
		s = "UTC " + time.Now().UTC().Format(time.StampMicro) + "  "
	case "off", "none":
		s = ""
	}
	return s
}


type Interpreter struct{}

// T is the interpreter type.
type T struct {
	trice <-chan []bare.Item
	w     display.Writer
}

// New creates an interpreter instance
func New(w display.Writer, trice <-chan []bare.Item) (*T, error) {
	p := &T{
		w:     w,
		trice: trice,
	}
	return p, nil
}

func (p *T) Interpret() {
	select {
	case t := <-p.trice:
		//fmt.Print(t)
		for _, x := range t {
			emitTrice(x)
		}
	}
}

func (p *T) Start() {
	go p.Interpret()
}

var (
	// Prefix is a (configurable) string added to each line end
	Prefix = ""

	// Postfix is a (configurable) string added to each line end
	Postfix = "\n"

	// param collector, usually not more than 16 bytes
	param = make([]byte, 0, 32)

	// css is a collector for line substrings
	// it is used only inside LineCollect() but needs to survive from call to call
	css []string
)

// Trice emits one trice. The byte slice 't' is a bare trice package.
// The ID can be 0, in that case only the data payload is saved
//
func emitTrice(t bare.Item) error {

	var b = []byte{
		t.Value[0],
		t.Value[1],
	}
	param = append(param, b...)

	// handle ID=0 case (only parame)
	if 0 == t.ID {
		if 0 == len(id.List) { // "none" as ID list or list empty
			fmt.Printf("% 20x\n", t) //  show raw data
			//mt.Fprintf(Tee, "% 20x\n", t) //  show raw data
		}
		if len(param) < 32 {
			return nil // only params
		} else {
			fmt.Println(param) //  show acc data
			param = param[:0]  // discard accumulated values
			return errors.New("data not consistent - too much ID=0 trices")
		}
	}

	// handle no id list case
	if 0 == len(id.List) { // "none" as ID list or list empty
		fmt.Printf("% 20x acc:", t) //  show raw data
		fmt.Println(param)          //  show acc data
		param = param[:0]           // empty for next trice
		return nil
	}

	// get index in id list
	index, err := id.Index(int(t.ID), id.List)
	if nil != err {
		// not found
		param = param[:0]  // empty d for next trice
		fmt.Println(param) //  show acc data
		return err         // i is unknown ID
	}

	it := id.List[index]
	s, _ := emitter(it, param)

	lineCollect(s)

	param = param[:0] // empty d for next trice
	return nil
}

// LineCollect collects s into an internal line substring slice
// When s ends with a newline it is trimmed and the slice goes to Out and is discarded afterwards
func lineCollect(s string) {
	s = emit.TrimBackslashes(s)

	a := func(su string) {
		css = append(css, su)
	}
	if 0 == len(css) {
		a(Prefix)
		a(timestamp())
	}
	if !strings.HasSuffix(s, "\n") {
		a(s)
		return
	}
	s = strings.TrimSuffix(s, "\n")
	a(s)
	a(Postfix)
	disp.WriteLine(css)
	css = css[:0] // discard slice data
}

// errFalseLen reports an error in case of false len, what could be caused by data garbage
func errFalseLen(it id.Item, t []byte) (string, error) {
	s := fmt.Sprintln("ERR:", it, "false len", len(t), "in data stream", t)
	return s, errors.New("data stream error")
}

// emmiter wworks fine with %x, %d and %o but NOT with %u for now
// %x is emitted in Go signed!
// For %u a format string parsing is needed to perform the correct casts.
func emitter(it id.Item, prm []byte) (string, error) {
	f, _, err := emit.LangCtoGoFmtStingConverter(it.FmtStrg)
	var s string
	var v0, v1, v2, v3 int16
	var w0, w1, w2, w3 int32
	var l0, l1 int64
	err = emit.EvalLen(it, prm)
	if nil != err {
		return errFalseLen(it, prm)
	}
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
		return "ERR: INTERNAL ERROR!!!", errors.New("ERR: INTERNAL ERROR")
	}
	return s, err
}
*/":
		l0 = int64(binary.LittleEndian.Uint64(prm[0:8]))
		l1 = int64(binary.LittleEndian.Uint64(prm[8:16]))
		s = fmt.Sprintf(f, l0, l1)
	default:
		return "ERR: INTERNAL ERROR!!!", errors.New("ERR: INTERNAL ERROR")
	}
	return s, err
}
*/
