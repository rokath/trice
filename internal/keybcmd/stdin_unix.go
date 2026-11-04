// SPDX-License-Identifier: MIT

//go:build !windows

package keybcmd

import (
	"errors"
	"os"
	"sync"

	"golang.org/x/sys/unix"
)

// openKeyboardSource registers a nonblocking duplicate with Go's poller so
// Close wakes pending terminal/pipe reads. The borrowed descriptor's shared
// status flags are restored only after the shell has stopped reading.
func openKeyboardSource(stdin *os.File) (keyboardSource, error) {
	// Avoid Fd, which switches an existing pollable pipe to blocking mode.
	raw, err := stdin.SyscallConn()
	if err != nil {
		return keyboardSource{}, err
	}
	var fd int
	var duplicateErr error
	if err := raw.Control(func(borrowed uintptr) {
		fd, duplicateErr = unix.Dup(int(borrowed))
	}); err != nil {
		return keyboardSource{}, err
	}
	if duplicateErr != nil {
		return keyboardSource{}, duplicateErr
	}
	unix.CloseOnExec(fd)
	flags, err := unix.FcntlInt(uintptr(fd), unix.F_GETFL, 0)
	if err != nil {
		_ = unix.Close(fd)
		return keyboardSource{}, err
	}
	if err := unix.SetNonblock(fd, true); err != nil {
		_ = unix.Close(fd)
		return keyboardSource{}, err
	}
	input := os.NewFile(uintptr(fd), stdin.Name())
	var once sync.Once // Do not close twice after the OS reuses the descriptor.
	var closeErr error
	return keyboardSource{
		Reader: input,
		interrupt: func() {
			once.Do(func() { closeErr = input.Close() })
		},
		release: func() error {
			once.Do(func() { closeErr = input.Close() })
			var restoreErr error
			controlErr := raw.Control(func(borrowed uintptr) {
				_, restoreErr = unix.FcntlInt(borrowed, unix.F_SETFL, flags)
			})
			return errors.Join(closeErr, controlErr, restoreErr)
		},
	}, nil
}
