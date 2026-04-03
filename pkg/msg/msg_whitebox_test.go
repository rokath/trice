// SPDX-License-Identifier: MIT

package msg

import (
	"bytes"
	"errors"
	"fmt"
	"io"
	"os"
	"strings"
	"testing"
)

// TestFatalInfoOnFalse verifies the expected behavior.
func TestFatalInfoOnFalse(t *testing.T) {
	origLogFatalf := logFatalf
	// After this test, replace the original fatal function
	defer func() { logFatalf = origLogFatalf }()

	var e []string
	logFatalf = func(format string, args ...interface{}) {
		if len(args) > 0 {
			e = append(e, fmt.Sprintf(format, args...))
		} else {
			e = append(e, format)
		}
	}

	err := errors.New("wrong")

	FatalOnErr(nil)
	if len(e) != 0 {
		t.Errorf("excepted no error, actual %v", len(e))
	}
	FatalOnErr(err)
	if len(e) != 1 {
		t.Errorf("excepted one error, actual %v", len(e))
	}
	e = e[:0] // clear

	FatalInfoOnErr(nil, "just in case")
	if len(e) != 0 {
		t.Errorf("excepted no error, actual %v", len(e))
	}
	FatalInfoOnErr(err, "just in case")
	if len(e) != 1 {
		t.Errorf("excepted one error, actual %v", len(e))
	}
	e = e[:0] // clear

	FatalOnTrue(false)
	if len(e) != 0 {
		t.Errorf("excepted no error, actual %v", len(e))
	}
	FatalOnTrue(true)
	if len(e) != 1 {
		t.Errorf("excepted one error, actual %v", len(e))
	}
	e = e[:0] // clear

	FatalOnFalse(true)
	if len(e) != 0 {
		t.Errorf("excepted no error, actual %v", len(e))
	}
	FatalOnFalse(false)
	if len(e) != 1 {
		t.Errorf("excepted one error, actual %v", len(e))
	}
	e = e[:0] // clear

	FatalInfoOnTrue(false, "its is true")
	if len(e) != 0 {
		t.Errorf("excepted no error, actual %v", len(e))
	}
	FatalInfoOnTrue(true, "its is true")
	if len(e) != 1 {
		t.Errorf("excepted one error, actual %v", len(e))
	}
	e = e[:0] // clear

	FatalInfoOnFalse(true, "info")
	if len(e) != 0 {
		t.Errorf("excepted no error, actual %v", len(e))
	}
	FatalInfoOnFalse(false, "info")
	if len(e) != 1 {
		t.Errorf("excepted one error, actual %v", len(e))
	}
	e = e[:0] // clear

}

// captureStdout captures stdout produced while the supplied function runs.
func captureStdout(tb testing.TB, f func()) string {
	tb.Helper()
	old := os.Stdout
	r, w, err := os.Pipe()
	if err != nil {
		tb.Fatalf("pipe failed: %v", err)
	}
	os.Stdout = w
	done := make(chan string, 1)
	go func() {
		var b bytes.Buffer
		_, _ = io.Copy(&b, r)
		done <- b.String()
	}()
	f()
	_ = w.Close()
	os.Stdout = old
	return <-done
}

// TestFmtFMessageHandlesMissingCallerInfo verifies the expected behavior.
func TestFmtFMessageHandlesMissingCallerInfo(t *testing.T) {
	var b bytes.Buffer
	fmtFMessage(&b, 0, "", 0, false, errors.New("x"))
	if !strings.Contains(b.String(), seriousError) {
		t.Fatalf("expected seriousError in output, got %q", b.String())
	}
}

// TestLogMessageHandlesMissingCallerInfo verifies the expected behavior.
func TestLogMessageHandlesMissingCallerInfo(t *testing.T) {
	origLogFatalf := logFatalf
	defer func() { logFatalf = origLogFatalf }()

	var got string
	logFatalf = func(format string, args ...interface{}) {
		got = fmt.Sprintf(format, args...)
	}

	logMessage(0, "", 0, false, errors.New("x"))
	if !strings.Contains(got, seriousError) {
		t.Fatalf("expected seriousError in log output, got %q", got)
	}
}

// TestOsExitDisallowFormatsVarArgs verifies the expected behavior.
func TestOsExitDisallowFormatsVarArgs(t *testing.T) {
	o := OsExitDisallow()
	defer OsExitAllow(o)

	out := captureStdout(t, func() {
		logFatalf("value=%d", 7)
	})
	if !strings.Contains(out, "value=7") {
		t.Fatalf("expected formatted output, got %q", out)
	}
}

// TestOsExitDisallowFormatsWithoutArgs verifies the expected behavior.
func TestOsExitDisallowFormatsWithoutArgs(t *testing.T) {
	o := OsExitDisallow()
	defer OsExitAllow(o)

	out := captureStdout(t, func() {
		logFatalf("plain")
	})
	if !strings.Contains(out, "plain") {
		t.Fatalf("expected plain output, got %q", out)
	}
}

// TestOnErrFAndOnErrFv verifies the expected behavior.
func TestOnErrFAndOnErrFv(t *testing.T) {
	var b bytes.Buffer
	OnErrF(&b, nil)
	if b.Len() != 0 {
		t.Fatalf("expected no output for nil error, got %q", b.String())
	}

	err := errors.New("boom")
	OnErrF(&b, err)
	if !strings.Contains(b.String(), "boom") {
		t.Fatalf("expected error output, got %q", b.String())
	}

	b.Reset()
	got := OnErrFv(&b, err)
	if got == nil || got.Error() != "boom" {
		t.Fatalf("expected returned error boom, got %v", got)
	}
	if !strings.Contains(b.String(), "boom") {
		t.Fatalf("expected error output, got %q", b.String())
	}
}

// TestTellRespectsVerbose verifies the expected behavior.
func TestTellRespectsVerbose(t *testing.T) {
	var b bytes.Buffer
	Verbose = false
	Tell(&b, "hidden")
	if b.Len() != 0 {
		t.Fatalf("expected no output when verbose is false, got %q", b.String())
	}
	Verbose = true
	Tell(&b, "shown")
	if !strings.Contains(b.String(), "shown") {
		t.Fatalf("expected output when verbose is true, got %q", b.String())
	}
	Verbose = false
}
