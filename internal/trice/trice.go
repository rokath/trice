// Copyright 2020 basti@blackoutcloud.de
//                Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package trice provides subcommands ScLog and ScReceive
// It provides optional decryption and activates receiver package,
// which in turn calls emit.Trice on every received trice.
package trice

import (
	"github.com/rokath/trice/internal/disp"
	"github.com/rokath/trice/internal/receiver"
)

/*
// ScLog is the subcommand log and connects to COM port and displays traces
func ScLog() error {
	cage.Enable()
	defer cage.Disable()

	return DoReceive()
}*/

// Connect starts a display server sy if sv is not empty, otherwise it assumes a running display server.
//
// It connects then to the running display server.
func Connect(sv string) error {
	if "" != sv {
		disp.StartServer(sv)
	}

	err := disp.Connect()
	disp.Out = disp.RemoteOut // re-direct output
	if nil != err {
		return err
	}

	disp.PtrRPC.Call("Server.Out", []string{""}, nil)
	disp.PtrRPC.Call("Server.Out", []string{""}, nil)
	disp.PtrRPC.Call("Server.Out", []string{""}, nil)
	disp.PtrRPC.Call("Server.Out", []string{"att:new connection from ", "read:" + receiver.Source, "..."}, nil)
	disp.PtrRPC.Call("Server.Out", []string{""}, nil)
	disp.PtrRPC.Call("Server.Out", []string{""}, nil)
	return nil
}

/*
// ScReceive is the subcommand remoteDisplay and acts as client connecting to the displayServer
// sv is the executable name to be started as remote display server (typically arg[0] == trice)
func ScReceive(sv string) error {
	err := NewConnection(sv)
	if err != nil {
		fmt.Println(err)
		return err
	}
	cmd.KeyboardInput()
	DoReceive() // does not return
	return nil
}
*/

// DoReceive connects to COM port and displays traces

/* DoReceive connects to COM port and displays traces
func DoReceive() error {

	SetUp()
	do()

	return nil
}

*/
