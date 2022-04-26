// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package args

import (
	"io/ioutil"
	"os"
	"testing"

	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/pkg/msg"
	"github.com/rokath/trice/pkg/tst"
	"github.com/tj/assert"
)

func _TestMain(m *testing.M) {
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

//  func _TestComX(t *testing.T) {
//  	args := []string{"trice", "log", "-p", "COMX", "-idList", "emptyFile"}
//  	expect := `can not open COMX`
//  	execHelper(t, args, expect)
//  }

func TestVersion(t *testing.T) {
	verbose = false
	v := []string{"", ""}
	testVersion(t, v)
	//m.Lock()
	verbose = true
	v[0] = "No logfile writing...\nhttps://github.com/rokath/trice\n"
	v[1] = "No logfile writing...done\n"
	testVersion(t, v)
	verbose = false
	//m.Unlock()
}

func _TestScan(t *testing.T) {
	fn := func() {
		err := Handler(os.Stdout, []string{"trice", "scan"})
		assert.Nil(t, err)
	}
	fn()
	act := tst.CaptureStdOut(fn)
	s := "Found port:"
	assert.Equal(t, s, act[:len(s)])
}

func testVersion(t *testing.T, v []string) {
	fi, err := os.Stat(os.Args[0])
	assert.Nil(t, err)
	buildTime := fi.ModTime().String()
	exp := v[0] + "version=devel, built " + buildTime + "\n" + v[1]

	fn := func() {
		msg.OnErr(Handler(os.Stdout, []string{"trice", "ver"}))
	}
	act := tst.CaptureStdOut(fn)
	assert.Equal(t, exp, act)

	fn = func() {
		msg.OnErr(Handler(os.Stdout, []string{"trice", "version"}))
	}
	act = tst.CaptureStdOut(fn)
	assert.Equal(t, exp, act)
}

/*
func _TestShutdown(t *testing.T) { // crashes

	//defer func() {
	//	msg.OsExitAllow(o)
	//}()
	log.SetFlags(0)
	fn := func() {
		o := msg.OsExitDisallow()
		err := Handler([]string{"trice", "sd"})
		assert.Nil(t,err)
		msg.OsExitAllow(o)
	}
	act := tst.CaptureStdOut(fn)
	s := "Found port:"
	assert.Equal(t, s, act[:len(s)])
}
*/
