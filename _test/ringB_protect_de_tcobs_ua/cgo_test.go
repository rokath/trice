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

func TestLogs(t *testing.T) {
	// triceLog is the log function for executing the trice logging on binary log data in buffer as space separated numbers.
	// It uses the inside fSys specified til.json and returns the log output.
	triceLog := func(t *testing.T, fSys *afero.Afero, buffer string) string {
		var o bytes.Buffer
		assert.Nil(t, args.Handler(io.Writer(&o), fSys, []string{"trice", "log", "-i", path.Join(triceDir, "/demoTIL.json"), "-p=BUFFER", "-args", buffer, "-hs=off", "-prefix=off", "-li=off", "-color=none", "-ts16", "time:    %04x", "-ts32", "time:%08x"}))
		return o.String()
	}
	triceLogSpecialTest(t, triceLog)
}

// triceLogSpecialTest ...
func triceLogSpecialTest(t *testing.T, triceLog logF) {
	osFSys := &afero.Afero{Fs: afero.NewOsFs()}
	out := make([]byte, 32768)
	setTriceBuffer(out)

	exp := targetActivity()

	// For the ring buffer, we need to call triceTransfer() at least for each Trice statement in targetActivity()
	// For the double buffer one call is enough.
	for i := 0; i < 5; i++ {
		triceTransfer()
	}

	length := triceOutDepth()
	bin := out[:length] // bin contains the binary trice data of trice message.

	buf := fmt.Sprint(bin)
	buffer := buf[1 : len(buf)-1]

	act := triceLog(t, osFSys, buffer) // convert binary buffer into result string

	triceClearOutBuffer()

	assert.Equal(t, exp, strings.TrimSuffix(act, "\n"))

}
