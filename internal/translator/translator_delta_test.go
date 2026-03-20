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
	defer func() {
		decoder.TargetStamp16 = saved16
		decoder.TargetStamp16Delta = saved16Delta
	}()

	decoder.TargetStamp16 = "time:%04x "
	decoder.TargetStamp16Delta = "dt:%4d"

	state := targetStampState{}

	gotFirst := renderTargetStampColumns(2, 65530, &state)
	wantFirst := "time:fffa dt:   -"
	if gotFirst != wantFirst {
		t.Fatalf("first delta mismatch:\nwant %q\ngot  %q", wantFirst, gotFirst)
	}

	gotSecond := renderTargetStampColumns(2, 4, &state)
	wantSecond := "time:0004 dt:  10"
	if gotSecond != wantSecond {
		t.Fatalf("wrapped delta mismatch:\nwant %q\ngot  %q", wantSecond, gotSecond)
	}
}

func TestRenderTargetStampColumns32EpochAndDelta(t *testing.T) {
	saved32 := decoder.TargetStamp32
	saved32Delta := decoder.TargetStamp32Delta
	defer func() {
		decoder.TargetStamp32 = saved32
		decoder.TargetStamp32Delta = saved32Delta
	}()

	decoder.TargetStamp32 = "epoch2006-01-02 15:04:05 UTC "
	decoder.TargetStamp32Delta = "dt:%8d"

	state := targetStampState{}
	ts0 := uint64(1700000000)
	gotFirst := renderTargetStampColumns(4, ts0, &state)
	wantFirst := time.Unix(int64(ts0), 0).UTC().Format("2006-01-02 15:04:05 UTC ") + "dt:       -"
	if gotFirst != wantFirst {
		t.Fatalf("first epoch delta mismatch:\nwant %q\ngot  %q", wantFirst, gotFirst)
	}

	gotSecond := renderTargetStampColumns(4, ts0+15, &state)
	wantSecond := time.Unix(int64(ts0+15), 0).UTC().Format("2006-01-02 15:04:05 UTC ") + "dt:      15"
	if gotSecond != wantSecond {
		t.Fatalf("second epoch delta mismatch:\nwant %q\ngot  %q", wantSecond, gotSecond)
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

	if decoder.TargetStamp0Delta != strings.Repeat(" ", len("dt:   0")) {
		t.Fatalf("unexpected auto ts0delta width: %q", decoder.TargetStamp0Delta)
	}

	decoder.TargetStamp0Delta = ""
	decoder.TargetStamp16Delta = "dt:%4d"
	decoder.TargetStamp32Delta = "dt:%5d"
	autoTargetStamp0Delta()

	if decoder.TargetStamp0Delta != "" {
		t.Fatalf("ts0delta should stay empty for mismatched widths, got %q", decoder.TargetStamp0Delta)
	}
}
