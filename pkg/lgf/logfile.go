// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package lgf // logfile

import (
	"io"
	"log"
	"os"

	"github.com/fatih/color"
)

// some references:
// https://stackoverflow.com/questions/54276178/golang-add-custom-os-file-to-os-stdout
// https://play.golang.org/p/CqUOP8aKL0
// https://gist.github.com/jmoiron/e9f72720cef51862b967
// https://medium.com/@hau12a1/golang-capturing-log-println-and-fmt-println-output-770209c791b4

// Name is the filename of the logfile. "off" inhibits logfile writing.
var Name = "off"

// Tee here only a helper for easy adaption between logfile0-2.go
//var Tee io.Writer = os.Stdout

var (
	oldOut, oldErr, lfHandle *os.File
	oldLog                   io.Writer
	err                      error
)

func prep() {
	if "off" == Name {
		return
	}
	oldOut = os.Stdout
	oldErr = os.Stderr
	oldLog = log.Writer()

	lfHandle, err = os.OpenFile(Name, os.O_RDWR|os.O_CREATE|os.O_APPEND, 0666)
	if err != nil {
		log.Fatalf("error opening file %s: %v", Name, err)
		Name = "off"
	}
}

func post() {
	if "off" == Name {
		return
	}
	os.Stdout = oldOut
	os.Stderr = oldErr
	log.SetOutput(oldLog)

	if nil != lfHandle {
		lfHandle.Close()
		lfHandle = nil
	}
}

// Enable starts take notes mode, means parallel writing into a file
func Enable() {
	prep()
	if "off" == Name {
		return
	}

	color.Output = io.MultiWriter(os.Stdout, lfHandle)
	color.Error = io.MultiWriter(os.Stderr, lfHandle)
	log.SetOutput(io.MultiWriter(oldLog, lfHandle))

	// THIS IS NOT POSSIBLE: :-( *os.File is usabla as io.Writer but io.Writer is not usable as *os.File )
	// teeOut := io.MultiWriter(os.Stdout, lfHandle)
	// os.Stdout = teeOut
	// TODO: quality teeOut as file interface
	// WORKAROUND: do not used fmt directly
}

// Disable ends take notes mode, means parallel writing into a file
func Disable() {
	post()
}
