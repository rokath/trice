// SPDX-License-Identifier: MIT

package cgot

import (
	"bytes"
	"fmt"
	"io"
	"os"
	"os/exec"
	"path"
	"path/filepath"
	"strings"
	"testing"

	"github.com/rokath/trice/internal/args"
	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func init() {
	triceLog = func(t *testing.T, fSys *afero.Afero, buffer string) string {
		var o bytes.Buffer
		f := args.Handler(io.Writer(&o), fSys,
			[]string{
				"trice", "log",
				"-i", path.Join(triceDir, "/demoTIL.json"),
				"-hs=off", "-prefix=off", "-li=off", "-color=off",
				"-p=BUFFER", "-args", buffer,
				"-ts0", "time:        ",
				"-ts16", "time:    %04x",
				"-ts32", "time:%08x",

				"-pf=COBS",
			},
		)
		assert.Nil(t, f)
		return o.String()
	}
	targetMode = "deferredModeBulk"
}

// TestStructuredTriceExamples checks the canonical C examples through the same
// target buffer, TREX decoder, and text CLI settings as the PC target suite.
// It selects only the new examples so unrelated legacy expectations cannot hide
// which structured call produced a mismatch.
func TestStructuredTriceExamples(t *testing.T) {
	fileSystem := &afero.Afero{Fs: afero.NewOsFs()}
	allResults := getExpectedResults(fileSystem, targetActivityC, -1)
	examples := []struct {
		name     string
		expected string
	}{
		{"8-bit value without timestamp", "time:        default: info:Small 7\n"},
		{"16-bit value with 16-bit timestamp", "time:    be16default: info:Current -123 mA\n"},
		{"32-bit value with 32-bit timestamp", "time:feed3322default: info:Voltage 3300 mV\n"},
		{"64-bit value without timestamp", "time:        default: info:Bytes 1234567890123\n"},
		{"inferred member fields and classic placeholder", "time:        default: info:State 3 rpm 1200 / 1\n"},
		{"float wrapper", "time:        default: info:Temp 23.5 C\n"},
		{"double wrapper", "time:    be16default: info:Energy 12.25 J\n"},
		{"string", "time:        default: info:Device pump\n"},
		{"bounded string", "time:        default: info:Code READY\n"},
		{"manual JSON field name", "time:    be16default: att:MyStructEvaluationFunction(json:ExA{Apple:-1, Birn:2, Fish:2.781000})\n"},
	}
	for _, example := range examples {
		t.Run(example.name, func(t *testing.T) {
			var sourceLine int
			for _, result := range allResults {
				if result.exps == example.expected {
					if sourceLine != 0 {
						t.Fatalf("duplicate C example for %q", example.expected)
					}
					sourceLine = result.line
				}
			}
			if sourceLine == 0 {
				t.Fatalf("missing C example for %q", example.expected)
			}

			buffer := make([]byte, 32768)
			setTriceBuffer(buffer)
			triceCheck(sourceLine)
			transferPendingTrices()
			binary := buffer[:triceOutDepth()]
			encoded := fmt.Sprint(binary)
			actual := triceLog(t, fileSystem, encoded[1:len(encoded)-1])
			triceClearOutBuffer()
			assert.Equal(t, example.expected, actual, "C example at line %d", sourceLine)
		})
	}
}

// TestPCStopsAtFirstMismatch runs the real C-backed comparison loops in a
// subprocess so their deliberate failure cannot fail this regression test.
// TRICE_TEST_NO_STOP must continue to govern packages, not comparisons within
// one package; otherwise one bad prefix can create thousands of follow-on errors.
func TestPCStopsAtFirstMismatch(t *testing.T) {
	const probeEnvironment = "TRICE_PC_FIRST_MISMATCH_PROBE"
	if mode := os.Getenv(probeEnvironment); mode != "" {
		defer setup(t)()
		wrongOutput := func(_ *testing.T, _ *afero.Afero, _ string) string {
			fmt.Println("UNEXPECTED_SECOND_LOG_CALL")
			return strings.Repeat("wrong", 20)
		}
		firstWrongOutput := func(_ *testing.T, _ *afero.Afero, _ string) string {
			return strings.Repeat("wrong", 20)
		}
		switch mode {
		case "line":
			calls := 0
			triceLogLineByLine(t, func(t *testing.T, fs *afero.Afero, data string) string {
				calls++
				if calls > 1 {
					return wrongOutput(t, fs, data)
				}
				return firstWrongOutput(t, fs, data)
			}, 2, targetActivityC)
		case "bulk":
			triceLogBulk(t, firstWrongOutput, 2, targetActivityC)
		case "combined":
			triceLogDirectAndDeferred(t, firstWrongOutput, wrongOutput, 2, targetActivityC)
		default:
			t.Fatalf("unknown mismatch probe %q", mode)
		}
		return
	}

	for _, mode := range []string{"line", "bulk", "combined"} {
		t.Run(mode, func(t *testing.T) {
			firstSourceLine := getExpectedResults(&afero.Afero{Fs: afero.NewOsFs()}, targetActivityC, 1)[0].line
			sourceReference := fmt.Sprintf("line %d", firstSourceLine)
			diagnosticMarker := "Error Trace:"
			if mode == "bulk" {
				sourceReference = fmt.Sprintf("triceCheck.c:%d", firstSourceLine)
				diagnosticMarker = "EXPECTATION FAILURE"
			}
			if mode == "combined" {
				sourceReference = fmt.Sprintf("0 {%d ", firstSourceLine)
			}
			command := exec.Command(os.Args[0], "-test.run=^TestPCStopsAtFirstMismatch$")
			artifacts := t.TempDir()
			command.Env = append(os.Environ(), probeEnvironment+"="+mode, "TRICE_TEST_NO_STOP=1", "TRICE_PC_TEST_ARTIFACT_DIR="+artifacts)
			output, err := command.CombinedOutput()
			require.Error(t, err, "the deliberately wrong first record must fail")
			assert.Equal(t, 1, strings.Count(string(output), diagnosticMarker), "one mismatch should produce one diagnostic: %s", output)
			assert.NotContains(t, string(output), "UNEXPECTED_SECOND_LOG_CALL", "the next record or deferred branch must not run")
			assert.Contains(t, string(output), sourceReference, "the first source line should identify the failure")
			if mode == "bulk" {
				wire, err := os.ReadFile(filepath.Join(artifacts, "deferred.bin"))
				require.NoError(t, err)
				assert.NotEmpty(t, wire, "keep the original C output for replay")
				text, err := os.ReadFile(filepath.Join(artifacts, "deferred.txt"))
				require.NoError(t, err)
				assert.Equal(t, strings.Repeat("wrong", 20), string(text), "keep the complete failed comparison text")
			}
		})
	}
}
