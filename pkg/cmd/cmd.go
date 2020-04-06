// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package cmd is responsibe for interpreting user commanmdline abnd executing commands
package cmd

import (
	"bufio"
	"fmt"
	"os"
	"runtime"
	"strings"

	"github.com/rokath/trice/pkg/disp"
)

// KeyboardInput expects user input from terminal
func KeyboardInput() { // https://tutorialedge.net/golang/reading-console-input-golang/
	reader := bufio.NewReader(os.Stdin)
	fmt.Println("Simple Shell")
	fmt.Println("------------")

	go func() {
		for {
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
			case "exitServer", "serverExit":
				var result int64
				var dummy []int64
				err := disp.PtrRpc.Call("Server.Exit", dummy, &result)
				fmt.Print(err)
			default:
				fmt.Printf("Unknown command '%s' - use 'help'\n", text)
			}
		}
	}() // https://stackoverflow.com/questions/16008604/why-add-after-closure-body-in-golang
}
