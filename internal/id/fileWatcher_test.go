// SPDX-License-Identifier: MIT

package id

import (
	"bytes"
	"errors"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"sync"
	"testing"
	"time"

	"github.com/fsnotify/fsnotify"
	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// watcherOutput safely observes diagnostics while the watcher is still running.
type watcherOutput struct {
	mu sync.Mutex
	b  bytes.Buffer
}

func (w *watcherOutput) Write(p []byte) (int, error) {
	w.mu.Lock()
	defer w.mu.Unlock()
	return w.b.Write(p)
}

func (w *watcherOutput) String() string {
	w.mu.Lock()
	defer w.mu.Unlock()
	return w.b.String()
}

// fakeFileWatcher provides controlled backend failures and observable release.
type fakeFileWatcher struct {
	events chan fsnotify.Event
	errors chan error
	added  []string
	addErr error
	closed bool
}

func newFakeFileWatcher() *fakeFileWatcher {
	return &fakeFileWatcher{events: make(chan fsnotify.Event, 8), errors: make(chan error, 8)}
}

func (f *fakeFileWatcher) Add(name string) error         { f.added = append(f.added, name); return f.addErr }
func (f *fakeFileWatcher) Close() error                  { f.closed = true; return nil }
func (f *fakeFileWatcher) Events() <-chan fsnotify.Event { return f.events }
func (f *fakeFileWatcher) Errors() <-chan error          { return f.errors }

// watcherFiles isolates both command paths and filesystem events from the repo.
func watcherFiles(t *testing.T) *afero.Afero {
	t.Helper()
	oldTIL, oldLI := FnJSON, LIFnJSON
	t.Cleanup(func() { FnJSON, LIFnJSON = oldTIL, oldLI })
	directory := t.TempDir()
	FnJSON, LIFnJSON = filepath.Join(directory, "til.json"), filepath.Join(directory, "li.json")
	return &afero.Afero{Fs: afero.NewOsFs()}
}

// TestWatchLookUpTablesRealSaves exercises the OS backend with both ordinary
// writes and the atomic replacement used by bind/insert. Deleted keys prove
// that a reload replaces a complete table rather than merging stale entries.
func TestWatchLookUpTablesRealSaves(t *testing.T) {
	fs := watcherFiles(t)
	lu := TriceIDLookUp{9: {Type: "TRICE", Strg: "old"}}
	li := TriceIDLookUpLI{9: {File: "old.c", Line: 9}}
	var mu sync.RWMutex
	var out watcherOutput
	require.NoError(t, fs.WriteFile(FnJSON, []byte(`{"17":{"Type":"TRICE16","Strg":"value={old_name}"}}`), 0o600))
	require.NoError(t, fs.WriteFile(LIFnJSON, []byte(`{"17":{"File":"first.c","Line":17}}`), 0o600))
	stop, err := WatchLookUpTables(&out, fs, lu, li, &mu)
	require.NoError(t, err)
	t.Cleanup(stop)
	waitFor := func(format, file string, line int) {
		t.Helper()
		require.Eventually(t, func() bool {
			mu.RLock()
			defer mu.RUnlock()
			return len(lu) == 1 && lu[17] == (TriceFmt{Type: "TRICE16_1", Strg: format}) &&
				len(li) == 1 && li[17] == (TriceLI{File: file, Line: line})
		}, 3*time.Second, 5*time.Millisecond, out.String())
	}
	waitFor("value={old_name}", "first.c", 17)

	// Each save follows the previous observed reload immediately: the old
	// five-second suppression would lose every update after the first one.
	for _, save := range []string{"direct_write", "atomic_replace", "second_atomic_replace"} {
		t.Run(save, func(t *testing.T) {
			til := []byte(`{"17":{"Type":"TRICE16","Strg":"value={` + save + `}"}}`)
			locations := []byte(`{"17":{"File":"` + save + `.c","Line":42}}`)
			if save == "direct_write" {
				require.NoError(t, fs.WriteFile(FnJSON, til, 0o600))
				require.NoError(t, fs.WriteFile(LIFnJSON, locations, 0o600))
			} else {
				require.NoError(t, atomicWriteFile(fs.Fs, FnJSON, til, 0o600))
				require.NoError(t, atomicWriteFile(fs.Fs, LIFnJSON, locations, 0o600))
			}
			waitFor("value={"+save+"}", save+".c", 42)
		})
	}
	assert.Empty(t, out.String(), "successful reload is silent without -v")
	stop()
	stop() // Closing twice must neither deadlock nor reuse a released watcher.
	require.NoError(t, fs.WriteFile(FnJSON, []byte(`{}`), 0o600))
	mu.RLock()
	assert.Len(t, lu, 1, "stop returns only after the last possible live-map write")
	mu.RUnlock()
}

// TestWatchLookUpTablesKeepsLastGoodState rejects partial/invalid input without
// leaking any decoded prefix into a live map, then recovers without restarting.
func TestWatchLookUpTablesKeepsLastGoodState(t *testing.T) {
	for _, target := range []string{"TIL", "LI"} {
		for _, bad := range []struct{ name, data string }{
			{"empty_during_truncate", ""},
			{"partial_JSON", `{"17":`},
			{"null_is_not_a_table", `null`},
			{"wrong_value_type_after_valid_entry", `{"17":{},"18":false}`},
		} {
			t.Run(target+"/"+bad.name, func(t *testing.T) {
				fs := watcherFiles(t)
				path := FnJSON
				if target == "LI" {
					path = LIFnJSON
				}
				lu := TriceIDLookUp{7: {Type: "TRICE", Strg: "last good"}}
				li := TriceIDLookUpLI{7: {File: "last.c", Line: 7}}
				require.NoError(t, fs.WriteFile(FnJSON, []byte(`{"7":{"Type":"TRICE","Strg":"last good"}}`), 0o600))
				require.NoError(t, fs.WriteFile(LIFnJSON, []byte(`{"7":{"File":"last.c","Line":7}}`), 0o600))
				require.NoError(t, fs.WriteFile(path, []byte(bad.data), 0o600))
				var mu sync.RWMutex
				var out watcherOutput
				stop, err := WatchLookUpTables(&out, fs, lu, li, &mu)
				require.NoError(t, err)
				t.Cleanup(stop)
				require.Eventually(t, func() bool { return bytes.Contains([]byte(out.String()), []byte("keeping last valid table")) }, 3*time.Second, 5*time.Millisecond)
				mu.RLock()
				assert.Equal(t, TriceIDLookUp{7: {Type: "TRICE", Strg: "last good"}}, lu)
				assert.Equal(t, TriceIDLookUpLI{7: {File: "last.c", Line: 7}}, li)
				mu.RUnlock()
				require.NoError(t, atomicWriteFile(fs.Fs, path, []byte(`{}`), 0o600))
				require.Eventually(t, func() bool {
					mu.RLock()
					defer mu.RUnlock()
					if target == "TIL" {
						return len(lu) == 0 && len(li) == 1
					}
					return len(li) == 0 && len(lu) == 1
				}, 3*time.Second, 5*time.Millisecond, "a valid empty object is an intentional empty table")
			})
		}
	}
}

// TestWatchLookUpTablesDeleteAndRecreate checks that a directory watch remains
// useful after both files disappear, and that separate directories are watched.
func TestWatchLookUpTablesDeleteAndRecreate(t *testing.T) {
	fs := watcherFiles(t)
	LIFnJSON = filepath.Join(t.TempDir(), "locations.json")
	lu, li := TriceIDLookUp{}, TriceIDLookUpLI{}
	var mu sync.RWMutex
	var out watcherOutput
	stop, err := WatchLookUpTables(&out, fs, lu, li, &mu)
	require.NoError(t, err)
	t.Cleanup(stop)
	for round := 0; round < 2; round++ {
		require.Eventually(t, func() bool { return bytes.Count([]byte(out.String()), []byte("will retry")) >= 2*(round+1) }, 3*time.Second, 5*time.Millisecond)
		message := fmt.Sprintf("back%d", round)
		require.NoError(t, fs.WriteFile(FnJSON, []byte(`{"17":{"Type":"TRICE","Strg":"`+message+`"}}`), 0o600))
		require.NoError(t, fs.WriteFile(LIFnJSON, []byte(`{"17":{"File":"`+message+`.c","Line":17}}`), 0o600))
		require.Eventually(t, func() bool {
			mu.RLock()
			defer mu.RUnlock()
			return lu[17].Strg == message && li[17].File == message+".c"
		}, 3*time.Second, 5*time.Millisecond)
		if round == 0 {
			require.NoError(t, os.Remove(FnJSON))
			require.NoError(t, os.Remove(LIFnJSON))
			mu.RLock()
			assert.Equal(t, message, lu[17].Strg, "removing a file preserves the last usable table")
			assert.Equal(t, message+".c", li[17].File)
			mu.RUnlock()
		}
	}
}

// TestWatchLookUpTablesSetupFailuresAndDisabledInputs verifies graceful setup
// failure, deduplicated directory registration, and joined resource cleanup.
func TestWatchLookUpTablesSetupFailuresAndDisabledInputs(t *testing.T) {
	fs := watcherFiles(t)
	oldFactory := newFileWatcher
	t.Cleanup(func() { newFileWatcher = oldFactory })
	var mu sync.RWMutex
	lu, li := TriceIDLookUp{}, TriceIDLookUpLI{}
	newFileWatcher = func() (fileWatcher, error) { return nil, errors.New("backend unavailable") }
	stop, err := WatchLookUpTables(io.Discard, fs, lu, li, &mu)
	assert.ErrorContains(t, err, "backend unavailable")
	stop()
	for _, disabled := range []string{"off", "none", "no", "emptyFile", ""} {
		FnJSON, LIFnJSON = disabled, disabled
		stop, err = WatchLookUpTables(io.Discard, fs, lu, li, &mu)
		assert.NoError(t, err, "disabled paths must not try to create a watcher")
		stop()
	}
	FnJSON, LIFnJSON = "til.json", "li.json"
	stop, err = WatchLookUpTables(io.Discard, &afero.Afero{Fs: afero.NewMemMapFs()}, lu, li, &mu)
	assert.NoError(t, err, "an in-memory test must not observe unrelated real files")
	stop()
	for _, addErr := range []error{errors.New("directory denied"), nil} {
		fake := newFakeFileWatcher()
		fake.addErr = addErr
		newFileWatcher = func() (fileWatcher, error) { return fake, nil }
		stop, err = WatchLookUpTables(io.Discard, fs, lu, li, &mu)
		if addErr != nil {
			assert.ErrorContains(t, err, "directory denied")
		} else {
			assert.NoError(t, err)
		}
		stop()
		assert.Len(t, fake.added, 1, "one directory holds both lookup files")
		assert.True(t, fake.closed, "even partial setup must release the backend")
	}
}

// TestWatchLookUpTablesKeepsHealthyDirectory verifies partial setup: a missing
// LI directory is reported while TIL reload remains active and stoppable.
func TestWatchLookUpTablesKeepsHealthyDirectory(t *testing.T) {
	fs := watcherFiles(t)
	LIFnJSON = filepath.Join(t.TempDir(), "not-created", "li.json")
	require.NoError(t, fs.WriteFile(FnJSON, []byte(`{"17":{"Type":"TRICE","Strg":"still reloads"}}`), 0o600))
	lu := TriceIDLookUp{}
	li := TriceIDLookUpLI{17: {File: "previous.c", Line: 17}}
	var mu sync.RWMutex
	stop, err := WatchLookUpTables(io.Discard, fs, lu, li, &mu)
	assert.ErrorContains(t, err, LIFnJSON)
	t.Cleanup(stop)
	require.Eventually(t, func() bool {
		mu.RLock()
		defer mu.RUnlock()
		return lu[17].Strg == "still reloads"
	}, 3*time.Second, 5*time.Millisecond)
	mu.RLock()
	assert.Equal(t, TriceLI{File: "previous.c", Line: 17}, li[17])
	mu.RUnlock()
}

// TestFileWatcherRetriesAndIgnoresUnrelatedEvents controls event delivery so
// recovery without another write notification and quiet repeated errors are observable.
func TestFileWatcherRetriesAndIgnoresUnrelatedEvents(t *testing.T) {
	fake := newFakeFileWatcher()
	var out watcherOutput
	var mu sync.Mutex
	attempts, fail := 0, true
	path := filepath.Join(t.TempDir(), "til.json")
	table := &watchedTable{path: path, pending: true, reload: func() error {
		mu.Lock()
		defer mu.Unlock()
		attempts++
		if fail {
			return errors.New("still incomplete")
		}
		return nil
	}}
	done, finished := make(chan struct{}), make(chan struct{})
	go func() { defer close(finished); runFileWatcher(&out, fake, []*watchedTable{table}, done, true) }()
	t.Cleanup(func() { close(done); <-finished })
	require.Eventually(t, func() bool { mu.Lock(); defer mu.Unlock(); return attempts >= 2 }, 3*time.Second, 5*time.Millisecond)
	assert.Equal(t, 1, bytes.Count([]byte(out.String()), []byte("still incomplete")), "identical failures do not flood stderr")
	mu.Lock()
	fail = false
	mu.Unlock()
	require.Eventually(t, func() bool { return bytes.Contains([]byte(out.String()), []byte("Reloaded")) }, 3*time.Second, 5*time.Millisecond)
	mu.Lock()
	before := attempts
	mu.Unlock()
	fake.events <- fsnotify.Event{Name: path + ".tmp", Op: fsnotify.Write}
	fake.events <- fsnotify.Event{Name: path, Op: fsnotify.Chmod}
	assert.Never(t, func() bool { mu.Lock(); defer mu.Unlock(); return attempts != before }, 150*time.Millisecond, 5*time.Millisecond)
	fake.errors <- errors.New("event overflow")
	require.Eventually(t, func() bool { mu.Lock(); defer mu.Unlock(); return attempts > before }, 3*time.Second, 5*time.Millisecond)
	assert.Contains(t, out.String(), "event overflow; rechecking tables")
}

// TestFileWatcherClosedChannels stops cleanly on either closed backend channel.
func TestFileWatcherClosedChannels(t *testing.T) {
	for _, channel := range []string{"events", "errors"} {
		t.Run(channel, func(t *testing.T) {
			fake := newFakeFileWatcher()
			if channel == "events" {
				close(fake.events)
			} else {
				close(fake.errors)
			}
			var out bytes.Buffer
			runFileWatcher(&out, fake, nil, nil, false)
			assert.Contains(t, out.String(), "restart trice log")
		})
	}
}
