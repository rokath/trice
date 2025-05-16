// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package tst provides some helper functions for testing.
package tst

import (
	"bytes"
	"fmt"
	"io"
	"io/ioutil"
	"os"
	"regexp"
	"strings"
	"testing"

	"github.com/rokath/trice/pkg/msg"
	"github.com/spf13/afero"

	"github.com/stretchr/testify/assert"

	"github.com/udhos/equalfile"
)

// TempFileName returns a temporary file name based on pattern.
// The file is created and deleted and only the name is delivered.
// The pattern should contain at least an asterisks. Example: "myFile-*.bin"
func TempFileName(pattern string) (s string) {
	tempFileHandle, e := ioutil.TempFile(os.TempDir(), pattern) // opens for read and write
	msg.OnErr(e)
	s = tempFileHandle.Name()
	msg.OnErr(tempFileHandle.Close())
	msg.InfoOnErr(os.Remove(s), s)
	return
}

// CaptureStdOut captures stdout and returns it as string.
func CaptureStdOut(f func()) string {
	old := os.Stdout // keep backup of the real stdout

	// re-direct stdout
	r, w, err := os.Pipe()
	msg.FatalOnErr(err)
	os.Stdout = w

	// copy the output in a separate goroutine so printing can't block indefinitely
	outC := make(chan string)
	go func() {
		var buf bytes.Buffer
		_, err = io.Copy(&buf, r)
		msg.OnErr(err)
		outC <- buf.String()
	}()

	f() // run the function

	// back to normal state
	msg.FatalOnErr(w.Close())
	os.Stdout = old

	// read output
	return <-outC
}

func standardizeSpaces(s string) string {
	return strings.Join(strings.Fields(s), " ")
}

// EqualLines compares exp and act line by line ignoring line endings and leading/trailing spaces.
func EqualLines(tb testing.TB, exp, act string) {

	// remove windows line endings
	//exp0 := strings.ReplaceAll(exp, "\r", "")
	//act0 := strings.ReplaceAll(act, "\r", "")
	//exp1 := strings.ReplaceAll(exp0, "\t", " ")
	//act1 := strings.ReplaceAll(act0, "\t", " ")
	//exp2 := strings.ReplaceAll(exp1, "  ", " ")
	//act2 := strings.ReplaceAll(act1, "  ", " ")

	expS := strings.Split(exp, "\n")
	actS := strings.Split(act, "\n")

	//fmt.Println("lines:", len(expS), len(actS))
	//assert.True(tb, len(expS) == len(actS))

	//if len(expS) != len(actS) {
	//	tb.Fail()
	//	return
	//}
	for i := range actS {
		e := standardizeSpaces(expS[i])
		a := standardizeSpaces(actS[i])
		if e != a {
			fmt.Println(i, "expLine:"+e)
			fmt.Println(i, "actLine:"+a)
			fmt.Println(i, "expLine:"+expS[i])
			fmt.Println(i, "actLine:"+actS[i])
			tb.Fail()
			return
		}
	}
}

// EqualTextFiles fails test if lines in fn0 & fn1 NOT equal.
// Line endings are ignored
func EqualTextFiles(t *testing.T, fSys *afero.Afero, fn0, fn1 string) {

	// Read entire file content, giving us little control but
	// making it very simple. No need to close the file.
	b0, e0 := fSys.ReadFile(fn0)
	assert.Nil(t, e0)
	b1, e1 := fSys.ReadFile(fn1)
	assert.Nil(t, e1)

	// Convert []byte to string and print to screen
	s0 := string(b0)
	s1 := string(b1)

	EqualLines(t, s0, s1)
}

// AssertEqualFiles fails test if content is NOT equal
func AssertEqualFiles(t *testing.T, fn0, fn1 string) {
	cmp := equalfile.New(nil, equalfile.Options{}) // compare using single mode
	ok, err := cmp.CompareFile(fn0, fn1)
	assert.Nil(t, err)
	assert.True(t, ok)
}

// NormalizeMapString trims insignificant whitespace in Go-style map[string]struct formatting.
//
// It is useful for comparing stringified maps where `fmt.Sprint(map)` includes
// formatting inconsistencies such as:
//   - trailing spaces after struct fields (e.g., "{t11 s11 }")
//   - extra spaces between map entries (e.g., "12:{... }")
//
// This behavior may vary subtly between Go versions (e.g., Go 1.18 vs 1.20+),
// where `fmt.Sprint` preserves more internal spacing from struct formatting.
//
// normalizeMapString helps produce stable, human-readable string output
// suitable for comparison in tests without altering the logical content.
func NormalizeMapString(s string) string {
	// Trim space after struct close
	s = regexp.MustCompile(`\s+}`).ReplaceAllString(s, "}")
	// Trim space before struct open
	s = regexp.MustCompile(`{\s+`).ReplaceAllString(s, "{")
	// Remove extra space after closing } of each map entry
	s = regexp.MustCompile(`}\s+`).ReplaceAllString(s, "} ")
	// Normalize single space between key:value entries
	s = regexp.MustCompile(`\s+`).ReplaceAllString(s, " ")
	return strings.TrimSpace(s)
}
