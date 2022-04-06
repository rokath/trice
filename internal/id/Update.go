// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

// source tree management

import (
	"fmt"
	"io"
	"io/ioutil"
	"os"
	"path/filepath"
	"reflect"
	"regexp"
	"strings"

	"github.com/rokath/trice/pkg/msg"
)

const (

	// patSourceFile is a regex pattern matching any source file for patching
	patSourceFile = "(\\.c|\\.h|\\.cc|\\.cpp|\\.hpp)$"

	// patSourceFile is a regex pattern matching any source file for patching
	patCFile = "(\\.c|\\.cc|\\.cpp)$"

	// patTrice matches any TRICE name variant https://regex101.com/r/IkIhV3/1, The (?i) says case-insensitive. (?U)=un-greedy -> only first match.
	patTypNameTRICE = `(?iU)(\b((TRICE((_(S|N)|0)|((8|16|32|64)*(_[0-9]*)*))))\b)`
	//               `     (\b((TRICE(_S|0|(8|16|32|64)*)))(_[1-9]*)*|\b)\s*\(\s*\bID\b\s*\(\s*.*[0-9]\s*\)\s*,\s*".*"\s*.*\)\s*;` // https://regex101.com/r/pPRsjf/1

	// patFmtString is a regex matching the first format string inside trice
	patFmtString = `"(.*)"`

	// patNbTRICE is a regex pattern matching any "TRICE*(Id(n), "", ... )". - see https://regex101.com/r/mllhNQ/1
	// https://regex101.com/r/4hz1r8/1
	patNbTRICE = patTypNameTRICE + `\s*\(` + patID + `\(\s*.*[0-9]\s*\)\s*,\s*` + patFmtString + `\s*.*\)`

	// https://regex101.com/r/hWMjhU/3 - match any kind of trice with or without len or ID
	patFullAnyTrice = patTypNameTRICE + `\s*\(\s*(` + patID + `\((\s*\d+)\s*\)\s*,)?\s*` + patFmtString + `\s*.*\)`

	// patAnyTriceStart finds a starting trice with opening '(': https://regex101.com/r/wPuT4M/1
	patAnyTriceStart = patTypNameTRICE + `\s*\(`

	// patNextFormatSpecifier is a regex to find next format specifier in a string (exclude %%*)
	patNextFormatSpecifier = `(?:^|[^%])(%[0-9\.#]*(b|c|d|e|f|g|E|F|G|h|i|l|L|n|o|O|p|q|t|u|x|X))`

	// patTriceNoLen finds next `TRICEn` without length specifier: https://regex101.com/r/vSvOEc/1
	patTriceNoLen = `(?i)(\bTRICE(|8|16|32|64)\b)`

	patID = `\s*\bId\b\s*` // `\s*\b(I|i)d\b\s*`

	patNumber = `\d+`

	// patNbID is a regex pattern matching any (first in string) "Id(n)" and usable in matches of matchNbTRICE
	patNbID = `\b` + patID + `\(\s*[0-9]*\s*\)`

	// patIdInsideTrice finds if an `( Id(n) ,"` sequence exists inside trice
	patIDInsideTrice = `(?U)\(` + patID + `\((\s*\d+)\s*\)\s*,\s*"`

	// patTriceFileId finds first occurrence, see https://regex101.com/r/hWMjhU/4
	patTriceFileId = `#define\s*TRICE_FILE\s*Id\([0-9]*\)`

	patIncludeTriceHeader = `#include\s*["<]trice\.h[">]`
)

var (
	matchSourceFile          = regexp.MustCompile(patSourceFile)
	matchNbTRICE             = regexp.MustCompile(patNbTRICE)
	matchNbID                = regexp.MustCompile(patNbID)
	matchTypNameTRICE        = regexp.MustCompile(patTypNameTRICE)
	matchFmtString           = regexp.MustCompile(patFmtString)
	matchNextFormatSpecifier = regexp.MustCompile(patNextFormatSpecifier)
	matchFullAnyTrice        = regexp.MustCompile(patFullAnyTrice)
	matchTriceNoLen          = regexp.MustCompile(patTriceNoLen)
	matchIDInsideTrice       = regexp.MustCompile(patIDInsideTrice)
	matchAnyTriceStart       = regexp.MustCompile(patAnyTriceStart)
	matchTriceFileId         = regexp.MustCompile(patTriceFileId)
	matchNumber              = regexp.MustCompile(patNumber)
	matchIncludeTriceHeader  = regexp.MustCompile(patIncludeTriceHeader)

	ExtendMacrosWithParamCount bool

	// DefaultTriceBitWidth tells the bit width of TRICE macros having no bit width in their names, like TRICE32 or TRICE8.
	//
	// One target can use only one bith width for bare TRICE macros and the setting inside the target code must match DefaultTriceBitWidth.
	DefaultTriceBitWidth = "32" // todo: create compiler switch for other options "8", "16", "32", "64"
)

// updateParamCountAndID0 stays in text as long as trice statements are found.
// If a TRICE* is found, it is getting an Id(0) inserted, and it is also extended by _n.
// according to the format specifier count inside the format-string. Both only if not already existent.
// A none with the format specifier count matching _n is intentionally not corrected.
// About a not matching parameter count the C compiler will complain later.
// trice statements ending with letter 'i' keep the 'i' at the end.
// Short *Trices* like Trice0 or Trice16_1i need to have an id(0) instead of Id(0) but that gets corrected
// automatically when the id n is inserted.
// text is the full file contents, which could be modified, therefore it is also returned with a modified flag
func updateParamCountAndID0(w io.Writer, text string, extendMacroName bool) (string, bool) {
	var modified bool
	subs := text[:] // create a copy of text and assign it to subs
	for {
		loc := matchFullAnyTrice.FindStringIndex(subs) // find the next TRICE location in file
		if nil == loc {
			return text, modified // done
		}
		trice := subs[loc[0]:loc[1]] // the whole TRICE*(*);
		triceC := trice              // make a copy
		if extendMacroName {
			locNoLen := matchTriceNoLen.FindStringIndex(trice) // find the next TRICE no len location in trice
			if nil != locNoLen {                               // need to add len to trice name
				n := formatSpecifierCount(triceC)
				triceNameNoLen := triceC[locNoLen[0]:locNoLen[1]]
				triceNameWithLen := addFormatSpecifierCount(w, triceNameNoLen, n)
				// In this example `Trice8_1( id(  800), "rd:Trice8 line %d, %d\n", __LINE__, 2 );` triceNameNoLen is "Trice8",
				// that is the Trice8 after "rd:" and triceNameWithLen is Trice8_1.
				// The following line would replace the first `Trice8` which is part of `Trice8_1` with `Trice8_1` resulting in
				// `Trice8_1_1( id(  800), "rd:Trice8 line %d, %d\n", __LINE__, 2 );` what is a bug.
				// triceC = strings.Replace(triceC, triceNameNoLen, triceNameWithLen, 1) // insert _n
				// Therefore the replacement is donne manually:
				triceC = triceC[:locNoLen[0]] + triceNameWithLen + triceC[locNoLen[1]:] // insert _n
				modified = true
				if Verbose {
					fmt.Fprint(w, triceNameNoLen)
					fmt.Fprint(w, " -> ")
					fmt.Fprintln(w, triceNameWithLen)
				}
			}
		}
		// here trice name in triceC contains _n and now we need to check for Id existence.
		// triceC could have been modified here but text is unchanged so far.
		idLoc := matchIDInsideTrice.FindStringIndex(triceC)
		if nil == idLoc { // no Id(n) inside trice, so we add it
			triceO := matchAnyTriceStart.FindString(triceC) // TRICE*( part (the trice start)
			triceU := triceO + " Id(0),"
			triceC = strings.Replace(triceC, triceO, triceU, 1) // insert Id(0) into trice copy
			modified = true
			if Verbose {
				fmt.Fprint(w, triceO)
				fmt.Fprint(w, " -> ")
				fmt.Fprintln(w, triceU)
			}
		}
		if modified {
			text = strings.Replace(text, trice, triceC, 1) // this works, because a trice gets changed only once
		}
		subs = subs[loc[1]:] // The replacement makes text not shorter, so next search can start at loc[1]
	}
}

// formatSpecifierCount parses s for format specifier and returns the found count.
func formatSpecifierCount(s string) (count int) {
	xs := "any"
	for xs != "" {
		lo := matchNextFormatSpecifier.FindStringIndex(s)
		xs = matchNextFormatSpecifier.FindString(s)
		if xs != "" { // found
			count++
			s = s[lo[1]:]
		} else {
			xs = ""
		}
	}
	return
}

// addFormatSpecifierCount extends s with _n and returns it as sl
func addFormatSpecifierCount(w io.Writer, s string, n int) (sl string) {
	if 0 < n && n < 99 { // patch
		sl = fmt.Sprintf(s+"_%d", n) // TRICE*_n
	} else {
		if n != 0 {
			fmt.Fprintln(w, "Parse error: ", n, " % format specifier found inside ", s)
		}
		sl = s
	}
	return
}

func isSourceFile(fi os.FileInfo) bool {
	return matchSourceFile.MatchString(fi.Name())
}

func refreshList(w io.Writer, root string, lu TriceIDLookUp, tflu triceFmtLookUp) {
	if Verbose {
		fmt.Fprintln(w, "dir=", root)
		fmt.Fprintln(w, "List=", FnJSON)
	}
	msg.FatalInfoOnErr(filepath.Walk(root, visitRefresh(w, lu, tflu)), "failed to walk tree")
}

// Additional actions needed: (Option -dry-run lets do a check in advance.)
// - Insert in all TRICE messages without ID an `Id(0),`
// - Check if all TRICE messages have correct parameter count and adapt the count without touching the Id(n),
// - Check if ID list has same ID more than one time and remove younger items with message.
//  - Read til.json in a map and write til.json after the map was finally manipulated back to til.json.
// - Check if Source tree has same ID with different TRICE strings.
//   - Keep the ID which is in til.json and set others to 0 with message.
//   - If none of the more than 1 time used ID is in til.json set all to 0 with message.
// - Check if in source code exist IDs not in til.json so far and extend til.json if there is no conflict.
//  - If the ID in soure code is already used in til.json differently set the ID in source code to 0 with message.
// NOT NEEDED: - Check if in til.json ID's not in source tree and mark them with `removed` timestamp.
// NOT NEEDED:   - If several source trees use same til.json, the `removed` timestamp is without sense.
// NOT NEEDED:   - If a `removed` timestamp is set, but the ID is in the source tree the `removed` timestamp is set to 0.

// idsUpdate is parsing source tree root which is part of Srcs and performing these actions:
// - replace.Type( Id(0), ...) with.Type( Id(n), ...)
// - find duplicate.Type( Id(n), ...) and replace one of them if *Trices* are not identical
// - extend file fnIDList
func idsUpdate(w io.Writer, root string, lu TriceIDLookUp, tflu triceFmtLookUp, pListModified *bool) {
	if Verbose && FnJSON != "emptyFile" {
		fmt.Fprintln(w, "dir=", root)
		fmt.Fprintln(w, "List=", FnJSON)
	}
	msg.FatalInfoOnErr(filepath.Walk(root, visitUpdate(w, lu, tflu, pListModified)), "failed to walk tree")
}

func readFile(w io.Writer, path string, fi os.FileInfo, err error) (string, error) {
	if err != nil || fi.IsDir() || !isSourceFile(fi) {
		return "", err // forward any error and do nothing
	}
	if Verbose {
		fmt.Fprintln(w, path)
	}
	read, err := ioutil.ReadFile(path)
	if nil != err {
		return "", err
	}
	text := string(read)
	return text, nil
}

func visitRefresh(w io.Writer, lu TriceIDLookUp, tflu triceFmtLookUp) filepath.WalkFunc {
	return func(path string, fi os.FileInfo, err error) error {
		text, err := readFile(w, path, fi, err)
		if nil != err {
			return err
		}
		refreshIDs(w, text, lu, tflu) // update IDs: Id(0) -> Id(M)
		return nil
	}
}

func isCFile(path string) bool {
	s := strings.ToLower(path)
	if strings.HasSuffix(s, patCFile) {
		fmt.Println(patCFile, "works")
		return true
	}
	if strings.HasSuffix(s, ".c") {
		return true
	}
	if strings.HasSuffix(s, ".cpp") {
		return true
	}
	if strings.HasSuffix(s, ".cxx") {
		return true
	}
	return false
}

// modifyTriceFileIdLine inserts a new TRICE_FILE pattern id in the line.
//
// The id is 0 or already used in a different way
// The id is 0: Check lu&tflu for t.Type == "TRICE_FILE" && t.Strg == fileName
func modifyTriceFileIdLine(w io.Writer, _ TriceIDLookUp, tflu triceFmtLookUp, inText, fileName string) (outText string, fileModified bool) {
	t := TriceFmt{"TRICE_FILE", fileName}
	if SharedIDs {
		if fid, ok := tflu[t]; ok {
			fmt.Fprintf(w, "Trice fileName %s inside map known with fid %d.\n", fileName, fid)
		}
		// todo: insert fid in file
	}
	// todo: get new id
	// todo: insert fid in file
	// todo: tflu[t] = TriceID(n)
	outText = inText
	return
}

// insertTriceFileIdLine inserts a TRICE_FILE pattern line immediately after a '#include "trice.h"' line
//
// If no '#include "trice.h"' is found, the file is not touched.
// If variable SharedIDs is true, lu&tflu are checked, if t.Type == "TRICE_FILE" && t.Strg == fileName exists and that id is used.
// If variable SharedIDs is false, a new id is used.
func insertTriceFileIdLine(w io.Writer, lu TriceIDLookUp, tflu triceFmtLookUp, inText, fileName string, sharedIDs bool, min, max TriceID, searchMethod string, pListModified *bool) (outText string, fileModified bool) {
	loc := matchIncludeTriceHeader.FindStringIndex(inText)
	if loc == nil {
		if Verbose {
			fmt.Fprintln(w, fileName)
		}
		outText = "" // a CFile not containing "#include "Trice.h" is expected to have no TRICE macros inside
		return       // and therefore no need to parse it any further. Todo: refactoring
	}
	t := TriceFmt{"TRICE_FILE", fileName}
	id, ok := tflu[t]
	if !(sharedIDs && ok) {
		id = lu.newID(w, min, max, searchMethod)
		lu[id] = t
		tflu[t] = id
		*pListModified = true
	}
	outText = inText[:loc[1]] + fmt.Sprintf("\n#define TRICE_FILE Id(%d)", id) + inText[loc[1]:]
	fileModified = true
	fmt.Fprintf(w, "#define TRICE_FILE Id(%d) inserted into %s\n", id, fileName)
	return
}

// updateTriceFileId parses inText for a TRICE_FILE pattern and generates or updates it, if needed.
//
// If no TRICE_FILE pattern exist, function insertTriceFileIdLine is called.
// If a TRICE_FILE pattern exist the following happen:
//	* Check if id inside TRICE_FILE pattern is not 0 and exists in til.json with t.Type == "TRICE_FILE" && t.Strg == fileName.
//    * yes: That is the normal case and fn returns with outText = inText
//    * no: function call modifyTriceFileIdLine
func updateTriceFileId(w io.Writer, lu TriceIDLookUp, tflu triceFmtLookUp, inText, fileName string, sharedIDs bool, min, max TriceID, searchMethod string, pListModified *bool) (outText string, fileModified bool) {
	// check if file contains a TRICE_FILE pattern
	locFID := matchTriceFileId.FindStringIndex(inText)
	if locFID == nil {
		if Verbose {
			fmt.Fprintln(w, "In", fileName, "no TRICE_FILE pattern found.")
		}
		outText, fileModified = insertTriceFileIdLine(w, lu, tflu, inText, fileName, sharedIDs, min, max, searchMethod, pListModified)
		return
	}

	// get the file id value
	sFID := inText[locFID[0]:locFID[1]]
	locID := matchNumber.FindStringIndex(sFID)
	sID := sFID[locID[0]:locID[1]] // This is just the file id as number string
	var n int
	_, err := fmt.Sscanf(sID, "%d", &n) // closing bracket in format string omitted intensionally
	msg.OnErrF(w, err)                  // because spaces after id otherwise are not tolerated
	if Verbose {
		fmt.Fprintf(w, "Trice file id '%d' found inside %s in '%s'\n", n, fileName, sID)
	}
	// check the file id value
	if n != 0 {
		if t, ok := lu[TriceID(n)]; ok { // n found
			if t.Type == "TRICE_FILE" && t.Strg == fileName { // in sync
				outText = inText // nothing to do
				return
			}
		}
	}
	outText, fileModified = modifyTriceFileIdLine(w, lu, tflu, inText, fileName)
	return
}

func visitUpdate(w io.Writer, lu TriceIDLookUp, tflu triceFmtLookUp, pListModified *bool) filepath.WalkFunc {
	// WalkFunc is the type of the function called for each file or directory
	// visited by Walk. The path argument contains the argument to Walk as a
	// prefix; that is, if Walk is called with "dir", which is a directory
	// containing the file "a", the walk function will be called with argument
	// "dir/a". The info argument is the os.FileInfo for the named path.
	//
	// If there was a problem walking to the file or directory named by path, the
	// incoming error will describe the problem and the function can decide how
	// to handle that error (and Walk will not descend into that directory). In the
	// case of an error, the info argument will be nil. If an error is returned,
	// processing stops. The sole exception is when the function returns the special
	// value SkipDir. If the function returns SkipDir when invoked on a directory,
	// Walk skips the directory's contents entirely. If the function returns SkipDir
	// when invoked on a non-directory file, Walk skips the remaining files in the
	// containing directory.
	return func(path string, fi os.FileInfo, err error) error {
		text, err := readFile(w, path, fi, err)
		if nil != err {
			return err
		}
		fileName := filepath.Base(path)
		var fileModified2 bool
		if isCFile(fileName) {
			text, fileModified2 = updateTriceFileId(w, lu, tflu, text, fileName, SharedIDs, Min, Max, SearchMethod, pListModified)
		}
		refreshIDs(w, text, lu, tflu) // update IDs: Id(0) -> Id(M)

		textN, fileModified0 := updateParamCountAndID0(w, text, ExtendMacrosWithParamCount)                                 // update parameter count: TRICE* to TRICE*_n and insert missing Id(0)
		textU, fileModified1 := updateIDsUniqOrShared(w, SharedIDs, Min, Max, SearchMethod, textN, lu, tflu, pListModified) // update IDs: Id(0) -> Id(M)

		// write out
		fileModified := fileModified0 || fileModified1 || fileModified2
		if fileModified && !DryRun {
			if Verbose {
				fmt.Fprintln(w, "Changed: ", path)
			}
			err = ioutil.WriteFile(path, []byte(textU), fi.Mode())
			if nil != err {
				return fmt.Errorf("failed to change %s: %v", path, err)
			}
		}
		return nil
	}
}

// triceIDParse returns an extracted id and found as true if t starts with s.th. like 'TRICE*( Id(n)...'
// nbID is the extracted string part containing 'Id(n)'.
func triceIDParse(t string) (nbID string, id TriceID, found bool) {
	nbID = matchNbID.FindString(t)
	if nbID == "" {
		msg.InfoOnTrue(Verbose, fmt.Sprintln("No 'Id(n)' or 'id(n)' found inside "+t))
		return
	}
	var n int
	_, err := fmt.Sscanf(nbID, "Id(%d", &n) // closing bracket in format string omitted intentionally
	if nil == err {                         // because spaces after id otherwise are not tolerated
		id = TriceID(n)
		found = true
		return
	}
	msg.Info(fmt.Sprintln("no 'Id(n' found inside " + nbID))
	return
}

// triceFmtParse returns an extracted tf and found as true if t is s.th. like 'TRICE*( Id(n), "..." );'.
func triceFmtParse(t string) (tf TriceFmt, found bool) {
	tf.Type = matchTypNameTRICE.FindString(t)
	if tf.Type == "" {
		msg.Info(fmt.Sprintln("no 'TRICE*' found inside " + t))
		return
	}
	match := matchFmtString.FindAllStringSubmatch(t, 1)
	if nil == match {
		msg.Info(fmt.Sprintln("no 'trice format string' found inside " + t))
		return
	}
	tf.Strg = match[0][1]
	found = true
	return
}

// triceParse expects a string t containing a trice macro in the form `TRICE*(Id(n), "...", ...);`
// Returned nbID is the extracted string part containing 'Id(n)'.
// Returned id is the scanned n inside Id(n), only and only if n is a single decimal number.
// Returned tf is the recognized trice.
// Only on success found is true.
func triceParse(t string) (nbID string, id TriceID, tf TriceFmt, found bool) {
	nbID, id, found = triceIDParse(t)
	if !found {
		return
	}
	tf, found = triceFmtParse(t)
	return
}

// refreshIDs parses text for valid trices tf and adds them to lu & tflu.
func refreshIDs(w io.Writer, text string, lu TriceIDLookUp, tflu triceFmtLookUp) {
	subs := text[:] // create a copy of text and assign it to subs
	for {
		loc := matchNbTRICE.FindStringSubmatchIndex(subs) // find the next TRICE location in file
		if nil == loc {
			return // done
		}
		nbTRICE := subs[loc[0]:loc[1]] // full trice expression with Id(n)
		// prepare subs for next loop
		subs = subs[loc[1]:] // A possible Id(0) replacement makes subs not shorter, so next search can start at loc[1].
		// A case like 'TRICE*( Id(                             0                              ), "");' is not expected.

		_, id, tf, found := triceParse(nbTRICE)
		if !found {
			continue
		}
		tfS := tf
		tfS.Type = strings.ToUpper(tfS.Type) // Lower case and upper case Type are not distinguished.

		// In lu id could point to a different tf. So we need to check that and invalidate id in that case.
		// - That typically happens after tf was changed in source but the id not.
		// - Also the source file with id:tf could be added from a different project and refresh could not add it to lu because id is used differently.
		// Without this check double used IDs are silently loose one of their usages, what is ok, but this way we get a warning.
		if id != 0 {
			if tfL, ok := lu[id]; ok { // found
				tfL.Type = strings.ToUpper(tfL.Type)
				if !reflect.DeepEqual(tfS, tfL) { // Lower case and upper case Type are not distinguished.
					fmt.Fprintln(w, "Id", id, "already used differently, ignoring it.")
					id = -id // mark as invalid
				}
			}
		}
		if id > 0 {
			lu[id] = tf
			tflu[tfS] = id // no distinction for lower and upper case Type
		}
	}
}

// updateIDsUniqOrShared parses text for new or invalid *Trices* 'tf' and gives them the legacy id if 'tf' is already in lu & tflu.
// An invalid trice is a trice without Id(n) or with Id(0) or which changed somehow. Exampes: 'TRICE0( Id(12) ,"foo");' was changed to 'TRICE0( Id(12) ,"bar");'
// If 'TRICE0( Id(99) ,"bar");' is in lu & tflu the invalid trice changes to 'TRICE0( Id(99) ,"bar");'. Otherwise instead of 99 a so far unused id is taken.
// Or: 'TRICE0( Id(12) ,"foo");' was changed to 'TRICE0( Id(13) ,"foo");'. Then lu & tflu are extended accordingly, or, if 13 is already used, it is replaced with a new id.
// Otherwise, a new id is generated, text patched and lu & tflu are extended.
// To work correctly, lu & tflu need to be in a refreshed state, means have all id:tf pairs from Srcs tree already inside.
// text is returned afterwards and true if text was changed and *pListModified set true if s.th. was changed.
// *pListModified in result is true if any file was changed.
// tflu holds the tf in upper case.
// lu holds the tf in source code case. If in source code upper and lower case occur, than only one can be in lu.
// sharedIDs, if true, reuses IDs for identical format strings.
func updateIDsUniqOrShared(w io.Writer, sharedIDs bool, min, max TriceID, searchMethod string, text string, lu TriceIDLookUp, tflu triceFmtLookUp, pListModified *bool) (string, bool) {
	var fileModified bool
	subs := text[:] // create a copy of text and assign it to subs
	for {
		loc := matchNbTRICE.FindStringIndex(subs) // find the next TRICE location in file
		if nil == loc {
			return text, fileModified // done
		}
		nbTRICE := subs[loc[0]:loc[1]] // full trice expression with Id(n)
		// prepare subs for next loop
		subs = subs[loc[1]:] // A possible Id(0) replacement makes subs not shorter, so next search can start at loc[1].
		// A case like 'TRICE*( Id(                             0                              ), "");' is not expected.

		nbID, id, tf, found := triceParse(nbTRICE)
		if !found {
			continue
		}
		tf.Type = strings.ToUpper(tf.Type) // Lower case and upper case Type are not distinguished for normal trices in shared IDs mode.

		// In lu id could point to a different tf. So we need to check that and invalidate id in that case.
		// - That typically happens after tf was changed in source but the id not.
		// - Also the source file with id:tf could be added from a different project and refresh could not add it to lu because id is used differently.
		if id != 0 {
			if tfL, ok := lu[id]; ok { // found
				tfL.Type = strings.ToUpper(tfL.Type) // Lower case and upper case Type are not distinguished for normal trices in shared IDs mode.
				if !reflect.DeepEqual(tf, tfL) {
					id = -id // mark as invalid
				}
			}
		}
		if id <= 0 { // marked as invalid: id is 0 or inside lu used differently

			invalID := nbID
			invalTRICE := nbTRICE

			if id, found = tflu[tf]; sharedIDs && found { // yes, we can use it in shared IDs mode
				msg.FatalInfoOnTrue(id == 0, "no id 0 allowed in map")
			} else { // no, we need a new one
				id = lu.newID(w, min, max, searchMethod) // a prerequisite is an in a previous step refreshed lu
				*pListModified = true
			}
			// patch the id into text
			nID := fmt.Sprintf("Id(%5d)", id)
			if Verbose {
				if nID != invalID {
					fmt.Fprint(w, invalID, " -> ")
				}
				fmt.Fprintln(w, nID)
			}
			nbTRICE := strings.Replace(nbTRICE, invalID, nID, 1)
			text = strings.Replace(text, invalTRICE, nbTRICE, 1)
			fileModified = true
		}
		// update map: That is needed after an invalid trice or if id:tf is valid but not inside lu & tflu yet, for example after manual code changes or forgotten refresh before update.
		lu[id] = tf
		tflu[tf] = id // no distinction for lower and upper case Type
	}
}

// zeroSourceTreeIds is overwriting with 0 all id's from source code tree srcRoot. It does not touch idlist.
func zeroSourceTreeIds(w io.Writer, srcRoot string, run bool) {
	err := filepath.Walk(srcRoot, visitZeroSourceTreeIds(w, run))
	if err != nil {
		panic(err)
	}
}

func visitZeroSourceTreeIds(w io.Writer, run bool) filepath.WalkFunc {
	// WalkFunc is the type of the function called for each file or directory
	// visited by Walk. The path argument contains the argument to Walk as a
	// prefix; that is, if Walk is called with "dir", which is a directory
	// containing the file "a", the walk function will be called with argument
	// "dir/a". The info argument is the os.FileInfo for the named path.
	//
	// If there was a problem walking to the file or directory named by path, the
	// incoming error will describe the problem and the function can decide how
	// to handle that error (and Walk will not descend into that directory). In the
	// case of an error, the info argument will be nil. If an error is returned,
	// processing stops. The sole exception is when the function returns the special
	// value SkipDir. If the function returns SkipDir when invoked on a directory,
	// Walk skips the directory's contents entirely. If the function returns SkipDir
	// when invoked on a non-directory file, Walk skips the remaining files in the
	// containing directory.
	return func(path string, fi os.FileInfo, err error) error {
		if fi.IsDir() || !isSourceFile(fi) || err != nil {
			return err // forward any error and do nothing
		}
		if Verbose {
			fmt.Fprintln(w, path)
		}
		read, err := ioutil.ReadFile(path)
		if err != nil {
			return err
		}
		s := string(read)
		a, b := 0, len(s)
		subs := s[a:b]
		modified := false

		for {
			var found bool
			if !found {
				found, modified, subs, s = zeroNextID(w, modified, subs, s)
				break
			}
		}

		if modified && run {
			err = ioutil.WriteFile(path, []byte(s), 0)
		}
		return err
	}
}

// first retval flag is if an ID was zeroed, others are updated input values. if an ID wsa zeroed
// - modified gets true
// - subs gets shorter
// - s is updated
func zeroNextID(w io.Writer, modified bool, subs, s string) (bool, bool, string, string) {
	loc := matchNbTRICE.FindStringIndex(subs)
	if nil == loc {
		return false, modified, subs, s
	}
	nbTRICE := subs[loc[0]:loc[1]]
	nbID := matchNbID.FindString(nbTRICE)
	if nbID == "" {
		msg.Info(fmt.Sprintln("No 'Id(n)' found inside " + nbTRICE))
		return false, modified, subs, s
	}

	zeroID := "Id(0)"
	fmt.Fprintln(w, nbID, " -> ", zeroID)

	zeroTRICE := strings.Replace(nbTRICE, nbID, zeroID, 1)
	s = strings.Replace(s, nbTRICE, zeroTRICE, 1)
	// 2^32 has 9 ciphers and shortest trice has 14 chars: TRICE0(Id(1),"");
	// The replacement of n with 0 makes s shorter, so the next search shoud start like 10 chars earlier.
	subs = subs[loc[1]-10:]
	return true, true, subs, s
}
