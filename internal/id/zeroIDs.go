// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

// source tree management

import (
	"fmt"
	"io"
	"os"
	"path/filepath"
	"strconv"
	"strings"

	"github.com/rokath/trice/pkg/ant"
	"github.com/spf13/afero"
)

// SubCmdIdZero performs sub-command zero, setting trice IDs in source tree to 0.
func SubCmdIdZero(w io.Writer, fSys *afero.Afero) error {
	return cmdSwitchTriceIDs(w, fSys, triceIDZeroing)
}

// triceIDZeroing reads file, processes it and writes it back, if needed.
func triceIDZeroing(w io.Writer, fSys *afero.Afero, path string, fileInfo os.FileInfo, a *ant.Admin) error {

	in, err := fSys.ReadFile(path)
	if err != nil {
		return err
	}
	if Verbose {
		fmt.Fprintln(w, path)
	}

	var liPath string

	if LiPathIsRelative {
		liPath = filepath.ToSlash(path)
	} else {
		liPath = filepath.Base(path)
	}

	out, modified, err := zeroTriceIDs(w, liPath, in, a)
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

// zeroTriceIDs sets all trice IDs inside in to 0. If an ID is not inside til.json it is added.
// If an ID is inside til.json referencing to a different trice, it is reported and set to 0 inside in.
// All valid IDs are used to build a new li.json file.
func zeroTriceIDs(w io.Writer, path string, in []byte, a *ant.Admin) (out []byte, modified bool, err error) {
	var idn TriceID    // idn is the last found id inside the source.
	var idS string     // idS is the "iD(n)" statement, if found.
	var ignore bool    // ignore gets true if a found trice statement is skipped.
	rest := string(in) // rest is the so far not processed part of the file.
	outs := rest       // outs is the resulting string.
	var offset int     // offset is incremented by n, when rest is reduced by n.
	var delta int      // offset change cause by ID statement insertion
	var t TriceFmt     // t is the actual located trice.
	line := 1          // line counts source code lines, these start with 1.
	for {
		idn = 0                 // clear here
		loc := matchTrice(rest) // loc is the position of the next trice type (statement name with opening parenthesis followed by a format string).
		if loc == nil {
			break // done
		}
		if loc[3] == loc[4] { // No iD(n) found
			ignore = true
		} else {
			t.Type = rest[loc[0]:loc[1]]       // t.Type is the TRice8_2 or TRice part for example. Hint: TRice defaults to 32 bit if not configured differently.
			t.Strg = rest[loc[5]+1 : loc[6]-1] // Now we have the complete trice t (Type and Strg). We remove the double quotes wit +1 and -1.
			idS = rest[loc[3]:loc[4]]          // idS is where we expect n.
			nLoc := matchNb.FindStringIndex(idS)
			if nLoc == nil { // Someone wrote trice( iD(0x100), ...), trice( id(), ... ) or trice( iD(name), ...) for example.
				if Verbose {
					lineNumber := line + strings.Count(rest[:loc[6]], "\n")
					fmt.Fprintln(w, "unexpected syntax", idS, "in file", path, "line", lineNumber)
				}
				ignore = true
			} else { // This is the normal case like trice( iD( 111)... .
				nStrg := idS[nLoc[0]:nLoc[1]] // nStrg is the plain number string.
				n, err := strconv.Atoi(nStrg)
				if err == nil {
					idn = TriceID(n) // idn is the assigned id inside source file.
				} else { // unexpected
					fmt.Fprintln(w, err, nStrg) // report
					ignore = true
				}
				if idn == 0 {
					ignore = true
				}
			}
		}
		if ignore {
			line += strings.Count(rest[:loc[6]], "\n") // Keep line number up-to-date for location information.
			rest = rest[loc[6]:]
			offset += loc[6]
			ignore = false // clear here
			continue       // ignore such cases
		}
		// trice t (t.Type & t.Strg) is known now. idn holds the trice id found in the source. Example case: trice( iD(111), "foo, ... ")
		// We do not simply replace the ID with 0. We check til.json, extend it if needed and we build a new li.json.
		line += strings.Count(rest[:loc[1]], "\n") // Update line number for location information.
		a.Mutex.Lock()                             // several files could contain the same t or idn.
		tt, ok := idd.idToTrice[idn]               // check til.json.
		if !ok {                                   // idn is not inside til.json.
			idd.idToTrice[idn] = t // Add idn.
		} else { // idn is inside til.json.
			if tt != t { // idn references to a different t.
				fmt.Fprintln(w, "ID inside", path, "line", line, "refers to", t, "but is already used inside til.json for", tt)
				idn = 0 // silently set it to 0
			}
		}
		if idn != 0 {
			idd.idToLocNew[idn] = TriceLI{path, line} // Add idn to new location information.
			if Verbose {
				fmt.Fprintln(w, idn, path, line, "added to li")
			}
		}
		a.Mutex.Unlock()
		line += strings.Count(rest[loc[1]:loc[6]], "\n") // Keep line number up-to-date for location information.
		outs, delta = writeID(outs, offset, loc, t, 0)   // Do the clean.
		offset += delta
		modified = true
		rest = rest[loc[6]:]
		offset += loc[6]
	}
	if Verbose {
		fmt.Fprintln(w, len(idd.idToLocNew), "items inside li")
	}
	out = []byte(outs)
	return
}
