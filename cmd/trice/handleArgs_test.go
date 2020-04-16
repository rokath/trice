// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package main

import (
	"log"
	"os"
	"testing"
	"time"

	"github.com/rokath/trice/pkg/disp"
	"github.com/rokath/trice/pkg/lgf"
	"github.com/rokath/trice/pkg/lib"
	"github.com/rokath/trice/pkg/trice"
)

func ExampleHandleArgs_none() {
	HandleArgs([]string{"trice", ""})
	// Output:
	// try: 'trice help|h'
}

func ExampleHandleArgs_wrongSubcommand() {
	HandleArgs([]string{"trice", "xyz"})
	// Output:
	// try: 'trice help|h'
}

func ExampleHandleArgs_logListNotFound() {
	HandleArgs([]string{"trice", "log", "-list", "xxx.json", "-port", "COMscan", "-lf", "off"})
	// xOutput:
	// ID list C:\GitRepos\trice\cmd\trice\xxx.json not found, exit
	// ERROR GetFileAttributes: The system cannot find the file specified.
}

func ExampleHandleArgs_logCOM0() {
	HandleArgs([]string{"trice", "log", "-list", "none", "-port", "COM0"})
	// Output:
	// id list file none with 0 items
	// Error: Could not open serial port: Serial port not found
	// Could not set up serial port COM0
	// try -port COMscan
}

func ExampleHandleArgs_logNoParam() {
	HandleArgs([]string{"trice", "log", "", ""})
	//xOutput:
	// -baud int
	// 	COM baudrate (optional, default is 38400 (default 38400)
	// -list string
	// 	ID list path (optional) (default "./idlist.json")
	// -port string
	// 	subcommand (required, try COMscan)
}

func ExampleHandleArgs_logWrongParam() {
	HandleArgs([]string{"trice", "log", "-x", "y"})
	// xOutput: flag provided but not defined: -x
	// Usage of log:
	//  -baud int
	//        COM baudrate (optional, default is 38400 (default 38400)
	//  -port string
	//        subcommand (required, try COMscan)
}

func TestScVersion(t *testing.T) {
	afn := "testdata/actVersion.log"
	efn := "testdata/expVersion.log"
	os.Remove(afn)
	args := []string{"trice", "version", "-lf", afn}
	log.SetFlags(0)
	err := HandleArgs(args)
	lib.Ok(t, err)

	lib.CleanFile(afn)
	lib.CleanFile(efn)

	lib.EqualFiles(t, afn+".clean", efn+".clean")
	err = os.Remove(afn)
	lib.Ok(t, err)
	err = os.Remove(afn + ".clean")
	lib.Ok(t, err)
	err = os.Remove(efn + ".clean")
	lib.Ok(t, err)
}

func TestScHelp(t *testing.T) {
	afn := "testdata/actHelp.log"
	efn := "testdata/expHelp.log"
	os.Remove(afn)
	args := []string{"trice", "help", "-lf", afn}
	err := HandleArgs(args)
	lib.Ok(t, err)

	lib.CleanFile(afn)
	lib.CleanFile(efn)

	lib.EqualFiles(t, afn+".clean", efn+".clean")
	err = os.Remove(afn)
	lib.Ok(t, err)
	err = os.Remove(afn + ".clean")
	lib.Ok(t, err)
	err = os.Remove(efn + ".clean")
	lib.Ok(t, err)
}

func xTestScDisplayServer(t *testing.T) {
	afn := "testdata/actDisplayServer.log"
	efn := "testdata/expDisplayServer.log"
	os.Remove(afn)
	//done := make(chan bool, 1)

	log.SetFlags(0)
	go func() {
		time.Sleep(3000 * time.Millisecond)
		err := disp.Connect()
		lib.Ok(t, err)
		disp.StopServer() // calls os.Exit(0)
		//done <- true
	}()

	args := []string{"trice", "ds", "-lf", afn}
	HandleArgs(args)
	//<-done
	lib.CleanFile(afn)
	lib.CleanFile(efn)

	lib.EqualFiles(t, afn+".clean", efn+".clean")
	err := os.Remove(afn)
	lib.Ok(t, err)
	err = os.Remove(afn + ".clean")
	lib.Ok(t, err)
	err = os.Remove(efn + ".clean")
	lib.Ok(t, err)
}

func TestScDisplayServer(t *testing.T) {
	afn := "testdata/actDisplayServer.log"
	efn := "testdata/expDisplayServer.log"
	os.Remove(afn)

	lgf.Name = afn
	err := trice.NewConnection(true)
	lib.Ok(t, err)

	disp.PtrRPC.Call("Server.LogSetFlags", []int64{0}, nil)
	disp.PtrRPC.Call("Server.Out", []string{"a", "tst:test", "dbg:line"}, nil)
	//time.Sleep(1000 * time.Millisecond)

	disp.StopServer()
	lib.CleanFile(afn)
	lib.CleanFile(efn)

	lib.EqualFiles(t, afn+".clean", efn+".clean")
	err = os.Remove(afn)
	lib.Ok(t, err)
	err = os.Remove(afn + ".clean")
	lib.Ok(t, err)
	err = os.Remove(efn + ".clean")
	lib.Ok(t, err)
	//disp.StopServer()
}
