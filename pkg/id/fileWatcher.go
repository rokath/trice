// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

import (
	"fmt"
	"time"

	"github.com/fsnotify/fsnotify"
)

// ListFileWatcher checks id list file for changes
// taken from https://medium.com/@skdomino/watch-this-file-watching-in-go-5b5a247cf71f
func (p *ListT) FileWatcher() {

	// creates a new file watcher
	watcher, err := fsnotify.NewWatcher()
	if err != nil {
		fmt.Println("ERROR0", err)
	}
	defer watcher.Close()

	done := make(chan bool)
	go func() {
		var cnt int
		var now, last time.Time
		for {
			select {
			// watch for events
			case <-watcher.Events:
				//fmt.Printf("EVENT! %#v\n", event)

				now = time.Now()
				diff := now.Sub(last)
				if diff > 1000*time.Millisecond {
					fmt.Println("renew id.List")
					cnt++
					renewIDList()
					last = time.Now()
				}

			// watch for errors
			case err := <-watcher.Errors:
				fmt.Println("ERROR1", err)
			}
		}
	}()

	// out of the box fsnotify can watch a single file, or a single directory
	if err := watcher.Add(FnJSON); err != nil {
		fmt.Println("ERROR2", err)
	}
	fmt.Println(FnJSON, " watched now for changes")
	<-done
}

// renewIDList clears the internal id list and reads it again
func renewIDList() {
	if "none" != FnJSON {
		List = List[:0]
		List.Read(FnJSON)
	}
}
