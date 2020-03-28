// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// All rights reserved.
// Use of this source code is governed by a
// license that can be found in the LICENSE file.

// +build ignore

package lgf // logfile

import (
	"bytes"
	"fmt"
	"io"
	"log"
	"os"
	"sync"
)

var wg sync.WaitGroup

// used for restoring normal state
var quitTakeNotes chan bool

// Name is the filename of the logfile. "off" inhibits logfile writing.
var Name = "off"

// log file handle
var lfHandle *os.File

// Tee here only a helper for easy adaption to logfile1.go
var Tee *os.File

// Enable starts take notes mode, means parallel writing into a file
func Enable() {
	quitTakeNotes = make(chan bool)
	if "off" == Name {
		return
	}
	var err error
	lfHandle, err = os.OpenFile(Name, os.O_RDWR|os.O_CREATE|os.O_APPEND, 0666)
	if err != nil {
		log.Fatalf("error opening file: %v", err)
	}
	old := os.Stderr
	wg.Add(1)
	go func() {
		for {
			select {
			case <-quitTakeNotes: // back to normal state
				log.SetOutput(old)
				wg.Done()
				return
			}
		}
	}()
	takeNotes(&os.Stdout)
	Tee = os.Stdout // the new one
	takeNotes(&os.Stderr)
	n := os.Stderr
	log.SetOutput(n)
}

// Disable ends take notes mode, means parallel writing into a file
func Disable() {
	close(quitTakeNotes) // this is a multicast to all go routines listening this channel
	Tee = os.Stdout      // the old one
	wg.Wait()
}

// see also https://play.golang.org/p/PNqa5M8zo7

// takeNotes redirects output file handle out to a writer which goes into a pipe.
// It starts an endless go routine which c reads from the pipe and writes to
// the out file but also to the logfile
func takeNotes(out **os.File) {
	r, w, err := os.Pipe()
	if err != nil {
		panic(err)
	}
	old := *out
	*out = w
	//outC := make(chan string)

	wg.Add(1) // must be before go routine starts
	go func() {
		for {
			select {
			case <-quitTakeNotes: // back to normal state
				*out = old
				w.Close()
				wg.Done()
				return
			default:
				// copy the output in a separate goroutine so printing can't block indefinitely
				var buf bytes.Buffer
				n, e := io.Copy(&buf, r) // ISSUE: it seems io.Copy never returns, even after writing to w!
				var s string
				if nil != e {
					s = fmt.Sprint("io.Copy error", e, n)
				}
				s = buf.String()
				//outC <- s // this was just an additional trial, but probably senseless
				io.WriteString(old, s)
				io.WriteString(lfHandle, s)
			}
		}
	}()
	/*	wg.Add(1) // must be before go routine starts
		go func() {
			for {
				select {
				case <-quitTakeNotes:
					close(outC)
					wg.Done()
					return
				case s := <-outC:
					io.WriteString(old, s)
					io.WriteString(lfHandle, s)
				}
			}
		}()*/
}
