package tcobs

import (
	"errors"
	"fmt"
	"io"
)

type encoder struct {
	w    io.Writer // inner writer
	iBuf []byte    // input buffer
	iCnt int       // iCnt is byte count on hold in iBuf
}

// NewEncoder creates an encoder instance and returns its address.
// w will be used as inner writer and size is used as initial size for the inner buffer.
func NewEncoder(w io.Writer, size int) (p *encoder) {
	p = new(encoder)
	p.w = w
	p.iBuf = make([]byte, size)
	return
}

// Write encodes buffer and writes the encoded content.
func (p *encoder) Write(buffer []byte) (n int, e error) {
	if p.iCnt > 0 {
		e = errors.New("inner buffer not empty (needs Flush)")
		return
	}
	r := CEncode(p.iBuf, buffer)
	if r < 0 {
		e = fmt.Errorf("CEncode returned an internal error. Check tcobsEncode.c line %d", -r)
		return
	}
	n = len(buffer)
	enc := append(p.iBuf[:r], 0) // 0-delimiter
	m, e := p.w.Write(enc)
	if m == len(enc) { // all written
		return
	}
	// keep the leftovers for Flush
	p.iCnt = copy(p.iBuf, enc[m:])
	e = errors.New("inner buffer not empty (needs Flush)")
	return
}

// Flush tries to write inner buffer with the inner writer and returns nil when all data could be written.
func (p *encoder) Flush() error {
	n, e := p.w.Write(p.iBuf[:p.iCnt])
	p.iCnt -= n
	if p.iCnt == 0 {
		return e
	}
	return errors.New("inner buffer not empty yet")
}
