// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net

package stimargs

import (
	"bytes"
	"os"
	"testing"

	"github.com/rokath/trice/pkg/msg"
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

func testVersion(t *testing.T, v []string) {
	fi, err := os.Stat(os.Args[0])
	assert.Nil(t, err)
	buildTime := fi.ModTime().String()
	exp := v[0] + "version=devel, built " + buildTime + "\n" + v[1]

	var buf0 bytes.Buffer
	msg.OnErr(Handler(&buf0, []string{"stim", "ver"}))
	act0 := buf0.String()
	assert.Equal(t, exp, act0)

	var buf1 bytes.Buffer
	msg.OnErr(Handler(&buf1, []string{"stim", "version"}))
	act1 := buf1.String()
	assert.Equal(t, exp, act1)
}
