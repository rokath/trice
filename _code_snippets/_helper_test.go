// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// small helper functions for quick copy into packages code
package

import (
	"bufio"
	"bytes"
	"fmt"
	"github.com/udhos/equalfile"
	"io"
	"io/ioutil"
	"log"
	"math/rand"
	"os"
	"path/filepath"
	"reflect"
	"runtime"
	"sort"
	"strings"
	"testing"
)
...

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



// randomDynIPPort returns a random IP port number for testing.
// use 'rand.Seed(time.Now().UTC().UnixNano())' before to get different numbers on each first call.
func randomDynIPPort() (s string) {
	min := 49152
	max := 65535
	s = fmt.Sprint(rand.Intn(max-min) + min)
	return
}

// // checkDisplay is an object used for testing.
// // It implements the Linewriter interface.
// type checkDisplay struct {
// 	lines []string
// }

// newCheckDisplay creates a Display. It provides a Linewriter.
func newCheckDisplay() *checkDisplay {
	p := &checkDisplay{}
	return p
}

// writeLine is the implemented Linewriter interface for checkDisplay.
// It appends written lines to the internal data.
func (p *checkDisplay) writeLine(line []string) {
	s := strings.Join(line, "")
	p.lines = append(p.lines, s)
}

func (p *checkDisplay) checkLines(t *testing.T, lines []string) {
	tst.Equal(t, p.lines, lines)
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
	defer func(){ msg.InfoOnErr( "", f.Close() }()
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
	defer func(){ msg.InfoOnErr( "", f.Close() }()
	w := bufio.NewWriter(f)
	defer func(){ msg.InfoOnErr( "", f.Flush() }()
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
    err = WriteLines(outFile, lines)
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
	defer func(){ msg.InfoOnErr( "", file.Close() }()

	wFile, err := os.Create(fileName + ".clean")
	if err != nil {
		panic(err)
	}
	defer func(){ msg.InfoOnErr( "", wFile.Close() }()
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

// min returns the smaller int from a and b
func min(a, b int) int {
	if a < b {
		return a
	}
	return b
}


// tst.CaptureStdOut captures stdout and stderr.
func tst.CaptureStdOut(f func()) string {

	// keep backup of the real stdout
	old := os.Stdout

	// re-direct stdout
	r, w, err := os.Pipe()
	if nil != err {
		return err.Error()
	}
	os.Stdout = w
	defer func() {
		// restoring the real stdout
		os.Stdout = old
	}()

	// copy the output in a separate goroutine so printing can't block indefinitely
	outC := make(chan string)
	go func() {
		var buf bytes.Buffer
		io.Copy(&buf, r)
		outC <- buf.String()
	}()

	// run the function
	f()

	// back to normal state
	err = w.Close()
	if nil != err {
		return err.Error()
	}

	// read output
	return <-outC
}

type bytesViewer struct {
	r io.Reader
}

func newBytesViewer(o io.Reader) (i io.Reader) {
	return &bytesViewer{o}
}

func (p *bytesViewer) Read(b []byte) (n int, e error) {
	n, e = p.r.Read(b)
	fmt.Println("bytesViewer:", e, n, b[:n])
	return
}


// test helper ///////////////////////////////////////////////////////////////////////
//

// tst.Equal fails the test if exp is not equal to act.
func tst.Equal(tb testing.TB, exp, act interface{}) {
	if !reflect.DeepEqual(exp, act) {
		_, file, line, _ := runtime.Caller(1)
		log.Println("expect:", exp)
		log.Println("actual:", act)
		es := fmt.Sprintf("%v", exp)
		as := fmt.Sprintf("%v", act)
		log.Println("expect:", es)
		log.Println("actual:", as)
		log.Println("expect:", []byte(es))
		log.Println("actual:", []byte(as))
		fmt.Println(filepath.Base(file), line)
		tb.FailNow()
	}
}

//
// test helper ///////////////////////////////////////////////////////////////////////