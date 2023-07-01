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

type parseStateType int

const (
	normal parseStateType = iota
	noIdFound
	idFoundButWithoutNumber
	unexpectedCannotConvertNIntoNumber
)

type idStateType int

const (
	fmtNotKnown idStateType = iota
	used
)

// insertTriceIDs does the ID insertion task on in, uses internally local pointer idd and returns the result in out with modified==true when out != in.
// in is the read file path content and out is the write file content.
// a is used for mutex access to til.json. path is needed for location information checks.
// For reference look into file TriceUserGuide.md part "The `trice insert` Algorithm".
func insertTriceIDs(w io.Writer, path string, in []byte, a *ant.Admin) (out []byte, modified bool, err error) {

	//  tfmt, found := idd.idToFmt[TriceID(1)]

	var parseState parseStateType
	var idn TriceID     // idn is the next found id inside the source.
	rest := string(in)  // rest is the so far not processed part of the file
	var fmtSpace string // fmtSpace starts behind ID(n), if ID(n) exists, otherwise it starts behind found trice type.
	var idOld string    // idOld is the "iD(n)" part, if found otherwise "".
	var idNew string    // idNew is the "iD(x)" part for replacement of iD(0)

	for {
		idn = 0
		typeLoc := matchTypNameTRICE.FindStringIndex(rest) // typeLoc is the position of the next trice type (statement name without following parentheses).
		if typeLoc == nil {
			break // done
		}
		var t TriceFmt
		t.Type = rest[typeLoc[0]:typeLoc[1]]     // t.Type is the TRice8_2 or TRice part for example. Hint: TRice defaults to 32 bit if not configured differently.
		rest = rest[typeLoc[1]:]                 // We can set a new starting point here.
		idLoc := matchNbID.FindStringIndex(rest) // idLoc is the space for an eventually following ID(n).
		if idLoc == nil {                        // no ID(n) found
			idOld = "(" // just the opening parenthesis after t.Type
			fmtSpace = rest
			parseState = noIdFound
		} else { // ID(n) found
			idOld = rest[idLoc[0]:idLoc[1]]
			fmtSpace = rest[idLoc[1]:]
			nSpace := rest[idLoc[0]:idLoc[1]] // nSpace is where we expect n.
			nLoc := matchNb.FindStringIndex(nSpace)
			if nLoc == nil { // Someone wrote trice( iD(0x100), ...), trice( id(), ... ) or trice( iD(name), ...) for example.
				parseState = idFoundButWithoutNumber
			} else { // This is the normal case like trice( iD( 111)... .
				n, err := strconv.Atoi(nSpace[idLoc[0]:idLoc[1]])
				if err != nil {
					parseState = unexpectedCannotConvertNIntoNumber // That should never happen.
				} else { // ok
					parseState = normal
					idn = TriceID(n) // idn is the assigned id inside source file.
				}
			}
		}
		// example cases:
		// trice( "foo", ... );           --> idn =   0, parseState = noIdFound
		// trice( iD(0x111), "foo", ... ) --> idn =   0, parseState = idFoundButWithoutNumber
		// trice( iD(foo), "foo", ... )   --> idn =   0, parseState = idFoundButWithoutNumber
		// trice( iD(1.1), "foo", ... )   --> idn =   0, parseState = unexpectedCannotConvertNIntoNumber
		// trice( iD(0), "foo, ... ")     --> idn =   0, parseState = normal
		// trice( iD(111), "foo, ... ")   --> idn = 111, parseState = normal

		fmtLoc := matchFormatString(fmtSpace)
		if fmtLoc == nil {
			fmt.Fprintln(w, "No fmt string found after", t.Type)
			rest = fmtSpace // Skip (ID(n) if there was one. Example: trice( foo ) is ignored.
			continue        // This is unexpected, so we ignore, but report that.
		}

		t.Strg = fmtSpace[fmtLoc[0]:fmtLoc[1]] // Now we have the complete trice t (Type and Strg)
		if parseState == idFoundButWithoutNumber || parseState == unexpectedCannotConvertNIntoNumber {
			rest = fmtSpace[fmtLoc[1]:] // reduce search space
			continue                    // start over
		}
		// example cases:
		// trice( "foo", ... );           --> idn =   0, parseState = noIdFound
		// trice( iD(0), "foo, ... ")     --> idn =   0, parseState = normal
		// trice( iD(111), "foo, ... ")   --> idn = 111, parseState = normal
		// state here:
		// rest starts at first char after trice name. We need to keep that point for later ID(n) manipulation.
		// fmtSpace starts after ID(n). We need to keep that point for later ID(n) manipulation.
		// idn holds the trice id found in the source and parseState is set

		//var idState idStateType
		a.Mutex.Lock()
		idf, okf := idd.fmtToId[t] // idf holds all available trice IDs for t.
		if !okf {
			a.Mutex.Unlock()
			//idState = fmtNotKnown
		} else { // found (x times) inside til.json
			if len(idf) == 1 { // t occurs only one time, what is mostly the case.
				if idn == 0 || idn == idf[0] { // idn is 0 or identical what is the normal case.
					delete(idd.fmtToId, t) // We do not care if it is in an other file because til.json says, that t is used only one times.
					// Code could be copied into other files. Such cases are rare and we accept that an ID can move to an other file then.
				} else { // idn is not 0 and not inside til.json. That is the case after importing new sources with IDs.
					// Do not delete t from idd.fmtToId, because it refers to a different ID there.
					idd.idToFmt[idn] = t // Extend til.json.
				}
				//idState = used
				a.Mutex.Unlock()

				if idn != 0 { // The ID is already inside source file.
					rest = fmtSpace[fmtLoc[1]:] // reduce search space
					continue
				} else { // patch source
					var idName string
					if parseState == noIdFound { // TRice..., Trice..., trice..., TRICE... are the cases
						if t.Type[2] == 'i' {
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

// matchFormatString returns a two-element slice of integers defining the location of the leftmost match in s of the matchFmtString regular expression.
// The match itself is at s[loc[0]:loc[1]]. A return value of nil indicates no match.
// If the format string contains `\"` elements, the found sub strings are concatenated to the returned result.
func matchFormatString(s string) (loc []int) {
	ss := s
	for {
		fmtLoc := matchFmtString.FindStringIndex(ss)
		if fmtLoc == nil {
			return loc // done
		}
		if loc == nil {
			loc = fmtLoc // Keep start position and current ent position.
		}
		fs := ss[fmtLoc[0]:fmtLoc[1]]       // fs is the found string.
		if ss[len(fs)-2:len(fs)-1] != `\` { // Check for "a\"b" cases
			return loc // done
		}
		// s ends with `\"`, so we need to continue parsing
		ss = ss[fmtLoc[1]:]             // cut off ss start for next parsing round
		loc[1] += fmtLoc[1] - fmtLoc[0] // set new value for loc[1]
	}
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
