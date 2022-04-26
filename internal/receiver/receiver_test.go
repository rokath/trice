// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package receiver

import (
	"fmt"
	"io"
	"net"
	"os"
	"testing"

	"github.com/tj/assert"
)

func TestBUFFERReceiver(t *testing.T) {
	var verbose bool
	rc, err := NewReadCloser(os.Stdout, verbose, "BUFFER", "7 123 44")
	assert.Nil(t, err)
	b := make([]byte, 100)
	n, err := rc.Read(b)
	assert.Nil(t, err)
	assert.True(t, n == 3)
	assert.True(t, b[0] == 7)
	assert.True(t, b[2] == 44)
	n, err = rc.Read(b)
	assert.True(t, 0 == n)
	assert.True(t, io.EOF == err)
	assert.Nil(t, rc.Close())
}

func TestDUMPReceiver(t *testing.T) {
	var verbose bool
	rc, err := NewReadCloser(os.Stdout, verbose, "DUMP", "7B 1A ee,88, 5a")
	assert.Nil(t, err)
	b := make([]byte, 100)
	n, err := rc.Read(b)
	assert.Nil(t, err)
	assert.True(t, 5 == n)
	assert.True(t, 0x7b == b[0])
	assert.True(t, 0x5a == b[4])
	n, err = rc.Read(b)
	assert.True(t, 0 == n)
	assert.True(t, io.EOF == err)
	assert.Nil(t, rc.Close())
}

func TestFILEReceiver(t *testing.T) {
	dir := t.TempDir()
	fn := dir + "/trices.raw"
	f, err := os.Create(fn)
	assert.Nil(t, err)
	d := []byte{115, 111, 109, 101, 10}
	n, err := f.Write(d)
	assert.Nil(t, err)
	assert.True(t, n == len(d))

	var verbose bool
	rc, err := NewReadCloser(os.Stdout, verbose, "FILE", fn)
	assert.Nil(t, err)
	b := make([]byte, 100)
	n, err = rc.Read(b)
	assert.Nil(t, err)
	assert.True(t, 5 == n)
	assert.True(t, b[0] == 115)
	assert.True(t, b[4] == 10)
	rc.Close()
}

func TestTCP4Receiver(t *testing.T) {
	var addr net.Addr
	go func() {
		const (
			CONN_TYPE = "tcp4"
			CONN_HOST = "127.0.0.1"
			CONN_PORT = "" // let port be chosen
		)
		// Listen for incoming connections.
		l, err := net.Listen(CONN_TYPE, CONN_HOST+":"+CONN_PORT)
		assert.Nil(t, err)
		addr = l.Addr()
		// Close the listener when the application closes.
		defer l.Close()
		//fmt.Println("Listening on " + CONN_HOST + ":" + CONN_PORT)
		for {
			// Listen for an incoming connection.
			conn, err := l.Accept()
			assert.Nil(t, err)
			// Handle connections in a new goroutine.
			go handleRequest(conn)
		}
	}()

	var verbose bool
	for addr == nil { // wait until server is up
	}
	s := fmt.Sprint(addr)
	rc, err := NewReadCloser(os.Stdout, verbose, "TCP4", s)
	assert.Nil(t, err)
	b := make([]byte, 100)
	n, err := rc.Read(b)
	assert.Nil(t, err)
	assert.Equal(t, []byte{0x01, 0x7f, 0xFF}, b[:n])
}

// Handles incoming requests.
func handleRequest(conn net.Conn) {
	// Send a response back to person contacting us.
	conn.Write([]byte{0x01, 0x7f, 0xFF})
	// Close the connection when you're done with it.
	conn.Close()
}
