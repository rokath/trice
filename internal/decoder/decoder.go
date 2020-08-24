// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package decoder

import (
	"bufio"
	"encoding/binary"
	"errors"
	"io"

	"github.com/rokath/trice/internal/bare"
)

/*
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
*/
///////////////////////////////////////////////////////////////////////////////
// TRICE message struct
//

// trice & command message header for routing, syncing and conistency check
type triceMsgHeader struct {
	start byte // trice message header start value
	cad   byte // client address
	sad   byte // server address
	crc8  byte // ab^cad^sad^load[0]^load[1]^load[2]^load[3]
}

// trice message payload
type triceMsgLoad struct {
	id   uint16 // trice id
	data uint16 // 2 data byte
}

// command message payload
type cmdMsgLoad struct {
	tid uint8
	fid uint8
	pix uint8
	dpc uint8
}

//! \code
//! trice package: header without data packages
//!   |--------------------------------- fixed packet start0 byte 0xeb
//!   |   |----------------------------- client address (local address byte)
//!   |   |   |------------------------- server address (destination)
//!   |   |   |   |--------------------- exclusive-or checksum byte
//!   |   |   |   |   |----------------- ID low part
//!   |   |   |   |   |   |------------- ID high part
//!   |   |   |   |   |   |   |--------- Value Low part
//!   |   |   |   |   |   |   |   |----- Value High part
//!   v   v   v   v   v   v   v   v
//! 0xeb cad sad cr8 idL idH vaL  vaH
//!
//! com packet: header followed by 0...255 data packages
//!   |--------------------------------- fixed packet start0 byte 0xc0
//!   |   |----------------------------- following data package count fixed 1 for trice strings
//!   |   |   |------------------------- server address (destination)
//!   |   |   |   |--------------------- exclusive-or checksum byte
//!   |   |   |   |   |----------------- type identifyer byte
//!   |   |   |   |   |   |------------- function identifyer byte
//!   |   |   |   |   |   |   |--------- packet index (2 lsb packet type and and 6 msb cycle counter)
//!   |   |   |   |   |   |   |   |----- data package count
//!   v   v   v   v   v   v   v   v
//! 0xc0 cad sad cr8 tid fid pix dpc
//!
//! com type: (part of pix)
//!       bit1      |      bit0       | meaning
//!   COM_CMD_FLAG  | COM_ANSWER_FLAG |
//! ----------------|-----------------|------------------------------------------
//!         1       |        1        | \b Cmd = command expecting answer
//!         0       |        1        | \b Ans = answer to a command expecting answer
//!         1       |        0        | \b Msg = command not expecting an answer (message)
//!         0       |        0        | \b Buf = trice string package, when 0xffff==tidfid
//! \endcode
//! trice message packet
type triceMsg struct {
	hd triceMsgHeader // header
	ld triceMsgLoad   // payload
}

// Pkg is the basic command payload data structure. A Pkg contains a slice of bytes.  The byte count is coded implicit in the slice len.
type Pkg struct {
	Payload []byte // package data filled with len(Payload) bytes
}

// Command is the data type for commands.
//
// The data package count is given by len([]Pkg)
type Command struct {
	TID byte // type identifyer byte
	FID byte // function identifyer byte
	PIX byte // packet index (2 lsb packet type and and 6 msb cycle counter)
	//DPC byte  // data package count as real count value, 0 ^= no package, 255 ^= 255 packages
	//PID uint // package id (cycle counter)
	//TYPE byte
	//DATA []Pkg // each command can have several data packages as payload
	DataPackageCount uint8
	PKG              [][]byte
	r                io.Reader
}

func (c Command) DecodeData() error {
	var i uint8
	for i < c.DataPackageCount {
		var len uint16
		if err := binary.Read(c.r, binary.LittleEndian, &len); err != nil {
			return err
		}
		buf := make([]byte, len)
		io.ReadFull(c.r, buf)
		c.PKG = append(c.PKG, buf)
		i++
	}
	return nil
}

func (p Command) setVal() {
	/// x := &Pkg
	/// p.PKG.Pkg[0] = make([]byte,10)
	/// p.PKG = make([]Pkg,10)

}

type Book struct {
	TID byte
	FID byte
	PIX byte
	PKG [][]byte
}

func xxx() {

	var Some Book
	Some.TID = 111
	Some.FID = 0xff
	Some.PIX = 22
	dat0 := []byte{1, 2, 3, 4, 5}
	dat1 := []byte{11, 12}
	Some.PKG = [][]byte{dat0, dat1}
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
func New(r io.Reader, encoding []string,
	trice chan<- []bare.Item,
	command chan<- Command,
	ascii chan<- string,
	dropped chan<- byte,
	err chan<- error) (*T, error) {
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
