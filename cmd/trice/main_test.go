// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package main

import (
	"bytes"
	"fmt"
	"io"
	"testing"
	"time"

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

// TestDisplayServerStartAndShutdown fails with -race.
// ds should be a channel to avoid that, but I do notknow how to do that here without changing the args.Handler signature.
func _TestDisplayServerStartAndShutdown(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewOsFs()}
	//mtb := milkthisbuffer.New(500)
	//go mtb.StdoutAsync()
	//ds:=

	pipe1_reader, pipe1_writer := io.Pipe()

	//	go args.Handler(&ds, fSys, []string{"trice", "ds", "-color", "none", "-ipp", "61496"})
	go args.Handler(pipe1_writer, fSys, []string{"trice", "ds", "-color", "none", "-ipp", "61496"})
	//	expected := "displayServer @ localhost:61496\n"
	expected := "displayServer @ localhost:61496\n"
	out := make([]byte, 100)
	time.Sleep(10 * time.Millisecond)
	n, err := pipe1_reader.Read(out)
	assert.Nil(t, err)
	assert.Equal(t, n, len(expected))
	result := string(out)
	assert.Equal(t, expected, result[:n])

	//for ds.String() != expected {
	//	time.Sleep(time.Millisecond)
	//}
	var sd bytes.Buffer
	args.Handler(&sd, fSys, []string{"trice", "sd", "-ipp", "61496"})
	//assert.Equal(t, "displayServer @ localhost:61496\n\n\ndisplayServer shutdown\n\n\n", ds.String())
	assert.Equal(t, "", sd.String())
}
