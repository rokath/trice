// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// All rights reserved.
// Use of this source code is governed by a
// license that can be found in the LICENSE file.

// +build ignore

package lgf

// list management

import (
	"bytes"
	"io"
	"log"
	"os"
)

// Name is the filename of the logfile. "off" inhibits logfile writing.
var Name = "off"

// Tee here only a helper for easy adaption to logfile1.go
var Tee = os.Stdout

var (
	lfHandle                   *os.File // log file handle
	oldOut                     *os.File
	oldErr                     *os.File
	oldLog                     io.Writer
	stdoutWriter, stdoutReader *os.File
	stderrWriter, stderrReader *os.File
	logWriter, logReader       *os.File
)

// Enable starts take notes mode, means parallel writing into a file
func Enable() {
	Tee = os.Stdout
	var err error
	if "off" != Name {
		oldOut = os.Stdout
		oldErr = os.Stderr
		oldLog = log.Writer()

		stdoutReader, stdoutWriter, err = os.Pipe()
		if err != nil {
			panic(err)
		}
		stderrReader, stderrWriter, err = os.Pipe()
		if err != nil {
			panic(err)
		}
		logReader, logWriter, err = os.Pipe()
		if err != nil {
			panic(err)
		}

		os.Stdout = stdoutWriter
		Tee = stdoutWriter // for logfile1.go compability
		os.Stderr = stderrWriter
		log.SetOutput(logWriter)

		lfHandle, err = os.OpenFile(Name, os.O_RDWR|os.O_CREATE|os.O_APPEND, 0666)
		if err != nil {
			log.Fatalf("error opening file: %v", err)
		}

		go func() {
			var buf bytes.Buffer
			io.Copy(&buf, stdoutReader)
			io.WriteString(oldOut, buf.String())
			io.WriteString(lfHandle, buf.String())
		}()

		go func() {
			var buf bytes.Buffer
			io.Copy(&buf, stderrReader)
			io.WriteString(oldErr, buf.String())
			io.WriteString(lfHandle, buf.String())
		}()

		go func() {
			var buf bytes.Buffer
			io.Copy(&buf, logReader)
			io.WriteString(oldLog, buf.String())
			io.WriteString(lfHandle, buf.String())
		}()
	}
}

// Disable ends take notes mode, means parallel writing into a file
func Disable() {
	if nil != lfHandle {
		Tee = oldOut
		os.Stdout = oldOut
		os.Stderr = oldErr
		log.SetOutput(oldLog)

		stdoutWriter.Close() // does this kill the go routines or does it need more actions?
		stderrWriter.Close()
		logWriter.Close()

		lfHandle.Close()
		lfHandle = nil
	}
}

/*
// https://medium.com/@hau12a1/golang-capturing-log-println-and-fmt-println-output-770209c791b4
func captureOutput(f func()) string {
	reader, writer, err := os.Pipe()
	if err != nil {
		panic(err)
	}
	stdout := os.Stdout
	stderr := os.Stderr
	defer func() {
		os.Stdout = stdout
		os.Stderr = stderr
		log.SetOutput(os.Stderr)
	}()
	os.Stdout = writer
	os.Stderr = writer
	log.SetOutput(writer)
	out := make(chan string)
	wg := new(sync.WaitGroup)
	wg.Add(1)
	go func() {
		var buf bytes.Buffer
		wg.Done()
		io.Copy(&buf, reader)
		out <- buf.String()
	}()
	wg.Wait()
	f()
	writer.Close()
	return <-out
}

func scVersion1(lfn string) error {
	var err error
	re := captureOutput(func() {
		err = scVersion0(lfn)
	})
	old, lfHandle := setTee(lfn)
	fmt.Fprint(tee, re)
	resetTee(old, lfHandle)
	return err
}
*/
