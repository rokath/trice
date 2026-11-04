// SPDX-License-Identifier: MIT

// Package keybcmd is responsible for interpreting user commandline and executing commands
package keybcmd

import (
	"bufio"
	"context"
	"errors"
	"fmt"
	"io"
	"os"
	"strings"
	"sync"
	"time"

	"github.com/rokath/trice/internal/emitter"
)

// shutdownFn keeps the remote shutdown operation replaceable in focused tests.
var shutdownFn = emitter.ScShutdownRemoteDisplayServer

// loopAction processes one complete line, or the last nonempty line at EOF.
// Other read failures discard incomplete commands; writes are never retried
// because repeating a partially delivered command could execute it twice.
func loopAction(ctx context.Context, w io.Writer, reader *bufio.Reader, target io.Writer, ipa, ipp string) error {
	if err := ctx.Err(); err != nil {
		return err
	}
	fmt.Fprint(w, "-> ")
	text, readErr := reader.ReadString('\n')
	if err := ctx.Err(); err != nil {
		return err
	}
	if readErr != nil && (!errors.Is(readErr, io.EOF) || len(text) == 0) {
		return readErr
	}
	text = strings.Replace(text, "\r", "", -1) // Windows "\r\n" !
	text = strings.Replace(text, "\n", "", -1) // Linux "\n" !
	switch text {
	case "q", "quit":
		os.Exit(0)
	case "sd", "shutdown":
		err := shutdownFn(w, 1, ipa, ipp)
		if nil != err {
			fmt.Fprintln(w, err)
		}
	case "h", "help":
		fmt.Fprintln(w, "h|help      - this text")
		fmt.Fprintln(w, "sd|shutdown - kill trice display server (ends also this trice console)")
		fmt.Fprintln(w, "q|quit      - end trice console (keeps the trice display server active)")
		fmt.Fprintln(w, "Other commands are written to target.")
	default:
		fmt.Fprintf(w, "Command '%s'\n", text)
		b := append([]byte(text), 0)
		n, err := target.Write(b)
		if err != nil {
			return fmt.Errorf("write target command: %w", err)
		}
		if n != len(b) {
			return fmt.Errorf("write target command: %w", io.ErrShortWrite)
		}
	}
	return readErr
}

// keyboardSource owns an independently interruptible stdin handle. Interrupt
// wakes reads; release runs after the shell has stopped using the handle.
type keyboardSource struct {
	io.Reader
	interrupt func()
	release   func() error
}

// ReadInput starts the terminal shell. Its idempotent stop function interrupts
// pending input, calls interruptTarget to wake blocked writes, and joins the
// shell without closing the borrowed os.Stdin.
func ReadInput(target io.Writer, interruptTarget func()) (stop func(), err error) {
	source, err := openKeyboardSource(os.Stdin)
	if err != nil {
		return nil, fmt.Errorf("open keyboard input: %w", err)
	}
	return startKeyboardInput(os.Stdout, source, target, emitter.IPAddr, emitter.IPPort, interruptTarget), nil
}

// startKeyboardInput captures output and server settings before starting the
// goroutine so cleanup can safely restore process-wide test or CLI state.
func startKeyboardInput(w io.Writer, source keyboardSource, target io.Writer, ipa, ipp string, interruptTarget func()) func() {
	ctx, cancel := context.WithCancel(context.Background())
	done := make(chan struct{}) // Closing done joins every shell-side effect.
	go func() {
		defer close(done)
		err := keyboardInput(ctx, w, bufio.NewReader(source), target, ipa, ipp)
		if err != nil && !errors.Is(err, io.EOF) && ctx.Err() == nil {
			fmt.Fprintf(w, "Keyboard input stopped: %v\n", err)
		}
	}()
	var once sync.Once // Repeated stops must not release a reused OS handle.
	return func() {
		once.Do(func() {
			cancel()
			interruptTarget()
			// Cancellation may precede the OS read by a scheduling instant.
			// Keep interrupting until done proves no further read can start.
			ticker := time.NewTicker(10 * time.Millisecond)
			defer ticker.Stop()
			for {
				select {
				case <-done:
					if err := source.release(); err != nil {
						fmt.Fprintf(w, "Close keyboard input: %v\n", err)
					}
					return
				default:
					source.interrupt()
				}
				select {
				case <-done:
				case <-ticker.C:
				}
			}
		})
	}
}

// keyboardInput runs synchronously so every exit, including EOF and transport
// failure, can be observed and joined by the owner of the log run.
func keyboardInput(ctx context.Context, w io.Writer, reader *bufio.Reader, target io.Writer, ipa, ipp string) error {
	fmt.Fprintln(w, "Simple Shell (try 'help'):")
	fmt.Fprintln(w, "--------------------------")
	for {
		if err := loopAction(ctx, w, reader, target, ipa, ipp); err != nil {
			return err
		}
	}
}
