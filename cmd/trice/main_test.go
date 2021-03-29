// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package main

import (
	"fmt"
	"io/ioutil"
	"os"
	"testing"

	"github.com/rokath/trice/pkg/msg"
	"github.com/rokath/trice/pkg/tst"
	"github.com/tj/assert"
)

func getTemporaryFileName(pattern string) string {
	tempFileHandle, e := ioutil.TempFile(os.TempDir(), pattern) // opens for read and write
	msg.OnErr(e)
	tempFileName := tempFileHandle.Name()
	msg.OnErr(tempFileHandle.Close())
	return tempFileName
}

func TestMain(m *testing.M) {
	// do stuff before tests
	exitVal := m.Run()
	// do stuff after tests
	os.Exit(exitVal)
}

func Example_doit_a() {
	version = "1.2.3"
	commit = "myCommit"
	date = "2006-01-02_1504-05"
	os.Args = []string{"trice", "ver"}
	act := tst.CaptureStdOut(doit)
	fmt.Print(act)
	// Output:
	// version=1.2.3, commit=myCommit, built at 2006-01-02_1504-05
}

func Example_doit_b() {
	version = "1.2.3"
	commit = "myCommit"
	date = "2006-01-02_1504-05"
	os.Args = []string{"trice", "wrong"}
	act := tst.CaptureStdOut(doit)
	fmt.Print(act)
	// Output:
	// unknown subcommand 'wrong'. try: 'trice help|h'
}

func Example_doit_c() {
	fn := getTemporaryFileName("til*.JSON")
	// til is the trace id list content for tests
	til := `{
		"1047663": {
			"Type": "TRICE16_2",
			"Strg": "MSG: triceFifoMaxDepth = %d, select = %d\\n"
		}
	}
`
	b := []byte(til)
	msg.FatalOnErr(os.WriteFile(fn, b, 0644))

	os.Args = []string{"trice", "log", "-p", "BUFFER", "-args", "2, 124, 227, 255, 0, 0, 4, 0", "-ts", "off", "-color", "off", "-idlist", fn}
	act := tst.CaptureStdOut(doit)
	fmt.Print(act)
	// Output:
	// BUFFER: MSG: triceFifoMaxDepth = 4, select = 0
}

func TestDoit(t *testing.T) {
	version = "1.2.3"
	commit = "myCommit"
	date = "2006-01-02_1504-05"
	os.Args = []string{"trice", "ver"}
	act := tst.CaptureStdOut(doit)
	exp := "version=1.2.3, commit=myCommit, built at 2006-01-02_1504-05\n"
	assert.Equal(t, exp, act)
}
