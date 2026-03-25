package ant

import (
	"errors"
	"io"
	"os"
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
