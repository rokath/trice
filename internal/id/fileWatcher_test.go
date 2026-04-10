// SPDX-License-Identifier: MIT

package id

import (
	"bytes"
	"errors"
	"strings"
	"sync"
	"testing"
	"time"

	"github.com/fsnotify/fsnotify"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

type fakeFileWatcher struct {
	events chan fsnotify.Event
	errors chan error
	added  []string
	addErr error
}

func newFakeFileWatcher() *fakeFileWatcher {
	return &fakeFileWatcher{
		events: make(chan fsnotify.Event, 8),
		errors: make(chan error, 8),
	}
}

func (f *fakeFileWatcher) Add(name string) error {
	f.added = append(f.added, name)
	return f.addErr
}

func (f *fakeFileWatcher) Close() error { return nil }

func (f *fakeFileWatcher) Events() <-chan fsnotify.Event { return f.events }

func (f *fakeFileWatcher) Errors() <-chan error { return f.errors }

// TestFSNotifyWatcherChannelAdapters verifies the expected behavior.
func TestFSNotifyWatcherChannelAdapters(t *testing.T) {
	raw, err := fsnotify.NewWatcher()
	require.NoError(t, err)
	defer raw.Close()

	adapted := fsNotifyWatcher{raw}
	assert.True(t, raw.Events == adapted.Events())
	assert.True(t, raw.Errors == adapted.Errors())
}

// TestRunTILFileWatcherRefreshesOnEvent verifies the expected behavior.
func TestRunTILFileWatcherRefreshesOnEvent(t *testing.T) {
	defer Setup(t)()

	oldNow := fileWatcherNow
	oldSleep := fileWatcherSleep
	t.Cleanup(func() {
		fileWatcherNow = oldNow
		fileWatcherSleep = oldSleep
	})
	fileWatcherNow = func() time.Time { return time.Unix(10, 0) }
	fileWatcherSleep = func(time.Duration) {}

	require.NoError(t, FSys.WriteFile(FnJSON, []byte(`{"17":{"Type":"TRICE","Strg":"value=%d"}}`), 0o777))

	lu := make(TriceIDLookUp)
	var mu sync.RWMutex
	watcher := newFakeFileWatcher()
	done := make(chan struct{})
	var out bytes.Buffer

	go runTILFileWatcher(&out, FSys, &mu, lu, watcher, done)
	watcher.events <- fsnotify.Event{Name: FnJSON, Op: fsnotify.Write}

	assert.Eventually(t, func() bool {
		mu.RLock()
		defer mu.RUnlock()
		return lu[17].Type == "TRICE_1" && lu[17].Strg == "value=%d"
	}, time.Second, 10*time.Millisecond)

	close(done)
	assert.Contains(t, out.String(), "Refreshing id.List.")
}

// TestRunTILFileWatcherDebouncesEvents verifies the expected behavior.
func TestRunTILFileWatcherDebouncesEvents(t *testing.T) {
	defer Setup(t)()

	oldNow := fileWatcherNow
	oldSleep := fileWatcherSleep
	t.Cleanup(func() {
		fileWatcherNow = oldNow
		fileWatcherSleep = oldSleep
	})
	times := []time.Time{
		time.Unix(10, 0),
		time.Unix(11, 0),
	}
	fileWatcherNow = func() time.Time {
		now := times[0]
		if len(times) > 1 {
			times = times[1:]
		}
		return now
	}
	fileWatcherSleep = func(time.Duration) {}

	require.NoError(t, FSys.WriteFile(FnJSON, []byte(`{"17":{"Type":"TRICE","Strg":"value=%d"}}`), 0o777))

	lu := make(TriceIDLookUp)
	var mu sync.RWMutex
	watcher := newFakeFileWatcher()
	done := make(chan struct{})

	go runTILFileWatcher(ioDiscard{}, FSys, &mu, lu, watcher, done)
	watcher.events <- fsnotify.Event{Name: FnJSON, Op: fsnotify.Write}

	assert.Eventually(t, func() bool {
		mu.RLock()
		defer mu.RUnlock()
		return lu[17].Type == "TRICE_1"
	}, time.Second, 10*time.Millisecond)

	require.NoError(t, FSys.WriteFile(FnJSON, []byte(`{"17":{"Type":"TRICE8","Strg":"changed=%d"}}`), 0o777))
	watcher.events <- fsnotify.Event{Name: FnJSON, Op: fsnotify.Write}
	time.Sleep(20 * time.Millisecond)

	mu.RLock()
	assert.Equal(t, "TRICE_1", lu[17].Type)
	assert.Equal(t, "value=%d", lu[17].Strg)
	mu.RUnlock()

	close(done)
}

// TestRunLIFileWatcherRefreshesOnEvent verifies the expected behavior.
func TestRunLIFileWatcherRefreshesOnEvent(t *testing.T) {
	defer Setup(t)()

	oldNow := fileWatcherNow
	oldSleep := fileWatcherSleep
	t.Cleanup(func() {
		fileWatcherNow = oldNow
		fileWatcherSleep = oldSleep
	})
	fileWatcherNow = func() time.Time { return time.Unix(10, 0) }
	fileWatcherSleep = func(time.Duration) {}

	require.NoError(t, FSys.WriteFile(LIFnJSON, []byte(`{"17":{"File":"demo.c","Line":42}}`), 0o777))

	li := make(TriceIDLookUpLI)
	watcher := newFakeFileWatcher()
	done := make(chan struct{})
	var out bytes.Buffer

	go runLIFileWatcher(&out, FSys, li, watcher, done)
	watcher.events <- fsnotify.Event{Name: LIFnJSON, Op: fsnotify.Write}

	assert.Eventually(t, func() bool {
		entry, ok := li[17]
		return ok && entry.File == "demo.c" && entry.Line == 42
	}, time.Second, 10*time.Millisecond)

	close(done)
	assert.Contains(t, out.String(), "Refreshing li list.")
}

// TestRunFileWatchersReportErrors verifies the expected behavior.
func TestRunFileWatchersReportErrors(t *testing.T) {
	defer Setup(t)()

	oldNow := fileWatcherNow
	t.Cleanup(func() { fileWatcherNow = oldNow })
	fileWatcherNow = func() time.Time { return time.Unix(10, 0) }

	watcher := newFakeFileWatcher()
	done := make(chan struct{})
	var out bytes.Buffer

	go runTILFileWatcher(&out, FSys, &sync.RWMutex{}, TriceIDLookUp{}, watcher, done)
	watcher.errors <- errors.New("til boom")
	assert.Eventually(t, func() bool { return strings.Contains(out.String(), "TIL ERROR1") }, time.Second, 10*time.Millisecond)
	close(done)

	watcher = newFakeFileWatcher()
	done = make(chan struct{})
	out.Reset()
	go runLIFileWatcher(&out, FSys, TriceIDLookUpLI{}, watcher, done)
	watcher.errors <- errors.New("li boom")
	assert.Eventually(t, func() bool { return strings.Contains(out.String(), "LI ERROR1") }, time.Second, 10*time.Millisecond)
	close(done)
}

// TestRunFileWatchersStopOnClosedChannels verifies the expected behavior.
func TestRunFileWatchersStopOnClosedChannels(t *testing.T) {
	defer Setup(t)()

	watcher := newFakeFileWatcher()
	close(watcher.events)
	done := make(chan struct{})
	finished := make(chan struct{})
	go func() {
		runTILFileWatcher(ioDiscard{}, FSys, &sync.RWMutex{}, TriceIDLookUp{}, watcher, done)
		close(finished)
	}()
	assert.Eventually(t, func() bool {
		select {
		case <-finished:
			return true
		default:
			return false
		}
	}, time.Second, 10*time.Millisecond)

	watcher = newFakeFileWatcher()
	close(watcher.errors)
	finished = make(chan struct{})
	go func() {
		runLIFileWatcher(ioDiscard{}, FSys, TriceIDLookUpLI{}, watcher, done)
		close(finished)
	}()
	assert.Eventually(t, func() bool {
		select {
		case <-finished:
			return true
		default:
			return false
		}
	}, time.Second, 10*time.Millisecond)
}

// TestPublicFileWatcherWrappers verifies the expected behavior.
func TestPublicFileWatcherWrappers(t *testing.T) {
	defer Setup(t)()

	oldNewWatcher := newFileWatcher
	oldDone := fileWatcherDone
	oldVerbose := Verbose
	t.Cleanup(func() {
		newFileWatcher = oldNewWatcher
		fileWatcherDone = oldDone
		Verbose = oldVerbose
	})

	closedDone := make(chan struct{})
	close(closedDone)
	fileWatcherDone = func() <-chan struct{} { return closedDone }
	Verbose = true

	watcher := newFakeFileWatcher()
	newFileWatcher = func() (fileWatcher, error) { return watcher, nil }
	var out bytes.Buffer
	lu := TriceIDLookUp{}
	var mu sync.RWMutex
	lu.FileWatcher(&out, FSys, &mu)
	assert.Equal(t, []string{FnJSON}, watcher.added)
	assert.Contains(t, out.String(), "Create a new file watcher for TIL.")
	assert.Contains(t, out.String(), "watched now for changes")

	watcher = newFakeFileWatcher()
	newFileWatcher = func() (fileWatcher, error) { return watcher, nil }
	out.Reset()
	li := TriceIDLookUpLI{}
	li.FileWatcher(&out, FSys)
	assert.Equal(t, []string{LIFnJSON}, watcher.added)
	assert.Contains(t, out.String(), "Create a new file watcher for LI.")
	assert.Contains(t, out.String(), "watched now for changes")
}
