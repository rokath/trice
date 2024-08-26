// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

// source tree management

import (
	"bufio"
	"errors"
	"fmt"
	"io"
	"log"
	"math"
	"os"
	"path"
	"path/filepath"
	"slices"
	"strconv"
	"strings"

	"github.com/rokath/trice/pkg/ant"
	"github.com/spf13/afero"
)

// CompactSrcs adds local dir to Srcs if Srcs is empty and reduces variable Scrs to the minimum to address all intended folders.
func CompactSrcs() {
	if len(Srcs) == 0 { // Srcs is an array flag containing desired folders & files
		Srcs = append(Srcs, "./") // default value
	}
	for i, x := range Srcs {
		Srcs[i] = path.Clean(x)
	}
	slices.Sort(Srcs)
	Srcs = slices.Compact(Srcs)
}

var SkipAdditionalChecks bool

// SubCmdIdInsert performs sub-command insert, adding trice IDs to source tree.
func SubCmdIdInsert(w io.Writer, fSys *afero.Afero) (e error) {
	e = IDData.cmdSwitchTriceIDs(w, fSys, IDData.triceIDInsertion)
	if e != nil {
		return e
	}
	return
}

// triceIDInsertion reads file, processes it and writes it back, if needed.
func (p *idData) triceIDInsertion(w io.Writer, fSys *afero.Afero, path string, fileInfo os.FileInfo, a *ant.Admin) error {
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

	out, modified, err := p.insertTriceIDs(w, liPath, in, a)
	if err != nil {
		return err
	}

	if filepath.Base(path) == "triceConfig.h" {
		outs := string(out)
		x := strings.Index(outs, "#define TRICE_CLEAN 1")
		if x != -1 { // found
			outs := strings.Replace(outs, "#define TRICE_CLEAN 1", "#define TRICE_CLEAN 0", 1)
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
		}
	}

	return err
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
// in is the read file liPath content and out is the file content which needs to be written.
// a is used for mutex access to IDData. liPath is needed for location information.
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
//   - If trice is assigned to several IDs, the location information consulted. If a matching liPath exists, its first occurrence is used.
//
// - idInSourceIsNonZero, id is inside p.idToTrice with matching trice and not in p.triceToId -> used ID! -> create new ID && invalidate ID in source
// - idInSourceIsNonZero, id is inside p.idToTrice with different trice                       -> used ID! -> create new ID && invalidate ID in source
// - idInSourceIsNonZero, id is not inside p.idToTrice (cannot be inside p.triceToId)         -> add ID to p.idToTrice
// - idInSourceIsZero,    trice is not inside p.triceToId                                     -> create new ID & add ID to p.idToTrice
// - idInSourceIsZero,    trice is is inside p.triceToId                                      -> unused ID -> use ID (remove from p.triceToId)
//   - If trice is assigned to several IDs, the location information consulted. If a matching liPath exists, its first occurrence is used.
func (p *idData) insertTriceIDs(w io.Writer, liPath string, in []byte, a *ant.Admin) (out []byte, modified bool, err error) {
	var idn TriceID    // idn is the last found id inside the source.
	var idN TriceID    // idN is the to be written id into the source.
	var idS string     // idS is the "iD(n)" statement, if found.
	rest := string(in) // rest is the so far not processed part of the file.
	outs := rest       // outs is the resulting string.
	var offset int     // offset is incremented by n, when rest is reduced by n.
	var delta int      // offset change cause by ID statement insertion
	var t TriceFmt     // t is the actual located trice.
	line := 1          // line counts source code lines, these start with 1.
	for {              // file loop
		idn = 0                 // clear here
		idN = 0                 // clear here
		loc := matchTrice(rest) // loc is the position of the next trice type (statement name with opening parenthesis followed by a format string).
		if loc == nil {
			break // done
		}
		t.Type = rest[loc[0]:loc[1]]       // t.Type is the TRice8_2 or TRice part for example. Hint: TRice defaults to 32 bit if not configured differently.
		t.Strg = rest[loc[5]+1 : loc[6]-1] // Now we have the complete trice t (Type and Strg). We remove the double quotes wit +1 and -1.
		if !SkipAdditionalChecks {
			err = evaluateTriceParameterCount(t, line, rest[loc[6]:])
			if err != nil {
				return
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
			fmt.Fprintln(w, "Trice", t, "with ID", idn, "found in source file", liPath, ".")
		}
		// Example cases are:
		// - trice( "foo", ... );           --> idn =   0, loc[3] == loc[4]
		// - trice( iD(0), "foo, ... ")     --> idn =   0, loc[3] != loc[4]
		// - trice( iD(111), "foo, ... ")   --> idn = 111, loc[3] != loc[4]
		a.Mutex.Lock() // several files could contain the same t

		// process t
		ids := p.triceToId[t]
		if Verbose {
			fmt.Fprintln(w, "Trice ", t, " has", len(ids), "unused ID(s), but could be from different file(s). IDs=", ids, ".")
		}
		var filenameMatch bool
		idCandidateIndex := math.MaxInt
		idCandidateLine := math.MaxInt
	idsLoopEntry:
		// id slice ids loop
		for i, id := range ids { // It is possible, that idn == 0 or no id matches idn != 0 or ids is nil.
			if id == 0 {
				log.Fatal("id == 0   : unexpected case")
			}
			if idn != 0 && id != idn {
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
					fmt.Fprintln(w, "ID", idn, "has no location infomation, so we simply use this ID.")
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
			if LiPathIsRelative {
				filenameMatch = filepath.ToSlash(li.File) == liPath
			} else {
				filenameMatch = filepath.Base(li.File) == liPath
			}
			if !filenameMatch {
				if Verbose {
					fmt.Fprintln(w, "ID", id, "is from a different file: -> continue")
				}
				continue
			}
			// id is from the same file: use idn -> idUsable
			// id exists inside location information for this file and is usable, but it could occur
			// in file several times. In such cases we take the ID with the smallest line number first,
			// because we are reading from the beginning. Therefore we need to check that.
			if li.Line < idCandidateLine {
				idCandidateLine = li.Line
				idCandidateIndex = i
			}
			if idCandidateIndex < math.MaxInt {
				if idn == 0 { // This is the src was cleaned before case.
					if Verbose {
						fmt.Fprintln(w, "Even ID", id, "is maybe not part of the IDSpace, we use it again.")
					}
					idN = id
					ids = removeIDFromSlice(ids, idN)
					p.triceToId[t] = ids
					break
				}
				if Verbose {
					fmt.Fprintln(w, "ID", idn, "usable, so remove it from unused list.")
				}
				idN = ids[idCandidateIndex] // This gets into the source. No need to remove id from p.idToLocRef.
				ids = removeIndex(ids, idCandidateIndex)
				// remove id from ids now
				if len(ids) == 0 {
					delete(p.triceToId, t)
				} else {
					p.triceToId[t] = ids
				}
				break
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
			idN = p.newID()
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
				fmt.Fprintln(w, "Need to change source.", idn, " -> ", idN, " for ", t, "in file", liPath)
			}
			outs, delta = writeID(outs, offset, loc, t, idN)
			offset += delta
			modified = true
		}
		a.Mutex.Lock()
		if Verbose {
			fmt.Fprintln(w, "Add to new location information. ID:", idN, liPath, line)
		}
		p.idToLocNew[idN] = TriceLI{liPath, line}
		a.Mutex.Unlock()
		line += strings.Count(rest[loc[1]:loc[6]], "\n") // Keep line number up-to-date for location information.
		rest = rest[loc[6]:]
		offset += loc[6]
	}
	out = []byte(outs)
	return
}

// evaluateTriceParameterCount analyzes rest, if it has the correct parameter count according to t.
// rest starts immediately after the trice format string and can be very long (the remaining file contet),
// but we check only until the trice end and return nil if the evaluation is ok.
func evaluateTriceParameterCount(t TriceFmt, line int, rest string) (err error) {
	fsc := formatSpecifierCount(t.Strg)
	// At this stage we know the end of the format string (start of rest)
	// and also the count of the format specifier in it.
	// What we do not know, is, where the right closing bracket is. So we can expect fsc colons now:
	//     trice( "%02x, %02x", a, (b+c) );
	// But there are special cases, like:
	//     triceS( "%80s", ",,," );
	//     triceB( "dbg: %02X\n", buf, sizeof(buf));
	//     trice16F( "rpc:AFunctionName", buf, sizeof(buf));
	//     trice( "%d", SUM(3,4));
	cnt, err := countColonsUntilClosingBracket(rest)
	if err != nil {
		return err
	}
	lastChar := t.Type[len(t.Type)-1:]
	switch lastChar {
	case "S": // expect one colon followed by a string and a closing bracket
		if fsc != 1 {
			return fmt.Errorf("line %d %v should have exactly one format specifier and not %d", line, t, fsc)
		}
		if cnt != 1 {
			return fmt.Errorf("line %d %v should have exactly one parameter and not %d", line, t, cnt)
		}
	case "N":
		if fsc != 1 {
			return fmt.Errorf("line %d %v should have exactly one format specifier and not %d", line, t, fsc)
		}
		if cnt != 2 {
			return fmt.Errorf("line %d %v should have exactly two parameters and not %d", line, t, cnt)
		}
	case "B":
		if fsc != 1 {
			return fmt.Errorf("line %d %v should have exactly one format specifier and not %d", line, t, fsc)
		}
		if cnt != 2 {
			return fmt.Errorf("line %d %v should have exactly two parameters and not %d", line, t, cnt)
		}
	case "F":
		if fsc != 0 {
			return fmt.Errorf("line %d %v should have no format specifier and not %d", line, t, fsc)
		}
		if cnt != 2 {
			return fmt.Errorf("line %d %v should have exactly two parameters and not %d", line, t, cnt)
		}
	default:
		if strings.Contains(t.Type, "Assert") { // matches triceAssertTrue and triceAssertFalse
			if fsc != 0 || cnt != 1 {
				return fmt.Errorf("line %d %v should have no format specifiers and eactly one parameter, the boolean value (fsc=%d, cnt=%d)", line, t, fsc, cnt)
			}
			return
		}
		if fsc != cnt {
			return fmt.Errorf("line %d format specifier count %d != parameter count %d for %v", line, fsc, cnt, t)
		}
	}
	return
}

// countColonsUntilClosingBracket parses s until the first closing bracket
// and determines the number of colons.
// If there is in rest a starting string, all until the string end is ignored.
// If there is in rest a opening bracket, all until the next matching closing bracket is ignored.
func countColonsUntilClosingBracket(rest string) (count int, e error) {
	s := rest
restart:
	for i, c := range s {
		switch c {
		case '"':
			loc := matchStringLiteral(s[i:])
			if loc == nil {
				return count, errors.New("invalid:" + rest)
			}
			s = s[i:]
			s = s[loc[1]:]
			goto restart
		case '(':
			s = s[i+1:] // cut off including '('
			pos, e := matchBracketLiteral(s)
			if e != nil {
				return count, fmt.Errorf("invalid:%s", rest)
			}
			s = s[pos+1:] // cut off including ')'
			goto restart
		case ',':
			count++
		case ')':
			return count, nil
		}
	}
	return count, errors.New("no matching closing bracket found in: " + rest)
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

// matchStringLiteral returns a two-element slice of integers defining the location of the leftmost match in s of the matchFmtString regular expression.
// The match itself is at s[loc[0]:loc[1]]. A return value of nil indicates no match.
// If the format string contains `\"` elements, the found sub strings are concatenated to the returned result.
func matchStringLiteral(input string) (loc []int) {
	scanner := bufio.NewScanner(strings.NewReader(input))
	scanner.Split(stringLiterals)
	if scanner.Scan() {
		loc = append(loc, strings.Index(input, scanner.Text()))
		loc = append(loc, loc[0]+len(scanner.Text()))
	}
	return
}

// matchBracketLiteral assumes, the input starts with the very first character after an opening '('
// and returns the index of the closing ')'. Examples:
//
//	input,          index,  e
//	")"		          0   nil
//	"), )"		      0   nil
//	"M(a,")"), a)"	 11   nil
//	"."               0   err
func matchBracketLiteral(input string) (index int, e error) {
	s := input
restart:
	for i, c := range s {
		switch c {
		case ')':
			index += i
			return
		case '(': // enter recursion
			s = s[i+1:]    // cut off until behind '('
			index += i + 1 // adjust
			var idx int
			idx, e = matchBracketLiteral(s)
			if e != nil {
				return
			}
			s = s[idx+1:]    // cut off until behind ')'
			index += idx + 1 // adjust
			goto restart
		case '"':
			s = s[i:]
			index += i
			loc := matchStringLiteral(s)
			s = s[loc[1]:]
			index += loc[1]
			goto restart
		}
	}
	e = errors.New("no closing bracket in:" + input)
	return
}

// removeIndex removes the element at index from s.
// https://stackoverflow.com/questions/37334119/how-to-delete-an-element-from-a-slice-in-golang
func removeIndex(s TriceIDs, index int) TriceIDs {
	return append(s[:index], s[index+1:]...)
}
