// SPDX-License-Identifier: MIT

package id

import (
	"os"
	"testing"

	"github.com/spf13/afero"
)

type retryRenameFs struct {
	afero.Fs
	failuresLeft int
	calls        int
}

func (f *retryRenameFs) Rename(oldname, newname string) error {
	f.calls++
	if f.failuresLeft > 0 {
		f.failuresLeft--
		return os.ErrPermission
	}
	return f.Fs.Rename(oldname, newname)
}

func TestAtomicWriteFileRetriesTransientRenameFailure(t *testing.T) {
	fs := &retryRenameFs{
		Fs:           afero.NewBasePathFs(afero.NewOsFs(), t.TempDir()),
		failuresLeft: 2,
	}

	if err := afero.WriteFile(fs, "triceCheck.c", []byte("old"), 0o644); err != nil {
		t.Fatal(err)
	}

	if err := atomicWriteFile(fs, "triceCheck.c", []byte("new"), 0o644); err != nil {
		t.Fatal(err)
	}

	got, err := afero.ReadFile(fs, "triceCheck.c")
	if err != nil {
		t.Fatal(err)
	}

	if string(got) != "new" {
		t.Fatalf("unexpected file content: got %q", string(got))
	}

	if fs.calls != 3 {
		t.Fatalf("unexpected Rename call count: got %d, want 3", fs.calls)
	}
}

type permanentRenameFs struct {
	afero.Fs
	calls int
}

func (f *permanentRenameFs) Rename(oldname, newname string) error {
	f.calls++
	return os.ErrInvalid
}

func TestAtomicWriteFileDoesNotRetryPermanentRenameFailure(t *testing.T) {
	fs := &permanentRenameFs{Fs: afero.NewBasePathFs(afero.NewOsFs(), t.TempDir())}

	if err := afero.WriteFile(fs, "triceCheck.c", []byte("old"), 0o644); err != nil {
		t.Fatal(err)
	}

	if err := atomicWriteFile(fs, "triceCheck.c", []byte("new"), 0o644); err == nil {
		t.Fatal("expected error")
	}

	if fs.calls != 1 {
		t.Fatalf("unexpected Rename call count: got %d, want 1", fs.calls)
	}
}
