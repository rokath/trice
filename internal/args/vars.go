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

	// fsScRefresh is flag set for sub command 'refresh' for updating ID list without touching the sources.
	fsScRefresh *flag.FlagSet

	// fsScRenew is flag set for sub command 'renew' for re-newing ID list without touching the sources.
	fsScRenew *flag.FlagSet

	// fsScUpdate is flag set for sub command 'update' for updating ID list.
	fsScUpdate *flag.FlagSet

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

	// fsScZero is flag set for sub command 'zero' for clearing IDs in source tree.
	fsScZero *flag.FlagSet

	// pSrcZ is a string pointer to the safety string for scZero.
	pSrcZ *string

	allHelp           bool // flag for partial help
	displayServerHelp bool // flag for partial help
	helpHelp          bool // flag for partial help
	logHelp           bool // flag for partial help
	refreshHelp       bool // flag for partial help
	renewHelp         bool // flag for partial help
	scanHelp          bool // flag for partial help
	shutdownHelp      bool // flag for partial help
	updateHelp        bool // flag for partial help
	versionHelp       bool // flag for partial help
	zeroIDsHelp       bool // flag for partial help
)
