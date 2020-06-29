// whitebox test
package cmd

import (
	"bufio"
	"fmt"
	"io/ioutil"
	"os"
	"os/exec"
	"strings"
	"testing"

	"github.com/rokath/trice/internal/disp"
	"github.com/rokath/trice/pkg/lib"
)

// Test is just to have a test
func TestDummy(t *testing.T) {
	lib.Ok(t, nil)
}

// helper stimulates with keys the loop action and returns the captured output as byte slice.
func helper(keys string) []byte {

	// simulate input
	ioReader := strings.NewReader(keys)
	bufioReader := bufio.NewReader(ioReader)

	// capture output
	rescueStdout := os.Stdout
	r, w, _ := os.Pipe()
	os.Stdout = w

	loopAction(bufioReader)

	// restore
	w.Close()
	captured, _ := ioutil.ReadAll(r)
	os.Stdout = rescueStdout

	return captured
}

func TestHelp(t *testing.T) {
	stim := "help\r\n"
	exp := "-> h|help                   - this text\nexitServer|serverExit    - kill server\nq|quit                   - end program\n"
	cap := helper(stim)
	act := string(cap)
	lib.Equals(t, exp, act)
}

func TestShutdownServerWhenRunning(t *testing.T) {
	disp.IPPort = "61496" // need a tests unique port because parallel test execution
	cmd := exec.Command("trice")

	disp.StartServer(cmd.Path)

	stim := "sd\r\n"
	exp := "->127.0.0.1:" + disp.IPPort + ": wsarecv: An existing connection was forcibly closed by the remote host.\n"
	capt := helper(stim)

	// must remove some chars from the beginning, because xxxxx changes all the time
	discardLength := len("-> read tcp 127.0.0.1:xxxxx->127.0.0.1:" + disp.IPPort + ": wsarecv: An existing connection was forcibly closed by the remote host.\nread tcp 127.0.0.1:xxxxx")
	if discardLength >= len(capt) {
		capts := string(capt)
		fmt.Println(capts)
		t.Fail()
		return
	}
	act := string(capt[discardLength:])
	lib.Equals(t, exp, act)
}

func TestShutdownServerWhenNotRunning(t *testing.T) {
	disp.IPPort = "61495" // need a tests unique port because parallel test execution
	stim := "sd\r\n"
	exp := "-> dial tcp [::1]:" + disp.IPPort + ": connectex: No connection could be made because the target machine actively refused it.\ndial tcp [::1]:" + disp.IPPort + ": connectex: No connection could be made because the target machine actively refused it.\n"
	capt := helper(stim)
	act := string(capt)
	lib.Equals(t, exp, act)
}
