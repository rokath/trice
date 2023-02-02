package cgot

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

	osFSys := &afero.Afero{Fs: afero.NewOsFs()}
	mmFSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	wd := "d:/repos/trice/test/"

	// trice update testdata
	//var b bytes.Buffer
	//assert.Nil(t, args.Handler(io.Writer(&b), osFSys, []string{"trice", "u", "-i", wd + "testdata/generated_til.json", "-src", wd + "cgo_stackBuffer_noCycle_cobs"}))

	// prepare

	//cgot.CopyFileIntoFSys(t, mmFSys, "triceCheck.c", osFSys, wd+"testdata/generated_triceCheck.c") // needed for the expected results
	CopyFileIntoFSys(t, mmFSys, "til.json", osFSys, wd+"testdata/til.json") // needed for the trice log

	out := make([]byte, 32768)
	SetTriceBuffer(out)

	result := GetExpectedResults(osFSys, wd+"testdata/generated_triceCheck.c")

	// show generated til.json for debugging
	//tBytes, e := osFSys.ReadFile(wd + "testdata/til.json")
	//assert.Nil(t, e)
	//fmt.Println(string(tBytes))

	// show modified "triceCheck.c" for debugging
	//cBytes, e := osFSys.ReadFile(wd + "testdata/generated_triceCheck.c")
	//assert.Nil(t, e)
	//fmt.Println(string(cBytes))

	for i, r := range result {

		fmt.Println(i, r)

		// target activity
		TriceCheck(r.Line)
		length := TriceOutDepth()
		bin := out[:length] // bin contains the binary trice data of trice message i

		fmt.Println(i, bin) // // show data for debugging
		assert.Nil(t, mmFSys.WriteFile("fileBuffer.bin", bin, 0777))

		// trice log
		var o bytes.Buffer
		assert.Nil(t, args.Handler(io.Writer(&o), mmFSys, []string{"trice", "log", "-i", "til.json", "-p", "FILEBUFFER", "-args", "fileBuffer.bin", "-packageFraming", "COBS", "-ts", "off", "-prefix", "off", "-tsf", "", "-li", "off", "-color", "off"}))

		act := o.String()
		assert.Equal(t, r.Exp, strings.TrimSuffix(act, "\n"))
	}
}
