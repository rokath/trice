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

// InfoOnErr prints info and a common error message with location info when err is not nil.
func InfoOnErr(info string, err error) {
	if nil == err {
		return
	}
	fmt.Println(info)
	pc, fn, line, ok := runtime.Caller(1)
	fmtMessage(pc, fn, line, ok, err)
}

// FatalOnErr ends in osExit(1) if err not nil.
func FatalOnErr(info string, err error) {
	if nil == err {
		return
	}
	log.Println(info)
	pc, fn, line, ok := runtime.Caller(1)
	logMessage(pc, fn, line, ok, err)
}

// PanicOnErr ends in panic if err not nil.
func PanicOnErr(info string, err error) {
	if nil == err {
		return
	}
	log.Println(info)
	panic(err)
}

// InfoOnFalse prints info and a common error message with location info when flag is false.
func InfoOnFalse(info string, flag bool) {
	if true == flag {
		return
	}
	pc, fn, line, ok := runtime.Caller(1)
	fmtMessage(pc, fn, line, ok, errors.New(info))
}

// InfoOnTrue prints info and a common error message with location info when flag is true.
func InfoOnTrue(info string, flag bool) {
	if false == flag {
		return
	}
	pc, fn, line, ok := runtime.Caller(1)
	fmtMessage(pc, fn, line, ok, errors.New(info))
}

// FatalOnFalse prints info and a common error message with location info when err is not nil.
func FatalOnFalse(info string, flag bool) {
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
