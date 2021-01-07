// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package msg provides common message functions.
package msg

import (
	"fmt"
	"log"
	"runtime"
)

const (
	formatString = "[error] in %s[%s:%d] %v"
	seriousError = "[error] Could not recover caller information"
)

// OnErr prints a common error message with location info when err is not nil.
func OnErr(err error) {
	if nil == err {
		return
	}
	// notice that we're using 1, so it will actually log the where
	// the error happened, 0 = this function, we don't want that.
	pc, fn, line, ok := runtime.Caller(1)
	if ok {
		fmt.Printf(formatString, runtime.FuncForPC(pc).Name(), fn, line, err)
	} else {
		fmt.Printf(seriousError)
	}
}

// InfoOnErr prints info and a common error message with location info when err is not nil.
func InfoOnErr(info string, err error) {
	if nil == err {
		return
	}
	fmt.Println(info)
	// notice that we're using 1, so it will actually log the where
	// the error happened, 0 = this function, we don't want that.
	pc, fn, line, ok := runtime.Caller(1)
	if ok {
		fmt.Printf(formatString, runtime.FuncForPC(pc).Name(), fn, line, err)
	} else {
		fmt.Printf(seriousError)
	}
}

// FatalErr ends in osExit(1) if err not nil.
func FatalErr(err error) {
	if nil == err {
		return
	}
	// notice that we're using 1, so it will actually log the where
	// the error happened, 0 = this function, we don't want that.
	pc, fn, line, ok := runtime.Caller(1)
	if ok {
		log.Fatal(formatString, runtime.FuncForPC(pc).Name(), fn, line, err)
	} else {
		log.Fatal(seriousError)
	}
}

// InfoFatalErr ends in osExit(1) if err not nil.
func InfoFatalErr(info string, err error) {
	if nil == err {
		return
	}
	log.Println(info)
	// notice that we're using 1, so it will actually log the where
	// the error happened, 0 = this function, we don't want that.
	pc, fn, line, ok := runtime.Caller(1)
	if ok {
		log.Fatal(formatString, runtime.FuncForPC(pc).Name(), fn, line, err)
	} else {
		log.Fatal(seriousError)
	}
}

// PanicErr ends in panic if err not nil.
func PanicErr(err error) {
	if nil == err {
		return
	}
	panic(err)
}
