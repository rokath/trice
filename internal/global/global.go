// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package global holds global values for all packages.
//
// Including package main in other packages will result in aforbidden import cycle.
// Therefore package global serves as glue.
package global

import "os"

var (
	// Verbose gives additional informal output
	Verbose bool // init value is 'false'

	// OsExit is a function pointer to os.Exit.
	//
	// For unit tests this value can be changed to avoid leaving the test function immediately.
	// This is good for code under your control.
	//
	// To test code calling os.Exit() implicitely, like when log.Fatal() is used,
	// see https://stackoverflow.com/questions/26225513/how-to-test-os-exit-scenarios-in-go.
	OsExit func(code int) = os.Exit
)
