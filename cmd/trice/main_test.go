// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package main

import (
	"fmt"
	"io/ioutil"
	"log"
	"os"
	"sync"
	"testing"
	"time"

	"github.com/rokath/trice/internal/args"
	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/pkg/msg"
	"github.com/rokath/trice/pkg/tst"
	"github.com/tj/assert"
)

var (
	m *sync.RWMutex
)

func init() {
	m = new(sync.RWMutex)
}

func getTemporaryFileName(pattern string) string {
	tempFileHandle, e := ioutil.TempFile(os.TempDir(), pattern) // opens for read and write
	msg.OnErr(e)
	tempFileName := tempFileHandle.Name()
	msg.OnErr(tempFileHandle.Close())
	return tempFileName
}

func Example_doit_a() {
	m.Lock()
	version = "1.2.3"
	commit = "myCommit"
	date = "2006-01-02_1504-05"
	os.Args = []string{"trice", "ver"}
	doit()
	os.Args = os.Args[:0]
	version = ""
	commit = ""
	date = ""
	m.Unlock()
	// Output:
	// version=1.2.3, commit=myCommit, built at 2006-01-02_1504-05
}

func Example_doit_b() {
	m.Lock()
	os.Args = []string{"trice", "wrong"}
	doit()
	os.Args = os.Args[:0]
	m.Unlock()
	// Output:
	// unknown subcommand 'wrong'. try: 'trice help|h'
}

func TestDoit_ver(t *testing.T) {
	m.Lock()
	version = "1.2.3"
	commit = "myCommit"
	date = "2006-01-02_1504-05"
	os.Args = []string{"trice", "ver"}
	act := tst.CaptureStdOut(doit)
	exp := "version=1.2.3, commit=myCommit, built at 2006-01-02_1504-05\n"
	version = ""
	commit = ""
	date = ""
	m.Unlock()
	assert.Equal(t, exp, act)
}

func TestDoit_scan(t *testing.T) {
	m.Lock()
	os.Args = []string{"trice", "scan"}
	act := tst.CaptureStdOut(doit)
	m.Unlock()
	assert.Equal(t, "", act[:0])
}

//for some reason this test disturbs in parallel execution
func TestDoit_dssd(t *testing.T) {
	m.Lock()
	log.SetFlags(0)
	os.Args = []string{"trice", "ds", "-color", "off", "-ipp", "61496"}
	var actDS string
	go func() {
		actDS = tst.CaptureStdOut(doit)
	}()
	time.Sleep(100 * time.Millisecond)

	os.Args = []string{"trice", "sd", "-ipp", "61496"}
	actSD := tst.CaptureStdOut(doit)
	time.Sleep(100 * time.Millisecond)
	os.Args = os.Args[:0]
	m.Unlock()
	fmt.Println(actDS)
	assert.Equal(t, "displayServer @ localhost:61496\n", actDS)
	actM := actSD[0:19] + "xxxxx" + actSD[24:]
	assert.Equal(t, "\n\ndbg:displayServerxxxxxdown\n\n\naccept tcp 127.0.0.1:61496: use of closed network connection", actM)
}

func _TestDoit_dssd_a(t *testing.T) {
	m.Lock()
	log.SetFlags(0)
	os.Args = []string{"trice", "ds", "-color", "off", "-ipp", "61498"}
	var actDS string
	go func() {
		actDS = tst.CaptureStdOut(doit)
	}()
	time.Sleep(100 * time.Millisecond)

	os.Args = append(osArgsBUFFER, "-ds")
	act := tst.CaptureStdOut(doit)
	fmt.Print(act)

	os.Args = []string{"trice", "sd", "-ipp", "61498"}
	actSD := tst.CaptureStdOut(doit)
	os.Args = os.Args[:0]
	time.Sleep(100 * time.Millisecond)

	m.Unlock()
	fmt.Println(actDS)
	assert.Equal(t, "displayServer @ localhost:61498\nBUFFER: MSG: triceFifoMaxDepth = 4, select = 0\n", actDS)
	actM := actSD[0:19] + "xxxxx" + actSD[24:]
	assert.Equal(t, "\n\ndbg:displayServerxxxxxdown\n\n\naccept tcp 127.0.0.1:61498: use of closed network connection", actM)
}

func _TestDoit_dssd_b(t *testing.T) {
	m.Lock()
	os.Args = append(osArgsBUFFER, "-ds", "-autostart", "-ipp", "61499")
	act := tst.CaptureStdOut(doit)
	fmt.Print(act)
	time.Sleep(100 * time.Millisecond)

	os.Args = []string{"trice", "sd", "-ipp", "61499"}
	actSD := tst.CaptureStdOut(doit)
	os.Args = os.Args[:0]
	time.Sleep(100 * time.Millisecond)
	m.Unlock()
	actM := actSD[0:19] + "xxxxx" + actSD[24:]
	assert.Equal(t, "read tcp 127.0.0.1:xxxxx->127.0.0.1:61499: wsarecv: An existing connection was forcibly closed by the remote host.", actM)
}

// createTIL returns a name of a temporary file containing a valid trice id JSON map
func createTIL() (fn string) {
	// til is the trace id list content for tests
	til := `{
		"1047663": {
			"Type": "TRICE16_2",
			"Strg": "MSG: triceFifoMaxDepth = %d, select = %d\\n"
		}
	}
	`
	fn = getTemporaryFileName("til*.JSON") // temporary til
	b := []byte(til)
	msg.FatalOnErr(os.WriteFile(fn, b, 0644))
	return
}

// ... TRICE0( Id(101), "tata")...
// ... Trice0( id(102), "tata");...
// ... Trice16_1( id(103), "%d", 77);...
// ... TRICE0(  "tata");...
// ... TRICE0( "tata")...
// ... Trice0( "tata");...
// ... Trice16_1(  "%d", 77);...

func createCFile(dn string) (fn string) {
	text := `
	... TRICE0( Id(100), "tata");...
`
	fn = dn + "/file1.c"
	b := []byte(text)
	msg.FatalOnErr(os.WriteFile(fn, b, 0644))
	return
}

var osArgsBUFFER []string

func TestMain(t *testing.M) {
	// do stuff before tests
	fnTIL := createTIL()
	osArgsBUFFER = []string{"trice", "log", "-p", "BUFFER", "-args", "2, 124, 227, 255, 0, 0, 4, 0", "-ts", "off", "-color", "off", "-idlist", fnTIL}

	exitVal := t.Run() // tests

	// do stuff after tests
	msg.FatalOnErr(os.Remove(fnTIL))

	os.Exit(exitVal) // done
}

func Example_doit_c() {
	m.Lock()

	os.Args = osArgsBUFFER
	act := tst.CaptureStdOut(doit)
	os.Args = os.Args[:0]
	fmt.Print(act)
	m.Unlock()
	// Output:
	// BUFFER: MSG: triceFifoMaxDepth = 4, select = 0
}

func Example_doit_d() {
	m.Lock()
	os.Args = append(osArgsBUFFER, "-testTable", "-prefix", " }, `")
	act := tst.CaptureStdOut(doit)
	fmt.Print(act)
	os.Args = os.Args[:0]
	decoder.TestTableMode = false       // reset changed value
	emitter.Prefix = args.DefaultPrefix // reset changed value
	m.Unlock()
	// Output:
	//{ []byte{   2,124,227,255,  0,  0,  4,  0, }, `MSG: triceFifoMaxDepth = 4, select = 0`},
}

func Example_doit_e() {
	m.Lock()
	dn, e := ioutil.TempDir("", "sampledir")
	msg.FatalOnErr(e)
	fn := createCFile(dn)
	os.Args = []string{"trice", "zeroSourceTreeIds", "-src", dn, "-dry-run"}
	act := tst.CaptureStdOut(doit)
	b, e := os.ReadFile(fn)
	msg.FatalOnErr(e)
	fmt.Print(string(b))
	fmt.Print(act)
	msg.FatalOnErr(os.RemoveAll(dn))
	id.DryRun = false // reset changed value
	os.Args = os.Args[:0]
	m.Unlock()
	// Output:
	// ... TRICE0( Id(100), "tata");...
	// Id(100)  ->  Id(0)
}

func Example_doit_f() {
	m.Lock()
	dn, e := ioutil.TempDir("", "sampledir")
	msg.FatalOnErr(e)
	fn := createCFile(dn)
	os.Args = []string{"trice", "zeroSourceTreeIds", "-src", dn}
	act := tst.CaptureStdOut(doit)
	b, e := os.ReadFile(fn)
	msg.FatalOnErr(e)
	fmt.Print(string(b))
	fmt.Print(act)
	msg.FatalOnErr(os.RemoveAll(dn))
	os.Args = os.Args[:0]
	m.Unlock()
	// Output:
	// ... TRICE0( Id(0), "tata");...
	// Id(100)  ->  Id(0)
}

func Example_doit_g() {
	m.Lock()
	dn, e := ioutil.TempDir("", "sampledir")
	msg.FatalOnErr(e)
	fn := createCFile(dn)
	os.Args = []string{"trice", "zeroSourceTreeIds", "-src", dn}
	act := tst.CaptureStdOut(doit)
	b, e := os.ReadFile(fn)
	msg.FatalOnErr(e)
	fmt.Print(string(b))
	fmt.Print(act)
	msg.FatalOnErr(os.RemoveAll(dn))
	os.Args = os.Args[:0]
	m.Unlock()
	// Output:
	// ... TRICE0( Id(0), "tata");...
	// Id(100)  ->  Id(0)
}

func Example_scan() {
	m.Lock()
	dn, e := ioutil.TempDir("", "sampledir")
	msg.FatalOnErr(e)
	fn := createCFile(dn)
	os.Args = []string{"trice", "zeroSourceTreeIds", "-src", dn}
	act := tst.CaptureStdOut(doit)
	b, e := os.ReadFile(fn)
	msg.FatalOnErr(e)
	fmt.Print(string(b))
	fmt.Print(act)
	msg.FatalOnErr(os.RemoveAll(dn))
	os.Args = os.Args[:0]
	m.Unlock()
	// Output:
	// ... TRICE0( Id(0), "tata");...
	// Id(100)  ->  Id(0)
}

// todo: Tab am Anfang, Semikolon am Ende, id vs Id, mehrere in einer Zeile testen, alle Varianten in Liste
