// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package do

import (
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"net"
	"os"
	"path/filepath"
	"time"

	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/internal/link"
	"github.com/rokath/trice/internal/receiver"
	"github.com/rokath/trice/internal/translator"
	"github.com/rokath/trice/pkg/msg"
	"github.com/spf13/afero"
)

var (
	TCPOutAddr = ""
)

// distributeArgs is distributing values used in several packages.
// It must not be called before the appropriate arg parsing.
func DistributeArgs(w io.Writer, fSys *afero.Afero, logfileName string, verbose bool) io.Writer {

	id.Verbose = verbose
	link.Verbose = verbose
	decoder.Verbose = verbose
	emitter.Verbose = verbose
	receiver.Verbose = verbose
	translator.Verbose = verbose
	emitter.TestTableMode = decoder.TestTableMode

	if id.DefaultStampSize == 32 {
		id.StampSizeId = " ID(0),"
	} else if id.DefaultStampSize == 16 {
		id.StampSizeId = " Id(0),"
	} else {
		id.StampSizeId = " id(0),"
	}

	w = triceOutput(w, fSys, logfileName, verbose)
	evaluateColorPalette(w)
	return w
}

// triceOutput returns w as a a optional combined io.Writer. If fileName is given the returned io.Writer write a copy into the given file.
func triceOutput(w io.Writer, fSys *afero.Afero, fileName string, verbose bool) io.Writer {
	ioWriter := tcpWriter()

	// start logging only if fn not "none" or "off"
	if fileName == "none" || fileName == "off" {
		if verbose {
			fmt.Println("No logfile writing...")
		}
		return io.MultiWriter(w, ioWriter)
	}

	if filepath.Base(fileName) == "auto" { // "2006-01-02_1504-05_trice.log" is the pattern for default logfile name. The timestamp is replaced with the actual time.
		fileName = filepath.Join(filepath.Dir(fileName), time.Now().Format("2006-01-02_1504-05_trice.log")) // Replace timestamp in default log filename.
	}

	if verbose {
		fmt.Println("Logfile is:", fileName)
	}

	lfHandle, err := fSys.OpenFile(fileName, os.O_RDWR|os.O_CREATE|os.O_APPEND, 0666)
	msg.FatalOnErr(err)
	if verbose {
		fmt.Printf("Writing to logfile %s...\n", fileName)
	}

	return io.MultiWriter(w, ioWriter, lfHandle)
}

// evaluateColorPalette
func evaluateColorPalette(w io.Writer) {
	switch emitter.ColorPalette {
	case "off", "none", "default", "color":
		return
	default:
		fmt.Fprintln(w, "Ignoring unknown -color", emitter.ColorPalette, "using default.")
		emitter.ColorPalette = "default"
	}
}

func tcpWriter() io.Writer {
	if TCPOutAddr == "" {
		return ioutil.Discard
	}
	// The net.Listen() function makes the program a TCP server. This functions returns a Listener variable, which is a generic network listener for stream-oriented protocols.
	fmt.Println("Listening on " + TCPOutAddr + "...")
	listen, err := net.Listen("tcp", TCPOutAddr)
	if err != nil {
		log.Fatal(err)
	}
	defer listen.Close()

	// t is only after a successful call to Accept() that the TCP server can begin to interact with TCP clients.
	TCPConn, err := listen.Accept()
	fmt.Println("Accepting connection:", TCPConn)
	if err != nil {
		log.Fatal(err)
	}
	// Make a buffer to hold incoming data.
	buf := make([]byte, 1024)
	reqLen, err := TCPConn.Read(buf)
	if err != nil {
		fmt.Println("Error reading:", err.Error())
	}
	fmt.Println(string(buf[:reqLen]))
	TCPConn.Write([]byte("Trice connected...\r\n"))

	//defer TCPConn.Close()
	return TCPConn

}
