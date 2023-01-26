// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package cgo is a helper for testing the target C-code.
// Each C function gets a Go wrapper which is tested in appropriate test functions.
// For some reason inside the trice_test.go an 'import "C"' is not possible.
// All C-files in this folder referring to the trice sources this way avoiding code duplication.
// The Go functions defined here are not exported. They are called by the Go test functions in this package.
// This way the test functions are executing the trice C-code compiled with the triceConfig.h here.
package cgo

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
// #include "../testdata/triceCheck.c"
// #include "../testdata/cgoTrice_EditThisFile.c"
import "C"

// #include "../../src/cobs.c"

import (
	"bufio"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"testing"
	"unsafe"

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

// CopyFileIntoFSys copies fileName with its basename into fSys.
func CopyFileIntoFSys(t *testing.T, fSys *afero.Afero, fileName string) {
	r, e := os.Open(fileName)
	assert.Nil(t, e)

	w, e := fSys.Create(filepath.Base(fileName))
	assert.Nil(t, e)
	_, e = io.Copy(w, r)
	assert.Nil(t, e)
	assert.Nil(t, r.Close())
	assert.Nil(t, w.Close())
}
