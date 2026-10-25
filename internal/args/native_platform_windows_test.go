// SPDX-License-Identifier: MIT

package args

import (
	"os"
	"os/exec"
	"path/filepath"
	"syscall"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// TestNativeLockedSourceRollsBack uses a real Windows sharing lock. A failed
// source update must not publish mismatched dictionaries, and retrying after
// releasing the lock must succeed without repairing user files manually.
func TestNativeLockedSourceRollsBack(t *testing.T) {
	if os.Getenv("TRICE_BIND_INTEGRATION") != "1" {
		t.Skip("set TRICE_BIND_INTEGRATION=1 for native Windows sharing-lock integration")
	}
	root, err := filepath.Abs("../..")
	require.NoError(t, err)
	project := t.TempDir()
	binary := filepath.Join(project, "trice.exe")
	build := exec.Command("go", "build", "-o", binary, "./cmd/trice")
	build.Dir = root
	output, err := build.CombinedOutput()
	require.NoError(t, err, "%s", output)
	initial := map[string][]byte{
		"main.c":   []byte("#include \"trice.h\"\r\nvoid example(void) { trice(\"info:locked\"); }\r\n"),
		"til.json": []byte("{}\n"),
		"li.json":  []byte("{}\n"),
	}
	for name, data := range initial {
		require.NoError(t, os.WriteFile(filepath.Join(project, name), data, 0o644))
	}
	path, err := syscall.UTF16PtrFromString(filepath.Join(project, "main.c"))
	require.NoError(t, err)
	handle, err := syscall.CreateFile(path, syscall.GENERIC_READ, syscall.FILE_SHARE_READ, nil,
		syscall.OPEN_EXISTING, syscall.FILE_ATTRIBUTE_NORMAL, 0)
	require.NoError(t, err)
	defer func() {
		if handle != syscall.InvalidHandle {
			_ = syscall.CloseHandle(handle)
		}
	}()
	arguments := []string{"bind", "-src", "main.c", "-til", "til.json", "-li", "li.json", "-genDir", "generated"}
	command := exec.Command(binary, arguments...)
	command.Dir = project
	output, err = command.CombinedOutput()
	require.Error(t, err, "locked source must not be reported as bound: %s", output)
	assert.Contains(t, string(output), "main.c")
	for name, want := range initial {
		got, readErr := os.ReadFile(filepath.Join(project, name))
		require.NoError(t, readErr)
		assert.Equal(t, want, got, "failed binding preserves "+name)
	}
	require.NoError(t, syscall.CloseHandle(handle))
	handle = syscall.InvalidHandle
	command = exec.Command(binary, arguments...)
	command.Dir = project
	output, err = command.CombinedOutput()
	require.NoError(t, err, "retry after releasing the Windows lock: %s", output)
}
