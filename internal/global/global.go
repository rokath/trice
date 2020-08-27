// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package global holds global values and functions used in several internal packages.
//
// Including package main in other packages will result in aforbidden import cycle.
// Therefore package global serves as glue.
package global

/*
import (
	"log"
	"os"
	"runtime"
)

var (
	// Verbose gives additional informal output
	Verbose bool // init value is 'false'

	// OsExit is a function pointer to os.Exit.
	//
	// For unit tests this value can be changed to avoid leaving the test function immediately.
	// This is good for code under your control.
	//
	// To test code calling os.Exit() implicitely, like when log.Fatal() is used,
	// see https://stackoverflow.com/questions/26225513/how-to-test-os-exit-scenarios-in-go.
	OsExit func(code int) = os.Exit

	// Source is the trice receiver to use
	Source string

	// DestinationAddr is local trice address, used for routing in distributed systems.
	DestinationAddr = byte(0x60)

	// SourceAddr is remote trice address, used for routing in distributed systems.
	SourceAddr = byte(0x60)
)

// EvaluateWrap checks if the wrap in b contains valid trice header data.
//
// It returns true on success, otherwise false.
func EvaluateWrap(b []byte) bool {
	x := 0xc0 == b[0] && // start byte
		b[1] == SourceAddr && // todo remAddr
		b[2] == DestinationAddr && // todo locAddr
		b[3] == b[0]^b[1]^b[2]^b[4]^b[5]^b[6]^b[7] // crc8
	return x
}

// Fatal ends in osExit(1) if err not nil.
func Fatal(err error) {
	if nil == err {
		return
	}
	// notice that we're using 1, so it will actually log the where
	// the error happened, 0 = this function, we don't want that.
	pc, fn, line, _ := runtime.Caller(1)
	log.Fatalf("[error] in %s[%s:%d] %v", runtime.FuncForPC(pc).Name(), fn, line, err)
}

// Check returns true if err not nil.
func Check(err error) (b bool) {
	if nil == err {
		return
	}
	// notice that we're using 1, so it will actually log the where
	// the error happened, 0 = this function, we don't want that.
	pc, fn, line, _ := runtime.Caller(1)
	log.Printf("[error] in %s[%s:%d] %v", runtime.FuncForPC(pc).Name(), fn, line, err)
	b = true
	return
}
*/
