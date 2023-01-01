// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package main

import (
	"fmt"
	"io"
	"io/fs"
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
	osFs := os.DirFS("")
	doit(os.Stdout, osFs)
}

// doit is the action.
func doit(w io.Writer, osFs fs.FS) {

	// inject values
	args.Version = version
	args.Commit = commit
	args.Date = date

	rand.Seed(time.Now().UnixNano())

	e := args.Handler(w, osFs, os.Args)
	if nil != e {
		fmt.Fprintln(w, error.Error(e))
	}
}
