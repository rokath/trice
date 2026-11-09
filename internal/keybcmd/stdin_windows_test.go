// SPDX-License-Identifier: MIT

package keybcmd

import (
	"bytes"
	"fmt"
	"io"
	"os"
	"sync/atomic"
	"testing"
	"time"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
	"golang.org/x/sys/windows"
)

// keyboardPipeSequence keeps concurrent test processes and subtests from
// connecting to another case's named pipe.
var keyboardPipeSequence atomic.Uint64

// overlappedKeyboardPipe supplies a real asynchronous Windows stdin handle.
// The client connects before ConnectNamedPipe so setup needs no pending I/O.
func overlappedKeyboardPipe(t *testing.T) (*os.File, *os.File) {
	t.Helper()
	name := fmt.Sprintf(`\\.\pipe\trice-keyboard-%d-%d`, os.Getpid(), keyboardPipeSequence.Add(1))
	path, err := windows.UTF16PtrFromString(name)
	require.NoError(t, err)
	handle, err := windows.CreateNamedPipe(path, windows.PIPE_ACCESS_INBOUND|windows.FILE_FLAG_OVERLAPPED, windows.PIPE_TYPE_BYTE|windows.PIPE_WAIT, 1, 4096, 4096, 0, nil)
	require.NoError(t, err)
	input := os.NewFile(uintptr(handle), name)
	t.Cleanup(func() { _ = input.Close() })
	client, err := windows.CreateFile(path, windows.GENERIC_WRITE, 0, nil, windows.OPEN_EXISTING, 0, 0)
	require.NoError(t, err)
	writer := os.NewFile(uintptr(client), name)
	t.Cleanup(func() { _ = writer.Close() })
	require.ErrorIs(t, windows.ConnectNamedPipe(handle, nil), windows.ERROR_PIPE_CONNECTED)
	return input, writer
}

// TestWindowsKeyboardInputPipeModes checks cancellation before/after a read,
// command forwarding, EOF, and preservation of the borrowed stdin handle.
// Both synchronous and overlapped pipes are supported without user input.
func TestWindowsKeyboardInputPipeModes(t *testing.T) {
	for _, overlapped := range []bool{false, true} {
		for _, scenario := range []string{"immediate stop", "pending read", "command then stop", "EOF then stop"} {
			t.Run(fmt.Sprintf("overlapped=%t/%s", overlapped, scenario), func(t *testing.T) {
				limitKeyboardTest(t)
				var input, writer *os.File
				if overlapped {
					input, writer = overlappedKeyboardPipe(t)
				} else {
					var err error
					input, writer, err = os.Pipe()
					require.NoError(t, err)
					t.Cleanup(func() { _ = input.Close(); _ = writer.Close() })
				}
				source, err := openKeyboardSource(input)
				require.NoError(t, err)
				observed := &observedReader{Reader: source.Reader, entered: make(chan struct{})}
				source.Reader = observed
				var output bytes.Buffer
				target := &signalWriter{done: make(chan []byte, 1)}
				stop := startKeyboardInput(&output, source, target, "ipa", "ipp", func() {})
				defer stop()
				if scenario != "immediate stop" {
					select {
					case <-observed.entered:
					case <-time.After(5 * time.Second):
						t.Fatal("shell did not enter stdin read")
					}
				}
				if scenario == "command then stop" {
					_, err := io.WriteString(writer, "ping\n")
					require.NoError(t, err)
					command, err := waitForWrite(t, target.done)
					require.NoError(t, err)
					assert.Equal(t, []byte("ping\x00"), command)
				}
				if scenario == "EOF then stop" {
					require.NoError(t, writer.Close())
				}
				stop()
				stop()
				assert.NotContains(t, output.String(), "Keyboard input stopped:")
				if scenario != "EOF then stop" {
					_, err := io.WriteString(writer, "still open\n")
					require.NoError(t, err)
					remaining := make([]byte, len("still open\n"))
					_, err = io.ReadFull(input, remaining)
					require.NoError(t, err)
					assert.Equal(t, "still open\n", string(remaining))
				}
			})
		}
	}
}
