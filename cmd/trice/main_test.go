// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package main

import (
	"fmt"
	"os"
	"os/exec"
	"testing"

	"github.com/rokath/trice/pkg/lib"
)

func TestMain(m *testing.M) {
	// do stuff before tests
	exitVal := m.Run()
	// do stuff after tests
	os.Exit(exitVal)
}

func TestBadArgs(t *testing.T) {
	cmd := exec.Command("trice", "some", "bad", "args")
	out, err := cmd.CombinedOutput()
	act := string(out) // because out is []byte
	if nil != err {
		fmt.Println(act) // so we can see the full output
		t.Errorf("%v", err)
	}
	exp := "try: 'trice help|h'\n"
	lib.Equals(t, exp, act)
}

func TestNoArgs(t *testing.T) {
	cmd := exec.Command("trice", "")
	out, err := cmd.CombinedOutput()
	if nil != err {
		t.Fail()
	}
	act := string(out) // because out is []byte
	exp := "try: 'trice help|h'\n"
	lib.Equals(t, exp, act)
}

func TestVersionArg(t *testing.T) {
	cmd := exec.Command("trice", "v")
	out, err := cmd.CombinedOutput()
	if nil != err {
		t.Fail()
	}
	act := string(out) // because out is []byte
	exp := "No logfile writing...\nversion=devel, built unknown\nNo logfile writing...done\n"
	lib.Equals(t, exp, act)
}
