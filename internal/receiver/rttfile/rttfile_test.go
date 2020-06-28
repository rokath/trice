// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package rttfile_test is a blackbox test.
package rttfile_test

import (
	"testing"

	"github.com/rokath/trice/pkg/lib"
)

func TestDummy(t *testing.T) {
	lib.Ok(t, nil)
}

func TestWrap(t *testing.T) {
	// see TestRTTF in main_test.go
	lib.Ok(t, nil)
}

/* not 100% secure concerning last line
// TestRTTF expects installed trice compiled from actual sources.
//
// This kind of test does not work just with HandleArgs function, because of global.OsExit(0) on io.EOF in -source RND.
// Endless waiting there does also not work, so this apprpoach is just a quick solution.
func TestRTTF(t *testing.T) {
	cmd := exec.Command("trice", "log",
		"-idlist", "c:/repos/trice/test/til.json",
		"-s", "C:\\repos\\trice\\internal\\receiver\\rttfile\\testdata\\wrapTriceMsgs.bin",
		"-lg", "off",
		"-ts", "off",
		"-prefix", "none",
		"-color", "off")
	out, err := cmd.CombinedOutput()
	if nil != err {
		t.Fail()
	}
	act := string(out)
	//act = strings.Trim(act, `\r`)
	act = strings.Replace(act, `\r\n`, `\n`, -1)
	//act = strings.Replace(act, `\t`, "", -1)
	exp := `ISR:alive time 5121700 milliseconds
ISR:alive time 5121800 milliseconds
ISR:alive time 5121900 milliseconds
ISR:alive time 5122000 milliseconds
ERR:error       message, SysTick is -19095
WRN:warning     message, SysTick is -19483
ATT:attension   message, SysTick is -19871
DIA:diagnostics message, SysTick is -20259
TIM:timing      message, SysTick is -20647
DBG:debug       message, SysTick is -21035
SIG:signal      message, SysTick is -21423
RD:read         message, SysTick is -21811
WR:write        message, SysTick is -22199
ISR:interrupt   message, SysTick is -22587
TST:test        message, SysTick is -22975
MSG:normal      message, SysTick is -23363
INFO:informal   message, SysTick is -23751
time:ms = 5122000
ISR:alive time 5122100 milliseconds
ISR:alive time 5122200 milliseconds
ISR:alive time 5122300 milliseconds
ISR:alive time 5122400 milliseconds
ISR:alive time 5122500 milliseconds
ISR:alive time 5122600 milliseconds
ISR:alive time 5122700 milliseconds
ISR:alive time 5122800 milliseconds
ISR:alive time 5122900 milliseconds
ISR:alive time 5123000 milliseconds
tst:TRICE8  %03x ->  001  07f  -80  -01
tst:TRICE8   %4d ->    1  127 -128   -1
tst:TRICE8   %4o ->    1  177 -200   -1
tst:TRICE16  %05x ->   00001   07fff   -8000   -0001
tst:TRICE16   %6d ->       1   32767  -32768      -1
tst:TRICE16   %7o ->       1   77777 -100000      -1
tst:TRICE32_4 %09x ->      000000001      07fffffff       -80000000     -00000001
tst:TRICE32_4 %10d ->              1     2147483647     -2147483648            -1
att:64bit 0b1000100100010001100110100010001010101011001100111011110001000
time:ms = 5123000
ISR:alive time 5123100 milliseconds
ISR:alive time 5123200 milliseconds
ISR:alive time 5123300 milliseconds
ISR:alive time 5123400 milliseconds
ISR:alive time 5123500 milliseconds
ISR:alive time 5123600 milliseconds
ISR:alive time 5123700 milliseconds
ISR:alive time 5123800 milliseconds
ISR:alive time 5123900 milliseconds
ISR:alive time 5124000 milliseconds
tst:TRICE8 -1
tst:TRICE8 -1 -2
tst:TRICE8 -1 -2 -3
tst:TRICE8 -1 -2 -3 -4
tst:TRICE8 -1 -2 -3 -4 -5
tst:TRICE8 -1 -2 -3 -4 -5 -6
tst:TRICE8 -1 -2 -3 -4 -5 -6 -7
tst:TRICE8 -1 -2 -3 -4 -5 -6 -7 -8
time:ms = 5124000
ISR:alive time 5124100 milliseconds
ISR:alive time 5130100 milliseconds
ISR:alive time 5130200 milliseconds
`
	lib.Equals(t, exp, act)
}
*/
