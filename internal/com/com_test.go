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
