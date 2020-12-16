// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

//package main

// func TestVersionArg(t *testing.T) {
// 	cmd := exec.Command("trice", "v")
// 	out, err := cmd.CombinedOutput()
// 	if nil != err {
// 		t.Fail()
// 	}
// 	act := string(out) // because out is []byte
// 	exp := "version=devel, built unknown\n"
// 	assertEqual(t, exp, act)
// }
//
//
// func TestSIM0(t *testing.T) {
// 	cmd := exec.Command("trice", "log", "-idlist", "c:/repos/trice/test/til.json", "-source", "SIM", "-lg", "off", "-ts", "off", "-color", "off")
// 	out, err := cmd.CombinedOutput()
// 	if nil != err {
// 		t.Fail()
// 	}
// 	act := string(out)
// 	exp := `SIM: garbage
// SIM: ISR:interrupt   message, SysTick is      0
// SIM: ISR:interrupt   message, SysTick is    257
// SIM: garbage
// SIM: ISR:interrupt   message, SysTick is      0
// SIM: ISR:interrupt   message, SysTick is    257
// `
// 	assertEqual(t, exp, act)
// }
