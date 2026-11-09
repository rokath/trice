// SPDX-License-Identifier: MIT

package keybcmd

import (
	"os"
	"runtime"
	"sync"
	"time"
	"unsafe"

	"golang.org/x/sys/windows"
)

// cancelSynchronousKeyboardIO is absent from x/sys/windows. Load it from the
// system directory so synchronous stdin reads can be canceled by thread.
var cancelSynchronousKeyboardIO = windows.NewLazySystemDLL("kernel32.dll").NewProc("CancelSynchronousIo")

// windowsKeyboardReader keeps a synchronous read on its issuing OS thread.
// The mutex prevents cancellation from targeting a thread after Go reuses it
// for unrelated I/O, including a stdout pipe read in the same process.
type windowsKeyboardReader struct {
	input      *os.File
	handle     windows.Handle
	overlapped bool
	mu         sync.Mutex
	thread     windows.Handle
}

// Read owns the thread handle until concurrent cancellation has returned.
// Keeping os.File.Read preserves its console Unicode handling and overlapped
// pipe support; failures to acquire a cancellable thread stop the shell.
func (r *windowsKeyboardReader) Read(p []byte) (int, error) {
	runtime.LockOSThread()
	defer runtime.UnlockOSThread()
	thread, err := windows.OpenThread(windows.THREAD_TERMINATE, false, windows.GetCurrentThreadId())
	if err != nil {
		return 0, os.NewSyscallError("OpenThread", err)
	}
	r.mu.Lock()
	r.thread = thread
	r.mu.Unlock()
	defer func() {
		r.mu.Lock()
		r.thread = 0
		_ = windows.CloseHandle(thread)
		r.mu.Unlock()
	}()
	return r.input.Read(p)
}

// interrupt uses Go's deadline mechanism for overlapped handles, and thread
// cancellation for synchronous pipes and consoles. CancelIoEx can deadlock on
// synchronous I/O; a cancellation before Read starts is retried by the owner.
func (r *windowsKeyboardReader) interrupt() {
	r.mu.Lock()
	defer r.mu.Unlock()
	if r.thread == 0 {
		return
	}
	if r.overlapped {
		if err := r.input.SetReadDeadline(time.Now()); err != nil {
			// A duplicate may not be registered with Go's poller. Its explicit
			// overlapped event still supports handle cancellation. The issuing
			// thread remains pinned until this call returns.
			_ = windows.CancelIoEx(r.handle, nil)
		}
		return
	}
	_, _, _ = cancelSynchronousKeyboardIO.Call(uintptr(r.thread))
}

// keyboardHandleIsOverlapped queries the immutable open mode before any read.
// Console handles use synchronous I/O and do not support the file-mode query.
func keyboardHandleIsOverlapped(handle windows.Handle) (bool, error) {
	var mode uint32
	if windows.GetConsoleMode(handle, &mode) == nil {
		return false, nil
	}
	// FILE_MODE_INFORMATION contains a single ULONG. x/sys exposes the query
	// and mode flags, but not the FileModeInformation class value from wdm.h.
	const fileModeInformation = 16
	var status windows.IO_STATUS_BLOCK
	if err := windows.NtQueryInformationFile(handle, &status, (*byte)(unsafe.Pointer(&mode)), uint32(unsafe.Sizeof(mode)), fileModeInformation); err != nil {
		return false, os.NewSyscallError("NtQueryInformationFile", err)
	}
	return mode&(windows.FILE_SYNCHRONOUS_IO_ALERT|windows.FILE_SYNCHRONOUS_IO_NONALERT) == 0, nil
}

// openKeyboardSource borrows stdin through an owned duplicate. Reads are
// independently interruptible without closing the caller's handle.
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
	overlapped, err := keyboardHandleIsOverlapped(handle)
	if err != nil {
		_ = windows.CloseHandle(handle)
		return keyboardSource{}, err
	}
	input := os.NewFile(uintptr(handle), stdin.Name())
	reader := &windowsKeyboardReader{input: input, handle: handle, overlapped: overlapped}
	return keyboardSource{
		Reader:    reader,
		interrupt: reader.interrupt,
		release:   input.Close,
	}, nil
}
