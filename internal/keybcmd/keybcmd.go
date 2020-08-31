// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package keybcmd is responsibe for interpreting user commanmdline and executing commands
package keybcmd

import (
	"bufio"
	"fmt"
	"os"
	"runtime"
	"strings"

	"github.com/rokath/trice/internal/emitter"
)

// ReadInput expects user input from terminal
func ReadInput() { // https://tutorialedge.net/golang/reading-console-input-golang/
	reader := bufio.NewReader(os.Stdin)
	keyboardInput(reader)
}

// keyboardInput expects input from reader
func keyboardInput(reader *bufio.Reader) { // https://tutorialedge.net/golang/reading-console-input-golang/
	fmt.Println("Simple Shell")
	fmt.Println("------------")

	go func() {
		for {
			loopAction(reader, emitter.IPAddr, emitter.IPPort)
		}
	}() // https://stackoverflow.com/questions/16008604/why-add-after-closure-body-in-golang
}

func loopAction(reader *bufio.Reader, ipa, ipp string) {
	fmt.Print("-> ")
	text, _ := reader.ReadString('\n')
	// convert CRLF to LF
	e := "\n"
	if runtime.GOOS == "windows" {
		e = "\r\n"
	}
	text = strings.Replace(text, e, "", -1) // Linux "\n" !

	switch text {
	case "q", "quit":
		os.Exit(0)
	case "h", "help":
		fmt.Println("h|help                   - this text")
		fmt.Println("exitServer|serverExit    - kill server")
		fmt.Println("q|quit                   - end program")
	case "sd", "stopServer", "serverStop":
		err := emitter.ScShutdownRemoteDisplayServer(1, ipa, ipp)
		if nil != err {
			fmt.Println(err)
		}
	default:
		fmt.Printf("Unknown command '%s' - use 'help'\n", text)
	}
}
