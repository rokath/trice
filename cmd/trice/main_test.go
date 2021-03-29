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
	version = "1.2.3"
	commit = "myCommit"
	date = "2006-01-02_1504-05"
	os.Args = []string{"trice", "ver"}
	act := tst.CaptureStdOut(doit)
	fmt.Print(act)
	// Output:
	// version=1.2.3, commit=myCommit, built at 2006-01-02_1504-05
}

func Example_doit_b() {
	version = "1.2.3"
	commit = "myCommit"
	date = "2006-01-02_1504-05"
	os.Args = []string{"trice", "wrong"}
	act := tst.CaptureStdOut(doit)
	fmt.Print(act)
	// Output:
	// unknown subcommand 'wrong'. try: 'trice help|h'
}

func TestDoit_ver(t *testing.T) {
	version = "1.2.3"
	commit = "myCommit"
	date = "2006-01-02_1504-05"
	os.Args = []string{"trice", "ver"}
	act := tst.CaptureStdOut(doit)
	exp := "version=1.2.3, commit=myCommit, built at 2006-01-02_1504-05\n"
	version = ""
	commit = ""
	date = ""
	assert.Equal(t, exp, act)
}

func TestDoit_scan(t *testing.T) {
	os.Args = []string{"trice", "scan"}
	act := tst.CaptureStdOut(doit)
	assert.Equal(t, "", act[:0])
}

func TestDoit_dssd(t *testing.T) {
	log.SetFlags(0)
	os.Args = []string{"trice", "ds", "-color", "off"}
	var actDS string
	go func() {
		actDS = tst.CaptureStdOut(doit)
	}()
	time.Sleep(100 * time.Millisecond)
	os.Args = []string{"trice", "sd"}
	actSD := tst.CaptureStdOut(doit)
	fmt.Println(actDS)
	assert.Equal(t, "displayServer @ localhost:61497\n", actDS)
	actM := actSD[0:19] + "xxxxx" + actSD[24:]
	assert.Equal(t, "\n\ndbg:displayServerxxxxxdown\n\n\naccept tcp 127.0.0.1:61497: use of closed network connection", actM)
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

func TestMain(m *testing.M) {
	// do stuff before tests
	fnTIL := createTIL()
	osArgsBUFFER = []string{"trice", "log", "-p", "BUFFER", "-args", "2, 124, 227, 255, 0, 0, 4, 0", "-ts", "off", "-color", "off", "-idlist", fnTIL}

	exitVal := m.Run() // tests

	// do stuff after tests
	msg.FatalOnErr(os.Remove(fnTIL))

	os.Exit(exitVal) // done
}

func Example_doit_c() {
	m.Lock()

	os.Args = osArgsBUFFER
	act := tst.CaptureStdOut(doit)
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
	m.Unlock()
	// Output:
	// ... TRICE0( Id(0), "tata");...
	// Id(100)  ->  Id(0)
}

// todo: Tab am Anfang, Semikolon am Ende, id vs Id, mehrere in einer Zeile testen, alle Varianten in Liste
