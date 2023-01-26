package cgo_test

import (
	"bytes"
	"fmt"
	"io"
	"strings"
	"testing"

	"github.com/rokath/trice/internal/args"
	"github.com/spf13/afero"
	"github.com/tj/assert"
)

func TestTriceCheck(t *testing.T) {

	// prepare
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	CopyFileIntoFSys(t, fSys, "triceCheck.c")

	out := make([]byte, 32768)
	SetTriceBuffer(out)

	f, e := fSys.Open("triceCheck.c")
	assert.Nil(t, e)
	lines := LinesInFile(f)

	subStr := "//exp: "
	var expVector []string

	for _, line := range lines {
		fmt.Println("line:" + line)
		index := strings.LastIndex(line, subStr)
		if index > 0 {
			expOut := line[index+len(subStr)+1 : len(line)-1]
			expVector = append(expVector, expOut)
			fmt.Println("rest:" + expOut)
		}
	}

	fh, e := fSys.Create("til.json")
	assert.Nil(t, e)
	assert.Nil(t, fh.Close())

	// trice update
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "u", "-src", "."}))

	// show generated til.json for debugging
	tBytes, e := fSys.ReadFile("til.json")
	assert.Nil(t, e)
	fmt.Println(string(tBytes))

	// show modified "triceCheck.c" for debugging
	cBytes, e := fSys.ReadFile("triceCheck.c")
	assert.Nil(t, e)
	fmt.Println(string(cBytes))

	for i, exp := range expVector {

		fmt.Println(i, "exp:"+exp)

		// target activity
		TriceCheck(i)
		length := TriceOutDepth()
		bin := out[:length] // bin contains the binary trice data of trice message i

		fmt.Println(i, bin)

		assert.Nil(t, fSys.WriteFile("fileBuffer.bin", bin, 0777))

		// trice log
		var o bytes.Buffer
		assert.Nil(t, args.Handler(io.Writer(&o), fSys, []string{"trice", "log", "-p", "FILEBUFFER", "-args", "fileBuffer.bin", "-packageFraming", "COBS", "-ts", "off", "-prefix", "off", "-tsf", "", "-li", "off", "-color", "off"}))

		act := o.String()
		assert.Equal(t, exp, strings.TrimSuffix(act, "\n"))
	}
}
