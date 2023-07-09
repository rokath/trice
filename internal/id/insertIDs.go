// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

// source tree management

import (
	"bufio"
	"fmt"
	"io"
	"os"
	"strconv"
	"strings"

	"github.com/rokath/trice/pkg/ant"
	"github.com/spf13/afero"
)

// SubCmdIdInsert performs sub-command insert, adding trice IDs to source tree.
func SubCmdIdInsert(w io.Writer, fSys *afero.Afero) error {
	return cmdManageTriceIDs(w, fSys, triceIDInsertion)
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

	out, fileModified, err := insertTriceIDs(w, path, in, a)
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

type idParseStateType int

const (
	normal idParseStateType = iota
	noIdStatementFound
	//idFoundButWithoutNumber
	//unexpectedCannotConvertNIntoNumber
)

type idStateType int

const (
	idInSourceIsZero idStateType = iota
	idInSourceIsOK
	idInSourceIsUnknown
	idInSourceNeedsInvalidation
)

type fmtStateType int

const (
	fmtNotKnown fmtStateType = iota
	fmtIsKnown
)

// removeID removes one id from ids and returns that. It writes the reduced ids back to the triceToId map.
// If the reduced ids is empty, it removes the t key from the triceToId map.
func removeID(t TriceFmt, ids []TriceID, triceToId triceFmtLookUp) TriceID {
	return 0
}

// insertTriceIDs does the ID insertion task on in. insertTriceIDs uses internally local pointer idd because idd cannot be easily passed via parameters.
// insertTriceIDs returns the result in out with modified==true when out != in.
// in is the read file path content and out is the file content which needs to be written.
// a is used for mutex access to idd data. path is needed for location information.
// Data usage:
// - idd.idToTrice is the serialized til.json. It is extended with unknown and new IDs and written back to til.json finally.
// - idd.triceToId is the initially reverted idd.idToTrice. It is shrinked for each used ID amd used to find out if an ID is already fresh used.
//   - When starting idd.triceToId holds all IDs from til.json and no ID is fresh used yet. If an ID is to be (fresh) used it is removed from idd.triceToId.
//   - If an ID is found in idd.idToTrice but not found in idd.triceToId anymore, it is already (fresh) used and not usable again.
//   - If a new ID is generated, it is added to idd.idToTrice only. This way it gets automatically used.
//
// - idd.idToLocRef is only for reference (Refinement).
// - idd.idToLocNew is new generated during insertTriceIDs execution.
// For reference look into file TriceUserGuide.md part "The `trice insert` Algorithm".
// insertTriceIDs is executed parallel in many go routines, one for each file.
// It parses the file content from the beginning for the next trice statement, deals with it and continues until the end.
// When a trice statement was found, general cases are:
// - idInSourceIsNonZero, id is inside idd.idToTrice with matching trice and inside idd.triceToId -> use ID (remove from idd.triceToId)
// - idInSourceIsNonZero, id is inside idd.idToTrice with matching trice and not in idd.triceToId -> used ID! -> create new ID && invalidate ID in source
// - idInSourceIsNonZero, id is inside idd.idToTrice with different trice                         -> used ID! -> create new ID && invalidate ID in source
// - idInSourceIsNonZero, id is not inside idd.idToTrice (cannot be inside idd.triceToId)         -> add ID to idd.idToTrice
// - idInSourceIsZero,    trice is not inside idd.triceToId                                       -> create new ID
// - idInSourceIsZero,    trice is is inside idd.triceToId                                        -> unused ID -> use ID (remove from idd.triceToId)
//   - Refinement: If trice is assigned to several IDs, the location information could be consulted. If a matching path exists, its first occurance is used.
func insertTriceIDs(w io.Writer, path string, in []byte, a *ant.Admin) (out []byte, modified bool, err error) {
	var idParseState idParseStateType
	var idn TriceID     // idn is the last found id inside the source.
	rest := string(in)  // rest is the so far not processed part of the file
	var fmtSpace string // fmtSpace starts behind ID(n), if ID(n) exists, otherwise it starts behind found trice type.
	var idOld string    // idOld is the "iD(n)" part, if found otherwise "(".
	var idNew string    // idNew is the "iD(n)" part for replacement of idOld
	line := 1           // line counts source code lines, these start with 1.

	for {
		// idn = 0
		idParseState = normal
		typeLoc := matchTrice(rest) // typeLoc is the position of the next trice type (statement name without following parentheses).
		if typeLoc == nil {
			break // done
		}
		var t TriceFmt
		t.Type = rest[typeLoc[0]:typeLoc[1]]           // t.Type is the TRice8_2 or TRice part for example. Hint: TRice defaults to 32 bit if not configured differently.
		line += strings.Count(rest[:typeLoc[1]], "\n") // Keep line number up-to-date for location information.
		rest = rest[typeLoc[1]:]                       // We can set a new starting point here.
		idLoc := matchNbID.FindStringIndex(rest)       // idLoc is the space for an eventually following ID(n).
		if idLoc == nil {                              // no ID(n) found
			idOld = "(" // just the opening parenthesis after t.Type as a replacement anchor
			fmtSpace = rest
			idParseState = noIdStatementFound
			idn = 0
		} else { // ID(n) found
			idOld = rest[idLoc[0]:idLoc[1]] // idOld is where we expect n. Also usable as replacement anchor, if n needs modification.
			fmtSpace = rest[idLoc[1]:]
			nLoc := matchNb.FindStringIndex(idOld)
			if nLoc == nil { // Someone wrote trice( iD(0x100), ...), trice( id(), ... ) or trice( iD(name), ...) for example.
				if Verbose {
					fmt.Fprintln(w, "unexpected syntax", idOld)
				}
				continue // ignore such cases
			} else { // This is the normal case like trice( iD( 111)... .
				nStrg := idOld[nLoc[0]:nLoc[1]] // nStrng is the plain number string
				n, err := strconv.Atoi(nStrg)
				if err != nil {
					if Verbose {
						fmt.Fprintln(w, err, nStrg)
					}
					continue // ignore such cases
				} else { // ok
					idParseState = normal
					idn = TriceID(n) // idn is the assigned id inside source file.
				}
			}
		}
		fmtLoc := matchFormatString(fmtSpace)
		if fmtLoc == nil {
			fmt.Fprintln(w, "No fmt string found after", t.Type)
			rest = fmtSpace // Skip (ID(n) if there was one. Example: trice( foo ) is ignored.
			continue        // This is unexpected, so we ignore, but report that.
		}
		t.Strg = fmtSpace[fmtLoc[0]:fmtLoc[1]] // Now we have the complete trice t (Type and Strg)

		// example cases:
		// trice( "foo", ... );           --> idn =   0, idParseState = noIdStatementFound
		// trice( iD(0), "foo, ... ")     --> idn =   0, idParseState = normal
		// trice( iD(111), "foo, ... ")   --> idn = 111, idParseState = normal

		// state here:
		// trice t (t.Type & t.Strg) is known.
		// rest starts at first char after trice name. We need to keep that point for later ID(n) manipulation.
		// fmtSpace starts after ID(n). We need to keep that point for later ID(n) manipulation.
		// idn holds the trice id found in the source and idParseState is set.

		if idn == 0 { // normal case
			if ids, ok := idd.triceToId[t]; ok { // unused ID -> use ID (remove from idd.triceToId)
				idn = removeID(t, ids, idd.triceToId)
				//writeID()

			}

		}

		var idState idStateType
		if idn != 0 {
			triceItem, ok := idd.idToTrice[idn]
			if ok { // ID inside souce code is inside til.json
				if triceItem != t { // but for a different trice.
					idState = idInSourceNeedsInvalidation
				} else { // matches same trice
					idState = idInSourceIsOK
				}
			} else { // ID inside souce code is not inside til.json
				idState = idInSourceIsUnknown
			}
		}

		// state here:
		// idParseState        idState                       idn fmtState
		// normal              idInSourceNeedsInvalidation   != 0
		// normal              idInSourceIsOK                != 0
		// normal              idInSourceIsUnknown           != 0
		// normal                                            == 0
		// noIdStatementFound  (nil)                         == 0

		a.Mutex.Lock()
		var fmtState fmtStateType
		idf, okf := idd.triceToId[t] // idf holds all available trice IDs for t.
		if !okf {
			a.Mutex.Unlock()
			fmtState = fmtNotKnown
		} else { // found (x times) inside til.json
			fmtState = fmtIsKnown
			if len(idf) == 1 { // t occurs only one time, what is mostly the case.
				if idn == 0 { // id inside source is 0 what is the normal case.
					idState = idInSourceIsZero
					delete(idd.triceToId, t) // We do not care if it is in an other file because til.json says, that t is used only one times.
					// Code could be copied into other files. Such cases are rare and we accept that an ID can move to an other file then.
				} else if idn == idf[0] { // id inside source is not 0 and identical with value inside til.json what is a normal case.
					idState = idInSourceIsOK
					delete(idd.triceToId, t) // not usable again
				} else { // idn is not 0 and not inside til.json. That is the case after importing new sources with IDs.
					idState = idInSourceIsUnknown
					// Do not delete t from idd.fmtToId, because it refers to a different ID, if at all there.
					idd.idToTrice[idn] = t // Extend til.json.
				}
				a.Mutex.Unlock()
				fmt.Fprintln(w, fmtState)
				if idState == idInSourceIsOK { // The ID is already inside source file.
					rest = fmtSpace[fmtLoc[1]:] // reduce search space
					// todo: extend location information
					continue
				} else if idState == idInSourceIsUnknown { // The ID inside source file is now integrated into ti.json.
					rest = fmtSpace[fmtLoc[1]:] // reduce search space
					// todo: extend location information
					continue
				} else { // patch source
					var idName string
					if idParseState == noIdStatementFound { // TRice..., Trice..., trice..., TRICE... are the cases
						if t.Type[2] == 'i' { // small letter
							idName = "iD"
						} else {
							if DefaultStampSize == 32 {
								idName = "ID"
							} else if DefaultStampSize == 16 {
								idName = "Id"
							} else {
								idName = "id"
							}
						}
					}
					if idOld == "(" { // no id statement at all
						idNew = fmt.Sprintf("( %s(%d),", idName, idf[0])
					} else { // idOld is s.th. like Id(0)
						idNew = fmt.Sprintf("%s(%d)", idName, idf[0])
					}
					rest = strings.Replace(rest, idOld, idNew, 1)
				}
			} else {
				//for _, id := range idf {
				//_, okl := idd.idToLocRef[id]
				fmt.Fprintln(w, "ignoring multi ids on", t, "for now")
				//  if !okl { // no location information
				//  	continue
				//  }
				//  if li.File == path {
				//  	// todo
				//  }
				//}
			}
		}

		// now we have the complete t

		nbTRICE := ""                         //rest[loc[0]:loc[1]]
		nbID := matchNbID.FindString(nbTRICE) // nbID is the `ID(n)` inside the trice statement.
		if nbID == "" {
			// no id(n) - todo
		}
		nb := matchNb.FindString(nbID) // nb is the plain id number
		if nb == "0" {
			//  	id , ok = idd.fmtToId
		} else {
			//  	id, ok = idd.fmtToId
		}

	}
	return in, modified, err
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
			fallthrough
		case '\'':
			fallthrough
		case '`':
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

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

//  // triceIDInsertion does the real ID insertion task.
//  // If file was modified it returns fileModified 1.
//  func triceIDInsertionLegacyForReference(
//  	w io.Writer, // messages destination
//  	fSys *afero.Afero, // file system
//  	path string, // file name
//  	fileInfo os.FileInfo, // fileInfo is needed to pass the permissions when opening source files - could probably optimized away.
//  	ilu TriceIDLookUp, // id lookup map with history, extendable
//  	flu triceFmtLookUp, // fmt lookup map with history, extendable
//  	lim TriceIDLookUpLI, // location information map only as lookup reference
//  	itemLu TriceItemLookUpID, // build from current source tree
//  	idLu TriceIDLookupItem, // build from current source tree
//  ) (fileModified int, err error) {
//
//  	text, err := readFile(w, fSys, path, fileInfo, nil)
//  	if nil != err {
//  		return 0, err
//  	}
//
//  	var dummy bool
//
//  	// todo: each file is parsed 3 times -> put this in one function
//  	textN, fileModified0 := insertParamCountAndID0(w, text, ExtendMacrosWithParamCount)                                                        // update parameter count: TRICE* to TRICE*_n
//  	textU, fileModified1 := insertIDsAtSamePosition(w, false /*SharedIDs*/, Min, Max, SearchMethod, textN, ilu, flu, &dummy /*pListModified*/) // update IDs: Id(0) -> Id(M)
//
//  	// probably not needed anymore
//  	parseValidTriceIDsAndUpdateLocationInfo(w, path, textU, ilu, flu, lim) // parses text for valid trices tf and adds them to ilu & flu and updates location information map lim.                                                                                                 // workaround: do it again to update li.json.
//
//  	// write out
//  	if fileModified0 || fileModified1 /*|| fileModified2*/ {
//  		fileModified = 1
//  	}
//  	if fileModified == 1 && !DryRun {
//  		if Verbose {
//  			fmt.Fprintln(w, "Changed: ", path)
//  		}
//  		err = fSys.WriteFile(path, []byte(textU), fileInfo.Mode())
//  		if err != nil {
//  			return 0, fmt.Errorf("failed to change %s: %v", path, err)
//  		}
//  	}
//
//  	return fileModified, nil
//  }
//
//  // insertParamCountAndID0 is for now a copy of updateParamCountAndID0
//  func insertParamCountAndID0(w io.Writer, text string, extendMacroName bool) (string, bool) {
//  	var modified bool
//  	subs := text[:] // create a copy of text and assign it to subs
//  	for {
//  		loc := matchFullAnyTrice.FindStringIndex(subs) // find the next TRICE location in file
//  		if nil == loc {
//  			return text, modified // done
//  		}
//  		trice := subs[loc[0]:loc[1]] // the whole TRICE*(*);
//  		triceC := trice              // make a copy
//  		if extendMacroName {
//  			locNoLen := matchTriceNoLen.FindStringIndex(trice) // find the next TRICE no len location in trice
//  			if nil != locNoLen {                               // need to add len to trice name
//  				n := formatSpecifierCount(triceC)
//  				triceNameNoLen := triceC[locNoLen[0]:locNoLen[1]]
//  				triceNameWithLen := addFormatSpecifierCount(w, triceNameNoLen, n)
//  				// In this example `Trice8_1( id(  800), "rd:Trice8 line %d, %d\n", __LINE__, 2 );` triceNameNoLen is "Trice8",
//  				// that is the Trice8 after "rd:" and triceNameWithLen is Trice8_1.
//  				// The following line would replace the first `Trice8` which is part of `Trice8_1` with `Trice8_1` resulting in
//  				// `Trice8_1_1( id(  800), "rd:Trice8 line %d, %d\n", __LINE__, 2 );` what is a bug.
//  				// triceC = strings.Replace(triceC, triceNameNoLen, triceNameWithLen, 1) // insert _n
//  				// Therefore the replacement is donne manually:
//  				triceC = triceC[:locNoLen[0]] + triceNameWithLen + triceC[locNoLen[1]:] // insert _n
//  				modified = true
//  				if Verbose {
//  					fmt.Fprint(w, triceNameNoLen)
//  					fmt.Fprint(w, " -> ")
//  					fmt.Fprintln(w, triceNameWithLen)
//  				}
//  			}
//  		}
//  		// here trice name in triceC contains _n and now we need to check for Id existence.
//  		// triceC could have been modified here but text is unchanged so far.
//  		idLoc := matchIDInsideTrice.FindStringIndex(triceC)
//  		if nil == idLoc { // no Id(n) inside trice, so we add it
//  			triceO := matchAnyTriceStart.FindString(triceC) // TRICE*( part (the trice start)
//  			var triceU string
//  			if strings.Contains(triceO, "ice") {
//  				triceU = triceO + " iD(0),"
//  			} else {
//  				triceU = triceO + StampSizeId
//  			}
//  			triceC = strings.Replace(triceC, triceO, triceU, 1) // insert Id(0) into trice copy
//  			modified = true
//  			if Verbose {
//  				fmt.Fprint(w, triceO)
//  				fmt.Fprint(w, " -> ")
//  				fmt.Fprintln(w, triceU)
//  			}
//  		}
//  		if modified {
//  			text = strings.Replace(text, trice, triceC, 1) // this works, because a trice gets changed only once
//  		}
//  		subs = subs[loc[1]:] // The replacement makes text not shorter, so next search can start at loc[1]
//  	}
//  }
//
//  // insertIDsAtSamePosition is for now a copy of updateIDsUniqOrShared
//  func insertIDsAtSamePosition(w io.Writer, _ /*sharedIDs*/ bool, min, max TriceID, searchMethod string, text string, ilu TriceIDLookUp, flu triceFmtLookUp, pListModified *bool) (string, bool) {
//  	var fileModified bool
//  	subs := text[:] // create a copy of text and assign it to subs
//  	for {
//  		loc := matchNbTRICE.FindStringIndex(subs) // find the next TRICE location in file
//  		if nil == loc {
//  			return text, fileModified // done
//  		}
//  		nbTRICE := subs[loc[0]:loc[1]] // full trice expression with Id(n)
//  		// prepare subs for next loop
//  		subs = subs[loc[1]:] // A possible Id(0) replacement makes subs not shorter, so next search can start at loc[1].
//  		// A case like 'TRICE*( Id(                             0                              ), "");' is not expected.
//
//  		nbID, id, tf, idTypeResult := triceParse(nbTRICE)
//  		//if idTypeResult == idTypeS8 {
//  		//	continue
//  		//}
//  		tf.Type = strings.ToUpper(tf.Type) // Lower case and upper case Type are not distinguished for normal trices in shared IDs mode.
//
//  		// In ilu id could point to a different tf. So we need to check that and invalidate id in that case.
//  		// - That typically happens after tf was changed in source but the id not.
//  		// - Also the source file with id:tf could be added from a different project and refresh could not add it to ilu because id is used differently.
//  		if id != 0 {
//  			if tfL, ok := ilu[id]; ok { // found
//  				tfL.Type = strings.ToUpper(tfL.Type) // Lower case and upper case Type are not distinguished for normal trices in shared IDs mode.
//  				if !reflect.DeepEqual(tf, tfL) {
//  					id = -id // mark as invalid
//  				}
//  			}
//  		}
//  		if id <= 0 { // marked as invalid: id is 0 or inside ilu used differently
//
//  			invalID := nbID
//  			invalTRICE := nbTRICE
//
//  			//<<<<<<<<< Temporary merge branch 1
//  			//			// we need a new one
//  			//			id = ilu.newID(w, min, max, searchMethod) // a prerequisite is an in a previous step refreshed lu
//  			//			*pListModified = true
//  			//			// patch the id into text
//  			//			nID := fmt.Sprintf("Id(%5d)", id)
//  			//=========
//  			//if id, found := flu[tf]; sharedIDs && found { // yes, we can use it in shared IDs mode
//  			//	msg.FatalInfoOnTrue(id == 0, "no id 0 allowed in map")
//  			//} else
//  			//{ // no, we need a new one
//  			id = ilu.newID(w, min, max, searchMethod) // a prerequisite is an in a previous step refreshed lu
//  			*pListModified = true
//  			//}
//  			var nID string // patch the id into text
//  			switch idTypeResult {
//  			case idTypeS8:
//  				nID = fmt.Sprintf("iD(%5d)", id) // todo: patID
//  			case idTypeS4:
//  				nID = fmt.Sprintf("ID(%5d)", id) // todo: patID
//  			case idTypeS2:
//  				nID = fmt.Sprintf("Id(%5d)", id) // todo: patID
//  			case idTypeS0:
//  				nID = fmt.Sprintf("id(%5d)", id) // todo: patID
//  			}
//  			//>>>>>>>>> Temporary merge branch 2
//  			if Verbose {
//  				if nID != invalID {
//  					fmt.Fprint(w, invalID, " -> ")
//  				}
//  				fmt.Fprintln(w, nID)
//  			}
//  			nbTRICE := strings.Replace(nbTRICE, invalID, nID, 1)
//  			text = strings.Replace(text, invalTRICE, nbTRICE, 1)
//  			fileModified = true
//  		}
//  		// update map: That is needed after an invalid trice or if id:tf is valid but not inside ilu & flu yet, for example after manual code changes or forgotten refresh before update.
//  		ilu[id] = tf
//  		addID(tf, id, flu)
//  	}
//  }
//
//  // parseValidTriceIDsAndUpdateLocationInfo is for now a copy of refreshIDs.
//  // refreshIDs parses text for valid trices tf and adds them to ilu & flu and updates location information map lim.
//  //
//  // parseValidTriceIDsAndUpdateLocationInfo ist called for each file (fileName)
//  func parseValidTriceIDsAndUpdateLocationInfo(w io.Writer, fileName, text string, ilu TriceIDLookUp, flu triceFmtLookUp, lim TriceIDLookUpLI) {
//  	subs := text[:] // create a copy of text and assign it to subs
//  	line := 1       // source cole lines start with 1 for some reason
//  	for {
//  		loc := matchNbTRICE.FindStringSubmatchIndex(subs) // find the next TRICE location in file
//  		if nil == loc {
//  			return // done
//  		}
//  		line += strings.Count(subs[:loc[0]], "\n")
//  		nbTRICE := subs[loc[0]:loc[1]] // full trice expression with Id(n)
//  		// prepare subs for next loop
//  		subs = subs[loc[1]:] // A possible Id(0) replacement makes subs not shorter, so next search can start at loc[1].
//  		// A case like 'TRICE*( Id(                             0                              ), "");' is not expected.
//
//  		_, id, tf, _ /*found*/ := triceParse(nbTRICE)
//  		//  if found == idTypeS8 {
//  		//  	continue
//  		//  }
//  		tfS := tf
//  		tfS.Type = strings.ToUpper(tfS.Type) // Lower case and upper case Type are not distinguished.
//
//  		// In ilu id could point to a different tf. So we need to check that and invalidate id in that case.
//  		// - That typically happens after tf was changed in source but the id not.
//  		// - Also the source file with id:tf could be added from a different project and refresh could not add it to ilu because id is used differently.
//  		// Without this check double used IDs are silently loose one of their usages, what is ok, but this way we get a warning.
//  		if id != 0 {
//  			if tfL, ok := ilu[id]; ok { // found
//  				tfL.Type = strings.ToUpper(tfL.Type)
//  				if !reflect.DeepEqual(tfS, tfL) { // Lower case and upper case Type are not distinguished.
//  					fmt.Fprintln(w, "Id", id, "already used differently, ignoring it.") // todo: patID
//  					id = -id                                                            // mark as invalid
//  				}
//  			}
//  		}
//  		if id > 0 {
//  			var li TriceLI
//  			li.File = filepath.Base(fileName)
//  			li.Line = line
//  			lim[id] = li
//  			ilu[id] = tf
//  			addID(tfS, id, flu)
//  		}
//  	}
//  }
