// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

// source tree management

import (
	"fmt"
	"io"
	"os"

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

// insertTriceIDs does the ID insertion task on in, uses local pointer idd and returns the result in out with modified==true when any changes where made.
// For reference look into file TriceUserGuide.md part "The `trice insert` Algorithm".
func insertTriceIDs(w io.Writer, path string, in []byte, a *ant.Admin) (out []byte, modified bool, err error) {

	//text := string(read)
	// todo: insert action using path && wd
	a.Mutex.RLock()
	tfmt, found := idd.idToFmt[TriceID(1)]
	_, err = fmt.Fprintln(w, "inserting to", path, found, tfmt)
	a.Mutex.RUnlock()

	return in, modified, err
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
