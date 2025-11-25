package cgot

import (
	"bytes"
	"io"
	"path"
	"testing"

	"github.com/rokath/trice/internal/args"
	"github.com/spf13/afero"
	"github.com/tj/assert"
)

func init() {
	// triceLog is the log function for executing the trice logging on binary log data in buffer as space separated numbers.
	// It uses the inside fSys specified til.json and returns the log output.
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
				"-pf=NONE",
			},
		)
		assert.Nil(t, f)
		return o.String()
	}
	targetMode = "deferredMode"
}
