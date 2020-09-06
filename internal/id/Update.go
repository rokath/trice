// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

// source tree management

import (
	"fmt"
	"io/ioutil"
	"os"
	"path/filepath"
	"regexp"
	"strings"
)

// todo: remove static values from parameter lists (Verbose)

const (
	// patSourceFile is a regex pattern matching any source file for patching
	patSourceFile = "(\\.c|\\.h|\\.cc|\\.cpp|\\.hpp)$"

	// patNbTRICE is a regex pattern matching any "TRICE*(Id(n), "", ... )". - see https://regex101.com/r/LNlAwY/7, The (?U) says non-greedy
	patNbTRICE = `(?U)(TRICE0|TRICE8_[1-8]|TRICE16_[1-4]|TRICE32_[1-4]|TRICE64_[1-4])\s*\(\s*\bId\b\s*\(\s*.*[0-9]\s*\)\s*,\s*".*"\s*.*\)`

	// patNbID is a regex pattern matching any (first in string) "Id(n)" and usable in matches of matchNbTRICE
	patNbID = `\bId\s*\(\s*[0-9]*\s*\)`

	// patTypNameTRICE is a regex pattern matching "TRICE*" inside trice
	patTypNameTRICE = `(\bTRICE0\b|\bTRICE8_[1-8]\b|\bTRICE16_[1-4]\b|\bTRICE32_[1-4]|\bTRICE64_[1-4])`

	// patFmtString is a regex matching the first format string inside trice
	patFmtString = `"(.*)"`

	// patFullTriceWithoutID is a regex find a TRICE* line without Id, The (?U) says non-greedy
	patFullTriceWithoutID = `(?U)(\bTRICE64|TRICE32|TRICE16|TRICE8|TRICE0\b)\s*\(\s*".*"\s*.*\)`

	// patTriceStartWithoutIDo is a regex
	patTriceStartWithoutIDo = `(\bTRICE64|TRICE32|TRICE16|TRICE8|TRICE0\b)\s*\(`

	// patTriceStartWithoutID is a regex
	patTriceStartWithoutID = `(\bTRICE64|TRICE32|TRICE16|TRICE8|TRICE0\b)\s*`

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

// Update is parsing source tree root and performing these actions:
// - replace FmtType( Id(0), ...) with FmtType( Id(n), ...)
// - find duplicate FmtType( Id(n), ...) and replace one of them if trices are not identical
// - extend file fnIDList
func (p *List) Update(root string, run, verbose bool) error {
	fmt.Println("dir=", root)
	fmt.Println("list=", p.fnJSON)
	listModified := false
	//err :=
	p.ReadListFile()
	//if nil != err {
	//	fmt.Println(fn + " not found, creating a new one")
	//	if true == run {
	//		err = p.write(fn)
	//		if nil != err {
	//			return fmt.Errorf("failed to write %s: %v", fn, err)
	//		}
	//	}
	//}
	err := filepath.Walk(root, visitUpdate(run, p, &listModified, verbose))
	if nil != err {
		return fmt.Errorf("failed to walk tree: %v", err)
	}
	if listModified && true == run {
		err = p.writeListFile()
		if nil != err {
			return fmt.Errorf("failed to write %s: %v", p.fnJSON, err)
		}
	}
	return nil
}

func visitUpdate(run bool, p *List, pListModified *bool, verbose bool) filepath.WalkFunc {
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
		if verbose {
			fmt.Println(path)
		}
		pathModified := false
		read, err := ioutil.ReadFile(path)
		if nil != err {
			return err
		}
		s := string(read)

		// update parameter count (TRICE* to TRICE*_n)
		subs := s[:len(s)]
		for {
			var found bool
			found, pathModified, subs, s = updateParamCount(pathModified, subs, s, verbose)
			if false == found {
				break
			}
		}

		// update IDs
		subs = s[:len(s)]
		for {
			var found bool
			found, pathModified, subs, s = updateNextID(p, pListModified, pathModified, subs, s, verbose)
			if false == found {
				break
			}
		}

		// write out
		if pathModified && true == run {
			err = ioutil.WriteFile(path, []byte(s), fi.Mode())
			if nil != err {
				return fmt.Errorf("failed to change %s: %v", path, err)
			}
		}
		return nil
	}
}

// updateNextID is getting these parameters:
//    - p = pointer to ID list
//    - pListModified = pointer to the 'ID list modified flag', which is set true if s.th. changed in the list
//    - modified = the 'file modified flag', which is returned set true if s.th. changed in the file
//    - subs = the remaining file contents
//    - s = the full filecontents, which could be modified
//    - verbose flag
// updateNextID is returning these values (left to right):
//    - id flag is true if an ID was changed
//    - modified flag is true when any id was changed in the file
//    - subs gets shorter
//    - s is updated
func updateNextID(p *List, pListModified *bool, modified bool, subs, s string, verbose bool) (bool, bool, string, string) {
	loc := matchNbTRICE.FindStringIndex(subs) // find the next TRICE location in file
	if nil == loc {
		return false, modified, subs, s // done
	}
	nbTRICE := subs[loc[0]:loc[1]]
	nbID := matchNbID.FindString(nbTRICE)
	if "" == nbID {
		fmt.Println("error: No 'Id(n)' found inside " + nbTRICE)
		return false, modified, subs, s
	}
	var id int
	_, err := fmt.Sscanf(nbID, "Id(%d", &id) // closing bracket in format string omitted intensionally
	if nil != err {                          // because spaces after id otherwise are not tolerated
		fmt.Println("error: No 'Id(n)' found inside " + nbID)
		return false, modified, subs, s
	}
	if 0 == id {
		zeroID := nbID
		zeroTRICE := nbTRICE
		id /*err*/ = p.newID()
		// if nil != err {
		// 	fmt.Println("error: No new ID found")
		// 	return false, modified, subs, s
		// }
		newID := fmt.Sprintf("Id(%5d)", id)
		if verbose {
			fmt.Println(zeroID, " -> ", newID)
		}
		nbTRICE := strings.Replace(nbTRICE, zeroID, newID, 1)
		s = strings.Replace(s, zeroTRICE, nbTRICE, 1)
		modified = true
	}
	// The replacement makes s not shorter, so next search can start at loc[1]
	subs = subs[loc[1]:]
	typNameTRICE := matchTypNameTRICE.FindString(nbTRICE)
	if "" == typNameTRICE {
		fmt.Println("error: no 'TRICE*' found inside " + typNameTRICE)
		return false, modified, subs, s
	}
	match := matchFmtString.FindAllStringSubmatch(nbTRICE, 1)
	fmtString := match[0][1]
	nID, flag := p.extendIDList(id, typNameTRICE, fmtString, verbose)
	if flag {
		*pListModified = true
		if nID != id { // a new id was generated
			//oID := fmt.Sprintf("Id(%5d)", id)
			newID := fmt.Sprintf("Id(%5d)", nID)
			if verbose {
				fmt.Println(nbID, " -> ", newID)
			}
			newTRICE := strings.Replace(nbTRICE, nbID, newID, 1)
			s = strings.Replace(s, nbTRICE, newTRICE, 1)
			modified = true
		}
	}
	return true, modified, subs, s // next done
}

// updateParamCount is called in a loop for each file as long TRICE* statements without ID() are found
// If a TRICE* is found it is getting an Id(0) inserted and it is also extended by _n
// according to the format specifier count inside the formatstring
//
// updateParamCount is getting these parameters:
//    - modified = the 'file modified flag', which is returned set true if s.th. changed in the file
//    - subs = the remaining file contents
//    - s = the full filecontents, which could be modified
//    - verbose flag
// updateNextID is returning these values (left to right):
//    - modified flag is true when anything was changed in the file
//    - subs gets shorter
//    - s is updated
func updateParamCount(modified bool, subs, s string, verbose bool) (bool, bool, string, string) {
	loc := matchFullTriceWithoutID.FindStringIndex(subs) // find the next TRICE location in file
	if nil == loc {
		return false, modified, subs, s // done
	}
	modified = true
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
		// todo: handle special case 0==n
	}

	if verbose {
		fmt.Println(trice)
		fmt.Println("->")
		fmt.Println(triceN)
	}
	s = strings.Replace(s, trice, triceN, 1) // modify s
	subs = subs[loc[1]:]                     // The replacement makes s not shorter, so next search can start at loc[1]
	return true, modified, subs, s           // next done
}

// ZeroSourceTreeIds is overwriting with 0 all id's from source code tree srcRoot. It does not touch idlist.
func zeroSourceTreeIds(srcRoot string, run bool) {
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

		fmt.Println(path)
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
		fmt.Println("No 'Id(n)' found inside " + nbTRICE)
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
