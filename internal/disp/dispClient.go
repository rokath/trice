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

	"github.com/rokath/trice/pkg/cage"
)

var (
	// PtrRPC is a pointer for remote calls valid after a succesful rpc.Dial()
	PtrRPC *rpc.Client
)

// StartServer starts a display server with the filename exe (if not already running)
func StartServer(exe string) {
	var shell string
	var clip []string
	if runtime.GOOS == "windows" {
		shell = "cmd"
		shellCmd := "/c start " + exe
		clip = append(clip, shellCmd, " ds -ipa "+IPAddr+" -ipp "+IPPort+" -lf "+cage.Name)
	} else if runtime.GOOS == "linux" {
		shell = "gnome-terminal" // this only works for gnome based linux desktop env
		clip = append(clip, "--", "/bin/bash", "-c", exe+" ds -ipa "+IPAddr+" -ipp "+IPPort+" -lf off")
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
func StopServer(ts int64) error {
	out([]string{"wrn:sending Server.Shutdown..."})

	err := PtrRPC.Call("Server.Shutdown", []int64{ts}, nil) // if 1st param nil -> gob: cannot encode nil value
	if nil != err {

		//todo: we expect here "wsarecv: An existing connection was forcibly closed by the remote host."

		fmt.Println(err)
	}
	return err
}

// RemoteOut does send the logstring s to the displayServer
// It is replacing emit.Out when trice acts as remote
func RemoteOut(s []string) error {
	return PtrRPC.Call("Server.Out", s, nil)
}

// Connect is called by the client and tries to dial.
// On success PtrRpc is valid afterwards and zhe output is re-directed
func Connect() error {
	var err error
	a := fmt.Sprintf("%s:%s", IPAddr, IPPort)
	out([]string{"sig:dialing ", a, " ..."})
	PtrRPC, err = rpc.Dial("tcp", a)
	if err != nil {
		fmt.Println(err)
		return err
	}
	return out([]string{"sig:...remoteDisplay @ ", a, " connected."})
}

// ScShutdownRemoteDisplayServer starts a client to send shutdown message to display server
func ScShutdownRemoteDisplayServer(ts int64) error {
	var err error
	err = Connect()
	if nil != err {
		return err
	}
	err = StopServer(ts)
	if nil != err {
		return err
	}
	return nil
}
