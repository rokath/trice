// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package translator converts...
package translator

import (
	"encoding/binary"
	"errors"
	"fmt"
	"io"
	"time"

	"github.com/rokath/trice/internal/id"
)

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
func NewEscTrices(sw io.StringWriter, list *id.List, in io.ReadCloser, hardReadError chan<- bool) *EscTranslator {
	p := new(EscTranslator)
	p.sw = sw
	p.list = list
	p.in = in
	p.syncBuffer = make([]byte, 0, 100) // todo: size
	p.done = make(chan int)
	const maxS = 100 // max count of trice strings in one round
	ss := make([]string, maxS)
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
				ss = ss[:maxS]
				n := p.readEsc(ss)
				if nil != p.savedErr && io.EOF != p.savedErr {
					fmt.Println("Read error", p.savedErr)
					in.Close()
					hardReadError <- true
					return
				}
				ss = ss[:n] // s contains the amount of trice strings successfully read and translated
				for _, s := range ss {
					_, p.savedErr = sw.WriteString(s)
				}
			}
		}
	}()
	return p
}

// readEsc2 uses inner reader
func (p *EscTranslator) readEsc(ss []string) (m int) {
	p.ErrorFatal()
	maxBytes := 4*len(ss) - len(p.syncBuffer) // shortest esc trice is 4 bytes: 0xec 0xdf idHi idLo, so surely read not more than space in ss
	rb := make([]byte, maxBytes)
	var n int
	n, p.savedErr = p.in.Read(rb)
	p.syncBuffer = append(p.syncBuffer, rb[:n]...) // merge with leftovers
parse:
	if len(p.syncBuffer) < 4 {
		return
	}
	if len(ss) == m {
		p.savedErr = errors.New("no space in string slice")
		return
	}
	for _, c := range p.syncBuffer {
		if 0xec != c {
			p.syncBuffer = p.syncBuffer[1:] // remove 1st char
			goto parse                      // no start byte
		}
		if 0xde == p.syncBuffer[1] {
			p.syncBuffer = p.syncBuffer[1:] // remove 1st char
			goto parse                      // no start byte: `0xec 0xde` is no valid esc packet start. Is is an escaped 0xec.
		}

		// p.syncBuffer[0] is esc, p.syncBuffer[1] is length code, now following the 2 id bytes in bigendian format (network byte order)
		id := (int(p.syncBuffer[2]) << 8) | int(p.syncBuffer[3])
		index := p.list.Index(id)
		if index < 0 { // unknown id
			ss[m] = redBalk + fmt.Sprintln("error: unknown id", id, "syncBuffer = ", p.syncBuffer)
			m++
			p.syncBuffer = p.syncBuffer[1:] // remove 1st char
			goto parse
		}
		p.item = p.list.Item(index)

		switch p.syncBuffer[1] { // length code evaluation
		case 0xdf: // no params
			if "TRICE0" == p.item.FmtType {
				ss[m] = fmt.Sprintf(p.item.FmtStrg)
				m++
				p.syncBuffer = p.syncBuffer[4:]
				goto parse
			}
			ss[m] = redBalk + fmt.Sprintln("error: ", p.syncBuffer)
			m++
			p.syncBuffer = p.syncBuffer[1:] // remove 1st char
			goto parse
		case 0xe0: // 1 byte param
			if len(p.syncBuffer) < 5 {
				return // wait
			}
			if "TRICE8_1" == p.item.FmtType {
				ss[m] = fmt.Sprintf(p.item.FmtStrg, int8(p.syncBuffer[4]))
				m++
				p.syncBuffer = p.syncBuffer[5:]
				goto parse
			}
			ss[m] = redBalk + fmt.Sprintln("error: ", p.syncBuffer)
			m++
			p.syncBuffer = p.syncBuffer[1:] // remove 1st char
			goto parse
		case 0xe1: // 2 bytes param
			if len(p.syncBuffer) < 6 {
				return // wait
			}
			if "TRICE8_2" == p.item.FmtType {
				ss[m] = fmt.Sprintf(p.item.FmtStrg,
					int8(p.syncBuffer[4]),
					int8(p.syncBuffer[5]))
				m++
				p.syncBuffer = p.syncBuffer[6:]
				goto parse
			}
			if "TRICE16_1" == p.item.FmtType {
				ss[m] = fmt.Sprintf(p.item.FmtStrg, int16(binary.BigEndian.Uint16(p.syncBuffer[4:6])))
				m++
				p.syncBuffer = p.syncBuffer[6:]
				goto parse
			}
			ss[m] = redBalk + fmt.Sprintln("error: ", p.syncBuffer)
			m++
			p.syncBuffer = p.syncBuffer[1:] // remove 1st char
			goto parse
		case 0xe2: // 4 bytes param or 3 bytes param and 1 byte padding
			if len(p.syncBuffer) < 7 {
				return // wait
			}
			if "TRICE8_3" == p.item.FmtType {
				ss[m] = fmt.Sprintf(p.item.FmtStrg,
					int8(p.syncBuffer[4]),
					int8(p.syncBuffer[5]),
					int8(p.syncBuffer[6]))
				m++
				p.syncBuffer = p.syncBuffer[7:]
				goto parse
			}
			if len(p.syncBuffer) < 8 {
				return // wait
			}
			if "TRICE8_4" == p.item.FmtType {
				ss[m] = fmt.Sprintf(p.item.FmtStrg,
					int8(p.syncBuffer[4]),
					int8(p.syncBuffer[5]),
					int8(p.syncBuffer[6]),
					int8(p.syncBuffer[7]))
				m++
				p.syncBuffer = p.syncBuffer[8:]
				goto parse
			}
			if "TRICE16_2" == p.item.FmtType {
				ss[m] = fmt.Sprintf(p.item.FmtStrg,
					int16(binary.BigEndian.Uint16(p.syncBuffer[4:6])),
					int16(binary.BigEndian.Uint16(p.syncBuffer[6:8])))
				m++
				p.syncBuffer = p.syncBuffer[8:]
				goto parse
			}
			if "TRICE32_1" == p.item.FmtType {
				ss[m] = fmt.Sprintf(p.item.FmtStrg, int32(binary.BigEndian.Uint32(p.syncBuffer[4:8])))
				m++
				p.syncBuffer = p.syncBuffer[8:]
				goto parse
			}
			ss[m] = redBalk + fmt.Sprintln("error: ", p.syncBuffer)
			m++
			p.syncBuffer = p.syncBuffer[1:] // remove 1st char
			goto parse
		case 0xe3: // 8 bytes param including 0-3 padding
			if "TRICE8_5" == p.item.FmtType {
				if len(p.syncBuffer) < 9 {
					return // wait
				}
				ss[m] = fmt.Sprintf(p.item.FmtStrg,
					int8(p.syncBuffer[4]),
					int8(p.syncBuffer[5]),
					int8(p.syncBuffer[6]),
					int8(p.syncBuffer[7]),
					int8(p.syncBuffer[8]))
				m++
				p.syncBuffer = p.syncBuffer[9:]
				goto parse
			}
			if len(p.syncBuffer) < 10 {
				return // wait
			}
			if "TRICE8_6" == p.item.FmtType {
				ss[m] = fmt.Sprintf(p.item.FmtStrg,
					int8(p.syncBuffer[4]),
					int8(p.syncBuffer[5]),
					int8(p.syncBuffer[6]),
					int8(p.syncBuffer[7]),
					int8(p.syncBuffer[8]),
					int8(p.syncBuffer[9]))
				m++
				p.syncBuffer = p.syncBuffer[10:]
				goto parse
			}
			if "TRICE16_3" == p.item.FmtType {
				ss[m] = fmt.Sprintf(p.item.FmtStrg,
					int16(binary.BigEndian.Uint16(p.syncBuffer[4:6])),
					int16(binary.BigEndian.Uint16(p.syncBuffer[6:8])),
					int16(binary.BigEndian.Uint16(p.syncBuffer[8:10])))
				m++
				p.syncBuffer = p.syncBuffer[10:]
				goto parse
			}
			if len(p.syncBuffer) < 11 {
				return // wait
			}
			if "TRICE8_7" == p.item.FmtType {
				ss[m] = fmt.Sprintf(p.item.FmtStrg,
					int8(p.syncBuffer[4]),
					int8(p.syncBuffer[5]),
					int8(p.syncBuffer[6]),
					int8(p.syncBuffer[7]),
					int8(p.syncBuffer[8]),
					int8(p.syncBuffer[9]),
					int8(p.syncBuffer[10]))
				m++
				p.syncBuffer = p.syncBuffer[11:]
				goto parse
			}
			if len(p.syncBuffer) < 12 {
				return // wait
			}
			if "TRICE8_8" == p.item.FmtType {
				ss[m] = fmt.Sprintf(p.item.FmtStrg,
					int8(p.syncBuffer[4]),
					int8(p.syncBuffer[5]),
					int8(p.syncBuffer[6]),
					int8(p.syncBuffer[7]),
					int8(p.syncBuffer[8]),
					int8(p.syncBuffer[9]),
					int8(p.syncBuffer[10]),
					int8(p.syncBuffer[11]))
				m++
				p.syncBuffer = p.syncBuffer[12:]
				return
			}
			if "TRICE16_4" == p.item.FmtType {
				ss[m] = fmt.Sprintf(p.item.FmtStrg,
					int16(binary.BigEndian.Uint16(p.syncBuffer[4:6])),
					int16(binary.BigEndian.Uint16(p.syncBuffer[6:8])),
					int16(binary.BigEndian.Uint16(p.syncBuffer[8:10])),
					int16(binary.BigEndian.Uint16(p.syncBuffer[10:12])))
				m++
				p.syncBuffer = p.syncBuffer[12:]
				goto parse
			}
			if "TRICE32_2" == p.item.FmtType {
				ss[m] = fmt.Sprintf(p.item.FmtStrg,
					int32(binary.BigEndian.Uint32(p.syncBuffer[4:8])),
					int32(binary.BigEndian.Uint32(p.syncBuffer[8:12])))
				m++
				p.syncBuffer = p.syncBuffer[12:]
				goto parse
			}
			if "TRICE64_1" == p.item.FmtType {
				ss[m] = fmt.Sprintf(p.item.FmtStrg,
					int64(binary.BigEndian.Uint64(p.syncBuffer[4:12])))
				m++
				p.syncBuffer = p.syncBuffer[12:]
				goto parse
			}
			ss[m] = redBalk + fmt.Sprintln("error: ", p.syncBuffer)
			m++
			p.syncBuffer = p.syncBuffer[1:] // remove 1st char
			goto parse
		case 0xe4: // 16 bytes param
			if len(p.syncBuffer) < 16 {
				return // wait
			}
			if "TRICE32_3" == p.item.FmtType {
				ss[m] = fmt.Sprintf(p.item.FmtStrg,
					int32(binary.BigEndian.Uint32(p.syncBuffer[4:8])),
					int32(binary.BigEndian.Uint32(p.syncBuffer[8:12])),
					int32(binary.BigEndian.Uint32(p.syncBuffer[12:16])))
				m++
				p.syncBuffer = p.syncBuffer[16:]
				goto parse
			}
			if len(p.syncBuffer) < 20 {
				return // wait
			}
			if "TRICE32_4" == p.item.FmtType {
				ss[m] = fmt.Sprintf(p.item.FmtStrg,
					int32(binary.BigEndian.Uint32(p.syncBuffer[4:8])),
					int32(binary.BigEndian.Uint32(p.syncBuffer[8:12])),
					int32(binary.BigEndian.Uint32(p.syncBuffer[12:16])),
					int32(binary.BigEndian.Uint32(p.syncBuffer[16:20])))
				m++
				p.syncBuffer = p.syncBuffer[20:]
				goto parse
			}
			if "TRICE64_2" == p.item.FmtType {
				ss[m] = fmt.Sprintf(p.item.FmtStrg,
					int64(binary.BigEndian.Uint64(p.syncBuffer[4:12])),
					int64(binary.BigEndian.Uint64(p.syncBuffer[12:20])))
				m++
				p.syncBuffer = p.syncBuffer[20:]
				goto parse
			}
			ss[m] = redBalk + fmt.Sprintln("error: ", p.syncBuffer)
			m++
			p.syncBuffer = p.syncBuffer[1:] // remove 1st char
			goto parse
		default:
			ss[m] = redBalk + fmt.Sprintln("error: ", p.syncBuffer)
			m++
			p.syncBuffer = p.syncBuffer[1:]
			goto parse // invalid length code
		}
	}
	return
}
