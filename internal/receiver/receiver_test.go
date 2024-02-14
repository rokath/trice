// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package receiver

import (
	"fmt"
	"io"
	"net"
	"testing"

	"github.com/spf13/afero"
	"github.com/tj/assert"
)

// TestBUFFERReceiver tests the NewReadWriteCloser BUFFER functionality.
func TestBUFFERReceiver(t *testing.T) {
	rc, err := NewReadWriteCloser(nil, nil, false, "BUFFER", "7 123 44")
	assert.Nil(t, err)
	b := make([]byte, 4)
	n, err := rc.Read(b)
	assert.Nil(t, err)
	assert.True(t, n == 3)
	assert.True(t, b[0] == 7)
	assert.True(t, b[1] == 123)
	assert.True(t, b[2] == 44)
	n, err = rc.Read(b)
	assert.True(t, n == 0)
	assert.True(t, err == io.EOF)
	assert.Nil(t, rc.Close())
}

// TestDUMPReceiver tests the NewReadWriteCloser DUMP functionality.
func TestDUMPReceiver(t *testing.T) {
	rc, err := NewReadWriteCloser(nil, nil, false, "DUMP", "7B 1A ee,88, 5a")
	assert.Nil(t, err)
	b := make([]byte, 100)
	n, err := rc.Read(b)
	assert.Nil(t, err)
	assert.True(t, n == 5)
	assert.True(t, b[0] == 0x7b)
	assert.True(t, b[4] == 0x5a)
	n, err = rc.Read(b)
	assert.True(t, n == 0)
	assert.True(t, io.EOF == err)
	assert.Nil(t, rc.Close())
}

// TestFILEReceiver tests the NewReadWriteCloser FILE functionality.
func TestFILEReceiver(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewOsFs()}
	fn := "./trices.raw"
	f, err := fSys.Create(fn)
	assert.Nil(t, err)
	d := []byte{115, 111, 109, 101, 10}
	n, err := f.Write(d)
	assert.Nil(t, err)
	assert.True(t, n == len(d))
	rc, err := NewReadWriteCloser(nil, fSys, false, "FILE", fn)
	assert.Nil(t, err)
	b := make([]byte, 100)
	n, err = rc.Read(b)
	assert.Nil(t, err)
	assert.True(t, n == len(d))
	assert.True(t, b[0] == 115)
	assert.True(t, b[4] == 10)
	rc.Close()
}

// TestTCP4Receiver tests the NewReadWriteCloser TCP4 functionality.
func TestTCP4Receiver(t *testing.T) {
	ch := make(chan net.Addr)

	go func() {
		const (
			CONN_TYPE = "tcp4"
			CONN_HOST = "127.0.0.1"
			CONN_PORT = "" // let port be chosen
		)
		// Listen for incoming connections.
		l, err := net.Listen(CONN_TYPE, CONN_HOST+":"+CONN_PORT)
		assert.Nil(t, err)
		ch <- l.Addr()
		// Close the listener when the application closes.
		defer l.Close()
		for {
			// Listen for an incoming connection.
			conn, err := l.Accept()
			assert.Nil(t, err)
			// Handle connections in a new goroutine.
			go handleRequest(conn)
		}
	}()

	addr := <-ch
	s := fmt.Sprint(addr)
	rc, err := NewReadWriteCloser(nil, nil, false, "TCP4", s)
	assert.Nil(t, err)
	b := make([]byte, 100)
	n, err := rc.Read(b)
	assert.Nil(t, err)
	assert.Equal(t, []byte{0x01, 0x7f, 0xFF}, b[:n])
}

// handleRequest handles incoming requests.
func handleRequest(conn net.Conn) {
	// Send a response back to person contacting us.
	conn.Write([]byte{0x01, 0x7f, 0xFF})
	// Close the connection when you're done with it.
	conn.Close()
}
