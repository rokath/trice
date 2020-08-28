// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// small helper functions for quick copy into packages code
package ...

import (
	"fmt"
	"io/ioutil"
	"log"
	"math/rand"
	"path/filepath"
	"reflect"
	"runtime"
	"strings"
	"testing"
	"time"
)

// test helper ///////////////////////////////////////////////////////////////////////
//

// assert fails the test if the condition is false.
func assert(tb testing.TB, condition bool, msg string, v ...interface{}) {
	if !condition {
		_, file, line, _ := runtime.Caller(1)
		fmt.Println(msg, filepath.Base(file), line, v...)
		tb.FailNow()
	}
}

// assertNil
// errorFail fails the test if an err is not nil.
func errorFail(tb testing.TB, err error) {
	if err != nil {
		_, file, line, _ := runtime.Caller(1)
		fmt.Println(err.Error(), filepath.Base(file), line)
		tb.FailNow()
	}
}

// assertEqual
// notEqualFail fails the test if exp is not equal to act.
func notEqualFail(tb testing.TB, exp, act interface{}) {
	if !reflect.DeepEqual(exp, act) {
		_, file, line, _ := runtime.Caller(1)
		log.Println("expect:", exp)
		log.Println("actual:", act)
		fmt.Println(filepath.Base(file), line)
		tb.FailNow()
	}
}

// assertEqualFiles
// notEqualFilesFail fails test if contence is NOT equal
func notEqualFilesFail(t *testing.T, fn0, fn1 string) {
	ok := equalFileContent(fn0, fn1)
	if false == ok {
		t.FailNow()
	}
}

// randomDynIPPort returns a random IP port number for testing.
// use 'rand.Seed(time.Now().UTC().UnixNano())' before to get different numbers on each first call.
func randomDynIPPort() (s string) {
	min := 49152
	max := 65535
	s = fmt.Sprint(rand.Intn(max-min) + min)
	return
}

// checkDisplay is an object used for testing.
// It implements the lineWriter interface.
type checkDisplay struct {
	lines []string
}

// newCheckDisplay creates a Display. It provides a lineWriter.
func newCheckDisplay() *checkDisplay {
	p := &checkDisplay{}
	return p
}

// writeLine is the implemented lineWriter interface for checkDisplay.
// It appends written lines to the internal data.
func (p *checkDisplay) writeLine(line []string) {
	s := strings.Join(line, "")
	p.lines = append(p.lines, s)
}

func (p *checkDisplay) checkLines(t *testing.T, lines []string) {
	notEqualFail(t, p.lines, lines)
}

//
// test helper ///////////////////////////////////////////////////////////////////////

// equalFileContent returns true if contece is equal
func equalFileContent(fn0, fn1 string) bool {
	cmp := equalfile.New(nil, equalfile.Options{}) // compare using single mode
	ok, err := cmp.CompareFile(fn0, fn1)
	if nil != err {
		ok = false
	}
	return ok
}

// readLines reads all lines of a file into lines.
// If file ends with a newline the last line in lines is empty.
// Do not use with big files.
func readLines(filename string) (lines []string, err error) {
	content, err := ioutil.ReadFile(filename)
	stringContent := string(content)
	if runtime.GOOS == "windows" {
		stringContent = strings.ReplaceAll(stringContent, "\r\n", "\n")
	}
	lines = strings.Split(stringContent, "\n")
	return
}

// readLines writes string slice containing lines to file
// https://stackoverflow.com/questions/7424340/read-in-lines-in-a-text-file-sort-then-overwrite-file
func readLines(file string) ([]string, error) {
	var lines []string
	f, err := os.Open(file)
	if err != nil {
		return nil, err
	}
	defer f.Close()
	r := bufio.NewReader(f)
	for {
		const delim = '\n'
		line, err := r.ReadString(delim)
		if err == nil || len(line) > 0 {
			if err != nil {
				line += string(delim)
			}
			lines = append(lines, line)
		}
		if err != nil {
			if err == io.EOF {
				break
			}
			return nil, err
		}
	}
	return lines, nil
}

// writeLines writes string slice containing lines to file
func writeLines(file string, lines []string) error {
	f, err := os.Create(file)
	if err != nil {
		return err
	}
	defer f.Close()
	w := bufio.NewWriter(f)
	defer w.Flush()
	for _, line := range lines {
		_, err := w.WriteString(line)
		if err != nil {
			return err
		}
	}
	return nil
}

// SortLines reads inFile, sorts the lines and writes to outFile
// inFile and outFile can be the same
func sortLines(inFile, outFile string) {
    lines, err := ReadLines(inFile)
    if err != nil {
        fmt.Println(err)
        os.Exit(1)
    }
    sort.Strings(lines)
    err = WriteLines(OutFile, lines)
    if err != nil {
        fmt.Println(err)
        os.Exit(1)
    }
}

// uniqueString removes string duplicates in string slice
// https://www.golangprograms.com/remove-duplicate-values-from-slice.html
func uniqueString(ss []string) []string {
	keys := make(map[string]bool)
	//list := []string{}
	var list []string
	for _, entry := range ss {
		if _, value := keys[entry]; !value {
			keys[entry] = true
			list = append(list, entry)
		}
	}
	return list
}

// uniqLines reads inFile, removes duplicate lines and writes to outFile
// inFile and outFile can be the same
// it returns count of remaining lines plus 1 (io.EOF ?)
func uniqLines(t *testing.T, inFile, outFile string) int {
	il, err := ReadLines(inFile)
	Ok(t, err)
	ol := uniqueString(il)
	err = WriteLines(outFile, ol)
	Ok(t, err)
	return len(ol)
}

// cleanFile accepts a string representing a path and converts \r\n into \n
// (taken from https://github.com/cgati/dos2unix/blob/master/main.go)
func cleanFile(fileName string) {
	file, err := os.Open(fileName)
	if err != nil {
		panic(err)
	}
	defer file.Close()

	wFile, err := os.Create(fileName + ".clean")
	if err != nil {
		panic(err)
	}
	defer wFile.Close()
	w := bufio.NewWriter(wFile)

	data := make([]byte, 128)
	for {
		data = data[:cap(data)]
		n, err := file.Read(data)
		if err != nil {
			if err == io.EOF {
				break
			}
		}
		data = data[:n]
		var indexesToDelete []int
		for i, b := range data {
			if b == '\r' {
				if i+1 < len(data) {
					if data[i+1] == '\n' {
						indexesToDelete = append(indexesToDelete, i)
					}
				}
				data[i] = '\n'
			}
		}
		data = removeFromSlice(data, indexesToDelete)
		w.Write(data)
	}
	w.Flush()
}

// removeFromSlice accepts a byte slice and an integer slice
// and deletes each index from the integer slice from the
// byte slice
// (taken from https://github.com/cgati/dos2unix/blob/master/main.go)
func removeFromSlice(data []byte, indexes []int) []byte {
	for _, i := range indexes {
		data = append(data[:i], data[i+1:]...)
	}
	return data
}

func fileLine() string {
	_, fileName, fileLine, ok := runtime.Caller(1)
	var s string
	if ok {
		s = fmt.Sprintf("%s:%d", fileName, fileLine)
	} else {
		s = ""
	}
	return s
}
