// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package main

import (
	"fmt"
	"os"
	"os/exec"
	"testing"
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
	notEqualFail(t, exp, act)
}

func TestNoArgs(t *testing.T) {
	cmd := exec.Command("trice", "")
	out, err := cmd.CombinedOutput()
	if nil != err {
		t.Fail()
	}
	act := string(out) // because out is []byte
	exp := "try: 'trice help|h'\n"
	notEqualFail(t, exp, act)
}

/*
func TestVersionArg(t *testing.T) {
	cmd := exec.Command("trice", "v")
	out, err := cmd.CombinedOutput()
	if nil != err {
		t.Fail()
	}
	act := string(out) // because out is []byte
	exp := "version=devel, built unknown\n"
	notEqualFail(t, exp, act)
}
*/
/*
func TestSIM0(t *testing.T) {
	cmd := exec.Command("trice", "log", "-idlist", "c:/repos/trice/test/til.json", "-source", "SIM", "-lg", "off", "-ts", "off", "-color", "off")
	out, err := cmd.CombinedOutput()
	if nil != err {
		t.Fail()
	}
	act := string(out)
	exp := `SIM: garbage
SIM: ISR:interrupt   message, SysTick is      0
SIM: ISR:interrupt   message, SysTick is    257
SIM: garbage
SIM: ISR:interrupt   message, SysTick is      0
SIM: ISR:interrupt   message, SysTick is    257
`
	notEqualFail(t, exp, act)
}
*/
