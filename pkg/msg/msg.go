// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package msg provides common message functions.
package msg

import (
	"errors"
	"fmt"
	"io"
	"log"
	"os"
	"path/filepath"
	"runtime"
	"sync"
)

// Info prints info with location info.
func Info(info string) {
	pc, fn, line, ok := runtime.Caller(1)
	fmtMessage(pc, fn, line, ok, errors.New(info))
}

// OnErrF prints info and a common error message with location info when err is not nil.
func OnErrF(w io.Writer, err error) {
	if nil == err {
		return
	}
	pc, fn, line, ok := runtime.Caller(1)
	fmtFMessage(w, pc, fn, line, ok, err)
}

// OnErr prints info and a common error message with location info when err is not nil.
func OnErr(err error) {
	if nil == err {
		return
	}
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

// InfoOnErr prints info and a common error message with location info when err is not nil.
func InfoOnErr(err error, info string) {
	if nil == err {
		return
	}
	fmt.Println(info)
	pc, fn, line, ok := runtime.Caller(1)
	fmtMessage(pc, fn, line, ok, err)
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

// OnTrue

// OnTrue prints info and a common error message with location info when flag is true.
func OnTrue(flag bool) {
	if !flag {
		return
	}
	pc, fn, line, ok := runtime.Caller(1)
	fmtMessage(pc, fn, line, ok, nil)
}

// FatalOnTrue ends in osExit(1) if flag is true.
func FatalOnTrue(flag bool) {
	if !flag {
		return
	}
	pc, fn, line, ok := runtime.Caller(1)
	logMessage(pc, fn, line, ok, nil)
}

// InfoOnTrue

// InfoOnTrue prints info and a common error message with location info when flag is true.
func InfoOnTrue(flag bool, info string) {
	if !flag {
		return
	}
	pc, fn, line, ok := runtime.Caller(1)
	fmtMessage(pc, fn, line, ok, errors.New(info))
}

// FatalInfoOnTrue prints info and a common error message with location info when err is not nil.
func FatalInfoOnTrue(flag bool, info string) {
	if !flag {
		return
	}
	fmt.Println(info)
	pc, fn, line, ok := runtime.Caller(1)
	logMessage(pc, fn, line, ok, errors.New(info))
}

// OnFalse

// OnFalse prints info and a common error message with location info when flag is false.
func OnFalse(flag bool) {
	if flag {
		return
	}
	pc, fn, line, ok := runtime.Caller(1)
	fmtMessage(pc, fn, line, ok, nil)
}

// FatalOnFalse ends in osExit(1) if flag is false.
func FatalOnFalse(flag bool) {
	if flag {
		return
	}
	pc, fn, line, ok := runtime.Caller(1)
	logMessage(pc, fn, line, ok, nil)
}

// InfoOnFalse prints info and a common error message with location info when flag is false.
func InfoOnFalse(flag bool, info string) {
	if flag {
		return
	}
	pc, fn, line, ok := runtime.Caller(1)
	fmtMessage(pc, fn, line, ok, errors.New(info))
}

// FatalInfoOnFalse prints info and a common error message with location info when err is not nil.
func FatalInfoOnFalse(flag bool, info string) {
	if flag {
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

var (
	logFatalf = log.Fatalf // https://stackoverflow.com/questions/30688554/how-to-test-go-function-containing-log-fatal/45380105
)

func fmtFMessage(w io.Writer, pc uintptr, fn string, line int, ok bool, err error) {
	funcName := runtime.FuncForPC(pc).Name()
	fileName := filepath.Base(fn)
	if ok {
		fmt.Fprintf(w, formatString+"\n", fileName, line, funcName, err)
	} else {
		fmt.Fprintln(w, seriousError)
	}
}

func fmtMessage(pc uintptr, fn string, line int, ok bool, err error) {
	fmtFMessage(os.Stdout, pc, fn, line, ok, err)
}

func logMessage(pc uintptr, fn string, line int, ok bool, err error) {
	funcName := runtime.FuncForPC(pc).Name()
	fileName := filepath.Base(fn)
	if ok {
		logFatalf(formatString, fileName, line, funcName, err)
	} else {
		logFatalf("%s", seriousError)
	}
}

// -----------------------------------------------

type OrigLogFatalf func(format string, v ...interface{})

var m *sync.RWMutex

func init() {
	m = new(sync.RWMutex)
}

// OsExitDisallow replace the original fatal function
func OsExitDisallow() (o OrigLogFatalf) {
	m.Lock()
	o = logFatalf

	logFatalf = func(format string, args ...interface{}) {
		if len(args) > 0 {
			fmt.Printf(format, args)
		} else {
			fmt.Print(format)
		}
	}
	return
}

// OsExitAllow place the original fatal function back
func OsExitAllow(o OrigLogFatalf) {
	logFatalf = o
	m.Unlock()
}
