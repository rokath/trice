// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

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

// FileWatcher checks the id list file for changes.
// taken from https://medium.com/@skdomino/watch-this-file-watching-in-go-5b5a247cf71f
func (lu TriceIDLookUp) FileWatcher(w io.Writer, fSys *afero.Afero, m *sync.RWMutex) {

	if Verbose {
		fmt.Fprintln(w, "Create a new file watcher for TIL.")
	}
	watcher, err := fsnotify.NewWatcher()
	msg.FatalOnErr(err)
	defer func() {
		if Verbose {
			fmt.Fprintln(w, "Closing file watcher TIL.")
		}
		msg.OnErr(watcher.Close())
	}()

	done := make(chan bool)
	go func() {
		if Verbose {
			fmt.Fprintln(w, "TIL Watching...")
		}
		var now, last time.Time
		for {
			select {
			// watch for events
			case event, ok := <-watcher.Events:
				if Verbose {
					fmt.Fprintln(w, "TIL EVENT:", event, ok, time.Now().UTC())
				}
				now = time.Now()
				diff := now.Sub(last)
				if diff > 5000*time.Millisecond {
					fmt.Fprintln(w, "Refreshing id.List.")
					time.Sleep(100 * time.Millisecond) // When refreshing too fast, FnJSON seems to be empty.
					m.Lock()
					msg.FatalOnErr(lu.fromFile(fSys, FnJSON))
					lu.AddFmtCount(w)
					m.Unlock()
					last = time.Now()
				}

			// watch for errors
			case err := <-watcher.Errors:
				fmt.Fprintln(w, "TIL ERROR1", err, time.Now().UTC())
			}
		}
	}()

	if Verbose {
		fmt.Fprintln(w, "Out of the box fsnotify can watch a single file, or a single directory (TIL).")
	}
	msg.InfoOnErr(watcher.Add(FnJSON), "ERROR2")
	if Verbose {
		fmt.Fprintln(w, FnJSON, "watched now for changes")
	}
	<-done
}

// FileWatcher checks the id location information file for changes.
// taken from https://medium.com/@skdomino/watch-this-file-watching-in-go-5b5a247cf71f
func (li TriceIDLookUpLI) FileWatcher(w io.Writer, fSys *afero.Afero) {

	if Verbose {
		fmt.Fprintln(w, "Create a new file watcher for LI.")
	}
	watcher, err := fsnotify.NewWatcher()
	msg.FatalOnErr(err)
	defer func() {
		if Verbose {
			fmt.Fprintln(w, "Closing file watcher LI.")
		}
		msg.OnErr(watcher.Close())
	}()

	done := make(chan bool)
	go func() {
		if Verbose {
			fmt.Fprintln(w, "LI Watching...")
		}
		var now, last time.Time
		for {
			select {
			// watch for events
			case event, ok := <-watcher.Events:
				if Verbose {
					fmt.Fprintln(w, "LI EVENT:", event, ok, time.Now().UTC())
				}
				now = time.Now()
				diff := now.Sub(last)
				if diff > 5000*time.Millisecond {
					fmt.Fprintln(w, "Refreshing li list.")
					time.Sleep(100 * time.Millisecond) // When refreshing too fast, LIFnJSON seems to be empty.
					msg.FatalOnErr(li.fromFile(fSys, LIFnJSON))
					last = time.Now()
				}

			// watch for errors
			case err := <-watcher.Errors:
				fmt.Fprintln(w, "LI ERROR1", err, time.Now().UTC())
			}
		}
	}()

	if Verbose {
		fmt.Fprintln(w, "Out of the box fsnotify can watch a single file, or a single directory (LI).")
	}
	msg.InfoOnErr(watcher.Add(LIFnJSON), "ERROR2")
	if Verbose {
		fmt.Fprintln(w, LIFnJSON, "watched now for changes")
	}
	<-done
}
