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
)

// FileWatcher checks id List file for changes
// taken from https://medium.com/@skdomino/watch-this-file-watching-in-go-5b5a247cf71f
func (lu TriceIDLookUp) FileWatcher(w io.Writer, m *sync.RWMutex) {

	// creates a new file watcher
	watcher, err := fsnotify.NewWatcher()
	msg.FatalOnErr(err)
	defer func() { msg.OnErr(watcher.Close()) }()

	done := make(chan bool)
	go func() {
		var now, last time.Time
		for {
			select {
			// watch for events
			case event, ok := <-watcher.Events:
				fmt.Fprintln(w, "EVENT:", event, ok, time.Now().UTC())

				now = time.Now()
				diff := now.Sub(last)
				if diff > 5000*time.Millisecond {
					fmt.Fprintln(w, "refreshing id.List")
					m.Lock()
					msg.FatalOnErr(lu.fromFile(FnJSON))
					lu.AddFmtCount(w)
					m.Unlock()
					last = time.Now()
				}

			// watch for errors
			case err := <-watcher.Errors:
				fmt.Fprintln(w, "ERROR1", err, time.Now().UTC())
			}
		}
	}()

	// out of the box fsnotify can watch a single file, or a single directory
	msg.InfoOnErr(watcher.Add(FnJSON), "ERROR2")
	if Verbose {
		fmt.Fprintln(w, FnJSON, "watched now for changes")
	}
	<-done
}
