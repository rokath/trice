// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package decoder

import (
	"fmt"
	"io"

	"github.com/rokath/trice/internal/id"
)

// Bare is the Decoding instance for bare encoded trice's.
type Bare struct {
	decoderData
	payload []int // value payload
}

// NewBareDecoder provides an BareDecoder instance.
// l is the trice id list in slice of struct format.
// in is the usable reader for the input bytes.
func NewBareDecoder(lut id.TriceIDLookUp, in io.Reader, endian bool) Decoder {
	p := &Bare{}
	p.in = in
	p.syncBuffer = make([]byte, 0, defaultSize)
	p.lut = lut
	p.endian = endian
	return Decoder(p)
}

// Read is the provided read method for bare decoding of next string as byte slice.
// It uses inner reader p.in and internal id look-up table to fill b with a string.
// b is a slice of bytes with a len for the max expected string size.
// n is the count of read bytes inside b.
// Read returns one trice string (optionally starting wth a channel specifier).
// A line can contain several trice strings.
func (p *Bare) Read(b []byte) (n int, err error) {
	sizeMsg := fmt.Sprintln("e:buf too small, expecting", defaultSize, "bytes.")
	if len(b) < len(sizeMsg) {
		return
	}
	if len(b) < defaultSize {
		n = copy(b, sizeMsg)
		return
	}
	p.b = b
	// fill intermediate read buffer for pack encoding
	n, err = p.in.Read(b) // use b as intermediate buffer to avoid allocation
	// p.syncBuffer can contain unprocessed bytes from last call.
	p.syncBuffer = append(p.syncBuffer, b[:n]...) // merge with leftovers
	n = 0
	if nil != err && io.EOF != err {
		return
	}
	// Even err could be io.EOF some valid data possibly in p.syncBuffer.
	// In case of file input (J-LINK usage) a plug off is not detectable here.
	for {
		if len(p.syncBuffer) < 4 {
			return // wait
		}
		p.bc = 4
		head := int(p.readU32(p.syncBuffer[0:4]))
		if 0x89abcdef == uint(head) { // sync trice
			p.rub(4)
			continue
		}
		triceID := id.TriceID(head >> 16)          // 2 msb bytes are the ID
		p.payload = append(p.payload, 0xffff&head) // next 2 bytes are payload
		if 8 < len(p.payload) {
			p.payload = p.payload[:0]
			return p.outOfSync(fmt.Sprintf("too much payload data %d", len(p.payload)))
		}
		if 0 == triceID {
			p.rub(4)
			continue
		}
		var ok bool
		p.trice, ok = p.lut[triceID] // check lookup table
		if !ok {
			p.payload = p.payload[:0]
			return p.outOfSync(fmt.Sprintf("unknown triceID %5d", triceID))
		}
		if !p.payloadLenOk() {
			p.payload = p.payload[:0]
			return p.outOfSync(fmt.Sprintf("unecpected payload len %d", p.expectedPayloadLen()))
		}
		p.rub(4)
		// ID and count are ok
		return p.sprintTrice()
	}
}

// payloadLenOk returns true if the transmitted count information matches the expected count
func (p *Bare) payloadLenOk() bool {
	x := p.expectedPayloadLen()
	return len(p.payload) == x || -1 == x
}

// expectedPayloadLen returns expected len for triceType.
// It returns -1 for an unknown value an -2 for unknown triceType.
func (p *Bare) expectedPayloadLen() int {
	switch p.trice.Type {
	case "TRICE0", "TRICE8_1", "TRICE8_2", "TRICE16_1", "trice0", "trice8_1", "trice8_2", "trice16_1":
		return 1
	case "TRICE8_3", "TRICE8_4", "TRICE16_2", "TRICE32_1", "trice8_3", "trice8_4", "trice16_2", "trice32_1":
		return 2
	case "TRICE8_5", "TRICE8_6", "TRICE16_3", "trice8_5", "trice8_6", "trice16_3":
		return 3
	case "TRICE8_7", "TRICE8_8", "TRICE16_4", "TRICE32_2", "TRICE64_1", "trice8_7", "trice8_8", "trice16_4", "trice32_2", "trice64_1":
		return 4
	case "TRICE32_3", "trice32_3":
		return 6
	case "TRICE32_4", "TRICE64_2", "trice32_4", "trice64_2":
		return 8
	case "TRICE_S", "trice_S":
		return -1 // unknown count
	default:
		return -2 // unknown trice type
	}
}

/*
// sprintTrice generates the trice string.
func (p *Bare) sprintTrice() (n int, err error) {
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
	}
	return p.outOfSync(fmt.Sprintf("Unexpected trice.Type %s", p.trice.Type))
}
*/

// sprintTrice generates the trice string.
func (p *Bare) sprintTrice( /*cnt int*/ ) (n int, e error) {
	// ID and count are ok
	switch p.trice.Type {
	case "TRICE0", "trice0":
		return p.trice0()
	case "TRICE8_1", "trice8_1":
		return p.trice81()
	case "TRICE8_2", "trice8_2":
		return p.trice82()
	case "TRICE8_3", "trice8_3":
		return p.trice83()
	case "TRICE8_4", "trice8_4":
		return p.trice84()
	case "TRICE8_5", "trice8_5":
		return p.trice85()
	case "TRICE8_6", "trice8_6":
		return p.trice86()
	case "TRICE8_7", "trice8_7":
		return p.trice87()
	case "TRICE8_8", "trice8_8":
		return p.trice88()
	case "TRICE16_1", "trice16_1":
		return p.trice161()
	case "TRICE16_2", "trice16_2":
		return p.trice162()
	case "TRICE16_3", "trice16_3":
		return p.trice163()
	case "TRICE16_4", "trice16_4":
		return p.trice164()
	case "TRICE32_1", "trice32_1":
		return p.trice321()
	case "TRICE32_2", "trice32_2":
		return p.trice322()
	case "TRICE32_3", "trice32_3":
		return p.trice323()
	case "TRICE32_4", "trice32_4":
		return p.trice324()
	case "TRICE64_1", "trice64_1":
		return p.trice641()
	case "TRICE64_2", "trice64_2":
		return p.trice642()
		//	case "TRICE_S", "trice_s":
		//		return p.triceS(cnt)
	default:
		return p.outOfSync(fmt.Sprintf("Unexpected trice.Type %s", p.trice.Type))
	}
}

func (p *Bare) trice0() (n int, e error) {
	n = copy(p.b, fmt.Sprintf(p.trice.Strg))
	p.payload = p.payload[:0]
	return
}

func (p *Bare) trice81() (n int, e error) {
	// to do: Evaluate p.trice.Strng for %u, change to %d and use uint8 than.
	b0 := int8(p.payload[0])
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, b0))
	p.payload = p.payload[:0]
	return
}

func (p *Bare) trice82() (n int, e error) {
	b0 := int8(p.payload[0] >> 8)
	b1 := int8(p.payload[0])
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, b0, b1))
	p.payload = p.payload[:0]
	return
}

func (p *Bare) trice83() (n int, e error) {
	b0 := int8(p.payload[0] >> 8)
	b1 := int8(p.payload[0])
	b2 := int8(p.payload[1])
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, b0, b1, b2))
	p.payload = p.payload[:0]
	return
}

func (p *Bare) trice84() (n int, e error) {
	b0 := int8(p.payload[0] >> 8)
	b1 := int8(p.payload[0])
	b2 := int8(p.payload[1] >> 8)
	b3 := int8(p.payload[1])
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, b0, b1, b2, b3))
	p.payload = p.payload[:0]
	return
}

func (p *Bare) trice85() (n int, e error) {
	b0 := int8(p.payload[0] >> 8)
	b1 := int8(p.payload[0])
	b2 := int8(p.payload[1] >> 8)
	b3 := int8(p.payload[1])
	b4 := int8(p.payload[2])
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, b0, b1, b2, b3, b4))
	p.payload = p.payload[:0]
	return
}

func (p *Bare) trice86() (n int, e error) {
	b0 := int8(p.payload[0] >> 8)
	b1 := int8(p.payload[0])
	b2 := int8(p.payload[1] >> 8)
	b3 := int8(p.payload[1])
	b4 := int8(p.payload[2] >> 8)
	b5 := int8(p.payload[2])
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, b0, b1, b2, b3, b4, b5))
	p.payload = p.payload[:0]
	return
}

func (p *Bare) trice87() (n int, e error) {
	b0 := int8(p.payload[0] >> 8)
	b1 := int8(p.payload[0])
	b2 := int8(p.payload[1] >> 8)
	b3 := int8(p.payload[1])
	b4 := int8(p.payload[2] >> 8)
	b5 := int8(p.payload[2])
	b6 := int8(p.payload[3])
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, b0, b1, b2, b3, b4, b5, b6))
	p.payload = p.payload[:0]
	return
}

func (p *Bare) trice88() (n int, e error) {
	b0 := int8(p.payload[0] >> 8)
	b1 := int8(p.payload[0])
	b2 := int8(p.payload[1] >> 8)
	b3 := int8(p.payload[1])
	b4 := int8(p.payload[2] >> 8)
	b5 := int8(p.payload[2])
	b6 := int8(p.payload[3] >> 8)
	b7 := int8(p.payload[3])
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, b0, b1, b2, b3, b4, b5, b6, b7))
	p.payload = p.payload[:0]
	return
}

func (p *Bare) trice161() (n int, e error) {
	d0 := int16(p.payload[0])
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0))
	p.payload = p.payload[:0]
	return
}

func (p *Bare) trice162() (n int, e error) {
	d0 := int16(p.payload[0])
	d1 := int16(p.payload[1])
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0, d1))
	p.payload = p.payload[:0]
	return
}

func (p *Bare) trice163() (n int, e error) {
	d0 := int16(p.payload[0])
	d1 := int16(p.payload[1])
	d2 := int16(p.payload[2])
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0, d1, d2))
	p.payload = p.payload[:0]
	return
}

func (p *Bare) trice164() (n int, e error) {
	d0 := int16(p.payload[0])
	d1 := int16(p.payload[1])
	d2 := int16(p.payload[2])
	d3 := int16(p.payload[3])
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0, d1, d2, d3))
	p.payload = p.payload[:0]
	return
}

func (p *Bare) trice321() (n int, e error) {
	d0 := int32(p.payload[0])<<16 | int32(p.payload[1])
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0))
	p.payload = p.payload[:0]
	return
}

func (p *Bare) trice322() (n int, e error) {
	d0 := int32(p.payload[0]<<16 | p.payload[1])
	d1 := int32(p.payload[2]<<16 | p.payload[3])
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0, d1))
	p.payload = p.payload[:0]
	return
}

func (p *Bare) trice323() (n int, e error) {
	d0 := int32(p.payload[0]<<16 | p.payload[1])
	d1 := int32(p.payload[2]<<16 | p.payload[3])
	d2 := int32(p.payload[4]<<16 | p.payload[5])
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0, d1, d2))
	p.payload = p.payload[:0]
	return
}

func (p *Bare) trice324() (n int, e error) {
	d0 := int32(p.payload[0]<<16 | p.payload[1])
	d1 := int32(p.payload[2]<<16 | p.payload[3])
	d2 := int32(p.payload[4]<<16 | p.payload[5])
	d3 := int32(p.payload[6]<<16 | p.payload[7])
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0, d1, d2, d3))
	p.payload = p.payload[:0]
	return
}

func (p *Bare) trice641() (n int, e error) {
	d0 := int64(p.payload[0]<<48 | p.payload[1]<<32 | p.payload[2]<<16 | p.payload[3])
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0))
	p.payload = p.payload[:0]
	return
}

func (p *Bare) trice642() (n int, e error) {
	d0 := int64(p.payload[0]<<48 | p.payload[1]<<32 | p.payload[2]<<16 | p.payload[3])
	d1 := int64(p.payload[4]<<48 | p.payload[5]<<32 | p.payload[6]<<16 | p.payload[7])
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0, d1))
	p.payload = p.payload[:0]
	return
}
