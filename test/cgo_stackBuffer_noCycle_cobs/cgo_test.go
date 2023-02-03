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

// triceLog logs binary trice data inside fileName and return output as string
func triceLog(t *testing.T, fSys *afero.Afero, fileName string) string {
	var o bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&o), fSys, []string{"trice", "log", "-i", "til.json", "-p", "FILEBUFFER", "-args", "fileBuffer.bin", "-packageFraming", "COBS", "-ts", "off", "-prefix", "off", "-tsf", "", "-li", "off", "-color", "off"}))
	return o.String()
}

func TestTriceCheck(t *testing.T) {

	osFSys := &afero.Afero{Fs: afero.NewOsFs()}
	mmFSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	wd := "d:/repos/trice/test/"

	CopyFileIntoFSys(t, mmFSys, "til.json", osFSys, wd+"testdata/til.json") // needed for the trice log
	out := make([]byte, 32768)
	SetTriceBuffer(out)

	result := GetExpectedResults(osFSys, wd+"testdata/generated_triceCheck.c")

	for i, r := range result {

		fmt.Println(i, r)

		// target activity
		TriceCheck(r.Line)
		length := TriceOutDepth()
		bin := out[:length] // bin contains the binary trice data of trice message i

		//fmt.Println(i, bin) // // show data for debugging
		assert.Nil(t, mmFSys.WriteFile("fileBuffer.bin", bin, 0777))

		act := triceLog(t, mmFSys, "fileBuffer.bin")
		assert.Equal(t, r.Exp, strings.TrimSuffix(act, "\n"))
	}
}

func triceLog2(t *testing.T, fSys *afero.Afero, buffer string) string {
	var o bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&o), fSys, []string{"trice", "log", "-i", "til.json", "-p", "BUFFER", "-args", buffer, "-packageFraming", "COBS", "-ts", "off", "-prefix", "off", "-tsf", "", "-li", "off", "-color", "off"}))
	return o.String()

}

func TestTriceCheck2(t *testing.T) {

	osFSys := &afero.Afero{Fs: afero.NewOsFs()}
	mmFSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	wd := "d:/repos/trice/test/"

	CopyFileIntoFSys(t, mmFSys, "til.json", osFSys, wd+"testdata/til.json") // needed for the trice log
	out := make([]byte, 32768)
	SetTriceBuffer(out)

	result := GetExpectedResults(osFSys, wd+"testdata/generated_triceCheck.c")

	for i, r := range result {

		fmt.Println(i, r)

		// target activity
		TriceCheck(r.Line)
		length := TriceOutDepth()
		bin := out[:length] // bin contains the binary trice data of trice message i

		buf := fmt.Sprint(bin)
		buffer := buf[1 : len(buf)-1]

		act := triceLog2(t, mmFSys, buffer)
		assert.Equal(t, r.Exp, strings.TrimSuffix(act, "\n"))
	}
}

/*
type logf func(t *testing.T, fSys *afero.Afero, buffer string) string

func TriceCheck3(f logf, t *testing.T, fSys *afero.Afero, buffer string) {

	osFSys := &afero.Afero{Fs: afero.NewOsFs()}
	mmFSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	wd := "d:/repos/trice/test/"

	CopyFileIntoFSys(t, mmFSys, "til.json", osFSys, wd+"testdata/til.json") // needed for the trice log
	out := make([]byte, 32768)
	SetTriceBuffer(out)

	result := GetExpectedResults(osFSys, wd+"testdata/generated_triceCheck.c")

	for i, r := range result {

		fmt.Println(i, r)

		// target activity
		TriceCheck(r.Line)
		length := TriceOutDepth()
		bin := out[:length] // bin contains the binary trice data of trice message i

		buf := fmt.Sprint(bin)
		buffer := buf[1 : len(buf)-1]

		act := f(t, mmFSys, buffer)
		assert.Equal(t, r.Exp, strings.TrimSuffix(act, "\n"))
	}
}


func Test(  t *testing.T ){
	TriceCheck3( triceLog2, t, )
}
*/
