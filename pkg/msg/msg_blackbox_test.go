// SPDX-License-Identifier: MIT

package msg_test

import (
	"errors"
	"strings"
	"testing"

	"github.com/rokath/trice/pkg/msg"
	"github.com/rokath/trice/pkg/tst"
)

// TestInfoOutput verifies the expected behavior.
func TestInfoOutput(t *testing.T) {
	out := tst.CaptureStdOut(func() {
		msg.Info("code issue")
	})
	if !strings.Contains(out, "code issue") {
		t.Fatalf("missing message in output: %q", out)
	}
	if !strings.Contains(out, "msg_blackbox_test.go") {
		t.Fatalf("missing file marker in output: %q", out)
	}
}

// TestOnErrOutput verifies the expected behavior.
func TestOnErrOutput(t *testing.T) {
	out := tst.CaptureStdOut(func() {
		msg.OnErr(nil)
		msg.OnErr(errors.New("s.th. went wrong"))
	})
	if !strings.Contains(out, "s.th. went wrong") {
		t.Fatalf("missing error string in output: %q", out)
	}
}

// TestInfoOnErrOutput verifies the expected behavior.
func TestInfoOnErrOutput(t *testing.T) {
	out := tst.CaptureStdOut(func() {
		msg.InfoOnErr(nil, "just in case")
		msg.InfoOnErr(errors.New("s.th. went wrong"), "just in case")
	})
	if !strings.Contains(out, "just in case") {
		t.Fatalf("missing info line in output: %q", out)
	}
	if !strings.Contains(out, "s.th. went wrong") {
		t.Fatalf("missing error line in output: %q", out)
	}
}

// TestOnTrueAndOnFalseOutput verifies the expected behavior.
func TestOnTrueAndOnFalseOutput(t *testing.T) {
	outTrue := tst.CaptureStdOut(func() {
		msg.OnTrue(false)
		msg.OnTrue(true)
	})
	if !strings.Contains(outTrue, "<nil>") {
		t.Fatalf("missing OnTrue marker in output: %q", outTrue)
	}

	outFalse := tst.CaptureStdOut(func() {
		msg.OnFalse(true)
		msg.OnFalse(false)
	})
	if !strings.Contains(outFalse, "<nil>") {
		t.Fatalf("missing OnFalse marker in output: %q", outFalse)
	}
}

// TestInfoOnTrueAndInfoOnFalseOutput verifies the expected behavior.
func TestInfoOnTrueAndInfoOnFalseOutput(t *testing.T) {
	outTrue := tst.CaptureStdOut(func() {
		msg.InfoOnTrue(false, "just in case")
		msg.InfoOnTrue(true, "just in case")
	})
	if !strings.Contains(outTrue, "just in case") {
		t.Fatalf("missing InfoOnTrue text in output: %q", outTrue)
	}

	outFalse := tst.CaptureStdOut(func() {
		msg.InfoOnFalse(true, "just in case")
		msg.InfoOnFalse(false, "just in case")
	})
	if !strings.Contains(outFalse, "just in case") {
		t.Fatalf("missing InfoOnFalse text in output: %q", outFalse)
	}
}

// TestFatalFunctionsNoopPaths verifies the expected behavior.
func TestFatalFunctionsNoopPaths(t *testing.T) {
	out := tst.CaptureStdOut(func() {
		msg.FatalOnErr(nil)
		msg.FatalInfoOnErr(nil, "just in case")
		msg.FatalOnTrue(false)
		msg.FatalInfoOnTrue(false, "just in case")
		msg.FatalOnFalse(true)
		msg.FatalInfoOnFalse(true, "just in case")
	})
	if out != "" {
		t.Fatalf("expected no output for noop fatal paths, got: %q", out)
	}
}
