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
	triceLog = func(t *testing.T, fSys *afero.Afero, buffer string) string {
		//var o bytes.Buffer
		x := make([]byte, 0, 1000000)
		o := bytes.NewBuffer(x)
		f := args.Handler(io.Writer(o), fSys,
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
		//b, _ := io.ReadAll(o)
		//return string(b)
	}
	doTestTriceLogLineByLine = true
	doTestTriceLogBulk = false // true
	doTestTriceLogDirectAndDeferred = false
}
