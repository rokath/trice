// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package main

import (
	"fmt"
	"io"
	"math/rand"
	"os"
	"time"

	"github.com/rokath/trice/internal/args"
	"github.com/spf13/afero"
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
	fSys := &afero.Afero{Fs: afero.NewOsFs()} // os.DirFS("")
	doit(os.Stdout, fSys)
}

// doit is the action.
func doit(w io.Writer, fSys *afero.Afero) {

	// inject values
	args.Version = version
	args.Commit = commit
	args.Date = date

	rand.Seed(time.Now().UnixNano())

	e := args.Handler(w, fSys, os.Args)
	if e != nil {
		fmt.Fprintln(w, error.Error(e))
	}
}
