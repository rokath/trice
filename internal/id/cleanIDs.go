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
	"time"

	"github.com/rokath/trice/pkg/ant"
	"github.com/rokath/trice/pkg/msg"
	"github.com/spf13/afero"
)

// SubCmdIdClean performs sub-command clean, zeroing or removing trice IDs from source tree.
func SubCmdIdClean(w io.Writer, fSys *afero.Afero) error {
	return IDData.cmdSwitchTriceIDs(w, fSys, IDData.triceIDCleaning)
}

// triceIDCleaning reads file, processes it and writes it back, if needed.
func (p *idData) triceIDCleaning(w io.Writer, fSys *afero.Afero, path string, fileInfo os.FileInfo, a *ant.Admin) (err error) {

	///////////////////////////////////////////////////////////////////////////////
	// cache stuff:
	//
	home, err := os.UserHomeDir()
	p.join(err)
	cache := filepath.Join(home, ".trice/cache")
	var cleanedCachePath string
	var cacheExists bool

	if _, err = os.Stat(cache); err == nil { // cache folder exists

		// This cache code works in conjunction with the cache code in function triceIDInsertion.
		cacheExists = true
		fullPath, err := filepath.Abs(path)
		p.join(err)

		// remove first colon, if exists (Windows)
		before, after, _ := strings.Cut(fullPath, ":")
		fullPath = before + after

		// construct cleanedCachePath
		cleanedCachePath = filepath.Join(cache, cleanedCacheFolderName, fullPath)

		// If no cleanedCachePath, execute clean operation
		cCache, err := os.Lstat(cleanedCachePath)
		if err != nil {
			msg.Tell(w, "no cleaned Cache file")
			goto clean
		}

		// If path content equals cleanedCachePath content, we are done.
		if fileInfo.ModTime() == cCache.ModTime() {
			msg.Tell(w, "trice c was executed before, nothing to do")
			return msg.OnErrFv(w, p.err) // `trice c File`: File == cCache ? done
		}
		fmt.Println("fileInfo.ModTime() != cCache.ModTime()", fileInfo.ModTime(), cCache.ModTime())

		// Construct insertedCachePath.
		insertedCachePath := filepath.Join(cache, insertedCacheFolderName, fullPath)

		// If no insertedCachePath, execute clean operation.
		iCache, err := os.Lstat(insertedCachePath)
		if err != nil {
			msg.Tell(w, "no inserted Cache file")
			goto clean
		}

		// If path content equals insertedCachePath content, we can copy cleanedCachePath to path.
		// We know here, that cleanedCachePath exists and path was not edited.
		if fileInfo.ModTime() == iCache.ModTime() {
			// trice i File: File == iCache ? cCache -> F (trice c was executed before)

			msg.Tell(w, "trice c was executed before, copy cCache into file")
			p.copyFileWithMTime(path, cleanedCachePath)
			return msg.OnErrFv(w, p.err) // That's it.
		}
		fmt.Println("fileInfo.ModTime() != cCache.ModTime()", fileInfo.ModTime(), cCache.ModTime())
		msg.Tell(w, "File was edited, invalidate inserted cache")
		os.Remove(insertedCachePath)
		os.Remove(cleanedCachePath)
	}
	//
	///////////////////////////////////////////////////////////////////////////////

clean:

	msg.Tell(w, "process cleaning")
	in, err := fSys.ReadFile(path)
	if err != nil {
		return msg.OnErrFv(w, p.err)
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

	out, modified, err := cleanTriceIDs(w, liPath, in, a)
	if err != nil {
		return msg.OnErrFv(w, p.err)
	}

	if filepath.Base(path) == "triceConfig.h" {
		outs := string(out)
		x := strings.Index(outs, "#define TRICE_CLEAN 0")
		if x != -1 { // found
			outs := strings.Replace(outs, "#define TRICE_CLEAN 0", "#define TRICE_CLEAN 1", 1)
			out = []byte(outs)
			modified = true
		}
	}
	if modified {
		if Verbose {
			fmt.Fprintln(w, "Changed: ", path)
		}
		if !DryRun {
			err = fSys.WriteFile(path, out, fileInfo.Mode())
			p.join(err)
			msg.Tell(w, "restoring file mtime")
			err = os.Chtimes(path, time.Time{}, fileInfo.ModTime())
			p.join(err)
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// cache stuff:
	//
	if !cacheExists {
		return msg.OnErrFv(w, p.err)
	}

	// The file could have been modified by the user but if IDs are not touched, modified is false.
	// So we need to update the cache.
	msg.Tell(w, "Copy (user) modified and with cleaned IDs file into the cache.")
	err = os.MkdirAll(filepath.Dir(cleanedCachePath), os.ModeDir)
	p.join(err)
	p.copyFileWithMTime(cleanedCachePath, path)

	// Set cleanedCachePath mtime to (updated) path mtime.
	iFile, err := os.Lstat(path)
	p.join(err)
	err = os.Chtimes(cleanedCachePath, time.Time{}, iFile.ModTime())
	p.join(err)
	//
	///////////////////////////////////////////////////////////////////////////////

	return msg.OnErrFv(w, p.err)
}

// cleanTriceIDs sets all trice IDs inside in to 0. If an ID is not inside til.json it is added.
// If an ID is inside til.json referencing to a different trice, it is set to 0 inside in.
// All valid IDs are used to build a new li.json file.
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
		tt, ok := IDData.idToTrice[idn]            // check til.json.
		if !ok {                                   // idn is not inside til.json.
			IDData.idToTrice[idn] = t // Add idn.
		} else { // idn is inside til.json.
			if tt != t { // idn references to a different t.
				fmt.Fprintln(w, "ID", idn, "inside", path, "line", line, "refers to", t, "but is used inside til.json for", tt, "- setting it to 0.")
				idn = 0 // silently set it to 0
			}
		}
		if idn != 0 {
			IDData.idToLocNew[idn] = TriceLI{path, line} // Add idn to new location information.
			if Verbose {
				fmt.Fprintln(w, idn, path, line, "added to li")
			}
		}
		a.Mutex.Unlock()
		line += strings.Count(rest[loc[1]:loc[6]], "\n") // Keep line number up-to-date for location information.
		outs, delta = cleanID(outs, offset, loc, t)      // Do the clean.
		offset += delta
		modified = true
		rest = rest[loc[6]:]
		offset += loc[6]
	}
	if Verbose {
		fmt.Fprintln(w, len(IDData.idToLocNew), "items inside li")
	}
	out = []byte(outs)
	return
}
