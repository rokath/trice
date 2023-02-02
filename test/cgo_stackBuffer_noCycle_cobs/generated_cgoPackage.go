// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package cgot is a helper for testing the target C-code.
// Each C function gets a Go wrapper which is tested in appropriate test functions.
// For some reason inside the trice_test.go an 'import "C"' is not possible.
// All C-files in this folder referring to the trice sources this way avoiding code duplication.
// The Go functions defined here are not exported. They are called by the Go test functions in this package.
// This way the test functions are executing the trice C-code compiled with the triceConfig.h here.
package cgot

// #include <stdint.h>
// void TriceCheck( int n );
// unsigned TriceOutDepth( void );
// void CgoSetTriceBuffer( uint8_t* buf );
// #cgo CFLAGS: -g -Wall -I../../src
// #include "../../src/trice.c"
// #include "../../src/xtea.c"
// #include "../../src/triceStreamBuffer.c"
// #include "../../src/triceStackBuffer.c"
// #include "../../src/triceDoubleBuffer.c"
// #include "../../src/tcobsv1Encode.c"
// #include "../../src/tcobsv1Decode.c"
// #include "../testdata/generated_triceCheck.c"
// #include "../testdata/cgoTrice.c"
import "C"

// #include "../../src/cobs.c"

import (
	"bufio"
	"fmt"
	"io"
	"path/filepath"
	"strings"
	"testing"
	"unsafe"

	"github.com/rokath/trice/pkg/msg"
	"github.com/spf13/afero"
	"github.com/tj/assert"
)

// SetTriceBuffer tells the underlying C code where to output the trice byte stream.
func SetTriceBuffer(o []byte) {
	Cout := (*C.uchar)(unsafe.Pointer(&o[0]))
	C.CgoSetTriceBuffer(Cout)
}

// TriceCheck performs triceCheck C-code sequence n.
func TriceCheck(n int) {
	C.TriceCheck(C.int(n))
}

// TriceOutDepth returns the actual out buffer depth.
func TriceOutDepth() int {
	return int(C.TriceOutDepth())
}

// Dump prints the byte slice as hex in one line.
func Dump(w io.Writer, b []byte) {
	fmt.Fprint(w, "exp := []byte{ ")
	for _, x := range b {
		fmt.Fprintf(w, "0x%02x, ", x)
	}
	fmt.Fprintln(w, "}")
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

// CopyFileIntoFSys copies srcFn with its basename into destFSys as destFn.
func CopyFileIntoFSys(t *testing.T, destFSys *afero.Afero, destFn string, srcFSys *afero.Afero, srcFn string) {
	r, e := srcFSys.Open(srcFn)
	assert.Nil(t, e)

	w, e := destFSys.Create(filepath.Base(destFn))
	assert.Nil(t, e)

	_, e = io.Copy(w, r)
	assert.Nil(t, e)
	assert.Nil(t, r.Close())
	assert.Nil(t, w.Close())
}

type Result struct {
	Line int
	Exp  string
}

func GetExpectedResults(fSys *afero.Afero, filename string) (result []Result) {
	// get all file lines into a []string
	f, e := fSys.Open(filename)
	msg.OnErr(e)
	lines := LinesInFile(f)

	for i, line := range lines {
		subStr := "//exp: "
		fmt.Printf("line%4d: %s", i, line)
		index := strings.LastIndex(line, subStr)
		if index > 0 {
			var r Result
			r.Line = i + 1 // 1st line number is 1 and not 0
			r.Exp = line[index+len(subStr)+1 : len(line)-1]
			result = append(result, r)
		}
	}
	return
}
