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

// TestDisplayServerStartAndShutdownLegacy fails with -race
func _TestDisplayServerStartAndShutdownLegacy(t *testing.T) {
	var ds, sd bytes.Buffer
	fSys := &afero.Afero{Fs: afero.NewOsFs()} // osFs := os.DirFS("")
	go args.Handler(&ds, fSys, []string{"trice", "ds", "-color", "none", "-ipp", "61495"})
	for ds.String() != "displayServer @ localhost:61495\n" {
	}
	args.Handler(&sd, fSys, []string{"trice", "sd", "-ipp", "61495"})
	assert.Equal(t, "displayServer @ localhost:61495\n\n\ndisplayServer shutdown\n\n\n", ds.String())
	assert.Equal(t, "", sd.String())
}

// TestDisplayServerStartAndShutdown fails with -race.
// ds should be a channel to avoid that, but I do notknow how to do that here without changing the args.Handler signature.
func _TestDisplayServerStartAndShutdown0(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewOsFs()}

	var ds bytes.Buffer
	go args.Handler(&ds, fSys, []string{"trice", "ds", "-color", "none", "-ipp", "61497"})

	for ds.String() != "displayServer @ localhost:61497\n" {
		time.Sleep(1 * time.Millisecond)
	}

	expect := "displayServer @ localhost:61497\n"
	actual := ds.String()
	assert.Equal(t, expect, actual)

	var sd bytes.Buffer
	args.Handler(&sd, fSys, []string{"trice", "sd", "-ipp", "61497"})

	exp := "displayServer @ localhost:61497\n\n\ndisplayServer shutdown\n\n\n"
	act := ds.String()
	assert.Equal(t, exp, act)
	assert.Equal(t, "", sd.String())
}

// TestDisplayServerStartAndShutdown1 does not work for some reason!
// ds should be a channel to avoid that, but I do notknow how to do that here without changing the args.Handler signature.
func _TestDisplayServerStartAndShutdown1(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewOsFs()}

	rd, wr := io.Pipe()
	go args.Handler(wr, fSys, []string{"trice", "ds", "-color", "none", "-ipp", "61498"})
	time.Sleep(100 * time.Millisecond)

	expect := "displayServer @ localhost:61498\n"
	out := make([]byte, 1000)
	n, err := rd.Read(out)
	assert.Nil(t, err)
	actual := string(out[:n])
	assert.Equal(t, expect, actual)

	var sd bytes.Buffer
	args.Handler(&sd, fSys, []string{"trice", "sd", "-ipp", "61498"})
	time.Sleep(100 * time.Millisecond)
	wr.Close() // close the writer, so the reader knows there's no more data

	exp := "displayServer @ localhost:61498\n\n\ndisplayServer shutdown\n\n\n"
	n, err = rd.Read(out)
	assert.Nil(t, err)
	act := string(out[:n])
	assert.Equal(t, exp, act)

	assert.Equal(t, "", sd.String())
}
