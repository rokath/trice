// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package args

import (
	"bytes"
	"io/ioutil"
	"os"
	"testing"

	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/pkg/msg"
	"github.com/tj/assert"
)

func TestMain(m *testing.M) {
	id.FnJSON = getTemporaryFileName("til-*.JSON")
	code := m.Run()
	msg.OnErr(os.Remove(id.FnJSON))
	os.Exit(code) // os.Exit() does not respect defer statements
}

func getTemporaryFileName(pattern string) string {
	tempFileHandle, e := ioutil.TempFile(os.TempDir(), pattern) // opens for read and write
	msg.OnErr(e)
	tempFileName := tempFileHandle.Name()
	msg.OnErr(tempFileHandle.Close())
	return tempFileName
}

func TestHelp(t *testing.T) {
	args := []string{"trice", "help"}
	expect := `syntax: 'trice sub-command' [params]
      example 'trice h -help': Print help for help.
      `
	execHelper(t, args, expect)
}

func TestXyz(t *testing.T) {
	args := []string{"trice", "xyz"}
	expect := `unknown sub-command 'xyz'. try: 'trice help|h'`
	execHelper(t, args, expect)
}

func TestXyzStim(t *testing.T) {
	args := []string{"stim", "xyz"}
	expect := `unknown sub-command 'xyz'. try: 'stim help|h'`
	execHelper(t, args, expect)
}

func TestNoArgs(t *testing.T) {
	args := []string{"trice"}
	expect := `no args, try: 'trice help'`
	execHelper(t, args, expect)
}

func TestUpdate(t *testing.T) {
	args := []string{"trice", "update", "-idList", "emptyFile"}
	expect := ``
	execHelper(t, args, expect)
}

func TestUpdateV(t *testing.T) {
	args := []string{"trice", "update", "-idList", "emptyFile", "-verbose"}
	expect := `0 ID's in List emptyFile listModified= false
	`
	execHelper(t, args, expect)
}

func TestComX(t *testing.T) {
	args := []string{"trice", "log", "-p", "COMX", "-idList", "emptyFile"}
	expect := `can not open COMX
`
	execHelper(t, args, expect)
}

func TestScan(t *testing.T) {
	var act bytes.Buffer
	osFs := os.DirFS("")
	err := Handler(&act, osFs, []string{"trice", "scan"})
	assert.Nil(t, err)
	s := "Found port:"
	buf := act.Bytes()
	assert.Equal(t, s, string(buf[:len(s)]))
}

func TestVersion(t *testing.T) {
	verbose = false
	v := []string{"", ""}
	testVersion(t, v)

	verbose = true
	v[0] = "https://github.com/rokath/trice\n"
	v[1] = ""
	testVersion(t, v)
	verbose = false
}

func testVersion(t *testing.T, v []string) {
	fi, err := os.Stat(os.Args[0])
	assert.Nil(t, err)
	Date = fi.ModTime().String()
	exp := v[0] + "version=devel, built " + Date + "\n" + v[1]
	var buf0 bytes.Buffer
	osFs := os.DirFS("")
	msg.OnErr(Handler(&buf0, osFs, []string{"trice", "ver"}))
	act0 := buf0.String()
	assert.Equal(t, exp, act0)

	var buf1 bytes.Buffer
	msg.OnErr(Handler(&buf1, osFs, []string{"trice", "version"}))
	act1 := buf1.String()
	assert.Equal(t, exp, act1)
}
