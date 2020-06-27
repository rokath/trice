// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package global holds global valuea for all packages.
//
// Including package main in other packages will result in aforbidden import cycle.
// Therefore package global serves as glue.
package global

var (
	// Verbose gives additional informal output
	Verbose bool
)
