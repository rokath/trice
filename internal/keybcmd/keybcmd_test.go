// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// whitebox test
package keybcmd

import (
	"bufio"
	"fmt"
	"io/ioutil"
	"os"
	"strings"
	"testing"

	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/pkg/lib"
)

// stimulate injects keys to the loop action and returns the captured output as byte slice.
func stimulate(keys, ipa, ipp string) []byte {

	// simulate input
	ioReader := strings.NewReader(keys)
	bufioReader := bufio.NewReader(ioReader)

	// capture output
	rescueStdout := os.Stdout
	r, w, _ := os.Pipe()
	os.Stdout = w

	loopAction(bufioReader, ipa, ipp)

	// restore
	w.Close()
	captured, _ := ioutil.ReadAll(r)
	os.Stdout = rescueStdout

	return captured
}

// todo: Linux

func TestHelp(t *testing.T) {
	exp := "-> h|help                   - this text\nexitServer|serverExit    - kill server\nq|quit                   - end program\n"
	cap := stimulate("help\r\n", "", "")
	act := string(cap)
	lib.Equals(t, exp, act)
}

// NOT RELIABLE
func TestShutdownServerWhenRunning(t *testing.T) {
	ipp := randomDynIPPort()
	p := emitter.NewRemoteDisplay("trice.exe", "", "localhost", ipp)
	capt := stimulate("sd\r\n", p.IPAddr, p.IPPort)
	// must remove some chars from the beginning, because xxxxx changes all the time
	discardLength := len("-> read tcp 127.0.0.1:xxxxx->")
	if discardLength >= len(capt) {
		capts := string(capt)
		fmt.Println(capts)
		t.Fail()
	}
	exp := "127.0.0.1:" + p.IPPort + ": wsarecv: An existing connection was forcibly closed by the remote host.\n"
	act := string(capt[discardLength:])
	notEqualFail(t, exp, act)
}
