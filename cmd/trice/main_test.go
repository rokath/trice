// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package main

import (
	"bytes"
	"fmt"
	"testing"

	"github.com/rokath/trice/internal/args"
	"github.com/spf13/afero"
	"github.com/tj/assert"
)

func TestTriceVersion1(t *testing.T) {
	args.Version = "1.2.3"
	args.Commit = "myCommit"
	args.Date = "2006-01-02_1504-05"
	var b bytes.Buffer
	fSys := &afero.Afero{Fs: afero.NewOsFs()} // osFs := os.DirFS("")
	e := args.Handler(&b, fSys, []string{"trice", "version"})
	assert.Nil(t, e)
	assert.Equal(t, "version=1.2.3, commit=myCommit, built at 2006-01-02_1504-05\n", b.String())
}

func TestTriceWrong1(t *testing.T) {
	var b bytes.Buffer
	fSys := &afero.Afero{Fs: afero.NewOsFs()} // osFs := os.DirFS("")
	e := args.Handler(&b, fSys, []string{"trice", "wrong"})
	assert.Equal(t, "", b.String())
	assert.Equal(t, "unknown sub-command 'wrong'. try: 'trice help|h'\n", fmt.Sprintln(e))
}

func TestDisplayServerStartAndShutdown(t *testing.T) {
	var ds, sd bytes.Buffer
	fSys := &afero.Afero{Fs: afero.NewOsFs()} // osFs := os.DirFS("")
	go args.Handler(&ds, fSys, []string{"trice", "ds", "-color", "none", "-ipp", "61496"})
	for ds.String() != "displayServer @ localhost:61496\n" {
	}
	args.Handler(&sd, fSys, []string{"trice", "sd", "-ipp", "61496"})
	assert.Equal(t, "displayServer @ localhost:61496\n\n\ndisplayServer shutdown\n\n\n", ds.String())
	assert.Equal(t, "", sd.String())
}
