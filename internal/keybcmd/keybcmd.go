// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package keybcmd is responsible for interpreting user commandline and executing commands
package keybcmd

import (
	"bufio"
	"fmt"
	"os"
	"strings"

	"github.com/rokath/trice/internal/emitter"
)

func loopAction(reader *bufio.Reader, ipa, ipp string) {
	fmt.Print("-> ")
	text, _ := reader.ReadString('\n')
	text = strings.Replace(text, "\r", "", -1) // Windows "\r\n" !
	text = strings.Replace(text, "\n", "", -1) // Linux "\n" !

	switch text {
	case "q", "quit":
		os.Exit(0)
	case "sd", "shutdown":
		err := emitter.ScShutdownRemoteDisplayServer(os.Stdout, 1, ipa, ipp)
		if nil != err {
			fmt.Println(err)
		}
	case "h", "help":
		fmt.Println("h|help      - this text")
		fmt.Println("sd|shutdown - kill trice display server (ends also this trice console)")
		fmt.Println("q|quit      - end trice console (keeps the trice display server active)")
		fmt.Println("Other commands are written to target.")
	default:
		fmt.Printf("Command '%s'\n", text)
	}
}

// ReadInput expects user input from terminal
func ReadInput() { // https://tutorialedge.net/golang/reading-console-input-golang/
	reader := bufio.NewReader(os.Stdin)
	keyboardInput(reader)
}

// keyboardInput expects input from reader
func keyboardInput(reader *bufio.Reader) { // https://tutorialedge.net/golang/reading-console-input-golang/
	fmt.Println("Simple Shell (try 'help'):")
	fmt.Println("--------------------------")

	go func() {
		for {
			loopAction(reader, emitter.IPAddr, emitter.IPPort)
		}
	}() // https://stackoverflow.com/questions/16008604/why-add-after-closure-body-in-golang
}
