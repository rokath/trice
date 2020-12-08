// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package args implements the commandline interface and calls the appropriate commands.
// Its function Handler is called in package main as entry point.
package args

import (
	"flag"
)

var (
	// Version is the program version number and is injected from main package.
	Version string

	// Commit is the program checksum and injected from main package.
	Commit string

	// Date is the compile time and injected from main package.
	Date string

	// verbose gives mor information on output if set. This variable is copied into the appropriate packages.
	verbose bool

	// fsScUpdate is flag set for sub command 'update' for updating ID list.
	fsScUpdate *flag.FlagSet

	// fsScZero is flag set for sub command 'zero' for clearing IDs in source tree.
	fsScZero *flag.FlagSet

	// pSrcZ is a string pointer to the safety string for scZero.
	pSrcZ *string

	// fsScHelp is flag set for sub command 'help'.
	fsScHelp *flag.FlagSet

	// fsScLog is flag set for sub command 'log'.
	fsScLog *flag.FlagSet

	// fsScSv is flag set for sub command 'displayServer'.
	fsScSv *flag.FlagSet

	// fsScVerseion is flag set for sub command 'version'.
	fsScVersion *flag.FlagSet

	// fsScScan is flag set for sub command 'scan'.
	fsScScan *flag.FlagSet

	// fsScSdSv is flag set for sub command 'shutdownServer'.
	fsScSdSv *flag.FlagSet

	// used to replace "default" args value for STLINK and JLINK port
	defaultLinkArgs = "-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0 -RTTSearchRanges 0x20000000_0x1000"

	// used to replace "default" args value for COMn port
	defaultCOMArgs = ""

	// used to replace "default" args value for BUFFER port
	defaultBUFFERArgs = "0 0 0 0"
)
