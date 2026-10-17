// SPDX-License-Identifier: MIT

package translator

import (
	"bytes"
	"encoding/json"
	"io"
	"strings"
	"sync"
	"testing"

	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/internal/trexDecoder"
	"github.com/rokath/trice/internal/vis"
	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// configureStampExpressions isolates globals and selects the issue's down-counter.
func configureStampExpressions(t *testing.T, format string) {
	t.Helper()
	configureStructuredTest(t, format)
	decoder.TargetStamp16 = `printf("time:%.3f us",unwrap(47999-ts,48000)/48)`
	decoder.TargetStamp16Delta = `printf("dt:%.3f us",delta(unwrap(47999-ts,48000))/48)`
	decoder.ShowTargetStamp16Passed, decoder.ShowTargetStamp16DeltaPassed = true, true
}

// runStampWire uses real TREX records for all presentation and observation checks.
func runStampWire(t *testing.T, lut id.TriceIDLookUp, input []byte, router *vis.Router) (string, string) {
	t.Helper()
	var output, diagnostics bytes.Buffer
	dec := trexDecoder.New(&diagnostics, lut, new(sync.RWMutex), nil, bytes.NewReader(input), decoder.LittleEndian)
	if controller, ok := dec.(decoder.VisRecordController); ok {
		controller.SetVisRecordEnabled(router != nil)
	}
	err := decodeAndComposeLoopOutput(&output, &diagnostics, emitter.New(&output), dec, lut, nil, router)
	require.ErrorIs(t, err, io.EOF)
	return output.String(), diagnostics.String()
}

// TestStampExpressionsTrackInvisibleWraps exercises every output mode and each
// event-selection path, including visualization suppression and delta-only output.
func TestStampExpressionsTrackInvisibleWraps(t *testing.T) {
	for _, format := range []string{"text", "json", "kv"} {
		for _, filter := range []string{"pick", "ban", "level", "vis", "delta only"} {
			t.Run(format+"/"+filter, func(t *testing.T) {
				configureStampExpressions(t, format)
				lut := id.TriceIDLookUp{1: {Type: "TRICE0", Strg: "err:visible\n"}, 2: {Type: "TRICE0", Strg: "msg:heartbeat\n"}}
				var router *vis.Router
				switch filter {
				case "pick", "delta only":
					emitter.Pick = []string{"err"}
					if filter == "delta only" {
						decoder.TargetStamp16 = ""
					}
				case "ban":
					emitter.Ban = []string{"msg"}
				case "level":
					emitter.LogLevel = "wrn"
				case "vis":
					var err error
					router, err = vis.NewRouter(io.Discard, &afero.Afero{Fs: afero.NewMemMapFs()}, lut, new(sync.RWMutex), []string{`msg:printf("%d",ts)@heartbeats;log=drop`}, false)
					require.NoError(t, err)
					t.Cleanup(func() { require.NoError(t, router.Close()) })
				}
				require.NoError(t, emitter.ResolveFilterSelectors())
				var input []byte
				for i, raw := range []uint64{47999, 23999, 47999, 23999, 47999, 23999, 47999} {
					tid := uint16(2)
					if i == 0 || i == 6 {
						tid = 1
					}
					input = append(input, structuredStampedPacket(tid, 16, raw)...)
				}
				output, diagnostics := runStampWire(t, lut, input, router)
				assert.Empty(t, diagnostics)
				assert.NotContains(t, output, "heartbeat")
				assert.Equal(t, 2, strings.Count(output, "visible"))
				switch format {
				case "text":
					want := "0.000 us - us visible\n3000.000 us 3000.000 us visible\n"
					if filter == "delta only" {
						want = "- us visible\n3000.000 us visible\n"
					}
					assert.Equal(t, want, output)
				case "json":
					lines := strings.Split(strings.TrimSpace(output), "\n")
					require.Len(t, lines, 2)
					var first, last map[string]any
					require.NoError(t, json.Unmarshal([]byte(lines[0]), &first))
					require.NoError(t, json.Unmarshal([]byte(lines[1]), &last))
					assert.NotContains(t, first, "ts16Delta")
					assert.Equal(t, "3000.000 us", last["ts16Delta"])
					if filter == "delta only" {
						assert.NotContains(t, last, "ts16")
					} else {
						assert.Equal(t, "3000.000 us", last["ts16"])
					}
				case "kv":
					lines := strings.Split(strings.TrimSpace(output), "\n")
					require.Len(t, lines, 2)
					assert.NotContains(t, lines[0], "ts16Delta=")
					assert.Contains(t, lines[1], `ts16Delta="3000.000 us"`)
				}
			})
		}
	}
}

// TestStampExpressionsEmptyAndComposedCalls covers zero-valued stamps, empty
// messages, missing stamps, diagnostics, and the existing first-call line stamp.
func TestStampExpressionsEmptyAndComposedCalls(t *testing.T) {
	configureStampExpressions(t, "text")
	lut := id.TriceIDLookUp{
		1: {Type: "TRICE0", Strg: "msg:start"},
		2: {Type: "TRICE0", Strg: "msg: part"},
		3: {Type: "TRICE0", Strg: "msg: end\n"},
		4: {Type: "TRICE0", Strg: ""},
		5: {Type: "TRICE0", Strg: "msg:next\n"},
	}
	var input []byte
	for _, event := range []struct {
		tid uint16
		raw uint64
	}{{1, 47999}, {2, 23999}, {4, 0}, {3, 47999}, {5, 23999}} {
		input = append(input, structuredStampedPacket(event.tid, 16, event.raw)...)
	}
	output, diagnostics := runStampWire(t, lut, input, nil)
	assert.Empty(t, diagnostics)
	assert.Equal(t, "0.000 us - us start part end\n1500.000 us 1500.000 us next\n", output)

	// An empty first sample still establishes the explicitly selected time origin.
	decoder.TargetStamp16 = `printf("time:%d",unwrap(47999-ts,48000,0))`
	decoder.TargetStamp16Delta = ""
	input = append(structuredStampedPacket(4, 16, 23999), structuredStampedPacket(5, 16, 0)...)
	output, diagnostics = runStampWire(t, lut, input, nil)
	assert.Empty(t, diagnostics)
	assert.Equal(t, "23999 next\n", output)
}

// TestStampExpressionsMissingRecordsAndWidths keeps real zero, absent stamps,
// and independent width histories distinct without relying on message tags.
func TestStampExpressionsMissingRecordsAndWidths(t *testing.T) {
	configureStampExpressions(t, "json")
	decoder.TargetStamp16 = `printf("%d",unwrap(ts,10))`
	decoder.TargetStamp32 = `printf("%d",unwrap(ts,10))`
	decoder.ShowTargetStamp32Passed = true
	decoder.TargetStamp16Delta = ""
	lut := id.TriceIDLookUp{1: {Type: "TRICE0", Strg: "msg:ready"}}
	var input []byte
	input = append(input, structuredStampedPacket(1, 16, 8)...)
	input = append(input, structuredStampedPacket(1, 32, 2)...)
	input = append(input, structuredPacket(1, nil)...)
	input = append(input, structuredStampedPacket(1, 16, 0)...)
	input = append(input, structuredStampedPacket(1, 32, 5)...)
	output, diagnostics := runStampWire(t, lut, input, nil)
	assert.Empty(t, diagnostics)
	lines := strings.Split(strings.TrimSpace(output), "\n")
	require.Len(t, lines, 5)
	assert.Contains(t, lines[0], `"ts16":"8"`)
	assert.Contains(t, lines[1], `"ts32":"2"`)
	assert.NotContains(t, lines[2], `"ts16"`)
	assert.NotContains(t, lines[2], `"ts32"`)
	assert.Contains(t, lines[3], `"ts16":"10"`)
	assert.Contains(t, lines[4], `"ts32":"5"`)
	// Replaying the same source starts a new tracking session.
	again, _ := runStampWire(t, lut, input, nil)
	assert.Equal(t, output, again)
}

// TestStampExpressionErrorsRetainApplicationAndDeltaBaseline verifies unavailable
// fields, startup recognition, and recovery after a failed field render.
func TestStampExpressionErrorsRetainApplicationAndDeltaBaseline(t *testing.T) {
	for _, format := range []string{"text", "json", "kv"} {
		t.Run(format, func(t *testing.T) {
			configureStampExpressions(t, format)
			decoder.TargetStamp16 = ""
			decoder.TargetStamp16Delta = `printf("dt:%d",delta(unwrap(ts,10))/(ts-2))`
			lut := id.TriceIDLookUp{1: {Type: "TRICE0", Strg: "msg:retained\n"}}
			var input []byte
			for _, raw := range []uint64{0, 1, 2, 3} {
				input = append(input, structuredStampedPacket(1, 16, raw)...)
			}
			output, diagnostics := runStampWire(t, lut, input, nil)
			assert.Equal(t, 4, strings.Count(output, "retained"))
			assert.Contains(t, diagnostics, "-ts16delta unavailable:")
			assert.Contains(t, diagnostics, "division by zero")
			if format == "text" {
				assert.Equal(t, "- retained\n-1 retained\n? retained\n2 retained\n", output)
			} else {
				lines := strings.Split(strings.TrimSpace(output), "\n")
				require.Len(t, lines, 4)
				assert.NotContains(t, lines[0], "ts16Delta")
				assert.Contains(t, lines[2], "?")
				assert.Contains(t, lines[3], "2")
			}
		})
	}
}

// TestStampExpressionsIgnoreMalformedEvents prevents a diagnostic's header from
// becoming a usable sample or a fabricated zero within the counter history.
func TestStampExpressionsIgnoreMalformedEvents(t *testing.T) {
	configureStampExpressions(t, "json")
	decoder.TargetStamp16 = `printf("%d",unwrap(ts,10))`
	decoder.TargetStamp16Delta = ""
	lut := id.TriceIDLookUp{1: {Type: "TRICE0", Strg: "msg:valid"}, 2: {Type: "TRICE0", Strg: "msg:{unclosed"}}
	input := append(structuredStampedPacket(1, 16, 8), structuredStampedPacket(2, 16, 0)...)
	input = append(input, structuredStampedPacket(1, 16, 9)...)
	output, diagnostics := runStampWire(t, lut, input, nil)
	assert.Contains(t, diagnostics, "unclosed")
	assert.Equal(t, 2, strings.Count(output, "valid"))
	assert.Contains(t, output, `"ts16":"9"`)
	assert.NotContains(t, output, `"ts16":"19"`)
}
