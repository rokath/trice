// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package main

import (
	"os"
	"sync"
	"testing"
	"time"

	"github.com/rokath/trice/pkg/disp"
	"github.com/rokath/trice/pkg/lgf"
	"github.com/rokath/trice/pkg/lib"
	"github.com/rokath/trice/pkg/trice"
)

func TestScVersion(t *testing.T) {
	afn := "testdata/actVersion.log"
	efn := "testdata/expVersion.log"
	os.Remove(afn)
	args := []string{"trice", "version", "-lf", afn}
	linkTime = "testTime"
	lib.Ok(t, HandleArgs(args))
	lib.EqualFiles2(t, afn, efn)
	lib.Ok(t, os.Remove(afn))
}

func TestScHelp(t *testing.T) {
	afn := "testdata/actHelp.log"
	efn := "testdata/expHelp.log"
	os.Remove(afn)
	args := []string{"trice", "help", "-lf", afn}
	lib.Ok(t, HandleArgs(args))
	lib.EqualFiles2(t, afn, efn)
	lib.Ok(t, os.Remove(afn))
}

func ExampleHandleArgs_none() {
	HandleArgs([]string{"trice", ""})
	// Output:
	// try: 'trice help|h'
}

func ExampleHandleArgs_wrongSubcommand() {
	HandleArgs([]string{"trice", "xyz"})
	// Output:
	// try: 'trice help|h'
}

func ExampleHandleArgs_logCOM0() {
	HandleArgs([]string{"trice", "log", "-list", "none", "-port", "COM0"})
	// Output:
	// id list file none with 0 items
	// Error: Could not open serial port: Serial port not found
	// Could not set up serial port COM0
	// try -port COMscan
}

// TestScDisplayServer checks if "-ds" switch works (start command)
func TestScDisplayServer(t *testing.T) {
	afn := "testdata/actDisplayServer.log"
	efn := "testdata/expDisplayServer.log"
	os.Remove(afn)
	lgf.Name = afn

	lib.Ok(t, trice.NewConnection("C:\\Users\\ms\\go\\bin\\trice.exe"))
	lib.Ok(t, disp.PtrRPC.Call("Server.Out", []string{"msg:test ", "dbg:line 1."}, nil))
	lib.Ok(t, disp.PtrRPC.Call("Server.Out", []string{"att:test ", "sig:line 2."}, nil))

	// stop display server
	lib.Ok(t, disp.ScShutdownRemoteDisplayServer(0))
	lib.EqualFiles2(t, afn, efn)

}

// TestServerStartStop checks if display server can be stopped remotely
func TestServerStartStop(t *testing.T) {
	afn := "testdata/actServerStartStopWg.log"
	efn := "testdata/expServerStartStopWg.log"
	os.Remove(afn)

	var wg sync.WaitGroup
	wg.Add(1)
	go func() {
		defer wg.Done()
		time.Sleep(100 * time.Millisecond)

		// stop display server
		lib.Ok(t, disp.ScShutdownRemoteDisplayServer(0))
		lib.EqualFiles2(t, afn, efn)
	}()

	// start display server
	args := []string{"trice", "ds", "-lf", afn}
	lib.Ok(t, HandleArgs(args))
	wg.Wait()
	lib.Ok(t, os.Remove(afn))
}

/* HOW TO PUT TEXT FILE IN READABLE STRING ?
func TestServerStartStop(t *testing.T) {
	e := 'displayServer @ localhost:61497
	[7;38;5;118mdialing [0mlocalhost:61497 ...
	[7;38;5;118m...remoteDisplay @ [0mlocalhost:61497 connected.
	[7;38;5;11;41msending Server.Shutdown...[0m


	[7;38;5;130mdisplayServer shutdown[0m


	[7;38;5;118mdialing [0mlocalhost:61497 ...
	[7;38;5;118m...remoteDisplay @ [0mlocalhost:61497 connected.
	[7;38;5;11;41msending Server.Shutdown...[0m


	[7;38;5;130mdisplayServer shutdown[0m'

	afn := "testdata/actServerStartStop.log"
	os.Remove(afn)
	//	var wg sync.WaitGroup
	//	wg.Add(1)
	go func() {
		//		defer wg.Done()
		time.Sleep(1000 * time.Millisecond)
		lib.Ok(t, disp.ScShutdownRemoteDisplayServer(0))
		lib.EqualFile(t, afn, e)
	}()
	args := []string{"trice", "ds", "-lf", afn}
	lib.Ok(t, HandleArgs(args))
	//wg.Wait()
	time.Sleep(2000 * time.Millisecond)
}
*/

// How to fix that?
func xxxExampleHandleArgs_logListNotFound() {
	HandleArgs([]string{"trice", "log", "-list", "xxx.json", "-port", "COMscan", "-lf", "off"})
	// Output:
	// ID list C:\GitRepos\trice\cmd\trice\testdata\xxx.json not found, exit
	// ERROR GetFileAttributes: The system cannot find the file specified.
}

// How to fix that?
func xxxExampleHandleArgs_logNoParam() {
	HandleArgs([]string{"trice", "log", "", ""})
	// Output:
	// -baud int
	// 	COM baudrate (optional, default is 38400 (default 38400)
	// -list string
	// 	ID list path (optional) (default "./idlist.json")
	// -port string
	// 	subcommand (required, try COMscan)
}

// How to fix that?
func xxxExampleHandleArgs_logWrongParam() {
	HandleArgs([]string{"trice", "log", "-x", "y"})
	// Output: flag provided but not defined: -x
	//Usage of log:
	//  -baud int
	//    	COM baudrate (default 115200)
	//  -color string
	//    	color set, options: off|alternate (default "default")
	//  -key string
	//    	decrypt passphrase (default "none")
	//  -lf string
	//    	append all output to logfile, set to "off" (default "trice.log")
	//  -list string
	//    	trice ID list path (default "til.json")
	//  -port string
	//    	COM port, options: COM1|...|COM999 (default "COMscan")
	//  -postfix string
	//    	append postfix to all lines (default "\n")
	//  -prefix string
	//    	prepend prefix to all lines, set to "off" (default "COMport:")
	//  -show
	//    	show passphrase
	//  -ts string
	//    	timestamp, options: off|UTCmicro (default "LOCmicro")
}
