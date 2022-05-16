// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package main

import (
	"fmt"
	"io"
	"math/rand"
	"os"
	"time"

	"github.com/rokath/trice/internal/args"
)

var (
	// do not initialize, goreleaser will handle that
	version string

	// do not initialize, goreleaser will handle that
	commit string

	// do not initialize, goreleaser will handle that
	date string
)

// main is the entry point.
func main() {
	doit(os.Stdout)
}

// doit is the action.
func doit(w io.Writer) {

	// inject values
	args.Version = version
	args.Commit = commit
	args.Date = date
	/*
		const addr = "localhost:64000"
		// The net.Listen() function makes the program a TCP server. This functions returns a Listener variable, which is a generic network listener for stream-oriented protocols.
		fmt.Println("Listening on " + addr + "...")
		listen, err := net.Listen("tcp", addr)
		if err != nil {
			log.Fatal(err)
		}
		//defer listen.Close()
		go func() {
			for { // wait for a connection
				// t is only after a successful call to Accept() that the TCP server can begin to interact with TCP clients.
				conn, err := listen.Accept()
				fmt.Println("Accepting connection:", conn)
				if err != nil {
					fmt.Println(err)
					continue // try again
				}
				fmt.Println("A new connection exist, add it to the write list.")

				// Make a buffer to hold incoming data.
				buf := make([]byte, 1024)
				reqLen, err := conn.Read(buf)
				if err != nil {
					fmt.Println("Error reading:", err.Error())
				}
				fmt.Println(string(buf[:reqLen]))
				conn.Write([]byte("Trice connected...\n"))
				//args.TriceOutWriter = conn //io.MultiWriter(args.TriceOutWriter, conn)
				conn.Close()
			}
		}()
	*/
	rand.Seed(time.Now().UnixNano())
	e := args.Handler(os.Args)
	if nil != e {
		fmt.Fprintln(w, error.Error(e))
	}
}
