// SPDX-License-Identifier: MIT

package keybcmd

import (
	"bufio"
	"bytes"
	"io"
	"os"
	"strings"
	"sync"
	"testing"

	"github.com/rokath/trice/internal/emitter"
)

// TestLoopActionDefault ensures unknown commands are forwarded to target with termination byte.
func TestLoopActionDefault(t *testing.T) {
	input := bufio.NewReader(strings.NewReader("foo\n"))
	var target bytes.Buffer

	out := captureStdout(t, func() {
		loopAction(input, &target, "ipa", "ipp")
	})

	if !strings.Contains(out, "Command 'foo'") {
		t.Fatalf("stdout missing command message: %q", out)
	}
	expected := append([]byte("foo"), 0)
	if !bytes.Equal(target.Bytes(), expected) {
		t.Fatalf("target mismatch: got %q", target.Bytes())
	}
}

// TestLoopActionHelp documents that help prints the usage guide without writing to the target.
func TestLoopActionHelp(t *testing.T) {
	input := bufio.NewReader(strings.NewReader("help\n"))
	var target bytes.Buffer

	out := captureStdout(t, func() {
		loopAction(input, &target, "ipa", "ipp")
	})

	if !strings.Contains(out, "h|help      - this text") {
		t.Fatalf("help not printed: %q", out)
	}
	if target.Len() != 0 {
		t.Fatalf("target should stay empty for help input")
	}
}

// TestLoopActionShutdown confirms the shutdown hook is invoked when the user issues the shutdown command.
func TestLoopActionShutdown(t *testing.T) {
	input := bufio.NewReader(strings.NewReader("sd\n"))
	var target bytes.Buffer

	called := false
	mu := sync.Mutex{}
	shutdownFn = func(io.Writer, int64, ...string) error {
		mu.Lock()
		called = true
		mu.Unlock()
		return nil
	}
	defer func() {
		shutdownFn = emitter.ScShutdownRemoteDisplayServer
	}()

	captureStdout(t, func() {
		loopAction(input, &target, "ipa", "ipp")
	})

	mu.Lock()
	if !called {
		t.Fatalf("shutdownFn was not invoked")
	}
	mu.Unlock()
}

// captureStdout captures stdout produced while the supplied function runs.
func captureStdout(t *testing.T, fn func()) string {
	t.Helper()
	old := os.Stdout
	r, w, err := os.Pipe()
	if err != nil {
		t.Fatalf("pipe: %v", err)
	}
	os.Stdout = w

	var buf bytes.Buffer
	done := make(chan struct{})
	go func() {
		_, _ = io.Copy(&buf, r)
		close(done)
	}()

	fn()

	_ = w.Close()
	<-done
	os.Stdout = old
	return buf.String()
}
