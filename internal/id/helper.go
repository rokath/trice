// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

// source tree management

import (
	"bufio"
	"errors"
	"fmt"
	"io"
	"os"
	"path"
	"runtime"
	"slices"
	"strconv"
	"strings"
	"time"

	"github.com/rokath/trice/pkg/msg"
	"github.com/spf13/afero"
)

var SkipAdditionalChecks bool

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

// fileExists returns true, if path exits.
func fileExists(fSys *afero.Afero, path string) bool {
	if _, err := fSys.Stat(path); err == nil {
		return true // path exists
	} else if errors.Is(err, os.ErrNotExist) {
		return false // path does *not* exist
	} else {
		return false
		// Schrodinger: file may or may not exist. See err for details.
		// Therefore, do *NOT* use !os.IsNotExist(err) to test for file existence
	}
} // https://stackoverflow.com/questions/12518876/how-to-check-if-a-file-exists-in-go

// CopyFileWithMTime copies file src into dst and sets dst mtime equal to src mtime.
func CopyFileWithMTime(fSys *afero.Afero, dst, src string) error {
	srcMTime := MTime(fSys, src) // get mtime before opening src
	source, err := fSys.Open(src)
	if err != nil {
		return err
	}
	defer source.Close()

	destination, err := fSys.Create(dst)
	if err != nil {
		return err
	}
	// No defer destination.Close() here because of following func fSys.Chtimes().

	_, err = io.Copy(destination, source)
	if err != nil {
		return err
	}

	err = destination.Sync() // Sync here to ensure file is written. (Maybe not needed and/or not working)
	if err != nil {
		return err
	}

	err = destination.Close() // Close here because of following func fSys.Chtimes().
	if err != nil {
		return err
	}

	return fSys.Chtimes(dst, time.Time{}, srcMTime) // Copy src mtime.
}

func MTime(fSys *afero.Afero, fName string) time.Time {
	stat, err := fSys.Stat(fName)
	msg.FatalOnErr(err)
	return stat.ModTime()
}

// join appends err to p.err, when err is not nil.
func (p *idData) join(err error) {
	if err != nil {
		// notice that we're using 1, so it will actually log the where
		// the error happened, 0 = this function, we don't want that.
		pc, fn, line, _ := runtime.Caller(1)
		loc := fmt.Errorf("[error] in %s[%s:%d] %v", runtime.FuncForPC(pc).Name(), fn, line, err)
		p.err = errors.Join(p.err, loc, err)
	}
}

// evaluateTriceParameterCount analyzes rest, if it has the correct parameter count according to t.
// rest starts immediately after the trice format string and can be very long (the remaining file context),
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
	gap := ""
	if SpaceInsideParenthesis {
		gap = " "
	}
	var idName string
	if t.Type[2] == 'i' { // lower case letter
		idName = "iD("
	} else {
		if loc[3] != loc[4] {
			idName = s[offset+loc[3]:offset+loc[3]+2] + "("
		} else {
			if DefaultStampSize == 32 {
				idName = "ID("
			} else if DefaultStampSize == 16 {
				idName = "Id("
			} else {
				idName = "id("
			}
		}
	}
	idName = gap + idName + gap
	first := s[:offset+loc[2]]                            // first is the not touched s part before the replacement space.
	idSiz := loc[5] - loc[2]                              // idSiz is the size of the replaced ID space inside the source code.
	last := s[offset+loc[5]:]                             // last is the not touched s part after the replacement space.
	idIns := idName + strconv.Itoa(int(id)) + gap + "), " // idIns is the ID statement replace string.
	result = first + idIns + last                         //
	delta = len(idIns) - idSiz                            // delta is the offset change.
	return
}

// cleanID inserts id 0 into s or removes ID statement according to loc information and returns the result together with the changed len.
func cleanID(s string, offset int, loc []int, t TriceFmt) (result string, delta int) {
	// code is similar to writeiD code for clarity.
	if t.Type[2] == 'I' { // Upper case letter (s.th. like TRICE*...), we set id just to 0.
		return writeID(s, offset, loc, t, 0)
	}
	first := s[:offset+loc[2]] // first is the not touched s part before the replacement space.
	idSiz := loc[5] - loc[2]   // idSiz is the size of the replaced ID space inside the source code.
	last := s[offset+loc[5]:]  // last is the not touched s part after the replacement space.
	var idIns string           // replacement string
	if SpaceInsideParenthesis {
		idIns = " "
	}
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

/*
// removeIndex removes the element at index from s.
// https://stackoverflow.com/questions/37334119/how-to-delete-an-element-from-a-slice-in-golang
func removeIndex(s TriceIDs, index int) TriceIDs {
	return append(s[:index], s[index+1:]...)
}
*/
