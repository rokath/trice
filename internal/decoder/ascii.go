// -build x
// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package decoder

import (
	"fmt"
	"io"
	"time"
)

// // IF is the provided display interface.
// type AsciiIF interface {
// 	Reader([]byte) (int, error)
// 	Writer([]display.Line) (int, error)
// }

type ascii struct {
	bytes    io.Reader // reader for input data stream
	newlines []string
	delim    byte
	strings  StringWriter // the trice string writer to use

}

// newStringsFromASCIIDecoder creates an instance to de-serialize strings fom ascii byte stream
func newStringsFromASCIIDecoder(r io.Reader, s StringWriter) *ascii {
	p := &ascii{
		bytes:    r,
		newlines: []string{"\n", "\r\n", "\n\r"},
		delim:    0,
		strings:  s,
	}
	return p
}

// Unmarshal parses the JSON-encoded data and stores the result
// in the value pointed to by v. If v is nil or not a pointer,
// Unmarshal returns an InvalidUnmarshalError.
func Unmarshal(data []byte) ([]string, error) {
	var ss []string
	if 0 == len(data) {
		return ss, nil
	}

	ss = append(ss, "PREFIX", "TS") //, s)
	return ss, nil
}

func (p *ascii) unmarshal() {
	b := make([]byte, 4096)
	n, err := p.bytes.Read(b)
	if nil != err && io.EOF != err {
		fmt.Println(err)
		time.Sleep(time.Second)
	}
	b = b[:n]

	ss, _ := Unmarshal(b)

	p.strings.Write(ss)
}

// stringsFromASCIIDecode assumes the bytes coming from io.Reader as ASCII data stream.
// It writes strings using the StringWriter following these rules:
// As newline are considered '\n' or '\r\n'.
//
// Start with prefix if enabled.s
// Followed by timestamp if enabled.
// Followed by any number of 0 terminated strings not containing newline.
// If a string contains newline it is split at the first newline and the newline is removed.
// The first part of the string goes to StringWriter. The 2nd part is kept internally.
// The postfix is written to StringWriter.
// Than the 2nd part is treated as as a new string for the next line.
//
func (p *T) stringsFromASCIIDecode() {
	a := newStringsFromASCIIDecoder(p.bytes, p.strings)
	go a.unmarshal()

}
