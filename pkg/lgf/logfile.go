// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package lgf is for handling the logfile(s)
package lgf

import (
	"github.com/rokath/cage"
)

// some references:
// https://stackoverflow.com/questions/54276178/golang-add-custom-os-file-to-os-stdout
// https://play.golang.org/p/CqUOP8aKL0
// https://gist.github.com/jmoiron/e9f72720cef51862b967
// https://medium.com/@hau12a1/golang-capturing-log-println-and-fmt-println-output-770209c791b4

// Name is the filename of the logfile. "off" inhibits logfile writing.
var Name = "off"

var pContainer *cage.Container

// Enable starts take notes mode, means parallel writing into a file
func Enable() {
	pContainer = cage.Start(Name)
}

// Disable ends take notes mode, means parallel writing into a file
func Disable() {
	cage.Stop(pContainer)
}

/*
var (
	oldOut, oldErr, lfHandle *os.File
	oldLog                   io.Writer
	err                      error
	enabled                  = false
	rS, wS, rE, wE           *os.File
)

// Enable starts take notes mode, means parallel writing into a file
func Enable0() {
	if "off" == Name || true == enabled {
		return
	}
	defer func() {
		enabled = true
	}()

	oldOut = os.Stdout
	oldErr = os.Stderr
	oldLog = log.Writer()

	lfHandle, err = os.OpenFile(Name, os.O_RDWR|os.O_CREATE|os.O_APPEND, 0666)
	if err != nil {
		log.Fatalf("error opening file %s: %v", Name, err)
		Name = "off"
		return
	}

	log.SetOutput(io.MultiWriter(oldLog, lfHandle))

	color.Output = io.MultiWriter(os.Stdout, lfHandle)
	color.Error = io.MultiWriter(os.Stderr, lfHandle)
	//os.Stdout = io.MultiWriter(oldOut, lfHandle).(*os.File) // type assertion used but: panic: interface conversion: io.Writer is *io.multiWriter, not *os.File
	//os.Stderr = io.MultiWriter(oldErr, lfHandle).(*os.File) // type assertion used but: panic: interface conversion: io.Writer is *io.multiWriter, not *os.File

}

// Disable ends take notes mode, means parallel writing into a file
func Disable0() {
	if false == enabled {
		return
	}
	defer func() {
		enabled = false
	}()

	os.Stdout = oldOut
	os.Stderr = oldErr
	log.SetOutput(oldLog)

	if nil != lfHandle {
		lfHandle.Close()
		lfHandle = nil
	}
}

// Enable2 starts take notes mode, means parallel writing into a file
func Enable2() {
	if "off" == Name || true == enabled {
		return
	}
	defer func() {
		enabled = true
	}()

	oldOut = os.Stdout
	oldErr = os.Stderr
	oldLog = log.Writer()

	lfHandle, err = os.OpenFile(Name, os.O_RDWR|os.O_CREATE|os.O_APPEND, 0666)
	if err != nil {
		log.Fatalf("error opening file %s: %v", Name, err)
		Name = "off"
		return
	}

	// tee for log
	log.SetOutput(io.MultiWriter(oldLog, lfHandle))

	// tee for stdout
	rS, wS, err = os.Pipe()
	if err != nil {
		panic(err)
	}
	doneS := make(chan error, 1)
	os.Stdout = wS
	stdout := io.MultiWriter(os.Stdout, lfHandle)
	go func() {
		_, err := io.Copy(stdout, rS)
		if nil != err {
			doneS <- err
		}
	}()

	// tee for stderr
	rE, wE, err = os.Pipe()
	if err != nil {
		panic(err)
	}
	doneE := make(chan error, 1)
	os.Stderr = wE
	stderr := io.MultiWriter(os.Stderr, lfHandle)
	go func() {
		_, err := io.Copy(stderr, rE)
		if nil != err {
			doneE <- err
		}
	}()
}

// Disable ends take notes mode, means parallel writing into a file
func Disable2() {
	if false == enabled {
		return
	}
	defer func() {
		enabled = false
	}()

	os.Stdout = oldOut
	os.Stderr = oldErr
	log.SetOutput(oldLog)

	if nil != lfHandle {
		lfHandle.Close()
		lfHandle = nil
	}

	rS.Close()
	rE.Close()

}
*/
