// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// All rights reserved.
// Use of this source code is governed by a
// license that can be found in the LICENSE file.

// +build lf1
// `go build -tags lf1` does include this file

package lgf

// list management

import (
	"bytes"
	"io"
	"log"
	"os"
)

var (
	stdoutWriter, stdoutReader *os.File
	stderrWriter, stderrReader *os.File
	logWriter, logReader       *os.File
)

// Enable starts take notes mode, means parallel writing into a file
func Enable() {
	prep()
	if "off" == Name {
		return
	}
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
	Tee = stdoutWriter // for logfile0-2.go compability
	os.Stderr = stderrWriter
	log.SetOutput(logWriter)

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

// Disable ends take notes mode, means parallel writing into a file
func Disable() {
	if "off" == Name {
		return
	}
	stdoutWriter.Close() // does this kill the go routines or does it need more actions?
	stderrWriter.Close()
	logWriter.Close()
	post()
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
	re := captureOutput(func() {
		err = scVersion0(lfn)
	})
	old, lfHandle := setTee(lfn)
	fmt.Fprint(tee, re)
	resetTee(old, lfHandle)
	return err
}
*/
