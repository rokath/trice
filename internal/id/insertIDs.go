// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

// source tree management

import (
	"fmt"
	"io"
	"log"
	"math"
	"os"
	"path/filepath"
	"strconv"
	"strings"

	"github.com/rokath/trice/pkg/ant"
	"github.com/rokath/trice/pkg/msg"
	"github.com/spf13/afero"
)

// SubCmdIdInsert performs sub-command insert, adding trice IDs to source tree.
func SubCmdIdInsert(w io.Writer, fSys *afero.Afero) (e error) {
	e = IDData.cmdSwitchTriceIDs(w, fSys, IDData.triceIDInsertion)
	if e != nil {
		return e
	}
	return
}

// processTriceIDInsertion reads file, processes it and writes it back, if needed.
func (p *idData) processTriceIDInsertion(w io.Writer, fSys *afero.Afero, path string, fileInfo os.FileInfo, a *ant.Admin) error {
	if p.err != nil {
		return p.err
	}
	msg.Tell(w, "process inserting")
	// The file has an mtime from last user edit and we keep this as reference.
	// Inserting IDs takes part as an for the makefile invisible action.

	in, err := fSys.ReadFile(path)
	p.join(err)
	msg.Tell(w, path)

	out, modified, err := p.insertTriceIDs(w, toLIPath(path), in, a)
	p.join(err)

	if filepath.Base(path) == "triceConfig.h" && p.err == nil {
		outs := string(out)
		x := strings.Index(outs, "#define TRICE_CLEAN 1")
		if x != -1 { // found
			outs := strings.Replace(outs, "#define TRICE_CLEAN 1", "#define TRICE_CLEAN 0", 1)
			out = []byte(outs)
			modified = true
		}
	}
	if modified { // IDs inserted
		if !DryRun && p.err == nil {
			err = fSys.WriteFile(path, out, fileInfo.Mode())
			p.join(err)
		}
	}
	return p.err
}

// removeIDFromSlice searches ids for id, removes its first occurance and returns the result.
func removeIDFromSlice(ids []TriceID, id TriceID) []TriceID {
	for index, iD := range ids {
		if iD == id {
			ids[index] = ids[len(ids)-1] // copy last element to index
			ids = ids[:len(ids)-1]       // remove last element
			return ids
		}
	}
	return ids // unchanged, if id not found inside ids
}

// insertTriceIDs does the ID insertion task on in and returns the result in out with modified==true when out != in.
//
// in is the read file path content and out is the file content which needs to be written.
// a is used for mutex access to IDData. path is needed for location information.
// insertTriceIDs is intended to be used in several Go routines (one for each file) for faster ID insertion.
// Data usage:
// - p.idToTrice is the serialized til.json. It is extended with unknown and new IDs and written back to til.json finally.
// - p.triceToId is the initially reverted p.idToTrice. It is shrunk for each used ID amd used to find out if an ID is already fresh used.
//   - When starting, p.triceToId holds all IDs from til.json and no ID is fresh used yet. If an ID is to be (fresh) used it is removed from p.triceToId.
//   - If an ID is found in p.idToTrice but not found in p.triceToId anymore, it is already (fresh) used and not usable again.
//   - If a new ID is generated, it is added to p.idToTrice only. This way it gets automatically used.
//
// - p.idToLocRef is only for reference and not changed. It is the "old" location information.
// - p.idToLocNew is new generated during insertTriceIDs execution and finally written back to li.json as "new" location information.
// For reference look into file TriceUserGuide.md part "The `trice insert` Algorithm".
// insertTriceIDs parses the file content from the beginning for the next trice statement, deals with it and continues until the file content end.
// When a trice statement was found, general cases are:
// - idInSourceIsNonZero, id is inside p.idToTrice with matching trice and inside p.triceToId -> use ID (remove from p.triceToId)
//   - If trice is assigned to several IDs, the location information consulted. If a matching path exists, its first occurrence is used.
//
// - idInSourceIsNonZero, id is inside p.idToTrice with matching trice and not in p.triceToId -> used ID! -> create new ID && invalidate ID in source
// - idInSourceIsNonZero, id is inside p.idToTrice with different trice                       -> used ID! -> create new ID && invalidate ID in source
// - idInSourceIsNonZero, id is not inside p.idToTrice (cannot be inside p.triceToId)         -> add ID to p.idToTrice
// - idInSourceIsZero,    trice is not inside p.triceToId                                     -> create new ID & add ID to p.idToTrice
// - idInSourceIsZero,    trice is is inside p.triceToId                                      -> unused ID -> use ID (remove from p.triceToId)
//   - If trice is assigned to several IDs, the location information consulted. If a matching path exists, its first occurrence is used.
func (p *idData) insertTriceIDs(w io.Writer, path string, in []byte, a *ant.Admin) (out []byte, modified bool, err error) {
	var idn TriceID    // idn is the last found id inside the source.
	var idN TriceID    // idN is the to be written id into the source.
	var idS string     // idS is the "iD(n)" statement, if found.
	rest := string(in) // rest is the so far not processed part of the file.
	outs := rest       // outs is the resulting string.
	var offset int     // offset is incremented by n, when rest is reduced by n.
	var delta int      // offset change cause by ID statement insertion
	var t TriceFmt     // t is the actual located trice.
	line := 1          // line counts source code lines, these start with 1.
	if p.err != nil {
		return
	}
	var fileIds TriceIDs // Here we collect all Trice IDs of a Trice in the current file.
	for {                // file loop
		idn = 0                 // clear here, idn gets a != 0 value, if the actual Trice has already an ID != 0.
		idN = 0                 // clear here, idN gets assigned an ID for writing that value to the Trice.
		loc := matchTrice(rest) // loc is the position of the next trice type (statement name with opening parenthesis followed by a format string).
		if loc == nil {
			break // done
		}
		t.Type = rest[loc[0]:loc[1]]       // t.Type is the TRice8_2 or TRice part for example. Hint: TRice defaults to 32 bit if not configured differently.
		t.Strg = rest[loc[5]+1 : loc[6]-1] // Now we have the complete trice t (Type and Strg). We remove the double quotes wit +1 and -1.
		if !SkipAdditionalChecks {
			linesOffset := strings.Count(rest[:loc[6]], "\n")
			err = evaluateTriceParameterCount(t, line+linesOffset, rest[loc[6]:])
			if err != nil {
				fmt.Fprintln(w, path, err)
				os.Exit(-1)
			}
		}
		if loc[3] != loc[4] { // iD(n) found
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
					idn = TriceID(n)
					if Verbose {
						fmt.Fprintln(w, "ID ", idn, " is the assigned id inside source file.")
					}
				} else {
					fmt.Fprintln(w, "unexpected ", err, nStrg) // report
					line += strings.Count(rest[:loc[6]], "\n") // Keep line number up-to-date for location information.
					rest = rest[loc[6]:]
					offset += loc[6]
					continue // ignore such cases
				}
			}
		}
		if Verbose {
			fmt.Fprintln(w, "Trice", t, "with ID", idn, "found in source file", path, ".")
		}
		// Example cases are:
		// - trice( "foo", ... );           --> idn =   0, loc[3] == loc[4]
		// - trice( iD(0), "foo, ... ")     --> idn =   0, loc[3] != loc[4]
		// - trice( iD(111), "foo, ... ")   --> idn = 111, loc[3] != loc[4]
		a.Mutex.Lock() // several files could contain the same t

		// process t
		ids := p.triceToId[t] // ids contains all ID´s of a Trice.
		if Verbose {
			fmt.Fprintln(w, "Trice ", t, " has", len(ids), "unused ID(s), but could be from different file(s). IDs=", ids, ".")
		}
		var filenameMatch bool
		idSmallestLineIndex := math.MaxInt
		idCandidateLine := math.MaxInt
	idsLoopEntry:
		// id slice ids loop
		// ids could contain a bunch of ID´s, all fitting for the same Trice.
		// We need first to filter out all ID´s of the actual file.
		// Afterwards we take the ID with the smallest line number.
		for _, id := range ids { // It is possible, that idn == 0 or no id matches idn != 0 or ids is nil.
			if id == 0 {
				log.Fatal("id == 0   : unexpected case") // ids should not contain a 0.
			}
			if idn != 0 && id != idn { // Trice has an ID, idn, but that is not the one we are processing right now.
				if Verbose {
					// The idn value does not match. If idn exists not at all inside ids we will later add it to til.json. See func TestAddIDToTilJSON.
					// In func TestGenerateNewIDIfUsedToTilJSON we face the issue, that idn is not found inside ids as well, because it is used for a different trice.
					// But we could assign one of the existing id inside ids to idn. For that we use goto idsLoopEntry later.
					fmt.Fprintln(w, "ID", idn, "!=", id, "continue...")
				}
				continue
			}
			// id == idn or idn == 0 here
			li, ok := p.idToLocRef[id] // Get location information.
			if !ok {
				if Verbose {
					fmt.Fprintln(w, "ID", idn, "has no location information, so we simply use this ID.")
				}
				if idn != 0 {
					if Verbose {
						fmt.Fprintln(w, "ID", idn, "is usable, so we simply use this ID and remove id from ids now.")
					}
					idN = idn
					ids = removeIDFromSlice(ids, idN)
					p.triceToId[t] = ids
				}
				break
			}
			filenameMatch = path == toLIPath(li.File)

			if !filenameMatch {
				if Verbose {
					fmt.Fprintln(w, "ID", id, "is from a different file: -> continue")
				}
				continue
			}
			// id is from the same file: use idn -> idUsable
			// id exists inside location information for this file and is usable, but it could occur in file several times.
			fileIds = append(fileIds, id)
		}
		for i, id := range fileIds { // It is possible, that idn == 0 or no id matches idn != 0 or ids is nil.
			li := p.idToLocRef[id] // Get location information.

			//In such cases we take the ID with the smallest line number first,
			// because we are reading from the beginning. Therefore we need to check that.
			if li.Line < idCandidateLine {
				idCandidateLine = li.Line
				idSmallestLineIndex = i
			}
		}
		if idSmallestLineIndex < math.MaxInt {
			id := fileIds[idSmallestLineIndex]
			if idn == 0 { // This is the "src was cleaned before" case.
				if Verbose {
					fmt.Fprintln(w, "Even ID", id, "is maybe not part of the IDSpace, we use it again.")
				}
				idN = id
				ids = removeIDFromSlice(ids, idN)
				p.triceToId[t] = ids
			}
			if Verbose {
				fmt.Fprintln(w, "ID", idn, "usable, so remove it from unused list.")
			}
			idN = id              // ids[idSmallestLineIndex] // This gets into the source. No need to remove id from p.idToLocRef.
			fileIds = fileIds[:0] // discard fileIds values

			// remove id from ids now
			if len(ids) == 0 {
				delete(p.triceToId, t)
			} else {
				p.triceToId[t] = ids
			}
		}

		if Verbose {
			fmt.Fprintln(w, "If no match was found inside ids we assign the ID found in source file.")
		}
		if idN == 0 {
			tm, used := p.idToTrice[idn]
			if !used {
				if Verbose {
					fmt.Fprintln(w, "It is possible, that idn is 0, so we assign it to idN", idn, "-->", idN)
				}
				idN = idn // It is possible, that idn is 0, for example when `TRICE( id(0), "Hi!" );` is inside src.
			} else {
				if tm != t {
					if Verbose {
						fmt.Fprintln(w, "ID", idn, "used in TIL with tm=", tm, " but in src it is used for t=", t, "Force changing the ID.")
					}
					if len(ids) > 0 { // At this point we could take an id from ids.
						if Verbose {
							fmt.Fprintln(w, "Discard value found in src. (case ID in src already used differently):", idn)
						}
						idn = 0
						goto idsLoopEntry
					}
				} else {
					if Verbose {
						fmt.Fprintln(w, "ID", idn, "used in TIL and in src for t=", tm)
					}
					if len(ids) > 0 {
						if Verbose {
							fmt.Fprintln(w, "ids=", ids, "so take an ID from there.")
						}
						if Verbose {
							fmt.Fprintln(w, "Discard value found in src. (case line duplication):", idn)
						}
						idn = 0
						goto idsLoopEntry
					}
				}
			}
		}
		if idN == 0 { // newID
			idN = p.newID(t) // t.Strg contains the Trice tag information: string until the first colon matches a string in emitter.colorChannels.
			if Verbose {
				fmt.Fprintln(w, "Create a new ID ", idN, " for ", t)
			}
		} else {
			p.removeIDFromIDSpace(TriceID(idN)) // When an ID was found in source file, it could be part of the IDSpace, so we need to remove it to avoid double usage.
		}
		p.idToTrice[idN] = t // add ID to p.idToTrice
		a.Mutex.Unlock()
		line += strings.Count(rest[:loc[1]], "\n") // Update line number for location information.
		if idN != idn {
			if Verbose {
				fmt.Fprintln(w, "Need to change source.", idn, " -> ", idN, " for ", t, "in file", path)
			}
			outs, delta = writeID(outs, offset, loc, t, idN)
			offset += delta
			modified = true
		}
		a.Mutex.Lock()
		if Verbose {
			fmt.Fprintln(w, "Add to new location information. ID:", idN, path, line)
		}
		p.idToLocNew[idN] = TriceLI{path, line}
		a.Mutex.Unlock()
		line += strings.Count(rest[loc[1]:loc[6]], "\n") // Keep line number up-to-date for location information.
		rest = rest[loc[6]:]
		offset += loc[6]
	}
	out = []byte(outs)
	return
}
