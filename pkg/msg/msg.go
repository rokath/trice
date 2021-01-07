// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package msg provides common message functions.
package msg

import (
	"fmt"
	"log"
	"path/filepath"
	"runtime"
)

// OnErr prints a common error message with location info when err is not nil.
func OnErr(err error) {
	if nil == err {
		return
	}
	pc, fn, line, ok := runtime.Caller(1)
	fmtMessage(pc, fn, line, ok, err)
}

// InfoOnErr prints info and a common error message with location info when err is not nil.
func InfoOnErr(info string, err error) {
	if nil == err {
		return
	}
	fmt.Println(info)
	pc, fn, line, ok := runtime.Caller(1)
	fmtMessage(pc, fn, line, ok, err)
}

// FatalErr ends in osExit(1) if err not nil.
func FatalErr(err error) {
	if nil == err {
		return
	}
	pc, fn, line, ok := runtime.Caller(1)
	logMessage(pc, fn, line, ok, err)
}

// InfoFatalErr ends in osExit(1) if err not nil.
func InfoFatalErr(info string, err error) {
	if nil == err {
		return
	}
	log.Println(info)
	pc, fn, line, ok := runtime.Caller(1)
	logMessage(pc, fn, line, ok, err)
}

// PanicErr ends in panic if err not nil.
func PanicErr(err error) {
	if nil == err {
		return
	}
	panic(err)
}

const (
	formatString = "[error] in %s[%s:%d] %v"
	seriousError = "[error] Could not recover caller information"
)

func fmtMessage(pc uintptr, fn string, line int, ok bool, err error) {
	funcName := runtime.FuncForPC(pc).Name()
	fileName := filepath.Base(fn)
	if ok {
		fmt.Printf(formatString, funcName, fileName, line, err)
	} else {
		fmt.Printf(seriousError)
	}
}

func logMessage(pc uintptr, fn string, line int, ok bool, err error) {
	funcName := runtime.FuncForPC(pc).Name()
	fileName := filepath.Base(fn)
	if ok {
		log.Fatal(formatString, funcName, fileName, line, err)
	} else {
		log.Fatal(seriousError)
	}
}
