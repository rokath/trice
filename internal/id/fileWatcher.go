// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

import (
	"fmt"
	"sync"
	"time"

	"github.com/fsnotify/fsnotify"
	"github.com/rokath/trice/pkg/msg"
)

// FileWatcher checks id List file for changes
// taken from https://medium.com/@skdomino/watch-this-file-watching-in-go-5b5a247cf71f
func (lu TriceIDLookUp) FileWatcher(m *sync.RWMutex) {

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
				fmt.Println("EVENT:", event, ok, time.Now().UTC())

				now = time.Now()
				diff := now.Sub(last)
				if diff > 5000*time.Millisecond {
					fmt.Println("refreshing id.List")
					m.Lock()
					lu.fromFile(FnJSON)
					m.Unlock()
					last = time.Now()
				}

			// watch for errors
			case err := <-watcher.Errors:
				fmt.Println("ERROR1", err, time.Now().UTC())
			}
		}
	}()

	// out of the box fsnotify can watch a single file, or a single directory
	msg.InfoOnErr(watcher.Add(FnJSON), "ERROR2")
	if Verbose {
		fmt.Println(FnJSON, "watched now for changes")
	}
	<-done
}
