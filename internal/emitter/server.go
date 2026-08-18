// SPDX-License-Identifier: MIT

package emitter

import (
	"errors"
	"fmt"
	"io"
	"log"
	"net"
	"net/rpc"
	"time"

	"github.com/rokath/trice/pkg/msg"
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This code was derived from the information in:
// https://stackoverflow.com/questions/37122401/execute-another-go-program-from-within-a-golang-program/37123869#37123869
// "4 - another way is using "net/rpc", this is the best way for calling another function from another program."
//

// DisplayServer exposes RPC methods for remote line rendering.
type DisplayServer struct { // must be exported for rpc.Register
	Display colorDisplay

	// listener belongs exclusively to this server instance. Shutdown closes it
	// to unblock the matching accept loop without package-global coordination.
	listener net.Listener
}

// WriteLine is the exported server method for string display, if trice tool acts as display server.
// By declaring it as a Server struct method it is registered as RPC destination.
func (p *DisplayServer) WriteLine(line []string, reply *int64) error {
	*reply = int64(len(line))
	p.Display.WriteLine(line)
	return nil // todo: ? p.Display.lw.Err
}

// ColorPalette is the exported server function for color palette, if trice tool acts as display server.
// By declaring it as a Server struct method it is registered as RPC destination.
func (p *DisplayServer) ColorPalette(s []string, reply *int64) error {
	ColorPalette = s[0]
	*reply = 0
	return nil
}

// LogSetFlags updates the process log package flags remotely.
func (p *DisplayServer) LogSetFlags(f []int64, r *int64) error {
	flags := int(f[0])
	log.SetFlags(flags)
	*r = f[0]
	return nil
}

// Shutdown emits a shutdown marker and closes the server listener.
func (p *DisplayServer) Shutdown(ts []int64, _ *int64) error {
	timeStamp := ts[0]
	p.Display.WriteLine([]string{""})
	p.Display.WriteLine([]string{""})
	if timeStamp == 1 { // for normal usage
		p.Display.WriteLine([]string{"time:" + time.Now().String(), "dbg:displayServer shutdown"})
	} else { // for testing
		p.Display.WriteLine([]string{"dbg:displayServer shutdown"})
	}
	p.Display.WriteLine([]string{""})
	p.Display.WriteLine([]string{""})
	if p.listener != nil {
		msg.OnErr(p.listener.Close())
	}
	return nil
}

// ScDisplayServer serves RPC display requests until the listener is closed.
func ScDisplayServer(w io.Writer) error {
	a := fmt.Sprintf("%s:%s", IPAddr, IPPort)
	fmt.Fprintln(w, "displayServer @", a)
	srv := &DisplayServer{Display: *newColorDisplay(w, ColorPalette)}
	rpcServer := rpc.NewServer()
	if err := rpcServer.Register(srv); err != nil {
		fmt.Fprintln(w, err)
		return err
	}
	listener, err := net.Listen("tcp", a)
	if nil != err {
		fmt.Fprintln(w, err)
		return err
	}
	// The listener is assigned before any RPC handler can run and remains
	// immutable afterwards. net.Listener supports concurrent Accept and Close.
	srv.listener = listener
	for {
		conn, err := listener.Accept()
		if nil != err {
			if errors.Is(err, net.ErrClosed) {
				return err
			}
			continue
		}
		go rpcServer.ServeConn(conn)
	}
}
