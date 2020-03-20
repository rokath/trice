// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// All rights reserved.
// Use of this source code is governed by a
// license that can be found in the LICENSE file.

package main

func ExampleHandleArgs_none() {
	HandleArgs("./", []string{"trice", ""})
	// Output:
	// try: 'trice help|h'
}

func ExampleHandleArgs_wrongSubcommand() {
	HandleArgs("./", []string{"trice", "xyz"})
	// Output:
	// try: 'trice help|h'
}

func ExampleHandleArgs_logListNotFound() {
	HandleArgs("./", []string{"trice", "log", "-list", "xxx.json", "-port", "COMscan"})
	// Output: ID list C:\GitRepos\trice\cmd\trice\xxx.json not found, exit
}

func ExampleHandleArgs_logCOM0() {
	HandleArgs("./", []string{"trice", "log", "-list", "none", "-port", "COM0"})
	// Output:
	// id list file none with 0 items
	// Error: Could not open serial port: Serial port not found
	// Could not set up serial port COM0
	// try -port COMscan
}

func ExampleHandleArgs_logNoParam() {
	HandleArgs("./", []string{"trice", "log", "", ""})
	//xOutput:
	// -baud int
	// 	COM baudrate (optional, default is 38400 (default 38400)
	// -list string
	// 	ID list path (optional) (default "./idlist.json")
	// -port string
	// 	subcommand (required, try COMscan)
}

func ExampleHandleArgs_logWrongParam() {
	HandleArgs("./", []string{"trice", "log", "-x", "y"})
	// xOutput: flag provided but not defined: -x
	// Usage of log:
	//  -baud int
	//        COM baudrate (optional, default is 38400 (default 38400)
	//  -port string
	//        subcommand (required, try COMscan)
}
