// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

// source tree management

import (
	"fmt"
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

	// patTrice matches any TRICE name variant https://regex101.com/r/jJGKvL/1, The (?i) says case insensitive
	patTypNameTRICE = `(?i)(\b((TRICE((_S|0)|((8|16|32|64)(_[0-9]*)?))))i*\b)`

	// patFmtString is a regex matching the first format string inside trice
	patFmtString = `"(.*)"`

	// patNbTRICE is a regex pattern matching any "TRICE*(Id(n), "", ... )". - see https://regex101.com/r/mllhNQ/1
	patNbTRICE = patTypNameTRICE + `\s*\(` + patID + `\(\s*.*[0-9]\s*\)\s*,\s*` + patFmtString + `\s*.*\)`

	// https://regex101.com/r/hWMjhU/3 - match any kind of trice with or without len or ID
	patFullAnyTrice = patTypNameTRICE + `\s*\(\s*(` + patID + `\((\s*\d+)\s*\)\s*,)?\s*` + patFmtString + `\s*.*\)`

	// patAnyTriceStart finds a starting trice with opening '(': https://regex101.com/r/wPuT4M/1
	patAnyTriceStart = patTypNameTRICE + `\s*\(`

	// patNextFormatSpecifier is a regex to find next format specifier in a string (exclude %%*)
	patNextFormatSpecifier = `(?:^|[^%])(%[0-9\.#]*(b|c|d|u|x|X|o|f))`

	// patTriceNoLen finds next `TRICEn` without length specifier: https://regex101.com/r/vSvOEc/1
	patTriceNoLen = `(?i)(\bTRICE(8|16|32|64)i?\b)`

	patID = `\s*\b(I|i)d\b\s*`

	// patNbID is a regex pattern matching any (first in string) "Id(n)" and usable in matches of matchNbTRICE
	patNbID = `\b` + patID + `\(\s*[0-9]*\s*\)`

	// patIdInsideTrice finds if an `( Id(n) ,"` sequence exists inside trice
	patIDInsideTrice = `(?U)\(` + patID + `\((\s*\d+)\s*\)\s*,\s*"`
)

var (
	matchSourceFile            = regexp.MustCompile(patSourceFile)
	matchNbTRICE               = regexp.MustCompile(patNbTRICE)
	matchNbID                  = regexp.MustCompile(patNbID)
	matchTypNameTRICE          = regexp.MustCompile(patTypNameTRICE)
	matchFmtString             = regexp.MustCompile(patFmtString)
	matchNextFormatSpecifier   = regexp.MustCompile(patNextFormatSpecifier)
	matchFullAnyTrice          = regexp.MustCompile(patFullAnyTrice)
	matchTriceNoLen            = regexp.MustCompile(patTriceNoLen)
	matchIDInsideTrice         = regexp.MustCompile(patIDInsideTrice)
	matchAnyTriceStart         = regexp.MustCompile(patAnyTriceStart)
	ExtendMacrosWithParamCount bool
)

// updateParamCountAndID0 stays in text as long as trice statements are found.
// If a TRICE* is found it is getting an Id(0) inserted and it is also extended by _n
// according to the format specifier count inside the formatstring. Both only if not already existent.
// A not with the format specifier count matching _n is intentionally not corrected.
// About a not matching parameter count the C compiler will complain later.
// trice statements ending with letter 'i' keep the 'i' ath the end.
// Short trices like Trice0 or Trice16_1i need to have an id(0) instead of Id(0) but that gets corrected
// automatically when the id n is inserted.
// text is the full filecontents, which could be modified, therefore it is also returned with a modified flag
func updateParamCountAndID0(text string, extendMacroName bool) (string, bool) {
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
				n := FormatSpecifierCount(triceC)
				triceNameNoLen := triceC[locNoLen[0]:locNoLen[1]]
				triceNameWithLen := addFormatSpecifierCount(triceNameNoLen, n)
				// In this example `Trice8_1( id(  800), "rd:Trice8 line %d, %d\n", __LINE__, 2 );` triceNameNoLen is "Trice8",
				// that is the Trice8 after "rd:" and triceNameWithLen is Trice8_1.
				// The following line would replace the first `Trice8` which is part of `Trice8_1` with `Trice8_1` resulting in
				// `Trice8_1_1( id(  800), "rd:Trice8 line %d, %d\n", __LINE__, 2 );` what is a bug.
				// triceC = strings.Replace(triceC, triceNameNoLen, triceNameWithLen, 1) // insert _n
				// Therefore the replacement is donne manually:
				triceC = triceC[:locNoLen[0]] + triceNameWithLen + triceC[locNoLen[1]:] // insert _n
				modified = true
				if Verbose {
					fmt.Print(triceNameNoLen)
					fmt.Print(" -> ")
					fmt.Println(triceNameWithLen)
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
				fmt.Print(triceO)
				fmt.Print(" -> ")
				fmt.Println(triceU)
			}
		}
		if modified {
			text = strings.Replace(text, trice, triceC, 1) // this works, because a trice gets changed only once
		}
		subs = subs[loc[1]:] // The replacement makes text not shorter, so next search can start at loc[1]
	}
}

// FormatSpecifierCount parses s for format specifier and returns the found count.
func FormatSpecifierCount(s string) (count int) {
	xs := "any"
	for "" != xs {
		lo := matchNextFormatSpecifier.FindStringIndex(s)
		xs = matchNextFormatSpecifier.FindString(s)
		if "" != xs { // found
			count++
			s = s[lo[1]:]
		} else {
			xs = ""
		}
	}
	return
}

// addFormatSpecifierCount extends s or si with _n or _ni and returns it as sl
func addFormatSpecifierCount(s string, n int) (sl string) {
	if 0 < n && n < 9 { // patch
		if 'i' == s[len(s)-1] { // last letter is 'i'
			sl = fmt.Sprintf(s[:len(s)-1]+"_%di", n) // TRICE*_ni
		} else {
			sl = fmt.Sprintf(s+"_%d", n) // TRICE*_n
		}
	} else {
		fmt.Println("Parse error: ", n, " % format specifier found inside ", s)
		sl = s
	}
	return
}

func isSourceFile(fi os.FileInfo) bool {
	return matchSourceFile.MatchString(fi.Name())
}

func refreshList(root string, lu TriceIDLookUp, tflu TriceFmtLookUp) {
	if Verbose {
		fmt.Println("dir=", root)
		fmt.Println("List=", FnJSON)
	}
	msg.FatalInfoOnErr(filepath.Walk(root, visitRefresh(lu, tflu)), "failed to walk tree")
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

// Update is parsing source tree root which is part of Srcs and performing these actions:
// - replace.Type( Id(0), ...) with.Type( Id(n), ...)
// - find duplicate.Type( Id(n), ...) and replace one of them if trices are not identical
// - extend file fnIDList
func IDsUpdate(root string, lu TriceIDLookUp, tflu TriceFmtLookUp, pListModified *bool) {
	if Verbose {
		fmt.Println("dir=", root)
		fmt.Println("List=", FnJSON)
	}
	msg.FatalInfoOnErr(filepath.Walk(root, visitUpdate(lu, tflu, pListModified)), "failed to walk tree")
}

func readFile(path string, fi os.FileInfo, err error) (string, error) {
	if err != nil || fi.IsDir() || !isSourceFile(fi) {
		return "", err // forward any error and do nothing
	}
	if Verbose {
		fmt.Println(path)
	}
	read, err := ioutil.ReadFile(path)
	if nil != err {
		return "", err
	}
	text := string(read)
	return text, nil
}

func visitRefresh(lu TriceIDLookUp, tflu TriceFmtLookUp) filepath.WalkFunc {
	return func(path string, fi os.FileInfo, err error) error {
		text, err := readFile(path, fi, err)
		if nil != err {
			return err
		}
		refreshIDs(text, lu, tflu) // update IDs: Id(0) -> Id(M)
		return nil
	}
}

func visitUpdate(lu TriceIDLookUp, tflu TriceFmtLookUp, pListModified *bool) filepath.WalkFunc {
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
		text, err := readFile(path, fi, err)
		if nil != err {
			return err
		}
		refreshIDs(text, lu, tflu) // update IDs: Id(0) -> Id(M)

		textN, fileModified0 := updateParamCountAndID0(text, ExtendMacrosWithParamCount)                                       // update parameter count: TRICE* to TRICE*_n and insert missing Id(0)
		textU, fileModified1 := updateIDsUniqOrShared(SharedIDs, MinShort, MaxShort, Min, Max, textN, lu, tflu, pListModified) // update IDs: Id(0) -> Id(M)

		// write out
		fileModified := fileModified0 || fileModified1
		if fileModified && !DryRun {
			if Verbose {
				fmt.Println("Changed: ", path)
			}
			err = ioutil.WriteFile(path, []byte(textU), fi.Mode())
			if nil != err {
				return fmt.Errorf("failed to change %s: %v", path, err)
			}
		}
		return nil
	}
}

// triceIDParse returns an extracted id and true if t starts with s.th. like 'TRICE*( Id(n)...'
// nbID is the extracted string part containing 'Id(n)'.
func triceIDParse(t string) (nbID string, id TriceID, ok bool) {
	nbID = matchNbID.FindString(t)
	if "" == nbID {
		msg.InfoOnTrue(Verbose, fmt.Sprintln("No 'Id(n)' or 'id(n)' found inside "+t))
		return
	}
	var n int
	_, err := fmt.Sscanf(nbID, "Id(%d", &n) // closing bracket in format string omitted intensionally
	if nil == err {                         // because spaces after id otherwise are not tolerated
		id = TriceID(n)
		ok = true
		return
	}
	_, err = fmt.Sscanf(nbID, "id(%d", &n) // closing bracket in format string omitted intensionally
	if nil == err {
		id = TriceID(n)
		ok = true
		return
	}
	msg.Info(fmt.Sprintln("no 'Id(n' or 'id(n' found inside " + nbID))
	return
}

// triceFmtParse returns an extracted tf and true if t is s.th. like 'TRICE*( Id(n), "..." );'
func triceFmtParse(t string) (tf TriceFmt, ok bool) {
	tf.Type = matchTypNameTRICE.FindString(t)
	if "" == tf.Type {
		msg.Info(fmt.Sprintln("no 'TRICE*' found inside " + t))
		return
	}
	match := matchFmtString.FindAllStringSubmatch(t, 1)
	if nil == match {
		msg.Info(fmt.Sprintln("no 'trice format string' found inside " + t))
		return
	}
	tf.Strg = match[0][1]
	ok = true
	return
}

// triceParse expects a string t containing a trice macro in the form `TRICE*(Id(n), "...", ...);`
// Returned nbID is the extracted string part containing 'Id(n)'.
// Returned id is the scanned n inside Id(n), only and only if n is a single decimal number.
// Returned tf is the recognized trice.
// Only on success ok is true.
func triceParse(t string) (nbID string, id TriceID, tf TriceFmt, ok bool) {
	nbID, id, ok = triceIDParse(t)
	if !ok {
		return
	}
	tf, ok = triceFmtParse(t)
	return
}

// refreshIDs parses text for valid trices tf and adds them to lu & tflu.
func refreshIDs(text string, lu TriceIDLookUp, tflu TriceFmtLookUp) {
	subs := text[:] // create a copy of text and assign it to subs
	for {
		loc := matchNbTRICE.FindStringIndex(subs) // find the next TRICE location in file
		if nil == loc {
			return // done
		}
		nbTRICE := subs[loc[0]:loc[1]] // full trice expression with Id(n)
		// prepare subs for next loop
		subs = subs[loc[1]:] // A possible Id(0) replacement makes subs not shorter, so next search can start at loc[1].
		// A case like 'TRICE*( Id(                             0                              ), "");' is not expected.

		_, id, tf, ok := triceParse(nbTRICE)
		if !ok {
			continue
		}
		tfS := tf
		tfS.Type = strings.ToUpper(tfS.Type) // Lower case and upper case Type are not distinguished.

		// In lu id could point to a different tf. So we need to check that and invalidate id in that case.
		// - That typically happens after tf was changed in source but the id not.
		// - Also the source file with id:tf could be added from a different project and refresh could not add it to lu because id is used differently.
		// Without this check double used IDs are silently loose one of their usages, what is ok, but this way we get a warning.
		if 0 != id {
			if tfL, ok := lu[id]; ok { // found
				tfL.Type = strings.ToUpper(tfL.Type)
				if !reflect.DeepEqual(tfS, tfL) { // Lower case and upper case Type are not distinguished.
					fmt.Println("Id", id, "already used differently, ignoring it.")
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

// updateIDsUniqOrShared parses text for new or invalid trices 'tf' and gives them the legacy id if 'tf' is already in lu & tflu.
// An invalid trice is a trice without Id(n) or with Id(0) or which changed somehow. Exampes: 'TRICE0( Id(12) ,"foo");' was changed to 'TRICE0( Id(12) ,"bar");'
// If 'TRICE0( Id(99) ,"bar");' is in lu & tflu the invalid trice changes to 'TRICE0( Id(99) ,"bar");'. Otherwise instead of 99 a so far unused id is taken.
// Or: 'TRICE0( Id(12) ,"foo");' was changed to 'TRICE0( Id(13) ,"foo");'. Than lu & tflu are extended accordingly, or, if 13 is already used, it is replaced with a new id.
// Otherwise a new id is generated, text patched and lu & tflu are extended.
// To work correctly, lu & tflu need to be in a refreshed state, means have all id:tf pairs from Srcs tree already inside.
// text is returned afterwards and true if text was changed and *pListModified set true if s.th. was changed.
// *pListModified in result is true if any file was changed.
// tflu holds the tf in upper case.
// lu holds the tf in source code case. If in source code upper and lower case occur, than only one can be in lu.
func updateIDsUniqOrShared(sharedIDs bool, smin, smax, min, max TriceID, text string, lu TriceIDLookUp, tflu TriceFmtLookUp, pListModified *bool) (string, bool) {
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

		nbID, id, tf, ok := triceParse(nbTRICE)
		if !ok {
			continue
		}
		tfS := tf
		st := isShortTrice(tf)
		if !st && sharedIDs {
			tfS.Type = strings.ToUpper(tfS.Type) // Lower case and upper case Type are not distinguished for normal trices in shared IDs mode.
		}
		// In lu id could point to a different tf. So we need to check that and invalidate id in that case.
		// - That typically happens after tf was changed in source but the id not.
		// - Also the source file with id:tf could be added from a different project and refresh could not add it to lu because id is used differently.
		if 0 != id {
			if tfL, ok := lu[id]; ok { // found
				if !st && sharedIDs {
					tfL.Type = strings.ToUpper(tfL.Type) // Lower case and upper case Type are not distinguished for normal trices in shared IDs mode.
				}
				if !reflect.DeepEqual(tfS, tfL) {
					id = -id // mark as invalid
				}
			}
		}
		if id <= 0 { // marked as invalid: id is 0 or inside lu used differently
			invalID := nbID
			invalTRICE := nbTRICE
			// It is possible tf is already in tflu (and lu) here, so check it.
			if id, ok = tflu[tfS]; sharedIDs && ok { // yes, we can use it in shared IDs mode
				msg.FatalInfoOnTrue(0 == id, "no id 0 allowed in map")
			} else { // no, we need a new one
				if st {
					id = lu.newID(smin, smax) // a prerequisite is a in a previous step refreshed lu
				} else {
					id = lu.newID(min, max) // a prerequisite is a in a previous step refreshed lu
				}
				*pListModified = true
			}
			// patch the id into text
			var nID string
			if st {
				nID = fmt.Sprintf("id(%5d)", id)
			} else {
				nID = fmt.Sprintf("Id(%6d)", id)
			}
			if Verbose {
				if nID != invalID {
					fmt.Print(invalID, " -> ")
				}
				fmt.Println(nID)
			}
			nbTRICE := strings.Replace(nbTRICE, invalID, nID, 1)
			text = strings.Replace(text, invalTRICE, nbTRICE, 1)
			fileModified = true
		}
		// update map: That is needed after an invalid trice or if id:tf is valid but not inside lu & tflu yet, for example after manual code changes or forgotten refresh before update.
		lu[id] = tf
		tflu[tfS] = id // no distinction for lower and upper case Type
	}
}

func isShortTrice(tf TriceFmt) bool {
	if 'T' == tf.Type[0] && 'r' == tf.Type[1] {
		return true
	}
	return false
}

// ZeroSourceTreeIds is overwriting with 0 all id's from source code tree srcRoot. It does not touch idlist.
func ZeroSourceTreeIds(srcRoot string, run bool) {
	err := filepath.Walk(srcRoot, visitZeroSourceTreeIds(run))
	if err != nil {
		panic(err)
	}
}

func visitZeroSourceTreeIds(run bool) filepath.WalkFunc {
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
			fmt.Println(path)
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
			found, modified, subs, s = zeroNextID(modified, subs, s)
			if false == found {
				break
			}
		}

		if modified && true == run {
			err = ioutil.WriteFile(path, []byte(s), 0)
		}
		return err
	}
}

// first retval flag is if an ID was zeroed, others are updated input values. if an ID wsa zeroed
// - modified gets true
// - subs gets shorter
// - s is updated
func zeroNextID(modified bool, subs, s string) (bool, bool, string, string) {
	loc := matchNbTRICE.FindStringIndex(subs)
	if nil == loc {
		return false, modified, subs, s
	}
	nbTRICE := subs[loc[0]:loc[1]]
	nbID := matchNbID.FindString(nbTRICE)
	if "" == nbID {
		msg.Info(fmt.Sprintln("No 'Id(n)' found inside " + nbTRICE))
		return false, modified, subs, s
	}

	zeroID := "Id(0)"
	fmt.Println(nbID, " -> ", zeroID)

	zeroTRICE := strings.Replace(nbTRICE, nbID, zeroID, 1)
	s = strings.Replace(s, nbTRICE, zeroTRICE, 1)
	// 2^32 has 9 ciphers and shortest trice has 14 chars: TRICE0(Id(1),"");
	// The replacement of n with 0 makes s shorter, so the next search shoud start like 10 chars earlier.
	subs = subs[loc[1]-10:]
	return true, true, subs, s
}
