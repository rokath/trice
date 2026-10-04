// SPDX-License-Identifier: MIT

package id

import (
	"encoding/json"
	"errors"
	"fmt"
	"io"
	"path/filepath"
	"sync"
	"time"

	"github.com/fsnotify/fsnotify"
	"github.com/spf13/afero"
)

// fileWatcher abstracts OS events so error and shutdown paths can be tested.
type fileWatcher interface {
	Add(string) error
	Close() error
	Events() <-chan fsnotify.Event
	Errors() <-chan error
}

// fsNotifyWatcher exposes the backend channels through the testable interface.
type fsNotifyWatcher struct{ *fsnotify.Watcher }

func (w fsNotifyWatcher) Events() <-chan fsnotify.Event { return w.Watcher.Events }
func (w fsNotifyWatcher) Errors() <-chan error          { return w.Watcher.Errors }

// newFileWatcher allows tests to inject setup errors without exhausting OS resources.
var newFileWatcher = func() (fileWatcher, error) {
	w, err := fsnotify.NewWatcher()
	return fsNotifyWatcher{w}, err
}

// watchedTable keeps retry state private to the watcher goroutine. A failed
// reload never modifies the live table; repeated identical errors are quiet.
type watchedTable struct {
	path      string
	reload    func() error
	pending   bool
	lastError string
}

// WatchLookUpTables starts automatic reload for the current log command.
// Directory watches survive atomic file replacement. All readers of either
// live map must use mutex. The returned, idempotent stop function joins the
// goroutine, so no diagnostics or table writes can outlive the command.
// Virtual filesystems and explicit disabled/test filenames have no OS watcher.
// Setup errors leave existing tables usable and may affect only one directory.
func WatchLookUpTables(w io.Writer, fSys *afero.Afero, lu TriceIDLookUp, li TriceIDLookUpLI, mutex *sync.RWMutex) (func(), error) {
	noop := func() {}
	if _, realFS := fSys.Fs.(*afero.OsFs); !realFS {
		return noop, nil
	}
	var tables []*watchedTable
	for _, table := range []struct {
		path    string
		enabled bool
		reload  func(string) error
	}{
		{FnJSON, lu != nil, func(path string) error {
			data, err := fSys.ReadFile(path)
			if err != nil {
				return err
			}
			var next TriceIDLookUp
			if err := json.Unmarshal(data, &next); err != nil {
				return err
			}
			if next == nil {
				return errors.New("expected a JSON object, got null")
			}
			// Apply the same normalization and diagnostics as initial loading;
			// existing historical entries must not block newer valid entries.
			next.AddFmtCount(w)
			mutex.Lock()
			clear(lu)
			for key, value := range next {
				lu[key] = value
			}
			mutex.Unlock()
			return nil
		}},
		{LIFnJSON, li != nil, func(path string) error {
			data, err := fSys.ReadFile(path)
			if err != nil {
				return err
			}
			var next TriceIDLookUpLI
			if err := json.Unmarshal(data, &next); err != nil {
				return err
			}
			if next == nil {
				return errors.New("expected a JSON object, got null")
			}
			mutex.Lock()
			clear(li)
			for key, value := range next {
				li[key] = value
			}
			mutex.Unlock()
			return nil
		}},
	} {
		if !table.enabled || table.path == "" || table.path == "emptyFile" || table.path == "off" || table.path == "none" || table.path == "no" {
			continue
		}
		path, err := filepath.Abs(table.path)
		if err != nil {
			return noop, err
		}
		tables = append(tables, &watchedTable{path: path, pending: true, reload: func() error { return table.reload(path) }})
	}
	if len(tables) == 0 {
		return noop, nil
	}
	watcher, err := newFileWatcher()
	if err != nil {
		return noop, err
	}
	// Register each directory once; an unavailable LI directory must not stop TIL reload.
	directories := make(map[string]error)
	var active []*watchedTable
	var setupErrors []error
	for _, table := range tables {
		directory := filepath.Dir(table.path)
		addErr, registered := directories[directory]
		if !registered {
			addErr = watcher.Add(directory)
			directories[directory] = addErr
		}
		if addErr != nil {
			setupErrors = append(setupErrors, fmt.Errorf("watch %s: %w", table.path, addErr))
			continue
		}
		active = append(active, table)
	}
	if len(active) == 0 {
		_ = watcher.Close()
		return noop, errors.Join(setupErrors...)
	}
	done, finished := make(chan struct{}), make(chan struct{})
	verbose := Verbose // Command settings must not be read after a later command resets them.
	go func() {
		defer close(finished)
		defer watcher.Close()
		runFileWatcher(w, watcher, active, done, verbose)
	}()
	var once sync.Once
	return func() { once.Do(func() { close(done); <-finished }) }, errors.Join(setupErrors...)
}

// runFileWatcher coalesces saves at their trailing edge, rather than discarding
// all events for several seconds after the first one. Failed reads are retried
// even if the backend emits no further event. Initial pending reloads close the
// gap between the caller's initial read and directory-watch registration.
func runFileWatcher(w io.Writer, watcher fileWatcher, tables []*watchedTable, done <-chan struct{}, verbose bool) {
	const settle = 100 * time.Millisecond
	timer := time.NewTimer(settle)
	defer timer.Stop()
	for {
		select {
		case <-done:
			return
		case event, ok := <-watcher.Events():
			if !ok {
				fmt.Fprintln(w, "warning: automatic table reload stopped: file event channel closed; restart trice log")
				return
			}
			if event.Op&(fsnotify.Write|fsnotify.Create|fsnotify.Rename|fsnotify.Remove) == 0 {
				continue
			}
			for _, table := range tables {
				if filepath.Clean(event.Name) == table.path {
					table.pending = true
					timer.Reset(settle)
				}
			}
		case err, ok := <-watcher.Errors():
			if !ok {
				fmt.Fprintln(w, "warning: automatic table reload stopped: file error channel closed; restart trice log")
				return
			}
			fmt.Fprintf(w, "warning: automatic table reload: %v; rechecking tables\n", err)
			for _, table := range tables {
				table.pending = true
			}
			timer.Reset(settle)
		case <-timer.C:
			for _, table := range tables {
				if !table.pending {
					continue
				}
				if err := table.reload(); err != nil {
					if message := err.Error(); message != table.lastError {
						fmt.Fprintf(w, "warning: cannot reload %s: %s; keeping last valid table, will retry\n", table.path, message)
						table.lastError = message
					}
					timer.Reset(250 * time.Millisecond)
					continue
				}
				table.pending, table.lastError = false, ""
				if verbose {
					fmt.Fprintf(w, "Reloaded %s\n", table.path)
				}
			}
		}
	}
}
