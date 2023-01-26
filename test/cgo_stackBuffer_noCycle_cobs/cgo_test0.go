package cgo_test

import (
	"bytes"
	"io"
	"testing"

	"github.com/rokath/trice/internal/args"
	"github.com/spf13/afero"
	"github.com/tj/assert"
)

func _TestTriceSequences(t *testing.T) {
	out := make([]byte, 1024)
	SetTriceBuffer(out)

	for i, exp := range triceBytes {
		TriceCheck(i)
		len := TriceOutDepth()
		act := out[:len]
		assert.Equal(t, exp, act)
	}
}

// triceBytes contains the expected trice byte streams when C.TriceCode is executed.
// This is COBS encoded.
var triceBytes = [][]byte{
	//cnt, loID, hiID, cycl, len,                 32-bit, delim
	{0x09, 0x02, 0x40, 0xc0, 0x4, 0xff, 0xff, 0xff, 0xff, 0x00},
}

func TestTriceBytesCOBS(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}

	// create til.json
	jFn := "til.json"
	JSONFile := `{
	"2": {
		"Type": "TRICE8_1",
		"Strg": "value=%d\\n"
	}
}`
	assert.Nil(t, fSys.WriteFile(jFn, []byte(JSONFile), 0777))
	assert.Nil(t, fSys.WriteFile("fileBuffer.bin", triceBytes[0], 0777))

	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "log", "-p", "FILEBUFFER", "-args", "fileBuffer.bin", "-packageFraming", "COBS", "-ts", "off", "-prefix", "off", "-tsf", ""}))

	exp := "value=-1\n"
	act := b.String()
	assert.Equal(t, exp, act)
}
