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
	"time"

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

// TestKeyboardInputStartsShellAndProcessesCommands verifies the async shell banner and forwarding behavior.
func TestKeyboardInputStartsShellAndProcessesCommands(t *testing.T) {
	reader := bufio.NewReader(strings.NewReader("hello\n"))
	target := &signalWriter{done: make(chan []byte, 1)}
	out := captureStdout(t, func() {
		keyboardInput(reader, target)
		_, _ = waitForWrite(t, target.done)
	})

	if !strings.Contains(out, "Simple Shell (try 'help'):") {
		t.Fatalf("missing shell banner: %q", out)
	}
}

// TestReadInputUsesOsStdin verifies ReadInput consumes commands from os.Stdin.
func TestReadInputUsesOsStdin(t *testing.T) {
	oldStdin := os.Stdin
	r, w, err := os.Pipe()
	if err != nil {
		t.Fatalf("pipe: %v", err)
	}
	os.Stdin = r
	t.Cleanup(func() {
		os.Stdin = oldStdin
		_ = r.Close()
		_ = w.Close()
	})

	var target bytes.Buffer
	signalled := &signalWriter{done: make(chan []byte, 1)}
	captureStdout(t, func() {
		ReadInput(signalled)
		_, _ = io.WriteString(w, "ping\n")
		got, err := waitForWrite(t, signalled.done)
		if err != nil {
			t.Fatal(err)
		}
		target.Write(got)
	})

	if !bytes.Equal(target.Bytes(), append([]byte("ping"), 0)) {
		t.Fatalf("ReadInput did not forward command from stdin: %q", target.Bytes())
	}
}

type signalWriter struct {
	done chan []byte
}

func (s *signalWriter) Write(p []byte) (int, error) {
	cp := append([]byte(nil), p...)
	select {
	case s.done <- cp:
	default:
	}
	return len(p), nil
}

func waitForWrite(t *testing.T, done <-chan []byte) ([]byte, error) {
	t.Helper()
	select {
	case got := <-done:
		return got, nil
	case <-time.After(200 * time.Millisecond):
		return nil, io.EOF
	}
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
