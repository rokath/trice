// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package main

import (
	"fmt"
	"io/ioutil"
	"os"
	"os/exec"
	"path/filepath"
	"runtime"
	"testing"

	capturer "github.com/kami-zh/go-capturer"
	"github.com/rokath/trice/pkg/lib"
)

func TestScVersion(t *testing.T) {
	afn := "testdata/actVersion.log"
	efn := "testdata/expVersion.log"
	os.Remove(afn)
	args := []string{"trice", "version", "-lg", afn}
	lib.Ok(t, HandleArgs(args))
	lib.EqualFiles2(t, afn, efn)
	lib.Ok(t, os.Remove(afn))
}

func Example_v() {
	fn := func() {
		HandleArgs([]string{"trice", "v"})
	}
	act := capturer.CaptureOutput(fn)
	fmt.Print(act)
	// Output:
	// version=devel, built testTime
}

func Example_ver() {
	fn := func() {
		HandleArgs([]string{"trice", "ver"})
	}
	act := capturer.CaptureOutput(fn)
	fmt.Print(act)
	// Output:
	// version=devel, built testTime
}

func Example_version() {
	fn := func() {
		HandleArgs([]string{"trice", "version"})
	}
	act := capturer.CaptureOutput(fn)
	fmt.Print(act)
	// Output:
	// version=devel, built testTime
}

func Example_handleArgsNone() {
	fn := func() {
		HandleArgs([]string{"trice", ""})
	}
	act := capturer.CaptureOutput(fn)
	fmt.Print(act)
	// Output:
	// try: 'trice help|h'
}

func Example_wrongSubcommand() {
	fn := func() {
		HandleArgs([]string{"trice", "xyz"})
	}
	act := capturer.CaptureOutput(fn)
	fmt.Print(act)
	// Output:
	// try: 'trice help|h'
}

func Example_vwrongSubcommand() {
	fn := func() {
		HandleArgs([]string{"trice", "xyz"})
	}
	act := capturer.CaptureOutput(fn)
	fmt.Print(act)
	// Output:
	// try: 'trice help|h'
}

func Example_handleArgsLogCOM0() {
	fn := func() {
		HandleArgs([]string{"trice", "log", "-idlist", "none", "-s", "COM0", "-lg", "none"})
	}
	act := capturer.CaptureOutput(fn)
	fmt.Print(act)
	// Output:
	// id list file none with 0 items on device COM
	// Error: Could not open serial port: Serial port not found
}

func Example_handleArgsLogListNotFound() { // cmdLineNotOk
	err := HandleArgs([]string{"trice", "log", "-idlist", "xxx.json", "-s", "COM0", "-lg", "off"})
	fmt.Print(err)
	// Output:
	// ID list xxx.json not found
	// <nil>
}

func Example_handleArgsLogListNotFound2() { // cmdLineNotOk
	fn := func() {
		HandleArgs([]string{"trice", "log", "-idlist", "xxx.json", "-s", "COM1", "-lg", "off"})
	}
	act := capturer.CaptureOutput(fn)
	fmt.Print(act)
	// Output:
	// ID list xxx.json not found
}

func Example_handleArgsRNDwrap() { // 19/8 = 2 lines
	fn := func() {
		HandleArgs([]string{"trice", "log", "-idlist", "c:/repos/trice/test/til.json", "-source", "RND", "-rndLimit", "19", "-lg", "off"})
	}
	act := capturer.CaptureOutput(fn)
	fmt.Print(act)
	// Output:
	// id list file C:\repos\trice\test\til.json with 437 items on device RND
	// C:\repos\trice\test\til.json watched now for changes
	// 2020/06/24 20:22:43 trice.Log error unknown ID [235 96 96 69 80 196 157 167] []
	// xyz
	// 2020/06/24 20:22:43 trice.Log error unknown ID [235 96 96 216 119 228 147 51] []
}

func Example_handleArgsRNDbare() { // cmdLineNotOk
	fn := func() {
		HandleArgs([]string{"trice", "log", "-idlist", "c:/repos/trice/test/til.json", "-source", "RND", "-rndLimit", "19", "-rndMode", "BareModeNoSync", "-lg", "off"})
	}
	act := capturer.CaptureOutput(fn)
	fmt.Print(act)
	// Output:
	// id list file C:\repos\trice\test\til.json with 437 items on device RND
	// C:\repos\trice\test\til.json watched now for changes
	// RND: Jun 24 20:31:21.533294  trice:discarding byte 0x9f (dez 159, char '
	// RND: Jun 24 20:31:21.540294  trice:discarding byte 0xc6 (dez 198, char 'Æ')
	// RND: Jun 24 20:31:21.546294  trice:discarding byte 0x29 (dez 41, char ')')
	// RND: Jun 24 20:31:21.565294  trice:discarding byte 0x83 (dez 131, char '')
	// RND: Jun 24 20:31:21.588326  trice:discarding byte 0xc8 (dez 200, char 'È')
}

func Example_handleArgsRNDchaos() { // cmdLineNotOk
	fn := func() {
		HandleArgs([]string{"trice", "log", "-idlist", "c:/repos/trice/test/til.json", "-source", "RND", "-rndLimit", "10", "-rndMode", "ChaosMode", "-lg", "off"})
	}
	act := capturer.CaptureOutput(fn)
	fmt.Print(act)
	// Output:
	// id list file C:\repos\trice\test\til.json with 437 items on device RND
	// C:\repos\trice\test\til.json watched now for changes
	// RND: Jun 24 20:46:20.970053  trice:discarding byte 0x9f (dez 159, char '
	// RND: Jun 24 20:46:20.972054  trice:discarding byte 0x90 (dez 144, char '
	// RND: Jun 24 20:46:20.974054  trice:discarding byte 0xa3 (dez 163, char '£')
}

func Example_handleArgsSIM0() { // cmdLineNotOk
	fn := func() {
		HandleArgs([]string{"trice", "log", "-idlist", "c:/repos/trice/test/til.json", "-source", "SIM", "-lg", "off"})
	}
	act := capturer.CaptureOutput(fn)
	fmt.Print(act)
	// Output:
	// id list file C:\repos\trice\test\til.json with 437 items on device SIM
	// C:\repos\trice\test\til.json watched now for changes
	// SIM: Jun 24 21:01:17.103782  garbage
	// SIM: Jun 24 21:01:17.115772  ISR:interrupt   message, SysTick is      0
	// SIM: Jun 24 21:01:17.117776  ISR:interrupt   message, SysTick is    257
	// SIM: Jun 24 21:01:17.118787  garbage
	// SIM: Jun 24 21:01:17.135788  ISR:interrupt   message, SysTick is      0
	// SIM: Jun 24 21:01:17.137786  ISR:interrupt   message, SysTick is    257
}

/* works not when parallel testing is inabled
// TestScDisplayServer checks if "-ds" switch works (start command)
func TestScDisplayServer(t *testing.T) {
	afn := "testdata/actDisplayServer.log"
	efn := "testdata/expDisplayServer.log"
	os.Remove(afn)
	cage.Name = afn

	lib.Ok(t, trice.Connect("C:\\Users\\ms\\go\\bin\\trice.exe"))
	lib.Ok(t, disp.PtrRPC.Call("Server.Out", []string{"msg:test ", "dbg:line 1."}, nil))
	lib.Ok(t, disp.PtrRPC.Call("Server.Out", []string{"att:test ", "sig:line 2."}, nil))

	// stop display server
	lib.Ok(t, disp.ScShutdownRemoteDisplayServer(0))
	lib.EqualFiles2(t, afn, efn)
	time.Sleep(200 * time.Millisecond) // may be a wait for displaySever is down now is needed here
	lib.Ok(t, os.Remove(afn))
}

// TestServerStartStop checks if display server can be started and stopped remotely
func TestServerStartStop(t *testing.T) {
	afn := "testdata/actServerStartStopWg.log"
	efn := "testdata/expServerStartStopWg.log"
	os.Remove(afn)

	var wg sync.WaitGroup
	wg.Add(1)
	go func() {
		defer wg.Done()
		time.Sleep(200 * time.Millisecond)

		// stop display server
		lib.Ok(t, disp.ScShutdownRemoteDisplayServer(0))
	}()

	// start display server
	args := []string{"trice", "ds", "-lg", afn}
	HandleArgs(args)
	wg.Wait()
	lib.EqualFiles2(t, afn, efn)
	lib.Ok(t, os.Remove(afn))
}
*/

/* TODO: adapt to parameter set
func TestScHelp(t *testing.T) {
	afn := "testdata/actHelp.log"
	efn := "testdata/expHelp.log"
	os.Remove(afn)
	args := []string{"trice", "help", "-lg", afn}
	lib.Ok(t, HandleArgs(args))
	lib.EqualFiles2(t, afn, efn)
	lib.Ok(t, os.Remove(afn))
}
*/

/* This is just tryout code but we do not reach the test aim with it:
var flag2 bool

// TestHRNDchaos expects installed trice compiled from actual sources.
//
// This kind of test does not work just with HandleArgs function, because of os.Exit(0) on io.EOF in -source RND.
// Endless waiting there does also not work, so this apprpoach is just a quick solution.
func TestRNDchaos2(t *testing.T) {
	var act string
	if false == flag2 { // 1st entry
		flag2 = true
		onExit := func(x int) {
			e := 0 // expected value for x from call global.osExit(x)
			lib.Equals(t, e, x)
			TestRNDchaos2(t) // trigger 2nd entry
		}

		global.OsExit = onExit // change os.Exit() behaviour
		cage.Name = "c:/repos/trice/flag2.txt"
		cage.Enable()
		defer cage.Disable()
		func() {
			HandleArgs([]string{"trice", "log",
				"-idlist", "c:/repos/trice/til.json",
				"-source", "RND",
				"-rndLimit", "10",
				"-rndMode", "ChaosMode",
				"-lg", "off",
				"-ts", "off",
				"-color", "off"})
		}()
		// no return here

		// This does not work either, because no return here
		//act = capturer.CaptureOutput(fn)
		//fmt.Print(act)

	} else { // 2nd entry
		b, _ := ioutil.ReadFile("c:/repos/trice/flag2.txt") // just pass the file name
		act = string(b)
		exp := `RND: trice:discarding byte 0x9f (dez 159, char ' ')
RND: trice:discarding byte 0x90 (dez 144, char ' ')
RND: trice:discarding byte 0xa3 (dez 163, char ' ')
`
		lib.Equals(t, exp, act)
		// test goes well until here
		// If os.Exit follows here the test ends in every case without error message even it failed.
		// If os.Exit follows not the execution context goes back to the ReadAtLeast and the test does not end because of endless cycle.
		os.Exit(0)
	}
}
*/

// TestRNDChaos checks if random data are working
func TestRNDChaos(t *testing.T) {
	// https://stackoverflow.com/questions/26225513/how-to-test-os-exit-scenarios-in-go
	logFile := "TestRNDChaos.txt"
	if os.Getenv("BE_EOF_CRASHER") == "1" { // here inside debug test does not stop
		os.Remove(logFile) // secure logFile not exists already
		HandleArgs([]string{"trice", "log",
			"-idlist", "c:/repos/trice/til.json",
			"-source", "RND",
			"-rndLimit", "10", // 7 values are kept inside in hope next value makes a valid header
			"-rndMode", "ChaosMode",
			"-lg", logFile,
			"-ts", "off",
			"-color", "off"})
		return
	}
	cmd := exec.Command(os.Args[0], "-test.run=TestRNDChaos") // test fn name!
	cmd.Env = append(os.Environ(), "BE_EOF_CRASHER=1")
	err := cmd.Run()
	e, ok := err.(*exec.ExitError)
	if ok && !e.Success() {
		t.Fail()
		return
	}
	b, _ := ioutil.ReadFile(logFile) // just pass the file name
	os.Remove(logFile)               // must be before lib.Equals
	act := string(b)
	exp := `RND: trice:discarding byte 0x9f (dez 159, char ' ')
RND: trice:discarding byte 0x90 (dez 144, char ' ')
RND: trice:discarding byte 0xa3 (dez 163, char ' ')
`
	lib.Equals(t, exp, act)
}

// TestRTTF
func TestRTTF(t *testing.T) {
	pc := make([]uintptr, 10) // at least 1 entry needed
	runtime.Callers(0, pc)    // for debugging first parameter must be 1
	f := runtime.FuncForPC(pc[0])
	fullName := f.Name() // full name like "github/rokath/trice/cmd/trice.TestRTTF"
	fmt.Println("fullName:", fullName)
	baseName := filepath.Base(fullName)
	fmt.Println("baseName:", baseName)
	fnName := filepath.Ext(baseName)[1:]
	fmt.Println("fnName:", fnName)
	logFile := baseName + ".txt"
	fmt.Println("logfile:", logFile)
	if os.Getenv("BE_EOF_"+fnName) == "1" { // here inside debug test does not stop
		os.Remove(logFile) // secure logFile not exists already
		HandleArgs([]string{"trice", "log",
			"-idlist", "c:/repos/trice/til.json",
			"-s", "C:\\repos\\trice\\internal\\receiver\\rttfile\\testdata\\wrapTriceMsgs.bin",
			"-lg", logFile,
			"-ts", "off",
			"-prefix", "none",
			"-color", "off"})
		return
	}
	cmd := exec.Command(os.Args[0], "-test.run="+fnName) // test fn name!
	cmd.Env = append(os.Environ(), "BE_EOF_"+fnName+"=1")
	err := cmd.Run()
	if nil != err {
		//t.Fail()
	}
	/*e, ok := err.(*exec.ExitError)
	ok = false
	if ok && !e.Success() {
		t.Fail()
		return
	}*/
	b, _ := ioutil.ReadFile(logFile) // just pass the file name
	os.Remove(logFile)               // must be before lib.Equals
	acts := string(b)
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
` // ISR:alive time 5130200 milliseconds
	lib.Equals(t, exp, acts)
}

func frameForOsExitTests(logFile string, parameters []string, exp string, t *testing.T) {
	pc := make([]uintptr, 10) // at least 1 entry needed
	runtime.Callers(2, pc)    // for debugging first parameter must be 2
	f := runtime.FuncForPC(pc[0])
	fullName := f.Name() // full name like "github/rokath/trice/cmd/trice.TestRTTF"
	fmt.Println("fullName:", fullName)
	baseName := filepath.Base(fullName)
	fmt.Println("baseName:", baseName)
	fnName := filepath.Ext(baseName)[1:]
	fmt.Println("fnName:", fnName)
	//logFile := baseName + ".txt"
	fmt.Println("logfile:", logFile)
	if os.Getenv("BE_EOF_"+fnName) == "1" { // here inside debug test does not stop
		os.Remove(logFile) // secure logFile not exists already
		HandleArgs(parameters)
		return
	}
	fmt.Println(os.Args[0])
	cmd := exec.Command(os.Args[0], "-test.run="+fnName) // test fn name!
	cmd.Env = append(os.Environ(), "BE_EOF_"+fnName+"=1")
	err := cmd.Run()
	if nil != err {
		//t.Fail()
	}
	/*e, ok := err.(*exec.ExitError)
	ok = false
	if ok && !e.Success() {
		t.Fail()
		return
	}*/
	b, _ := ioutil.ReadFile(logFile) // just pass the file name
	os.Remove(logFile)               // must be before lib.Equals
	act := string(b)
	if len(act) > len(exp) { // because of os.Exit act sometimes has not always the same length
		act = string(b[:len(exp)]) // shorten act to length of exp, exp has granted minumum length
	}
	lib.Equals(t, exp, act)
}

func TestRTTFX(t *testing.T) {
	logFile := "TestRTTFX.txt" // todo: randomName

	cmd := []string{"trice", "log",
		"-idlist", "c:/repos/trice/til.json",
		"-s", "C:\\repos\\trice\\internal\\receiver\\rttfile\\testdata\\wrapTriceMsgs.bin",
		"-lg", logFile,
		"-ts", "off",
		"-prefix", "none",
		"-color", "off"}

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
`
	frameForOsExitTests(logFile, cmd, exp, t)
}

func TestRNDChaosX(t *testing.T) {
	logFile := "TestRNDChaosX.txt" // todo: randomName

	cmd := []string{"trice", "log",
		"-idlist", "c:/repos/trice/til.json",
		"-source", "RND",
		"-rndLimit", "10", // 7 values are kept inside in hope next value makes a valid header
		"-rndMode", "ChaosMode",
		"-lg", logFile,
		"-ts", "off",
		"-color", "off"}

	exp := `RND: trice:discarding byte 0x9f (dez 159, char ' ')
RND: trice:discarding byte 0x90 (dez 144, char ' ')
RND: trice:discarding byte 0xa3 (dez 163, char 'X')
`
	frameForOsExitTests(logFile, cmd, exp, t)
}
