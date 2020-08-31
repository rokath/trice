// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package args

import (
	"fmt"
	"io/ioutil"
	"os"
	"os/exec"
	"path/filepath"
	"runtime"
	"testing"
	"time"
)

var ()

func TestScVersion(t *testing.T) {

	fn := func() {
		Handler([]string{"trice", "h"})
	}
	act := captureStdout(fn)

	exp := `syntax: 'trice subcommand' [params]
	subcommand 'help', 'h' for command line usage
	  -lg string
			short for -logfile (default "off")
	  -logfile string
			append all output to logfile, options: 'none|filename|auto', 'auto' for "2006-01-02_1504-05_trice.log" with actual time (default "off")
	  -v	verbose, more informal output if used
	subcommand 'u', 'upd', 'update' for updating ID list and source files
	  -dry-run
			no changes are applied
	  -i string
			short for '-idlist' (default "til.json")
	  -idlist string
			trice ID list path, 'none' possible (default "til.json")
	  -src value
			source dir or file, multi use possible, default './'
	  -v	verbose, more informal output if used
	subcommand 'l', 'log' for displaying trice logs coming from source
	  -a	short for '-autostart'
	  -autostart
			autostart displayserver @ ipa:ipp (works not good with windows, because of cmd and powershell color issues and missing cli params in wt and gitbash)
	  -baud int
			COM baudrate, valid only for '-source COMn' (default 115200)
	  -color string
			color set, 'off' disables color handling ("w:x"->"w:x"), 'none' disables channels color ("w:x"->"x"), options: 'off|none' (default "default")
	  -displayserver
			send trice lines to displayserver @ ipa:ipp
	  -ds
			short for '-displayserver'
	  -e string
			short for -encoding (default "bare")
	  -encoding string
			trice transmit data format type, options: 'ascii|wrap' (default "bare")
	  -i string
			short for '-idlist' (default "til.json")
	  -idlist string
			trice ID list path, 'none' possible (default "til.json")
	  -ipa string
			ip address like '127.0.0.1' (default "localhost")
	  -ipp string
			16 bit port number (default "61497")
	  -jlink string
			passed parameter string, valid only for '-source JLRTT', see JLinkRTTLogger in SEGGER UM08001_JLink.pdf (default "-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0")
	  -key
			show encryption key
	  -lg string
			short for -logfile (default "off")
	  -logfile string
			append all output to logfile, options: 'none|filename|auto', 'auto' for "2006-01-02_1504-05_trice.log" with actual time (default "off")
	  -password string
			decrypt passphrase (default "none")
	  -prefix string
			line prefix, options: any string or 'off|none' or 'source:' followed by 0-12 spaces, 'source:' will be replaced by source value e.g., 'COM17:' (default "source: ")
	  -pw string
			short for -password (default "none")
	  -s string
			short for -source (default "JLINK")
	  -source string
			receiver device, options: 'COMn|JLINK|STLINK|filename|SIM|RND|HTTP' (default "JLINK")
	  -suffix string
			append suffix to all lines, options: any string
	  -ts string
			PC timestamp for logs and logfile name, options: 'off|none|UTCmicro|zero' (default "LOCmicro")
	  -v	verbose, more informal output if used
	subcommand 'zeroSourceTreeIds' for setting all TRICE IDs to 0 in source tree, avoid using this subcommand normally
	  -dry-run
			no changes are applied
	  -src string
			zero all Id(n) inside source tree dir, required
	subcommand 'v', 'ver', 'version' for displaying version information
	  -lg string
			short for -logfile (default "off")
	  -logfile string
			append all output to logfile, options: 'none|filename|auto', 'auto' for "2006-01-02_1504-05_trice.log" with actual time (default "off")
	  -v	verbose, more informal output if used
	subcommand 'ds', 'displayServer' starts a display server, use in a separate console, on Windows use wt or a linux shell like git-bash to avoid color issues, several instances of 'trice l -ds' will send output there
	  -color string
			color set, options: 'off|none' (default "default")
	  -ipa string
			ip address like '127.0.0.1' (default "localhost")
	  -ipp string
			16 bit port number (default "61497")
	  -lg string
			short for -logfile (default "off")
	  -logfile string
			append all output to logfile, options: 'none|filename|auto', 'auto' for "2006-01-02_1504-05_trice.log" with actual time (default "off")
	subcommand 'sd', 'shutdownServer' ends display server at IPA:IPP, works also on a remote mashine
	  -ipa string
			ip address like '127.0.0.1' (default "localhost")
	  -ipp string
			16 bit port number (default "61497")
	examples:
		'trice update -src ../A -src ../../B' parses ../A and ../../B with all subdirectories for TRICE IDs to update and adjusts til.json
		'trice l -s COM15 -baud 38400 -d wrap display wrap data format trice log messages from COM15
		'trice l display bare data format trice log messages from default source
		'trice zeroSourceTreeIds -dir ../A' sets all TRICE IDs to 0 in ./A
		'trice v -v' shows verbose version information
		`
	assertEqualLines(t, exp, act)
}

func TestVersion(t *testing.T) {
	fi, err := os.Stat(os.Args[0])
	assertNil(t, err)
	buildTime := fi.ModTime().String()
	exp := "version=devel, built " + buildTime + "\n"

	fn := func() {
		Handler([]string{"trice", "v"})
	}
	act := captureStdout(fn)
	assertEqual(t, exp, act)

	fn = func() {
		Handler([]string{"trice", "ver"})
	}
	act = captureStdout(fn)
	assertEqual(t, exp, act)

	fn = func() {
		Handler([]string{"trice", "version"})
	}
	act = captureStdout(fn)
	assertEqual(t, exp, act)
}

func Example_handlerNone() {
	fn := func() {
		Handler([]string{"trice", ""})
	}
	act := captureStdout(fn)
	fmt.Print(act)
	// Output:
	// try: 'trice help|h'
}

func Example_wrongSubcommand() {
	fn := func() {
		Handler([]string{"trice", "xyz"})
	}
	act := captureStdout(fn)
	fmt.Print(act)
	// Output:
	// try: 'trice help|h'
}

func Example_vwrongSubcommand() {
	fn := func() {
		Handler([]string{"trice", "xyz"})
	}
	act := captureStdout(fn)
	fmt.Print(act)
	// Output:
	// try: 'trice help|h'
}

// tempFileName returns a valid temporary filename or an empty string
func tempFileName(pattern string) string {
	// get a temporary file name
	fh, err := ioutil.TempFile(os.TempDir(), pattern) // opens for read and write
	if nil != err {
		return ""
	}
	name := fh.Name()
	fh.Close()
	return name
}

func frameForOsExitTests(parameters []string, exp string, t *testing.T) {
	pc := make([]uintptr, 10) // at least 1 entry needed
	runtime.Callers(2, pc)    // for debugging first parameter must be 2
	f := runtime.FuncForPC(pc[0])
	fullName := f.Name() // full name like "github/rokath/trice/cmd/trice.TestRTTF"
	fmt.Println("fullName:", fullName)
	baseName := filepath.Base(fullName)
	fmt.Println("baseName:", baseName)
	fnName := filepath.Ext(baseName)[1:]
	fmt.Println("fnName:", fnName)
	logFile := baseName + ".log"
	fmt.Println("logfile:", logFile)
	par := append(parameters, "-idlist", "c:/repos/trice/test/til.json", "-color", "off", "-ts", "none", "-lg", logFile)
	if os.Getenv("BE_EOF_"+fnName) == "1" { // here inside debug test does not stop
		//os.Remove(logFile) // secure logFile not exists already
		Handler(par)
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
	time.Sleep(3 * time.Second)
	b, _ := ioutil.ReadFile(logFile) // just pass the file name
	os.Remove(logFile)               // must be before assertEqual
	act := string(b)
	if len(act) > len(exp) { // because of os.Exit act sometimes has not always the same length
		act = string(b[:len(exp)]) // shorten act to length of exp, exp has granted minumum length
	}
	assertEqual(t, exp, act)
}

/*
func TestRTTFX(t *testing.T) {
	cmd := []string{"trice", "log",
		"-s", "C:\\repos\\trice\\internal\\receiver\\rttfile\\testdata\\wrapTriceMsgs.bin",
		"-prefix", "none",
	}
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
	frameForOsExitTests(cmd, exp, t)
}
*/
/*
func TestRNDChaosX(t *testing.T) {
	cmd := []string{"trice", "log",
		"-source", "RND",
		"-rndLimit", "10", // 7 values are kept inside in hope next value makes a valid header
		"-rndMode", "ChaosMode",
	}
	exp := `RND: wrn:discarding byte 0x9f (dez 159, char ' ')
RND: wrn:discarding byte 0x90 (dez 144, char ' ')
RND: wrn:discarding byte 0xa3 (dez 163, char ' ')
`
	frameForOsExitTests(cmd, exp, t)
}
*/
/*
func TestHandlerRNDbare(t *testing.T) { // cmdLineNotOk
	cmd := []string{
		"trice", "log",
		"-source", "RND",
		"-rndLimit", "190",
		"-rndMode", "BareModeNoSync",
	}
	exp := `RND: wrn:discarding byte 0x9f (dez 159, char ' ')
RND: wrn:discarding byte 0xc6 (dez 198, char ' ')
RND: wrn:discarding byte 0x29 (dez 41, char ')')
RND: wrn:discarding byte 0x83 (dez 131, char ' ')
RND: wrn:discarding byte 0xc8 (dez 200, char ' ')
`
	frameForOsExitTests(cmd, exp, t)
}
*/
/*
func TestHandlerRNDwrap(t *testing.T) { // cmdLineNotOk
	cmd := []string{
		"trice", "log",
		"-source", "RND",
		"-rndLimit", "19",
	}
	exp := `trice.Log error unknown ID [235 96 96 69 80 196 157 167] []
trice.Log error unknown ID [235 96 96 216 119 228 147 51] []
`
	frameForOsExitTests(cmd, exp, t)
}
*/
/*
func TestHandlerSIM0(t *testing.T) {
	cmd := []string{"trice", "log", "-source", "SIM"}

	exp := `SIM: garbage
SIM: ISR:interrupt   message, SysTick is      0
SIM: ISR:interrupt   message, SysTick is    257
SIM: garbage
SIM: ISR:interrupt   message, SysTick is      0
SIM: ISR:interrupt   message, SysTick is    257
`
	frameForOsExitTests(cmd, exp, t)
}
*/
/*
func TestHandlerCOMx(t *testing.T) {
	cmd := []string{"trice", "log", "-source", "COMx"}

	exp := `Serial port not found try 'trice s' to check for serial ports
`
	frameForOsExitTests(cmd, exp, t)
}
*/
/* TestScDisplayServer checks if "-ds" switch works (start command)
func TestScDisplayServer(t *testing.T) {
	afn := "testdata/actDisplayServer.log"
	efn := "testdata/expDisplayServer.log"
	os.Remove(afn)
	cage.Name = afn
	disp.IPPort = "61501"
	Source = ""
	assertNil(t, connect("C:\\Users\\ms\\go\\bin\\trice.exe"))
	assertNil(t, disp.PtrRPC.Call("Server.Out", []string{"msg:test ", "dbg:line 1."}, nil))
	assertNil(t, disp.PtrRPC.Call("Server.Out", []string{"att:test ", "sig:line 2."}, nil))

	// stop display server
	assertNil(t, disp.ScShutdownRemoteDisplayServer(0))
	assertEqualTextFiles(t, afn, efn)
	time.Sleep(200 * time.Millisecond) // may be a wait for displaySever is down now is needed here
	assertNil(t, os.Remove(afn))
}

// TestServerStartStop checks if display server can be started and stopped remotely
func TestServerStartStop(t *testing.T) {
	afn := "testdata/actServerStartStopWg.log"
	efn := "testdata/expServerStartStopWg.log"
	os.Remove(afn)
	disp.IPPort = "61502"
	var wg sync.WaitGroup
	wg.Add(1)
	go func() {
		defer wg.Done()
		time.Sleep(200 * time.Millisecond)

		// stop display server
		assertNil(t, disp.ScShutdownRemoteDisplayServer(0))
	}()

	// start display server
	args := []string{"trice", "ds", "-lg", afn}
	Handler(args)
	wg.Wait()
	assertEqualTextFiles(t, afn, efn)
	assertNil(t, os.Remove(afn))
}
*/
/*
// TODO: adapt to parameter set
func TestScHelp(t *testing.T) {
	afn := "testdata/actHelp.log"
	efn := "testdata/expHelp.log"
	os.Remove(afn)
	args := []string{"trice", "help", "-lg", afn}
	assertNil(t, Handler(args))
	assertEqualTextFiles(t, afn, efn)
	assertNil(t, os.Remove(afn))
}
*/
/* This is just tryout code but we do not reach the test aim with it:
var flag2 bool

// TestHRNDchaos expects installed trice compiled from actual sources.
//
// This kind of test does not work just with Handler function, because of os.Exit(0) on io.EOF in -source RND.
// Endless waiting there does also not work, so this approach is just a quick solution.
func TestRNDchaos2(t *testing.T) {
	var act string
	if false == flag2 { // 1st entry
		flag2 = true
		onExit := func(x int) {
			e := 0 // expected value for x from call global.osExit(x)
			assertEqual(t, e, x)
			TestRNDchaos2(t) // trigger 2nd entry
		}

		//global.OsExit = onExit // change os.Exit() behaviour
		cage.Name = "c:/repos/trice/flag2.txt"
		cage.Enable()
		defer cage.Disable()
		func() {
			Handler([]string{"trice", "log",
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
		asertEqual(t, exp, act)
		// test goes well until here
		// If os.Exit follows here the test ends in every case without error message even it failed.
		// If os.Exit follows not the execution context goes back to the ReadAtLeast and the test does not end because of endless cycle.
		os.Exit(0)
	}
}
*/
