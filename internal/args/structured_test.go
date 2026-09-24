// SPDX-License-Identifier: MIT

package args

import (
	"bytes"
	"encoding/binary"
	"io"
	"math"
	"os"
	"testing"

	"github.com/rokath/trice/internal/decoder"
	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
)

// TestStructuredCLIValidationPrecedesIO checks both public logging entry points.
// Rejected options must not open the input channel or create the requested file.
func TestStructuredCLIValidationPrecedesIO(t *testing.T) {
	oldStart := startLogLoop
	t.Cleanup(func() { startLogLoop = oldStart; FlagsInit() })
	for _, tt := range []struct {
		name      string
		options   []string
		errorText string
	}{
		{"unknown format", []string{"-logFormat", "yaml"}, "expected text, json, or kv"},
		{"CHAR has no events", []string{"-logFormat", "json", "-encoding", "CHAR"}, "requires -encoding TREX"},
		{"DUMP has no events", []string{"-logFormat", "kv", "-encoding", "DUMP"}, "requires -encoding TREX"},
	} {
		for _, entry := range []string{"trice", "tlog"} {
			t.Run(entry+"/"+tt.name, func(t *testing.T) {
				FlagsInit()
				fs := &afero.Afero{Fs: afero.NewMemMapFs()}
				started := false
				startLogLoop = func(io.Writer, *afero.Afero) error { started = true; return nil }
				options := append(append([]string{}, tt.options...), "-logfile", "must-not-exist.log")
				var err error
				if entry == "trice" {
					err = Handler(io.Discard, fs, append([]string{"trice", "log"}, options...))
				} else {
					err = LogHandler(io.Discard, fs, append([]string{"tlog"}, options...))
				}
				assert.ErrorContains(t, err, tt.errorText)
				assert.False(t, started)
				exists, statErr := fs.Exists("must-not-exist.log")
				assert.NoError(t, statErr)
				assert.False(t, exists)
			})
		}
	}
}

// TestStructuredCLIFormatResetsAndReturnsSinkFailures verifies command-local
// format state and a non-zero error reaching the CLI after a machine sink fails.
func TestStructuredCLIFormatResetsAndReturnsSinkFailures(t *testing.T) {
	oldStart := startLogLoop
	t.Cleanup(func() { startLogLoop = oldStart; FlagsInit() })
	FlagsInit()
	fs := &afero.Afero{Fs: afero.NewMemMapFs()}
	var formats []string
	startLogLoop = func(io.Writer, *afero.Afero) error {
		formats = append(formats, decoder.LogFormat)
		return io.ErrClosedPipe
	}
	assert.ErrorIs(t, LogHandler(io.Discard, fs, []string{"tlog", "-logFormat", "json"}), io.ErrClosedPipe)
	assert.ErrorIs(t, LogHandler(io.Discard, fs, []string{"tlog"}), io.ErrClosedPipe)
	assert.Equal(t, []string{"json", "text"}, formats)
}

// TestStructuredCLIReplayKeepsMachineSinksClean runs the public command with a
// real wire packet, dictionary loading, verbose diagnostics, binary recording,
// and formatted logfile output. All fixtures stay in an isolated filesystem.
func TestStructuredCLIReplayKeepsMachineSinksClean(t *testing.T) {
	for _, tt := range []struct{ format, want string }{
		{"json", `{"tag":"info","level":"INFO","message":"Motor 3: 87.5 C","fields":{"motor_id":3,"temperature_c":87.5}}` + "\n"},
		{"kv", "tag=info level=INFO message=\"Motor 3: 87.5 C\" field.motor_id=3 field.temperature_c=87.5\n"},
	} {
		t.Run(tt.format, func(t *testing.T) {
			FlagsInit()
			t.Cleanup(FlagsInit)
			fs := &afero.Afero{Fs: afero.NewMemMapFs()}
			assert.NoError(t, fs.WriteFile("til.json", []byte(`{"100":{"Type":"TRICE32_2","Strg":"info:Motor {motor_id}: {temperature_c:%.1f C}"}}`), 0o644))
			packet := binary.LittleEndian.AppendUint16(nil, 0x4064)
			packet = binary.LittleEndian.AppendUint16(packet, 0x08c0)
			packet = binary.LittleEndian.AppendUint32(packet, 3)
			packet = binary.LittleEndian.AppendUint32(packet, math.Float32bits(87.5))
			assert.NoError(t, fs.WriteFile("capture.bin", packet, 0o644))
			stdoutReader, stdoutWriter, err := os.Pipe()
			if !assert.NoError(t, err) {
				return
			}
			defer stdoutReader.Close()
			stderrReader, stderrWriter, err := os.Pipe()
			if !assert.NoError(t, err) {
				stdoutWriter.Close()
				return
			}
			defer stderrReader.Close()
			originalOut, originalErr := os.Stdout, os.Stderr
			// The output is small enough for the pipes; restore process streams
			// before assertions so testing diagnostics remain visible on failure.
			defer func() { os.Stdout, os.Stderr = originalOut, originalErr }()
			os.Stdout, os.Stderr = stdoutWriter, stderrWriter
			var application bytes.Buffer
			err = Handler(&application, fs, []string{"trice", "log", "-p", "FILEBUFFER", "-args", "capture.bin", "-pf", "none", "-til", "til.json", "-li", "off", "-hs", "off", "-ts", "off", "-logFormat", tt.format, "-v", "-showInputBytes", "-logfile", "output.log", "-binaryLogfile", "raw.bin", "-addNL", "-prefix", "ignored-prefix", "-suffix", "ignored-suffix"})
			os.Stdout, os.Stderr = originalOut, originalErr
			assert.NoError(t, stdoutWriter.Close())
			assert.NoError(t, stderrWriter.Close())
			strayOutput, stdoutErr := io.ReadAll(stdoutReader)
			diagnostics, stderrErr := io.ReadAll(stderrReader)
			assert.NoError(t, err)
			assert.NoError(t, stdoutErr)
			assert.NoError(t, stderrErr)
			assert.Empty(t, string(strayOutput), "no setup or verbose messages may bypass the machine sink")
			assert.Contains(t, string(diagnostics), "Read ID List file til.json")
			assert.Contains(t, string(diagnostics), "Encoding is TREX")
			assert.Equal(t, tt.want, application.String())
			logfile, err := fs.ReadFile("output.log")
			assert.NoError(t, err)
			assert.Equal(t, tt.want, string(logfile))
			raw, err := fs.ReadFile("raw.bin")
			assert.NoError(t, err)
			assert.Equal(t, packet, raw, "machine presentation never rewrites raw capture bytes")
		})
	}
}
