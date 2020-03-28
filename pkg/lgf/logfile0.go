// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// All rights reserved.
// Use of this source code is governed by a
// license that can be found in the LICENSE file.

// +build lf0
// `go build -tags lf0` does include this file

package lgf // logfile

import (
	"io"
	"log"
	"os"
)

// Enable starts take notes mode, means parallel writing into a file
func Enable() {
	prep()
	if "off" == Name {
		return
	}
	lfHandle, err = os.OpenFile(Name, os.O_RDWR|os.O_CREATE|os.O_APPEND, 0666)
	if err != nil {
		log.Fatalf("error opening file: %v", err)
	}
	Tee = io.MultiWriter(os.Stdout, lfHandle)
	log.SetOutput(Tee) // ok
	// THIS IS NOT POSSIBLE: :-( *os.File is usabla as io.Writer but io.Writer is not usable as *os.File )
	//os.Stdout = Tee
	//os.Stderr = Tee
	// WORKAROUND: Fprint instead of Print, but than remoteDisplay shows no colors
}

// Disable ends take notes mode, means parallel writing into a file
func Disable() {
	post()
}
