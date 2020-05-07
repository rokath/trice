package receiver

import (
	"fmt"
	"net"
	"time"
)

const endpoint = "127.0.0.1:19021"

type SeggerRTT struct {
	receiverT // inherit
}

// NewSeggerRTTReceiver creates an instance
func NewSeggerRTTReceiver() *SeggerRttReceiver {
	s := &SeggerRttReceiver{
		receiverT: receiverT{"SeggerRttReceiver", false, make(chan []byte), make(chan []byte)},
	}
	return s
}

// Start starts receiving of RTT data
func (p *SeggerRTT) Start() {
	p.receivingData = true
	go p.receiving()
}

// Stop stops receiving of RTT data
func (p *SeggerRTT) Stop() {
	p.receivingData = false
}

func (p *SeggerRTT) receiving() {
	fmt.Printf("JLinkLogViewer reading from %s\n", endpoint)
	for {
		jlinkExeAddr, err := net.ResolveTCPAddr("tcp4", endpoint)
		if err != nil {
			<-time.After(2 * time.Second)
			continue
		}

		conn, err := net.DialTCP("tcp4", nil, jlinkExeAddr)
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
