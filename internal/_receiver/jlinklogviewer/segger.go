// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package segger reads from SeggerRTT.
package segger

import (
	"fmt"
	"net"
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

// Read() is part of the exported interface io.ReadCloser. It reads a slice of bytes.
func (p *RTT) Read(buf []byte) (int, error) {
	return p.conn.Read(buf)
}

// Close is part of the exported interface io.ReadCloser. It ends the connection.
func (p *RTT) Close() error {
	return p.conn.Close()
}

// open() initializes the SeggerRTT  receiver
//
// It connects over TCP.
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
	return nil
}
