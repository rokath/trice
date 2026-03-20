package args

import (
	"bytes"
	"strings"
	"testing"

	"github.com/spf13/afero"
)

func TestLogHelpShowsDeltaTimestampFlags(t *testing.T) {
	FlagsInit()

	var out bytes.Buffer
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	err := Handler(&out, fSys, []string{"trice", "help", "-log"})
	if err != nil {
		t.Fatalf("unexpected help error: %v", err)
	}

	got := out.String()
	for _, snippet := range []string{
		"-ts0delta string",
		"-ts16delta string",
		"-ts32delta string",
		`"epoch..." is not supported for deltas`,
	} {
		if !strings.Contains(got, snippet) {
			t.Fatalf("help output misses %q\n%s", snippet, got)
		}
	}
}
