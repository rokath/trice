// SPDX-License-Identifier: MIT

package link

import (
	"errors"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"strings"
	"testing"

	"github.com/spf13/afero"
)

type removingFs struct {
	afero.Fs
}

func (r removingFs) Remove(name string) error {
	err := r.Fs.Remove(name)
	if err != nil {
		return err
	}
	return fmt.Errorf("removed %s", filepath.Base(name))
}

// TestNewDeviceJLinkDefaults verifies the default JLink constructor fills required arguments and cleans up temporary files.
func TestNewDeviceJLinkDefaults(t *testing.T) {
	t.Helper()

	wd, err := os.Getwd()
	if err != nil {
		t.Fatalf("getwd: %v", err)
	}
	t.Cleanup(func() {
		_ = os.Chdir(wd)
	})

	tempDir := t.TempDir()
	if err := os.Chdir(tempDir); err != nil {
		t.Fatalf("chdir: %v", err)
	}

	base := removingFs{Fs: afero.NewOsFs()}
	fs := &afero.Afero{Fs: base}
	dev := NewDevice(io.Discard, fs, "JLink", "-Device STM32F030R8 -If SWD")

	if dev.Exec != "JLinkRTTLogger" {
		t.Fatalf("expected JLink exec, got %q", dev.Exec)
	}
	if dev.Lib != "JLinkARM" {
		t.Fatalf("expected JLinkARM lib, got %q", dev.Lib)
	}

	if !hasArg(dev.args, "-Device") {
		t.Fatalf("device argument missing: %v", dev.args)
	}
	if !hasArg(dev.args, "-If") {
		t.Fatalf("interface argument missing: %v", dev.args)
	}
	if !hasArg(dev.args, "-RTTChannel") {
		t.Fatalf("RTTChannel argument missing: %v", dev.args)
	}
	if !hasArg(dev.args, "-Speed") {
		t.Fatalf("Speed argument missing: %v", dev.args)
	}

	if dev.tempLogFileName == "" {
		t.Fatalf("temp log file not set")
	}

	if _, err := os.Stat(dev.tempLogFileName); err != nil {
		t.Fatalf("temp file does not exist: %v", err)
	}

	err = dev.Close()
	if err != nil {
		t.Fatalf("close: %v", err)
	}

	if _, statErr := os.Stat(dev.tempLogFileName); !errors.Is(statErr, os.ErrNotExist) {
		t.Fatalf("temp file still exists %v", statErr)
	}
}

// TestNewDeviceOpenOCDDefaults verifies the OpenOCD branch configures the static command arguments without panicking.
func TestNewDeviceOpenOCDDefaults(t *testing.T) {
	fs := &afero.Afero{Fs: afero.NewMemMapFs()}

	dev := NewDevice(io.Discard, fs, "OPENOCD", "")

	if dev.Exec != "openocd" {
		t.Fatalf("expected openocd exec, got %q", dev.Exec)
	}
	if dev.Lib != "libusb-1.0" {
		t.Fatalf("expected libusb-1.0 lib, got %q", dev.Lib)
	}
	if len(dev.args) != 2 || dev.args[0] != "-f" || dev.args[1] != "openocd.cfg" {
		t.Fatalf("unexpected openocd args: %v", dev.args)
	}
	if dev.tempLogFileName != "" {
		t.Fatalf("openocd should not use a temp file, got %q", dev.tempLogFileName)
	}
}

// TestNewDeviceUsesExplicitBinLogfile verifies that an explicit .bin target is preserved instead of creating a temp file.
func TestNewDeviceUsesExplicitBinLogfile(t *testing.T) {
	wd, err := os.Getwd()
	if err != nil {
		t.Fatalf("getwd: %v", err)
	}
	t.Cleanup(func() {
		_ = os.Chdir(wd)
	})

	tempDir := t.TempDir()
	if err := os.Chdir(tempDir); err != nil {
		t.Fatalf("chdir: %v", err)
	}

	fs := &afero.Afero{Fs: afero.NewOsFs()}
	dev := NewDevice(io.Discard, fs, "STLINK", "-Device STM32F030R8 -If SWD -Speed 4000 -RTTChannel 0 custom-output.bin")

	if filepath.Base(dev.tempLogFileName) != "custom-output.bin" {
		t.Fatalf("expected explicit bin logfile, got %q", dev.tempLogFileName)
	}
	if dev.args[len(dev.args)-1] != dev.tempLogFileName {
		t.Fatalf("expected final arg to use absolute logfile path, got %v", dev.args)
	}
}

// TestDeviceReadWriteDelegateToTempLogHandle verifies Read and Write forward to the opened logfile handle.
func TestDeviceReadWriteDelegateToTempLogHandle(t *testing.T) {
	fs := &afero.Afero{Fs: afero.NewMemMapFs()}
	fh, err := fs.Create("temp.bin")
	if err != nil {
		t.Fatalf("create: %v", err)
	}
	if _, err := fh.Write([]byte("hello")); err != nil {
		t.Fatalf("seed write: %v", err)
	}
	if _, err := fh.Seek(0, io.SeekStart); err != nil {
		t.Fatalf("seek: %v", err)
	}

	dev := &Device{tempLogFileHandle: fh}
	buf := make([]byte, 5)
	n, err := dev.Read(buf)
	if err != nil {
		t.Fatalf("read: %v", err)
	}
	if string(buf[:n]) != "hello" {
		t.Fatalf("unexpected read data %q", string(buf[:n]))
	}

	n, err = dev.Write([]byte("!"))
	if err != nil {
		t.Fatalf("write: %v", err)
	}
	if n != 1 {
		t.Fatalf("unexpected write count %d", n)
	}
}

// hasArg reports whether the argument list contains the named option.
func hasArg(args []string, name string) bool {
	for _, arg := range args {
		if strings.EqualFold(arg, name) {
			return true
		}
	}
	return false
}
