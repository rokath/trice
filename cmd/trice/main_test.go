// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package main

import (
	"bytes"
	"os"
	"sync"
	"testing"

	"github.com/rokath/trice/internal/args"
	"github.com/rokath/trice/pkg/tst"
)

var (
	m *sync.RWMutex
)

func init() {
	m = new(sync.RWMutex)
}

func execHelper(t *testing.T, expect string) {
	m.Lock()
	defer m.Unlock()
	args.FlagsInit() // maybe needed for clearance of previous tests (global vars)
	var out bytes.Buffer
	doit(&out)
	act := out.String()
	tst.EqualLines(t, expect, act)
}

func TestTriceWrong(t *testing.T) {
	os.Args = []string{"trice", "wrong"}
	expect := `unknown sub-command 'wrong'. try: 'trice help|h'
	`
	execHelper(t, expect)
}

func _TestTriceVersion(t *testing.T) {
	version = "1.2.3"
	commit = "myCommit"
	date = "2006-01-02_1504-05"
	os.Args = []string{"trice", "version"}
	expect := `version=1.2.3, commit=myCommit, built at 2006-01-02_1504-05
	`
	execHelper(t, expect)
	version = ""
	commit = ""
	date = ""
}

/*
func stdoutDoit() {
	doit(os.Stdout)
}

var osArgsBUFFER []string

func example(args []string) {
	m.Lock()
	dn, e := ioutil.TempDir("", "sampledir")
	msg.FatalOnErr(e)
	//fn := createCFile(dn)
	os.Args = append(args, "-src", dn)
	//act := tst.CaptureStdOut(stdoutDoit)
	//b, e := os.ReadFile(fn)
	msg.FatalOnErr(e)
	msg.FatalOnErr(os.RemoveAll(dn))
	id.DryRun = false // reset changed value
	os.Args = os.Args[:0]
	m.Unlock()
}

//for some reason this test disturbs in parallel execution
func _TestDoitDssd(t *testing.T) {
	o := msg.OsExitDisallow()
	defer msg.OsExitAllow(o)
	log.SetFlags(0)
	os.Args = []string{"trice", "ds", "-color", "off", "-ipp", "61496"}
	var outDS bytes.Buffer
	args.FlagsInit()
	go func() {
		doit(&outDS)
	}()
	time.Sleep(100 * time.Millisecond)
	actDS := outDS.String()
	expDS := "displayServer @ localhost:61496\n"
	tst.EqualLines(t, expDS, actDS)

	os.Args = []string{"trice", "sd", "-ipp", "61496", "-v"}
	var outSD bytes.Buffer
	go func() {
		doit(&outSD)
	}()
	time.Sleep(1000000 * time.Millisecond)
	actSD := outSD.String()
	actM := actSD //[0:19] + "xxxxx" + actSD[24:]
	expSD := "\n\ndbg:displayServerxxxxxdown\n\n\naccept tcp 127.0.0.1:61496: use of closed network connection"
	tst.EqualLines(t, expSD, actM)
}

func _ExampleDoit5() {
	example([]string{"trice", "zeroSourceTreeIds", "-dry-run"})
	// Output:
	// ... TRICE0( Id(100), "tata");...
	// Id(100)  ->  Id(0)
}

func _ExampleDoit6() {
	example([]string{"trice", "zeroSourceTreeIds"})
	// Output:
	// ... TRICE0( Id(0), "tata");...
	// Id(100)  ->  Id(0)
}

func _getTemporaryFileName(pattern string) string {
	tempFileHandle, e := ioutil.TempFile(os.TempDir(), pattern) // opens for read and write
	msg.OnErr(e)
	tempFileName := tempFileHandle.Name()
	msg.OnErr(tempFileHandle.Close())
	return tempFileName
}

//  func TestMain(t *testing.M) {
//  	// do stuff before tests
//  	fnTIL := createTIL()
//  	osArgsBUFFER = []string{"trice", "log", "-p", "BUFFER", "-args", "2, 124, 227, 255, 0, 0, 4, 0", "-ts", "off", "-color", "off", "-idlist", fnTIL}
//
//  	exitVal := t.Run() // tests
//
//  	// do stuff after tests
//  	msg.FatalOnErr(os.Remove(fnTIL))
//
//  	os.Exit(exitVal) // done
//  }

func _ExampleDoit3() {
	m.Lock()

	os.Args = osArgsBUFFER
	//act := tst.CaptureStdOut(stdoutDoit)
	os.Args = os.Args[:0]
	m.Unlock()
	// Output:
	// BUFFER: MSG: triceFifoMaxDepth = 4, select = 0
}

func _ExampleDoit4() {
	m.Lock()
	os.Args = append(osArgsBUFFER, "-testTable", "-prefix", " }, `")
	//act := tst.CaptureStdOut(stdoutDoit)
	os.Args = os.Args[:0]
	decoder.TestTableMode = false       // reset changed value
	emitter.Prefix = args.DefaultPrefix // reset changed value
	m.Unlock()
	// Output:
	//{ []byte{   2,124,227,255,  0,  0,  4,  0, }, `MSG: triceFifoMaxDepth = 4, select = 0`},
}

//  // createTIL returns a name of a temporary file containing a valid trice id JSON map
//  func _createTIL() (fn string) {
//  	// til is the trace id list content for tests
//  	til := `{
//  		"1047663": {
//  			"Type": "TRICE16_2",
//  			"Strg": "MSG: triceFifoMaxDepth = %d, select = %d\\n"
//  		}
//  	}
//  	`
//  	fn = getTemporaryFileName("til*.JSON") // temporary til
//  	b := []byte(til)
//  	msg.FatalOnErr(os.WriteFile(fn, b, 0644))
//  	return
//  }

//  func _createCFile(dn string) (fn string) {
//  	text := `
//  	... TRICE0( Id(100), "tata");...
//  `
//  	fn = dn + "/file1.c"
//  	b := []byte(text)
//  	msg.FatalOnErr(os.WriteFile(fn, b, 0644))
//  	return
//  }

func _TestDoitDssd1(t *testing.T) {
	o := msg.OsExitDisallow()
	defer msg.OsExitAllow(o)
	m.Lock()
	log.SetFlags(0)
	os.Args = []string{"trice", "ds", "-color", "off", "-ipp", "61498"}
	var actDS string
	go func() {
		actDS = tst.CaptureStdOut(stdoutDoit)
	}()
	time.Sleep(100 * time.Millisecond)

	//os.Args = append(osArgsBUFFER, "-ds")
	//actSD := tst.CaptureStdOut(stdoutDoit)

	os.Args = []string{"trice", "sd", "-ipp", "61498"}
	actSD := tst.CaptureStdOut(stdoutDoit)
	os.Args = os.Args[:0]
	time.Sleep(100 * time.Millisecond)

	m.Unlock()
	assert.Equal(t, "displayServer @ localhost:61498\nBUFFER: MSG: triceFifoMaxDepth = 4, select = 0\n", actDS)
	actM := actSD[0:19] + "xxxxx" + actSD[24:]
	assert.Equal(t, "\n\ndbg:displayServerxxxxxdown\n\n\naccept tcp 127.0.0.1:61498: use of closed network connection", actM)
}

func _TestDoitDssd2(t *testing.T) {
	o := msg.OsExitDisallow()
	defer msg.OsExitAllow(o)
	m.Lock()
	os.Args = append(osArgsBUFFER, "-ds", "-autostart", "-ipp", "61499")
	//act := tst.CaptureStdOut(stdoutDoit)
	time.Sleep(100 * time.Millisecond)

	os.Args = []string{"trice", "sd", "-ipp", "61499"}
	actSD := tst.CaptureStdOut(stdoutDoit)
	os.Args = os.Args[:0]
	time.Sleep(100 * time.Millisecond)
	m.Unlock()
	actM := actSD[0:19] + "xxxxx" + actSD[24:]
	assert.Equal(t, "read tcp 127.0.0.1:xxxxx->127.0.0.1:61499: wsarecv: An existing connection was forcibly closed by the remote host.", actM)
}
*/
