// SPDX-License-Identifier: MIT

package keybcmd

import (
	"bufio"
	"bytes"
	"context"
	"errors"
	"io"
	"os"
	"runtime/debug"
	"strings"
	"sync"
	"testing"
	"testing/iotest"
	"time"

	"github.com/rokath/trice/internal/emitter"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// TestLoopActionDefault ensures unknown commands are forwarded to target with termination byte.
func TestLoopActionDefault(t *testing.T) {
	input := bufio.NewReader(strings.NewReader("foo\n"))
	var target bytes.Buffer

	out := captureStdout(t, func() {
		assert.NoError(t, loopAction(context.Background(), os.Stdout, input, &target, "ipa", "ipp"))
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
		assert.NoError(t, loopAction(context.Background(), os.Stdout, input, &target, "ipa", "ipp"))
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
		assert.NoError(t, loopAction(context.Background(), os.Stdout, input, &target, "ipa", "ipp"))
	})

	mu.Lock()
	if !called {
		t.Fatalf("shutdownFn was not invoked")
	}
	mu.Unlock()
}

// TestKeyboardInputStartsShellAndProcessesCommands verifies the async shell banner and forwarding behavior.
func TestKeyboardInputStartsShellAndProcessesCommands(t *testing.T) {
	limitKeyboardTest(t)
	target := &signalWriter{done: make(chan []byte, 1)}
	out := captureStdout(t, func() {
		source := keyboardSource{Reader: strings.NewReader("hello\n"), interrupt: func() {}, release: func() error { return nil }}
		stop := startKeyboardInput(os.Stdout, source, target, "ipa", "ipp", func() {})
		defer stop()
		got, err := waitForWrite(t, target.done)
		require.NoError(t, err)
		assert.Equal(t, []byte("hello\x00"), got)
	})

	if !strings.Contains(out, "Simple Shell (try 'help'):") {
		t.Fatalf("missing shell banner: %q", out)
	}
}

// TestReadInputUsesOsStdin verifies ReadInput consumes commands from os.Stdin.
func TestReadInputUsesOsStdin(t *testing.T) {
	limitKeyboardTest(t)
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
		stop, err := ReadInput(signalled, func() {})
		require.NoError(t, err)
		defer stop()
		_, _ = io.WriteString(w, "ping\n")
		got, err := waitForWrite(t, signalled.done)
		if err != nil {
			t.Fatal(err)
		}
		target.Write(got)
		stop()
		stop() // Cleanup is idempotent and leaves the original stdin usable.
		_, err = io.WriteString(w, "still open\n")
		require.NoError(t, err)
		remaining := make([]byte, len("still open\n"))
		_, err = io.ReadFull(r, remaining)
		require.NoError(t, err)
		assert.Equal(t, "still open\n", string(remaining))
	})

	if !bytes.Equal(target.Bytes(), append([]byte("ping"), 0)) {
		t.Fatalf("ReadInput did not forward command from stdin: %q", target.Bytes())
	}
}

// limitKeyboardTest includes deferred shutdown and cleanup in a short budget.
// A panic prints every goroutine even when a blocking Windows cancellation
// prevents an ordinary assertion or cleanup from returning to the test runner.
func limitKeyboardTest(t *testing.T) {
	t.Helper()
	watchdog := time.AfterFunc(15*time.Second, func() {
		debug.SetTraceback("all")
		panic("keyboard input test " + t.Name() + " timed out after 15s")
	})
	t.Cleanup(func() { watchdog.Stop() })
}

// signalWriter hands copied commands to the test without shared buffer access.
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

// waitForWrite bounds failures without using sleeps to synchronize the shell.
func waitForWrite(t *testing.T, done <-chan []byte) ([]byte, error) {
	t.Helper()
	select {
	case got := <-done:
		return got, nil
	case <-time.After(5 * time.Second):
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
	defer func() { os.Stdout = old; _ = r.Close() }()

	var buf bytes.Buffer
	done := make(chan struct{})
	go func() {
		_, _ = io.Copy(&buf, r)
		close(done)
	}()

	func() {
		defer func() { _ = w.Close(); <-done }()
		fn()
	}()
	return buf.String()
}

// TestKeyboardInputReadBoundaries distinguishes an intentional blank command
// from absent input, and never forwards a partial line after a read failure.
func TestKeyboardInputReadBoundaries(t *testing.T) {
	readErr := errors.New("keyboard disconnected")
	for _, tc := range []struct {
		name, input, want string
		failure           error
	}{
		{name: "empty EOF"},
		{name: "last line without newline", input: "last", want: "last\x00"},
		{name: "intentional empty line", input: "\n", want: "\x00"},
		{name: "Windows empty line", input: "\r\n", want: "\x00"},
		{name: "Windows command", input: "hi\r\n", want: "hi\x00"},
		{name: "multiple lines and final EOF", input: "one\n\ntwo", want: "one\x00\x00two\x00"},
		{name: "read failure without data", failure: readErr},
		{name: "partial command at read failure", input: "incomplete", failure: readErr},
		{name: "complete command before read failure", input: "ok\npartial", want: "ok\x00", failure: readErr},
	} {
		t.Run(tc.name, func(t *testing.T) {
			var input io.Reader = strings.NewReader(tc.input)
			wantErr := error(io.EOF)
			if tc.failure != nil {
				input = io.MultiReader(input, iotest.ErrReader(tc.failure))
				wantErr = tc.failure
			}
			var target, output bytes.Buffer
			err := keyboardInput(context.Background(), &output, bufio.NewReader(input), &target, "ipa", "ipp")
			assert.ErrorIs(t, err, wantErr)
			assert.Equal(t, tc.want, target.String())
			assert.Contains(t, output.String(), "Simple Shell (try 'help'):")
		})
	}
}

// commandResultWriter counts attempts so write failures cannot silently consume
// later commands or cause an unsafe retry of already delivered bytes.
type commandResultWriter struct {
	count    int
	err      error
	attempts int
	command  []byte
}

func (w *commandResultWriter) Write(b []byte) (int, error) {
	w.attempts++
	w.command = append([]byte(nil), b...)
	return w.count, w.err
}

// TestKeyboardInputStopsOnWriteFailure checks errors and short writes, including
// the case where some command bytes have already reached the transport.
func TestKeyboardInputStopsOnWriteFailure(t *testing.T) {
	writeErr := errors.New("target disconnected")
	for _, tc := range []struct {
		name  string
		count int
		err   error
		want  error
	}{
		{"failed write", 0, writeErr, writeErr},
		{"partial failed write", 1, writeErr, writeErr},
		{"short write", 1, nil, io.ErrShortWrite},
		{"empty write", 0, nil, io.ErrShortWrite},
	} {
		t.Run(tc.name, func(t *testing.T) {
			target := &commandResultWriter{count: tc.count, err: tc.err}
			err := keyboardInput(context.Background(), io.Discard, bufio.NewReader(strings.NewReader("hi\nnext\n")), target, "ipa", "ipp")
			assert.ErrorIs(t, err, tc.want)
			assert.Equal(t, 1, target.attempts)
			assert.Equal(t, []byte("hi\x00"), target.command)
		})
	}
}

// observedReader announces entry to a real OS read, allowing stop to race with
// the beginning of that read without timing assumptions or leaked goroutines.
type observedReader struct {
	io.Reader
	entered chan struct{}
	once    sync.Once
}

func (r *observedReader) Read(b []byte) (int, error) {
	r.once.Do(func() { close(r.entered) })
	return r.Reader.Read(b)
}

// TestKeyboardInputStopInterruptsPendingStdinRead exercises the native stdin
// adapter repeatedly and joins before inspecting output or restoring globals.
func TestKeyboardInputStopInterruptsPendingStdinRead(t *testing.T) {
	limitKeyboardTest(t)
	for range 20 {
		input, writer, err := os.Pipe()
		require.NoError(t, err)
		source, err := openKeyboardSource(input)
		require.NoError(t, err)
		observed := &observedReader{Reader: source.Reader, entered: make(chan struct{})}
		source.Reader = observed
		var output, target bytes.Buffer
		stop := startKeyboardInput(&output, source, &target, "ipa", "ipp", func() {})
		select {
		case <-observed.entered:
		case <-time.After(5 * time.Second):
			t.Error("shell did not enter stdin read")
		}
		stop()
		assert.Empty(t, target.String())
		assert.NotContains(t, output.String(), "Keyboard input stopped:")
		assert.NoError(t, input.Close())
		assert.NoError(t, writer.Close())
	}
}

// observedWriter announces a pending pipe write; closing its peer is the same
// shutdown action used by the logger to unblock a stalled target command.
type observedWriter struct {
	io.Writer
	entered chan struct{}
}

func (w observedWriter) Write(b []byte) (int, error) {
	close(w.entered)
	return w.Writer.Write(b)
}

// TestKeyboardInputStopInterruptsPendingWrite proves shutdown also joins when
// the target has stopped consuming commands, not just while stdin is idle.
func TestKeyboardInputStopInterruptsPendingWrite(t *testing.T) {
	limitKeyboardTest(t)
	reader, writer := io.Pipe()
	defer reader.Close()
	defer writer.Close()
	target := observedWriter{Writer: writer, entered: make(chan struct{})}
	source := keyboardSource{Reader: strings.NewReader("hi\n"), interrupt: func() {}, release: func() error { return nil }}
	var output bytes.Buffer
	stop := startKeyboardInput(&output, source, target, "ipa", "ipp", func() { _ = reader.Close() })
	defer stop()
	select {
	case <-target.entered:
	case <-time.After(5 * time.Second):
		t.Fatal("shell did not enter target write")
	}
	stop()
	assert.NotContains(t, output.String(), "Keyboard input stopped:")
}

// TestReadInputReportsClosedStdin verifies startup errors are returned before
// a shell goroutine is created, allowing the logger to close its transport.
func TestReadInputReportsClosedStdin(t *testing.T) {
	input, writer, err := os.Pipe()
	require.NoError(t, err)
	require.NoError(t, input.Close())
	defer writer.Close()
	original := os.Stdin
	os.Stdin = input
	defer func() { os.Stdin = original }()
	stop, err := ReadInput(io.Discard, func() {})
	if stop != nil {
		defer stop()
	}
	assert.ErrorContains(t, err, "open keyboard input")
	assert.Nil(t, stop)
}
