// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package main

import (
	"io"
	"log"
	"os"

	"github.com/rokath/trice/internal/args"
	"github.com/spf13/afero"
)

var (
	version   string // do not initialize, goreleaser will handle that
	commit    string // do not initialize, goreleaser will handle that
	date      string // do not initialize, goreleaser will handle that
	branch    string // like "main"
	gitState  string // "clean" or "dirty"
	gitStatus string // changed file list (from build.sh)
	builtBy   string // empty or "goreleaser"
)

// main is the entry point.
func main() {
	fSys := &afero.Afero{Fs: afero.NewOsFs()}
	e := doit(os.Stdout, fSys)
	if e != nil {
		log.Fatal(e)
	}
}

// doit is the action.
func doit(w io.Writer, fSys *afero.Afero) error {

	// inject values
	args.Version = version
	args.Commit = commit
	args.Date = date
	args.Branch = branch
	args.GitState = gitState
	args.GitStatus = gitStatus
	args.BuiltBy = builtBy

	return args.Handler(w, fSys, os.Args)
}
