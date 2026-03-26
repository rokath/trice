// SPDX-License-Identifier: MIT

package ant

import (
	"bytes"
	"errors"
	"io"
	"os"
	"path/filepath"
	"strings"
	"sync/atomic"
	"testing"

	"github.com/spf13/afero"
)

func TestWalkRejectsNilAction(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	if err := fSys.MkdirAll("root", 0o755); err != nil {
		t.Fatalf("mkdir failed: %v", err)
	}

	a := &Admin{
		Trees: []string{"root"},
	}

	err := a.Walk(io.Discard, fSys)
	if err == nil || !strings.Contains(err.Error(), "nil action") {
		t.Fatalf("expected nil action error, got: %v", err)
	}
}

func TestWalkRejectsNilFileSystem(t *testing.T) {
	a := &Admin{
		Trees:  []string{"root"},
		Action: func(io.Writer, *afero.Afero, string, os.FileInfo, *Admin) error { return nil },
	}
	err := a.Walk(io.Discard, nil)
	if err == nil || !strings.Contains(err.Error(), "nil filesystem") {
		t.Fatalf("expected nil filesystem error, got: %v", err)
	}
}

func TestWalkUsesDefaultMatcher(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	if err := fSys.MkdirAll("root/sub", 0o755); err != nil {
		t.Fatalf("mkdir failed: %v", err)
	}
	if err := fSys.WriteFile("root/a.txt", []byte("a"), 0o644); err != nil {
		t.Fatalf("write failed: %v", err)
	}
	if err := fSys.WriteFile("root/sub/b.txt", []byte("b"), 0o644); err != nil {
		t.Fatalf("write failed: %v", err)
	}

	var got int32
	a := &Admin{
		Trees: []string{"root"},
		Action: func(_ io.Writer, _ *afero.Afero, _ string, fi os.FileInfo, _ *Admin) error {
			if fi.IsDir() {
				t.Fatal("action must only be called for files")
			}
			atomic.AddInt32(&got, 1)
			return nil
		},
	}

	if err := a.Walk(io.Discard, fSys); err != nil {
		t.Fatalf("walk failed: %v", err)
	}
	if got != 2 {
		t.Fatalf("expected action for 2 files, got %d", got)
	}
}

func TestWalkWithNilWriterUsesDiscard(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	if err := fSys.MkdirAll("root", 0o755); err != nil {
		t.Fatalf("mkdir failed: %v", err)
	}
	if err := fSys.WriteFile("root/a.txt", []byte("a"), 0o644); err != nil {
		t.Fatalf("write failed: %v", err)
	}
	var got int32
	a := &Admin{
		Trees: []string{"root"},
		Action: func(_ io.Writer, _ *afero.Afero, _ string, _ os.FileInfo, _ *Admin) error {
			atomic.AddInt32(&got, 1)
			return nil
		},
	}
	if err := a.Walk(nil, fSys); err != nil {
		t.Fatalf("walk failed: %v", err)
	}
	if got != 1 {
		t.Fatalf("expected action count 1, got %d", got)
	}
}

func TestWalkIgnoresMissingTrees(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	a := &Admin{
		Trees:            []string{"missing"},
		MatchingFileName: func(_ os.FileInfo) bool { return true },
		Action:           func(io.Writer, *afero.Afero, string, os.FileInfo, *Admin) error { return nil },
	}

	err := a.Walk(io.Discard, fSys)
	if err != nil {
		t.Fatalf("expected missing tree to be non-fatal, got: %v", err)
	}
}

func TestWalkResetsErrorCountPerRun(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	if err := fSys.MkdirAll("root", 0o755); err != nil {
		t.Fatalf("mkdir failed: %v", err)
	}
	if err := fSys.WriteFile("root/a.txt", []byte("a"), 0o644); err != nil {
		t.Fatalf("write failed: %v", err)
	}

	a := &Admin{
		Trees:            []string{"root"},
		MatchingFileName: func(_ os.FileInfo) bool { return true },
		Action:           func(io.Writer, *afero.Afero, string, os.FileInfo, *Admin) error { return errors.New("boom") },
	}

	if err := a.Walk(io.Discard, fSys); err == nil {
		t.Fatal("expected first walk to fail")
	}

	a.Action = func(io.Writer, *afero.Afero, string, os.FileInfo, *Admin) error { return nil }
	if err := a.Walk(io.Discard, fSys); err != nil {
		t.Fatalf("expected second walk to succeed, got: %v", err)
	}
}

func TestWalkAggregatesActionErrors(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	if err := fSys.MkdirAll("root", 0o755); err != nil {
		t.Fatalf("mkdir failed: %v", err)
	}
	if err := fSys.WriteFile("root/a.txt", []byte("a"), 0o644); err != nil {
		t.Fatalf("write failed: %v", err)
	}
	if err := fSys.WriteFile("root/b.txt", []byte("b"), 0o644); err != nil {
		t.Fatalf("write failed: %v", err)
	}

	a := &Admin{
		Trees:            []string{"root"},
		MatchingFileName: func(_ os.FileInfo) bool { return true },
		Action: func(io.Writer, *afero.Afero, string, os.FileInfo, *Admin) error {
			return errors.New("boom")
		},
	}

	err := a.Walk(io.Discard, fSys)
	if err == nil || !strings.Contains(err.Error(), "2 walk errors") {
		t.Fatalf("expected 2 action errors, got: %v", err)
	}
}

func TestWalkExcludeTreeSkipsNestedFiles(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	if err := fSys.MkdirAll("root/keep", 0o755); err != nil {
		t.Fatalf("mkdir failed: %v", err)
	}
	if err := fSys.MkdirAll("root/skip/deeper", 0o755); err != nil {
		t.Fatalf("mkdir failed: %v", err)
	}
	if err := fSys.WriteFile("root/keep/a.txt", []byte("a"), 0o644); err != nil {
		t.Fatalf("write failed: %v", err)
	}
	if err := fSys.WriteFile("root/skip/deeper/b.txt", []byte("b"), 0o644); err != nil {
		t.Fatalf("write failed: %v", err)
	}
	var got int32
	a := &Admin{
		Trees:        []string{"root"},
		ExcludeTrees: []string{"root/skip"},
		Action: func(_ io.Writer, _ *afero.Afero, path string, _ os.FileInfo, _ *Admin) error {
			if strings.Contains(path, "skip") {
				t.Fatalf("excluded path reached action: %s", path)
			}
			atomic.AddInt32(&got, 1)
			return nil
		},
	}
	if err := a.Walk(io.Discard, fSys); err != nil {
		t.Fatalf("walk failed: %v", err)
	}
	if got != 1 {
		t.Fatalf("expected one processed file, got %d", got)
	}
}

func TestWalkVerboseExcludeMessage(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	if err := fSys.MkdirAll("root/skip", 0o755); err != nil {
		t.Fatalf("mkdir failed: %v", err)
	}
	var out bytes.Buffer
	a := &Admin{
		Trees:        []string{"root"},
		ExcludeTrees: []string{"root/skip"},
		Verbose:      true,
		Action:       func(io.Writer, *afero.Afero, string, os.FileInfo, *Admin) error { return nil },
	}
	if err := a.Walk(&out, fSys); err != nil {
		t.Fatalf("walk failed: %v", err)
	}
	want := "exclude: " + filepath.Join("root", "skip") + "/"
	if !strings.Contains(out.String(), want) {
		t.Fatalf("expected verbose exclude output, got: %q", out.String())
	}
}
