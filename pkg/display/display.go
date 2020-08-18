// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package display

import (
	"fmt"
	"log"
	"net/rpc"
	"os"
	"os/exec"
	"runtime"
	"strings"

	"github.com/rokath/trice/internal/global"
)

// io
// type StringWriter interface {
//    WriteString(s string) (n int, err error)
//}

// LineWriter is the common interface for output devices.
// string slice `line` contains all string parts of one line. The last string part is without newline char.
type LineWriter interface {
	WriteLine(line []string)
}

// LocalDisplay is an object used for displaying.
type LocalDisplay struct {
	err          error
	colorPalette string
}

// NewLocal creates a LocalDisplay.
func NewLocal(colorPalette string) *LocalDisplay {
	p := &LocalDisplay{nil, colorPalette}
	return p
}

// WriteLine is implementing the LineWriter interface for LocalDisplay.
func (p *LocalDisplay) WriteLine(line []string) {
	if nil != p.err {
		log.Fatal(p.err)
	}
	_, p.err = fmt.Println(strings.Join(line, ""))
}

// RemoteDisplay is transferring to a remote display object.
type RemoteDisplay struct {
	err     error
	ipa     string      // IP addr
	ipp     string      // IP port
	exe     string      // remote server executable
	logFile string      // remote server logfile
	PtrRPC  *rpc.Client // PtrRPC is a pointer for remote calls valid after a succesful rpc.Dial()
}

// NewRemote creates a connection to a remote Display. It accepts 0 to 4 string arguments. More arguments arfe ignored.
// For not given parameters default values are taken. The parameters are in the following order.
// args[0] (ipa) is the IP address to be used to connect to the remote display.
// args[1] (ipp) is the IP port to be used to connect to the remote display.
// args[2] (exe), is a programm started to create a remote server instance if not already running.
// If the remote server is already running on ips:ipp than a start of a 2nd instace is not is possible. This is silently ignored.
// args[3] (logFile) ist the filename the remote server should use. This value is used only if the remote server gets started.
func NewRemote(args ...string) *RemoteDisplay {
	args = append(args, "", "", "", "") // make sure to have at least 4 elements in ars
	p := &RemoteDisplay{
		err:     nil,
		ipa:     "localhost",
		ipp:     "65497",
		exe:     os.Args[0],
		logFile: "none",
		PtrRPC:  nil,
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
		p.logFile = args[3]
	}
	if "" != p.exe {
		p.startServer()
	}
	p.err = p.connect()
	return p
}

// WriteLine is implementing the LineWriter interface for RemoteDisplay.
func (p *RemoteDisplay) WriteLine(line []string) {
	if nil != p.err {
		log.Fatal(p.err)
	}
	p.err = p.PtrRPC.Call("Server.Out", line, nil) // TODO: Change to "Server.WriteLine"
}

// startServer starts a display server with the filename exe (if not already running)
func (p *RemoteDisplay) startServer() {
	var shell string
	var clip []string
	if runtime.GOOS == "windows" {
		shell = "cmd"
		shellCmd := "/c start " + p.exe
		clip = append(clip, shellCmd, " ds -ipa "+p.ipa+" -ipp "+p.ipp+" -lg "+p.logFile)
	} else if runtime.GOOS == "linux" {
		shell = "gnome-terminal" // this only works for gnome based linux desktop env
		clip = append(clip, "--", "/bin/bash", "-c", p.exe+" ds -ipa "+p.ipa+" -ipp "+p.ipp+" -lg "+p.logFile)
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
	addr := p.ipa + ":" + p.ipp
	if global.Verbose {
		p.LineWrite([]string{"sig:dialing ", addr, " ..."})
	}
	p.PtrRPC, err = rpc.Dial("tcp", addr)
	if err != nil {
		fmt.Println(err)
		return err
	}
	if global.Verbose {
		p.LineWrite([]string{"sig:...remoteDisplay @ ", addr, " connected."})
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
	p.LineWrite([]string{"wrn:sending Server.Shutdown..."})

	p.err = p.PtrRPC.Call("Server.Shutdown", []int64{ts}, nil) // if 1st param nil -> gob: cannot encode nil value
	if nil != p.err {

		//todo: we expect here "wsarecv: An existing connection was forcibly closed by the remote host."

		fmt.Println(p.err)
	}
	return p.err
}
