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
	"sync"

	"github.com/rokath/trice/pkg/lgf"
)

var (
	// IPAddr is the display server ip addr
	IPAddr string = "localhost" // default value for testing

	// IPPort is the display server ip addr
	IPPort string = "61497" // default value for testing

	// mux is for syncing line output
	mux sync.Mutex
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

/*
// Adder is a demo for a 2nd function
func (p *Server) Adder(u [2]int64, reply *int64) error {
	*reply = u[0] + u[1]
	return nil
}
*/

// LogSetFlags is called remotely to shutdown display server
func (p *Server) LogSetFlags(f []int64, r *int64) error {
	flags := int(f[0])
	log.SetFlags(flags)
	*r = f[0]
	return nil
}

var exit = false

// Exit is called remotely to shutdown display server
func (p *Server) Exit([]int64, *int64) error {
	exit = true
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

	ln, err := net.Listen("tcp", a)
	if err != nil {
		fmt.Println(err)
		return err
	}
	for false == exit {
		c, err := ln.Accept()
		if err != nil {
			continue
		}
		go rpc.ServeConn(c)
	}
	return nil
}
