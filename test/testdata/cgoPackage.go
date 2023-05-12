// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package cgot is a helper for testing the target C-code.
// Each C function gets a Go wrapper which is tested in appropriate test functions.
// For some reason inside the trice_test.go an 'import "C"' is not possible.
// The C-files referring to the trice sources this way avoiding code duplication.
// The Go functions defined here are not exported. They are called by the Go test functions in this package.
// This way the test functions are executing the trice C-code compiled with the triceConfig.h here.
// Inside ./testdata this file is named cgoPackage.go where it is maintained.
// The test/updateTestData.sh script copied this file under the name generated_cgoPackage.go into various
// package folders, where it is used separately.
package cgot

// #include <stdint.h>
// void TriceCheck( int n );
// void TriceTransfer( void );
// unsigned TriceOutDepth( void );
// void CgoSetTriceBuffer( uint8_t* buf );
// void CgoClearTriceBuffer( void );
// #cgo CFLAGS: -g -I../../src
// #include "../../src/cobsDecode.c"
// #include "../../src/cobsEncode.c"
// #include "../../src/tcobsv1Decode.c"
// #include "../../src/tcobsv1Encode.c"
// #include "../../src/trice.c"
// #include "../../src/trice16.c"
// #include "../../src/trice32.c"
// #include "../../src/trice64.c"
// #include "../../src/trice8.c"
// #include "../../src/triceAuxiliary.c"
// #include "../../src/triceDoubleBuffer.c"
// #include "../../src/triceRingBuffer.c"
// #include "../../src/triceStackBuffer.c"
// #include "../../src/triceStaticBuffer.c"
// #include "../../src/xtea.c"
// #include "../testdata/triceCheck.c"
// #include "../testdata/cgoTrice.c"
import "C"

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
	triceDir  string // triceDir holds the trice directory path.
	testLines = 15   // testLines is the common number of tested lines in triceCheck. The value -1 is for all lines, what takes time.
)

type triceMode int

const (
	directTransfer triceMode = iota
	deferredTransfer
)

// https://stackoverflow.com/questions/23847003/golang-tests-and-working-directory
func init() {
	_, filename, _, _ := runtime.Caller(0) // filename is the test executable inside the package dir like cgo_stackBuffer_noCycle_tcobs
	testDir := path.Dir(filename)
	triceDir = path.Join(testDir, "../../")
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

// triceTransfer performs the deferred trice output.
func triceTransfer() {
	C.TriceTransfer()
}

// triceOutDepth returns the actual out buffer depth.
func triceOutDepth() int {
	return int(C.TriceOutDepth())
}

// triceClearOutBuffer tells the trice kernel, that the data has been red.
func triceClearOutBuffer() {
	C.CgoClearTriceBuffer()
}

// linesInFile does get the lines in a file and stores them in a string slice.
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
		s := strings.Split(line, "//")
		if len(s) == 2 { // just one "//"
			lineEnd := s[1]
			subStr := "exp:"
			index := strings.LastIndex(lineEnd, subStr)
			if index >= 0 {
				var r results
				r.line = i + 1 // 1st line number is 1 and not 0
				r.exps = strings.TrimSpace(lineEnd[index+len(subStr) : len(lineEnd)])
				result = append(result, r)
			}
		}
	}
	return
}

// logF is the log function type for executing the trice logging on binary log data in buffer as space separated numbers.
// It uses the inside fSys specified til.json and returns the log output.
type logF func(t *testing.T, fSys *afero.Afero, buffer string) string

// triceLogTest creates a list of expected results from triceDir+"./test/testdata/triceCheck.c".
// It loops over the result list and executes for each result the compiled C-code.
// It passes the received binary data as buffer to the triceLog function of type logF.
// This function is test package specific defined. The file cgoPackage.go is
// copied into all specific test packages and compiled there together with the
// triceConfig.h, which holds the test package specific target code configuration.
// limit is the count of executed test lines starting from the beginning. -1 ist for all.
func triceLogTest(t *testing.T, triceLog logF, limit int, mode triceMode) {

	osFSys := &afero.Afero{Fs: afero.NewOsFs()}
	//mmFSys := &afero.Afero{Fs: afero.NewMemMapFs()}

	// CopyFileIntoFSys(t, mmFSys, "til.json", osFSys, td+"./til.json") // needed for the trice log
	out := make([]byte, 32768)
	setTriceBuffer(out)

	result := getExpectedResults(osFSys, triceDir+"./test/testdata/triceCheck.c")

	var count int
	for i, r := range result {

		count++
		if limit >= 0 && count >= limit {
			return
		}
		fmt.Println(i, r)

		// target activity
		triceCheck(r.line)
		if mode == deferredTransfer {
			triceTransfer()
		}
		length := triceOutDepth()
		bin := out[:length] // bin contains the binary trice data of trice message i

		buf := fmt.Sprint(bin)
		buffer := buf[1 : len(buf)-1]

		act := triceLog(t, osFSys, buffer)
		triceClearOutBuffer()

		assert.Equal(t, r.exps, strings.TrimSuffix(act, "\n"))
	}
}
