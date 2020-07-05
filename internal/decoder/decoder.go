// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package decoder reads from differently encoded byte streams and writes decoded items to appropriate writers.
//
// To use it you need a to provide a byte reader like COM or FILE and information about the byte stream encoding.
// Also a new decoder expects writers according the expected items.
// decoder has a StringWriter interface where it writes decoded trices as string slice.
// Because strings can arrive as trices but also as command messages the decoder needs to handle both in one turn
// for correct synchronisation.
// The decoder is extendable in a way to get additional writers for some data type.
// Also the decoding can made be able to handle different encodings in one data stream.
// In future a heuristic approach for auto encoding could be implemented as well.
package decoder

import (
	"errors"
	"fmt"
	"io"
	"time"
)

// Pkg is the command payload data structure
type Pkg struct {
	//uint16 Count1 // count-1 value, 0 ^= 1 value, 65535 ^= 65536 values
	//uint32 Count // count value, 0 ^= empty payload, 65535 ^= 65535 bytes inside Payload - this allows empty packages inbetween
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

/*
// IF is the provided decoder interface.
type IF interface {
	Reader([]byte) (int, error)
	StringWriter([]string) (int, error)
	CommandWriter([]Command) (int, error)
}*/

// T is the decoder type with encoding methods for the byte stream.
// If mixed data the decoder needs to try several encoding methods
type T struct {
	bytes    io.Reader     // reader for input data stream
	encoding []string      // the way input data encoded
	strings  StringWriter  // the trice string writer to use
	commands CommandWriter // the command writer to use
	decode   func()        // the decoding function to use
}

// New provides a decoder with encoding properties.
// It expects r as reader and s & c as writers
func New(r io.Reader, encoding []string, s StringWriter, c CommandWriter) (*T, error) {
	p := &T{
		bytes:    r,
		encoding: encoding,
		strings:  s,
		commands: c,
	}
	if 1 != len(encoding) {
		return nil, errors.New("only one encoding method supported yet")
	}
	switch encoding[0] {
	case "ascii":
		p.decode = p.asciiDecode
	case "bare":
		p.decode = p.bareDecode
	case "wrap", "wrapped":
		p.decode = p.wrapDecode
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

// asciiDecode assumes the bytes as ASCII data steam
func (p *T) asciiDecode() {
	b := make([]byte, 1024)
	n, err := p.bytes.Read(b)
	if nil != err {
		fmt.Println(err)
		time.Sleep(time.Second)
	}
	b = b[:n]
	s := string(b)
	var ss []string
	ss = append(ss, s)

	// write the decoded strings
	n, err = p.strings.Write(ss)
	if nil != err {
		fmt.Println(err)
		time.Sleep(time.Second)
	}
	if 1 != n {
		fmt.Println(n)
		time.Sleep(time.Second)
	}

	if nil != p.commands {
		var cs []Command
		// write the decoded commands
		p.commands.Write(cs)
		if nil != err {
			fmt.Println(err)
			time.Sleep(time.Second)
		}
		if 1 != n {
			fmt.Println(n)
			time.Sleep(time.Second)
		}
	}
}

// bareDecode assumes the bytes as bare data stream
func (p *T) bareDecode() {
}

// wrapDecode assumes the bytes as wrapped data stream
func (p *T) wrapDecode() {
}
