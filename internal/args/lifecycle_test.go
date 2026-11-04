// SPDX-License-Identifier: MIT

package args

import (
	"bytes"
	"errors"
	"io"
	"net"
	"os"
	"strconv"
	"testing"
	"time"

	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// lifecycleFS records actual file ownership below both receiver wrappers.
// Lookup files use the same FS but assertions address only input and binary log.
type lifecycleFS struct {
	afero.Fs
	opened    map[string][]*lifecycleFile
	failClose string
	failIO    string
}

// lifecycleFile delegates file operations and counts every attempted Close.
type lifecycleFile struct {
	afero.File
	closes int
	err    error
	ioErr  error
}

// Read and Write inject failures at the actual input/logfile boundary.
func (f *lifecycleFile) Read(b []byte) (int, error) {
	if f.ioErr != nil {
		return 0, f.ioErr
	}
	return f.File.Read(b)
}

func (f *lifecycleFile) Write(b []byte) (int, error) {
	if f.ioErr != nil {
		return 0, f.ioErr
	}
	return f.File.Write(b)
}

func (f *lifecycleFile) Close() error {
	f.closes++
	return errors.Join(f.File.Close(), f.err)
}

func (fs *lifecycleFS) Open(name string) (afero.File, error) {
	f, err := fs.Fs.Open(name)
	return fs.record(name, f, err)
}

func (fs *lifecycleFS) OpenFile(name string, flag int, perm os.FileMode) (afero.File, error) {
	f, err := fs.Fs.OpenFile(name, flag, perm)
	return fs.record(name, f, err)
}

// record assigns an optional close failure without skipping the real close.
func (fs *lifecycleFS) record(name string, f afero.File, err error) (afero.File, error) {
	if err != nil {
		return f, err
	}
	wrapped := &lifecycleFile{File: f}
	if name == fs.failClose {
		wrapped.err = errors.New("simulated close failure")
	}
	if name == fs.failIO {
		wrapped.ioErr = errors.New("simulated IO failure")
	}
	fs.opened[name] = append(fs.opened[name], wrapped)
	return wrapped, nil
}

// TestCLIReplayClosesInputAndBinaryLogWithDebugWrapper exercises the real
// command path. Both files close once on EOF, read/write failure and close error.
func TestCLIReplayClosesInputAndBinaryLogWithDebugWrapper(t *testing.T) {
	for _, tc := range []struct{ name, failClose, failIO, problem string }{
		{"successful_EOF", "", "", ""},
		{"input_close_failure", "capture.bin", "", "simulated close failure"},
		{"logfile_close_failure", "recorded.bin", "", "simulated close failure"},
		{"input_read_failure", "", "capture.bin", "simulated IO failure"},
		{"logfile_write_failure", "", "recorded.bin", "simulated IO failure"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			FlagsInit()
			t.Cleanup(FlagsInit)
			base := afero.NewMemMapFs()
			assert.NoError(t, afero.WriteFile(base, "capture.bin", []byte("hello\n"), 0o600))
			fs := &lifecycleFS{Fs: base, opened: make(map[string][]*lifecycleFile), failClose: tc.failClose, failIO: tc.failIO}
			var output bytes.Buffer
			err := Handler(&output, &afero.Afero{Fs: fs}, []string{"trice", "log", "-p", "FILEBUFFER", "-args", "capture.bin", "-encoding", "CHAR", "-i", "emptyFile", "-li", "off", "-hs", "off", "-ts", "off", "-prefix", "", "-suffix", "", "-color", "none", "-showInputBytes", "-binaryLogfile", "recorded.bin"})
			if tc.problem == "" {
				assert.NoError(t, err)
			} else {
				assert.ErrorContains(t, err, tc.problem)
			}
			for _, name := range []string{"capture.bin", "recorded.bin"} {
				if assert.Len(t, fs.opened[name], 1, name) {
					assert.Equal(t, 1, fs.opened[name][0].closes, name)
				}
			}
			data, readErr := afero.ReadFile(base, "recorded.bin")
			assert.NoError(t, readErr)
			if tc.failIO == "" {
				assert.Equal(t, "hello\n", string(data))
				assert.Contains(t, output.String(), "Input(68 65 6c 6c 6f 0a)")
				assert.Contains(t, output.String(), "hello\n")
			} else {
				assert.Empty(t, data, "failed input or logfile writes cannot produce a complete recording")
			}
		})
	}
}

// TestRemoteDisplayJoinsKeyboardBeforeReturning uses an empty replay and a
// local display connection with no event RPCs. EOF and keyboard-start failure
// close the transport chain once and leave no shell consuming borrowed stdin.
func TestRemoteDisplayJoinsKeyboardBeforeReturning(t *testing.T) {
	for _, mode := range []string{"plain", "diagnostic wrappers", "closed stdin"} {
		t.Run(mode, func(t *testing.T) {
			FlagsInit()
			t.Cleanup(FlagsInit)
			input, writer, err := os.Pipe()
			require.NoError(t, err)
			defer input.Close()
			defer writer.Close()
			originalInput, originalOutput := os.Stdin, os.Stdout
			output, err := os.OpenFile(os.DevNull, os.O_WRONLY, 0)
			require.NoError(t, err)
			os.Stdin, os.Stdout = input, output
			defer func() { os.Stdin, os.Stdout = originalInput, originalOutput; _ = output.Close() }()
			if mode == "closed stdin" {
				require.NoError(t, input.Close())
			}
			base := afero.NewMemMapFs()
			require.NoError(t, afero.WriteFile(base, "empty.bin", nil, 0o600))
			fs := &lifecycleFS{Fs: base, opened: make(map[string][]*lifecycleFile)}
			// Dial can complete through the local accept queue. Accept after
			// Handler returns so the fixture needs no extra server goroutine.
			listener, err := net.ListenTCP("tcp4", &net.TCPAddr{IP: net.IPv4(127, 0, 0, 1)})
			require.NoError(t, err)
			defer listener.Close()
			require.NoError(t, listener.SetDeadline(time.Now().Add(5*time.Second)))
			port := strconv.Itoa(listener.Addr().(*net.TCPAddr).Port)
			arguments := []string{"trice", "log", "-p", "FILEBUFFER", "-args", "empty.bin", "-encoding", "CHAR", "-i", "emptyFile", "-li", "off", "-ds", "-ipa", "127.0.0.1", "-ipp", port}
			if mode == "diagnostic wrappers" {
				arguments = append(arguments, "-showInputBytes", "-binaryLogfile", "recorded.bin")
			}
			err = Handler(io.Discard, &afero.Afero{Fs: fs}, arguments)
			peer, acceptErr := listener.AcceptTCP()
			require.NoError(t, acceptErr)
			require.NoError(t, peer.Close())
			if mode == "closed stdin" {
				assert.ErrorContains(t, err, "open keyboard input")
			} else {
				require.NoError(t, err)
				// A surviving shell would race this read and consume the data.
				_, err = io.WriteString(writer, "still open\n")
				require.NoError(t, err)
				remaining := make([]byte, len("still open\n"))
				_, err = io.ReadFull(input, remaining)
				require.NoError(t, err)
				assert.Equal(t, "still open\n", string(remaining))
			}
			for name, opened := range fs.opened {
				require.Len(t, opened, 1, name)
				assert.Equal(t, 1, opened[0].closes, name)
			}
		})
	}
}
