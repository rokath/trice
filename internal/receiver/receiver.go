// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package receiver

import (
	"bytes"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"strings"

	"github.com/rokath/trice/internal/com"
	"github.com/rokath/trice/internal/link"
	"github.com/rokath/trice/pkg/msg"
)

var (
	// ShowInputBytes displays incoming bytes if set true.
	ShowInputBytes bool

	// Port is the trice receiver to use.
	Port string

	// PortArguments are the trice receiver device specific arguments.
	PortArguments string
)

// scanBytes assumes in s whitespace separated decimal numbers between 0 and 255 and returns them in buf
func scanBytes(s string) (buf []byte) {
	s = strings.ReplaceAll(s, ",", " ")
	s = strings.ReplaceAll(s, "\t", " ")
	s = strings.ReplaceAll(s, "\r", " ")
	s = strings.ReplaceAll(s, "\n", " ")
	s = strings.ReplaceAll(s, "  ", " ")
	as := strings.Split(s, " ")
	buf = make([]byte, 0)
	for _, a := range as {
		var b byte
		n, e := fmt.Sscan(a, &b)
		msg.FatalOnFalse(1 == n)
		msg.FatalOnErr(e)
		buf = append(buf, b)
	}
	return
}

// NewReadCloser returns a ReadCloser for the specified port and its args.
// err is nil on successful open.
// When port is "COMn" args can be used to be "TARM" to use a different driver for dynamic testing.
// When port is "BUFFER", args is expected to be a byte sequence in the same format as for example coming from one of the other ports.
// When port is "JLINK" args contains JLinkRTTLogger.exe specific parameters described inside UM08001_JLink.pdf.
// When port is "STLINK" args has the same format as for "JLINK"
func NewReadCloser(port, args string) (r io.ReadCloser, err error) {
	switch port {
	case "JLINK", "STLINK", "J-LINK", "ST-LINK":
		l := link.NewDevice(port, args)
		if nil != l.Open() {
			err = fmt.Errorf("can not open link device %s with args %s", port, args)
		}
		r = l
	case "BUFFER":
		//r = ioutil.NopCloser(bytes.NewBufferString(args))
		buf := scanBytes(args)
		r = ioutil.NopCloser(bytes.NewBuffer(buf))
	default: // assuming serial port
		var c com.COMport   // interface type
		if "TARM" == args { // for comparing dynamic behaviour
			c = com.NewCOMPortTarm(port)
		} else {
			c = com.NewCOMPortGoBugSt(port)
		}
		if !c.Open() {
			err = fmt.Errorf("can not open %s", port)
		}
		r = c
		return
	}
	return
}

// ////////////////////////////////////////////////////////////////////////////////////////////////
// dynamic debug helper
//

type bytesViewer struct {
	r io.ReadCloser
}

// NewBytesViewer returns a ReadCloser `in` which is internally using reader `from`.
// Calling the `in` Read method leads to internally calling the `from` Read method
// but lets to do some additional action like logging
func NewBytesViewer(from io.ReadCloser) (in io.ReadCloser) {
	return &bytesViewer{from}
}

func (p *bytesViewer) Read(buf []byte) (count int, err error) {
	count, err = p.r.Read(buf)
	if 0 < count || (nil != err && io.EOF != err) {
		log.Println("input bytes:", err, count, buf[:count])
	}
	return
}

// Close is needed to satify the ReadCloser interface.
func (p *bytesViewer) Close() error { return nil }

//
// ////////////////////////////////////////////////////////////////////////////////////////////////
