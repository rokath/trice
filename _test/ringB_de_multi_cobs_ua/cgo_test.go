// SPDX-License-Identifier: MIT

package cgot

import (
	"bytes"
	"fmt"
	"io"
	"path"
	"testing"

	"github.com/rokath/trice/internal/args"
	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
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
