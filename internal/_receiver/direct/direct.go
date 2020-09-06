// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package segger reads from SeggerRTT.
package direct

import (
	"bytes"
	"fmt"
	"io"
	"net"

	"github.com/rokath/trice/internal/emit"
	"github.com/rokath/trice/internal/receiver"
)

// RTT is the Segger RealTime Transfer reader interface.
type RTT struct {
	endpoint string
	conn     *net.TCPConn
}

// New creates an instance of RTT ReadCloser.
//
// It is intended to be used by receiver.New() which embeds its interface.
func New() *RTT {
	r := &RTT{} // create SeggerRTT instance
	return r
}

var t []byte // trice slice

// Read() is part of the exported interface io.ReadCloser. It reads a slice of bytes.
//
// Usually t is empty when Read is executed, so otherwise Read returns only some or all unread bytes from t.
func (p *RTT) Read(buf []byte) (int, error) {
	var n int
	var err error
	var r io.Reader

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

	r = p.conn
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
