// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

// source tree management

import (
	"fmt"
	"io"
	"os"
	"strconv"
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

	out, modified, err := cleanTriceIDs(w, path, in, a)
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

func cleanTriceIDs(w io.Writer, path string, in []byte, a *ant.Admin) (out []byte, modified bool, err error) {
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
					fmt.Fprintln(w, "unexpected syntax", idS)
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
		uct := t
		uct.Type = strings.ToUpper(t.Type) // Lower case and upper case Type are not distinguished.
		a.Mutex.Lock()                     // several files could contain the same t or idn.
		tt, ok := idd.idToTrice[idn]       // check til.json.
		if !ok {                           // idn is not inside til.json.
			idd.idToTrice[idn] = uct // Add idn.
		} else { // idn is inside til.json.
			if tt != uct { // idn references to a different t.
				idn = 0 // silently set it to 0
			}
		}
		line += strings.Count(rest[:loc[1]], "\n") // Update line number for location information.
		if idn != 0 {
			idd.idToLocNew[idn] = TriceLI{path, line} // Add idn to new location information.
		}
		line += strings.Count(rest[loc[1]:loc[6]], "\n") // Keep line number up-to-date for location information.
		outs, delta = writeID(outs, offset, loc, t, 0)   // Do the clean.
		offset += delta
		modified = true
		rest = rest[loc[6]:]
		offset += loc[6]
	}
	out = []byte(outs)
	return
}

//  // cleanTriceIDs does the ID clearance task on in, uses local pointer idd and returns the result in out with modified==true when any changes where made.
//  // For reference look into file TriceUserGuide.md part "The `trice insert` Algorithm".
//  // The trice id numbers n are set to 0, but the iD(n), id(n), Id(n) or iD(n) stay untouched.
//  func cleanTriceIDs(w io.Writer, path string, in []byte, _ *ant.Admin) (out []byte, modified bool) {
//
//  	//a.Mutex.RLock()
//  	//tfmt, found := idd.idToFmt[TriceID(1)]
//  	//_, err = fmt.Fprintln(w, "cleaning in", path, found, tfmt)
//  	//a.Mutex.RUnlock()
//
//  	s := string(in)
//  	rest := s
//
//  	for {
//  		loc := matchNbTRICE.FindStringIndex(rest) // loc is the position of the next trice statement.
//  		if loc == nil {
//  			break // done
//  		}
//  		nbTRICE := rest[loc[0]:loc[1]]
//  		nbID := matchNbID.FindString(nbTRICE) // nbID is the `ID(n)` inside the trice statement.
//  		if nbID == "" {
//  			rest = rest[loc[1]:] // cut off what is parsed
//  			continue
//  		}
//  		nb := matchNb.FindString(nbID) // nb is the plain id number
//  		if nb == "0" {
//  			rest = rest[loc[1]:] // cut off what is parsed
//  			continue
//  		}
//  		fmt.Fprintln(w, nb, " -> ", "0")
//
//  		zeroID := strings.Replace(nbID, nb, "0", 1)            // replace n with 0 inside ID statement
//  		zeroTRICE := strings.Replace(nbTRICE, nbID, zeroID, 1) // replace id(n) with id(0) inside trice statement
//  		s = strings.Replace(s, nbTRICE, zeroTRICE, 1)          // replace trice( id(n), "...", ...) with trice( id(0), "...", ...) inside s. This works because n is unique.
//  		rest = rest[loc[1]:]                                   // cut off what is parsed
//  		modified = true
//  	}
//
//  	out = []byte(s)
//  	return out, modified
//  }

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
