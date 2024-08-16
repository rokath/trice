// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// This file contains the server code.

package emitter

import (
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

// DisplayServer is the RPC struct for registered DisplayServer functions. Its methods must be exported.
type DisplayServer struct { // must be exported for rpc.Register
	Display colorDisplay // todo: lineWriter?
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

// LogSetFlags is called remotely to ...
func (p *DisplayServer) LogSetFlags(f []int64, r *int64) error {
	flags := int(f[0])
	log.SetFlags(flags)
	*r = f[0]
	return nil
}

// Shutdown is called remotely to shut down display server
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
	defer func() {
		msg.OnErr(listener.Close())
		exit = true // do not set true before closing listener, otherwise panic!
	}()
	return nil
}

var (
	// exit is usually false, when true the display server exits
	exit = false

	// conn is used only inside ScDisplayServer but here for Shutdown() trials
	conn net.Conn

	// listener is needed for shutdown
	listener net.Listener
)

// ScDisplayServer is the endless function called when trice tool acts as remote display.
// All in Server struct registered RPC functions are reachable, when displayServer runs.
func ScDisplayServer(w io.Writer) error {
	a := fmt.Sprintf("%s:%s", IPAddr, IPPort)
	fmt.Fprintln(w, "displayServer @", a)
	srv := new(DisplayServer)
	srv.Display = *newColorDisplay(w, ColorPalette)
	msg.OnErr(rpc.Register(srv))
	var err error
	listener, err = net.Listen("tcp", a)
	if nil != err {
		fmt.Fprintln(w, err)
		return err
	}
	for {
		conn, err = listener.Accept()
		if nil != err {
			if exit {
				return err
			}
			continue
		}
		go rpc.ServeConn(conn)
	}
}
