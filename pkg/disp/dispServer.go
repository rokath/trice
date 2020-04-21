// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package disp is for dispatching and displaying trice log lines.
// This file contains the server code.
package disp

import (
	"fmt"
	"log"
	"net"
	"net/rpc"
	"time"

	"github.com/rokath/trice/pkg/lgf"
)

var (
	// IPAddr is the display server ip addr
	IPAddr string = "localhost" // default value for testing

	// IPPort is the display server ip addr
	IPPort string = "61497" // default value for testing

	// ColorPalette is the used PC color set. It is initialized with its default value inside the appropriate subcommand.
	ColorPalette = "off" // default value for testing

)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This code was derived from the information in:
// https://stackoverflow.com/questions/37122401/execute-another-go-program-from-within-a-golang-program/37123869#37123869
// "4 - another way is using "net/rpc", this is best way for calling another function from another program."
//

// Server is the RPC struct for registered server dunctions
type Server struct{}

// Out is the exported server function for string display, if trice tool acts as display server.
// By declaring it as a Server struct method it is registered as RPC destination.
func (p *Server) Out(s []string, reply *int64) error {
	*reply = int64(len(s))
	return Out(s) // this function pointer has its default value on server side
}

// ColorPalette is the exported server function for color palette, if trice tool acts as display server.
// By declaring it as a Server struct method it is registered as RPC destination.
func (p *Server) ColorPalette(s []string, reply *int64) error {
	ColorPalette = s[0]
	*reply = 0
	return nil
}

// LogSetFlags is called remotely to shutdown display server
func (p *Server) LogSetFlags(f []int64, r *int64) error {
	flags := int(f[0])
	log.SetFlags(flags)
	*r = f[0]
	return nil
}

var (
	// exit is usually false, when true thwe display server exits
	exit = false

	// conn is used only inside ScDisplayServer
	conn net.Conn
	// ln is needed for shutdown
	ln net.Listener
)

// Shutdown is called remotely to shutdown display server
func (p *Server) Shutdown(ts []int64, y *int64) error {
	timeStamp := ts[0]
	out([]string{""})
	out([]string{""})
	if 1 == timeStamp { // for normal usage
		out([]string{"time:" + time.Now().String(), "dbg:displayServer shutdown"})
	} else { // for testing
		out([]string{"dbg:displayServer shutdown"})
	}
	out([]string{""})
	out([]string{""})
	defer func() {
		err := ln.Close()
		if nil != err {
			fmt.Println(err)
		}
		//fmt.Println("exit...")
		exit = true // do not set true before closing ln, otherwise panic!
		/*
			// no need for this code
			if nil != conn {
				fmt.Println("Calling net.Conn conn.Close()...")
				err = conn.Close()
				if nil != err {
					fmt.Println(err)
				} else {
					fmt.Println("Calling conn.Close()...done")
				}
			} else {
				fmt.Println("'conn' is nil, cannot call conn.Close()")
			}
		*/
	}()
	return nil
}

// ScDisplayServer is the endless function called when trice tool acts as remote display.
// All in Server struct registered RPC functions are reachable, when displayServer runs.
func ScDisplayServer() error {
	lgf.Enable()
	defer lgf.Disable()

	a := fmt.Sprintf("%s:%s", IPAddr, IPPort)
	fmt.Println("displayServer @", a)
	rpc.Register(new(Server))
	var err error
	ln, err = net.Listen("tcp", a)
	if nil != err {
		fmt.Println(err)
		return err
	}
	for {
		conn, err = ln.Accept()
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
