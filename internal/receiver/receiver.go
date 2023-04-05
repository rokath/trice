// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This is possibly obsolete information!
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Package receiver is responsible for getting and handling the bytes from the communication port.
// There ae different communication ports possible: COM, RTT, TCP...
// It is activated by the trice package and calls emit.Trice() on received bytes chunks.
//
// Implementation
// ==============
// - trice.DoReceive() is called when trice ist started in log or receiver mode
// - In dependence on receiverDevice DoSerial() or DoSeggerRTT() or ... is activated (named DoDevice() from now)
// - DoDevice()
//   - performs device specific initialization
//   - performs Start() which starts the go routine receiving(), responsible for raw data (see below)
//   - and ends in device.doReceive()
// - device is a composed type from triceReceiver (common functionality) and deviceReceiver (device specific functionality)
// - The newDevice(...) function creates the device specific instance and links to it the triceReceiver instance created by
// device.triceReceiver = newTriceReceiver(device). The device parameter for newTriceReceiver() is the linked device interface
// into the triceReceiver instance so that the in this way composed device instance can use the common triceReceiver parts as well.
// - receiving()
//  - scans the incoming data stream for valid headers,
//  - and puts these into the triceChannel, when trice headers
//  - or puts them together with the following package data into the buffer channel if it is a valid package
//  - It is up to the trice implementation to add more channels here for other protocols
// - doReceive() is the endless loop waiting for data in the channels filled in by receiving()
//  - All incoming buffer packets are collected into a byte slice for later usage (case `TRICE_RUNTIME_GENERATED_STRINGS_SUPPORT_EXPERIMENTAL`)
//  - If a trice header comes out of the triceChannel emit.Trice() is called
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

package receiver

import (
	"bytes"
	"encoding/hex"
	"fmt"
	"io"
	"log"
	"net"
	"os"
	"path/filepath"
	"strings"
	"time"
	"unicode"

	"github.com/rokath/trice/internal/com"
	"github.com/rokath/trice/internal/link"
	"github.com/rokath/trice/pkg/msg"
	"github.com/spf13/afero"
)

var (
	// ShowInputBytes displays incoming bytes if set true.
	ShowInputBytes bool

	// Port is the trice receiver to use.
	Port string

	// PortArguments are the trice receiver device specific arguments.
	PortArguments string

	// ExecCommand is a commandline to be executed.
	ExecCommand string

	// DefaultLinkArgs replaces "default" args value for ST-LINK and J-LINK port.
	DefaultLinkArgs = "-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0 -RTTSearchRanges 0x20000000_0x1000"

	// DefaultCOMArgs replaces "default" args value for serial port.
	DefaultCOMArgs = ""

	// DefaultTCP4Args replaces "default" args value for TCP4 port.
	DefaultTCP4Args = "localhost:17001" // OpenOCD starts a server on localhost:17001 where it dumps all RTT messages.

	// DefaultFileArgs replaces "default" args value for FILE port.
	DefaultFileArgs = "trices.raw"

	// DefaultBUFFERArgs replaces "default" args value for BUFFER port.
	DefaultBUFFERArgs = "0 0 0 0"

	// DefaultDumpArgs replaces "default" args value for BUFFER port.
	DefaultDumpArgs = ""

	// Verbose gives more information on output if set. The value is injected from main packages.
	Verbose bool

	// BinaryLogfileName holds a filename, the trice messages are stored to in binary form.
	BinaryLogfileName string
)

// spaceStringsBuilder returns str without whitespaces.
//
// Code copied from https://stackoverflow.com/questions/32081808/strip-all-whitespace-from-a-string
func spaceStringsBuilder(str string) string {
	var b strings.Builder
	b.Grow(len(str))
	for _, ch := range str {
		if !unicode.IsSpace(ch) {
			b.WriteRune(ch)
		}
	}
	return b.String()
}

// removeWhitespaces returns s without whitespaces.
func removeWhitespaces(s string) string {
	return spaceStringsBuilder(s)
}

// scanHexDump expects in s 2-bytes hexadecimal characters groups, optionally whitespace or comma separated like "09 a1 fe", and returns them as buf.
//
// s is expected to be a space separated hex print like "09 a1 fe"
func scanHexDump(s string) (buf []byte, e error) {
	s = strings.ReplaceAll(s, ",", " ")
	s = removeWhitespaces(s)
	return hex.DecodeString(s)
}

// scanBytes assumes in s whitespace separated decimal numbers between 0 and 255 and returns them in buf.
//
// TODO: improve code.
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
		fmt.Sscan(a, &b)
		// n, e := fmt.Sscan(a, &b)
		// msg.FatalOnFalse(1 == n)
		// msg.FatalOnErr(e)
		buf = append(buf, b)
	}
	return
}

// tcp4 holds an open tcp4 connection.
type tcp4 struct {
	w    io.Writer // os.Stdout
	conn *net.TCPConn
}

// newTCP4Connection returns a readCloser capable tcp4 instance.
func newTCP4Connection(_ io.Writer, endpoint string) *tcp4 {
	r := &tcp4{}
	addr, err := net.ResolveTCPAddr("tcp4", endpoint)
	if err != nil {
		log.Fatal(endpoint, err)
	}
	r.conn, err = net.DialTCP("tcp4", nil, addr)
	if err != nil {
		log.Fatal(addr, err)
	}
	return r
}

// Read is part of the exported interface io.ReadCloser. It reads a slice of bytes.
func (p *tcp4) Read(b []byte) (int, error) {
	return p.conn.Read(b)
}

func (p *tcp4) Write(b []byte) (int, error) {
	return p.conn.Write(b)
}

// Close is part of the exported interface io.ReadCloser. It ends the connection.
func (p *tcp4) Close() error {
	if Verbose {
		fmt.Fprintln(p.w, "Closing tcp4 device.")
	}
	return p.conn.Close()
}

// file holds an opened file handle.
type file struct {
	w  io.Writer // os.Stdout
	fn string
	fh afero.File
}

// newFileReader returns a readCloser capable file instance.
func newFileReader(_ io.Writer, fSys *afero.Afero, fn string) *file {
	r := &file{}
	fh, err := fSys.Open(fn)
	if err != nil {
		log.Fatal(fn, err)
	}
	r.fn = fn
	r.fh = fh
	return r
}

// Read is part of the exported interface io.ReadCloser. It reads a slice of bytes.
func (p *file) Read(b []byte) (int, error) {
	return p.fh.Read(b)
}

func (p *file) Write(b []byte) (int, error) {
	return len(b), nil // discard, do not change files! p.fh.Write(b)
}

// Close is part of the exported interface io.ReadCloser. It ends the connection.
func (p *file) Close() error {
	if Verbose {
		fmt.Fprintln(p.w, "Closing file", p.fn)
	}
	return p.fh.Close()
}

// NewReadWriteCloser returns a ReadCloser for the specified port and its args.
// err is nil on successful open.
// When port is "COMn" args can be used to be "TARM" to use a different driver for dynamic testing.
// When port is "DUMP", args is expected to be a space or comma separated hex print like "09 a1 fe"
// When port is "BUFFER", args is expected to be a decimal byte sequence in the same format as for example coming from one of the other ports.
// When port is "JLINK" args contains JLinkRTTLogger.exe specific parameters described inside UM08001_JLink.pdf.
// When port is "STLINK" args has the same format as for "JLINK"
func NewReadWriteCloser(w io.Writer, fSys *afero.Afero, verbose bool, port, args string) (r io.ReadWriteCloser, err error) {
	switch strings.ToUpper(port) {

	case "JLINK", "STLINK", "J-LINK", "ST-LINK":
		if PortArguments == "" { // nothing assigned in args
			PortArguments = DefaultLinkArgs
		}
		l := link.NewDevice(w, fSys, port, args)
		if nil != l.Open() {
			err = fmt.Errorf("can not open link device %s with args %s", port, args)
		}
		r = l
	case "TCP4":
		if PortArguments == "" { // nothing assigned in args
			PortArguments = DefaultTCP4Args
		}
		if ExecCommand != "" {
			fmt.Println("todo: execute ", ExecCommand)
		}
		l := newTCP4Connection(w, args)
		r = l
	case "FILE", "FILEBUFFER":
		if PortArguments == "" { // nothing assigned in args
			PortArguments = DefaultFileArgs
		}
		r = newFileReader(w, fSys, args)
	case "DUMP":
		if PortArguments == "" { // nothing assigned in args
			PortArguments = DefaultDumpArgs
		}
		var rwc io.ReadWriteCloser
		rwc = &buffer{} // Make the io.ReadWriteCloser actually do something.
		buf, _ := scanHexDump(args)
		rwc.Write(buf)
		r = rwc
	case "BUFFER":
		if PortArguments == "" { // nothing assigned in args
			PortArguments = DefaultBUFFERArgs
		}
		var rwc io.ReadWriteCloser
		rwc = &buffer{} // Make the io.ReadWriteCloser actually do something.
		buf := scanBytes(args)
		rwc.Write(buf)
		r = rwc
		//r = ioutil.NopCloser(bytes.NewBuffer(buf))
	default:
		if PortArguments == "" { // nothing assigned in args
			PortArguments = DefaultCOMArgs
		}
		if verbose {
			fmt.Println("Assuming", port, "is serial port.")
		}
		var c com.COMport // interface type
		//if args == "TARM" { // for comparing dynamic behavior
		//	c = com.NewCOMPortTarm(w, verbose, port)
		//} else {
		c = com.NewPort(w, port, verbose)
		//}
		if !c.Open() {
			err = fmt.Errorf("can not open %s", port)
		}
		r = c
	}
	return
}

// buffer is just here to make bytes.Buffer an io.ReadWriteCloser.
// Read about embedding to see how this works.
type buffer struct {
	bytes.Buffer
}

// Add a Close method to our buffer so that we satisfy io.ReadWriteCloser.
func (b *buffer) Close() error {
	b.Buffer.Reset()
	return nil
}

// ////////////////////////////////////////////////////////////////////////////////////////////////
// log input                                                                                     //
//
//	//
type binaryLogger struct {
	w io.Writer
	r io.ReadCloser
}

// NewBinaryLogger returns a ReadWriteCloser `in` which is internally using reader `from`.
// Calling the `in` Read method leads to internally calling the `from` Read method
// but lets to do some additional logging
func NewBinaryLogger(w io.Writer, fSys *afero.Afero, from io.ReadWriteCloser) (in io.ReadWriteCloser) {
	fn := BinaryLogfileName
	if fn == "none" || fn == "off" || fn == "nul" || fn == "" {
		return from
	}

	if filepath.Base(fn) == "auto" { // "2006-01-02_1504-05_trice.bin" is the pattern for default binary logfile name. The timestamp is replaced with the actual time.
		fn = filepath.Join(filepath.Dir(fn), time.Now().Format("2006-01-02_1504-05_trice.bin")) // Replace timestamp in default log filename.
	} // Otherwise, use cli defined log filename.

	if Verbose {
		fmt.Fprintf(w, "Writing trice input to binary logfile %s...\n", fn)
	}
	lfHandle, err := fSys.OpenFile(fn, os.O_RDWR|os.O_CREATE|os.O_APPEND, 0666)
	msg.FatalOnErr(err)

	p := new(binaryLogger)
	p.r = from
	p.w = lfHandle
	return p
}

func (p *binaryLogger) Read(buf []byte) (count int, err error) {
	count, err = p.r.Read(buf)
	if 0 < count || (err != nil && err != io.EOF) {
		p.w.Write(buf[:count])
	}
	return
}

// Close is needed to satisfy the ReadCloser interface.
func (p *binaryLogger) Close() error { return nil }

func (p *binaryLogger) Write(buf []byte) (count int, err error) { return 0, nil }

//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////////////////////////
// dynamic debug                                                                                 //
//
//	//
type bytesViewer struct {
	w io.Writer
	r io.ReadWriteCloser
}

// NewBytesViewer returns a ReadCloser `in` which is internally using reader `from`.
// Calling the `in` Read method leads to internally calling the `from` Read method
// but lets to do some additional action like logging
func NewBytesViewer(w io.Writer, from io.ReadWriteCloser) (in io.ReadWriteCloser) {
	p := &bytesViewer{w, from}
	return p
}

func (p *bytesViewer) Read(buf []byte) (count int, err error) {
	count, err = p.r.Read(buf)
	if 0 < count || (err != nil && err != io.EOF) {
		fmt.Fprint(p.w, "Input(")
		for i, x := range buf[:count] {
			if i < count-1 {
				fmt.Fprintf(p.w, "%02x ", x)
			} else {
				fmt.Fprintf(p.w, "%02x", x)
			}
		}
		fmt.Fprintln(p.w, ")")
	}
	return
}

// Close is needed to satisfy the ReadCloser interface.
func (p *bytesViewer) Close() error { return nil }

// Close is needed to satisfy the ReadCloser interface.
func (p *bytesViewer) Write(_ []byte) (int, error) { return 0, nil }

//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This is possibly obsolete code
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  // Loop prepares writing and list and provides a retry mechanism for unplugged UART.
//  func Loop() {
//
//  	list := id.New()
//  	sw := emitter.New(emitter.Prefix)
//
//  	for {
//
//  		// (re-)setup input port
//  		rc, e := NewReader(Port, PortArguments)
//  		if nil != e {
//  			if Verbose {
//  				fmt.Println(e)
//  			}
//  			return // true
//  		}
//  		defer rc.Close()
//  		if ShowInputBytes {
//  			rc = newBytesViewer(rc)
//  		}
//
//  		f := decoder.Translate(os.Stdout,sw, list, rc)
//  		if false == f {
//  			return
//  		}
//
//  		time.Sleep(100 * time.Millisecond) // retry interval
//  	}
//  }
//
//  // errorFatal ends in osExit(1) if err not nil.
//  func errorFatal(err error) {
//  	if nil == err {
//  		return
//  	}
//  	if Verbose {
//  		_, file, line, _ := runtime.Caller(1)
//  		log.Fatal(err, " "+filepath.Base(file)+" ", line)
//  	}
//  	log.Fatal(err)
//
//  // not needed
//  func runEsc2(sw *emitter.TriceLineComposer, list *id.List) {
//  again: // (re-)setup input port
//  	portReader, e := decoder.NewReader(Port, "")
//  	if nil != e {
//  		if verbose {
//  			fmt.Println(e)
//  		}
//  		time.Sleep(1000 * time.Millisecond)
//  		goto again
//  	}
//  	defer portReader.Close()
//  	if showInputBytes {
//  		portReader = newBytesViewer(portReader)
//  	}
//  	//var dec decoder.StringsReader = decoder.NewEsc(list, portReader)
//  }
//
//
//  // This works well but is not well refactored
//  func receiveEscTricesAndDisplayAnsiColor(
//  	sw *emitter.TriceLineComposer,
//  	rd io.ReadCloser,
//  	list *id.List,
//  	hardReadError chan bool) (et *translator.EscTranslator) {
//
//  	// uses rd for reception and the io.StringWriter interface sw for writing.
//  	// collects trice bytes to a complete esc trice message, generates the appropriate string using list and writes it to the provided io.StringWriter
//  	et = translator.NewEscTrices(sw, list, rd, hardReadError)
//  	return
//  }
//
//  func receiveBareSimpleTricesAndDisplayAnsiColor(
//  	sw *emitter.TriceLineComposer,
//  	rd io.ReadCloser,
//  	list *id.List,
//  	hardReadError chan bool) (bt *translator.BareTranslator) {
//
//  	// triceAtomsReceiver uses the io.Reader interface from s and implements the TriceAtomsReceiver interface.
//  	// It scans the raw input byte stream and decodes the trice atoms it transmits to the TriceAtomsReceiver interface.
//  	triceAtomsReceiver := receiver.NewTricesfromBare(rd, hardReadError)
//
//  	// bt uses triceAtomsReceiver for reception and the io.StringWriter interface sw for writing.
//  	// It collects trice atoms to a complete trice, generates the appropriate string using list and writes it to the provided io.StringWriter
//  	bt = translator.NewSimpleTrices(sw, list, triceAtomsReceiver)
//  	return
//  }
