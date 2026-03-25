// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

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

func TestFatalInfoOnFalse(t *testing.T) {
	origLogFatalf := logFatalf
	// After this test, replace the original fatal function
	defer func() { logFatalf = origLogFatalf }()

	var e []string
	logFatalf = func(format string, args ...interface{}) {
		if len(args) > 0 {
			e = append(e, fmt.Sprintf(format, args))
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

func TestFmtFMessageHandlesMissingCallerInfo(t *testing.T) {
	var b bytes.Buffer
	fmtFMessage(&b, 0, "", 0, false, errors.New("x"))
	if !strings.Contains(b.String(), seriousError) {
		t.Fatalf("expected seriousError in output, got %q", b.String())
	}
}

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
