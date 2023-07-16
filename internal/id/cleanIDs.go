// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

// source tree management

import (
	"fmt"
	"io"
	"os"
	"strings"

	"github.com/rokath/trice/pkg/ant"
	"github.com/spf13/afero"
)

// SubCmdIdClean performs sub-command clear, removing trice IDs from source tree.
func SubCmdIdClean(w io.Writer, fSys *afero.Afero) error {
	return cmdSwitchTriceIDs(w, fSys, triceIDCleaning)
}

// triceIDCleaning reads file, processes it and writes it back, if needed.
func triceIDCleaning(w io.Writer, fSys *afero.Afero, path string, fileInfo os.FileInfo, a *ant.Admin) error {

	in, err := fSys.ReadFile(path)
	if err != nil {
		return err
	}
	if Verbose {
		fmt.Fprintln(w, path)
	}

	out, modified := cleanTriceIDs(w, path, in, a)
	if err != nil {
		return err
	}

	if modified && !DryRun {
		if Verbose {
			fmt.Fprintln(w, "Changed: ", path)
		}
		err = fSys.WriteFile(path, out, fileInfo.Mode())
	}
	return err
}

// cleanTriceIDs does the ID clearance task on in, uses local pointer idd and returns the result in out with modified==true when any changes where made.
// For reference look into file TriceUserGuide.md part "The `trice insert` Algorithm".
// The trice id numbers n are set to 0, but the iD(n), id(n), Id(n) or iD(n) stay untouched.
func cleanTriceIDs(w io.Writer, path string, in []byte, _ *ant.Admin) (out []byte, modified bool) {

	//a.Mutex.RLock()
	//tfmt, found := idd.idToFmt[TriceID(1)]
	//_, err = fmt.Fprintln(w, "cleaning in", path, found, tfmt)
	//a.Mutex.RUnlock()

	s := string(in)
	rest := s

	for {
		loc := matchNbTRICE.FindStringIndex(rest) // loc is the position of the next trice statement.
		if loc == nil {
			break // done
		}
		nbTRICE := rest[loc[0]:loc[1]]
		nbID := matchNbID.FindString(nbTRICE) // nbID is the `ID(n)` inside the trice statement.
		if nbID == "" {
			rest = rest[loc[1]:] // cut off what is parsed
			continue
		}
		nb := matchNb.FindString(nbID) // nb is the plain id number
		if nb == "0" {
			rest = rest[loc[1]:] // cut off what is parsed
			continue
		}
		fmt.Fprintln(w, nb, " -> ", "0")

		zeroID := strings.Replace(nbID, nb, "0", 1)            // replace n with 0 inside ID statement
		zeroTRICE := strings.Replace(nbTRICE, nbID, zeroID, 1) // replace id(n) with id(0) inside trice statement
		s = strings.Replace(s, nbTRICE, zeroTRICE, 1)          // replace trice( id(n), "...", ...) with trice( id(0), "...", ...) inside s. This works because n is unique.
		rest = rest[loc[1]:]                                   // cut off what is parsed
		modified = true
	}

	out = []byte(s)
	return out, modified
}

/*
// cleanNextID starts subsIn parsing
// found flag is true if an ID was zeroed, others are updated input values. if an ID wsa zeroed
// - modified gets true
// - subs gets shorter
// - s is updated
func cleanNextID(w io.Writer, modifiedIn bool, subsIn, in string) (found bool, modifiedOut bool, subsOut string, out string) {
	modifiedOut = modifiedIn
	//subsOut = subsIn
	out = in
	loc := matchNbTRICE.FindStringIndex(subsIn)
	if nil == loc {
		return
	}
	nbTRICE := subsIn[loc[0]:loc[1]]
	nbID := matchNbID.FindString(nbTRICE)
	if nbID == "" {
		msg.Info(fmt.Sprintln("No 'Id(n)' found inside " + nbTRICE)) // todo: patID
		return
	}

	zeroID := "Id(0)" // todo: patID
	fmt.Fprintln(w, nbID, " -> ", zeroID)

	zeroTRICE := strings.Replace(nbTRICE, nbID, zeroID, 1)
	out = strings.Replace(out, nbTRICE, zeroTRICE, 1)
	// 2^32 has 9 ciphers and shortest trice has 14 chars: TRICE0(Id(1),"");
	// The replacement of n with 0 makes s shorter, so the next search should start like 10 chars earlier.
	subsOut = subsIn[loc[1]-10:]
	found = true
	modifiedOut = true
	return
}
*/
