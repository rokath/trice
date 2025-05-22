// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

// source tree management

import (
	"fmt"
	"io"
	"os"
	"path/filepath"
	"reflect"
	"strings"

	"github.com/rokath/trice/pkg/msg"
	"github.com/spf13/afero"
)

const (

	// patSourceFile is a regex pattern matching any source file for patching (https://stackoverflow.com/questions/1545080/c-code-file-extension-what-is-the-difference-between-cc-and-cpp)
	patSourceFile = "(\\.c|\\.cc|\\.cp|\\.cxx|\\.cpp|\\.CPP|\\.c\\+\\+|\\.C|\\.h|\\.hh|\\.hp|\\.hxx|\\.hpp|\\.HPP|\\.h\\+\\+|\\.H|\\.ixx|\\.inc)$"

	// patNbID is a regex pattern matching any first "Id(n)" and usable in matches after patTypNameTRICE. It works also over line breaks.
	patNbID = `(?Ui)\b(i)(D)\b\s*\(\s*\d+\s*\)` // https://regex101.com/r/2BlNSv/1

	// patFmtString is a regex matching the first format string inside trice even containing newlines and \"
	patFmtString = `"(?s)(.*)"` // https://stackoverflow.com/questions/159118/how-do-i-match-any-character-across-multiple-lines-in-a-regular-expression

	// patNextFormatSpecifier is a regex to find next format specifier in a string (exclude %%*)
	// todo: unify with decoder.patNextFormatSpecifier
	patNextFormatSpecifier = `(?:^|[^%])(%[\ +\-0-9\.#]*(b|c|d|e|f|g|E|F|G|h|i|l|L|n|o|O|p|q|s|t|u|U|x|X))`

	patID = `\s*\b(i|I)(d|D)\b\s*` // `\s*\b(I|i)d\b\s*`

	patNb = `\d+` // // `[0-9]*`
)

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

func isSourceFile(fi os.FileInfo) bool {
	return matchSourceFile.MatchString(fi.Name())
}

func refreshList(w io.Writer, fSys *afero.Afero, root string, ilu TriceIDLookUp, flu triceFmtLookUp, lim TriceIDLookUpLI) {
	if Verbose {
		fmt.Fprintln(w, "dir=", root)
		fmt.Fprintln(w, "List=", FnJSON)
	}
	msg.FatalInfoOnErr(filepath.Walk(root, visitRefresh(w, fSys, ilu, flu, lim)), "failed to walk tree")
}

func readFile(w io.Writer, fSys *afero.Afero, path string, fi os.FileInfo, err error) (string, error) {
	if err != nil || fi.IsDir() || !isSourceFile(fi) {
		return "", err // forward any error and do nothing
	}
	if Verbose {
		fmt.Fprintln(w, path)
	}
	read, err := fSys.ReadFile(path)
	if nil != err {
		return "", err
	}
	text := string(read)
	return text, nil
}

func visitRefresh(w io.Writer, fSys *afero.Afero, ilu TriceIDLookUp, flu triceFmtLookUp, lim TriceIDLookUpLI) filepath.WalkFunc {
	return func(path string, fi os.FileInfo, err error) error {
		text, err := readFile(w, fSys, path, fi, err)
		if nil != err {
			return err
		}
		refreshIDs(w, path, text, ilu, flu, lim) // update ilu & lim
		return nil
	}
}

// triceIDParse returns an extracted id and found as true if t starts with s.th. like 'TRICE*( Id(n)...'
// nbID is the extracted string part containing 'Id(n)'.
func triceIDParse(t string) (nbID string, id TriceID, found idType) {
	nbID = matchNbID.FindString(t)
	if nbID == "" {
		msg.InfoOnTrue(Verbose, fmt.Sprintln("No 'Id(n)' or 'id(n)' found inside "+t)) // todo: patID
		return
	}
	var n int
	_, err := fmt.Sscanf(nbID, "iD(%d", &n) // closing bracket in format string omitted intentionally // todo: patID
	if nil == err {                         // because spaces after id otherwise are not tolerated
		id = TriceID(n)
		found = idTypeS8
		return
	}
	_, err = fmt.Sscanf(nbID, "ID(%d", &n) // closing bracket in format string omitted intentionally // todo: patID
	if nil == err {                        // because spaces after id otherwise are not tolerated
		id = TriceID(n)
		found = idTypeS4
		return
	}
	_, err = fmt.Sscanf(nbID, "Id(%d", &n) // closing bracket in format string omitted intentionally // todo: patID
	if nil == err {                        // because spaces after id otherwise are not tolerated
		id = TriceID(n)
		found = idTypeS2
		return
	}
	_, err = fmt.Sscanf(nbID, "id(%d", &n) // closing bracket in format string omitted intentionally // todo: patID
	if nil == err {                        // because spaces after id otherwise are not tolerated
		id = TriceID(n)
		found = idTypeS0
		return
	}
	msg.Info(fmt.Sprintln("no 'Id(n' found inside " + nbID)) // todo: patID
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
func triceParse(t string) (nbID string, id TriceID, tf TriceFmt, found idType) {
	nbID, id, found = triceIDParse(t)
	tf, ok := triceFmtParse(t)
	if !ok {
		msg.Info(fmt.Sprintln("triceFmtParse reported !ok inside " + t))
	}
	return
}

// refreshIDs parses text for valid trices tf and adds them to ilu & flu and updates location information map lim.
func refreshIDs(w io.Writer, fileName, text string, ilu TriceIDLookUp, flu triceFmtLookUp, lim TriceIDLookUpLI) {
	subs := text[:] // create a copy of text and assign it to subs
	line := 1       // source code lines start with 1 for some reason
	var li TriceLI
	li.File = filepath.Base(fileName)
	for {
		loc := matchNbTRICE.FindStringSubmatchIndex(subs) // find the next TRICE location in file
		if nil == loc {
			return // done
		}
		line += strings.Count(subs[:loc[0]], "\n")
		nbTRICE := subs[loc[0]:loc[1]] // full trice expression with Id(n)
		// prepare subs for next loop
		subs = subs[loc[1]:] // A possible Id(0) replacement makes subs not shorter, so next search can start at loc[1].
		// A case like 'TRICE*( Id(                             0                              ), "");' is not expected.

		_, id, tf, _ /*found*/ := triceParse(nbTRICE)
		tfS := tf

		// In ilu id could point to a different tf. So we need to check that and invalidate id in that case.
		// - That typically happens after tf was changed in source but the id not.
		// - Also the source file with id:tf could be added from a different project and refresh could not add it to ilu because id is used differently.
		// Without this check double used IDs are silently loose one of their usages, what is ok, but this way we get a warning.
		if id != 0 {
			if tfL, ok := ilu[id]; ok { // found
				//tfL.Type = strings.ToUpper(tfL.Type)
				if !reflect.DeepEqual(tfS, tfL) { // Lower case and upper case Type are not distinguished.
					fmt.Fprintln(w, "Id", id, "in", fileName, "already used differently in", FnJSON, "- you have to decide!") // todo: patID
					id = -id                                                                                                  // mark as invalid
				}
			}
		}
		if id > 0 {
			li.Line = line
			lim[id] = li
			ilu[id] = tf
			addID(tfS, id, flu)
		}
	}
}

type idType int

const (
	idTypeS8 = 0
	idTypeS4 = 3
	idTypeS2 = 2
	idTypeS0 = 1
)
