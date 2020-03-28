// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// All rights reserved.
// Use of this source code is governed by a
// license that can be found in the LICENSE file.

// -build ignore

package lgf // logfile

import (
	"io"
	"log"
	"os"
)

// some references:
// https://stackoverflow.com/questions/54276178/golang-add-custom-os-file-to-os-stdout
// https://play.golang.org/p/CqUOP8aKL0
// https://gist.github.com/jmoiron/e9f72720cef51862b967
// https://medium.com/@hau12a1/golang-capturing-log-println-and-fmt-println-output-770209c791b4

// Name is the filename of the logfile. "off" inhibits logfile writing.
var Name = "off"

// log file handle
var lfHandle *os.File

// Tee is a writer all output should go to
var Tee io.Writer

var oldOut *os.File
var oldErr *os.File
var oldLog io.Writer

// Enable starts take notes mode, means parallel writing into a file
func Enable() {
	Tee = os.Stdout
	if "off" != Name {
		oldOut = os.Stdout
		oldErr = os.Stderr
		oldLog = log.Writer()
		var err error
		lfHandle, err = os.OpenFile(Name, os.O_RDWR|os.O_CREATE|os.O_APPEND, 0666)
		if err != nil {
			log.Fatalf("error opening file: %v", err)
		}
		Tee = io.MultiWriter(os.Stdout, lfHandle)
		log.SetOutput(Tee) // ok
		// THIS IS NOT POSSIBLE: :-( *os.File is usabla as io.Writer but io.Writer is not usable as *os.File )
		//os.Stdout = Tee
		//os.Stderr = Tee
	}
}

// Disable ends take notes mode, means parallel writing into a file
func Disable() {
	if nil != lfHandle {
		os.Stdout = oldOut
		os.Stderr = oldErr
		log.SetOutput(oldLog)
		lfHandle.Close()
		lfHandle = nil
	}
}
