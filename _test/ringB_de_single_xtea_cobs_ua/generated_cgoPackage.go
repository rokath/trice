// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package cgot is a helper for testing the target C-code.
// Each C function gets a Go wrapper which is tested in appropriate test functions.
// For some reason inside the trice_test.go an 'import "C"' is not possible.
// The C-files referring to the trice sources this way avoiding code duplication.
// The Go functions defined here are not exported. They are called by the Go test functions in this package.
// This way the test functions are executing the trice C-code compiled with the triceConfig.h here.
// Inside ./testdata this file is named cgoPackage.go where it is maintained.
// The ../renewIDs_in_examples_and_test_folder.sh script copies this file under the name generated_cgoPackage.go into various
// package folders, where it is used separately.
package cgot

// #include <stdint.h>
// void TriceCheck( int n );
// void TriceTransfer( void );
// unsigned TriceOutDepth( void );
// void CgoSetTriceBuffer( uint8_t* buf );
// void CgoClearTriceBuffer( void );
// #cgo CFLAGS: -g -I../../src -Wshadow -Wno-format-security
// #include "../../src/trice.c"
// #include "../../src/trice8.c"
// #include "../../src/trice16.c"
// #include "../../src/trice32.c"
// #include "../../src/trice64.c"
// #include "../../src/triceUart.c"
// #include "../../src/triceAuxiliary.c"
// #include "../../src/triceDoubleBuffer.c"
// #include "../../src/triceRingBuffer.c"
// #include "../../src/triceStackBuffer.c"
// #include "../../src/triceStaticBuffer.c"
// #include "../../src/xtea.c"
// #include "../../src/cobsDecode.c"
// #include "../../src/cobsEncode.c"
// #include "../../src/tcobsv1Decode.c"
// #include "../../src/tcobsv1Encode.c"
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

	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/trexDecoder"
	"github.com/rokath/trice/pkg/cipher"
	"github.com/rokath/trice/pkg/msg"
	"github.com/spf13/afero"
	"github.com/tj/assert"
)

var (
	testLines       = 10   // testLines is the common number of tested lines in triceCheck. The value -1 is for all lines, what takes time.
	triceDir        string // triceDir holds the trice directory path.
	targetActivityC string // triceCheckC contains the target test code.
	targetMode      string // targetMode is "direct..." OR "deferred..." OR "combinedMode" (direct AND deferred mode) and must fit the triceConfig.h settings to adapt the tests.
)

// https://stackoverflow.com/questions/23847003/golang-tests-and-working-directory
func init() {
	_, filename, _, _ := runtime.Caller(0) // filename is the test executable inside the package dir like cgo_stackBuffer_noCycle_tcobs
	testDir := path.Dir(filename)
	triceDir = path.Join(testDir, "../../")
	targetActivityC = path.Join(triceDir, "./_test/testdata/triceCheck.c")
	C.TriceInit()
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

func getExpectedResults(fSys *afero.Afero, filename string, maxTestlines int) (result []results) {
	// get all file lines into a []string
	f, e := fSys.Open(filename)
	msg.OnErr(e)
	lines := linesInFile(f)
	var testLinesCounter = 0
	for i, line := range lines {
		s := strings.Split(line, "//")
		if len(s) == 2 { // just one "//"
			lineEnd := s[1]
			index := strings.LastIndex(lineEnd, "exp: \"")
			if index == 0 { // pattern //exp: "
				var r results
				r.line = i + 1 // 1st line number is 1 and not 0
				//  r.exps = strings.TrimSpace(lineEnd[index+len(subStr):])
				s := lineEnd[6 : len(lineEnd)-1]
				r.exps = strings.ReplaceAll(s, "\\n", "\n")
				result = append(result, r)
				testLinesCounter++
				if maxTestlines > 0 && testLinesCounter >= maxTestlines {
					break
				}
			}
		}
	}

	skipAtStart := 0
	skipAtEnd := 0

	from := skipAtStart
	if len(result) < skipAtStart {
		from = 0
	}

	to := len(result) - skipAtEnd
	if len(result) < skipAtEnd {
		to = len(result)
	}
	return result[from:to]
}

// logF is the log function type for executing the trice logging on binary log data in buffer as space separated numbers.
// It uses the inside fSys specified til.json and returns the log output.
type logF func(t *testing.T, fSys *afero.Afero, buffer string) string

// triceLogLineByLine executes each triceCheck.c test line, gets its binary output and
// restarts the whole Trice log functionality for this, resulting in a long test duration.
// This test is avoidable for only-deferred modes which allow doTestTriceLogBulk=true,
// but useful for debugging.
// triceLogLineByLine creates a list of expected results from triceCheckC.
// It loops over the result list and executes for each result the compiled C-code.
// It passes the received binary data as buffer to the triceLog function of type logF.
// This function is test package specific defined. The file cgoPackage.go is
// copied into all specific test packages and compiled there together with the
// triceConfig.h, which holds the test package specific target code configuration.
// testLines is the count of executed test lines starting from the beginning. -1 ist for all.
func triceLogLineByLine(t *testing.T, triceLog logF, testLines int, triceCheckC string) {
	osFSys := &afero.Afero{Fs: afero.NewOsFs()}
	// CopyFileIntoFSys(t, mmFSys, "til.json", osFSys, td+"./til.json") // needed for the trice log
	out := make([]byte, 32768)
	setTriceBuffer(out)
	result := getExpectedResults(osFSys, triceCheckC, testLines)
	for i, v := range result {
		triceCheck(v.line) // target activity
		triceTransfer()    // This is only for deferred modes needed, but direct modes contain this as empty function.
		length := triceOutDepth()
		bin := out[:length] // bin contains the binary trice data of trice message i in r.line
		buf := fmt.Sprint(bin)
		buffer := buf[1 : len(buf)-1]
		act := triceLog(t, osFSys, buffer)
		triceClearOutBuffer()
		assert.Equal(t, v.exps, act, fmt.Sprintf("%d: line %d: len(exp)=%d, len(act)=%d", i, v.line, len(v.exps), len(act)))
	}
}

// triceLogBulk executes each triceCheck.c test line, gets its binary output and
// collects all these outputs into one (big) buffer. Then the Trice log functionality
// is started once for this (big) buffer and the whole output is generated. Afterwards
// this generated output is compared line by line with the expected results. The
// function triceLogBulk is much faster than triceLogLineByLine but difficult to debug.
// triceLogBulk creates a list of expected results from triceCheckC.
// It loops over the result list and executes for each result the compiled C-code.
// It passes the received binary data as buffer to the triceLog function of type logF.
// This function is test package specific defined. The file cgoPackage.go is
// copied into all specific test packages and compiled there together with the
// triceConfig.h, which holds the test package specific target code configuration.
// testLines is the count of executed test lines starting from the beginning. -1 ist for all.
func triceLogBulk(t *testing.T, triceLog logF, testLines int, triceCheckC string) {
	osFSys := &afero.Afero{Fs: afero.NewOsFs()}
	// CopyFileIntoFSys(t, mmFSys, "til.json", osFSys, td+"./til.json") // needed for the trice log

	// out is the binary trice data buffer until the next triceTransfer() call.
	// It must be able to hold binary Trice data for several Trice calls and should have at least the
	// configured target internal buffer size, here 65536 is a very safe value.
	out := make([]byte, 65536)
	setTriceBuffer(out)
	result := getExpectedResults(osFSys, triceCheckC, testLines)
	var bin []byte // bin collects the binary data.
	bulk := 5
	for i, r := range result {
		fmt.Print("i:", i, "\texecute triceCheck.c line:", r.line, "\texp:", r.exps)
		triceCheck(r.line) // target activity

		// In case "#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE" wee need to call triceTransfer
		// at least that often a trice was executed. Just in case a test line produces more than one trice message,
		// we do it 2*bulk times
		if i%bulk == 0 {
			for range 2 * bulk { // collect three trice messages before transfer}
				triceTransfer() // This is only for deferred modes needed, but direct modes contain this as empty function.
				length := triceOutDepth()
				bin = append(bin, out[:length]...)
				setTriceBuffer(out)
			}
		}
	}

	// For safety do some more transfers to get the last messages.
	for range 2 * bulk { // collect three trice messages before transfer}
		triceTransfer() // This is only for deferred modes needed, but direct modes contain this as empty function.
		length := triceOutDepth()
		bin = append(bin, out[:length]...)
		setTriceBuffer(out)
	}

	buf := fmt.Sprint(bin)             // buf is the ASCII representation of bin.
	buffer := buf[1 : len(buf)-1]      // buffer contains the bare data (without brackets).
	act := triceLog(t, osFSys, buffer) // act is the complete printed text.
	for i, v := range result {
		if len(act) >= len(v.exps) {
			a := act[:len(v.exps)] // get next part of actual data (usually a line).
			assert.Equal(t, v.exps, a, fmt.Sprintf("%d: line %d: len(exp)=%d, len(act)=%d", i, v.line, len(v.exps), len(a)))
			act = act[len(v.exps):]
		} else {
			fmt.Println(i, "of", len(result), "v.exps:", v.exps)
			fmt.Println(len(act), "act:", act)
			assert.Fail(t, fmt.Sprintf("%d: line %d: len(exp)=%d, len(act)=%d", i, v.line, len(v.exps), len(act)), "actual data too short")
		}
	}
	//assert.Fail(t, "forced fail")
}

// triceLogDirectAndDeferred works like triceLogTest but additionally expects doubled output: direct and deferred.
func triceLogDirectAndDeferred(t *testing.T, triceLog0, triceLog1 logF, testLines int, triceCheckC string) {
	g.getGlobalVarsDefaults() // read changed defaults
	osFSys := &afero.Afero{Fs: afero.NewOsFs()}
	// CopyFileIntoFSys(t, mmFSys, "til.json", osFSys, td+"./til.json") // needed for the trice log
	out := make([]byte, 32768)
	setTriceBuffer(out)
	result := getExpectedResults(osFSys, triceCheckC, testLines)

	for i, v := range result {
		triceCheck(v.line) // target activity

		{ // Check direct output line by line.
			length := triceOutDepth()
			bin := out[:length] // bin contains the binary trice data of trice message i
			buf := fmt.Sprint(bin)
			buffer := buf[1 : len(buf)-1]
			g.setGlobalVarsDefaults() // restore changed defaults
			act := triceLog0(t, osFSys, buffer)
			triceClearOutBuffer()
			assert.Equal(t, v.exps, act, fmt.Sprint(i, v))
		}
		{ // Check deferred output.
			if false {

			} else { // liny by line (slow)
				triceTransfer()
				length := triceOutDepth()
				bin := out[:length] // bin contains the binary trice data of trice message i
				buf := fmt.Sprint(bin)
				buffer := buf[1 : len(buf)-1]
				g.setGlobalVarsDefaults() // restore changed defaults
				act := triceLog1(t, osFSys, buffer)
				triceClearOutBuffer()
				assert.Equal(t, v.exps, act, fmt.Sprint(i, v))
			}
		}
	}
}

type globalDefaults struct {
	defaultPassword       string
	defaultPackageFraming string
	defaultDoubled16BitID bool
}

var (
	// g holds global vars default values
	g globalDefaults

	// triceLog is the log function for executing the trice logging on binary log data in buffer as space separated numbers.
	// It uses the inside fSys specified til.json and returns the log output.
	triceLog         func(t *testing.T, fSys *afero.Afero, buffer string) string
	triceLogDirect   func(t *testing.T, fSys *afero.Afero, buffer string) string
	triceLogDeferred func(t *testing.T, fSys *afero.Afero, buffer string) string
)

// Keep default values of global variables.
func (p *globalDefaults) getGlobalVarsDefaults() {
	p.defaultPassword = cipher.Password
	p.defaultPackageFraming = decoder.PackageFraming
	p.defaultDoubled16BitID = trexDecoder.Doubled16BitID
}

// setGlobalVarsDefaults sets all global variables in a definitive state.
// In Go, each package generates an individual test binary and they are tested parallel.
// All package tests are executed sequentially but use the same global variables.
// Therefore we have to reset the global variables in each test function.
func (p *globalDefaults) setGlobalVarsDefaults() {
	cipher.Password = p.defaultPassword
	decoder.PackageFraming = p.defaultPackageFraming
	trexDecoder.Doubled16BitID = p.defaultDoubled16BitID
}
