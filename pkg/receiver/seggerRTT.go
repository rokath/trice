package receiver

import (
	"fmt"
	"log"
	"net"
	"time"
)

const endpoint = "127.0.0.1:19021"

var conn *net.TCPConn // special extension

// seggerRTT may be unexported
type seggerRTT struct { // ConcreteDaemonB
	*triceReceiver // inherit
}

// NewSeggerRTTReceiver creates an instance
func newSeggerRTTReceiver() *seggerRTT {
	r := &seggerRTT{}
	r.triceReceiver = newTriceReceiver(r)
	return r
}

// Start starts receiving of RTT data
func (p *seggerRTT) Start() {
	p.receivingData = true
	go p.receiving()
}

// Stop stops receiving of RTT data
func (p *seggerRTT) Stop() {
	p.receivingData = false
}

func (p *seggerRTT) Read(buf []byte) (int, error) {
	return conn.Read(buf)
}

func (p *seggerRTT) receiving() {
	fmt.Printf("JLinkLogViewer reading from %s\n", endpoint)
	for {
		jlinkExeAddr, err := net.ResolveTCPAddr("tcp4", endpoint)
		if err != nil {
			<-time.After(2 * time.Second)
			continue
		}

		conn, err = net.DialTCP("tcp4", nil, jlinkExeAddr)
		if err != nil {
			<-time.After(2 * time.Second)
			continue
		}

		data := make([]byte, 1024)
		fmt.Printf("\n\n===============================================================================\n")
		for err == nil {
			n, err := conn.Read(data)
			if err != nil {
				conn.Close()
				<-time.After(2 * time.Second)
				break
			}
			fmt.Printf("%s", string(data[0:n]))
		}
	}
}

// DoSeggerRTT is the endless loop for trice logging
func DoSeggerRTT() {
	rtt := newSeggerRTTReceiver()

	rtt.Start()
	defer rtt.Stop()

	rtt.doReceive()
}

// export readBytes
func (p *seggerRTT) readBytes(count int) (int, []byte) {
	b := make([]byte, count) // the buffer size limits the read count
	n, err := p.Read(b)

	if err != nil {
		log.Fatal(err)
	}
	return n, b
}
