// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

// source tree management

import (
	"bufio"
	"fmt"
	"io"
	"math"
	"os"
	"path/filepath"
	"strconv"
	"strings"

	"github.com/rokath/trice/pkg/ant"
	"github.com/spf13/afero"
)

// SubCmdIdInsert performs sub-command insert, adding trice IDs to source tree.
func SubCmdIdInsert(w io.Writer, fSys *afero.Afero) error {
	return cmdSwitchTriceIDs(w, fSys, triceIDInsertion)
}

// triceIDInsertion reads file, processes it and writes it back, if needed.
func triceIDInsertion(w io.Writer, fSys *afero.Afero, path string, fileInfo os.FileInfo, a *ant.Admin) error {

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

	out, fileModified, err := insertTriceIDs(w, liPath, in, a)
	if err != nil {
		return err
	}

	if fileModified && !DryRun {
		if Verbose {
			fmt.Fprintln(w, "Changed: ", path)
		}
		err = fSys.WriteFile(path, out, fileInfo.Mode())
	}
	return err
}

// insertTriceIDs does the ID insertion task on in. insertTriceIDs uses internally local pointer IDData because IDData cannot be easily passed via parameters.
// insertTriceIDs returns the result in out with modified==true when out != in.
//
// in is the read file liPath content and out is the file content which needs to be written.
// a is used for mutex access to IDData. liPath is needed for location information.
// insertTriceIDs is intended to be used in several Go routines (one for each file) for faster ID insertion.
// Data usage:
// - idd.idToTrice is the serialized til.json. It is extended with unknown and new IDs and written back to til.json finally.
// - idd.triceToId is the initially reverted idd.idToTrice. It is shrunk for each used ID amd used to find out if an ID is already fresh used.
//   - When starting, idd.triceToId holds all IDs from til.json and no ID is fresh used yet. If an ID is to be (fresh) used it is removed from idd.triceToId.
//   - If an ID is found in idd.idToTrice but not found in idd.triceToId anymore, it is already (fresh) used and not usable again.
//   - If a new ID is generated, it is added to idd.idToTrice only. This way it gets automatically used.
//
// - idd.idToLocRef is only for reference and not changed. It is the "old" location information.
// - idd.idToLocNew is new generated during insertTriceIDs execution and finally written back to li.json as "new" location information.
// For reference look into file TriceUserGuide.md part "The `trice insert` Algorithm".
// insertTriceIDs parses the file content from the beginning for the next trice statement, deals with it and continues until the file content end.
// When a trice statement was found, general cases are:
// - idInSourceIsNonZero, id is inside idd.idToTrice with matching trice and inside idd.triceToId -> use ID (remove from idd.triceToId)
//   - If trice is assigned to several IDs, the location information consulted. If a matching liPath exists, its first occurrence is used.
//
// - idInSourceIsNonZero, id is inside idd.idToTrice with matching trice and not in idd.triceToId -> used ID! -> create new ID && invalidate ID in source
// - idInSourceIsNonZero, id is inside idd.idToTrice with different trice                         -> used ID! -> create new ID && invalidate ID in source
// - idInSourceIsNonZero, id is not inside idd.idToTrice (cannot be inside idd.triceToId)         -> add ID to idd.idToTrice
// - idInSourceIsZero,    trice is not inside idd.triceToId                                       -> create new ID & add ID to idd.idToTrice
// - idInSourceIsZero,    trice is is inside idd.triceToId                                        -> unused ID -> use ID (remove from idd.triceToId)
//   - If trice is assigned to several IDs, the location information consulted. If a matching liPath exists, its first occurrence is used.
func insertTriceIDs(w io.Writer, liPath string, in []byte, a *ant.Admin) (out []byte, modified bool, err error) {
	var idn TriceID    // idn is the last found id inside the source.
	var idN TriceID    // idN is the to be written id into the source.
	var idS string     // idS is the "iD(n)" statement, if found.
	rest := string(in) // rest is the so far not processed part of the file.
	outs := rest       // outs is the resulting string.
	var offset int     // offset is incremented by n, when rest is reduced by n.
	var delta int      // offset change cause by ID statement insertion
	var t TriceFmt     // t is the actual located trice.
	line := 1          // line counts source code lines, these start with 1.
	for {
		idn = 0                 // clear here
		idN = 0                 // clear here
		loc := matchTrice(rest) // loc is the position of the next trice type (statement name with opening parenthesis followed by a format string).
		if loc == nil {
			break // done
		}
		t.Type = rest[loc[0]:loc[1]]       // t.Type is the TRice8_2 or TRice part for example. Hint: TRice defaults to 32 bit if not configured differently.
		t.Strg = rest[loc[5]+1 : loc[6]-1] // Now we have the complete trice t (Type and Strg). We remove the double quotes wit +1 and -1.
		if loc[3] != loc[4] {              // iD(n) found
			idS = rest[loc[3]:loc[4]] // idS is where we expect n.
			nLoc := matchNb.FindStringIndex(idS)
			if nLoc == nil { // Someone wrote trice( iD(0x100), ...), trice( id(), ... ) or trice( iD(name), ...) for example.
				if Verbose {
					fmt.Fprintln(w, "unexpected syntax", idS)
				}
				line += strings.Count(rest[:loc[6]], "\n") // Keep line number up-to-date for location information.
				rest = rest[loc[6]:]
				offset += loc[6]
				continue // ignore such cases
			} else { // This is the normal case like trice( iD( 111)... .
				nStrg := idS[nLoc[0]:nLoc[1]] // nStrg is the plain number string.
				n, err := strconv.Atoi(nStrg)
				if err == nil {
					idn = TriceID(n) // idn is the assigned id inside source file.
				} else { // unexpected
					fmt.Fprintln(w, err, nStrg)                // report
					line += strings.Count(rest[:loc[6]], "\n") // Keep line number up-to-date for location information.
					rest = rest[loc[6]:]
					offset += loc[6]
					continue // ignore such cases
				}
			}
		}
		// trice t (t.Type & t.Strg) is known now. idn holds the trice id found in the source. Example cases are:
		// - trice( "foo", ... );           --> idn =   0, loc[3] == loc[4]
		// - trice( iD(0), "foo, ... ")     --> idn =   0, loc[3] != loc[4]
		// - trice( iD(111), "foo, ... ")   --> idn = 111, loc[3] != loc[4]
		a.Mutex.Lock()                          // several files could contain the same t
		if ids, ok := IDData.triceToId[t]; ok { // t has at least one unused ID, but it could be from a different file.
			var filenameMatch bool
			if len(ids) == 1 { // Most common case: just one ID for t.
				id := ids[0]
				// Even there is only one singe ID inside ids, we cannot take it, if it is for a different file.
				// ids could have been larger before and we would steel the id from a different file then.
				li, ok := IDData.idToLocRef[id] // Get location information.
				if LiPathIsRelative {
					filenameMatch = filepath.ToSlash(li.File) == liPath
				} else {
					filenameMatch = filepath.Base(li.File) == liPath
				}
				if ok && filenameMatch && (idn == 0 || idn == id) {
					// id exists inside location information for this file and is usable.
					idN = id
					delete(IDData.triceToId, t)
					goto idUsable
				}
				// If we arrive here, the location information does not match or idn != 0 and idn != id.
				// In such case a new ID is needed.
			} else { // Several IDs for t exist.
				idCandidateIndex := math.MaxInt
				idCandidateLine := math.MaxInt
				for i, id := range ids { // It is also possible, that no id matches idn != 0.
					li, ok := IDData.idToLocRef[id] // Get location information.
					if LiPathIsRelative {
						filenameMatch = filepath.ToSlash(li.File) == liPath
					} else {
						filenameMatch = filepath.Base(li.File) == liPath
					}
					if ok && filenameMatch && (idn == 0 || idn == id) {
						// id exists inside location information for this file and is usable, but it could occur
						// in path several times. In such cases we take the ID with the smallest line number first,
						// because we are reading from the beginning. Therefore we need to check that.
						if li.Line < idCandidateLine {
							idCandidateLine = li.Line
							idCandidateIndex = i
						}
					}
				}
				if idCandidateIndex < math.MaxInt { // usable, so remove from unused list.
					idN = ids[idCandidateIndex] // This gets into the source. No need to remove id from idd.idToLocRef.
					ids = removeIndex(ids, idCandidateIndex)
					if len(ids) == 0 {
						delete(IDData.triceToId, t)
					} else {
						IDData.triceToId[t] = ids
					}
					goto idUsable
				} else {
					// The case idn != 0 and idn != id is possible, when idn was manually written into the code or code with IDs was merged.
					// It is not expected, that in such cases idn is found inside idd.idToLocRef. Example:
					// TRice( iD(3), "foo" ) in file1.c && t{TRice, "foo"} gives []int{1,2}
					// li.json could contain ID 3 for file1.c, but that must be for a different trice then.

					idN = idn // IMPORTANT CHANGE!
					// Therefore such idn are discarded by not copying them to idN.
					IDData.idToTrice[idN] = t // add ID to idd.idToTrice

				}

			}
		} else if idn != 0 { // t is not known inside til.json and idn is not 0
			if tt, ok := IDData.idToTrice[idn]; ok { // idn in source is used in til.json differently
				if t == tt {
					fmt.Fprintln(w, "unexpected error!")
				}
				fmt.Fprintln(w, "ID found in", liPath, "and used for", t, "is used already in", FnJSON, "for", tt, "- assigning a new ID.")
			} else { // idn in source is not used in til.json - add idn to til.json
				IDData.idToTrice[idn] = t
				idN = idn
			}
		}
		if idN == 0 { // create a new ID
			idN = IDData.newID()
			IDData.idToTrice[idN] = t // add ID to idd.idToTrice
		}
	idUsable:
		a.Mutex.Unlock()
		line += strings.Count(rest[:loc[1]], "\n") // Update line number for location information.
		if idN != idn {                            // Need to change source.
			outs, delta = writeID(outs, offset, loc, t, idN)
			offset += delta
			modified = true
		}
		a.Mutex.Lock()
		IDData.idToLocNew[idN] = TriceLI{liPath, line} // Add to new location information.
		a.Mutex.Unlock()
		line += strings.Count(rest[loc[1]:loc[6]], "\n") // Keep line number up-to-date for location information.
		rest = rest[loc[6]:]
		offset += loc[6]
	}
	out = []byte(outs)
	return
}

// writeID inserts id into s according to loc information and returns the result together with the changed len.
func writeID(s string, offset int, loc []int, t TriceFmt, id TriceID) (result string, delta int) {
	var idName string
	if t.Type[2] == 'i' { // lower case letter
		idName = " iD("
	} else {
		if loc[3] != loc[4] {
			idName = " " + s[offset+loc[3]:offset+loc[3]+2] + "("
		} else {
			if DefaultStampSize == 32 {
				idName = " ID("
			} else if DefaultStampSize == 16 {
				idName = " Id("
			} else {
				idName = " id("
			}
		}
	}
	first := s[:offset+loc[2]]                      // first is the not touched s part before the replacement space.
	idSiz := loc[5] - loc[2]                        // idSiz is the size of the replaced ID space inside the source code.
	last := s[offset+loc[5]:]                       // last is the not touched s part after the replacement space.
	idIns := idName + strconv.Itoa(int(id)) + "), " // idIns is the ID statement replace string.
	result = first + idIns + last                   //
	delta = len(idIns) - idSiz                      // delta is the offset change.
	return
}

// cleanID inserts id 0 into s or removes ID statement according to loc information and returns the result together with the changed len.
func cleanID(s string, offset int, loc []int, t TriceFmt) (result string, delta int) {
	// code is similar to writeiD code for clarity.
	if t.Type[2] == 'I' { // Upper case letter (s.th. like TRICE*...), we set id just to 0.
		return writeID(s, offset, loc, t, 0)
	}
	first := s[:offset+loc[2]]    // first is the not touched s part before the replacement space.
	idSiz := loc[5] - loc[2]      // idSiz is the size of the replaced ID space inside the source code.
	last := s[offset+loc[5]:]     // last is the not touched s part after the replacement space.
	idIns := " "                  // replacement string
	result = first + idIns + last //
	delta = len(idIns) - idSiz    // delta is the offset change.
	return
}

// stringLiterals is explained in https://stackoverflow.com/questions/76587323.
var stringLiterals bufio.SplitFunc = func(data []byte, atEOF bool) (advance int, token []byte, err error) {
	scanning := false
	var delim byte
	var i int
	var start, end int
	for i < len(data) {
		b := data[i]
		switch b {
		case '\\': // skip escape sequences
			i += 2
			continue
		case '"':
			// If in a comment a ' occurs, this is taken as start point of a format string.
			// So we do not allow trice( 'hi') and trice( `hi` ).
			// One problem remains: A single unescaped " inside a comment excludes the rest of the file.
			//  	fallthrough
			//  case '\'':
			//  	fallthrough
			//  case '`':
			if scanning && delim == b {
				end = i + 1
				token = data[start:end]
				advance = end
				return
			} else if !scanning {
				scanning = true
				start = i
				delim = b
			}
		}
		i++
	}
	if atEOF {
		return len(data), nil, nil
	}
	return start, nil, nil
}

// matchFormatString returns a two-element slice of integers defining the location of the leftmost match in s of the matchFmtString regular expression.
// The match itself is at s[loc[0]:loc[1]]. A return value of nil indicates no match.
// If the format string contains `\"` elements, the found sub strings are concatenated to the returned result.
func matchFormatString(input string) (loc []int) {
	scanner := bufio.NewScanner(strings.NewReader(input))
	scanner.Split(stringLiterals)
	if scanner.Scan() {
		loc = append(loc, strings.Index(input, scanner.Text()))
		loc = append(loc, loc[0]+len(scanner.Text()))
	}
	return
}

// removeIndex removes the element at index from s.
// https://stackoverflow.com/questions/37334119/how-to-delete-an-element-from-a-slice-in-golang
func removeIndex(s TriceIDs, index int) TriceIDs {
	return append(s[:index], s[index+1:]...)
}
