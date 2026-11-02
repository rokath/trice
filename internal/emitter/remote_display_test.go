// SPDX-License-Identifier: MIT

package emitter

import (
	"bytes"
	"io"
	"net"
	"net/rpc"
	"strings"
	"testing"
	"time"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// TestAliasRemoteDisplayProvenance sends real RPC payloads through the display
// server without opening a TCP port or reparsing conflicting message prefixes.
func TestAliasRemoteDisplayProvenance(t *testing.T) {
	saved := snapshotEmitterState()
	t.Cleanup(func() { restoreEmitterState(saved) })
	HostStamp = "off"
	Prefix = ""
	Suffix = ""
	for _, test := range []struct{ client, server, want string }{
		{"none", "none", "first err:second\nerr:continuation\n"},
		{"none", "off", "wrn:first err:second\nerr:continuation\n"},
		{"off", "none", "first err:second\nerr:continuation\n"},
	} {
		t.Run(test.client+"/"+test.server, func(t *testing.T) {
			ColorPalette = test.client
			require.NoError(t, AddUserLabels())
			var output bytes.Buffer
			server := rpc.NewServer()
			require.NoError(t, server.RegisterName("DisplayServer", &DisplayServer{Display: *newColorDisplay(&output, test.server)}))
			clientConn, serverConn := net.Pipe()
			go server.ServeConn(serverConn)
			client := rpc.NewClient(clientConn)
			defer client.Close()
			defer serverConn.Close()
			remote := &remoteDisplay{PtrRPC: client}
			composer := newLineComposer(remote)
			_, err := composer.WriteEvent("wrn:first ", "WARNING", "wrn")
			require.NoError(t, err)
			_, err = composer.WriteEvent("err:second\nerr:continuation\n", "WARNING", "err")
			require.NoError(t, err)
			require.NoError(t, remote.Err)
			assert.Equal(t, test.want, output.String())
		})
	}
}

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
