// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package main

import (
	"fmt"
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
)

// main is the entry point.
func main() {
	doit()
}

// doit is the action.
func doit() {

	// inject values
	args.Version = version
	args.Commit = commit
	args.Date = date

	rand.Seed(time.Now().UnixNano())
	err := args.Handler(os.Args)
	if nil != err {
		fmt.Println(error.Error(err))
		os.Exit(1)
	}
	os.Exit(0)
}
