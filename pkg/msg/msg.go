// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package msg provides common message functions.
package msg

import (
	"errors"
	"fmt"
	"log"
	"path/filepath"
	"runtime"
)

// Info prints info with location info.
func Info(info string) {
	pc, fn, line, ok := runtime.Caller(1)
	fmtMessage(pc, fn, line, ok, errors.New(info))
}

// OnErr prints info and a common error message with location info when err is not nil.
func OnErr(err error) {
	if nil == err {
		return
	}
	pc, fn, line, ok := runtime.Caller(1)
	fmtMessage(pc, fn, line, ok, err)
}

// InfoOnErr prints info and a common error message with location info when err is not nil.
func InfoOnErr(err error, info string) {
	if nil == err {
		return
	}
	fmt.Println(info)
	pc, fn, line, ok := runtime.Caller(1)
	fmtMessage(pc, fn, line, ok, err)
}

// FatalOnErr ends in osExit(1) if err not nil.
func FatalOnErr(err error) {
	if nil == err {
		return
	}
	pc, fn, line, ok := runtime.Caller(1)
	logMessage(pc, fn, line, ok, err)
}

// FatalOnTrue ends in osExit(1) if flag is true.
func FatalOnTrue(flag bool) {
	if false == flag {
		return
	}
	pc, fn, line, ok := runtime.Caller(1)
	logMessage(pc, fn, line, ok, nil)
}

// FatalInfoOnErr ends in osExit(1) if err not nil.
func FatalInfoOnErr(err error, info string) {
	if nil == err {
		return
	}
	log.Println(info)
	pc, fn, line, ok := runtime.Caller(1)
	logMessage(pc, fn, line, ok, err)
}

// PanicOnErr ends in panic if err not nil.
func PanicOnErr(err error, info string) {
	if nil == err {
		return
	}
	log.Println(info)
	panic(err)
}

// OnFalse prints info and a common error message with location info when flag is false.
func OnFalse(flag bool) {
	if true == flag {
		return
	}
	pc, fn, line, ok := runtime.Caller(1)
	fmtMessage(pc, fn, line, ok, nil)
}

// OnTrue prints info and a common error message with location info when flag is true.
func OnTrue(flag bool) {
	if false == flag {
		return
	}
	pc, fn, line, ok := runtime.Caller(1)
	fmtMessage(pc, fn, line, ok, nil)
}

// InfoOnFalse prints info and a common error message with location info when flag is false.
func InfoOnFalse(flag bool, info string) {
	if true == flag {
		return
	}
	pc, fn, line, ok := runtime.Caller(1)
	fmtMessage(pc, fn, line, ok, errors.New(info))
}

// InfoOnTrue prints info and a common error message with location info when flag is true.
func InfoOnTrue(flag bool, info string) {
	if false == flag {
		return
	}
	pc, fn, line, ok := runtime.Caller(1)
	fmtMessage(pc, fn, line, ok, errors.New(info))
}

// FatalOnFalse prints info and a common error message with location info when err is not nil.
func FatalOnFalse(flag bool, info string) {
	if true == flag {
		return
	}
	fmt.Println(info)
	pc, fn, line, ok := runtime.Caller(1)
	logMessage(pc, fn, line, ok, errors.New(info))
}

const (
	formatString = "Error in %s:%d: func '%s' -> %v"
	seriousError = "Error: Could not recover caller information."
)

func fmtMessage(pc uintptr, fn string, line int, ok bool, err error) {
	funcName := runtime.FuncForPC(pc).Name()
	fileName := filepath.Base(fn)
	if ok {
		fmt.Printf(formatString, fileName, line, funcName, err)
	} else {
		fmt.Printf(seriousError)
	}
}

func logMessage(pc uintptr, fn string, line int, ok bool, err error) {
	funcName := runtime.FuncForPC(pc).Name()
	fileName := filepath.Base(fn)
	if ok {
		log.Fatalf(formatString, fileName, line, funcName, err)
	} else {
		log.Fatalf(seriousError)
	}
}
