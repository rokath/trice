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

// todo: remove static values from parameter lists (Verbose)

const (
	// patSourceFile is a regex pattern matching any source file for patching
	patSourceFile = "(\\.c|\\.h|\\.cc|\\.cpp|\\.hpp)$"

	// patNbTRICE is a regex pattern matching any "TRICE*(Id(n), "", ... )". - see https://regex101.com/r/id0uSF/1, The (?U) says non-greedy
	patNbTRICE = `(?U)(\bTRICE_S|trice_s|TRICE0|trice0|TRICE8_[1-8]|trice8_[1-8]|TRICE16_[1-4]|trice16_[1-4]|TRICE32_[1-4]|trice32_[1-4]|TRICE64_[1-4]|trice64_[1-4]\b)\s*\(\s*\bId\b\s*\(\s*.*[0-9]\s*\)\s*,\s*".*"\s*.*\)`

	// patNbID is a regex pattern matching any (first in string) "Id(n)" and usable in matches of matchNbTRICE
	patNbID = `\bId\s*\(\s*[0-9]*\s*\)`

	// patTypNameTRICE is a regex pattern matching "TRICE*" inside trice
	patTypNameTRICE = `(\bTRICE_S\b|\bTRICE0\b|\bTRICE8_[1-8]\b|\bTRICE16_[1-4]\b|\bTRICE32_[1-4]\b|\bTRICE64_[1-4]\b|\btrice_S\b|\btrice0\b|\btrice8_[1-8]\b|\btrice16_[1-4]\b|\btrice32_[1-4]\b|\btrice64_[1-4]\b)`

	// patFmtString is a regex matching the first format string inside trice
	patFmtString = `"(.*)"`

	// patFullTriceWithoutID is a regex find a TRICE* line without Id, The (?U) says non-greedy
	patFullTriceWithoutID = `(?U)(\bTRICE64|TRICE32|TRICE16|TRICE8|TRICE0|TRICE_S|trice64|trice32|trice16|trice8|trice0|trice_s\b)\s*\(\s*".*"\s*.*\)`

	// patTriceStartWithoutIDo is a regex
	patTriceStartWithoutIDo = `(\bTRICE64|TRICE32|TRICE16|TRICE8|TRICE0|TRICE_S|trice64|trice32|trice16|trice8|trice0|trice_s\b)\s*\(`

	// patTriceStartWithoutID is a regex
	patTriceStartWithoutID = `(\bTRICE64|TRICE32|TRICE16|TRICE8|TRICE0|TRICE_S|trice64|trice32|trice16|trice8|trice0|trice_s\b)\s*`

	// patNextFormatSpezifier is a regex find next format specifier in a string (exclude %%*)
	patNextFormatSpezifier = `(?:^|[^%])(%[0-9\.#]*(b|d|u|x|X|o|f))`
)

var (
	matchSourceFile           = regexp.MustCompile(patSourceFile)
	matchNbTRICE              = regexp.MustCompile(patNbTRICE)
	matchNbID                 = regexp.MustCompile(patNbID)
	matchTypNameTRICE         = regexp.MustCompile(patTypNameTRICE)
	matchFmtString            = regexp.MustCompile(patFmtString)
	matchFullTriceWithoutID   = regexp.MustCompile(patFullTriceWithoutID)
	matchTriceStartWithoutIDo = regexp.MustCompile(patTriceStartWithoutIDo)
	matchTriceStartWithoutID  = regexp.MustCompile(patTriceStartWithoutID)
	matchNextFormatSpezifier  = regexp.MustCompile(patNextFormatSpezifier)
)

func isSourceFile(fi os.FileInfo) bool {
	return matchSourceFile.MatchString(fi.Name())
}

func separatedIDsUpdate(root string, lu TriceIDLookUp, tflu TriceFmtLookUp, pListModified *bool) {
	// to do
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
func sharedIDsUpdate(root string, lu TriceIDLookUp, tflu TriceFmtLookUp, pListModified *bool) {
	if Verbose {
		fmt.Println("dir=", root)
		fmt.Println("List=", FnJSON)
	}
	msg.FatalInfoOnErr(filepath.Walk(root, visitUpdate(lu, tflu, pListModified)), "failed to walk tree")
}

func visitRefresh(lu TriceIDLookUp, tflu TriceFmtLookUp) filepath.WalkFunc {
	return func(path string, fi os.FileInfo, err error) error {
		if err != nil || fi.IsDir() || !isSourceFile(fi) {
			return err // forward any error and do nothing
		}
		if Verbose {
			fmt.Println(path)
		}
		read, err := ioutil.ReadFile(path)
		if nil != err {
			return err
		}
		text := string(read)
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
		if err != nil || fi.IsDir() || !isSourceFile(fi) {
			return err // forward any error and do nothing
		}
		if Verbose {
			fmt.Println(path)
		}
		read, err := ioutil.ReadFile(path)
		if nil != err {
			return err
		}
		text := string(read)
		textN := updateParamCount(text)                                        // update parameter count: TRICE* to TRICE*_n
		textU, fileModified := updateIDsShared(textN, lu, tflu, pListModified) // update IDs: Id(0) -> Id(M)

		// write out
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
		msg.InfoOnTrue(Verbose, fmt.Sprintln("No 'Id(n)' found inside "+t))
		return
	}

	var n int
	_, err := fmt.Sscanf(nbID, "Id(%d", &n) // closing bracket in format string omitted intensionally
	if nil != err {                         // because spaces after id otherwise are not tolerated
		msg.Info(fmt.Sprintln("no 'Id(n' found inside " + nbID))
		return
	}
	//n, e := strconv.Atoi(nbID)
	//msg.FatalOnErr(e) // todo: error is here not possible
	id = TriceID(n)
	ok = true
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
// Returned id is the scanned n inside Id(n), only and only if n is a single decimal number.
// Returned tf is the recognized trice.
// Returned nbID is the extracted string part containing 'Id(n)'.
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
			tflu[tfS] = id // no distiction for lower and upper case Type
		}
	}
}

// updateIDsShared parses text for new or invalid trices tf and gives them the legacy id if tf is already in lu & tflu.
// An invalid trice is a trice without Id(n) or with Id(0) or which changed somehow. Exampes: 'TRICE0( Id(12) ,"foo");' was changed to 'TRICE0( Id(12) ,"bar");'
// If 'TRICE0( Id(99) ,"bar");' is in lu & tflu the invalid trice changes to 'TRICE0( Id(99) ,"bar");'. Otherwise instead of 99 a so far unused id is taken.
// Or: 'TRICE0( Id(12) ,"foo");' was changed to 'TRICE0( Id(13) ,"foo");'. Than lu & tflu are extended accordingly, or, if 13 is already used is replaced with a new id.
// Otherwise a new id is generated, text patched and lu & tflu are extended.
// To work correctly, lu & tflu need to be in a refreshed state, means have all id:tf pairs from Srcs tree already inside.
// text is returned afterwards and true if text was changed and *pListModified set true if s.th. was changed.
// *pListModified in result is true if any file was changed.
// tflu holds the tf in upper case.
// lu holds the tf in source code case. If in source code upper and lower case occur, than only one can be in lu.
func updateIDsShared(text string, lu TriceIDLookUp, tflu TriceFmtLookUp, pListModified *bool) (string, bool) {
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
		tfS.Type = strings.ToUpper(tfS.Type) // Lower case and upper case Type are not distinguished.

		// In lu id could point to a different tf. So we need to check that and invalidate id in that case.
		// - That typically happens after tf was changed in source but the id not.
		// - Also the source file with id:tf could be added from a different project and refresh could not add it to lu because id is used differently.
		if 0 != id {
			if tfL, ok := lu[id]; ok { // found
				tfL.Type = strings.ToUpper(tfL.Type)
				if !reflect.DeepEqual(tfS, tfL) { // Lower case and upper case Type are not distinguished.
					id = -id // mark as invalid
				}
			}
		}
		if id <= 0 { // invalid: id is 0 or inside lu used differently
			invalID := nbID
			invalTRICE := nbTRICE
			// It is possible tf is already in tflu (and lu) here, so check it.
			if id, ok = tflu[tfS]; ok { // yes, we can use it
				msg.FatalOnTrue(0 == id) // no id 0 allowed in map
			} else { // no, we need a new one
				id = lu.newID() // a prerequisite is a in a previous step refreshed lu
			}
			// patch the id into text
			newID := fmt.Sprintf("Id(%5d)", id)
			if Verbose {
				if newID != invalID {
					fmt.Print(invalID, " -> ")
				}
				fmt.Println(newID)
			}
			nbTRICE := strings.Replace(nbTRICE, invalID, newID, 1)
			text = strings.Replace(text, invalTRICE, nbTRICE, 1)
			*pListModified = true
			fileModified = true
		}
		// update map: That is needed after an invalid trice or if id:tf is valid but not inside lu & tflu yet, for example after manual code changes or forgotten refresh before update.
		lu[id] = tf
		tflu[tfS] = id // no distiction for lower and upper case Type
	}
}

// updateParamCount stays in each file as long TRICE* statements without ID() are found.
// If a TRICE* is found it is getting an Id(0) inserted and it is also extended by _n
// according to the format specifier count inside the formatstring
//
// text is the full filecontents, which could be modified, therefore it is also returned
func updateParamCount(text string) string {
	subs := text[:] // create a copy of text and assign it to subs
	for {
		loc := matchFullTriceWithoutID.FindStringIndex(subs) // find the next TRICE location in file
		if nil == loc {
			return text // done
		}
		trice := subs[loc[0]:loc[1]]                                  // the whole TRICE*(*);
		triceO := matchTriceStartWithoutIDo.FindString(trice)         // TRICE*( part (the trice start)
		triceS := matchTriceStartWithoutID.FindString(trice)          // TRICE* part (the trice start)
		triceN := strings.Replace(trice, triceO, triceO+" Id(0),", 1) // insert Id(0)

		// count % format spezifier inside formatstring
		p := triceN
		var n int
		xs := "any"
		for "" != xs {
			lo := matchNextFormatSpezifier.FindStringIndex(p)
			xs = matchNextFormatSpezifier.FindString(p)
			if "" != xs { // found
				n++
				p = p[lo[1]:]
			} else {
				xs = ""
			}
		}
		if n > 0 { // patch
			newName := fmt.Sprintf(triceS+"_%d", n)              // TRICE*_n
			triceN = strings.Replace(triceN, triceS, newName, 1) // insert _n
		} else {
			// to do: handle special case 0==n
		}

		if Verbose {
			fmt.Println(trice)
			fmt.Println("->")
			fmt.Println(triceN)
		}
		text = strings.Replace(text, trice, triceN, 1) // modify s
		subs = subs[loc[1]:]                           // The replacement makes s not shorter, so next search can start at loc[1]
	}
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
