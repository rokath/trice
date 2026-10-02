// SPDX-License-Identifier: MIT

package args

import (
	"bytes"
	"errors"
	"os"
	"testing"

	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
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
