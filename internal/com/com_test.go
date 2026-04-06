// SPDX-License-Identifier: MIT

package com

import (
	"bytes"
	"errors"
	"io"
	"sync"
	"testing"
	"time"

	"go.bug.st/serial"
)

// dummyPort implements the serial.Port interface so tests can run without real hardware.
type dummyPort struct{}

func (dummyPort) Read([]byte) (int, error)           { return 0, io.EOF }
func (dummyPort) Write([]byte) (int, error)          { return 0, nil }
func (dummyPort) Close() error                       { return nil }
func (dummyPort) SetMode(*serial.Mode) error         { return nil }
func (dummyPort) Drain() error                       { return nil }
func (dummyPort) ResetInputBuffer() error            { return nil }
func (dummyPort) ResetOutputBuffer() error           { return nil }
func (dummyPort) SetDTR(bool) error                  { return nil }
func (dummyPort) SetRTS(bool) error                  { return nil }
func (dummyPort) SetReadTimeout(time.Duration) error { return nil }
func (dummyPort) Break(time.Duration) error          { return nil }

func (dummyPort) GetModemStatusBits() (*serial.ModemStatusBits, error) {
	return &serial.ModemStatusBits{}, nil
}

type trackingPort struct {
	readData []byte
	writes   [][]byte
	closed   bool
}

func (p *trackingPort) Read(buf []byte) (int, error) {
	n := copy(buf, p.readData)
	return n, nil
}

func (p *trackingPort) Write(buf []byte) (int, error) {
	cp := append([]byte(nil), buf...)
	p.writes = append(p.writes, cp)
	return len(buf), nil
}

func (p *trackingPort) Close() error                       { p.closed = true; return nil }
func (p *trackingPort) SetMode(*serial.Mode) error         { return nil }
func (p *trackingPort) Drain() error                       { return nil }
func (p *trackingPort) ResetInputBuffer() error            { return nil }
func (p *trackingPort) ResetOutputBuffer() error           { return nil }
func (p *trackingPort) SetDTR(bool) error                  { return nil }
func (p *trackingPort) SetRTS(bool) error                  { return nil }
func (p *trackingPort) SetReadTimeout(time.Duration) error { return nil }
func (p *trackingPort) Break(time.Duration) error          { return nil }
func (p *trackingPort) GetModemStatusBits() (*serial.ModemStatusBits, error) {
	return &serial.ModemStatusBits{}, nil
}

// resetComGlobals preserves and restores global serial settings around each test.
func resetComGlobals(t *testing.T) func() {
	t.Helper()
	oldParity := Parity
	oldStop := StopBits
	oldData := DataBits
	oldBaud := BaudRate
	return func() {
		Parity = oldParity
		StopBits = oldStop
		DataBits = oldData
		BaudRate = oldBaud
	}
}

// withSerialSettings configures minimal valid serial parameters for the admin helper tests.
func withSerialSettings(t *testing.T) {
	t.Helper()
	reset := resetComGlobals(t)
	if reset != nil {
		t.Cleanup(reset)
	}
	Parity = "none"
	StopBits = "1"
	DataBits = 8
	BaudRate = 115200
}

// TestGetSerialPortsFound validates the walker reports found ports when the serial open succeeds.
func TestGetSerialPortsFound(t *testing.T) {
	withSerialSettings(t)
	origOpen := openSerial
	origList := getPortsList
	t.Cleanup(func() {
		openSerial = origOpen
		getPortsList = origList
	})

	getPortsList = func() ([]string, error) {
		return []string{"COM1"}, nil
	}
	openCalled := make([]string, 0, 1)
	var mu sync.Mutex
	openSerial = func(name string, mode *serial.Mode) (serial.Port, error) {
		mu.Lock()
		openCalled = append(openCalled, name)
		mu.Unlock()
		return dummyPort{}, nil
	}

	var out bytes.Buffer
	ports, err := GetSerialPorts(&out)
	if err != nil {
		t.Fatalf("GetSerialPorts returned error: %v", err)
	}
	if len(ports) != 1 || ports[0] != "COM1" {
		t.Fatalf("unexpected ports list: %#v", ports)
	}
	if !bytes.Contains(out.Bytes(), []byte("Found port: ")) {
		t.Fatalf("expected success output, got %q", out.String())
	}
	mu.Lock()
	if len(openCalled) != 1 || openCalled[0] != "COM1" {
		t.Fatalf("openSerial not invoked properly: %#v", openCalled)
	}
	mu.Unlock()
}

// TestGetSerialPortsUsed ensures the helper still returns port list even when the port cannot be opened.
func TestGetSerialPortsUsed(t *testing.T) {
	withSerialSettings(t)
	origOpen := openSerial
	origList := getPortsList
	t.Cleanup(func() {
		openSerial = origOpen
		getPortsList = origList
	})

	errTest := errors.New("fail")
	getPortsList = func() ([]string, error) {
		return []string{"COM1"}, nil
	}
	openSerial = func(name string, mode *serial.Mode) (serial.Port, error) {
		return nil, errTest
	}

	var out bytes.Buffer
	ports, err := GetSerialPorts(&out)
	if err != nil {
		t.Fatalf("unexpected error: %v", err)
	}
	if len(ports) != 1 {
		t.Fatalf("expected ports even on error")
	}
	if !bytes.Contains(out.Bytes(), []byte("(used)")) {
		t.Fatalf("expected used marker, got %q", out.String())
	}
}

// TestPortReadWriteCloseWithoutOpen verifies the guard rails for unopened ports.
func TestPortReadWriteCloseWithoutOpen(t *testing.T) {
	p := &port{}
	_, err := p.Read(make([]byte, 1))
	if err == nil {
		t.Fatalf("expected read error for unopened port")
	}
	_, err = p.Write([]byte{1})
	if err == nil {
		t.Fatalf("expected write error for unopened port")
	}
	err = p.Close()
	if err == nil {
		t.Fatalf("expected close error for unopened port")
	}
}

// TestNewPortOpenReadWriteClose verifies serial operations use the injected port implementation.
func TestNewPortOpenReadWriteClose(t *testing.T) {
	withSerialSettings(t)
	origOpen := openSerial
	t.Cleanup(func() { openSerial = origOpen })

	var gotName string
	var gotMode serial.Mode
	stub := &trackingPort{readData: []byte("ok")}
	openSerial = func(name string, mode *serial.Mode) (serial.Port, error) {
		gotName = name
		gotMode = *mode
		return stub, nil
	}

	var out bytes.Buffer
	p := NewPort(&out, "COM7", true)
	if !p.Open() {
		t.Fatalf("expected Open to succeed")
	}
	if gotName != "COM7" {
		t.Fatalf("unexpected port name %q", gotName)
	}
	if gotMode.BaudRate != BaudRate || gotMode.DataBits != DataBits {
		t.Fatalf("unexpected serial mode %#v", gotMode)
	}

	buf := make([]byte, 2)
	n, err := p.Read(buf)
	if err != nil || string(buf[:n]) != "ok" {
		t.Fatalf("unexpected read result %d %v %q", n, err, string(buf[:n]))
	}
	n, err = p.Write([]byte("go"))
	if err != nil || n != 2 {
		t.Fatalf("unexpected write result %d %v", n, err)
	}
	if len(stub.writes) != 1 || string(stub.writes[0]) != "go" {
		t.Fatalf("unexpected writes %#v", stub.writes)
	}
	if err := p.Close(); err != nil {
		t.Fatalf("close failed: %v", err)
	}
	if !stub.closed {
		t.Fatalf("expected tracking port to be closed")
	}
	if !bytes.Contains(out.Bytes(), []byte("New COM port:")) {
		t.Fatalf("expected verbose constructor output, got %q", out.String())
	}
}

// TestGetSerialPortsErrorsAndEmptyList verifies passthrough behavior for discovery edge cases.
func TestGetSerialPortsErrorsAndEmptyList(t *testing.T) {
	withSerialSettings(t)
	origList := getPortsList
	t.Cleanup(func() { getPortsList = origList })

	getPortsList = func() ([]string, error) {
		return nil, errors.New("enumeration failed")
	}
	var out bytes.Buffer
	ports, err := GetSerialPorts(&out)
	if err == nil || len(ports) != 0 {
		t.Fatalf("expected discovery error, got ports=%v err=%v", ports, err)
	}

	getPortsList = func() ([]string, error) {
		return []string{}, nil
	}
	Verbose = true
	t.Cleanup(func() { Verbose = false })
	out.Reset()
	ports, err = GetSerialPorts(&out)
	if err != nil || len(ports) != 0 {
		t.Fatalf("expected empty list without error, got ports=%v err=%v", ports, err)
	}
	if !bytes.Contains(out.Bytes(), []byte("No serial ports found!")) {
		t.Fatalf("expected empty list message, got %q", out.String())
	}
}
