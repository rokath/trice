// SPDX-License-Identifier: MIT

package tst

import (
	"io"
	"os"
	"strings"
	"testing"
)

// TestTempFileNameCreatesAndDeletes ensures TempFileName returns a valid filename and removes it afterwards.
func TestTempFileNameCreatesAndDeletes(t *testing.T) {
	name := TempFileName("ant-test-*.tmp")
	if name == "" {
		t.Fatal("expected a filename, got empty")
	}
	if _, err := os.Stat(name); !os.IsNotExist(err) {
		t.Fatalf("temp file should be removed, but exists: %v", err)
	}
}

// TestCaptureStdOutCapturesOutput verifies the helper intercepts stdout output cleanly.
func TestCaptureStdOutCapturesOutput(t *testing.T) {
	out := CaptureStdOut(func() {
		_, _ = io.WriteString(os.Stdout, "hello world")
	})
	if out != "hello world" {
		t.Fatalf("unexpected stdout captured: %q", out)
	}
}

// TestNormalizeMapStringStabilizesSpacing documents how NormalizeMapString
// standardizes spaces inside fmt.Sprint-ed map blocks so tests compare deterministically.
func TestNormalizeMapStringStabilizesSpacing(t *testing.T) {
	input := "{key1: { a  , b},   key2:{c,d}}"
	clean := NormalizeMapString(input)
	if strings.HasPrefix(clean, " ") || strings.HasSuffix(clean, " ") {
		t.Fatalf("normalized string has leading/trailing space: %q", clean)
	}
	if strings.Contains(clean, "  ") {
		t.Fatalf("normalized string contains double spaces: %q", clean)
	}
}
