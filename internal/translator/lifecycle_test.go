// SPDX-License-Identifier: MIT

package translator

import (
	"bytes"
	"errors"
	"fmt"
	"io"
	"os"
	"strings"
	"sync"
	"testing"
	"time"

	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/internal/receiver"
	"github.com/rokath/trice/internal/trexDecoder"
	"github.com/stretchr/testify/assert"
)

// lifecycleInput supplies exact transport fragments, including empty reads and
// data accompanied by EOF. A read budget turns accidental endless polling into
// an observable error rather than a hung test. It owns no external resources.
type lifecycleInput struct {
	steps  []scriptedDecoderStep
	reads  int
	closes int
}

func (r *lifecycleInput) Read(b []byte) (int, error) {
	r.reads++
	if r.reads > len(r.steps)+20 {
		return 0, errors.New("input was polled after exhaustion")
	}
	if r.reads > len(r.steps) {
		return 0, io.EOF
	}
	s := r.steps[r.reads-1]
	return copy(b, s.data), s.err
}

func (r *lifecycleInput) Write(b []byte) (int, error) { return len(b), nil }
func (r *lifecycleInput) Close() error                { r.closes++; return nil }

// TestReplayDrainsBufferedRecordsBeforeEOF covers the actual decoder, not only
// a mock EOF. An empty record between two messages must not truncate the batch.
func TestReplayDrainsBufferedRecordsBeforeEOF(t *testing.T) {
	for _, framing := range []string{"none", "COBS"} {
		for _, fragmented := range []bool{false, true} {
			name := framing + "/one_read_with_EOF"
			if fragmented {
				name = framing + "/one_byte_fragments"
			}
			t.Run(name, func(t *testing.T) {
				configureStructuredTest(t, "text")
				decoder.PackageFraming = framing
				lut := id.TriceIDLookUp{
					100: {Type: "TRICE32_1", Strg: `msg:value=%d\n`},
					101: {Type: "TRICE0", Strg: ""},
					102: {Type: "TRICE0", Strg: "msg:last fragment"},
				}
				var wire []byte
				for _, packet := range [][]byte{structuredPacket(100, structuredWords(32, 7)), structuredPacket(101, nil), structuredPacket(102, nil)} {
					wire = append(wire, packet...)
				}
				if framing == "COBS" {
					// Three independently framed records, including trailing zero payload bytes.
					wire = []byte{6, 100, 0x40, 0xc0, 4, 7, 1, 1, 1, 0, 4, 101, 0x40, 0xc0, 1, 0, 4, 102, 0x40, 0xc0, 1, 0}
				}
				input := &lifecycleInput{}
				if fragmented {
					for _, b := range wire {
						input.steps = append(input.steps, scriptedDecoderStep{data: string([]byte{b})})
					}
				} else {
					input.steps = []scriptedDecoderStep{{data: string(wire), err: io.EOF}}
				}
				var output bytes.Buffer
				dec := trexDecoder.New(&output, lut, new(sync.RWMutex), nil, input, decoder.LittleEndian)
				err := decodeAndComposeLoop(&output, emitter.New(&output), dec, lut, nil, nil)
				assert.ErrorIs(t, err, io.EOF)
				assert.Equal(t, "value=7\nlast fragment\n", output.String())
				assert.LessOrEqual(t, input.reads, len(input.steps)+5, "no time-based polling after buffered records are drained")
			})
		}
	}
}

// TestReplayRetainsSplitStampsAndLongStrings splits every header/payload byte.
// Timestamp bytes, a duplicated 16-bit ID, and the long-length encoding must
// survive transport boundaries without manufacturing diagnostics or records.
func TestReplayRetainsSplitStampsAndLongStrings(t *testing.T) {
	for _, tc := range []struct {
		name, kind, format, want string
		wire                     []byte
		bits                     int
		stamp                    uint64
		double                   bool
	}{
		{"stamp16", "TRICE32_1", "msg:value=%d", "msg:value=7", []byte{100, 0x80, 0x34, 0x12, 0xc0, 4, 7, 0, 0, 0}, 16, 0x1234, false},
		{"stamp16_duplicated_ID", "TRICE32_1", "msg:value=%d", "msg:value=7", []byte{100, 0x80, 100, 0x80, 0x34, 0x12, 0xc0, 4, 7, 0, 0, 0}, 16, 0x1234, true},
		{"stamp32", "TRICE32_1", "msg:value=%d", "msg:value=7", []byte{100, 0xc0, 0x78, 0x56, 0x34, 0x12, 0xc0, 4, 7, 0, 0, 0}, 32, 0x12345678, false},
		{"long_runtime_string", "triceS", "msg:%s", "msg:" + strings.Repeat("x", 128), append([]byte{100, 0x40, 0x80, 0x80}, []byte(strings.Repeat("x", 128))...), 0, 0, false},
	} {
		t.Run(tc.name, func(t *testing.T) {
			configureStructuredTest(t, "text")
			trexDecoder.Doubled16BitID = tc.double
			input := &lifecycleInput{}
			for i, b := range tc.wire {
				step := scriptedDecoderStep{data: string([]byte{b})}
				if i == len(tc.wire)-1 {
					step.err = io.EOF // The last byte is still meaningful data.
				}
				input.steps = append(input.steps, step)
			}
			lut := id.TriceIDLookUp{100: {Type: tc.kind, Strg: tc.format}}
			dec := trexDecoder.New(io.Discard, lut, new(sync.RWMutex), nil, input, decoder.LittleEndian)
			buffer := make([]byte, 1024)
			for range len(tc.wire) - 1 {
				n, err := dec.Read(buffer)
				assert.NoError(t, err)
				assert.Zero(t, n, "an incomplete record has no text or diagnostic yet")
			}
			n, err := dec.Read(buffer)
			assert.NoError(t, err)
			assert.Equal(t, tc.want, string(buffer[:n]))
			record, available := dec.(decoder.RecordProvider).ApplicationRecord()
			assert.True(t, available)
			assert.Equal(t, tc.bits, record.StampBits)
			assert.Equal(t, tc.stamp, record.Stamp)
			n, err = dec.Read(buffer)
			assert.ErrorIs(t, err, io.EOF)
			assert.Zero(t, n)
		})
	}
}

// TestReplayEmptyAndTruncatedInputsTerminate checks both empty input and a
// partial final frame. Preserving the old diagnostic policy must not mean spin.
func TestReplayEmptyAndTruncatedInputsTerminate(t *testing.T) {
	for _, framing := range []string{"none", "COBS"} {
		for _, wire := range []string{"", "\x64", "\x64\x40", "\x64\x40\xc0\x04\x07"} {
			t.Run(framing+"/"+fmtBytes(wire), func(t *testing.T) {
				configureStructuredTest(t, "text")
				decoder.PackageFraming = framing
				input := &lifecycleInput{steps: []scriptedDecoderStep{{data: wire, err: io.EOF}}}
				lut := id.TriceIDLookUp{100: {Type: "TRICE32_1", Strg: `msg:%d\n`}}
				var output bytes.Buffer
				dec := trexDecoder.New(&output, lut, new(sync.RWMutex), nil, input, decoder.LittleEndian)
				assert.ErrorIs(t, decodeAndComposeLoop(&output, emitter.New(&output), dec, lut, nil, nil), io.EOF)
				assert.Empty(t, output.String(), "an incomplete record must not become an application message")
				assert.LessOrEqual(t, input.reads, 3)
			})
		}
	}
}

// fmtBytes gives truncated input cases readable names without embedding raw control bytes.
func fmtBytes(s string) string { return fmt.Sprintf("%x", s) }

// TestLivePausesAndReadFailures preserves tailing FILE behavior and distinguishes
// a TCP replay's empty read from its actual peer EOF.
func TestLivePausesAndReadFailures(t *testing.T) {
	for _, port := range []string{"FILE", "TCP4BUFFER"} {
		t.Run(port, func(t *testing.T) {
			configureStructuredTest(t, "text")
			receiver.Port = port
			failure := errors.New("transport disconnected")
			pause := error(nil)
			if port == "FILE" {
				pause = io.EOF
			}
			input := &lifecycleInput{steps: []scriptedDecoderStep{
				{err: pause}, {err: pause},
				{data: string(structuredPacket(100, nil))}, {err: failure},
			}}
			lut := id.TriceIDLookUp{100: {Type: "TRICE0", Strg: `msg:after pause\n`}}
			var output bytes.Buffer
			dec := trexDecoder.New(&output, lut, new(sync.RWMutex), nil, input, decoder.LittleEndian)
			assert.ErrorIs(t, decodeAndComposeLoop(&output, emitter.New(&output), dec, lut, nil, nil), failure)
			assert.Equal(t, "after pause\n", output.String())
		})
	}
}

// TestSignalHandlerStopJoinsWithoutClosingBorrowedInput tests cancellation via
// a completion channel instead of a sleep or a process-wide goroutine count.
func TestSignalHandlerStopJoinsWithoutClosingBorrowedInput(t *testing.T) {
	for range 50 {
		input := &lifecycleInput{}
		stop := startSignalHandler(io.Discard, input, nil)
		stop()
		assert.Zero(t, input.closes, "normal return leaves input closure to its owner")
	}
	stop := make(chan struct{})
	done := make(chan struct{})
	input := &lifecycleInput{}
	signals := make(chan os.Signal)
	go func() { defer close(done); handleSignals(io.Discard, input, nil, signals, stop) }()
	signals <- os.Interrupt // Handshake proves the handler received the signal.
	close(stop)
	select {
	case <-done:
		assert.Zero(t, input.closes, "normal completion cancels the pending shutdown grace period")
	case <-time.After(5 * time.Second):
		t.Fatal("signal handler did not stop")
	}
}
