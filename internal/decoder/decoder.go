// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package decoder

import (
	"bufio"
	"errors"
	"io"
	"time"

	"github.com/rokath/trice/internal/bare"
)

var (
	// TimeStampFormat is the PC timestamp format.
	TimeStampFormat = "off"
)

// Timestamp returns local time as string according var TimeStampFormat
// https://www.alexedwards.net/blog/an-overview-of-go-tooling#managing-dependencies
func Timestamp() string {
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

// Pkg is the basic command payload data structure. A Pkg contains a slice of bytes.  The byte count is coded implicit in the slice len.
type Pkg struct {
	Payload []byte // package data filled with len(Payload) bytes
}

// Command is the data type for commands
type Command struct {
	TID byte  // type identifyer byte
	FID byte  // function identifyer byte
	PIX byte  // packet index (2 lsb packet type and and 6 msb cycle counter)
	DPC byte  // data package count as real count value, 0 ^= no package, 255 ^= 255 packages
	PKG []Pkg // each command can have several data packages as payload
}

// StringWriter is the interface that wraps the basic string Write method.
//
// Write writes len(p) strings from p to the underlying data stream. It returns the number of strings
// written from p (0 <= n <= len(p)) and any error encountered that caused the write to stop early.
// Write must return a non-nil error if it returns n < len(p). Write must not modify the slice data, even temporarily.
//
// Implementations must not retain p.
type StringWriter interface {
	Write(p []string) (n int, err error)
}

// CommandWriter is the interface that wraps the basic command Write method.
//
// Write writes len(p) commands from p to the underlying data stream. It returns the number of commands
// written from p (0 <= n <= len(p)) and any error encountered that caused the write to stop early.
// Write must return a non-nil error if it returns n < len(p). Write must not modify the slice data, even temporarily.
//
// Implementations must not retain p.
type CommandWriter interface {
	Write(p []Command) (n int, err error)
}

// // IF is the provided decoder interface.
// type IF interface {
// 	Reader([]byte) (int, error)
// 	StringWriter([]string) (int, error)
// 	CommandWriter([]Command) (int, error)
// }

// T is the decoder type with encoding methods for the byte stream.
// If mixed data the decoder needs to try several encoding methods
type T struct {
	bytes    io.Reader          // reader for input data stream
	encoding []string           // the way input data encoded, several encodings possible
	decode   func()             // the decoding function to use
	trice    chan<- []bare.Item // the trice atoms
	ascii    chan<- string      // the ascii strings decoded
	command  chan<- Command     // the arrived commands
	dropped  chan<- byte        // the byte not interpreted
	err      chan<- error       // the byte not interpreted
}

// New provides a decoder with encoding properties.
// It expects r as reader and send-only channels
func New(r io.Reader, encoding []string, trice chan<- []bare.Item, command chan<- Command, ascii chan<- string, dropped chan<- byte, err chan<- error) (*T, error) {
	p := &T{
		bytes:    r,
		encoding: encoding,
		trice:    trice,
		command:  command,
		ascii:    ascii,
		dropped:  dropped,
		err:      err,
	}
	if 1 != len(encoding) {
		return nil, errors.New("only one encoding method supported yet")
	}
	switch encoding[0] {
	case "ascii":
		p.decode = p.stringsFromASCIIDecode
	case "bare":
		p.decode = p.bareDecode
	case "bareXTEACrypted":
		p.decode = p.bareXTEADecode
	case "wrap", "wrapped":
		p.decode = p.wrapDecode
	case "wrapXTEACrypted":
		p.decode = p.wrapXTEADecode
	default:
		return nil, errors.New("unknown encoding method")
	}
	return p, nil
}

// Start lets the decoder begin to work.
func (p *T) Start() {
	go func() {
		for {
			p.decode()
		}
	}()
}

// stringsFromASCIIDecode assumes the bytes coming from io.Reader are an ASCII data stream delimited by newlines.
// As newline are considered '\n' or '\r\n'.
// The incoming characters are split in strings also removing the newlines and transferred to the ascii channel.
func (p *T) stringsFromASCIIDecode() {
	scanner := bufio.NewScanner(p.bytes)
	for scanner.Scan() {
		p.ascii <- scanner.Text()
	}
	if err := scanner.Err(); err != nil {
		p.err <- err
	}
}

// bareDecode assumes the bytes as bare data stream
func (p *T) bareDecode() {
	x, _ := bare.NewReader(p.bytes, "bare")
	bare := make([]bare.Item, 4096)
	n, e := x.Read(bare)
	if nil != e && io.EOF != e {
		p.err <- e
	}
	bare = bare[:n]
	p.trice <- bare
}

// wrapDecode assumes the bytes as wrapped data stream
func (p *T) wrapDecode() {
	x, _ := bare.NewReader(p.bytes, "wrap")
	bare := make([]bare.Item, 4096)
	n, e := x.Read(bare)
	if nil != e && io.EOF != e {
		p.err <- e
	}
	bare = bare[:n]
	p.trice <- bare
}

// bareDecode assumes the bytes as bare data stream
func (p *T) bareXTEADecode() {
}

// wrapDecode assumes the bytes as wrapped data stream
func (p *T) wrapXTEADecode() {
}
