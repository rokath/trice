// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package cgot is a helper for testing the target C-code.
// Each C function gets a Go wrapper which is tested in appropriate test functions.
// For some reason inside the trice_test.go an 'import "C"' is not possible.
// The C-files referring to the trice sources this way avoiding code duplication.
// The Go functions defined here are not exported. They are called by the Go test functions in this package.
// This way the test functions are executing the trice C-code compiled with the triceConfig.h here.
// Inside ./testdate this file is named cgoPackage.go where it is maintained.
// The test/updateTestData.sh script copied this file under the name generated_cgoPackage.go into various
// package folders, where it is used separately.
package cgot

// #include <stdint.h>
// void TriceCheck( int n );
// unsigned TriceOutDepth( void );
// void CgoSetTriceBuffer( uint8_t* buf );
// #cgo CFLAGS: -g -Wall -I../../src
// #include "../../src/trice.c"
// #include "../testdata/generated_triceCheck.c"
// #include "../testdata/cgoTrice.c"
import "C"

// #include "../../src/cobs.c"

import (
	"bufio"
	"fmt"
	"path"
	"runtime"
	"strings"
	"testing"
	"unsafe"

	"github.com/rokath/trice/pkg/msg"
	"github.com/spf13/afero"
	"github.com/tj/assert"
)

var (
	testDataDir string // holds the testdata directory path
)

// https://stackoverflow.com/questions/23847003/golang-tests-and-working-directory
func init() {
	_, filename, _, _ := runtime.Caller(0)
	testDataDir = path.Join(path.Dir(filename), "../testdata")
}

// setTriceBuffer tells the underlying C code where to output the trice byte stream.
func setTriceBuffer(o []byte) {
	Cout := (*C.uchar)(unsafe.Pointer(&o[0]))
	C.CgoSetTriceBuffer(Cout)
}

// triceCheck performs triceCheck C-code sequence n.
func triceCheck(n int) {
	C.TriceCheck(C.int(n))
}

// triceOutDepth returns the actual out buffer depth.
func triceOutDepth() int {
	return int(C.TriceOutDepth())
}

// linesInFile does get the lines in a file and store them in a string slice.
func linesInFile(fh afero.File) []string { // https://www.dotnetperls.com/lines-file-go
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

// results contains the expected result string exps for line number line.
type results struct {
	line int
	exps string
}

func getExpectedResults(fSys *afero.Afero, filename string) (result []results) {
	// get all file lines into a []string
	f, e := fSys.Open(filename)
	msg.OnErr(e)
	lines := linesInFile(f)

	for i, line := range lines {
		subStr := "//exp: "
		index := strings.LastIndex(line, subStr)
		if index > 0 {
			var r results
			r.line = i + 1 // 1st line number is 1 and not 0
			r.exps = line[index+len(subStr)+1 : len(line)-1]
			result = append(result, r)
		}
	}
	return
}

// logF is the log function type for executing the trice logging on binary log data in buffer as space separated numbers.
// It uses the inside fSys specified til.json and returns the log output.
type logF func(t *testing.T, fSys *afero.Afero, buffer string) string

// triceLogTest
func triceLogTest(t *testing.T, triceLog logF) {

	osFSys := &afero.Afero{Fs: afero.NewOsFs()}
	//mmFSys := &afero.Afero{Fs: afero.NewMemMapFs()}

	// https://stackoverflow.com/questions/23847003/golang-tests-and-working-directory
	_, filename, _, _ := runtime.Caller(0)
	td := path.Join(path.Dir(filename), "../testdata")

	// CopyFileIntoFSys(t, mmFSys, "til.json", osFSys, td+"./til.json") // needed for the trice log
	out := make([]byte, 32768)
	setTriceBuffer(out)

	result := getExpectedResults(osFSys, td+"./generated_triceCheck.c")

	for i, r := range result {

		fmt.Println(i, r)

		// target activity
		triceCheck(r.line)
		length := triceOutDepth()
		bin := out[:length] // bin contains the binary trice data of trice message i

		buf := fmt.Sprint(bin)
		buffer := buf[1 : len(buf)-1]

		act := triceLog(t, osFSys, buffer)
		assert.Equal(t, r.exps, strings.TrimSuffix(act, "\n"))
	}
}

//  // CopyFileIntoFSys copies srcFn with its basename into destFSys as destFn.
//  func CopyFileIntoFSys(t *testing.T, destFSys *afero.Afero, destFn string, srcFSys *afero.Afero, srcFn string) {
//  	r, e := srcFSys.Open(srcFn)
//  	assert.Nil(t, e)
//
//  	w, e := destFSys.Create(filepath.Base(destFn))
//  	assert.Nil(t, e)
//
//  	_, e = io.Copy(w, r)
//  	assert.Nil(t, e)
//  	assert.Nil(t, r.Close())
//  	assert.Nil(t, w.Close())
//  }

//  // Dump prints the byte slice as hex in one line.
//  func Dump(w io.Writer, b []byte) {
//  	fmt.Fprint(w, "exp := []byte{ ")
//  	for _, x := range b {
//  		fmt.Fprintf(w, "0x%02x, ", x)
//  	}
//  	fmt.Fprintln(w, "}")
//  }
