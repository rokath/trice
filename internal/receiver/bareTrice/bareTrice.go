// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package bareTriceReader reads 4 byte atomic trices and extends them to 8 byte sure trices.
package bareTrice

import (
	"bytes"
	"fmt"
	"io"
	"net"

	"github.com/rokath/trice/internal/emit"
	"github.com/rokath/trice/internal/receiver"
)

// bare contains the inner reader to read from
type bare struct {
	r io.Reader // inner reader
}

// New creates an instance of RTT ReadCloser.
//
// It is intended to be used by receiver.New() which embeds its interface.
func NewReader(i io.Reader) *bare {
	r := &bare{} // create bare reader instance
	r.r = i
	return r
}

var t []byte // trice slice

// Read() is part of the exported interface io.ReadCloser. It reads a slice of bytes.
//
// Usually t is empty when Read is executed, so otherwise Read returns only some or all unread bytes from t.
//
// Read has an internal boolean state *inSync* which is true or false.
// Initially inSync is true. When the method OutOfSync() is called inSync=false and Read reads until a sync package is received.
// Then it automatically goes into the inSync==true state.
//
// When started first time it returns 0 bytes as long the total expected count is < 8.
// When total expected count is 8, it tries to read 4 bytes from inner reader with ReadFull.
// As long the inner reader does not return 4 bytes this Read fn return 0 bytes.
// When 4 bytes came from inner reader, 8 bytes are returned.
// The outer reader cannot detect if out of sync, because the crc is computed here.
//
// If some heuristic in the outer reader comes to the conclusion out-of-sync it calls the method OutOfSync.
// Then after start Read reads until a sync package is reached and then goes backward and provides the data.


read max 8 byte (one sure trice) and keep the 4 bare bytes
func (p *bare) Read(buf []byte) (int, error) {
	var n int
	var err error
	r := p.r // inner reader

	if 0 == len(buf) { // formal special case
		fmt.Println("read len 0 is not expected here")
		return 0, nil
	}

	if 0 < len(t) {
		fmt.Print("some unfinished reads:")
		fmt.Print("t:", t)
		n = copy(buf, t)
		t = t[n:]
		fmt.Print("buf:", buf)
		return n, nil
	}

	b := make([]byte, 4)
	n, err = io.ReadFull(r, b)
	if nil != err {
		return 0, err
	}

	t = []byte{0xeb, 0x60, 0x60, 0}
	t = append(t, b...)

	t[3] = t[0] ^ t[1] ^ t[2] ^ t[4] ^ t[5] ^ t[6] ^ t[7] // crc
	n = copy(buf, t)

	t = t[n:]
	return n, nil
}

// sync pattern to wait for
// The id 8*257 = 2056 is reserved for sync packages and occures only with the 16 bit parameter 2056.
// The target sends after reset such sync trice and should do so cyclically (each second is a good value).
var sp = []byte{22, 22, 22, 22}

// syncMsg returns true if b matches the sync pattern
func syncMsg(b []byte) bool {
	if bytes.Equal(b, sp) {
		return true
	}
	return false
}

// synchronize reads and discards bytes until the sync pattern is received.
//
// If the sync state is false all internally read bytes are discarded until the sync condition appears.
// The sync pattern is expected to be read after a target reset. Then an out of sync is not expected
// anymore. But the trice tool can also connect to a running target. For thas case the target should
// emit cyclically sync messages, for example each second.
func (p *RTT) synchronize() error {
	var r io.Reader
	r = p.conn
	b := make([]byte, 4)
	fmt.Println("synchronize...")
	_, err := io.ReadFull(r, b)
	if nil != err {
		return err
	}
	for false == syncMsg(b) {
		b, err = receiver.ReadNextByte(r, b)
		if nil != err {
			return err
		}
	}
	emit.Clear()
	fmt.Println("synchronize...done")
	return nil
}

// Close is part of the exported interface io.ReadCloser. It ends the connection.
func (p *RTT) Close() error {
	return p.conn.Close()
}

// Open initializes the SeggerRTT receiver
//
// It connects over TCP.
// It reads and discards bytes until it gets a sync message.
func (p *RTT) Open() error {
	p.endpoint = "127.0.0.1:19021"
	fmt.Printf("JLinkLogViewer reading from %s\n", p.endpoint)
	jlinkExeAddr, err := net.ResolveTCPAddr("tcp4", p.endpoint)
	if err != nil {
		return err
	}

	p.conn, err = net.DialTCP("tcp4", nil, jlinkExeAddr)
	if err != nil {
		return err
	}
	return p.synchronize()
}
