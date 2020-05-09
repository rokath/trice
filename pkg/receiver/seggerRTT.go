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
type seggerRTT struct {
	*triceReceiver
	name string
}

// NewSeggerRTTReceiver creates an instance
func newSeggerRTTReceiver() *seggerRTT {
	r := &seggerRTT{}
	r.triceReceiver = newTriceReceiver(r)
	r.name = "Segger RTT connection"
	return r
}

// Start starts receiving of RTT data
func (p *seggerRTT) start() {
	p.receivingData = true
	go p.receiving()
}

// Stop stops receiving of RTT data
func (p *seggerRTT) stop() {
	p.receivingData = false
}

func (p *seggerRTT) Read(buf []byte) (int, error) {
	return conn.Read(buf)
}

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

func (p *seggerRTT) cleanUp() {
	conn.Close()
}

/*
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
		//DoSeggerRTT()
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
*/

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
