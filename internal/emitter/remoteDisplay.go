// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package emitter

import (
	"fmt"
	"log"
	"net/rpc"
	"os/exec"
	"runtime"

	"github.com/rokath/trice/internal/global"
)

// remoteDisplay is transferring to a remote display object.
type remoteDisplay struct {
	err    error
	ipa    string      // IP addr
	ipp    string      // IP port
	exe    string      // remote server executable
	params string      // remote server additional parameters (despite "ds -ipa a.b.c.d -ipp nnnn")
	PtrRPC *rpc.Client // PtrRPC is a pointer for remote calls valid after a succesful rpc.Dial()
}

// newRemoteDisplay creates a connection to a remote Display.
// It accepts 0 to 4 string arguments. More arguments are ignored.
// For not given parameters default values are taken. The parameters are in the following order.
// args[0] (ipa) is the IP address to be used to connect to the remote display.
// args[1] (ipp) is the IP port to be used to connect to the remote display.
// args[2] (exe), is a programm started to create a remote server instance if not already running.
// If the remote server is already running on ips:ipp than a start of a 2nd instace is not is possible. This is silently ignored.
// args[3] (params) contains additional remote display (=trice) command parameters.
// This value is used only if the remote server gets started.
func newRemoteDisplay(args ...string) *remoteDisplay {
	args = append(args, "", "", "", "") // make sure to have at least 4 elements in args.
	p := &remoteDisplay{
		err:    nil,
		ipa:    "localhost",
		ipp:    "65497",
		exe:    "",        // per default assume remote display is already active.
		params: "-lg off", // no logfile as default when remote display is launched.
		PtrRPC: nil,
	}
	if "" != args[0] {
		p.ipa = args[0]
	}
	if "" != args[1] {
		p.ipp = args[1]
	}
	if "" != args[2] {
		p.exe = args[2]
	}
	if "" != args[3] {
		p.params = args[3]
	}
	if "" != p.exe {
		p.startServer()
	}
	p.connect()
	return p
}

// writeLine is implementing the lineWriter interface for RemoteDisplay.
func (p *remoteDisplay) writeLine(line []string) {
	global.Check(p.err)
	p.err = p.PtrRPC.Call("Server.Out", line, nil) // TODO: Change to "Server.WriteLine"
}

// startServer starts a display server with the filename exe (if not already running)
func (p *remoteDisplay) startServer() {
	var shell string
	var clip []string
	arguments := " ds -ipa " + p.ipa + " -ipp " + p.ipp + " " + p.params
	if runtime.GOOS == "windows" {
		shell = "cmd"
		shellCmd := "/c start " + p.exe
		clip = append(clip, shellCmd, arguments)
	} else if runtime.GOOS == "linux" {
		shell = "gnome-terminal" // this only works for gnome based linux desktop env
		clip = append(clip, "--", "/bin/bash", "-c", p.exe+arguments)
	} else {
		//log.Fatal("trice is running on unknown operating system")
		global.Check(p.err)
	}
	cmd := exec.Command(shell, clip...)

	err := cmd.Run()
	if err != nil {
		log.Println(clip)
		global.Check(err)
	}
}

// connect is called by the client and tries to dial.
// On success PtrRpc is valid afterwards and the output is re-directed.
// Otherwise an error code is stored inside remotDisplay.
func (p *remoteDisplay) connect() {
	addr := p.ipa + ":" + p.ipp
	if global.Verbose {
		fmt.Println("dialing " + addr + " ...")
	}
	p.PtrRPC, p.err = rpc.Dial("tcp", addr)
	if global.Verbose {
		fmt.Println("...remoteDisplay @ " + addr + " connected.")
	}
}

// ScShutdownRemoteDisplayServer connects to a client to send shutdown message to display server.
// 0==timeStamp -> no timestamp in shutdown message, use for tests.
// 1==timeStamp -> timestamp in shutdown message, use normally.
// It accepts 0 to 2 string arguments as args. More arguments are ignored.
// For not given parameters default values are taken. The parameters are in the following order.
// args[0] (ipa) is the IP address to be used to connect to the remote display.
// args[1] (ipp) is the IP port to be used to connect to the remote display.
func ScShutdownRemoteDisplayServer(timeStamp int64, args ...string) error {
	args = append(args, "", "") // make sure to have at least 2 elements in args.
	p := newRemoteDisplay(args[0], args[1])
	p.connect()
	p.stopServer(timeStamp)
	return p.err
}

// StopServer sends signal to display server to quit.
// `ts` is used as flag. If 1 shutdown message is with timestamp (default usage), if 0 shutdown message is without timestamp (for testing).
func (p *remoteDisplay) stopServer(ts int64) {
	if global.Verbose {
		fmt.Println("sending Server.Shutdown...")
	}
	p.err = p.PtrRPC.Call("Server.Shutdown", []int64{ts}, nil) // if 1st param nil -> gob: cannot encode nil value
}
