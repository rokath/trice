// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package main

import (
	"math/rand"
	"os"
	"time"

	"github.com/rokath/trice/internal/args"
)

var (
	// do not initialize, goreleaser will handle that
	version string

	// do not initialize, goreleaser will handle that
	commit string

	// do not initialize, goreleaser will handle that
	date string

	// linkTime ...
	linkTime = "testTime"
)

func main() {
	args.Version = version
	args.Commit = commit
	args.Date = date
	fi, err := os.Stat(os.Args[0])
	if nil == err {
		args.LinkTime = fi.ModTime().String()
	} else {
		args.LinkTime = "unknown" // on running main tests fileinfo is invalid
	}
	rand.Seed(time.Now().UnixNano())
	err = args.HandleArgs(os.Args)
	if nil != err {
		os.Exit(1)
	}
	os.Exit(0)
}
