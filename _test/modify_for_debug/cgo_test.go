package cgot

import (
	"bytes"
	"fmt"
	"io"
	"path"
	"strings"
	"testing"

	"github.com/rokath/trice/internal/args"
	"github.com/spf13/afero"
	"github.com/tj/assert"
)

func init() {
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
			},
		)
		assert.Nil(t, f)
		return o.String()
	}
	targetMode = "specificTest"
	specificTest = caseSpecificTest
}

// caseSpecificTest executes function triceLog. It creates more logs than fit into buffer and then silently suppresses the not fitting log.
func caseSpecificTest(t *testing.T, triceLog logF) {
	osFSys := &afero.Afero{Fs: afero.NewOsFs()}
	out := make([]byte, 32768)
	setTriceBuffer(out)

	exp := targetActivity()

	// For the ring buffer, we need to call triceTransfer() at least for each Trice statement in targetActivity()
	// For the double buffer one call is enough.
	for range 5 {
		triceTransfer()
	}

	length := triceOutDepth()
	bin := out[:length] // bin contains the binary trice data of trice message.

	buf := fmt.Sprint(bin) // convert into number string enclosed in brackets []
	buffer := buf[1 : len(buf)-1] // remove brackets []

	act := triceLog(t, osFSys, buffer) // convert binary buffer into result string

	triceClearOutBuffer()

	assert.Equal(t, exp, strings.TrimSuffix(act, "\n"))

}
