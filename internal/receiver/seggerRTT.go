// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package receiver

import (
	"fmt"
	"net"
	"time"
)

const endpoint = "127.0.0.1:19021"

var conn *net.TCPConn // special extension

// seggerRTT may be unexported
type seggerRTT struct {
	*triceReceiver
	name string
}

// NewSeggerRTTReceiver creates an instance
func newSeggerRTTReceiver() *seggerRTT {
	r := &seggerRTT{}                     // create SeggerRTT instance
	r.triceReceiver = newTriceReceiver(r) // create triceReceiver instance and link it
	r.name = "Segger RTT connection"
	return r
}

// Read is the exported method
func (p *seggerRTT) Read(buf []byte) (int, error) {
	return conn.Read(buf)
}

// setUp() initializes the SeggerRTT  receiver
//
// It connects over TCP.
func (p *seggerRTT) setUp() error {
	fmt.Printf("JLinkLogViewer reading from %s\n", endpoint)
	jlinkExeAddr, err := net.ResolveTCPAddr("tcp4", endpoint)
	if err != nil {
		return err
	}

	conn, err = net.DialTCP("tcp4", nil, jlinkExeAddr)
	if err != nil {
		return err
	}
	return nil
}

// cleanUp() de-initializes the receiver
func (p *seggerRTT) cleanUp() {
	conn.Close()
}

// DoSeggerRTT is the endless loop for trice logging
func DoSeggerRTT() {
	rtt := newSeggerRTTReceiver()

	for nil != rtt.setUp() {
		fmt.Println("Could not set up", rtt.name, ", trying again...")
		<-time.After(2 * time.Second)
		continue
	}
	fmt.Println("Opened", rtt.name)
	defer rtt.cleanUp()

	rtt.start()
	defer rtt.stop()

	rtt.doReceive()
}

/*
// export readBytes
func (p *seggerRTT) readBytes(count int) (int, []byte) {
	b := make([]byte, count) // the buffer size limits the read count
	n, err := p.Read(b)

	if err != nil {
		log.Fatal(err)
	}
	return n, b
}
*/
