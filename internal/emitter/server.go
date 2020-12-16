// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// This file contains the server code.

package emitter

import (
	"fmt"
	"log"
	"net"
	"net/rpc"
	"time"

	"github.com/rokath/trice/pkg/cage"
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This code was derived from the information in:
// https://stackoverflow.com/questions/37122401/execute-another-go-program-from-within-a-golang-program/37123869#37123869
// "4 - another way is using "net/rpc", this is best way for calling another function from another program."
//

// Server is the RPC struct for registered server dunctions
type Server struct {
	Display ColorDisplay // todo: LineWriter?
}

// func (p *Server) Out(line []string, reply *int64) error {
// 	return p.WriteLine(line, reply)
// }

// WriteLine is the exported server method for string display, if trice tool acts as display server.
// By declaring it as a Server struct method it is registered as RPC destination.
func (p *Server) WriteLine(line []string, reply *int64) error {
	*reply = int64(len(line))
	p.Display.writeLine(line)
	return nil // todo: ? p.Display.lw.Err
}

// ColorPalette is the exported server function for color palette, if trice tool acts as display server.
// By declaring it as a Server struct method it is registered as RPC destination.
func (p *Server) ColorPalette(s []string, reply *int64) error {
	ColorPalette = s[0]
	*reply = 0
	return nil
}

// LogSetFlags is called remotely to ...
func (p *Server) LogSetFlags(f []int64, r *int64) error {
	flags := int(f[0])
	log.SetFlags(flags)
	*r = f[0]
	return nil
}

// Shutdown is called remotely to shutdown display server
func (p *Server) Shutdown(ts []int64, _ *int64) error {
	timeStamp := ts[0]
	p.Display.writeLine([]string{""})
	p.Display.writeLine([]string{""})
	if 1 == timeStamp { // for normal usage
		p.Display.writeLine([]string{"time:" + time.Now().String(), "dbg:displayServer shutdown"})
	} else { // for testing
		p.Display.writeLine([]string{"dbg:displayServer shutdown"})
	}
	p.Display.writeLine([]string{""})
	p.Display.writeLine([]string{""})
	defer func() {
		err := listener.Close()
		if nil != err {
			fmt.Println(err)
		}
		//fmt.Println("exit...")
		exit = true // do not set true before closing listener, otherwise panic!
		//	// no need for this code
		//	if nil != conn {
		//		fmt.Println("Calling net.Conn conn.Close()...")
		//		err = conn.Close()
		//		if nil != err {
		//			fmt.Println(err)
		//		} else {
		//			fmt.Println("Calling conn.Close()...done")
		//		}
		//	} else {
		//		fmt.Println("'conn' is nil, cannot call conn.Close()")
		//	}
	}()
	return nil
}

var (
	// exit is usually false, when true the display server exits
	exit = false

	// conn is used only inside ScDisplayServer bute here for Shutdown() trials
	conn net.Conn

	// listener is needed for shutdown
	listener net.Listener
)

// ScDisplayServer is the endless function called when trice tool acts as remote display.
// All in Server struct registered RPC functions are reachable, when displayServer runs.
func ScDisplayServer() error {
	cage.Enable()
	defer cage.Disable()

	a := fmt.Sprintf("%s:%s", IPAddr, IPPort)
	fmt.Println("displayServer @", a)
	srv := new(Server)
	srv.Display = *NewColorDisplay(ColorPalette)
	rpc.Register(srv)
	var err error
	listener, err = net.Listen("tcp", a)
	if nil != err {
		fmt.Println(err)
		return err
	}
	for {
		conn, err = listener.Accept()
		if nil != err {
			if true == exit {
				//fmt.Println("exit...done")
				return err
			}
			//fmt.Println(err)
			continue
		}
		go rpc.ServeConn(conn)
	}
}
