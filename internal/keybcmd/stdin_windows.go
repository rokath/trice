// SPDX-License-Identifier: MIT

package keybcmd

import (
	"os"

	"golang.org/x/sys/windows"
)

// openKeyboardSource borrows stdin through an owned duplicate. CancelIoEx
// interrupts console and pipe reads without closing the caller's handle.
func openKeyboardSource(stdin *os.File) (keyboardSource, error) {
	// SyscallConn rejects closed files and preserves the caller's poller.
	// Fd would detach a pipe from that poller as an unwanted side effect.
	raw, err := stdin.SyscallConn()
	if err != nil {
		return keyboardSource{}, err
	}
	var handle windows.Handle
	var duplicateErr error
	process := windows.CurrentProcess()
	if err := raw.Control(func(borrowed uintptr) {
		duplicateErr = windows.DuplicateHandle(process, windows.Handle(borrowed), process, &handle, 0, false, windows.DUPLICATE_SAME_ACCESS)
	}); err != nil {
		return keyboardSource{}, err
	}
	if duplicateErr != nil {
		return keyboardSource{}, duplicateErr
	}
	input := os.NewFile(uintptr(handle), stdin.Name())
	return keyboardSource{
		Reader: input,
		interrupt: func() {
			// ERROR_NOT_FOUND also means the next read has not started yet;
			// the owner repeats cancellation until the shell is joined.
			_ = windows.CancelIoEx(handle, nil)
		},
		release: input.Close,
	}, nil
}
