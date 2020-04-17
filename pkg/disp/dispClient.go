// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package disp is for dispatching and displaying trice log lines.
// This file contains the client code.
package disp

import (
	"fmt"
	"log"
	"net/rpc"
	"os/exec"
	"runtime"

	"github.com/rokath/trice/pkg/lgf"
)

var (
	// PtrRPC is a pointer
	PtrRPC *rpc.Client
)

// StartServer starts a display server (if not already running)
func StartServer() {
	var shell string
	var clip []string
	if runtime.GOOS == "windows" {
		shell = "cmd"
		shellCmd := "/c start"
		clip = append(clip, shellCmd+" trice ds -lf "+lgf.Name+" -ipa "+IPAddr+" -ipp "+IPPort)
	} else if runtime.GOOS == "linux" {
		shell = "gnome-terminal" // this only works for gnome based linux desktop env
		clip = append(clip, "--", "/bin/bash", "-c", "trice displayServer -ipa "+IPAddr+" -ipp "+IPPort+" -lf "+lgf.Name)
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

// StopServer sends signal to display server to quit
func StopServer() {
	var result int64
	var dummy []int64
	err := PtrRPC.Call("Server.Exit", dummy, &result)
	fmt.Print(err)
}

// RemoteOut does send the logstring s to the displayServer
// It is replacing emit.Visuaize when trice acts as remote
func RemoteOut(s []string) error {
	// for a bit more accurate timestamps they should be added
	// here on the receiver side and not in the displayServer
	var result int64
	return PtrRPC.Call("Server.Out", s, &result)
}

// Connect is called by the client and tries to dial.
// On success PtrRpc is valid afterwards and zhe output is re-directed
func Connect() error {
	var err error
	a := fmt.Sprintf("%s:%s", IPAddr, IPPort)
	fmt.Println("remoteDisplay@", a)
	PtrRPC, err = rpc.Dial("tcp", a)
	if err != nil {
		fmt.Println(err)
		return err
	}
	fmt.Println("Connected...")
	Out = RemoteOut // re-direct output
	return nil
}
