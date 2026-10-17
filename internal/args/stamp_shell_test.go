// SPDX-License-Identifier: MIT

package args

import (
	"encoding/json"
	"fmt"
	"os"
	"os/exec"
	"path/filepath"
	"regexp"
	"strings"
	"testing"

	"github.com/rokath/trice/internal/printf"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// TestStampShellArgvHelper reports the native arguments received from a real shell.
// The environment guard keeps this subprocess helper inert during normal tests.
func TestStampShellArgvHelper(t *testing.T) {
	if os.Getenv("TRICE_STAMP_ARGV_HELPER") != "1" {
		return
	}
	for i, arg := range os.Args {
		if arg == "--" {
			value, err := json.Marshal(os.Args[i+1:])
			require.NoError(t, err)
			fmt.Println("STAMPARGV:" + string(value))
			return
		}
	}
	t.Fatal("missing helper argument separator")
}

// TestStampShellQuoting runs installed shells against the documented forms.
// Windows-specific cases run on Windows hosts; absent shells are explicit skips.
func TestStampShellQuoting(t *testing.T) {
	binary, err := os.Executable()
	require.NoError(t, err)
	const expression = `printf("msg:%12.3f us",unwrap(47999-ts,48000)/48)`
	tests := []struct {
		name, shell string
		args        []string
		batch       bool
	}{
		{"Bash and Git Bash", "bash", []string{"-c", `"$TRICE_STAMP_TEST_BINARY" -test.run=TestStampShellArgvHelper -- -ts16 'printf("msg:%12.3f us",unwrap(47999-ts,48000)/48)'`}, false},
		{"PowerShell standard", "pwsh", []string{"-NoProfile", "-Command", `if ($PSVersionTable.PSVersion -lt [version]'7.3') { exit 77 }; $PSNativeCommandArgumentPassing='Standard'; & $env:TRICE_STAMP_TEST_BINARY -test.run=TestStampShellArgvHelper -- -ts16 'printf("msg:%12.3f us",unwrap(47999-ts,48000)/48)'`}, false},
		{"PowerShell legacy", "powershell.exe", []string{"-NoProfile", "-Command", `& $env:TRICE_STAMP_TEST_BINARY --% -test.run=TestStampShellArgvHelper -- -ts16 "printf(\"msg:%12.3f us\",unwrap(47999-ts,48000)/48)"`}, false},
		{"cmd interactive", "cmd.exe", []string{"/D", "/Q"}, false},
		{"cmd batch percent escaping", "cmd.exe", nil, true},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			shell, err := exec.LookPath(tt.shell)
			if err != nil {
				t.Skipf("%s is not installed", tt.shell)
			}
			args := tt.args
			var input string // stdin avoids Go's argv quoting being reinterpreted by cmd /C.
			if tt.shell == "cmd.exe" {
				input = `"%TRICE_STAMP_TEST_BINARY%" -test.run=TestStampShellArgvHelper -- -ts16 "printf(\"msg:%12.3f us\",unwrap(47999-ts,48000)/48)"` + "\r\nexit\r\n"
			}
			if tt.batch {
				file := filepath.Join(t.TempDir(), "stamp.cmd")
				script := "@echo off\r\n" + `"%TRICE_STAMP_TEST_BINARY%" -test.run=TestStampShellArgvHelper -- -ts16 "printf(\"msg:%%12.3f us\",unwrap(47999-ts,48000)/48)"` + "\r\n"
				require.NoError(t, os.WriteFile(file, []byte(script), 0600))
				args = []string{"/D", "/Q"}
				input = `"` + file + `"` + "\r\nexit\r\n"
			}
			command := exec.Command(shell, args...)
			if input != "" {
				command.Stdin = strings.NewReader(input)
			}
			command.Env = append(os.Environ(), "TRICE_STAMP_ARGV_HELPER=1", "TRICE_STAMP_TEST_BINARY="+binary)
			output, err := command.CombinedOutput()
			if exit, ok := err.(*exec.ExitError); ok && tt.shell == "pwsh" && exit.ExitCode() == 77 {
				t.Skip("standard native argument passing requires PowerShell 7.3 or later")
			}
			require.NoError(t, err, "%s", output)
			for _, line := range strings.Split(string(output), "\n") {
				if _, value, ok := strings.Cut(line, "STAMPARGV:"); ok {
					var actual []string
					require.NoError(t, json.Unmarshal([]byte(value), &actual))
					assert.Equal(t, []string{"-ts16", expression}, actual)
					return
				}
			}
			t.Fatalf("shell did not report its arguments: %s", output)
		})
	}
}

// TestPublishedStampExamplesCompile prevents manual/help examples drifting from
// the accepted expression language without depending on ports or a target.
func TestPublishedStampExamplesCompile(t *testing.T) {
	argument := regexp.MustCompile(`-ts(16|32)(delta)?\s+'(printf\([^'\n]*\))'`)
	for _, path := range []string{"../../docs/rm/stamp_expressions.md", "../../docs/ref/trice-help-all.txt"} {
		data, err := os.ReadFile(path)
		require.NoError(t, err)
		examples := argument.FindAllStringSubmatch(string(data), -1)
		require.NotEmpty(t, examples, path)
		for _, example := range examples {
			bits := 16
			if example[1] == "32" {
				bits = 32
			}
			_, err := printf.ParseStamp(example[3], bits, example[2] == "delta")
			assert.NoError(t, err, "%s: %s", path, example[0])
		}
	}
}
