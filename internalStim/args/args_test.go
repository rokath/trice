// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net

package args

import (
	"bytes"
	"fmt"
	"os"
	"sync"
	"testing"

	"github.com/rokath/trice/pkg/msg"
	"github.com/rokath/trice/pkg/tst"
	"github.com/tj/assert"
)

func TestMain(m *testing.M) {
	code := m.Run()
	os.Exit(code)
}

func TestHelp(t *testing.T) {
	args := []string{"stim", "help"}
	expect := `syntax: 'stim sub-command' [params]
      example 'stim h -help': Print help for help.
      `
	execHelper(t, args, expect)
}

func TestXyz(t *testing.T) {
	args := []string{"stim", "xyz"}
	expect := `unknown sub-command 'xyz'. try: 'stim help|h'`
	execHelper(t, args, expect)
}

func TestNoArgs(t *testing.T) {
	args := []string{"stim"}
	expect := `no args, try: 'stim help'`
	execHelper(t, args, expect)
}

func TestVersion(t *testing.T) {
	verbose = false
	v := []string{"", ""}
	testVersion(t, v)

	verbose = true
	v[0] = "https://github.com/rokath/trice/cmd/stim\n"
	v[1] = ""
	testVersion(t, v)
	verbose = false
}

var m *sync.RWMutex

func init() {
	m = new(sync.RWMutex)
}

func execHelper(t *testing.T, input []string, expect string) {
	m.Lock()
	defer m.Unlock()
	var out bytes.Buffer
	FlagsInit() // maybe needed for clearance of previous tests (global vars) // todo: is already in init() called
	err := Handler(&out, input)
	if err != nil {
		fmt.Fprint(&out, err)
	}
	act := out.String()
	tst.EqualLines(t, expect, act)
}

func testVersion(t *testing.T, v []string) {
	fi, err := os.Stat(os.Args[0])
	assert.Nil(t, err)
	buildTime := fi.ModTime().String()
	exp := v[0] + "version=devel, built " + buildTime + "\n" + v[1]

	var buf0 bytes.Buffer
	msg.OnErr(Handler(&buf0, []string{"stim", "ver"}))
	act0 := string(buf0.Bytes())
	assert.Equal(t, exp, act0)

	var buf1 bytes.Buffer
	msg.OnErr(Handler(&buf1, []string{"stim", "version"}))
	act1 := string(buf1.Bytes())
	assert.Equal(t, exp, act1)
}
