// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package inputdummy simulates artificial trice data for testing.
//
// On creation a byte slice with mockup data is expected.
// When end of slice is reached reading starts from the beginning.
// When max byte is reached an io.EOF error occures.
// The time.Duration value can be used to delay the Read function.
package inputdummy

import (
	"io"
	"time"

	"github.com/rokath/trice/internal/global"
)

const (
	// NoDelay avoids any internal waiting.
	NoDelay = time.Duration(0)

	// NoLimit is used for limit and means endless (circular) data.
	NoLimit = 0
)

// Source delivers an endless circle of bytes.
type Source struct {
	b      []byte        // internal byte buffer to read from
	t      time.Duration // tinternal delay
	offset int           // next read position
	limit  int           // max bytes count readble, 0 means endless
}

// New creates an instance of inputDummy.
// b contains bytes it reads cyclically
func New(b []byte, t time.Duration, limit int) *Source {
	s := &Source{}
	s.b = b
	s.t = t
	s.limit = limit
	return s
}

// Read is an exported method.
func (p *Source) Read(b []byte) (int, error) {
	var i int
	time.Sleep(p.t)
	if nil == p.b || 0 == len(p.b) {
		return 0, io.EOF
	}
	if 0 == len(b) {
		return 0, nil
	}
	for i = range b {
		if 0 == p.limit || p.offset+i < p.limit {
			b[i] = p.b[(p.offset+i)%len(p.b)]
		} else {
			global.OsExit(0)
			//return i, io.EOF
		}
	}
	i++ // adjust to byte count
	p.offset += i
	return i, nil
}

/* works, but not needed now
// ReadString is an exported method.
func (p *Source) ReadString(delim byte) (string, error) {
	var err error
	var b []byte
	c := make([]byte, 1)
	for {
		_, err = io.ReadFull(p, c)
		if nil != err {
			break
		}
		b = append(b, c...)
		if delim == c[0] {
			break
		}
	}
	s := string(b)
	return s, err
}
*/
