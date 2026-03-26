package translator

import (
	"strings"
	"testing"
	"time"

	"github.com/rokath/trice/internal/decoder"
)

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

func TestTargetStampDisplayWidthKeepsUppercaseTagPrefix(t *testing.T) {
	if got := targetStampDisplayWidth(4, "Time:%12d"); got != len("Time:")+12 {
		t.Fatalf("unexpected display width for uppercase tag: %d", got)
	}
	if got := targetStampDisplayWidth(4, "timeStamp:%12d"); got != len("timeStamp:")+12 {
		t.Fatalf("unexpected display width for mixed-case tag: %d", got)
	}
}

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
