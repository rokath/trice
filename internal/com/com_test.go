package com

import (
	"bytes"
	"errors"
	"io"
	"strings"
	"testing"
	"time"

	"go.bug.st/serial"
)

type fakeSerialPort struct {
	readData []byte
	readPos  int
	written  bytes.Buffer
	closed   bool
}

func (f *fakeSerialPort) SetMode(_ *serial.Mode) error { return nil }
func (f *fakeSerialPort) Drain() error                 { return nil }
func (f *fakeSerialPort) ResetInputBuffer() error      { return nil }
func (f *fakeSerialPort) ResetOutputBuffer() error     { return nil }
func (f *fakeSerialPort) SetDTR(_ bool) error          { return nil }
func (f *fakeSerialPort) SetRTS(_ bool) error          { return nil }
func (f *fakeSerialPort) SetReadTimeout(_ time.Duration) error {
	return nil
}
func (f *fakeSerialPort) GetModemStatusBits() (*serial.ModemStatusBits, error) {
	return &serial.ModemStatusBits{}, nil
}
func (f *fakeSerialPort) Break(_ time.Duration) error { return nil }
func (f *fakeSerialPort) Close() error {
	f.closed = true
	return nil
}

func (f *fakeSerialPort) Read(p []byte) (int, error) {
	if f.readPos >= len(f.readData) {
		return 0, io.EOF
	}
	n := copy(p, f.readData[f.readPos:])
	f.readPos += n
	return n, nil
}

func (f *fakeSerialPort) Write(p []byte) (int, error) {
	return f.written.Write(p)
}

func TestNewPortNilWriterFallsBackToDiscard(t *testing.T) {
	BaudRate = 115200
	DataBits = 8
	Parity = "none"
	StopBits = "1"

	p := NewPort(nil, "COM1", true)
	if p.w == nil {
		t.Fatal("writer must not be nil")
	}
}

func TestReadWriteCloseWithoutOpen(t *testing.T) {
	BaudRate = 115200
	DataBits = 8
	Parity = "none"
	StopBits = "1"

	p := NewPort(io.Discard, "COM1", false)
	if _, err := p.Read(make([]byte, 4)); err == nil {
		t.Fatal("expected read error for unopened port")
	}
	if _, err := p.Write([]byte("x")); err == nil {
		t.Fatal("expected write error for unopened port")
	}
	if err := p.Close(); err == nil {
		t.Fatal("expected close error for unopened port")
	}
}

func TestOpenReadWriteClose(t *testing.T) {
	oldOpen := openSerial
	t.Cleanup(func() { openSerial = oldOpen })

	BaudRate = 115200
	DataBits = 8
	Parity = "even"
	StopBits = "2"

	fp := &fakeSerialPort{readData: []byte("abc")}
	openSerial = func(portName string, mode *serial.Mode) (serial.Port, error) {
		if portName != "ttyS0" {
			t.Fatalf("unexpected port name: %s", portName)
		}
		if mode.BaudRate != 115200 || mode.DataBits != 8 || mode.Parity != serial.EvenParity || mode.StopBits != serial.TwoStopBits {
			t.Fatalf("unexpected mode: %+v", *mode)
		}
		return fp, nil
	}

	p := NewPort(io.Discard, "ttyS0", false)
	if !p.Open() {
		t.Fatal("expected open success")
	}

	b := make([]byte, 8)
	n, err := p.Read(b)
	if err != nil {
		t.Fatalf("read failed: %v", err)
	}
	if got := string(b[:n]); got != "abc" {
		t.Fatalf("unexpected read data: %q", got)
	}

	n, err = p.Write([]byte("xyz"))
	if err != nil {
		t.Fatalf("write failed: %v", err)
	}
	if n != 3 || fp.written.String() != "xyz" {
		t.Fatalf("unexpected write result: n=%d data=%q", n, fp.written.String())
	}

	if err := p.Close(); err != nil {
		t.Fatalf("close failed: %v", err)
	}
	if !fp.closed {
		t.Fatal("expected underlying port close")
	}
}

func TestGetSerialPortsDeterministic(t *testing.T) {
	oldOpen := openSerial
	oldList := getPortsList
	oldVerbose := Verbose
	t.Cleanup(func() {
		openSerial = oldOpen
		getPortsList = oldList
		Verbose = oldVerbose
	})

	getPortsList = func() ([]string, error) {
		return []string{"COM_A", "COM_B"}, nil
	}
	openSerial = func(portName string, _ *serial.Mode) (serial.Port, error) {
		if portName == "COM_B" {
			return nil, errors.New("busy")
		}
		return &fakeSerialPort{}, nil
	}

	var out bytes.Buffer
	ports, err := GetSerialPorts(&out)
	if err != nil {
		t.Fatalf("GetSerialPorts failed: %v", err)
	}
	if len(ports) != 2 {
		t.Fatalf("unexpected port count: %d", len(ports))
	}

	s := out.String()
	if !strings.Contains(s, "Found port:  COM_A") {
		t.Fatalf("missing open port output: %q", s)
	}
	if !strings.Contains(s, "Found port:  COM_B (used)") {
		t.Fatalf("missing used port output: %q", s)
	}
}

func TestGetSerialPortsVerboseNoPorts(t *testing.T) {
	oldList := getPortsList
	oldVerbose := Verbose
	t.Cleanup(func() {
		getPortsList = oldList
		Verbose = oldVerbose
	})

	getPortsList = func() ([]string, error) { return nil, nil }
	Verbose = true

	var out bytes.Buffer
	ports, err := GetSerialPorts(&out)
	if err != nil {
		t.Fatalf("unexpected error: %v", err)
	}
	if len(ports) != 0 {
		t.Fatalf("expected no ports, got %d", len(ports))
	}
	if !strings.Contains(out.String(), "No serial ports found!") {
		t.Fatalf("missing verbose message: %q", out.String())
	}
}
