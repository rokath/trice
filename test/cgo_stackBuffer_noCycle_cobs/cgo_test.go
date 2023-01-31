package cgo_test

import (
	"bytes"
	"fmt"
	"io"
	"strings"
	"testing"

	"github.com/rokath/trice/internal/args"
	cgo "github.com/rokath/trice/test/cgo_stackBuffer_noCycle_cobs"
	"github.com/spf13/afero"
	"github.com/tj/assert"
)

func TestTriceCheck(t *testing.T) {

	osFSys := &afero.Afero{Fs: afero.NewOsFs()}
	mmFSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	wd := "d:/repos/trice/test/"

	// trice update testdata
	//var b bytes.Buffer
	//assert.Nil(t, args.Handler(io.Writer(&b), osFSys, []string{"trice", "u", "-i", wd + "testdata/generated_til.json", "-src", wd + "cgo_stackBuffer_noCycle_cobs"}))

	// prepare

	cgo.CopyFileIntoFSys(t, mmFSys, "triceCheck.c", osFSys, wd+"testdata/generated_triceCheck.c") // needed for the expected results
	cgo.CopyFileIntoFSys(t, mmFSys, "til.json", osFSys, wd+"testdata/generated_til.json")         // needed for the trice log

	out := make([]byte, 32768)
	cgo.SetTriceBuffer(out)

	f, e := mmFSys.Open("triceCheck.c")
	assert.Nil(t, e)
	lines := cgo.LinesInFile(f)

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

	// show generated til.json for debugging
	tBytes, e := mmFSys.ReadFile("til.json")
	assert.Nil(t, e)
	fmt.Println(string(tBytes))

	// show modified "triceCheck.c" for debugging
	cBytes, e := mmFSys.ReadFile("triceCheck.c")
	assert.Nil(t, e)
	fmt.Println(string(cBytes))

	for i, exp := range expVector {

		fmt.Println(i, "exp:"+exp)

		// target activity
		cgo.TriceCheck(i)
		length := cgo.TriceOutDepth()
		bin := out[:length] // bin contains the binary trice data of trice message i

		fmt.Println(i, bin) // // show data for debugging
		assert.Nil(t, mmFSys.WriteFile("fileBuffer.bin", bin, 0777))

		// trice log
		var o bytes.Buffer
		assert.Nil(t, args.Handler(io.Writer(&o), mmFSys, []string{"trice", "log", "-i", "til.json", "-p", "FILEBUFFER", "-args", "fileBuffer.bin", "-packageFraming", "COBS", "-ts", "off", "-prefix", "off", "-tsf", "", "-li", "off", "-color", "off"}))

		act := o.String()
		assert.Equal(t, exp, strings.TrimSuffix(act, "\n"))

		assert.Nil(t, mmFSys.Remove("fileBuffer.bin"))
	}
}
