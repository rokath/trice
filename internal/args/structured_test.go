// SPDX-License-Identifier: MIT

package args

import (
	"bytes"
	"encoding/binary"
	"encoding/json"
	"io"
	"math"
	"os"
	"testing"

	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/id"
	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
)

// TestReleaseCompatibilityWithV130Dictionaries uses unchanged TREX bytes and
// dictionary shapes from v1.3.0. Classic printf text stays compatible, whereas
// braces now belong to the template language. These are decoding tests, not a
// conversion of historical dictionaries; the original JSON must stay untouched.
func TestReleaseCompatibilityWithV130Dictionaries(t *testing.T) {
	for _, tc := range []struct {
		name, triceType, format, text, diagnostic string
		withValue                                 bool
	}{
		{"classic_printf_still_decodes", "TRICE32_1", `msg:count=%d\n`, "count=7\n", "", true},
		{"untagged_text_stays_literal", "TRICE0", `hi\n`, "hi\n", "", false},
		{"historical_field_shaped_literal_requires_its_old_decoder", "TRICE0", `msg:literal={x}\n`, "", "ignoring package", false},
		{"historical_set_literal_is_not_a_valid_field", "TRICE0", `msg:set={1,2}\n`, "", "invalid structured field name", false},
		{"current_escaped_braces_render_as_one_pair", "TRICE0", `msg:literal={{x}}\n`, "literal={x}\n", "", false},
		{"current_named_field_uses_the_same_scalar_wire_value", "TRICE32_1", `msg:value={x}\n`, "value=7\n", "", true},
	} {
		t.Run(tc.name, func(t *testing.T) {
			FlagsInit()
			t.Cleanup(FlagsInit)
			fs := &afero.Afero{Fs: afero.NewMemMapFs()}
			dictionary, err := json.Marshal(id.TriceIDLookUp{100: {Type: tc.triceType, Strg: tc.format}})
			if !assert.NoError(t, err) {
				return
			}
			assert.NoError(t, fs.WriteFile("til.json", dictionary, 0o600))
			packet := binary.LittleEndian.AppendUint16(nil, 0x4064)
			if tc.withValue {
				packet = binary.LittleEndian.AppendUint16(packet, 0x04c0)
				packet = binary.LittleEndian.AppendUint32(packet, 7)
			} else {
				packet = binary.LittleEndian.AppendUint16(packet, 0x00c0)
			}
			assert.NoError(t, fs.WriteFile("capture.bin", packet, 0o600))
			var out bytes.Buffer
			err = Handler(&out, fs, []string{"trice", "log", "-p", "FILEBUFFER", "-args", "capture.bin", "-pf", "none", "-i", "til.json", "-li", "off", "-hs", "off", "-ts", "off", "-prefix", "", "-suffix", "", "-color", "none"})
			assert.NoError(t, err, "a diagnostic record is not a fatal CLI failure")
			if tc.diagnostic != "" {
				assert.Contains(t, out.String(), tc.diagnostic)
				assert.NotContains(t, out.String(), "literal={x}\n", "the current host must not silently claim legacy literal-brace support")
			} else {
				assert.Equal(t, tc.text, out.String())
			}
			unchanged, err := fs.ReadFile("til.json")
			assert.NoError(t, err)
			assert.Equal(t, dictionary, unchanged, "logging never migrates a supplied dictionary")
		})
	}
}

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
		{"unknown format", []string{"-logFormat", "yaml"}, "expected text, json, kv, or key-value"},
		{"NDJSON is not a separate CLI value", []string{"-logFormat", "ndjson"}, "expected text, json, kv, or key-value"},
		{"CHAR has no events", []string{"-logFormat", "json", "-encoding", "CHAR"}, "requires -encoding TREX"},
		{"DUMP has no events", []string{"-logFormat", "KEY-VALUE", "-encoding", "DUMP"}, "requires -encoding TREX"},
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

// TestStructuredCLIFormatNames checks both command entry points and confirms
// that aliases reach the renderer as its canonical, case-sensitive names.
func TestStructuredCLIFormatNames(t *testing.T) {
	oldStart := startLogLoop
	t.Cleanup(func() { startLogLoop = oldStart; FlagsInit() })
	for _, tt := range []struct {
		name, input, want string
	}{
		{"default text", "", "text"},
		{"mixed-case text", "TeXt", "text"},
		{"uppercase JSON", "JSON", "json"},
		{"mixed-case JSON", "JsOn", "json"},
		{"uppercase KV", "KV", "kv"},
		{"mixed-case key-value", "KeY-VaLuE", "kv"},
		{"legacy kv", "kv", "kv"},
	} {
		for _, entry := range []string{"trice", "tlog"} {
			t.Run(entry+"/"+tt.name, func(t *testing.T) {
				FlagsInit()
				fs := &afero.Afero{Fs: afero.NewMemMapFs()}
				var actual string
				startLogLoop = func(io.Writer, *afero.Afero) error {
					actual = decoder.LogFormat
					return nil
				}
				options := []string{}
				if tt.input != "" {
					options = []string{"-logFormat", tt.input}
				}
				var err error
				if entry == "trice" {
					err = Handler(io.Discard, fs, append([]string{"trice", "log"}, options...))
				} else {
					err = LogHandler(io.Discard, fs, append([]string{"tlog"}, options...))
				}
				assert.NoError(t, err)
				assert.Equal(t, tt.want, actual)
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
		{"json", `{"tag":"INFO","level":"INFO","message":"Motor 3: 87.5 C","fields":{"motor_id":3,"temperature_c":87.5}}` + "\n"},
		{"JSON", `{"tag":"INFO","level":"INFO","message":"Motor 3: 87.5 C","fields":{"motor_id":3,"temperature_c":87.5}}` + "\n"},
		{"kv", "tag=INFO level=INFO message=\"Motor 3: 87.5 C\" field.motor_id=3 field.temperature_c=87.5\n"},
		{"key-value", "tag=INFO level=INFO message=\"Motor 3: 87.5 C\" field.motor_id=3 field.temperature_c=87.5\n"},
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
