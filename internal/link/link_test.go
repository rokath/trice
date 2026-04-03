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

// hasArg reports whether the argument list contains the named option.
func hasArg(args []string, name string) bool {
	for _, arg := range args {
		if strings.EqualFold(arg, name) {
			return true
		}
	}
	return false
}
