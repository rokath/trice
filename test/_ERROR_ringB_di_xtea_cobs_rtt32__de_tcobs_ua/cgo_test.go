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

func TestLogs(t *testing.T) {

	// triceLog is the log function for executing the trice logging on binary log data in buffer as space separated numbers.
	// It uses the inside fSys specified til.json and returns the log output.
	triceLog0 := func(t *testing.T, fSys *afero.Afero, buffer string) string {
		var o bytes.Buffer
		//                                                        trice  -p jlink -args "-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0"  -showID "deb:%04x"   -hs off                                            -pw MySecret    -pf COBS    -d16=true
		assert.Nil(t, args.Handler(io.Writer(&o), fSys, []string{"trice", "log", "-i", path.Join(triceDir, "/test/testdata/til.json"), "-p=BUFFER", "-args", buffer, "-hs=off", "-prefix=off", "-li=off", "-color=off", "-pw=MySecret", "-pf=COBS", "-d16=true"}))
		return o.String()
	}

	// triceLog is the log function for executing the trice logging on binary log data in buffer as space separated numbers.
	// It uses the inside fSys specified til.json and returns the log output.
	triceLog1 := func(t *testing.T, fSys *afero.Afero, buffer string) string {
		var o bytes.Buffer
		//                                                        trice    l                                                            -p com4                                                                                       -d16=false
		assert.Nil(t, args.Handler(io.Writer(&o), fSys, []string{"trice", "log", "-i", path.Join(triceDir, "/test/testdata/til.json"), "-p=BUFFER", "-args", buffer, "-hs=off", "-prefix=off", "-li=off", "-color=off", "-pf=TCOBS", "-d16=false"}))
		return o.String()
	}

	triceLogTest2(t, triceLog0, triceLog1, testLines)
}
