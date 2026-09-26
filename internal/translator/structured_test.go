// SPDX-License-Identifier: MIT

package translator

import (
	"bytes"
	"encoding/binary"
	"encoding/json"
	"errors"
	"io"
	"math"
	"strconv"
	"strings"
	"sync"
	"testing"
	"time"

	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/internal/trexDecoder"
	"github.com/rokath/trice/internal/vis"
	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
)

// structuredPacket constructs the real unstamped TREX wire format, including
// direct-output alignment. Tests exercise decoding rather than inventing fields
// from already formatted strings or constructing only serializer input.
func structuredPacket(tid uint16, payload []byte) []byte {
	packet := binary.LittleEndian.AppendUint16(nil, 0x4000|tid)
	packet = binary.LittleEndian.AppendUint16(packet, uint16(len(payload))<<8|0xc0)
	packet = append(packet, payload...)
	for len(packet)%4 != 0 {
		packet = append(packet, 0)
	}
	return packet
}

// structuredStampedPacket supplies the actual TREX timestamp header without
// depending on display formatting or serializer internals.
func structuredStampedPacket(tid uint16, bits int, stamp uint64) []byte {
	header := uint16(0x8000 | tid)
	if bits == 32 {
		header = 0xc000 | tid
	}
	packet := binary.LittleEndian.AppendUint16(nil, header)
	if bits == 16 {
		packet = binary.LittleEndian.AppendUint16(packet, uint16(stamp))
	} else {
		packet = binary.LittleEndian.AppendUint32(packet, uint32(stamp))
	}
	packet = binary.LittleEndian.AppendUint16(packet, 0x00c0)
	return packet
}

// structuredWords preserves all 64 bits of test values during wire assembly.
func structuredWords(bits int, values ...uint64) []byte {
	var payload []byte
	for _, value := range values {
		switch bits {
		case 8:
			payload = append(payload, byte(value))
		case 16:
			payload = binary.LittleEndian.AppendUint16(payload, uint16(value))
		case 32:
			payload = binary.LittleEndian.AppendUint32(payload, uint32(value))
		case 64:
			payload = binary.LittleEndian.AppendUint64(payload, value)
		}
	}
	return payload
}

// configureStructuredTest keeps legacy package globals isolated. No test in
// this suite opens ports, writes repository data, or depends on wall-clock text.
func configureStructuredTest(t *testing.T, format string) {
	t.Helper()
	configureTranslatorLoopTest(t)
	oldFormat, oldFraming := decoder.LogFormat, decoder.PackageFraming
	oldCycle, oldDisable := decoder.InitialCycle, trexDecoder.DisableCycleErrors
	oldAddNL := trexDecoder.AddNewlineToEachTriceMessage
	oldUnsigned := decoder.Unsigned
	oldDoubledID := trexDecoder.Doubled16BitID
	old32, old32Delta := decoder.TargetStamp32, decoder.TargetStamp32Delta
	old32Passed, old32DeltaPassed := decoder.ShowTargetStamp32Passed, decoder.ShowTargetStamp32DeltaPassed
	t.Cleanup(func() {
		decoder.LogFormat, decoder.PackageFraming = oldFormat, oldFraming
		decoder.InitialCycle, trexDecoder.DisableCycleErrors = oldCycle, oldDisable
		trexDecoder.AddNewlineToEachTriceMessage = oldAddNL
		decoder.Unsigned = oldUnsigned
		trexDecoder.Doubled16BitID = oldDoubledID
		decoder.TargetStamp32, decoder.TargetStamp32Delta = old32, old32Delta
		decoder.ShowTargetStamp32Passed, decoder.ShowTargetStamp32DeltaPassed = old32Passed, old32DeltaPassed
	})
	decoder.LogFormat, decoder.PackageFraming = format, "none"
	decoder.InitialCycle, trexDecoder.DisableCycleErrors = false, true
	trexDecoder.AddNewlineToEachTriceMessage = false
	decoder.Unsigned = false
	trexDecoder.Doubled16BitID = false
	decoder.TargetStamp32, decoder.TargetStamp32Delta = "", ""
	decoder.ShowTargetStamp32Passed, decoder.ShowTargetStamp32DeltaPassed = false, false
}

// runStructuredWire returns independent application and diagnostic channels.
func runStructuredWire(t *testing.T, lut id.TriceIDLookUp, input []byte) (string, string) {
	t.Helper()
	var output, diagnostics bytes.Buffer
	dec := trexDecoder.New(&diagnostics, lut, new(sync.RWMutex), nil, bytes.NewReader(input), decoder.LittleEndian)
	err := decodeAndComposeLoopOutput(&output, &diagnostics, emitter.New(&output), dec, lut, nil, nil)
	assert.ErrorIs(t, err, io.EOF)
	return output.String(), diagnostics.String()
}

// TestStructuredJSONIntegerBoundaries verifies type, magnitude, source order,
// and independence from signed text rendering at every supported target width.
func TestStructuredJSONIntegerBoundaries(t *testing.T) {
	for _, bits := range []int{8, 16, 32, 64} {
		t.Run(strconv.Itoa(bits)+" bit boundaries", func(t *testing.T) {
			configureStructuredTest(t, "json")
			maximum := uint64(math.MaxUint64)
			if bits < 64 {
				maximum = (uint64(1) << bits) - 1
			}
			minimum := uint64(1) << (bits - 1)
			lut := id.TriceIDLookUp{1: {Type: "TRICE" + strconv.Itoa(bits) + "_4", Strg: "info:{signed:%d} {unsigned:%u} {hex:%x} {boolean:%t}"}}
			output, diagnostics := runStructuredWire(t, lut, structuredPacket(1, structuredWords(bits, minimum, maximum, maximum, 2)))
			assert.Empty(t, diagnostics)
			// Decode just the integer fields as raw JSON to avoid float64 rounding.
			var raw map[string]json.RawMessage
			assert.NoError(t, json.Unmarshal([]byte(output), &raw))
			var fields map[string]json.RawMessage
			assert.NoError(t, json.Unmarshal(raw["fields"], &fields))
			assert.Equal(t, strconv.FormatInt(int64(minimum<<(64-bits))>>(64-bits), 10), string(fields["signed"]))
			assert.Equal(t, strconv.FormatUint(maximum, 10), string(fields["unsigned"]))
			assert.Equal(t, strconv.FormatUint(maximum, 10), string(fields["hex"]), "-unsigned=false must not reinterpret structured hex as signed")
			assert.Equal(t, "true", string(fields["boolean"]))
			assert.Contains(t, output, `"fields":{"signed":`)
			assert.Less(t, strings.Index(output, `"unsigned":`), strings.Index(output, `"hex":`))
		})
	}
}

// TestStructuredJSONFloatAndStringValues verifies that display precision never
// rounds or truncates the corresponding structured value.
func TestStructuredJSONFloatAndStringValues(t *testing.T) {
	for _, tt := range []struct {
		name, triceType, template string
		payload                   []byte
		expected                  string
	}{
		{"float32", "TRICE32_1", "info:{temperature:%.1f C}", structuredWords(32, uint64(math.Float32bits(1.25))), `{"tag":"info","level":"INFO","message":"1.2 C","fields":{"temperature":1.25}}` + "\n"},
		{"float64", "TRICE64_1", "warn:{value:%.2f}", structuredWords(64, math.Float64bits(1.234567890123)), `{"tag":"warn","level":"WARNING","message":"1.23","fields":{"value":1.234567890123}}` + "\n"},
		{"NaN has message but no fields", "TRICE32_1", "info:{value:%f}", structuredWords(32, uint64(math.Float32bits(float32(math.NaN())))), `{"tag":"info","level":"INFO","message":"NaN"}` + "\n"},
		{"positive infinity omitted", "TRICE64_1", "info:{value:%g}", structuredWords(64, math.Float64bits(math.Inf(1))), `{"tag":"info","level":"INFO","message":"+Inf"}` + "\n"},
		{"negative infinity omitted", "TRICE64_1", "info:{value:%g}", structuredWords(64, math.Float64bits(math.Inf(-1))), `{"tag":"info","level":"INFO","message":"-Inf"}` + "\n"},
		{"string clipping is presentation only", "triceS", "info:{message:%.3s}", []byte(`motor "A"`), `{"tag":"info","level":"INFO","message":"mot","fields":{"message":"motor \"A\""}}` + "\n"},
		{"runtime tag cannot reclassify string", "triceN", "{text:%s}", []byte(`err:C:\tmp\new`), `{"tag":"untagged","message":"err:C:\\tmp\\new","fields":{"text":"err:C:\\tmp\\new"}}` + "\n"},
	} {
		t.Run(tt.name, func(t *testing.T) {
			configureStructuredTest(t, "json")
			output, diagnostics := runStructuredWire(t, id.TriceIDLookUp{1: {Type: tt.triceType, Strg: tt.template}}, structuredPacket(1, tt.payload))
			assert.Empty(t, diagnostics)
			assert.Equal(t, tt.expected, output)
		})
	}
}

// TestStructuredRecordsPreserveEventBoundaries covers fragments, multiline and
// empty calls, literal braces, unknown tags, and collisions with host field names.
func TestStructuredRecordsPreserveEventBoundaries(t *testing.T) {
	configureStructuredTest(t, "json")
	lut := id.TriceIDLookUp{
		1: {Type: "TRICE32_1", Strg: `info:A{tag}`},
		2: {Type: "TRICE0", Strg: `info:B\nC\n`},
		3: {Type: "TRICE0", Strg: ""},
		4: {Type: "TRICE32_1", Strg: `mgs:set={{1,2}} {value}`},
	}
	var input []byte
	input = append(input, structuredPacket(1, structuredWords(32, 7))...)
	input = append(input, structuredPacket(2, nil)...)
	input = append(input, structuredPacket(3, nil)...)
	input = append(input, structuredPacket(4, structuredWords(32, 8))...)
	output, diagnostics := runStructuredWire(t, lut, input)
	assert.Empty(t, diagnostics)
	assert.Equal(t, "{\"tag\":\"info\",\"level\":\"INFO\",\"message\":\"A7\",\"fields\":{\"tag\":7}}\n"+
		"{\"tag\":\"info\",\"level\":\"INFO\",\"message\":\"B\\nC\\n\"}\n"+
		"{\"tag\":\"untagged\",\"message\":\"\"}\n"+
		"{\"tag\":\"untagged\",\"message\":\"mgs:set={1,2} 8\",\"fields\":{\"value\":8}}\n", output)
}

// TestStructuredKVQuotingAndFieldOrder exercises reserved names and all required
// escapes while retaining scalar booleans, addresses, and characters as typed data.
func TestStructuredKVQuotingAndFieldOrder(t *testing.T) {
	configureStructuredTest(t, "kv")
	lut := id.TriceIDLookUp{1: {Type: "TRICE32_3", Strg: "Warning:{tag:%c} {addr:%p} {enabled:%t}"}}
	output, diagnostics := runStructuredWire(t, lut, structuredPacket(1, structuredWords(32, 'A', 0x20001234, 0)))
	assert.Empty(t, diagnostics)
	assert.Equal(t, "tag=Warning level=WARNING message=\"A 20001234 false\" field.tag=\"A\" field.addr=0x20001234 field.enabled=false\n", output)

	text := "Motor \"A\"\\path\nline\r\ttab"
	output, diagnostics = runStructuredWire(t, id.TriceIDLookUp{1: {Type: "triceS", Strg: "info:{message:%s}"}}, structuredPacket(1, []byte(text)))
	assert.Empty(t, diagnostics)
	assert.Equal(t, "tag=info level=INFO message=\"Motor \\\"A\\\"\\\\path\\nline\\r\\ttab\" field.message=\"Motor \\\"A\\\"\\\\path\\nline\\r\\ttab\"\n", output)
}

// TestStructuredMetadataAreOptionalFacts checks that formatting columns never
// leak into records and zero-valued but present timestamps remain representable.
func TestStructuredMetadataAreOptionalFacts(t *testing.T) {
	configureStructuredTest(t, "json")
	decoder.ShowID = "debug:%7d "
	decoder.LocationInformationFormatString = "info:%21s%6d "
	id.LIFnJSON = "li.json"
	decoder.TargetStamp16 = "us"
	emitter.HostStamp = "zero"
	record := decoder.ApplicationRecord{Tag: "info", Message: "info:ready", ID: 7, HasID: true, Stamp: 0, StampBits: 16}
	state := targetStampState{}
	encoded, err := renderStructuredRecord(record, id.TriceIDLookUpLI{7: {File: "motor.c", Line: 42}}, time.Time{}, &state)
	assert.NoError(t, err)
	assert.Equal(t, `{"tag":"info","level":"INFO","message":"ready","id":7,"file":"motor.c","line":42,"ts16":"0_000","hs":"2006-01-02_1504-05"}`+"\n", string(encoded))
	decoder.ShowID, decoder.LocationInformationFormatString, decoder.TargetStamp16, emitter.HostStamp = "", "off", "", "off"
	encoded, err = renderStructuredRecord(record, nil, time.Time{}, &state)
	assert.NoError(t, err)
	assert.Equal(t, `{"tag":"info","level":"INFO","message":"ready"}`+"\n", string(encoded))
}

// TestStructuredKVMetadataUsesCLIOptions keeps location, ID, and host time
// under their existing switches while excluding text-only decorations.
func TestStructuredKVMetadataUsesCLIOptions(t *testing.T) {
	configureStructuredTest(t, "kv")
	decoder.ShowID = "debug:%7d "
	decoder.LocationInformationFormatString = "info:%21s%6d "
	id.LIFnJSON = "li.json"
	decoder.TargetStamp16 = "temp:%d C "
	emitter.HostStamp = "UTCmicro"
	emitter.Prefix, emitter.Suffix = "prefix:", ":suffix"
	now := time.Date(2024, time.January, 2, 3, 4, 5, 0, time.UTC)
	record := decoder.ApplicationRecord{Tag: "info", Message: "info:ready", ID: 7, HasID: true, Stamp: 25, StampBits: 16}
	state := targetStampState{}
	encoded, err := renderStructuredRecord(record, id.TriceIDLookUpLI{7: {File: "motor.c", Line: 42}}, now, &state)
	assert.NoError(t, err)
	assert.Equal(t, `tag=info level=INFO message="ready" id=7 file="motor.c" line=42 ts16="25 C" hs="UTC Jan  2 03:04:05.000000"`+"\n", string(encoded))

	decoder.ShowID, decoder.LocationInformationFormatString = "", "off"
	decoder.TargetStamp16, emitter.HostStamp = "", "off"
	encoded, err = renderStructuredRecord(record, id.TriceIDLookUpLI{7: {File: "motor.c", Line: 42}}, now, &state)
	assert.NoError(t, err)
	assert.Equal(t, `tag=info level=INFO message="ready"`+"\n", string(encoded))
}

// TestStructuredBuiltinStampFormatsKeepUnits verifies built-in CLI layouts are
// exported as strings without their text-display tags or outer padding.
func TestStructuredBuiltinStampFormatsKeepUnits(t *testing.T) {
	configureStructuredTest(t, "json")
	decoder.TargetStamp16, decoder.TargetStamp32 = "ms", "us"
	state := targetStampState{}
	for _, tt := range []struct {
		bits  int
		stamp uint64
		want  string
	}{
		{16, 1234, `{"tag":"info","level":"INFO","message":"ready","ts16":"1,234"}` + "\n"},
		{32, 1005000, `{"tag":"info","level":"INFO","message":"ready","ts32":"1,005_000"}` + "\n"},
	} {
		record := decoder.ApplicationRecord{Tag: "info", Message: "info:ready", StampBits: tt.bits, Stamp: tt.stamp}
		encoded, err := renderStructuredRecord(record, nil, time.Time{}, &state)
		assert.NoError(t, err)
		assert.Equal(t, tt.want, string(encoded))
	}
	assert.Equal(t, "2026-09-26 12:34:56", structuredTargetStamp("  2026-09-26 12:34:56  "), "colons in formatted time are data")
}

// TestStructuredTargetStampsKeepIndependentHistories exercises both timestamp
// widths in one stream. Each delta begins only after a same-width predecessor.
func TestStructuredTargetStampsKeepIndependentHistories(t *testing.T) {
	for _, tt := range []struct {
		format string
		want   []string
	}{
		{"json", []string{
			`{"tag":"info","level":"INFO","message":"ready","ts16":"65530 C"}` + "\n",
			`{"tag":"info","level":"INFO","message":"ready","ts32":"123456 us"}` + "\n",
			`{"tag":"info","level":"INFO","message":"ready","ts16":"4 C","ts16Delta":"10 C"}` + "\n",
			`{"tag":"info","level":"INFO","message":"ready","ts32":"123471 us","ts32Delta":"15 us"}` + "\n",
		}},
		{"kv", []string{
			`tag=info level=INFO message="ready" ts16="65530 C"` + "\n",
			`tag=info level=INFO message="ready" ts32="123456 us"` + "\n",
			`tag=info level=INFO message="ready" ts16="4 C" ts16Delta="10 C"` + "\n",
			`tag=info level=INFO message="ready" ts32="123471 us" ts32Delta="15 us"` + "\n",
		}},
	} {
		t.Run(tt.format, func(t *testing.T) {
			configureStructuredTest(t, tt.format)
			decoder.TargetStamp16, decoder.TargetStamp16Delta = "temp:%d C ", "step:%d C "
			decoder.TargetStamp32, decoder.TargetStamp32Delta = "time:%d us ", "dt:%d us "
			state := targetStampState{}
			for i, event := range []struct {
				bits  int
				stamp uint64
			}{{16, 65530}, {32, 123456}, {16, 4}, {32, 123471}} {
				record := decoder.ApplicationRecord{Tag: "info", Message: "info:ready", StampBits: event.bits, Stamp: event.stamp}
				encoded, err := renderStructuredRecord(record, nil, time.Time{}, &state)
				assert.NoError(t, err)
				assert.Equal(t, tt.want[i], string(encoded))
			}
		})
	}
}

// TestStructuredStampedWireDeltaState ensures the decoder loop passes one
// persistent state through real interleaved 16- and 32-bit TREX calls.
func TestStructuredStampedWireDeltaState(t *testing.T) {
	configureStructuredTest(t, "json")
	decoder.TargetStamp16, decoder.TargetStamp16Delta = "time:%d", "dt:%d"
	decoder.TargetStamp32, decoder.TargetStamp32Delta = "time:%d", "dt:%d"
	decoder.ShowTargetStamp16Passed, decoder.ShowTargetStamp32Passed = true, true
	decoder.ShowTargetStamp16DeltaPassed, decoder.ShowTargetStamp32DeltaPassed = true, true
	lut := id.TriceIDLookUp{1: {Type: "TRICE0", Strg: "info:ready"}}
	var input []byte
	input = append(input, structuredStampedPacket(1, 16, 65530)...)
	input = append(input, structuredStampedPacket(1, 32, 100)...)
	input = append(input, structuredStampedPacket(1, 16, 4)...)
	input = append(input, structuredStampedPacket(1, 32, 105)...)
	output, diagnostics := runStructuredWire(t, lut, input)
	assert.Empty(t, diagnostics)
	assert.Equal(t, `{"tag":"info","level":"INFO","message":"ready","ts16":"65530"}`+"\n"+
		`{"tag":"info","level":"INFO","message":"ready","ts32":"100"}`+"\n"+
		`{"tag":"info","level":"INFO","message":"ready","ts16":"4","ts16Delta":"10"}`+"\n"+
		`{"tag":"info","level":"INFO","message":"ready","ts32":"105","ts32Delta":"5"}`+"\n", output)
}

// TestStructuredTargetMetadataRequiresEnabledActualStamps rejects no-stamp
// placeholders and checks that absolute and delta switches act independently.
func TestStructuredTargetMetadataRequiresEnabledActualStamps(t *testing.T) {
	configureStructuredTest(t, "json")
	decoder.TargetStamp0, decoder.TargetStamp0Delta = "time:blank", "dt:blank"
	decoder.TargetStamp16, decoder.TargetStamp16Delta = "", "step:%d ticks"
	decoder.TargetStamp32, decoder.TargetStamp32Delta = "off", "none"
	state := targetStampState{}
	for _, tt := range []struct {
		name  string
		bits  int
		stamp uint64
		want  string
	}{
		{"unstamped", 0, 0, `{"tag":"info","level":"INFO","message":"ready"}` + "\n"},
		{"16-bit first delta", 16, 8, `{"tag":"info","level":"INFO","message":"ready"}` + "\n"},
		{"16-bit next delta without absolute", 16, 11, `{"tag":"info","level":"INFO","message":"ready","ts16Delta":"3 ticks"}` + "\n"},
		{"32-bit disabled", 32, 100, `{"tag":"info","level":"INFO","message":"ready"}` + "\n"},
	} {
		t.Run(tt.name, func(t *testing.T) {
			record := decoder.ApplicationRecord{Tag: "info", Message: "info:ready", StampBits: tt.bits, Stamp: tt.stamp}
			encoded, err := renderStructuredRecord(record, nil, time.Time{}, &state)
			assert.NoError(t, err)
			assert.Equal(t, tt.want, string(encoded))
		})
	}
}

// TestStructuredDiagnosticsAndSelectionRemainIndependent verifies a valid event
// still counts before filtering and a cycle diagnostic never becomes JSON.
func TestStructuredDiagnosticsAndSelectionRemainIndependent(t *testing.T) {
	configureStructuredTest(t, "json")
	emitter.LogLevel = "off"
	oldStatistics := emitter.TagStatistics
	emitter.TagStatistics = true
	t.Cleanup(func() { emitter.TagStatistics = oldStatistics })
	previousCount := emitter.TagEvents("info")
	trexDecoder.DisableCycleErrors = false
	packet := structuredPacket(1, structuredWords(32, 7))
	packet[2] = 0x10
	output, diagnostics := runStructuredWire(t, id.TriceIDLookUp{1: {Type: "TRICE32_1", Strg: "info:{count}"}}, packet)
	assert.Empty(t, output)
	assert.Contains(t, diagnostics, "CYCLE_ERROR")
	assert.NotContains(t, diagnostics, `"fields"`)
	assert.Equal(t, previousCount+1, emitter.TagEvents("info"))
}

// structuredFailWriter injects a sink failure after a valid record is decoded.
type structuredFailWriter struct{ short bool }

// Write makes short writes and explicit errors independently observable.
func (w structuredFailWriter) Write(p []byte) (int, error) {
	if w.short {
		return len(p) - 1, nil
	}
	return 0, io.ErrClosedPipe
}

// TestStructuredWriterFailuresAreReturned prevents failed output from silently
// consuming further records or restarting an otherwise healthy input stream.
func TestStructuredWriterFailuresAreReturned(t *testing.T) {
	for _, short := range []bool{false, true} {
		t.Run(map[bool]string{false: "writer error", true: "short write"}[short], func(t *testing.T) {
			configureStructuredTest(t, "json")
			lut := id.TriceIDLookUp{1: {Type: "TRICE0", Strg: "info:ready"}}
			dec := trexDecoder.New(io.Discard, lut, new(sync.RWMutex), nil, bytes.NewReader(structuredPacket(1, nil)), decoder.LittleEndian)
			err := decodeAndComposeLoopOutput(structuredFailWriter{short}, io.Discard, emitter.New(io.Discard), dec, lut, nil, nil)
			want := io.ErrClosedPipe
			if short {
				want = io.ErrShortWrite
			}
			assert.True(t, errors.Is(err, want), "%v must propagate", want)
		})
	}
}

// TestStructuredBufferScope rejects named arrays while retaining ordinary
// buffer events as messages. The same validation applies to external TIL files.
func TestStructuredBufferScope(t *testing.T) {
	configureStructuredTest(t, "json")
	output, diagnostics := runStructuredWire(t, id.TriceIDLookUp{1: {Type: "trice8B", Strg: "msg:{bytes:%02x}"}}, structuredPacket(1, []byte{1, 2}))
	assert.Empty(t, output)
	assert.Contains(t, diagnostics, "does not support named structured fields")
	output, diagnostics = runStructuredWire(t, id.TriceIDLookUp{1: {Type: "trice8B", Strg: "msg:%02x "}}, structuredPacket(1, []byte{1, 2}))
	assert.Empty(t, diagnostics)
	assert.Equal(t, `{"tag":"msg","message":"01 02 "}`+"\n", output)
}

// TestStructuredMixedConversionsPreserveArgumentPositions checks that ordinary
// printf arguments consume positions without becoming user fields.
func TestStructuredMixedConversionsPreserveArgumentPositions(t *testing.T) {
	configureStructuredTest(t, "json")
	output, diagnostics := runStructuredWire(t, id.TriceIDLookUp{1: {Type: "TRICE32_4", Strg: "info:%u {temperature:%.1f} %x {last}"}}, structuredPacket(1, structuredWords(32, 17, uint64(math.Float32bits(2.25)), 0xab, 42)))
	assert.Empty(t, diagnostics)
	assert.Equal(t, `{"tag":"info","level":"INFO","message":"17 2.2 ab 42","fields":{"temperature":2.25,"last":42}}`+"\n", output)
}

// TestStructuredLongMessageIsNotTruncated checks a field width larger than the
// legacy text Read buffer while retaining its unpadded numeric field value.
func TestStructuredLongMessageIsNotTruncated(t *testing.T) {
	configureStructuredTest(t, "json")
	output, diagnostics := runStructuredWire(t, id.TriceIDLookUp{1: {Type: "TRICE32_1", Strg: "info:{value:%70000d}"}}, structuredPacket(1, structuredWords(32, 7)))
	assert.Empty(t, diagnostics)
	var record struct {
		Message string
		Fields  map[string]int
	}
	if assert.NoError(t, json.Unmarshal([]byte(output), &record)) {
		assert.Len(t, record.Message, 70000)
		assert.True(t, strings.HasSuffix(record.Message, "7"))
		assert.Equal(t, map[string]int{"value": 7}, record.Fields)
	}
}

// TestStructuredTextPresentation verifies the default human output derives
// printf formatting from the same canonical template without exposing names.
func TestStructuredTextPresentation(t *testing.T) {
	configureStructuredTest(t, "text")
	output := runTREXApplicationText(t, "TRICE32_2", `info:Motor {motor_id}: {temperature:%.1f C}, set={{1,2}}\n`, structuredWords(32, 7, uint64(math.Float32bits(1.25))), func() {
		emitter.ColorPalette = "none"
	})
	assert.Equal(t, "Motor 7: 1.2 C, set={1,2}\n", output)
}

// TestStructuredBigEndianAndFormattedTimestamp uses an actual stamped packet.
// Target byte order does not change field values or the selected stamp format.
func TestStructuredBigEndianAndFormattedTimestamp(t *testing.T) {
	configureStructuredTest(t, "json")
	decoder.TargetStamp32 = "time:%d ticks"
	decoder.ShowTargetStamp32Passed = true
	packet := binary.BigEndian.AppendUint16(nil, 0xc001)
	packet = binary.BigEndian.AppendUint32(packet, math.MaxUint32)
	packet = binary.BigEndian.AppendUint16(packet, 0x08c0)
	packet = binary.BigEndian.AppendUint64(packet, math.MaxUint64)
	lut := id.TriceIDLookUp{1: {Type: "TRICE64_1", Strg: "info:{value:%u}"}}
	var output, diagnostics bytes.Buffer
	dec := trexDecoder.New(&diagnostics, lut, new(sync.RWMutex), nil, bytes.NewReader(packet), decoder.BigEndian)
	assert.ErrorIs(t, decodeAndComposeLoopOutput(&output, &diagnostics, emitter.New(&output), dec, lut, nil, nil), io.EOF)
	assert.Empty(t, diagnostics.String())
	assert.Equal(t, `{"tag":"info","level":"INFO","message":"18446744073709551615","ts32":"4294967295 ticks","fields":{"value":18446744073709551615}}`+"\n", output.String())
}

// TestStructuredMalformedDictionaryNeverEmitsRecords checks that invalid
// external schemas are diagnostics, including schemas without named fields.
func TestStructuredMalformedDictionaryNeverEmitsRecords(t *testing.T) {
	for _, tt := range []struct{ name, triceType, template, diagnostic string }{
		{"unbalanced template", "TRICE8_1", "info:{value", "unclosed"},
		{"duplicate fields", "TRICE8_2", "info:{x} {x}", "duplicate"},
		{"floating field on byte payload", "TRICE8_1", "info:{x:%f}", "float"},
		{"legacy floating format on byte payload", "TRICE8_1", "info:%f", "Invalid format specifier"},
		{"string field cannot claim an integer type", "triceS", "info:{value:%x}", "string fields require %s"},
	} {
		t.Run(tt.name, func(t *testing.T) {
			configureStructuredTest(t, "json")
			payload := []byte{1}
			if tt.triceType == "TRICE8_2" {
				payload = append(payload, 2)
			}
			output, diagnostics := runStructuredWire(t, id.TriceIDLookUp{1: {Type: tt.triceType, Strg: tt.template}}, structuredPacket(1, payload))
			assert.Empty(t, output, "failed records must never appear as valid JSON")
			assert.Contains(t, diagnostics, tt.diagnostic)
		})
	}
}

// TestStructuredSelectionAndVisualization checks ordering with real decoding:
// filters precede visualization, and log=drop removes the complete machine event.
func TestStructuredSelectionAndVisualization(t *testing.T) {
	for _, tt := range []struct{ name, filter, ruleSuffix, wantLog, wantVis string }{
		{"keep", "", "", `{"tag":"info","level":"INFO","message":"7\n","fields":{"value":7}}` + "\n", "7\n"},
		{"visualization consumes event", "", ";log=drop", "", "7\n"},
		{"ban", "ban", "", "", ""},
		{"pick excludes event", "pick", "", "", ""},
		{"threshold excludes event", "level", "", "", ""},
	} {
		t.Run(tt.name, func(t *testing.T) {
			configureStructuredTest(t, "json")
			switch tt.filter {
			case "ban":
				emitter.Ban = []string{"info"}
			case "pick":
				emitter.Pick = []string{"err"}
			case "level":
				emitter.LogLevel = "fatal"
			}
			assert.NoError(t, emitter.ResolveFilterSelectors())
			lut := id.TriceIDLookUp{1: {Type: "TRICE32_1", Strg: `info:{value}\n`}}
			fs := &afero.Afero{Fs: afero.NewMemMapFs()}
			var output, diagnostics bytes.Buffer
			mutex := new(sync.RWMutex)
			router, err := vis.NewRouter(&diagnostics, fs, lut, mutex, []string{`info:printf("%d\n",v0)@vis.txt` + tt.ruleSuffix}, false)
			if !assert.NoError(t, err) {
				return
			}
			dec := trexDecoder.New(&diagnostics, lut, mutex, nil, bytes.NewReader(structuredPacket(1, structuredWords(32, 7))), decoder.LittleEndian)
			dec.(decoder.VisRecordController).SetVisRecordEnabled(true)
			assert.ErrorIs(t, decodeAndComposeLoopOutput(&output, &diagnostics, emitter.New(&output), dec, lut, nil, router), io.EOF)
			assert.NoError(t, router.Close())
			visualized, err := fs.ReadFile("vis.txt")
			assert.NoError(t, err)
			assert.Equal(t, tt.wantVis, string(visualized))
			assert.Equal(t, tt.wantLog, output.String())
			assert.Empty(t, diagnostics.String())
		})
	}
}

// TestStructuredLevelIsIndependentOfTagWeight prevents presentation and filter
// settings from changing the semantic severity in exported records.
func TestStructuredLevelIsIndependentOfTagWeight(t *testing.T) {
	configureStructuredTest(t, "json")
	oldTags, oldLabels := emitter.Tags, emitter.UserLabel
	t.Cleanup(func() { emitter.Tags, emitter.UserLabel = oldTags, oldLabels })
	emitter.UserLabel = []string{"info:50", "motor:999"}
	assert.NoError(t, emitter.AddUserLabels())
	for _, tt := range []struct{ tag, want string }{
		{"Info", `{"tag":"Info","level":"INFO","message":"ready"}`},
		{"motor", `{"tag":"motor","message":"ready"}`},
		{"WaRn", `{"tag":"untagged","level":"WARNING","message":"WaRn:ready"}`},
	} {
		t.Run(tt.tag, func(t *testing.T) {
			encoded, err := renderStructuredRecord(decoder.ApplicationRecord{Tag: tt.tag, Message: tt.tag + ":ready"}, nil, time.Time{}, &targetStampState{})
			assert.NoError(t, err)
			assert.Equal(t, tt.want+"\n", string(encoded))
		})
	}
}

// TestStructuredFormattedX0KeepsEmptyEvents checks the ID-less path independently
// of TIL decoding; empty application text must not be mistaken for ignored data.
func TestStructuredFormattedX0KeepsEmptyEvents(t *testing.T) {
	configureStructuredTest(t, "json")
	oldX0 := decoder.TypeX0
	t.Cleanup(func() { decoder.TypeX0 = oldX0 })
	decoder.TypeX0 = "counted:%s"
	decoder.ShowID = "%d"
	output, diagnostics := runStructuredWire(t, nil, []byte{0, 0, 0, 0})
	assert.Empty(t, diagnostics)
	assert.Equal(t, `{"tag":"untagged","message":""}`+"\n", output, "no ID or fields may be invented for X0")
	decoder.TypeX0 = "counted:ignore"
	output, diagnostics = runStructuredWire(t, nil, []byte{0, 0, 0, 0})
	assert.Empty(t, output)
	assert.Empty(t, diagnostics)
}

// TestStructuredClassicBuffersKeepCompleteMessages covers the field-free
// compatibility path for every buffer width, plus large repeated formatting and
// function-style logs that exceed the legacy text decoder's output buffer.
func TestStructuredClassicBuffersKeepCompleteMessages(t *testing.T) {
	for _, bits := range []int{8, 16, 32, 64} {
		t.Run(strconv.Itoa(bits)+" bit buffer", func(t *testing.T) {
			configureStructuredTest(t, "json")
			lut := id.TriceIDLookUp{1: {Type: "trice" + strconv.Itoa(bits) + "B", Strg: "msg:%70000d "}}
			output, diagnostics := runStructuredWire(t, lut, structuredPacket(1, structuredWords(bits, 1, 2)))
			assert.Empty(t, diagnostics)
			var record struct {
				Message string
				Fields  map[string]any
			}
			if assert.NoError(t, json.Unmarshal([]byte(output), &record)) {
				assert.Equal(t, strings.Repeat(" ", 69999)+"1 "+strings.Repeat(" ", 69999)+"2 ", record.Message)
				assert.Nil(t, record.Fields, "buffer elements must not invent named scalar fields")
			}
		})
	}
	t.Run("function-style buffer", func(t *testing.T) {
		configureStructuredTest(t, "json")
		prefix := strings.Repeat("x", 70000)
		output, diagnostics := runStructuredWire(t, id.TriceIDLookUp{1: {Type: "trice8F", Strg: "msg:" + prefix}}, structuredPacket(1, []byte{1, 2}))
		assert.Empty(t, diagnostics)
		var record struct{ Message string }
		if assert.NoError(t, json.Unmarshal([]byte(output), &record)) {
			assert.Equal(t, prefix+"(01)(02)\n", record.Message)
		}
	})
}
