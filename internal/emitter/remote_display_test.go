// SPDX-License-Identifier: MIT

package emitter

import (
	"io"
	"net"
	"net/rpc"
	"strings"
	"testing"
	"time"
)

type shutdownStub struct {
	writeLines chan []string
	shutdown   chan []int64
}

func (s *shutdownStub) WriteLine(line []string, reply *int64) error {
	cp := append([]string(nil), line...)
	s.writeLines <- cp
	*reply = int64(len(cp))
	return nil
}

func (s *shutdownStub) Shutdown(ts []int64, _ *struct{}) error {
	s.shutdown <- ts
	return nil
}

// TestRemoteDisplayRPCWriteAndShutdown drives a stub RPC server and exercises WriteLine plus shutdown handling.
func TestRemoteDisplayRPCWriteAndShutdown(t *testing.T) {
	requireWindowsTCPTestsEnabled(t)

	l, err := net.Listen("tcp", "127.0.0.1:0")
	if err != nil {
		t.Fatalf("listen failed: %v", err)
	}
	defer l.Close()

	addr := l.Addr().String()
	host, port, err := net.SplitHostPort(addr)
	if err != nil {
		t.Fatalf("split hostport: %v", err)
	}

	stub := &shutdownStub{
		writeLines: make(chan []string, 1),
		shutdown:   make(chan []int64, 1),
	}
	server := rpc.NewServer()
	if err := server.RegisterName("DisplayServer", stub); err != nil {
		t.Fatalf("register failed: %v", err)
	}

	go func() {
		for {
			conn, acceptErr := l.Accept()
			if acceptErr != nil {
				return
			}
			go server.ServeConn(conn)
		}
	}()

	IPAddr = host
	IPPort = port

	waitForTCPListener(t, addr)

	client := newRemoteDisplay(io.Discard, []string{""})
	client.WriteLine([]string{"test line"})
	if client.Err != nil {
		t.Fatalf("WriteLine failed: %v", client.Err)
	}

	select {
	case got := <-stub.writeLines:
		if len(got) != 1 || !strings.Contains(got[0], "test line") {
			t.Fatalf("unexpected payload: %#v", got)
		}
	case <-time.After(2 * time.Second):
		t.Fatalf("no WriteLine payload received")
	}

	client.stopServer(0)
	if err := client.Err; err != nil {
		t.Fatalf("stopServer returned error: %v", err)
	}
	select {
	case ts := <-stub.shutdown:
		if len(ts) != 1 || ts[0] != 0 {
			t.Fatalf("unexpected shutdown ts: %#v", ts)
		}
	case <-time.After(2 * time.Second):
		t.Fatalf("shutdown call missing")
	}
}
