// SPDX-License-Identifier: MIT

package id

import (
	"fmt"
	"io"
	"sync"
	"time"

	"github.com/fsnotify/fsnotify"
	"github.com/rokath/trice/pkg/msg"
	"github.com/spf13/afero"
)

type fileWatcher interface {
	// Add registers the file or directory that should be observed.
	Add(string) error
	// Close releases the watcher resources.
	Close() error
	// Events exposes the raw fsnotify event stream.
	Events() <-chan fsnotify.Event
	// Errors exposes watcher errors reported by the backend.
	Errors() <-chan error
}

type fsNotifyWatcher struct {
	*fsnotify.Watcher
}

// Events adapts the embedded fsnotify watcher to the small testable interface.
func (w fsNotifyWatcher) Events() <-chan fsnotify.Event { return w.Watcher.Events }

// Errors adapts the embedded fsnotify watcher to the small testable interface.
func (w fsNotifyWatcher) Errors() <-chan error { return w.Watcher.Errors }

var (
	// newFileWatcher is injected in tests so the public FileWatcher methods can run
	// against a deterministic fake watcher instead of the real fsnotify backend.
	newFileWatcher = func() (fileWatcher, error) { w, err := fsnotify.NewWatcher(); return fsNotifyWatcher{w}, err }
	// fileWatcherNow is separated so debounce-related tests do not depend on wall clock time.
	fileWatcherNow = time.Now
	// fileWatcherSleep is separated so tests can skip the artificial delay after a change event.
	fileWatcherSleep = time.Sleep
	// fileWatcherDone returns the shutdown channel used by the public wrapper methods.
	// Production keeps the previous "run forever" behavior by returning nil.
	fileWatcherDone = func() <-chan struct{} { return nil }
	// fileWatcherDebounce prevents repeated refreshes when a single save operation emits multiple events.
	fileWatcherDebounce = 5000 * time.Millisecond
	// fileWatcherReadDelay gives the writer side a brief moment to finish replacing the JSON file.
	fileWatcherReadDelay = 100 * time.Millisecond
)

// FileWatcher checks the id list file for changes.
//
// The public method is intentionally thin:
//   - create and register the underlying watcher
//   - keep the legacy verbose log output
//   - delegate the actual event loop to runTILFileWatcher
//
// The extracted loop keeps the runtime behavior but makes the logic unit-testable.
// Tests can replace the watcher implementation, the clock, the sleep function and the
// stop signal without touching the external API.
func (lu TriceIDLookUp) FileWatcher(w io.Writer, fSys *afero.Afero, m *sync.RWMutex) {
	if Verbose {
		fmt.Fprintln(w, "Create a new file watcher for TIL.")
	}
	watcher, err := newFileWatcher()
	msg.FatalOnErr(err)
	defer func() {
		if Verbose {
			fmt.Fprintln(w, "Closing file watcher TIL.")
		}
		msg.OnErr(watcher.Close())
	}()

	if Verbose {
		fmt.Fprintln(w, "Out of the box fsnotify can watch a single file, or a single directory (TIL).")
	}
	msg.InfoOnErr(watcher.Add(FnJSON), "ERROR2")
	if Verbose {
		fmt.Fprintln(w, FnJSON, "watched now for changes")
	}
	runTILFileWatcher(w, fSys, m, lu, watcher, fileWatcherDone())
}

// FileWatcher checks the id location information file for changes.
//
// The implementation mirrors the TIL watcher but refreshes li.json instead of til.json.
// Keeping both wrappers symmetric simplifies the tests and helps future refactorings.
func (li TriceIDLookUpLI) FileWatcher(w io.Writer, fSys *afero.Afero) {
	if Verbose {
		fmt.Fprintln(w, "Create a new file watcher for LI.")
	}
	watcher, err := newFileWatcher()
	msg.FatalOnErr(err)
	defer func() {
		if Verbose {
			fmt.Fprintln(w, "Closing file watcher LI.")
		}
		msg.OnErr(watcher.Close())
	}()

	if Verbose {
		fmt.Fprintln(w, "Out of the box fsnotify can watch a single file, or a single directory (LI).")
	}
	msg.InfoOnErr(watcher.Add(LIFnJSON), "ERROR2")
	if Verbose {
		fmt.Fprintln(w, LIFnJSON, "watched now for changes")
	}
	runLIFileWatcher(w, fSys, li, watcher, fileWatcherDone())
}

// runTILFileWatcher contains the actual event loop for til.json updates.
//
// It exits when:
//   - the caller closes done
//   - the event channel is closed
//   - the error channel is closed
//
// Using a dedicated helper keeps the externally visible API unchanged while allowing
// deterministic unit tests for refresh, debounce and error handling.
func runTILFileWatcher(w io.Writer, fSys *afero.Afero, m *sync.RWMutex, lu TriceIDLookUp, watcher fileWatcher, done <-chan struct{}) {
	if Verbose {
		fmt.Fprintln(w, "TIL Watching...")
	}
	var last time.Time
	for {
		select {
		case <-done:
			return
		case event, ok := <-watcher.Events():
			if !ok {
				return
			}
			if Verbose {
				fmt.Fprintln(w, "TIL EVENT:", event, ok, fileWatcherNow().UTC())
			}
			now := fileWatcherNow()
			if now.Sub(last) > fileWatcherDebounce {
				fmt.Fprintln(w, "Refreshing id.List.")
				fileWatcherSleep(fileWatcherReadDelay)
				m.Lock()
				msg.FatalOnErr(lu.fromFile(fSys, FnJSON))
				lu.AddFmtCount(w)
				m.Unlock()
				last = now
			}
		case err, ok := <-watcher.Errors():
			if !ok {
				return
			}
			fmt.Fprintln(w, "TIL ERROR1", err, fileWatcherNow().UTC())
		}
	}
}

// runLIFileWatcher is the li.json equivalent of runTILFileWatcher.
//
// The debounce logic is intentionally shared at the behavioral level: repeated file-system
// events caused by one save operation should trigger only one reload in the configured interval.
func runLIFileWatcher(w io.Writer, fSys *afero.Afero, li TriceIDLookUpLI, watcher fileWatcher, done <-chan struct{}) {
	if Verbose {
		fmt.Fprintln(w, "LI Watching...")
	}
	var last time.Time
	for {
		select {
		case <-done:
			return
		case event, ok := <-watcher.Events():
			if !ok {
				return
			}
			if Verbose {
				fmt.Fprintln(w, "LI EVENT:", event, ok, fileWatcherNow().UTC())
			}
			now := fileWatcherNow()
			if now.Sub(last) > fileWatcherDebounce {
				fmt.Fprintln(w, "Refreshing li list.")
				fileWatcherSleep(fileWatcherReadDelay)
				msg.FatalOnErr(li.fromFile(fSys, LIFnJSON))
				last = now
			}
		case err, ok := <-watcher.Errors():
			if !ok {
				return
			}
			fmt.Fprintln(w, "LI ERROR1", err, fileWatcherNow().UTC())
		}
	}
}
