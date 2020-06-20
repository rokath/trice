// whitebox test
package cmd

import (
	"bufio"
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
	cmd := exec.Command("trice")
	//out, err := cmd.CombinedOutput()
	//exp0 := "ggg"
	//lib.Equals(t, nil, err)
	//lib.Equals(t, exp0, string(out))
	disp.StartServer(cmd.Path)
	//time.Sleep(3 * time.Second)
	stim := "sd\r\n"
	exp := "->127.0.0.1:61497: wsarecv: An existing connection was forcibly closed by the remote host.\n"
	capt := helper(stim)
	act := string(capt[len("-> read tcp 127.0.0.1:xxxxx->127.0.0.1:61497: wsarecv: An existing connection was forcibly closed by the remote host.\nread tcp 127.0.0.1:xxxxx"):]) // must remove from the beginning, because xxxxx changes all the time
	lib.Equals(t, exp, act)
}

func TestShutdownServerWhenNotRunning(t *testing.T) {
	stim := "sd\r\n"
	exp := "-> dial tcp [::1]:61497: connectex: No connection could be made because the target machine actively refused it.\ndial tcp [::1]:61497: connectex: No connection could be made because the target machine actively refused it.\n"
	capt := helper(stim)
	act := string(capt)
	lib.Equals(t, exp, act)
}
