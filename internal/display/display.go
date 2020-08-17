// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package display

import (
	"fmt"
	"log"
	"net/rpc"
	"os/exec"
	"runtime"
	"strings"

	"github.com/rokath/trice/internal/global"
)

// LineWriter is the common interface for output devices.
// string slice `line` contains all string parts of one line. The last string part is without newline char.
type LineWriter interface {
	LineWrite(line []string)
}

// LocalDisplay is an object used for displaying.
// It has no properties.
type LocalDisplay struct {
	err error
}

// LineWrite is implementing the LineWriter interface for LocalDisplay.
func (p *LocalDisplay) LineWrite(line []string) {
	if nil == p.err {
		_, p.err = fmt.Println(strings.Join(line, ""))
	}
}

// NewLocalDisplay creates a Display with `properties`.
func NewLocalDisplay(properies string) *LocalDisplay {
	p := &LocalDisplay{}
	return p
}

// RemoteDisplay is transferring to a remote display object.
type RemoteDisplay struct {
	err                    error
	exe, logFile, ipAddr string
	// PtrRPC is a pointer for remote calls valid after a succesful rpc.Dial()
	PtrRPC *rpc.Client
}

// NewRemoteDisplay creates a connection to a remote Display.
// `exe`, if not empty, ist a programm started to create a remote server instance if not already running.
// If the remote server is already running or on an other machine use the empty string "" as parameter.
// If the remote server is already running on ips:ipp and exe is not "", than a start of a 2nd instace is not is possible.
// `logFile` ist the filename the remote server should use. This value is used only if the remote server gets started.
// ipAddr is in "ipa:ipp" format, like "127.0.0.1:23456".
// `ipa` is the IP address to be used to connect to the remote display.
// `ipp` is the IP port to be used to connect to the remote display.
func NewRemoteDisplay(exe, logFile, ipAddr string) (*RemoteDisplay, error) {
	p := &RemoteDisplay{nil, exe, logFile, ipAddr, nil}
	if "" != p.exe {
		p.startServer()
	}
	p.err := p.connect()
	return p, p.err
}

// LineWrite is implementing the LineWriter interface for RemoteDisplay.
func (p *RemoteDisplay) LineWrite(line []string) {
	if nil != p.err {
		p.err = p.PtrRPC.Call("Server.Out", line, nil) // TODO: Change to "Server.WriteLine"
	}
}

// startServer starts a display server with the filename exe (if not already running)
func (p *RemoteDisplay) startServer() {
	var shell string
	var clip []string
	if runtime.GOOS == "windows" {
		shell = "cmd"
		shellCmd := "/c start " + p.exe
		//clip = append(clip, shellCmd, " ds -ip "+IPAddr+" -lg "+cage.Name)
		clip = append(clip, shellCmd, " ds -ip "+p.ipAddr+" -lg "+p.logFile)
	} else if runtime.GOOS == "linux" {
		shell = "gnome-terminal" // this only works for gnome based linux desktop env
		//clip = append(clip, "--", "/bin/bash", "-c", exe+" ds -ipa "+IPAddr+" -ipp "+IPPort+" -lg off")
		clip = append(clip, "--", "/bin/bash", "-c", p.exe+" ds -ip "+p.ipAddr+" -lg "+p.logFile)
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

// connect is called by the client and tries to dial.
// On success PtrRpc is valid afterwards and the output is re-directed
func (p *RemoteDisplay) connect() error {
	var err error
	addr := p.ipa+":"+p.ipp)
	if global.Verbose {
		p.WriteLine([]string{"sig:dialing ", addr, " ..."})
	}
	p.PtrRPC, err = rpc.Dial("tcp", a)
	if err != nil {
		fmt.Println(err)
		return err
	}
	if global.Verbose {
		p.WriteLine([]string{"sig:...remoteDisplay @ ", a, " connected."})
	}
	return err
}

// ScShutdownRemoteDisplayServer starts a client to send shutdown message to display server.
func (p *RemoteDisplay) ScShutdownRemoteDisplayServer(ts int64) error {
	if nil != p.err {
		return p.err
	}
	p.err = p.connect()
	if nil != p.err {
		return p.err
	}
	p.err = p.StopServer(ts)
	if nil != p.err {
		return p.err
	}
	return nil
}

// StopServer sends signal to display server to quit.
// `ts` is used as flag. If 1 shutdown message is with timestamp (default usage), if 0 shutdown message is without timestamp (for testing).
func (p *RemoteDisplay) StopServer(ts int64) error {
	p.WriteLine([]string{"wrn:sending Server.Shutdown..."})

	p.err = p.PtrRPC.Call("Server.Shutdown", []int64{ts}, nil) // if 1st param nil -> gob: cannot encode nil value
	if nil != p.err {

		//todo: we expect here "wsarecv: An existing connection was forcibly closed by the remote host."

		fmt.Println(p.err)
	}
	return p.err
}
