// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package disp

import (
	"fmt"
	"log"
	"net"
	"net/rpc"
	"os/exec"
	"runtime"
	"sync"

	"github.com/fatih/color"
	"github.com/rokath/trice/pkg/lgf"
)

var (
	// IpAddr is the display server ip addr
	IpAddr string

	// IpPort is the display server ip addr
	IpPort string

	// Visualize is an exported function pointer, which can be redirected for example to a client call
	Visualize = visualize

	// PtrRpc is a pointer
	PtrRpc *rpc.Client

	// mux is for syncing line output
	mux sync.Mutex
)

// StartServer starts a display server (if not already running)
func StartServer() {
	var shell string
	var clip []string
	if runtime.GOOS == "windows" {
		shell = "cmd"
		shellCmd := "/c start"
		clip = append(clip, shellCmd+" trice displayServer -ipa "+IpAddr+" -ipp "+IpPort+" -lf off")
	} else if runtime.GOOS == "linux" {
		shell = "gnome-terminal" // this only works for gnome based linux desktop env
		clip = append(clip, "--", "/bin/bash", "-c", "trice displayServer -ipa "+IpAddr+" -ipp "+IpPort+" -lf off")
	} else {
		log.Fatal("trice is running on unknown operating system")
	}
	cmd := exec.Command(shell, clip...)

	err := cmd.Run()
	if err != nil {
		log.Println(clip)
		log.Fatal(err)
	}
}

// visualize displays ss and sets color.
// ss is a slice containing substring parts of one line.
// Each substring inside ss is result of Trice or contains prefix,
// timestamp or postfix and can have its own color prefix.
// The last substring inside the slice is definitly the last in
// the line and has already trimmed its newline.
// Linebreaks inside the substrings are not handled separately (yet).
func visualize(ss []string) error {
	var c *color.Color
	var line string

	mux.Lock()
	for _, s := range ss {
		c, s = colorChannel(s)
		line += c.Sprint(s)
	}
	o := color.NoColor
	color.NoColor = true
	c.Print(line) // here better use siply fmt.Print, but then the io.Writer to os.file assignment issue must be solved
	color.NoColor = o
	mux.Unlock()
	return nil
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This code was derived from the information in:
// https://stackoverflow.com/questions/37122401/execute-another-go-program-from-within-a-golang-program/37123869#37123869
// "4 - another way is using "net/rpc", this is best way for calling another function from another program."
//

// Server is the RPC struct for registered server dunctions
type Server struct{}

// Visualize is the exported server function for string display, if trice tool acts as display server.
// By declaring is as a Server struct method it is registered as RPC destination.
func (p *Server) Visualize(s []string, reply *int64) error {
	*reply = int64(len(s))
	return Visualize(s) // this function pointer has its default value on server side
}

/*
// Adder is a demo for a 2nd function
func (p *Server) Adder(u [2]int64, reply *int64) error {
	*reply = u[0] + u[1]
	return nil
}
*/

// ScServer is the endless function called when trice tool acts as remote display.
// All in Server struct registered RPC functions are reachable, when displayServer runs.
func ScServer() error {
	lgf.Enable()
	defer lgf.Disable()

	a := fmt.Sprintf("%s:%s", IpAddr, IpPort)
	fmt.Println("displayServer @", a)
	rpc.Register(new(Server))

	ln, err := net.Listen("tcp", a)
	if err != nil {
		fmt.Println(err)
		return err
	}
	for {
		c, err := ln.Accept()
		if err != nil {
			continue
		}
		go rpc.ServeConn(c)
	}
}

// remoteVisualize does send the logstring s to the displayServer
// It is replacing emit.Visuaize when trice acts as remote
func RemoteVisualize(s []string) error {
	// for a bit more accurate timestamps they should be added
	// here on the receiver side and not in the displayServer
	var result int64
	err := PtrRpc.Call("Server.Visualize", s, &result)
	if err != nil {
		return err
	}
	//fmt.Println("result is", result) // not needed here
	return nil
}

// Connect is called by the client and tries to dial.
// On success PtrRpc is valid afterwards and zhe output is re-directed
func Connect() error {
	var err error
	a := fmt.Sprintf("%s:%s", IpAddr, IpPort)
	fmt.Println("remoteDisplay@", a)
	PtrRpc, err = rpc.Dial("tcp", a)
	if err != nil {
		fmt.Println(err)
		return err
	}
	fmt.Println("Connected...")
	Visualize = RemoteVisualize // re-direct output
	return nil
}
