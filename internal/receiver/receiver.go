// SPDX-License-Identifier: MIT

// Package receiver opens raw input streams for the host logger. Framing and
// event decoding belong to the decoder selected by the translator.
//
// args.logLoop owns the io.ReadWriteCloser returned by NewReadWriteCloser and
// any byte-display or binary-capture wrappers. Closing the outermost wrapper
// releases its source and capture file; diagnostic writers remain open.
// Wrappers forward outgoing target commands separately from received bytes.
package receiver

import (
	"bytes"
	"encoding/hex"
	"errors"
	"fmt"
	"io"
	"log"
	"net"
	"os"
	"path/filepath"
	"strings"
	"sync"
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

	// DefaultLinkArgs replaces "default" args value for ST-LINK and J-LINK port.
	DefaultLinkArgs = "-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0 -RTTSearchRanges 0x20000000_0x1000"

	// DefaultCOMArgs replaces "default" args value for serial port.
	DefaultCOMArgs = "-baud 115200"

	// DefaultTCP4Args replaces "default" args value for TCP4 port.
	DefaultTCP4Args = "localhost:17001" // OpenOCD starts a server on localhost:17001 where it dumps all RTT messages.

	// DefaultUDP4Args replaces "default" args value for UDP4 port.
	DefaultUDP4Args = "0.0.0.0:17005"

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

// newTCP4Connection opens a TCP command and input connection owned by the caller.
func newTCP4Connection(w io.Writer, endpoint string) *tcp4 {
	r := &tcp4{w: w}
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

// udp4 holds a UDP listener without a configured reply peer.
type udp4 struct {
	w    io.Writer // os.Stdout
	conn *net.UDPConn
}

// newUDPConnection opens a UDP input listener owned by the caller.
func newUDPConnection(w io.Writer, endpoint string) *udp4 {
	r := &udp4{w: w}
	addr, err := net.ResolveUDPAddr("udp4", endpoint)
	if err != nil {
		log.Fatal(endpoint, err)
	}

	r.conn, err = net.ListenUDP("udp", addr)
	if err != nil {
		log.Fatalf("Failed to listen on UDP port: %v", err)
	}

	return r
}

// Read is part of the exported interface io.ReadCloser. It reads a slice of bytes.
func (p *udp4) Read(b []byte) (int, error) {
	n, _, err := p.conn.ReadFromUDP(b) // peer address is ignored
	return n, err
}

// Write rejects commands because this listener has no configured reply peer.
func (p *udp4) Write(b []byte) (int, error) {
	return 0, errors.New("UDP4 target commands are not supported: no reply address is configured")
}

// Close is part of the exported interface io.ReadCloser. It ends the connection.
func (p *udp4) Close() error {
	if Verbose {
		fmt.Fprintln(p.w, "Closing udp4 device.")
	}
	return p.conn.Close()
}

// file holds an opened file handle.
type file struct {
	w  io.Writer // os.Stdout
	fn string
	fh afero.File
}

// newFileReader opens fn read-only; command writes are discarded.
func newFileReader(w io.Writer, fSys *afero.Afero, fn string) *file {
	r := &file{w: w}
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

// NewReadWriteCloser opens the selected transport or replay source.
// The caller closes the returned stream, or its outermost wrapper; w is borrowed
// for diagnostics. TCP, UDP and file constructors call log.Fatal on open errors.
//
// DUMP/HEX accept hexadecimal bytes such as "09 a1 fe"; BUFFER/DEC accept decimal
// bytes such as "9 161 254". JLINK/STLINK use logger arguments; OPENOCD uses the
// fixed "-f openocd.cfg" configuration in link.NewDevice.
func NewReadWriteCloser(w io.Writer, fSys *afero.Afero, verbose bool, port, args string) (r io.ReadWriteCloser, err error) {
	if Verbose {
		if args == "default" {
			fmt.Fprintln(w, "Assigning default arguments for port", port)
		}
	}

	switch strings.ToUpper(port) {

	case "JLINK", "STLINK", "J-LINK", "ST-LINK":
		if args == "default" { // nothing assigned in args
			args = DefaultLinkArgs
		}
		if Verbose {
			fmt.Fprintln(w, "PortArguments=", args)
		}
		l := link.NewDevice(w, fSys, port, args)
		if l.Open() != nil {
			err = fmt.Errorf("can not open link device %s with args %s", port, args)
		}
		r = l
	case "OPENOCD", "OPEN-OCD", "OOCD", "OCD":
		l := link.NewDevice(w, fSys, port, args)
		if l.Open() != nil {
			err = fmt.Errorf("can not get openocd device %s with args %s", port, args)
		}
		r = l
	case "TCP4", "TCP4BUFFER": // TCP4BUFFER is undocumented, because it is used just for tests.
		if args == "default" { // nothing assigned in args
			args = DefaultTCP4Args
		}
		if Verbose {
			fmt.Fprintln(w, "PortArguments=", args)
		}
		l := newTCP4Connection(w, args)
		r = l
	case "UDP4":
		if args == "default" { // nothing assigned in args
			args = DefaultUDP4Args
		}
		if Verbose {
			fmt.Fprintln(w, "PortArguments=", args)
		}
		l := newUDPConnection(w, args)
		r = l
	case "FILE", "FILEBUFFER":
		if args == "default" { // nothing assigned in args
			args = DefaultFileArgs
		}
		if Verbose {
			fmt.Fprintln(w, "PortArguments=", args)
		}
		r = newFileReader(w, fSys, args)
	case "DUMP", "HEX":
		if args == "default" { // nothing assigned in args
			args = DefaultDumpArgs
		}
		if Verbose {
			fmt.Fprintln(w, "PortArguments=", args)
		}
		var rwc io.ReadWriteCloser
		rwc = &buffer{} // Make the io.ReadWriteCloser actually do something.
		buf, _ := scanHexDump(args)
		rwc.Write(buf)
		r = rwc
	case "BUFFER", "DEC":
		if args == "default" { // nothing assigned in args
			args = DefaultBUFFERArgs
		}
		if Verbose {
			fmt.Fprintln(w, "PortArguments=", args)
		}
		var rwc io.ReadWriteCloser
		rwc = &buffer{} // Make the io.ReadWriteCloser actually do something.
		buf := scanBytes(args)
		rwc.Write(buf)
		r = rwc
	default:
		if args == "default" { // nothing assigned in args
			args = DefaultCOMArgs
		}
		if verbose {
			fmt.Fprintln(w, "Assuming", port, "is serial port.")
			fmt.Fprintln(w, "PortArguments=", args)
		}
		var c com.COMport // interface type
		c = com.NewPort(w, port, verbose)
		if !c.Open() {
			err = fmt.Errorf("can not open %s", port)
		}
		r = c
	}
	return
}

// IsFiniteInput distinguishes replay sources from live streams. TCP4BUFFER
// finishes on peer EOF, never on a temporary empty read; FILE keeps tailing.
func IsFiniteInput(port string) bool {
	switch strings.ToUpper(port) {
	case "FILEBUFFER", "TCP4BUFFER", "HEX", "DUMP", "DEC", "BUFFER":
		return true
	default:
		return false
	}
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
	w         io.Writer
	r         io.ReadWriteCloser
	closeOnce sync.Once // Closing a wrapper owns both source and logfile, once.
	closeErr  error
}

// NewBinaryLogger appends received bytes to BinaryLogfileName unless capture is
// disabled. The wrapper owns from and the capture file, closing each once.
// The diagnostic writer w is borrowed; outgoing commands are only forwarded.
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

// Read preserves the source's byte count and error, including bytes read with
// EOF. Capture failures are joined with the input error without hiding the count.
func (p *binaryLogger) Read(buf []byte) (count int, err error) {
	count, err = p.r.Read(buf)
	if count == 0 {
		return
	}
	written, writeErr := p.w.Write(buf[:count])
	if writeErr != nil {
		return count, errors.Join(err, fmt.Errorf("write binary logfile: %w", writeErr))
	}
	if written != count {
		return count, errors.Join(err, io.ErrShortWrite)
	}
	return
}

// Close releases both owned resources even if closing the first one fails.
func (p *binaryLogger) Close() error {
	p.closeOnce.Do(func() {
		if p.r != nil {
			p.closeErr = p.r.Close()
		}
		if closer, ok := p.w.(io.Closer); ok {
			p.closeErr = errors.Join(p.closeErr, closer.Close())
		}
	})
	return p.closeErr
}

// Write preserves the command channel; only incoming bytes belong in the log.
func (p *binaryLogger) Write(buf []byte) (int, error) { return p.r.Write(buf) }

//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////////////////////////
// dynamic debug                                                                                 //
//
//	//
type bytesViewer struct {
	w         io.Writer
	r         io.ReadWriteCloser
	closeOnce sync.Once // The diagnostic writer is borrowed; only the source is owned.
	closeErr  error
}

// NewBytesViewer displays received bytes in hexadecimal on w and forwards target
// commands unchanged. The wrapper owns from but leaves the borrowed w open.
func NewBytesViewer(w io.Writer, from io.ReadWriteCloser) (in io.ReadWriteCloser) {
	p := &bytesViewer{w: w, r: from}
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

// Close delegates to the source once and leaves the borrowed writer open.
func (p *bytesViewer) Close() error {
	p.closeOnce.Do(func() {
		if p.r != nil {
			p.closeErr = p.r.Close()
		}
	})
	return p.closeErr
}

// Write forwards commands without presenting them as received bytes.
func (p *bytesViewer) Write(buf []byte) (int, error) { return p.r.Write(buf) }

//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////
