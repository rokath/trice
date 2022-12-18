// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package main

import (
	"fmt"
	"io"
	"math/rand"
	"os"
	"time"

	"github.com/rokath/trice/internalStim/stimargs"
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
	doit(os.Stdout)
}

// doit is the action.
func doit(w io.Writer) {

	// inject values
	stimargs.Version = version
	stimargs.Commit = commit
	stimargs.Date = date

	rand.Seed(time.Now().UnixNano())

	e := stimargs.Handler(w, os.Args)
	if nil != e {
		fmt.Fprintln(w, error.Error(e))
	}
}
