// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package emitter does...
package emitter

import (
	"fmt"
	"log"
	"net/rpc"
	"os/exec"
	"runtime"
	"strings"
	"time"

	"github.com/rokath/trice/internal/global"
)

// lineWriter is the common interface for output devices.
// string slice `line` contains all string parts of one line.
// The last string part is without newline char and must be handled by the output device.
type lineWriter interface {
	writeLine([]string)
}

// lineTranslator imnplements a lineWriter interface.
// It uses an internal lineWriter lw to write to.
// It converts the channel information to color data using colorPalette.
// In case of a remote display the lineTranslator should be used there.
type lineTranslator struct {
	lw           lineWriter
	colorPalette string
}

// newLineTranslatorANSI translates lines to ANSI colors according to colorPalette.
// It provides a lineWriter interface and uses internally a lineWriter.
func newLineTranslatorANSI(lw lineWriter, colorPalette string) *lineTranslator {
	p := &lineTranslator{lw, colorPalette}
	return p
}

// writeLine consumes a full line, translates it and writes it to the internal lineWriter.
func (p *lineTranslator) writeLine(line []string) {
	p.lw.writeLine(line) // TODO: translation
}

type triceLineComposer struct {
	timestampFormat string
	prefix          string
	suffix          string
	line            []string   // line collector
	lw              lineWriter // internal interface
	err             error
}

// NewLineComposer constructs log lines according to these rules:...
// It provides an io.StringWriter interface which is used for the reception of (trice) strings.
// NewLineComposer uses lw for writing the generated lines.
func newLineComposer(lw lineWriter, timestampFormat, prefix, postfix string) *triceLineComposer {
	p := &triceLineComposer{lw, timestampFormat, prefix, postfix, make([]string, 0, 80), nil}
	return p
}

// timestamp returns local time as string according var p.timeStampFormat
func (p *triceLineComposer) timestamp() string {
	var s string
	switch p.timestampFormat {
	case "LOCmicro":
		s = time.Now().Format(time.StampMicro) + "  "
	case "UTCmicro":
		s = "UTC " + time.Now().UTC().Format(time.StampMicro) + "  "
	case "off", "none":
		s = ""
	default:
		s = p.timeStampFormat + " "
	}
	return s
}

// WriteString implements the io.StringWriter interface for triceLineComposer
// If s is empty, WriteString returns 0, nil.
// If p.line is empty, the actual timestamp and the prefix are added to p.line.
// If s is without newline it is added to p.line and WriteString returns.
// If s ends with newline it is added to p.line and also the suffix is added to p.line.
// If s contains several newlines it is split there and the substrings are hanled accordingly.
// That means it writes internally a separate line for each substring in s ending with a newline.
func (p *triceLineComposer) WriteString(s string) (n int, err error) {
	n = len(s)
	sn := strings.ReplaceAll(s, "\r\n", "\n")
	ss := strings.Split(sn, "\n")

	// play ground results:
	// fmt.Printf("%q\n", strings.Split("", "\n"))            // [""]                 // 0 new line
	// fmt.Printf("%q\n", strings.Split("\n", "\n"))          // ["" ""]              // 1 new lne
	// fmt.Printf("%q\n", strings.Split("\n\n", "\n"))        // ["" "" ""]           // 2 new lines
	// fmt.Printf("%q\n", strings.Split("a b c", "\n"))       // ["a b c"]            // 0 new line
	// fmt.Printf("%q\n", strings.Split("a\nb\nc\n", "\n"))   // ["a" "b" "c" ""]     // 3 new lines
	// fmt.Printf("%q\n", strings.Split("a\n\nb\nc\n", "\n")) // ["a" "" "b" "c" ""]  // 4 new lines
	// fmt.Printf("%q\n", strings.Split("a\nb\nc", "\n"))     // ["a" "b" "c"]        // 2 new lines
	// derived rule: len(ss)-1 is count of new lines
	lineEndCount := len(ss) - 1

	// One string with several newlines gets the identical timestamp.
	// If a string was already started and gets completed with a following WriteString call,
	// it keeps its original timestamp, but if following lines inside s they get a new timestamp.
	ts := p.timestamp()
	for _, sx := range ss {
		if 0 == len(p.line) { // start new line
			if 0 < lineEndCount { // and complete line
				p.line = append(p.line, ts, p.prefix, sx, p.suffix)
				p.lw.writeLine(p.line)
				p.line = p.line[:0]
				lineEndCount--
			} else { // and keep it
				p.line = append(p.line, ts, p.prefix, sx)
			}
		} else { // a line was started before
			if 0 < lineEndCount { // now complete line
				p.line = append(p.line, sx, p.suffix)
				p.lw.writeLine(p.line)
				p.line = p.line[:0]
				lineEndCount--
			} else { // extend line
				p.line = append(p.line, sx)
			}
		}
	}
	return
}

// localDisplay is an object used for displaying.
// localDisplay implements the lineWriter interface
type localDisplay struct {
	err error
}

// newLocalDisplay creates a localDisplay. It provides a lineWriter
func newLocalDisplay() *localDisplay {
	p := &localDisplay{}
	return p
}

// writeLine is the implemented lineWriter interface for localDisplay.
func (p *localDisplay) writeLine(line []string) {
	global.Fatal(p.err)
	s := strings.Join(line, "")
	_, p.err = fmt.Println(s)
}

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
