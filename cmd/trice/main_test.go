// SPDX-License-Identifier: MIT

package main

import (
	"bytes"
	"fmt"
	"io"
	"net"
	"net/rpc"
	"os"
	"testing"
	"time"

	"github.com/rokath/trice/internal/args"
	"github.com/rokath/trice/internal/do"
	"github.com/rokath/trice/internal/id"
	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
)

// reserveLoopbackAddress selects an ephemeral IPv4 port. Using an explicit
// address avoids platform-dependent localhost resolution between IPv4 and
// IPv6 while the server and shutdown client start concurrently.
func reserveLoopbackAddress(t *testing.T) (host, port, address string) {
	t.Helper()
	listener, err := net.Listen("tcp", "127.0.0.1:0")
	if err != nil {
		t.Fatalf("cannot reserve loopback address: %v", err)
	}
	address = listener.Addr().String()
	if err := listener.Close(); err != nil {
		t.Fatalf("cannot release reserved loopback address %s: %v", address, err)
	}
	host, port, err = net.SplitHostPort(address)
	if err != nil {
		t.Fatalf("cannot split loopback address %s: %v", address, err)
	}
	return host, port, address
}

// waitForDisplayServer waits for the listener rather than for its earlier
// startup text. ScDisplayServer emits that text immediately before net.Listen,
// so the text alone does not prove that an RPC client can connect yet.
func waitForDisplayServer(t *testing.T, address string, serverDone <-chan error) {
	t.Helper()
	deadline := time.Now().Add(3 * time.Second)
	for {
		connection, err := net.DialTimeout("tcp", address, 100*time.Millisecond)
		if err == nil {
			_ = connection.Close()
			return
		}
		select {
		case serverErr := <-serverDone:
			t.Fatalf("display server stopped before becoming reachable at %s: %v", address, serverErr)
		default:
		}
		if time.Now().After(deadline) {
			t.Fatalf("display server did not become reachable at %s: %v", address, err)
		}
		time.Sleep(10 * time.Millisecond)
	}
}

// setupTest - look for "This is the greatest answer in this thread" in  https://stackoverflow.com/questions/23729790/how-can-i-do-test-setup-using-the-testing-package-in-go
func setupTest(t *testing.T, fSys *afero.Afero) func() {
	//fmt.Println("Setup code for", t.Name())
	fmt.Println("enter_" + t.Name())
	version = ""
	commit = ""
	date = ""
	args.Version = version
	args.Commit = commit
	args.Date = date
	args.Verbose = false
	args.LogfileName = "off"

	id.Verbose = false
	id.DryRun = false
	id.FnJSON = "til.json"
	id.LIFnJSON = "li.json"
	id.Min = id.TriceID(1000)
	id.Max = id.TriceID(7999)
	id.SearchMethod = "random"
	id.LIRoot = ""
	id.LIMaxDirs = 0
	//id.Srcs                     ArrayFlag
	//id.IDData                   idData
	id.ExtendMacrosWithParamCount = false
	id.DefaultTriceBitWidth = "32"

	id.DefaultStampSize = 32
	id.StampSizeId = ""

	do.DistributeArgs(os.Stdout, fSys, args.LogfileName, args.Verbose)

	// create empty til.json
	assert.Nil(t, fSys.WriteFile("til.json", []byte(``), 0777))

	// create empty li.json
	assert.Nil(t, fSys.WriteFile("li.json", []byte(``), 0777))

	// tear down later
	return func() {
		//fmt.Println("tear-down code for ", t.Name())
		fmt.Println("leave_" + t.Name())
	}
}

// TestTriceVersion1 verifies the expected behavior.
func TestTriceVersion1(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	setupTest(t, fSys)
	defer setupTest(t, fSys)()

	args.Version = "1.2.3"
	args.Commit = "myCommit"
	args.Date = "2006-01-02_1504-05"
	var b bytes.Buffer
	e := args.Handler(&b, fSys, []string{"trice", "version"})
	assert.Nil(t, e)
	assert.Equal(t, "version=1.2.3, commit=myCommit, built at 2006-01-02_1504-05\n", b.String())
}

// TestTriceWrong1 verifies the expected behavior.
func TestTriceWrong1(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer setupTest(t, fSys)()

	var b bytes.Buffer
	e := args.Handler(&b, fSys, []string{"trice", "wrong"})
	assert.Equal(t, "", b.String())
	assert.Equal(t, "unknown sub-command 'wrong'. try: 'trice help|h'\n", fmt.Sprintln(e))
}

// TestDisplayServerStartAndShutdownLegacy fails with -race
//
// When this test is executed before TestDisplayServerStartAndShutdown0() the TestDisplayServerStartAndShutdown0() gives this message:
// "Error in server.go:91: func 'github.com/rokath/trice/internal/emitter.ScDisplayServer' -> rpc: service already defined: DisplayServer"
// - You're using the default RPC server when you call rpc.Register, so no, you can't have more than one.
// - If you want more than one, make separate RPC servers: http://play.golang.org/p/g7VHJ03E-j
// So we disable this test for now.
func _TestDisplayServerStartAndShutdownLegacy(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer setupTest(t, fSys)()

	var ds, sd bytes.Buffer
	go args.Handler(&ds, fSys, []string{"trice", "ds", "-color", "none", "-ipp", "61495"})
	for ds.String() != "displayServer @ localhost:61495\n" {
	}
	args.Handler(&sd, fSys, []string{"trice", "sd", "-ipp", "61495"})
	assert.Equal(t, "displayServer @ localhost:61495\n\n\ndisplayServer shutdown\n\n\n", ds.String())
	assert.Equal(t, "", sd.String())
}

// TestDisplayServerStartAndShutdown0 verifies CLI-level display-server startup
// and RPC shutdown after the listener is ready for connections. Calling the
// shutdown CLI through args.Handler here would run the process-global argument
// parser concurrently with the server CLI, unlike two real trice processes.
func TestDisplayServerStartAndShutdown0(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer setupTest(t, fSys)()
	host, port, address := reserveLoopbackAddress(t)

	var ds bytes.Buffer
	serverDone := make(chan error, 1)
	go func() {
		serverDone <- args.Handler(&ds, fSys, []string{"trice", "ds", "-color", "none", "-ipa", host, "-ipp", port})
	}()
	waitForDisplayServer(t, address, serverDone)

	client, err := rpc.Dial("tcp", address)
	if err != nil {
		t.Fatalf("cannot connect shutdown RPC client to %s: %v", address, err)
	}
	var shutdownReply int64
	shutdownErr := client.Call("DisplayServer.Shutdown", []int64{0}, &shutdownReply)
	assert.NoError(t, shutdownErr)
	assert.NoError(t, client.Close())
	select {
	case <-serverDone:
	case <-time.After(3 * time.Second):
		t.Fatal("display server did not stop after the shutdown RPC")
	}

	exp := "displayServer @ " + address + "\n\n\ndisplayServer shutdown\n\n\n"
	act := ds.String()
	assert.Equal(t, exp, act)
}

// TestDisplayServerStartAndShutdown1 does not come to an end  for some reason!
// ds should be a channel to avoid that, but I do notknow how to do that here without changing the args.Handler signature.
func _TestDisplayServerStartAndShutdown1(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer setupTest(t, fSys)()

	rd, wr := io.Pipe()
	go args.Handler(wr, fSys, []string{"trice", "ds", "-color", "none", "-ipp", "61498"})
	time.Sleep(100 * time.Millisecond)

	expect := "displayServer @ localhost:61498\n"
	out := make([]byte, 1000)
	n, err := rd.Read(out)
	assert.Nil(t, err)
	actual := string(out[:n])
	assert.Equal(t, expect, actual)

	var sd bytes.Buffer
	args.Handler(&sd, fSys, []string{"trice", "sd", "-ipp", "61498"})
	time.Sleep(100 * time.Millisecond)
	wr.Close() // close the writer, so the reader knows there's no more data

	exp := "displayServer @ localhost:61498\n\n\ndisplayServer shutdown\n\n\n"
	n, err = rd.Read(out)
	assert.Nil(t, err)
	act := string(out[:n])
	assert.Equal(t, exp, act)

	assert.Equal(t, "", sd.String())
}
