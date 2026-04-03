// SPDX-License-Identifier: MIT

package msg

import (
	"bytes"
	"errors"
	"strings"
	"testing"
)

// TestOnErrF verifies that OnErrF prints the provided error into the writer.
func TestOnErrF(t *testing.T) {
	var buf bytes.Buffer
	OnErrF(&buf, errors.New("boom"))
	if !strings.Contains(buf.String(), "boom") {
		t.Fatalf("expected error output, got %q", buf.String())
	}
}

// TestOnErrFv checks that the report returns the error and prints it when non-nil.
func TestOnErrFv(t *testing.T) {
	var buf bytes.Buffer
	err := OnErrFv(&buf, errors.New("boom"))
	if err == nil {
		t.Fatalf("expected error to be returned")
	}
	if !strings.Contains(buf.String(), "boom") {
		t.Fatalf("expected buffer to contain the message, got %q", buf.String())
	}
}

// TestOnTrue demonstrates log output when the flag is true.
func TestOnTrue(t *testing.T) {
	out := captureStdout(t, func() {
		OnTrue(true)
	})
	if !strings.Contains(out, "Error in") {
		t.Fatalf("expected output, got %q", out)
	}
}

// TestOnFalse demonstrates log output when the flag is false.
func TestOnFalse(t *testing.T) {
	out := captureStdout(t, func() {
		OnFalse(false)
	})
	if !strings.Contains(out, "Error in") {
		t.Fatalf("expected output, got %q", out)
	}
}

// TestInfoOnErr asserts the InfoOnErr helper prints both info and error.
func TestInfoOnErr(t *testing.T) {
	out := captureStdout(t, func() {
		InfoOnErr(errors.New("boom"), "info")
	})
	if !strings.Contains(out, "info") || !strings.Contains(out, "boom") {
		t.Fatalf("expected info and error in output, got %q", out)
	}
}
