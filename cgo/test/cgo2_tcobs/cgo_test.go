package cgo

import (
	"bufio"
	"bytes"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"strings"
	"testing"

	"github.com/rokath/trice/internal/args"
	"github.com/spf13/afero"
	"github.com/tj/assert"
)

func _TestTriceSequences(t *testing.T) {
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

// copyFileIntoFSys copies fileName with its basename into fSys.
func copyFileIntoFSys(t *testing.T, fSys *afero.Afero, fileName string) {
	r, e := os.Open(fileName)
	assert.Nil(t, e)

	w, e := fSys.Create(filepath.Base(fileName))
	assert.Nil(t, e)
	_, e = io.Copy(w, r)
	assert.Nil(t, e)
	assert.Nil(t, r.Close())
	assert.Nil(t, w.Close())
}

// LinesInFile does get the lines in a file and store them in a string slice. Use the bufio and os imports.
func LinesInFile(fh afero.File) []string { // https://www.dotnetperls.com/lines-file-go
	//f, _ := os.Open(fileName)
	// Create new Scanner.
	scanner := bufio.NewScanner(fh)
	result := []string{}
	// Use Scan.
	for scanner.Scan() {
		line := scanner.Text()
		// Append line to result.
		result = append(result, line)
	}
	return result
}

func TestTriceCheck(t *testing.T) {

	// prepare
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	copyFileIntoFSys(t, fSys, "triceCheck.c")

	out := make([]byte, 32768)
	setTriceBuffer(out)

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
		triceCheck(i)
		length := triceOutDepth()
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
