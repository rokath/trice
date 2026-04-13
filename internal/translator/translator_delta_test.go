// SPDX-License-Identifier: MIT

package translator

import (
	"bytes"
	"io"
	"os"
	"os/exec"
	"runtime"
	"strings"
	"syscall"
	"testing"
	"time"

	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/internal/receiver"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

type scriptedDecoderStep struct {
	data            string
	err             error
	lastTriceID     id.TriceID
	targetTimestamp uint64
	targetStampSize int
}

type scriptedDecoder struct {
	steps []scriptedDecoderStep
	index int
}

func (d *scriptedDecoder) Read(buf []byte) (int, error) {
	if d.index >= len(d.steps) {
		return 0, io.EOF
	}
	step := d.steps[d.index]
	d.index++
	decoder.LastTriceID = step.lastTriceID
	decoder.TargetTimestamp = step.targetTimestamp
	decoder.TargetTimestampSize = step.targetStampSize
	return copy(buf, step.data), step.err
}

func (*scriptedDecoder) SetInput(io.Reader) {}

func configureTranslatorLoopTest(t *testing.T) {
	t.Helper()

	savedPort := receiver.Port
	savedHostStamp := emitter.HostStamp
	savedPrefix := emitter.Prefix
	savedSuffix := emitter.Suffix
	savedColorPalette := emitter.ColorPalette
	savedDisplayRemote := emitter.DisplayRemote
	savedTestTableMode := emitter.TestTableMode
	savedBan := emitter.Ban
	savedPick := emitter.Pick
	savedLIFnJSON := id.LIFnJSON
	savedShowID := decoder.ShowID
	savedTargetStamp := decoder.TargetStamp
	savedTargetStamp16 := decoder.TargetStamp16
	savedTargetStamp16Delta := decoder.TargetStamp16Delta
	savedTargetStamp0 := decoder.TargetStamp0
	savedTargetStamp0Delta := decoder.TargetStamp0Delta
	savedShowTargetStamp0Passed := decoder.ShowTargetStamp0Passed
	savedShowTargetStamp16Passed := decoder.ShowTargetStamp16Passed
	savedShowTargetStamp0DeltaPassed := decoder.ShowTargetStamp0DeltaPassed
	savedShowTargetStamp16DeltaPassed := decoder.ShowTargetStamp16DeltaPassed
	savedLocationFmt := decoder.LocationInformationFormatString
	savedLastTriceID := decoder.LastTriceID
	savedTargetTimestamp := decoder.TargetTimestamp
	savedTargetTimestampSize := decoder.TargetTimestampSize

	t.Cleanup(func() {
		receiver.Port = savedPort
		emitter.HostStamp = savedHostStamp
		emitter.Prefix = savedPrefix
		emitter.Suffix = savedSuffix
		emitter.ColorPalette = savedColorPalette
		emitter.DisplayRemote = savedDisplayRemote
		emitter.TestTableMode = savedTestTableMode
		emitter.Ban = savedBan
		emitter.Pick = savedPick
		id.LIFnJSON = savedLIFnJSON
		decoder.ShowID = savedShowID
		decoder.TargetStamp = savedTargetStamp
		decoder.TargetStamp16 = savedTargetStamp16
		decoder.TargetStamp16Delta = savedTargetStamp16Delta
		decoder.TargetStamp0 = savedTargetStamp0
		decoder.TargetStamp0Delta = savedTargetStamp0Delta
		decoder.ShowTargetStamp0Passed = savedShowTargetStamp0Passed
		decoder.ShowTargetStamp16Passed = savedShowTargetStamp16Passed
		decoder.ShowTargetStamp0DeltaPassed = savedShowTargetStamp0DeltaPassed
		decoder.ShowTargetStamp16DeltaPassed = savedShowTargetStamp16DeltaPassed
		decoder.LocationInformationFormatString = savedLocationFmt
		decoder.LastTriceID = savedLastTriceID
		decoder.TargetTimestamp = savedTargetTimestamp
		decoder.TargetTimestampSize = savedTargetTimestampSize
	})

	receiver.Port = "BUFFER"
	emitter.HostStamp = "off"
	emitter.Prefix = ""
	emitter.Suffix = ""
	emitter.ColorPalette = "none"
	emitter.DisplayRemote = false
	emitter.TestTableMode = true
	emitter.Ban = nil
	emitter.Pick = nil
	id.LIFnJSON = "off"
	decoder.ShowID = ""
	decoder.TargetStamp = "off"
	decoder.TargetStamp16 = ""
	decoder.TargetStamp16Delta = ""
	decoder.TargetStamp0 = ""
	decoder.TargetStamp0Delta = ""
	decoder.ShowTargetStamp0Passed = false
	decoder.ShowTargetStamp16Passed = false
	decoder.ShowTargetStamp0DeltaPassed = false
	decoder.ShowTargetStamp16DeltaPassed = false
	decoder.LocationInformationFormatString = "%s:%d "
	decoder.LastTriceID = 0
	decoder.TargetTimestamp = 0
	decoder.TargetTimestampSize = 0
}

// TestRenderTargetStampColumns16DeltaWraparound verifies the expected behavior.
func TestRenderTargetStampColumns16DeltaWraparound(t *testing.T) {
	saved16 := decoder.TargetStamp16
	saved16Delta := decoder.TargetStamp16Delta
	saved0Delta := decoder.TargetStamp0Delta
	defer func() {
		decoder.TargetStamp16 = saved16
		decoder.TargetStamp16Delta = saved16Delta
		decoder.TargetStamp0Delta = saved0Delta
	}()

	decoder.TargetStamp16 = "time:%04x "
	decoder.TargetStamp16Delta = "dt:%4d"
	decoder.TargetStamp0Delta = ""

	state := targetStampState{}

	gotFirst := renderTargetStamp(2, 65530) + "default: " + renderTargetDelta(2, 65530, &state) + "default: "
	wantFirst := "time:fffa default: dt:   -default: "
	if gotFirst != wantFirst {
		t.Fatalf("first delta mismatch:\nwant %q\ngot  %q", wantFirst, gotFirst)
	}

	gotSecond := renderTargetStamp(2, 4) + "default: " + renderTargetDelta(2, 4, &state) + "default: "
	wantSecond := "time:0004 default: dt:  10default: "
	if gotSecond != wantSecond {
		t.Fatalf("wrapped delta mismatch:\nwant %q\ngot  %q", wantSecond, gotSecond)
	}
}

// TestRenderBuiltinTargetDeltaUsesTimePrefix verifies the expected behavior.
func TestRenderBuiltinTargetDeltaUsesTimePrefix(t *testing.T) {
	saved16Delta := decoder.TargetStamp16Delta
	saved32Delta := decoder.TargetStamp32Delta
	saved0Delta := decoder.TargetStamp0Delta
	defer func() {
		decoder.TargetStamp16Delta = saved16Delta
		decoder.TargetStamp32Delta = saved32Delta
		decoder.TargetStamp0Delta = saved0Delta
	}()

	decoder.TargetStamp16Delta = "us"
	decoder.TargetStamp32Delta = "ms"
	decoder.TargetStamp0Delta = ""

	state16 := targetStampState{}
	got16First := renderTargetDelta(2, 10, &state16)
	if got16First != strings.Repeat(" ", len(strings.TrimPrefix("time:       0_000", "time:"))) {
		t.Fatalf("unexpected first builtin ts16delta placeholder: %q", got16First)
	}
	got16Second := renderTargetDelta(2, 106, &state16)
	if got16Second != "time:       0_096" {
		t.Fatalf("unexpected builtin ts16delta rendering: %q", got16Second)
	}

	state32 := targetStampState{}
	got32First := renderTargetDelta(4, 10, &state32)
	if got32First != strings.Repeat(" ", len(strings.TrimPrefix("time: 0:00:00,000", "time:"))) {
		t.Fatalf("unexpected first builtin ts32delta placeholder: %q", got32First)
	}
	got32Second := renderTargetDelta(4, 150, &state32)
	if got32Second != "time: 0:00:00,140" {
		t.Fatalf("unexpected builtin ts32delta rendering: %q", got32Second)
	}
}

// TestRenderTargetStampColumns32EpochAndDelta verifies the expected behavior.
func TestRenderTargetStampColumns32EpochAndDelta(t *testing.T) {
	saved32 := decoder.TargetStamp32
	saved32Delta := decoder.TargetStamp32Delta
	saved0Delta := decoder.TargetStamp0Delta
	defer func() {
		decoder.TargetStamp32 = saved32
		decoder.TargetStamp32Delta = saved32Delta
		decoder.TargetStamp0Delta = saved0Delta
	}()

	decoder.TargetStamp32 = "epoch2006-01-02 15:04:05 UTC "
	decoder.TargetStamp32Delta = "dt:%8d"
	decoder.TargetStamp0Delta = ""

	state := targetStampState{}
	ts0 := uint64(1700000000)
	gotFirst := renderTargetStamp(4, ts0) + "default: " + renderTargetDelta(4, ts0, &state) + "default: "
	wantFirst := time.Unix(int64(ts0), 0).UTC().Format("2006-01-02 15:04:05 UTC ") + "default: dt:       -default: "
	if gotFirst != wantFirst {
		t.Fatalf("first epoch delta mismatch:\nwant %q\ngot  %q", wantFirst, gotFirst)
	}

	gotSecond := renderTargetStamp(4, ts0+15) + "default: " + renderTargetDelta(4, ts0+15, &state) + "default: "
	wantSecond := time.Unix(int64(ts0+15), 0).UTC().Format("2006-01-02 15:04:05 UTC ") + "default: dt:      15default: "
	if gotSecond != wantSecond {
		t.Fatalf("second epoch delta mismatch:\nwant %q\ngot  %q", wantSecond, gotSecond)
	}
}

// TestRenderTargetDeltaFallsBackToCommonPlaceholder verifies the expected behavior.
func TestRenderTargetDeltaFallsBackToCommonPlaceholder(t *testing.T) {
	saved0Delta := decoder.TargetStamp0Delta
	saved16Delta := decoder.TargetStamp16Delta
	saved32Delta := decoder.TargetStamp32Delta
	defer func() {
		decoder.TargetStamp0Delta = saved0Delta
		decoder.TargetStamp16Delta = saved16Delta
		decoder.TargetStamp32Delta = saved32Delta
	}()

	decoder.TargetStamp0Delta = strings.Repeat(" ", len(strings.TrimPrefix("time:       0_000", "time:")))
	decoder.TargetStamp16Delta = "us"
	decoder.TargetStamp32Delta = ""

	state := targetStampState{}
	got := renderTargetDelta(4, 123, &state)
	if got != decoder.TargetStamp0Delta {
		t.Fatalf("expected placeholder delta for ts32 line, got %q", got)
	}
}

// TestRenderTargetDeltaComputesCommonPlaceholderWithoutTs0Delta verifies the expected behavior.
func TestRenderTargetDeltaComputesCommonPlaceholderWithoutTs0Delta(t *testing.T) {
	saved0Delta := decoder.TargetStamp0Delta
	saved16Delta := decoder.TargetStamp16Delta
	saved32Delta := decoder.TargetStamp32Delta
	saved16Passed := decoder.ShowTargetStamp16DeltaPassed
	saved32Passed := decoder.ShowTargetStamp32DeltaPassed
	defer func() {
		decoder.TargetStamp0Delta = saved0Delta
		decoder.TargetStamp16Delta = saved16Delta
		decoder.TargetStamp32Delta = saved32Delta
		decoder.ShowTargetStamp16DeltaPassed = saved16Passed
		decoder.ShowTargetStamp32DeltaPassed = saved32Passed
	}()

	decoder.TargetStamp0Delta = ""
	decoder.TargetStamp16Delta = "us"
	decoder.TargetStamp32Delta = ""
	decoder.ShowTargetStamp16DeltaPassed = false
	decoder.ShowTargetStamp32DeltaPassed = false

	state := targetStampState{}
	got := renderTargetDelta(4, 123, &state)
	want := strings.Repeat(" ", len(strings.TrimPrefix("time:       0_000", "time:")))
	if got != want {
		t.Fatalf("expected computed placeholder delta for ts32 line, got %q", got)
	}
}

// TestRenderTargetDeltaExplicitEmptyDisablesTs16Placeholder verifies the expected behavior.
func TestRenderTargetDeltaExplicitEmptyDisablesTs16Placeholder(t *testing.T) {
	saved16Delta := decoder.TargetStamp16Delta
	saved32Delta := decoder.TargetStamp32Delta
	saved16Passed := decoder.ShowTargetStamp16DeltaPassed
	saved32Passed := decoder.ShowTargetStamp32DeltaPassed
	defer func() {
		decoder.TargetStamp16Delta = saved16Delta
		decoder.TargetStamp32Delta = saved32Delta
		decoder.ShowTargetStamp16DeltaPassed = saved16Passed
		decoder.ShowTargetStamp32DeltaPassed = saved32Passed
	}()

	decoder.TargetStamp16Delta = ""
	decoder.TargetStamp32Delta = "dt:%5d"
	decoder.ShowTargetStamp16DeltaPassed = true
	decoder.ShowTargetStamp32DeltaPassed = true

	state := targetStampState{}
	if got := renderTargetDelta(2, 123, &state); got != "" {
		t.Fatalf("expected explicit empty ts16delta to suppress placeholder, got %q", got)
	}
}

// TestRenderTargetDeltaExplicitEmptyDisablesTs32Placeholder verifies the expected behavior.
func TestRenderTargetDeltaExplicitEmptyDisablesTs32Placeholder(t *testing.T) {
	saved16Delta := decoder.TargetStamp16Delta
	saved32Delta := decoder.TargetStamp32Delta
	saved16Passed := decoder.ShowTargetStamp16DeltaPassed
	saved32Passed := decoder.ShowTargetStamp32DeltaPassed
	defer func() {
		decoder.TargetStamp16Delta = saved16Delta
		decoder.TargetStamp32Delta = saved32Delta
		decoder.ShowTargetStamp16DeltaPassed = saved16Passed
		decoder.ShowTargetStamp32DeltaPassed = saved32Passed
	}()

	decoder.TargetStamp16Delta = "dt:%4d"
	decoder.TargetStamp32Delta = ""
	decoder.ShowTargetStamp16DeltaPassed = true
	decoder.ShowTargetStamp32DeltaPassed = true

	state := targetStampState{}
	if got := renderTargetDelta(4, 123, &state); got != "" {
		t.Fatalf("expected explicit empty ts32delta to suppress placeholder, got %q", got)
	}
}

// TestRenderTargetDeltaExplicitEmptyDisablesTs0Placeholder verifies the expected behavior.
func TestRenderTargetDeltaExplicitEmptyDisablesTs0Placeholder(t *testing.T) {
	saved0Delta := decoder.TargetStamp0Delta
	saved16Delta := decoder.TargetStamp16Delta
	saved0Passed := decoder.ShowTargetStamp0DeltaPassed
	defer func() {
		decoder.TargetStamp0Delta = saved0Delta
		decoder.TargetStamp16Delta = saved16Delta
		decoder.ShowTargetStamp0DeltaPassed = saved0Passed
	}()

	decoder.TargetStamp0Delta = ""
	decoder.TargetStamp16Delta = "dt:%4d"
	decoder.ShowTargetStamp0DeltaPassed = true

	state := targetStampState{}
	if got := renderTargetDelta(0, 0, &state); got != "" {
		t.Fatalf("expected explicit empty ts0delta to suppress placeholder, got %q", got)
	}
}

// TestTargetStampDisplayWidthUsesGenericTagPrefix verifies the expected behavior.
func TestTargetStampDisplayWidthUsesGenericTagPrefix(t *testing.T) {
	if got := targetStampDisplayWidth(4, "time:%12d"); got != 12 {
		t.Fatalf("unexpected display width for time tag: %d", got)
	}
	if got := targetStampDisplayWidth(4, "timestamp:%12d"); got != 12 {
		t.Fatalf("unexpected display width for timestamp tag: %d", got)
	}
	if got := targetStampDisplayWidth(4, "%12d"); got != 12 {
		t.Fatalf("unexpected display width without tag: %d", got)
	}
}

// TestTargetStampDisplayWidthKeepsUppercaseTagPrefix verifies the expected behavior.
func TestTargetStampDisplayWidthKeepsUppercaseTagPrefix(t *testing.T) {
	if got := targetStampDisplayWidth(4, "Time:%12d"); got != len("Time:")+12 {
		t.Fatalf("unexpected display width for uppercase tag: %d", got)
	}
	if got := targetStampDisplayWidth(4, "timeStamp:%12d"); got != len("timeStamp:")+12 {
		t.Fatalf("unexpected display width for mixed-case tag: %d", got)
	}
}

// TestAutoTargetStamp0Delta verifies the expected behavior.
func TestAutoTargetStamp0Delta(t *testing.T) {
	saved0Delta := decoder.TargetStamp0Delta
	saved16Delta := decoder.TargetStamp16Delta
	saved32Delta := decoder.TargetStamp32Delta
	saved0Passed := decoder.ShowTargetStamp0DeltaPassed
	defer func() {
		decoder.TargetStamp0Delta = saved0Delta
		decoder.TargetStamp16Delta = saved16Delta
		decoder.TargetStamp32Delta = saved32Delta
		decoder.ShowTargetStamp0DeltaPassed = saved0Passed
	}()

	decoder.ShowTargetStamp0DeltaPassed = false
	decoder.TargetStamp0Delta = ""
	decoder.TargetStamp16Delta = "dt:%4d"
	decoder.TargetStamp32Delta = ""
	autoTargetStamp0Delta()

	if decoder.TargetStamp0Delta != strings.Repeat(" ", len("   0")) {
		t.Fatalf("unexpected auto ts0delta width: %q", decoder.TargetStamp0Delta)
	}

	decoder.TargetStamp0Delta = ""
	decoder.TargetStamp16Delta = "us"
	decoder.TargetStamp32Delta = "ms"
	autoTargetStamp0Delta()

	if decoder.TargetStamp0Delta != strings.Repeat(" ", len(strings.TrimPrefix("time: 0:00:00,000", "time:"))) {
		t.Fatalf("unexpected auto ts0delta width for builtin formats: %q", decoder.TargetStamp0Delta)
	}

	decoder.TargetStamp0Delta = ""
	decoder.TargetStamp16Delta = "dt:%4d"
	decoder.TargetStamp32Delta = "dt:%5d"
	autoTargetStamp0Delta()

	if decoder.TargetStamp0Delta != strings.Repeat(" ", len("    0")) {
		t.Fatalf("ts0delta should use widest mismatched width, got %q", decoder.TargetStamp0Delta)
	}
}

// TestPrepareTargetStampFormats validates defaulting and invalid epoch delta combinations.
func TestPrepareTargetStampFormats(t *testing.T) {
	savedTargetStamp := decoder.TargetStamp
	saved16 := decoder.TargetStamp16
	saved32 := decoder.TargetStamp32
	saved0 := decoder.TargetStamp0
	saved16Delta := decoder.TargetStamp16Delta
	saved32Delta := decoder.TargetStamp32Delta
	saved16Passed := decoder.ShowTargetStamp16Passed
	saved32Passed := decoder.ShowTargetStamp32Passed
	saved0Passed := decoder.ShowTargetStamp0Passed
	defer func() {
		decoder.TargetStamp = savedTargetStamp
		decoder.TargetStamp16 = saved16
		decoder.TargetStamp32 = saved32
		decoder.TargetStamp0 = saved0
		decoder.TargetStamp16Delta = saved16Delta
		decoder.TargetStamp32Delta = saved32Delta
		decoder.ShowTargetStamp16Passed = saved16Passed
		decoder.ShowTargetStamp32Passed = saved32Passed
		decoder.ShowTargetStamp0Passed = saved0Passed
	}()

	decoder.TargetStamp = "ms"
	decoder.TargetStamp16 = ""
	decoder.TargetStamp32 = ""
	decoder.TargetStamp0 = ""
	decoder.TargetStamp16Delta = ""
	decoder.TargetStamp32Delta = ""
	decoder.ShowTargetStamp16Passed = false
	decoder.ShowTargetStamp32Passed = false
	decoder.ShowTargetStamp0Passed = false
	if err := prepareTargetStampFormats(); err != nil {
		t.Fatalf("unexpected error: %v", err)
	}
	if decoder.TargetStamp16 != "ms" || decoder.TargetStamp32 != "ms" || decoder.TargetStamp0 != DefaultTargetStamp0 {
		t.Fatalf("unexpected default formats: %q %q %q", decoder.TargetStamp0, decoder.TargetStamp16, decoder.TargetStamp32)
	}

	decoder.TargetStamp = "us"
	decoder.TargetStamp16Delta = "epoch"
	if err := prepareTargetStampFormats(); err == nil {
		t.Fatalf("expected invalid ts16delta error")
	}

	decoder.TargetStamp16Delta = ""
	decoder.TargetStamp32Delta = "epoch+1"
	if err := prepareTargetStampFormats(); err == nil {
		t.Fatalf("expected invalid ts32delta error")
	}
}

// TestCorrectWrappedTimestamp verifies plausible timestamps are corrected into a future epoch window.
func TestCorrectWrappedTimestamp(t *testing.T) {
	got := correctWrappedTimestamp(1)
	if got.Year() <= 2038 {
		t.Fatalf("expected wrapped timestamp beyond 2038, got %v", got)
	}

	plain := correctWrappedTimestamp(uint32(time.Date(2024, 1, 2, 3, 4, 5, 0, time.UTC).Unix()))
	if plain.Year() != 2024 {
		t.Fatalf("expected unchanged plausible timestamp, got %v", plain)
	}
}

// TestLocationInformation verifies lookup, fallback, and verbose-disabled behavior.
func TestLocationInformation(t *testing.T) {
	savedFormat := decoder.LocationInformationFormatString
	savedVerbose := Verbose
	defer func() {
		decoder.LocationInformationFormatString = savedFormat
		Verbose = savedVerbose
	}()

	decoder.LocationInformationFormatString = "%s:%d "
	Verbose = false
	li := id.TriceIDLookUpLI{
		17: {File: "/tmp/demo/main.c", Line: 42},
	}

	if got := locationInformation(17, li); got != "main.c:42 " {
		t.Fatalf("unexpected location info %q", got)
	}
	if got := locationInformation(18, li); got != ":0 " {
		t.Fatalf("unexpected missing location fallback %q", got)
	}

	decoder.LocationInformationFormatString = "off"
	Verbose = true
	if got := locationInformation(17, li); got != "no li" {
		t.Fatalf("unexpected verbose fallback %q", got)
	}
}

// TestFormatTargetDeltaValueBuiltinFormats verifies the expected behavior.
func TestFormatTargetDeltaValueBuiltinFormats(t *testing.T) {
	assert.Equal(t, " 1:02:03,004", formatTargetDeltaValue(4, "ms", 3723004))
	assert.Equal(t, "   4,005_006", formatTargetDeltaValue(4, "us", 4005006))
	assert.Equal(t, "       7,008", formatTargetDeltaValue(2, "ms", 7008))
	assert.Equal(t, "       9_010", formatTargetDeltaValue(2, "us", 9010))
	assert.Equal(t, "raw", formatTargetDeltaValue(0, "raw", 123))
	assert.Equal(t, "delta:11", formatTargetDeltaValue(4, "delta:%d", 11))
}

// TestSplitSingleFormatDirective verifies the expected behavior.
func TestSplitSingleFormatDirective(t *testing.T) {
	prefix, width, leftAlign, suffix, ok := splitSingleFormatDirective("dt:%-6d us")
	assert.True(t, ok)
	assert.Equal(t, "dt:", prefix)
	assert.Equal(t, 6, width)
	assert.True(t, leftAlign)
	assert.Equal(t, " us", suffix)

	prefix, width, leftAlign, suffix, ok = splitSingleFormatDirective("literal %% %04x done")
	assert.True(t, ok)
	assert.Equal(t, "literal %% ", prefix)
	assert.Equal(t, 4, width)
	assert.False(t, leftAlign)
	assert.Equal(t, " done", suffix)

	_, _, _, _, ok = splitSingleFormatDirective("missing-%")
	assert.False(t, ok)

	_, _, _, _, ok = splitSingleFormatDirective("unsupported:%s")
	assert.False(t, ok)
}

// TestFormatMissingTargetDelta verifies the expected behavior.
func TestFormatMissingTargetDelta(t *testing.T) {
	assert.Equal(t, "dt:     - us", formatMissingTargetDelta(4, "dt:%6d us"))
	assert.Equal(t, "dt:-       us", formatMissingTargetDelta(4, "dt:%-7d us"))

	got := formatMissingTargetDelta(2, "us")
	want := strings.Repeat(" ", targetStampDisplayWidth(2, "us"))
	assert.Equal(t, want, got)
}

// TestTargetStampWidthAndFormatFallbacks verifies direct width helpers and fallback formatting.
func TestTargetStampWidthAndFormatFallbacks(t *testing.T) {
	assert.Equal(t, len("ts:0007"), targetStampWidth(4, "ts:%04d"))
	assert.Equal(t, "stamp:7", formatTargetStamp(4, "stamp:%d", 7))
	assert.Equal(t, "stamp:7", formatTargetStamp(2, "stamp:%d", 7))
	assert.Equal(t, "marker", formatTargetStamp(0, "marker", 99))
	assert.Equal(t, "", formatTargetStamp(99, "stamp:%d", 7))
	assert.Equal(t, "", renderTargetStamp(99, 7))

	abs, delta := targetStampFormats(99)
	assert.Equal(t, "", abs)
	assert.Equal(t, "", delta)
	assert.False(t, targetDeltaExplicitlyDisabled(99))
}

// TestDecodeAndComposeLoopPrependsMetadataOncePerLine verifies that line-start metadata is emitted only once.
func TestDecodeAndComposeLoopPrependsMetadataOncePerLine(t *testing.T) {
	configureTranslatorLoopTest(t)

	id.LIFnJSON = "on"
	decoder.ShowID = "ID:%d "
	decoder.TargetStamp = "off"
	decoder.TargetStamp16 = "TS:%d "
	decoder.TargetStamp16Delta = "DT:%d "
	decoder.ShowTargetStamp16Passed = true
	decoder.ShowTargetStamp16DeltaPassed = true

	li := id.TriceIDLookUpLI{
		17: {File: "/tmp/demo/main.c", Line: 42},
	}
	dec := &scriptedDecoder{
		steps: []scriptedDecoderStep{
			{data: "message", lastTriceID: 17, targetTimestamp: 1234, targetStampSize: 2},
			{data: " end\n", lastTriceID: 99, targetTimestamp: 5678, targetStampSize: 2},
		},
	}
	var out bytes.Buffer
	sw := emitter.New(&out)

	err := decodeAndComposeLoop(&out, sw, dec, id.TriceIDLookUp{}, li)
	require.ErrorIs(t, err, io.EOF)
	assert.Equal(t, "main.c:42 TS:1234  DT:-  ID:17  message end\n", out.String())
}

// TestDecodeAndComposeLoopFlushesPartialBufferLine verifies that buffered ports flush an unfinished line on EOF.
func TestDecodeAndComposeLoopFlushesPartialBufferLine(t *testing.T) {
	configureTranslatorLoopTest(t)

	dec := &scriptedDecoder{
		steps: []scriptedDecoderStep{
			{data: "partial", lastTriceID: 3},
		},
	}
	var out bytes.Buffer
	sw := emitter.New(&out)

	err := decodeAndComposeLoop(io.Discard, sw, dec, nil, nil)
	require.ErrorIs(t, err, io.EOF)
	assert.Equal(t, "partial\n", out.String())
}

// TestDecodeAndComposeLoopHonorsBanFilter verifies that filtered lines are suppressed before metadata is written.
func TestDecodeAndComposeLoopHonorsBanFilter(t *testing.T) {
	configureTranslatorLoopTest(t)

	emitter.Ban = []string{"dbg"}
	dec := &scriptedDecoder{
		steps: []scriptedDecoderStep{
			{data: "dbg:hidden\n", lastTriceID: 23},
		},
	}
	var out bytes.Buffer
	sw := emitter.New(&out)

	err := decodeAndComposeLoop(io.Discard, sw, dec, nil, nil)
	require.ErrorIs(t, err, io.EOF)
	assert.Empty(t, out.String())
}

// TestFormatTargetStampEpochAndBuiltinVariants verifies additional built-in timestamp branches.
func TestFormatTargetStampEpochAndBuiltinVariants(t *testing.T) {
	assert.Equal(t, "", formatTargetStamp32("", 123))
	assert.Equal(t, "time:   4,005_006", formatTargetStamp32("us", 4005006))
	assert.Equal(t, "time:       7,008", formatTargetStamp16("ms", 7008))
	assert.Equal(t, "time:       9_010", formatTargetStamp16("us", 9010))

	ts := uint64(1700000000)
	assert.Equal(t,
		time.Unix(int64(ts), 0).UTC().Format("2006-01-02 15:04:05 UTC"),
		formatTargetStamp32("epoch", ts),
	)
	assert.Equal(t,
		time.Unix(int64(ts), 0).UTC().Format("15:04"),
		formatTargetStamp32("epoch15:04", ts),
	)
}

// TestTranslateInvalidEndiannessExits verifies the early fatal path in Translate.
func TestTranslateInvalidEndiannessExits(t *testing.T) {
	if os.Getenv("TRICE_TRANSLATE_INVALID_ENDIANNESS") == "1" {
		TriceEndianness = "invalid"
		Translate(io.Discard, nil, nil, nil, nil, nil)
		return
	}

	cmd := exec.Command(os.Args[0], "-test.run=TestTranslateInvalidEndiannessExits")
	cmd.Env = append(os.Environ(), "TRICE_TRANSLATE_INVALID_ENDIANNESS=1")
	err := cmd.Run()
	require.Error(t, err)

	exitErr, ok := err.(*exec.ExitError)
	require.True(t, ok)
	assert.NotZero(t, exitErr.ExitCode())
}

type closeRecorder struct {
	closed bool
}

func (c *closeRecorder) Read([]byte) (int, error) { return 0, io.EOF }

func (c *closeRecorder) Close() error {
	c.closed = true
	return nil
}

// TestHandleSIGTERMExitsAndCloses verifies the shutdown path in a subprocess.
func TestHandleSIGTERMExitsAndCloses(t *testing.T) {
	if runtime.GOOS == "windows" {
		t.Skip("syscall.SIGTERM subprocess signaling is not supported on Windows")
	}

	if os.Getenv("TRICE_HANDLE_SIGTERM") == "1" {
		Verbose = true
		handleSIGTERM(io.Discard, &closeRecorder{})
		return
	}

	cmd := exec.Command(os.Args[0], "-test.run=TestHandleSIGTERMExitsAndCloses")
	cmd.Env = append(os.Environ(), "TRICE_HANDLE_SIGTERM=1")
	require.NoError(t, cmd.Start())
	time.Sleep(150 * time.Millisecond)
	require.NoError(t, cmd.Process.Signal(syscall.SIGTERM))
	require.NoError(t, cmd.Wait())
}
