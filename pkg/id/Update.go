// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// All rights reserved.
// Use of this source code is governed by a
// license that can be found in the LICENSE file.

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

// https://gist.github.com/tdegrunt/045f6b3377f3f7ffa408 replace strings recursively

var matchSourceFile = regexp.MustCompile("(\\.c|\\.h|\\.cc|\\.cpp|\\.hpp)$")

// matchNbTL is a as constant used compiled regex matching any (first in line) "TL*(Id(n), "", ... );". - see https://regex101.com/r/LNlAwY/7
var matchNbTL = regexp.MustCompile(`(\bTL0\b|\bTL8_[1-8]\b|\bTL16_[1-4]\b|\bTL32_[1-2])\s*\(\s*\bId\b\s*\(\s*.*[0-9]\s*\)\s*,\s*".*"\s*.*\)\s*;`)

// matchNbID is a as constant used compiled regex matching any (first in string) "Id(n)" and usable in matches of matchNbTL
var matchNbID = regexp.MustCompile(`\bId\s*\(\s*[0-9]*\s*\)`)

// matchTypNameTL is a as constant used compiled regex matching "TL*" inside traceLog
var matchTypNameTL = regexp.MustCompile(`(\bTL0\b|\bTL8_[1-8]\b|\bTL16_[1-4]\b|\bTL32_[1-2])`)

// matchFmtString is a as constant used compiled regex matching the first format string inside traceLog
var matchFmtString = regexp.MustCompile(`"(.*)"`)

func isSourceFile(fi os.FileInfo) bool {
	return matchSourceFile.MatchString(fi.Name())
}

// Update is parsing source tree root and performing these actions:
// - replace FmtType( Id(0), ...) with FmtType( Id(n), ...)
// - find duplicate FmtType( Id(n), ...) and replace one of them if traceLogs are not identical
// - extend file fnIDList
func (p *List) Update(root, fn string, run bool) error {
	fmt.Println("dir=", root)
	fmt.Println("list=", fn)
	listModified := false
	err := p.Read(fn)
	if nil != err {
		fmt.Println(fn + " not found, creating a new one")
		if true == run {
			err = p.write(fn)
			if nil != err {
				return fmt.Errorf("failed to write %s: %v", fn, err)
			}
		}
	}
	err = filepath.Walk(root, visitUpdate(run, p, &listModified))
	if nil != err {
		return fmt.Errorf("failed to walk tree: %v", err)
	}
	if listModified && true == run {
		err = p.write(fn)
		if nil != err {
			return fmt.Errorf("failed to write %s: %v", fn, err)
		}
	}
	return nil
}

func visitUpdate(run bool, p *List, pListModified *bool) filepath.WalkFunc {
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
		fmt.Println(path)
		pathModified := false
		read, err := ioutil.ReadFile(path)
		if nil != err {
			return err
		}
		s := string(read)
		subs := s[:len(s)]

		for {
			var found bool
			found, pathModified, subs, s = updateNextID(p, pListModified, pathModified, subs, s)
			if false == found {
				break
			}
		}
		if pathModified && true == run {
			err = ioutil.WriteFile(path, []byte(s), 0)
			if nil != err {
				return fmt.Errorf("failed to change %s: %v", path, err)
			}
		}
		return nil
	}
}

// first retval flag is if an ID was zeroed, others are updated input values. if an ID wsa zeroed
// - modified gets true
// - subs gets shorter
// - s is updated
func updateNextID(p *List, pListModified *bool, modified bool, subs, s string) (bool, bool, string, string) {
	loc := matchNbTL.FindStringIndex(subs)
	if nil == loc {
		return false, modified, subs, s
	}
	nbTL := subs[loc[0]:loc[1]]
	nbID := matchNbID.FindString(nbTL)
	if "" == nbID {
		fmt.Println("No 'Id(n)' found inside " + nbTL)
		return false, modified, subs, s
	}
	var id int
	_, err := fmt.Sscanf(nbID, "Id(%d", &id) // closing bracket in format string omitted intensionally
	if nil != err {                          // because spaces after id otherwise are not tolerated
		fmt.Println("No 'Id(n)' found inside " + nbID)
		return false, modified, subs, s
	}
	if 0 == id {
		zeroID := nbID
		zeroTL := nbTL
		id, err = p.newID()
		if nil != err {
			fmt.Println("No new ID found")
			return false, modified, subs, s
		}
		nbID = fmt.Sprintf("Id(%5d)", id)
		fmt.Println(zeroID, " -> ", nbID)
		nbTL := strings.Replace(nbTL, zeroID, nbID, 1)
		s = strings.Replace(s, zeroTL, nbTL, 1)
		modified = true
	}
	// The replacement makes s not shorter, so next seach can start at loc[1]
	subs = subs[loc[1]:]
	typNameTL := matchTypNameTL.FindString(nbTL)
	if "" == typNameTL {
		fmt.Println("no 'TL*' found inside " + typNameTL)
		return false, modified, subs, s
	}
	match := matchFmtString.FindAllStringSubmatch(nbTL, 1)
	fmtString := match[0][1]
	nID, flag := p.extend(id, typNameTL, fmtString)
	if flag {
		*pListModified = true
		if nID != id { // a new id was generated
			oID := fmt.Sprintf("Id(%5d)", id)
			nID := fmt.Sprintf("Id(%5d)", nID)
			fmt.Println(oID, " -> ", nID)
			newTL := strings.Replace(nbTL, oID, nID, 1)
			s = strings.Replace(s, nbTL, newTL, 1)
			modified = true
		}
	}
	return true, modified, subs, s
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
	loc := matchNbTL.FindStringIndex(subs)
	if nil == loc {
		return false, modified, subs, s
	}
	nbTL := subs[loc[0]:loc[1]]
	nbID := matchNbID.FindString(nbTL)
	if "" == nbID {
		fmt.Println("No 'Id(n)' found inside " + nbTL)
		return false, modified, subs, s
	}

	zeroID := "Id(0)"
	fmt.Println(nbID, " -> ", zeroID)

	zeroTL := strings.Replace(nbTL, nbID, zeroID, 1)
	s = strings.Replace(s, nbTL, zeroTL, 1)
	// 2^32 has 9 ciphers and shortest traceLog has 14 chars: TL0(Id(1),"");
	// The replacement of n with 0 makes s shorter, so the next search shoud start like 10 chars earlier.
	subs = subs[loc[1]-10:]
	return true, true, subs, s
}
