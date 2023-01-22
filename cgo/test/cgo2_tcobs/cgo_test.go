package cgo2

import (
	"bytes"
	"fmt"
	"io"
	"testing"

	"github.com/rokath/trice/internal/args"
	"github.com/spf13/afero"
	"github.com/tj/assert"
)

func TestTriceSequences(t *testing.T) {
	out := make([]byte, 1024)
	setTriceBuffer(out)

	for i, exp := range triceBytes {
		triceCheck(i)
		len := triceOutDepth()
		act := out[:len]
		assert.Equal(t, exp, act)
	}
}

// triceBytes contains the expected trice byte streams when C.TriceCode is executed.
// This is TCOBSv1 encoded.
var triceBytes = [][]byte{
	//{0x09, 0xdb, 0x55, 0xc0, 0x4, 0xff, 0xff, 0xff, 0xff, 0x00},
	{0x09, 0x02, 0x40, 0xc0, 0x4, 0xff, 0xff, 0xff, 0xff, 0x00},
}

func TestTrice88(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}

	// create til.json
	jFn := "til.json"
	JSONFile := `{
	"2": {
		"Type": "TRICE8_1",
		"Strg": "msg:value=%d\\n"
	},
}`
	assert.Nil(t, fSys.WriteFile(jFn, []byte(JSONFile), 0777))

	s := fmt.Sprintln(triceBytes[0])
	fmt.Print(s)
	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "log", "-p", "BUFFER", "-args", s}))

	fmt.Print(b.String())
	t.Fail()
}
