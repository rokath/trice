// SPDX-License-Identifier: MIT

package translator

import (
	"bytes"
	"fmt"
	"io"
	"os"
	"os/exec"
	"runtime"
	"strings"
	"sync"
	"syscall"
	"testing"
	"time"

	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/internal/receiver"
	"github.com/rokath/trice/internal/trexDecoder"
	"github.com/rokath/trice/internal/vis"
	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

type scriptedDecoderStep struct {
	data            string
	err             error
	lastTriceID     id.TriceID
	targetTimestamp uint64
	targetStampSize int
	blankMetadata   bool
}

type scriptedDecoder struct {
	steps []scriptedDecoderStep
	index int
}

// classifiedScriptedDecoder supplies a format-string tag for each scripted
// call, so selection tests exercise the same typed boundary as TREX decoding.
type classifiedScriptedDecoder struct {
	scriptedDecoder
	tags []string
	span []decoder.OutputSpan
}

// Read retains the tag of the current call without inferring it from payload.
func (d *classifiedScriptedDecoder) Read(buf []byte) (int, error) {
	index := d.scriptedDecoder.index
	n, err := d.scriptedDecoder.Read(buf)
	d.span = nil
	if n > 0 {
		d.span = []decoder.OutputSpan{{Start: 0, End: n, Kind: decoder.OutputApplication, Tag: d.tags[index]}}
	}
	return n, err
}

// DecodedOutputSpans returns the boundary of the latest scripted call.
func (d *classifiedScriptedDecoder) DecodedOutputSpans() []decoder.OutputSpan { return d.span }

func (d *scriptedDecoder) Read(buf []byte) (int, error) {
	if d.index >= len(d.steps) {
		return 0, io.EOF
	}
	step := d.steps[d.index]
	d.index++
	decoder.LastTriceID = step.lastTriceID
	decoder.TargetTimestamp = step.targetTimestamp
	decoder.TargetTimestampSize = step.targetStampSize
	decoder.BlankMetadata = step.blankMetadata
	return copy(buf, step.data), step.err
}

func (*scriptedDecoder) SetInput(io.Reader) {}

// visScriptedDecoder adds the optional typed-record capability to deterministic translator-loop input.
type visScriptedDecoder struct {
	scriptedDecoder
	records   []decoder.VisRecord
	current   decoder.VisRecord
	available bool
	enabled   bool
	span      []decoder.OutputSpan
}

// Read returns one formatted message and retains the corresponding test record for the same iteration.
func (d *visScriptedDecoder) Read(buffer []byte) (int, error) {
	recordIndex := d.scriptedDecoder.index
	count, err := d.scriptedDecoder.Read(buffer)
	d.available = false
	d.span = nil
	if d.enabled && count > 0 && recordIndex < len(d.records) {
		d.current = d.records[recordIndex]
		d.available = true
		tag, _, _ := strings.Cut(d.current.Format, ":")
		d.span = []decoder.OutputSpan{{Start: 0, End: count, Kind: decoder.OutputApplication, Tag: tag}}
	}
	return count, err
}

// DecodedOutputSpans makes the visualization record subject to the same
// event-wide selection as production decoders.
func (d *visScriptedDecoder) DecodedOutputSpans() []decoder.OutputSpan { return d.span }

// SetVisRecordEnabled implements decoder.VisRecordController for production-equivalent enable behavior.
func (d *visScriptedDecoder) SetVisRecordEnabled(enabled bool) {
	d.enabled = enabled
	if !enabled {
		d.available = false
	}
}

// VisRecord returns only the typed record belonging to the most recent scripted Read.
func (d *visScriptedDecoder) VisRecord() (decoder.VisRecord, bool) {
	return d.current, d.available
}

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
	savedLogLevel := emitter.LogLevel
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
	savedBlankMetadata := decoder.BlankMetadata
	savedLIMaxDirs := id.LIMaxDirs

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
		emitter.LogLevel = savedLogLevel
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
		decoder.BlankMetadata = savedBlankMetadata
		id.LIMaxDirs = savedLIMaxDirs
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
	emitter.LogLevel = "all"
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
	decoder.BlankMetadata = false
	id.LIMaxDirs = 0
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
	savedMaxDirs := id.LIMaxDirs
	defer func() {
		decoder.LocationInformationFormatString = savedFormat
		Verbose = savedVerbose
		id.LIMaxDirs = savedMaxDirs
	}()

	decoder.LocationInformationFormatString = "%s:%d "
	Verbose = false
	id.LIMaxDirs = 0
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

	err := decodeAndComposeLoop(&out, sw, dec, id.TriceIDLookUp{}, li, nil)
	require.ErrorIs(t, err, io.EOF)
	assert.Equal(t, "main.c:42 TS:1234  DT:-  ID:17  message end\n", out.String())
}

// TestDecodeAndComposeLoopBlanksMetadataWhenRequested verifies aligned metadata suppression for X0 output.
func TestDecodeAndComposeLoopBlanksMetadataWhenRequested(t *testing.T) {
	configureTranslatorLoopTest(t)

	id.LIFnJSON = "on"
	decoder.ShowID = "deb:%5d"
	decoder.TargetStamp0 = "TS:      "
	decoder.ShowTargetStamp0Passed = true
	decoder.LocationInformationFormatString = decoder.LiFmtDefault

	li := id.TriceIDLookUpLI{
		17: {File: "/tmp/demo/main.c", Line: 42},
	}
	dec := &scriptedDecoder{
		steps: []scriptedDecoderStep{
			{data: "x0\n", lastTriceID: 17, targetStampSize: 0, blankMetadata: true},
		},
	}
	var out bytes.Buffer
	sw := emitter.New(&out)

	err := decodeAndComposeLoop(&out, sw, dec, id.TriceIDLookUp{}, li, nil)
	require.ErrorIs(t, err, io.EOF)
	want := strings.Repeat(" ", visibleMetadataWidth(locationInformation(17, li))) +
		"TS:       " +
		strings.Repeat(" ", visibleMetadataWidth("deb:   17")) +
		" x0\n"
	assert.Equal(t, want, out.String())
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

	err := decodeAndComposeLoop(io.Discard, sw, dec, nil, nil, nil)
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

	err := decodeAndComposeLoop(io.Discard, sw, dec, nil, nil, nil)
	require.ErrorIs(t, err, io.EOF)
	assert.Empty(t, out.String())
}

// runTREXApplicationText decodes one unstamped type-S0 event with ID 1. The
// format template, rather than the formatted payload, supplies its tag candidate.
func runTREXApplicationText(t *testing.T, triceType, format string, payload []byte, configure func()) string {
	t.Helper()
	configureTranslatorLoopTest(t)
	oldFraming := decoder.PackageFraming
	oldInitialCycle := decoder.InitialCycle
	oldDisableCycleErrors := trexDecoder.DisableCycleErrors
	t.Cleanup(func() {
		decoder.PackageFraming = oldFraming
		decoder.InitialCycle = oldInitialCycle
		trexDecoder.DisableCycleErrors = oldDisableCycleErrors
	})
	decoder.PackageFraming = "none"
	decoder.InitialCycle = true
	trexDecoder.DisableCycleErrors = false
	if configure != nil {
		configure()
	}

	input := []byte{0x01, 0x40, 0xc0, byte(len(payload))}
	input = append(input, payload...)
	lut := id.TriceIDLookUp{1: {Type: triceType, Strg: format}}
	var output bytes.Buffer
	dec := trexDecoder.New(&output, lut, new(sync.RWMutex), nil, bytes.NewReader(input), decoder.LittleEndian)
	sw := emitter.New(&output)

	err := decodeAndComposeLoop(&output, sw, dec, lut, nil, nil)
	require.ErrorIs(t, err, io.EOF)
	return output.String()
}

// TestTREXApplicationTagNormalization verifies host-only untagged assignment,
// presentation modes, and ordinary Pick/Ban behavior.
func TestTREXApplicationTagNormalization(t *testing.T) {
	tests := []struct {
		name      string
		format    string
		palette   string
		configure func()
		want      string
	}{
		{name: "missing tag", format: `Hello\n`, palette: "none", want: "Hello\n"},
		{name: "unknown tag", format: `mgs:blah\n`, palette: "none", want: "mgs:blah\n"},
		{name: "empty prefix", format: `:blah\n`, palette: "none", want: ":blah\n"},
		{name: "normal text colon", format: `12:34\n`, palette: "none", want: "12:34\n"},
		{name: "explicit untagged", format: `untagged:Hello\n`, palette: "none", want: "Hello\n"},
		{name: "known tag", format: `msg:Hello\n`, palette: "none", want: "Hello\n"},
		{name: "off shows synthetic prefix", format: `mgs:blah\n`, palette: "off", want: "untagged:mgs:blah\n"},
		{name: "off shows explicit prefix once", format: `untagged:Hello\n`, palette: "off", want: "untagged:Hello\n"},
		{name: "prefix and suffix stay metadata", format: `Hello\n`, palette: "off", configure: func() {
			emitter.Prefix = "host:"
			emitter.Suffix = ":tail"
		}, want: "host:untagged:Hello:tail\n"},
		{name: "pick untagged", format: `mgs:blah\n`, palette: "off", configure: func() {
			emitter.Pick = []string{"untagged"}
		}, want: "untagged:mgs:blah\n"},
		{name: "ban untagged", format: `mgs:blah\n`, palette: "off", configure: func() {
			emitter.Ban = []string{"untagged"}
		}, want: ""},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got := runTREXApplicationText(t, "TRICE_0", tt.format, nil, func() {
				emitter.ColorPalette = tt.palette
				if tt.configure != nil {
					tt.configure()
				}
			})
			assert.Equal(t, tt.want, got)
		})
	}
}

// TestTREXUsesTemplateInsteadOfRuntimeValueForTag verifies that a dynamic
// string beginning with a known tag cannot change an untagged template's group.
func TestTREXUsesTemplateInsteadOfRuntimeValueForTag(t *testing.T) {
	got := runTREXApplicationText(t, "TRICE_S", `%s\n`, []byte("err:runtime"), func() {
		emitter.ColorPalette = "off"
		emitter.Pick = []string{"untagged"}
	})
	assert.Equal(t, "untagged:err:runtime\n", got)
}

// runTREXPartialCalls sends one ID-only record per format template. Each record
// models a distinct Trice call, so line joining cannot hide an event boundary.
func runTREXPartialCalls(t *testing.T, formats []string, addNewline bool, configure func()) string {
	t.Helper()
	configureTranslatorLoopTest(t)
	oldFraming := decoder.PackageFraming
	oldInitialCycle := decoder.InitialCycle
	oldAddNewline := trexDecoder.AddNewlineToEachTriceMessage
	oldDoubledID := trexDecoder.Doubled16BitID
	oldSingleFraming := trexDecoder.SingleFraming
	oldDisableCycleErrors := trexDecoder.DisableCycleErrors
	oldNewlineIndent := decoder.NewlineIndent
	t.Cleanup(func() {
		decoder.PackageFraming = oldFraming
		decoder.InitialCycle = oldInitialCycle
		trexDecoder.AddNewlineToEachTriceMessage = oldAddNewline
		trexDecoder.Doubled16BitID = oldDoubledID
		trexDecoder.SingleFraming = oldSingleFraming
		trexDecoder.DisableCycleErrors = oldDisableCycleErrors
		decoder.NewlineIndent = oldNewlineIndent
	})
	decoder.PackageFraming = "none"
	decoder.InitialCycle = true
	trexDecoder.AddNewlineToEachTriceMessage = addNewline
	trexDecoder.Doubled16BitID = false
	trexDecoder.SingleFraming = false
	trexDecoder.DisableCycleErrors = false
	decoder.NewlineIndent = -1
	if configure != nil {
		configure()
	}

	lut := make(id.TriceIDLookUp, len(formats))
	input := make([]byte, 0, len(formats)*4)
	for i, format := range formats {
		// An unstamped S0 record stores the ID in the first two bytes, a
		// sequential cycle marker in the third, and a zero payload size.
		triceID := id.TriceID(i + 1)
		lut[triceID] = id.TriceFmt{Type: "TRICE_0", Strg: format}
		input = append(input, byte(triceID), 0x40, 0xc0+byte(i), 0)
	}
	var out bytes.Buffer
	dec := trexDecoder.New(&out, lut, new(sync.RWMutex), nil, bytes.NewReader(input), decoder.LittleEndian)
	sw := emitter.New(&out)
	require.ErrorIs(t, decodeAndComposeLoop(&out, sw, dec, lut, nil, nil), io.EOF)
	return out.String()
}

// TestPartialCallsWithPickRecordsCurrentLineJoining captures today's early
// Pick decision. M13 can change the expected output when selection moves to
// complete events, without losing these boundary cases.
func TestPartialCallsWithPickRecordsCurrentLineJoining(t *testing.T) {
	// The decoder currently indents continuation lines by 13 spaces when
	// no metadata columns request additional width.
	continuationIndent := strings.Repeat(" ", 13)
	tests := []struct {
		name       string
		formats    []string
		pick       []string
		ban        []string
		addNewline bool
		want       string
	}{
		{name: "accepted A rejected B newline accepted C", formats: []string{`msg:A`, `dbg:B\n`, `msg:C\n`}, pick: []string{"msg"}, want: "AC\n"},
		{name: "ban removes only B and its newline", formats: []string{`msg:A`, `dbg:B\n`, `msg:C\n`}, ban: []string{"dbg"}, want: "AC\n"},
		{name: "rejected first call leaves no prefix", formats: []string{`dbg:A`, `msg:B\n`}, pick: []string{"msg"}, want: "B\n"},
		{name: "rejected last newline leaves an open line for EOF", formats: []string{`msg:A`, `dbg:B\n`}, pick: []string{"msg"}, want: "A\n"},
		{name: "rejected newline does not end accepted line", formats: []string{`msg:A`, `dbg:\n`, `msg:C\n`}, pick: []string{"msg"}, want: "AC\n"},
		{name: "accepted multiline call emits both lines with current continuation indent", formats: []string{`msg:A\nB\n`}, pick: []string{"msg"}, want: "A\n" + continuationIndent + "B\n"},
		{name: "rejected multiline call emits neither line", formats: []string{`dbg:A\nB\n`, `msg:C\n`}, pick: []string{"msg"}, want: "C\n"},
		{name: "accepted newline-only call emits an empty line", formats: []string{`msg:\n`}, pick: []string{"msg"}, want: "\n"},
		{name: "empty call emits no line", formats: []string{``}, pick: []string{"untagged"}, want: ""},
		{name: "addNL gives an empty call a newline", formats: []string{``}, pick: []string{"untagged"}, addNewline: true, want: "\n"},
		{name: "all rejected calls emit nothing", formats: []string{`dbg:A`, `dbg:B\n`}, pick: []string{"msg"}, want: ""},
		{name: "accepted open line is flushed at EOF", formats: []string{`msg:A`}, pick: []string{"msg"}, want: "A\n"},
		{name: "addNL ends each accepted call", formats: []string{`msg:A`, `dbg:B`, `msg:C`}, pick: []string{"msg"}, addNewline: true, want: "A\nC\n"},
		{name: "addNL currently adds another indented line after an existing newline", formats: []string{`msg:A\n`}, pick: []string{"msg"}, addNewline: true, want: "A\n" + continuationIndent + "\n"},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got := runTREXPartialCalls(t, tt.formats, tt.addNewline, func() {
				emitter.Pick = tt.pick
				emitter.Ban = tt.ban
			})
			assert.Equal(t, tt.want, got)
		})
	}
}

// TestPartialCallsWithLevelUsesEventDecisions verifies that a rejected call
// contributes neither its text nor its newline, including later lines.
func TestPartialCallsWithLevelUsesEventDecisions(t *testing.T) {
	tests := []struct {
		name    string
		formats []string
		want    string
	}{
		{name: "rejected middle call leaves the accepted line open", formats: []string{`msg:A`, `dbg:B\n`, `msg:C\n`}, want: "AC\n"},
		{name: "rejected multiline call contributes no continuation", formats: []string{`dbg:A\nB\n`}, want: ""},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got := runTREXPartialCalls(t, tt.formats, false, func() {
				emitter.LogLevel = "info"
			})
			assert.Equal(t, tt.want, got)
		})
	}
}

// TestEncodedCallsCombinePickAndLevel verifies the documented selector example
// with actual TREX event boundaries: picking two groups does not bypass the
// higher error threshold, even when a rejected warning ends a partial line.
func TestEncodedCallsCombinePickAndLevel(t *testing.T) {
	got := runTREXPartialCalls(t, []string{`err:A`, `wrn:B\n`, `err:C\n`, `msg:D\n`}, false, func() {
		emitter.Pick = []string{"err", "wrn"}
		emitter.LogLevel = "err"
	})
	assert.Equal(t, "AC\n", got)
}

// TestDecodedStatisticsPrecedeSelection exercises actual TREX calls across
// palettes. Three IDs and tag groups are recorded even though one warning is
// visible; formatting the ID report again cannot inflate the tag total.
func TestDecodedStatisticsPrecedeSelection(t *testing.T) {
	for _, palette := range []string{"off", "none", "default"} {
		t.Run(palette, func(t *testing.T) {
			savedTags := emitter.Tags
			savedUserLabel := emitter.UserLabel
			savedTagStatistics := emitter.TagStatistics
			savedAllStatistics := emitter.AllStatistics
			savedTriceStatistics := decoder.TriceStatistics
			savedIDStat := decoder.IDStat
			savedIDLUT := decoder.IDLUT
			savedLILUT := decoder.LILUT
			t.Cleanup(func() {
				emitter.Tags = savedTags
				emitter.UserLabel = savedUserLabel
				emitter.TagStatistics = savedTagStatistics
				emitter.AllStatistics = savedAllStatistics
				decoder.TriceStatistics = savedTriceStatistics
				decoder.IDStat = savedIDStat
				decoder.IDLUT = savedIDLUT
				decoder.LILUT = savedLILUT
			})

			output := runTREXPartialCalls(t, []string{`wrn:A\n`, `dbg:B\n`, `mgs:C\n`}, false, func() {
				emitter.UserLabel = nil
				require.NoError(t, emitter.AddUserLabels())
				emitter.ColorPalette = palette
				if palette != "off" {
					emitter.HostStamp = "zero"
					decoder.ShowID = "ID:%d "
				}
				emitter.TagStatistics = true
				emitter.AllStatistics = false
				decoder.TriceStatistics = true
				decoder.IDStat = make(map[id.TriceID]int)
				emitter.Pick = []string{"wrn"}
				emitter.LogLevel = "wrn"
			})
			assert.Contains(t, output, "A")
			assert.NotContains(t, output, "B")
			assert.NotContains(t, output, "C")
			assert.Equal(t, 1, strings.Count(output, "\n"), "only one event is visible")
			if palette != "off" {
				assert.Contains(t, output, "2006-01-02_1504-05", "host metadata is preserved")
				assert.Contains(t, output, "ID:1", "only the selected event contributes an ID column")
				assert.NotContains(t, output, "ID:2")
			}
			assert.Equal(t, 1, emitter.TagEvents("wrn"))
			assert.Equal(t, 1, emitter.TagEvents("dbg"))
			assert.Equal(t, 1, emitter.TagEvents("untagged"))
			assert.Equal(t, map[id.TriceID]int{1: 1, 2: 1, 3: 1}, decoder.IDStat)

			decoder.IDLUT = id.TriceIDLookUp{
				1: {Type: "TRICE_0", Strg: `wrn:A\n`},
				2: {Type: "TRICE_0", Strg: `dbg:B\n`},
				3: {Type: "TRICE_0", Strg: `mgs:C\n`},
			}
			var report bytes.Buffer
			decoder.PrintTriceStatistics(&report)
			decoder.PrintTriceStatistics(&report)
			emitter.PrintTagStatistics(&report)
			assert.Contains(t, report.String(), "3 Trice messsges")
			assert.Equal(t, 1, emitter.TagEvents("wrn"), "repeated reports do not count display text")
			assert.Equal(t, 1, emitter.TagEvents("dbg"))
			assert.Equal(t, 1, emitter.TagEvents("untagged"))
		})
	}
}

// TestTagStatisticsFollowCallsNotOutputLines checks both directions of the
// mismatch: two Trice calls can share one line, while one call can span two.
func TestTagStatisticsFollowCallsNotOutputLines(t *testing.T) {
	savedTags := emitter.Tags
	savedUserLabel := emitter.UserLabel
	savedTagStatistics := emitter.TagStatistics
	savedAllStatistics := emitter.AllStatistics
	t.Cleanup(func() {
		emitter.Tags = savedTags
		emitter.UserLabel = savedUserLabel
		emitter.TagStatistics = savedTagStatistics
		emitter.AllStatistics = savedAllStatistics
	})
	output := runTREXPartialCalls(t, []string{`wrn:A`, `wrn:B\n`, `wrn:C\nD\n`}, false, func() {
		emitter.UserLabel = nil
		require.NoError(t, emitter.AddUserLabels())
		emitter.TagStatistics = true
		emitter.AllStatistics = false
	})
	assert.Equal(t, "AB\nC\n             D\n", output)
	assert.Equal(t, 3, emitter.TagEvents("wrn"), "count calls, independent of line count")
}

// TestPartialCallMetadataAndDeltaRecordsCurrentOrigin checks that only the
// first accepted call of a line supplies its metadata and advances the delta.
// The rejected B and accepted same-line C timestamps do not become delta bases.
func TestPartialCallMetadataAndDeltaRecordsCurrentOrigin(t *testing.T) {
	configureTranslatorLoopTest(t)
	emitter.Pick = []string{"wrn"}
	emitter.HostStamp = "zero"
	emitter.Prefix = "<"
	emitter.Suffix = ">"
	id.LIFnJSON = "on"
	decoder.ShowID = "ID:%d "
	decoder.TargetStamp16 = "TS:%d "
	decoder.TargetStamp16Delta = "DT:%d "
	decoder.ShowTargetStamp16Passed = true
	decoder.ShowTargetStamp16DeltaPassed = true
	li := id.TriceIDLookUpLI{
		1: {File: "/tmp/a.c", Line: 11},
		2: {File: "/tmp/b.c", Line: 22},
		3: {File: "/tmp/c.c", Line: 33},
		4: {File: "/tmp/d.c", Line: 44},
	}
	dec := &scriptedDecoder{steps: []scriptedDecoderStep{
		{data: "wrn:A", lastTriceID: 1, targetTimestamp: 10, targetStampSize: 2},
		{data: "dbg:B\n", lastTriceID: 2, targetTimestamp: 20, targetStampSize: 2},
		{data: "wrn:C\n", lastTriceID: 3, targetTimestamp: 30, targetStampSize: 2},
		{data: "wrn:D\n", lastTriceID: 4, targetTimestamp: 40, targetStampSize: 2},
	}}
	var out bytes.Buffer
	sw := emitter.New(&out)
	require.ErrorIs(t, decodeAndComposeLoop(&out, sw, dec, nil, li, nil), io.EOF)
	assert.Equal(t, "2006-01-02_1504-05 <a.c:11 TS:10  DT:-  ID:1  AC>\n"+
		"2006-01-02_1504-05 <d.c:44 TS:40  DT:30  ID:4  D>\n", out.String())
}

// TestLevelSelectionKeepsMetadataAndVisibleDelta verifies that low-weight
// payloads and their columns disappear together while surviving lines retain
// every configured column, even when its presentation text looks low priority.
func TestLevelSelectionKeepsMetadataAndVisibleDelta(t *testing.T) {
	configureTranslatorLoopTest(t)
	emitter.LogLevel = "wrn"
	emitter.HostStamp = "zero"
	emitter.Prefix = "DBG:<"
	emitter.Suffix = ">"
	id.LIFnJSON = "on"
	decoder.ShowID = "ID:%d "
	decoder.TargetStamp16 = "TS:%d "
	decoder.TargetStamp16Delta = "DT:%d "
	decoder.ShowTargetStamp16Passed = true
	decoder.ShowTargetStamp16DeltaPassed = true
	li := id.TriceIDLookUpLI{
		1: {File: "/tmp/low.c", Line: 10},
		2: {File: "/tmp/first.c", Line: 20},
		3: {File: "/tmp/low-again.c", Line: 30},
		4: {File: "/tmp/second.c", Line: 40},
	}
	dec := &classifiedScriptedDecoder{
		scriptedDecoder: scriptedDecoder{steps: []scriptedDecoderStep{
			{data: "dbg:hidden\n", lastTriceID: 1, targetTimestamp: 10, targetStampSize: 2},
			{data: "wrn:shown\n", lastTriceID: 2, targetTimestamp: 20, targetStampSize: 2},
			{data: "dbg:hidden-again\n", lastTriceID: 3, targetTimestamp: 30, targetStampSize: 2},
			{data: "wrn:next\n", lastTriceID: 4, targetTimestamp: 40, targetStampSize: 2},
		}},
		tags: []string{"dbg", "wrn", "dbg", "wrn"},
	}
	var out bytes.Buffer
	sw := emitter.New(&out)
	require.ErrorIs(t, decodeAndComposeLoop(&out, sw, dec, nil, li, nil), io.EOF)
	assert.Equal(t, "2006-01-02_1504-05 DBG:<first.c:20 TS:20  DT:-  ID:2  shown>\n"+
		"2006-01-02_1504-05 DBG:<second.c:40 TS:40  DT:20  ID:4  next>\n", out.String())
}

// TestLevelSelectionWithDocumentedColumns exercises the manual's combined
// timestamp, location, ID, prefix, and suffix configuration. A fixed host
// stamp replaces UTCmicro so each accepted line has a stable expected value.
func TestLevelSelectionWithDocumentedColumns(t *testing.T) {
	configureTranslatorLoopTest(t)
	savedTargetStamp32 := decoder.TargetStamp32
	savedTargetStamp32Passed := decoder.ShowTargetStamp32Passed
	t.Cleanup(func() {
		decoder.TargetStamp32 = savedTargetStamp32
		decoder.ShowTargetStamp32Passed = savedTargetStamp32Passed
	})
	emitter.LogLevel = "wrn"
	emitter.HostStamp = "zero"
	emitter.Prefix = "["
	emitter.Suffix = "]"
	id.LIFnJSON = "on"
	decoder.LocationInformationFormatString = "[%20s:%3d] "
	decoder.ShowID = "[id=%d] "
	decoder.TargetStamp0 = "[no target stamp] "
	decoder.TargetStamp16 = "[t16 = %9d] "
	decoder.TargetStamp32 = "[t32 = %9d] "
	decoder.ShowTargetStamp0Passed = true
	decoder.ShowTargetStamp16Passed = true
	decoder.ShowTargetStamp32Passed = true
	li := id.TriceIDLookUpLI{
		1: {File: "/tmp/no-stamp.c", Line: 10},
		2: {File: "/tmp/hidden.c", Line: 20},
		3: {File: "/tmp/short.c", Line: 30},
		4: {File: "/tmp/long.c", Line: 40},
	}
	dec := &classifiedScriptedDecoder{
		scriptedDecoder: scriptedDecoder{steps: []scriptedDecoderStep{
			{data: "wrn:no stamp\n", lastTriceID: 1, targetStampSize: 0},
			{data: "dbg:hidden\n", lastTriceID: 2, targetTimestamp: 15, targetStampSize: 2},
			{data: "wrn:short stamp\n", lastTriceID: 3, targetTimestamp: 25, targetStampSize: 2},
			{data: "wrn:long stamp\n", lastTriceID: 4, targetTimestamp: 35, targetStampSize: 4},
		}},
		tags: []string{"wrn", "dbg", "wrn", "wrn"},
	}
	var out bytes.Buffer
	sw := emitter.New(&out)
	require.ErrorIs(t, decodeAndComposeLoop(&out, sw, dec, nil, li, nil), io.EOF)
	lines := strings.Split(strings.TrimSuffix(out.String(), "\n"), "\n")
	require.Len(t, lines, 3, "only the three warning events should emit lines")
	for i, want := range []struct {
		location, stamp, event string
		id                     int
	}{
		{location: "no-stamp.c: 10", stamp: "[no target stamp]", event: "no stamp", id: 1},
		{location: "short.c: 30", stamp: "[t16 =        25]", event: "short stamp", id: 3},
		{location: "long.c: 40", stamp: "[t32 =        35]", event: "long stamp", id: 4},
	} {
		assert.Contains(t, lines[i], "2006-01-02_1504-05 [", "host stamp and prefix")
		assert.Contains(t, lines[i], want.location, "source location")
		assert.Contains(t, lines[i], want.stamp, "target stamp")
		assert.Contains(t, lines[i], fmt.Sprintf("[id=%d]", want.id), "first visible event ID")
		assert.True(t, strings.HasSuffix(lines[i], want.event+"]"), "payload and suffix: %q", lines[i])
	}
	assert.NotContains(t, out.String(), "hidden")
	assert.NotContains(t, out.String(), "hidden.c")
	assert.NotContains(t, out.String(), "[id=2]")
}

// TestPartialCallMetadataStartsAtFirstAcceptedCall makes a rejected initial
// fragment visible through its absence: the first emitted line uses B's ID,
// location and timestamp, and the next delta starts from B rather than A.
func TestPartialCallMetadataStartsAtFirstAcceptedCall(t *testing.T) {
	configureTranslatorLoopTest(t)
	emitter.Pick = []string{"wrn"}
	id.LIFnJSON = "on"
	decoder.ShowID = "ID:%d "
	decoder.TargetStamp16 = "TS:%d "
	decoder.TargetStamp16Delta = "DT:%d "
	decoder.ShowTargetStamp16Passed = true
	decoder.ShowTargetStamp16DeltaPassed = true
	li := id.TriceIDLookUpLI{
		1: {File: "/tmp/a.c", Line: 11},
		2: {File: "/tmp/b.c", Line: 22},
		3: {File: "/tmp/c.c", Line: 33},
	}
	dec := &scriptedDecoder{steps: []scriptedDecoderStep{
		{data: "dbg:A", lastTriceID: 1, targetTimestamp: 10, targetStampSize: 2},
		{data: "wrn:B\n", lastTriceID: 2, targetTimestamp: 20, targetStampSize: 2},
		{data: "wrn:C\n", lastTriceID: 3, targetTimestamp: 30, targetStampSize: 2},
	}}
	var out bytes.Buffer
	sw := emitter.New(&out)
	require.ErrorIs(t, decodeAndComposeLoop(&out, sw, dec, nil, li, nil), io.EOF)
	assert.Equal(t, "b.c:22 TS:20  DT:-  ID:2  B\n"+
		"c.c:33 TS:30  DT:10  ID:3  C\n", out.String())
}

// TestByteOrientedDecodersDoNotCreateUntaggedEvents verifies that arbitrary
// CHAR and DUMP read chunks remain byte-oriented rather than becoming events.
func TestByteOrientedDecodersDoNotCreateUntaggedEvents(t *testing.T) {
	tests := []struct {
		encoding string
		input    []byte
	}{
		{encoding: "CHAR", input: []byte("plain:stream\n")},
		{encoding: "DUMP", input: []byte{0x01, 0x02}},
	}
	for _, tt := range tests {
		t.Run(tt.encoding, func(t *testing.T) {
			configureTranslatorLoopTest(t)
			oldDumpLineByteCount := decoder.DumpLineByteCount
			t.Cleanup(func() { decoder.DumpLineByteCount = oldDumpLineByteCount })
			decoder.DumpLineByteCount = 16
			var output bytes.Buffer
			dec, err := decoder.NewForEncoding(tt.encoding, io.Discard, nil, nil, nil, bytes.NewReader(tt.input), decoder.LittleEndian)
			require.NoError(t, err)
			sw := emitter.New(&output)

			err = decodeAndComposeLoop(&output, sw, dec, nil, nil, nil)
			require.ErrorIs(t, err, io.EOF)
			assert.NotContains(t, output.String(), "untagged:")
		})
	}
}

// TestTREXDiagnosticsBypassApplicationFilters verifies that representative
// decoder failures remain local tool output while valid application text from
// the same Read still obeys the configured filters.
func TestTREXDiagnosticsBypassApplicationFilters(t *testing.T) {
	tests := []struct {
		name            string
		framing         string
		input           []byte
		lut             id.TriceIDLookUp
		configureFilter func()
		want            string
		wantNot         string
		wantAppCount    int
	}{
		{
			name:    "unknown ID survives pick",
			framing: "cobs",
			input:   []byte{0x06, 0x01, 0x40, 0xc0, 0x01, 0x2a, 0x00},
			lut:     id.TriceIDLookUp{},
			configureFilter: func() {
				emitter.Pick = []string{"err"}
			},
			want: "unknown ID",
		},
		{
			name:    "damaged frame survives ban all",
			framing: "cobs",
			input:   []byte{0x03, 0x11, 0x00},
			lut:     id.TriceIDLookUp{},
			configureFilter: func() {
				emitter.Ban = []string{"all"}
			},
			want:    "inconsistent COBS buffer",
			wantNot: "unknown ID",
		},
		{
			name:    "short packet survives pick",
			framing: "cobs",
			input:   []byte{0x02, 0xaa, 0x00},
			lut:     id.TriceIDLookUp{},
			configureFilter: func() {
				emitter.Pick = []string{"err"}
			},
			want: "unsupported short packet size 1",
		},
		{
			name:    "cycle diagnostic survives level off",
			framing: "none",
			input:   []byte{0x01, 0x40, 0x10, 0x01, 0x2a},
			lut: id.TriceIDLookUp{
				1: {Type: "TRICE8_1", Strg: "msg:v=%d\\n"},
			},
			configureFilter: func() {
				emitter.LogLevel = "off"
			},
			want:         "CYCLE_ERROR",
			wantNot:      "v=42",
			wantAppCount: 1,
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			configureTranslatorLoopTest(t)
			savedTags := emitter.Tags
			savedUserLabel := emitter.UserLabel
			savedTagStatistics := emitter.TagStatistics
			savedAllStatistics := emitter.AllStatistics
			savedTriceStatistics := decoder.TriceStatistics
			savedIDStat := decoder.IDStat
			t.Cleanup(func() {
				emitter.Tags = savedTags
				emitter.UserLabel = savedUserLabel
				emitter.TagStatistics = savedTagStatistics
				emitter.AllStatistics = savedAllStatistics
				decoder.TriceStatistics = savedTriceStatistics
				decoder.IDStat = savedIDStat
			})
			emitter.UserLabel = nil
			require.NoError(t, emitter.AddUserLabels())
			emitter.TagStatistics = true
			emitter.AllStatistics = false
			decoder.TriceStatistics = true
			decoder.IDStat = make(map[id.TriceID]int)
			oldFraming := decoder.PackageFraming
			oldInitialCycle := decoder.InitialCycle
			oldDisableCycleErrors := trexDecoder.DisableCycleErrors
			t.Cleanup(func() {
				decoder.PackageFraming = oldFraming
				decoder.InitialCycle = oldInitialCycle
				trexDecoder.DisableCycleErrors = oldDisableCycleErrors
			})
			decoder.PackageFraming = tt.framing
			decoder.InitialCycle = false
			trexDecoder.DisableCycleErrors = false
			tt.configureFilter()

			var output bytes.Buffer
			dec := trexDecoder.New(
				&output,
				tt.lut,
				new(sync.RWMutex),
				nil,
				bytes.NewReader(tt.input),
				decoder.LittleEndian,
			)
			sw := emitter.New(&output)

			err := decodeAndComposeLoop(&output, sw, dec, tt.lut, nil, nil)
			require.ErrorIs(t, err, io.EOF)
			assert.Contains(t, output.String(), tt.want)
			assert.NotContains(t, output.String(), "untagged:")
			if tt.wantNot != "" {
				assert.NotContains(t, output.String(), tt.wantNot)
			}
			assert.Len(t, decoder.IDStat, tt.wantAppCount, "only successfully decoded IDs are counted")
			assert.Equal(t, tt.wantAppCount, emitter.TagEvents("msg"), "valid application output counts before filtering")
			assert.Zero(t, emitter.TagEvents("CYCLE_ERROR"), "tool diagnostics are not application events")
		})
	}
}

// TestTREXDiagnosticsUseSeparateWriter verifies that a machine-oriented
// application composer can remain free of textual decoder diagnostics.
func TestTREXDiagnosticsUseSeparateWriter(t *testing.T) {
	configureTranslatorLoopTest(t)
	oldFraming := decoder.PackageFraming
	oldInitialCycle := decoder.InitialCycle
	oldDisableCycleErrors := trexDecoder.DisableCycleErrors
	t.Cleanup(func() {
		decoder.PackageFraming = oldFraming
		decoder.InitialCycle = oldInitialCycle
		trexDecoder.DisableCycleErrors = oldDisableCycleErrors
	})
	decoder.PackageFraming = "none"
	decoder.InitialCycle = false
	trexDecoder.DisableCycleErrors = false

	lut := id.TriceIDLookUp{
		1: {Type: "TRICE8_1", Strg: "msg:v=%d\\n"},
	}
	input := []byte{0x01, 0x40, 0x10, 0x01, 0x2a}
	var diagnostics bytes.Buffer
	var application bytes.Buffer
	dec := trexDecoder.New(
		&diagnostics,
		lut,
		new(sync.RWMutex),
		nil,
		bytes.NewReader(input),
		decoder.LittleEndian,
	)
	sw := emitter.New(&application)

	err := decodeAndComposeLoop(&diagnostics, sw, dec, lut, nil, nil)
	require.ErrorIs(t, err, io.EOF)
	assert.Contains(t, diagnostics.String(), "CYCLE_ERROR")
	assert.NotContains(t, diagnostics.String(), "v=42")
	assert.Equal(t, "v=42\n", application.String())
	assert.NotContains(t, application.String(), "CYCLE_ERROR")
}

// runTranslatorVisCase executes one formatted/typed record through filter, router, and line composer.
func runTranslatorVisCase(t *testing.T, tag, option, filter string, levels ...string) (normalOutput string, visOutput string) {
	t.Helper()
	configureTranslatorLoopTest(t)
	if len(levels) != 0 {
		emitter.LogLevel = levels[0]
	}
	switch filter {
	case "ban":
		emitter.Ban = []string{tag}
	case "exclude-by-pick":
		emitter.Pick = []string{"other"}
	}

	format := tag + ":value=%d\n"
	lut := id.TriceIDLookUp{
		7: {Type: "TRICE32_1", Strg: format},
	}
	fileSystem := &afero.Afero{Fs: afero.NewMemMapFs()}
	var output bytes.Buffer
	router, err := vis.NewRouter(
		&output,
		fileSystem,
		lut,
		new(sync.RWMutex),
		[]string{fmt.Sprintf(`%s:printf("%%d\n",v0)@vis.txt%s`, tag, option)},
		false,
	)
	require.NoError(t, err)

	record := decoder.VisRecord{
		ID:         7,
		Type:       "TRICE32_1",
		Format:     format,
		ValueCount: 1,
		SingleLine: true,
	}
	record.Values[0] = decoder.VisValue{
		Kind:   decoder.VisValueSigned,
		Bits:   32,
		Signed: 7,
	}
	dec := &visScriptedDecoder{
		scriptedDecoder: scriptedDecoder{
			steps: []scriptedDecoderStep{{data: tag + ":value=7\n", lastTriceID: 7}},
		},
		records: []decoder.VisRecord{record},
	}
	dec.SetVisRecordEnabled(true)
	sw := emitter.New(&output)

	err = decodeAndComposeLoop(&output, sw, dec, lut, nil, router)
	require.ErrorIs(t, err, io.EOF)
	require.NoError(t, router.Close())
	visualized, err := fileSystem.ReadFile("vis.txt")
	require.NoError(t, err)
	return output.String(), string(visualized)
}

// TestDecodeAndComposeLoopIntegratesVisAfterFiltering verifies ordering, drop, and unchanged tag handling.
func TestDecodeAndComposeLoopIntegratesVisAfterFiltering(t *testing.T) {
	t.Run("successful drop suppresses normal output", func(t *testing.T) {
		normal, visualized := runTranslatorVisCase(t, "msg", ";log=drop", "")
		assert.Empty(t, normal)
		assert.Equal(t, "7\n", visualized)
	})

	t.Run("ban makes record invisible to vis", func(t *testing.T) {
		normal, visualized := runTranslatorVisCase(t, "msg", ";log=drop", "ban")
		assert.Empty(t, normal)
		assert.Empty(t, visualized)
	})

	t.Run("pick exclusion makes record invisible to vis", func(t *testing.T) {
		normal, visualized := runTranslatorVisCase(t, "msg", ";log=drop", "exclude-by-pick")
		assert.Empty(t, normal)
		assert.Empty(t, visualized)
	})

	t.Run("vis does not register unknown normal tag", func(t *testing.T) {
		normal, visualized := runTranslatorVisCase(t, "imu_custom", "", "")
		assert.Equal(t, "imu_custom:value=7\n", normal)
		assert.Equal(t, "7\n", visualized)
	})

	t.Run("level rejects both normal output and visualization", func(t *testing.T) {
		normal, visualized := runTranslatorVisCase(t, "msg", "", "", "notice")
		assert.Empty(t, normal)
		assert.Empty(t, visualized)
	})

	t.Run("level boundary admits visualization with drop", func(t *testing.T) {
		normal, visualized := runTranslatorVisCase(t, "msg", ";log=drop", "", "info")
		assert.Empty(t, normal)
		assert.Equal(t, "7\n", visualized)
	})
}

// TestTREXVisIntegrationRoutesUnstampedAndBothStampWidths exercises real encoded decoder input end to end.
func TestTREXVisIntegrationRoutesUnstampedAndBothStampWidths(t *testing.T) {
	configureTranslatorLoopTest(t)
	savedTags := emitter.Tags
	savedUserLabel := emitter.UserLabel
	savedTagStatistics := emitter.TagStatistics
	savedAllStatistics := emitter.AllStatistics
	savedTriceStatistics := decoder.TriceStatistics
	savedIDStat := decoder.IDStat
	t.Cleanup(func() {
		emitter.Tags = savedTags
		emitter.UserLabel = savedUserLabel
		emitter.TagStatistics = savedTagStatistics
		emitter.AllStatistics = savedAllStatistics
		decoder.TriceStatistics = savedTriceStatistics
		decoder.IDStat = savedIDStat
	})
	emitter.UserLabel = []string{"sample"}
	require.NoError(t, emitter.AddUserLabels())
	emitter.TagStatistics = true
	emitter.AllStatistics = false
	decoder.TriceStatistics = true
	decoder.IDStat = make(map[id.TriceID]int)
	oldFraming := decoder.PackageFraming
	oldInitialCycle := decoder.InitialCycle
	oldDoubledID := trexDecoder.Doubled16BitID
	oldAddNewline := trexDecoder.AddNewlineToEachTriceMessage
	oldDisableCycleErrors := trexDecoder.DisableCycleErrors
	t.Cleanup(func() {
		decoder.PackageFraming = oldFraming
		decoder.InitialCycle = oldInitialCycle
		trexDecoder.Doubled16BitID = oldDoubledID
		trexDecoder.AddNewlineToEachTriceMessage = oldAddNewline
		trexDecoder.DisableCycleErrors = oldDisableCycleErrors
	})
	decoder.PackageFraming = "none"
	decoder.InitialCycle = true
	trexDecoder.Doubled16BitID = false
	trexDecoder.AddNewlineToEachTriceMessage = false
	trexDecoder.DisableCycleErrors = false

	lut := id.TriceIDLookUp{
		1: {Type: "TRICE32_1", Strg: `sample:unstamped=%d\n`},
		2: {Type: "TRICE32_1", Strg: `sample:stamp16=%d\n`},
		3: {Type: "TRICE32_1", Strg: `sample:stamp32=%d\n`},
	}
	// Three unframed little-endian records carry no stamp, a 16-bit stamp,
	// and a 32-bit stamp respectively. Their cycle counters are c0, c1, c2.
	input := []byte{
		0x01, 0x40, 0xc0, 0x04, 10, 0, 0, 0,
		0x02, 0x80, 20, 0, 0xc1, 0x04, 20, 0, 0, 0,
		0x03, 0xc0, 30, 0, 0, 0, 0xc2, 0x04, 30, 0, 0, 0,
	}
	lutMutex := new(sync.RWMutex)
	dec := trexDecoder.New(
		io.Discard,
		lut,
		lutMutex,
		id.TriceIDLookUpLI{},
		bytes.NewReader(input),
		decoder.LittleEndian,
	)
	controller, ok := dec.(decoder.VisRecordController)
	require.True(t, ok)
	controller.SetVisRecordEnabled(true)

	fileSystem := &afero.Afero{Fs: afero.NewMemMapFs()}
	var normal bytes.Buffer
	router, err := vis.NewRouter(
		&normal,
		fileSystem,
		lut,
		lutMutex,
		[]string{
			`sample:printf("%d\n",v0)@all.csv;log=drop`,
			`sample:printf("%d,%d\n",ts16,v0)@stamp16.csv`,
			`sample:printf("%d,%d\n",ts32,v0)@stamp32.csv`,
		},
		false,
	)
	require.NoError(t, err)
	sw := emitter.New(&normal)

	err = decodeAndComposeLoop(&normal, sw, dec, lut, nil, router)
	require.ErrorIs(t, err, io.EOF)
	require.NoError(t, router.Close())
	assert.Empty(t, normal.String())
	assert.Equal(t, 3, emitter.TagEvents("sample"), "visualization drops text, not decoded event totals")
	assert.Equal(t, map[id.TriceID]int{1: 1, 2: 1, 3: 1}, decoder.IDStat)

	all, err := fileSystem.ReadFile("all.csv")
	require.NoError(t, err)
	assert.Equal(t, "10\n20\n30\n", string(all))
	stamp16, err := fileSystem.ReadFile("stamp16.csv")
	require.NoError(t, err)
	assert.Equal(t, "20,20\n", string(stamp16))
	stamp32, err := fileSystem.ReadFile("stamp32.csv")
	require.NoError(t, err)
	assert.Equal(t, "30,30\n", string(stamp32))
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
		Translate(io.Discard, nil, nil, nil, nil, nil, nil)
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
		handleSIGTERM(io.Discard, &closeRecorder{}, nil)
		return
	}

	cmd := exec.Command(os.Args[0], "-test.run=TestHandleSIGTERMExitsAndCloses")
	cmd.Env = append(os.Environ(), "TRICE_HANDLE_SIGTERM=1")
	require.NoError(t, cmd.Start())
	time.Sleep(150 * time.Millisecond)
	require.NoError(t, cmd.Process.Signal(syscall.SIGTERM))
	require.NoError(t, cmd.Wait())
}
