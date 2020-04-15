// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package main

import (
	"os"
	"testing"

	"github.com/rokath/trice/pkg/lib"
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

func TestScHelp(t *testing.T) {
	afn := "testdata/acthelp.log"
	efn := "testdata/exphelp.log"
	os.Remove(afn)
	args := []string{"trice", "help", "-lf", afn}
	err := HandleArgs(args)
	lib.Ok(t, err)

	lib.CleanFile(afn)
	lib.CleanFile(efn)

	lib.EqualFiles(t, afn+".clean", efn+".clean")
	err = os.Remove(afn)
	err = os.Remove(afn + ".clean")
	err = os.Remove(efn + ".clean")
	lib.Ok(t, err)
}
