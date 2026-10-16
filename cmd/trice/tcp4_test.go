// SPDX-License-Identifier: MIT

package main

import (
	"bufio"
	"bytes"
	"errors"
	"fmt"
	"net"
	"testing"
	"time"

	"github.com/rokath/trice/internal/args"
	"github.com/rokath/trice/internal/do"
	"github.com/rokath/trice/pkg/tst"
	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
)

// tcpTestListener owns one real loopback connection, including the accepted
// socket that the output handler otherwise retains after finite input ends.
type tcpTestListener struct {
	*net.TCPListener
	accepted chan net.Conn
}

// newTCPTestListener binds once to an OS-assigned port. Deadlines also bound
// failure paths; cleanup closes both sides even when an assertion stops a test.
func newTCPTestListener(t *testing.T) *tcpTestListener {
	t.Helper()
	listener, err := net.ListenTCP("tcp4", &net.TCPAddr{IP: net.IPv4(127, 0, 0, 1)})
	if !assert.NoError(t, err) {
		t.FailNow()
	}
	assert.NoError(t, listener.SetDeadline(time.Now().Add(5*time.Second)))
	tracked := &tcpTestListener{TCPListener: listener, accepted: make(chan net.Conn, 1)}
	t.Cleanup(tracked.closeSockets)
	return tracked
}

// Accept retains the socket for cleanup and bounds the handler's handshake.
func (l *tcpTestListener) Accept() (net.Conn, error) {
	connection, err := l.TCPListener.Accept()
	if err != nil {
		return nil, err
	}
	if err := connection.SetDeadline(time.Now().Add(5 * time.Second)); err != nil {
		_ = connection.Close()
		return nil, err
	}
	l.accepted <- connection
	return connection, nil
}

// closeSockets is safe before or after the handler closes its listener.
func (l *tcpTestListener) closeSockets() {
	_ = l.Close()
	select {
	case connection := <-l.accepted:
		_ = connection.Close()
	default:
	}
}

// handleConnection serves a single test TCP connection with predefined data.
func handleConnection(conn net.Conn) error {
	// Close the connection when we're done
	defer conn.Close()

	// No incoming data to read.
	//     buf := make([]byte, 1024)
	//     n, err := conn.Read(buf)

	// That is the used source code line:
	// TRice( iD(16201), "w: Hello! 👋🙂 \a\n" ); // with sound!

	// That is copied from the terminal
	//  ms@DESKTOP-7POEGPB MINGW64 ~/repos/trice/examples/G0B1_inst (master)
	//  $ trice log -p com6 -pw MySecret -pf COBS -debug -showID "deb:%5d"
	//  COBS: 09 92 19 06 45 0b 10 56 3a 00
	//  ->TRICE: 92 19 06 45 0b 10 56 3a
	//  -> DEC:  49 ff 00 00 00 00 c0 00
	//  Aug 21 13:15:54.639047  com6:                main.c    75    0,000_000 16201  Hello! 👋🙂

	// That is the 0-terminated transfer package
	_, err := conn.Write([]byte{0x09, 0x92, 0x19, 0x06, 0x45, 0x0b, 0x10, 0x56, 0x3a, 0x00})
	return err
}

// TestTCP4Reception sets up a trivial trice log server, which ends on request a single trice package.
// This is a COBS encoded and encrypted package, derived from real data.
// When the Trice tool is started, it read from the trice log server over TCP4 and decodes the received package.
func TestTCP4Reception(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer setupTest(t, fSys)()

	// Keep the old one-letter prefix in these captured-message fixtures.
	// It is no longer a built-in tag, so every receiver must preserve "w:".
	til := `{
	"16201": {
		"Type": "TRice",
		"Strg": "w: Hello! 👋🙂\\n"
	}
}`

	// create a minimalistic til.json
	assert.Nil(t, fSys.WriteFile("til.json", []byte(til), 0777))

	// One connection is sufficient for this finite-input fixture. Closing the
	// listener unblocks Accept on failure, and the test always joins its server.
	listener := newTCPTestListener(t)
	serverDone := make(chan error, 1)
	go func() {
		connection, err := listener.Accept()
		if err == nil {
			err = handleConnection(connection)
		}
		serverDone <- err
	}()
	defer func() {
		listener.closeSockets()
		select {
		case err := <-serverDone:
			if !errors.Is(err, net.ErrClosed) {
				assert.NoError(t, err)
			}
		case <-time.After(5 * time.Second):
			t.Error("TCP reception server did not stop")
		}
	}()

	// We use "-port TCP4BUFFER" just for the test, to force the Trice tool to shutdown after receiving a package.
	// In real life, the user will enter "-port TCP4" instead. To keep things simple we switch off all unnecessary information.
	input := []string{"trice", "log", "-port", "TCP4BUFFER", "-args", listener.Addr().String(), "-pw", "MySecret", "-pf", "cobs", "-li", "off", "-hs", "off", "-color", "none", "-prefix", "off", "-ts", "off"}
	expect := `w: Hello! 👋🙂
`

	var out bytes.Buffer
	err := args.Handler(&out, fSys, input)
	if err != nil {
		fmt.Fprint(&out, err)
	}
	assert.Nil(t, err)

	act := out.String()
	tst.EqualLines(t, expect, act)

}

// TestHEX verifies the expected behavior.
func TestHEX(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer setupTest(t, fSys)()

	til := `{
	"16201": {
		"Type": "TRice",
		"Strg": "w: Hello! 👋🙂\\n"
	}
}`

	// create a minimalistic til.json
	assert.Nil(t, fSys.WriteFile("til.json", []byte(til), 0777))
	input := []string{"trice", "log", "-port", "HEX", "-args", "09 92 19 06 45 0b 10 56 3a,00", "-pw", "MySecret", "-pf", "cobs", "-li", "off", "-hs", "off", "-color", "none", "-prefix", "off", "-ts", "off"}
	expect := `w: Hello! 👋🙂
`

	var out bytes.Buffer
	err := args.Handler(&out, fSys, input)
	if err != nil {
		fmt.Fprint(&out, err)
	}
	assert.Nil(t, err)

	act := out.String()
	tst.EqualLines(t, expect, act)
}

// TestDUMP verifies the expected behavior.
func TestDUMP(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer setupTest(t, fSys)()

	til := `{
	"16201": {
		"Type": "TRice",
		"Strg": "w: Hello! 👋🙂\\n"
	}
}`

	// create a minimalistic til.json
	assert.Nil(t, fSys.WriteFile("til.json", []byte(til), 0777))
	input := []string{"trice", "log", "-port", "DUMP", "-args", "09 92 19 06 45 0b 10 56 3a,00", "-pw", "MySecret", "-pf", "cobs", "-li", "off", "-hs", "off", "-color", "none", "-prefix", "off", "-ts", "off"}
	expect := `w: Hello! 👋🙂
`

	var out bytes.Buffer
	err := args.Handler(&out, fSys, input)
	if err != nil {
		fmt.Fprint(&out, err)
	}
	assert.Nil(t, err)

	act := out.String()
	tst.EqualLines(t, expect, act)
}

// TestBUFFER verifies the expected behavior.
func TestBUFFER(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer setupTest(t, fSys)()

	til := `{
	"16201": {
		"Type": "TRice",
		"Strg": "w: Hello! 👋🙂\\n"
	}
}`

	// create a minimalistic til.json
	assert.Nil(t, fSys.WriteFile("til.json", []byte(til), 0777))
	input := []string{"trice", "log", "-port", "BUFFER", "-args", "9 146 25 6 69 11 16 86 58 00", "-pw", "MySecret", "-pf", "cobs", "-li", "off", "-hs", "off", "-color", "none", "-prefix", "off", "-ts", "off"}
	expect := `w: Hello! 👋🙂
`

	var out bytes.Buffer
	err := args.Handler(&out, fSys, input)
	if err != nil {
		fmt.Fprint(&out, err)
	}
	assert.Nil(t, err)

	act := out.String()
	tst.EqualLines(t, expect, act)
}

// TestDEC verifies the expected behavior.
func TestDEC(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer setupTest(t, fSys)()

	til := `{
	"16201": {
		"Type": "TRice",
		"Strg": "w: Hello! 👋🙂\\n"
	}
}`

	// create a minimalistic til.json
	assert.Nil(t, fSys.WriteFile("til.json", []byte(til), 0777))
	input := []string{"trice", "log", "-port", "DEC", "-args", "9 146 25 6 69 11 16 86 58 00", "-pw", "MySecret", "-pf", "cobs", "-li", "off", "-hs", "off", "-color", "none", "-prefix", "off", "-ts", "off"}
	expect := `w: Hello! 👋🙂
`

	var out bytes.Buffer
	err := args.Handler(&out, fSys, input)
	if err != nil {
		fmt.Fprint(&out, err)
	}
	assert.Nil(t, err)

	act := out.String()
	tst.EqualLines(t, expect, act)
}

// TestHEXToTCP verifies the expected behavior.
func TestHEXToTCP(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer setupTest(t, fSys)()

	til := `{
	"16201": {
		"Type": "TRice",
		"Strg": "w: Hello! 👋🙂\\n"
	}
}`

	listener := newTCPTestListener(t)
	// Inject the live socket rather than reserving a port and reopening it.
	// Restore globals before setupTest's deferred teardown can open output again.
	savedListen, savedAddress := do.TCPListen, do.TCPOutAddr
	do.TCPListen = func(network, address string) (net.Listener, error) {
		if network != "tcp" || address != listener.Addr().String() {
			return nil, fmt.Errorf("unexpected TCP output endpoint %s %s", network, address)
		}
		return listener, nil
	}
	defer func() { do.TCPListen, do.TCPOutAddr = savedListen, savedAddress }()

	// create a minimalistic til.json
	assert.Nil(t, fSys.WriteFile("til.json", []byte(til), 0777))
	input := []string{"trice", "log", "-port", "HEX", "-args", "09 92 19 06 45 0b 10 56 3a,00", "-pw", "MySecret", "-pf", "cobs", "-li", "off", "-hs", "off", "-color", "none", "-prefix", "off", "-ts", "off", "-tcp", listener.Addr().String()}
	exp := "w: Hello! 👋🙂\n"

	// The fixture already listens before Handler starts, so dialing needs no
	// sleep or readiness probe. Join before restoring any package globals.
	var output bytes.Buffer
	var handlerErr error
	handlerDone := make(chan struct{})
	go func() {
		handlerErr = args.Handler(&output, fSys, input)
		close(handlerDone)
	}()
	defer func() {
		listener.closeSockets()
		select {
		case <-handlerDone:
			assert.NoError(t, handlerErr, output.String())
		case <-time.After(5 * time.Second):
			t.Error("TCP output handler did not stop")
		}
	}()

	conn, err := net.DialTimeout("tcp4", listener.Addr().String(), 5*time.Second)
	if !assert.NoError(t, err) {
		t.FailNow()
	}
	defer conn.Close()
	assert.NoError(t, conn.SetDeadline(time.Now().Add(5*time.Second)))

	n, e := conn.Write([]byte{0})
	assert.Nil(t, e)
	assert.Equal(t, 1, n)

	// A TCP read may return a partial line; compare one complete decoded record.
	act, e := bufio.NewReader(conn).ReadString('\n')
	assert.Nil(t, e)
	fmt.Println("exp=", exp)
	fmt.Println("act=", act)
	//assert.Equal(t, len(exp), n) // TODO: Error: Not equal: expected: 17 actual  : 18
	tst.EqualLines(t, exp, act)
}
