// SPDX-License-Identifier: MIT

package id

// source tree management

import (
	"bufio"
	"errors"
	"fmt"
	"io"
	"os"
	"path"
	"regexp"
	"runtime"
	"slices"
	"sort"
	"strconv"
	"strings"
	"time"

	"github.com/rokath/trice/internal/fmtspec"
	"github.com/rokath/trice/pkg/msg"
	"github.com/spf13/afero"
)

var SkipAdditionalChecks bool

const (
	triceInsertOffMarker        = "TRICE_INSERT_OFF"
	triceInsertOnMarker         = "TRICE_INSERT_ON"
	triceDiagnosticContextLines = 6
	triceDiagnosticMaxLineRunes = 240
)

// maskTriceInsertDisabledRegions blanks source regions between TRICE_INSERT_OFF
// and TRICE_INSERT_ON markers while preserving byte offsets and line numbers.
func maskTriceInsertDisabledRegions(s string) string {
	var (
		disabled    bool
		inBlockCmnt bool
		inLineCmnt  bool
		inString    bool
		inChar      bool
		escaped     bool
	)
	out := []byte(s)
	for i := 0; i < len(s); i++ {
		if !inString && !inChar {
			if hasMarkerAt(s, i, triceInsertOffMarker) {
				disabled = true
				i += len(triceInsertOffMarker) - 1
				continue
			}
			if hasMarkerAt(s, i, triceInsertOnMarker) {
				disabled = false
				i += len(triceInsertOnMarker) - 1
				continue
			}
		}

		c := s[i]
		if disabled && c != '\n' && c != '\r' {
			out[i] = ' '
		}

		switch {
		case inLineCmnt:
			if c == '\n' {
				inLineCmnt = false
			}
		case inBlockCmnt:
			if c == '*' && i+1 < len(s) && s[i+1] == '/' {
				if disabled {
					out[i+1] = ' '
				}
				inBlockCmnt = false
				i++
			}
		case inString:
			if escaped {
				escaped = false
			} else if c == '\\' {
				escaped = true
			} else if c == '"' {
				inString = false
			}
		case inChar:
			if escaped {
				escaped = false
			} else if c == '\\' {
				escaped = true
			} else if c == '\'' {
				inChar = false
			}
		default:
			if c == '/' && i+1 < len(s) {
				switch s[i+1] {
				case '/':
					if disabled {
						out[i+1] = ' '
					}
					inLineCmnt = true
					i++
				case '*':
					if disabled {
						out[i+1] = ' '
					}
					inBlockCmnt = true
					i++
				}
			} else if c == '"' {
				inString = true
			} else if c == '\'' {
				inChar = true
			}
		}
	}
	return string(out)
}

func hasMarkerAt(s string, pos int, marker string) bool {
	if !strings.HasPrefix(s[pos:], marker) {
		return false
	}
	before := pos > 0 && isCIdentifierByte(s[pos-1])
	afterPos := pos + len(marker)
	after := afterPos < len(s) && isCIdentifierByte(s[afterPos])
	return !before && !after
}

func isCIdentifierByte(b byte) bool {
	return b == '_' || '0' <= b && b <= '9' || 'A' <= b && b <= 'Z' || 'a' <= b && b <= 'z'
}

// findClosingParentis returns the index of the closing parenthesis ')' that matches
// an assumed opening parenthesis before the given startAt index in the string s.
// It skips parentheses that appear inside double-quoted strings and correctly handles
// escaped quotes (e.g., \" within a string literal).
func findClosingParentis(s string, startAt int) int {
	// Assumes an opening parenthesis exists somewhere before s[startAt],
	inStr, esc, count := false, false, 1

	for i := startAt; i < len(s); i++ {
		switch c := s[i]; {
		case esc:
			esc = false
		case c == '\\':
			esc = true
		case c == '"':
			inStr = !inStr
		case !inStr && c == '(':
			count++
		case !inStr && c == ')':
			count--
			if count == 0 {
				return i
			}
		}
	}
	return -1
}

func resolveTriceAlias(t *TriceFmt) {
	isAlias := slices.Contains(TriceAliases, t.Type)
	isSAlias := slices.Contains(TriceSAliases, t.Type)

	if isAlias && isSAlias {
		isSAlias = false
		// The same alias registered for trice() and triceS(). Let's analyze the last letter
		if len(t.Type) > 0 {
			last := t.Type[len(t.Type)-1]
			if last == 's' || last == 'S' {
				isAlias = false
				isSAlias = true
			}
		}
	}

	if isAlias {
		t.Alias = t.Type
		// QUESTION: What can be an easy way to map aliases to a variety of triceX_Y?
		t.Type = "trice"
	} else if isSAlias {
		t.Alias = t.Type
		t.Type = "triceS"
	}
}

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

func ProcessAliases() {
	suffix := `\b` // Word boundary after macro name

	// Core TRICE pattern (without closing \b)
	baseTricePattern := `(?i)\bTRICE(?:0|_0|Assert\w*|(?:8|16|32|64)*(?:_*[0-9SNBF]*)*)`

	// Combine static and dynamic aliases
	merged := append(TriceAliases, TriceSAliases...)

	// Escape dynamic macros and sort longest first
	var escapedExtras []string
	for _, word := range merged {
		if word != "" {
			escapedExtras = append(escapedExtras, suffix+regexp.QuoteMeta(word)+suffix)
		}
	}
	sort.SliceStable(escapedExtras, func(i, j int) bool {
		return len(escapedExtras[i]) > len(escapedExtras[j])
	})

	// Combine a full pattern with dynamic macros
	allPatterns := append([]string{baseTricePattern + suffix}, escapedExtras...)
	finalMacroPattern := `(` + strings.Join(allPatterns, `|`) + `)`

	// Compile a basic macro name matcher
	matchTypNameTRICE = regexp.MustCompile(finalMacroPattern)

	// Full TRICE call pattern (assumes patID and patFmtString exist)
	patNbTRICE := finalMacroPattern + `\s*\(` + patID + `\(\s*.*[0-9]\s*\)\s*,\s*` + patFmtString + `\s*.*\)`
	matchNbTRICE = regexp.MustCompile(patNbTRICE)

	// Partial matcher for the start of any TRICE-style or aliaS macro call
	matchAnyTriceStart = regexp.MustCompile(finalMacroPattern + `\s*\(`)
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

func triceTypeCategory(typeName string) string {
	upper := strings.ToUpper(typeName)
	if strings.Contains(upper, "ASSERT") {
		return "Assert"
	}
	for _, suffix := range []string{"S", "N", "B", "F"} {
		if strings.HasSuffix(upper, "_"+suffix) || strings.HasSuffix(upper, suffix) {
			return suffix
		}
	}
	return ""
}

func hasFormatSpecifierKind(specs []fmtspec.Spec, kind fmtspec.Kind) bool {
	for _, spec := range specs {
		if spec.Kind == kind {
			return true
		}
	}
	return false
}

func validateTriceFormatSpecifierKinds(t TriceFmt, specs []fmtspec.Spec) error {
	switch triceTypeCategory(t.Type) {
	case "S", "N":
		if len(specs) == 1 && !isTriceStringFormatSpecifier(specs[0]) {
			return fmt.Errorf("%v should use a string format specifier", t)
		}
	default:
		if hasFormatSpecifierKind(specs, fmtspec.KindString) {
			return fmt.Errorf("%v contains a string format specifier outside triceS/triceN", t)
		}
	}
	return nil
}

func isTriceStringFormatSpecifier(spec fmtspec.Spec) bool {
	if spec.Kind == fmtspec.KindString {
		return true
	}
	return spec.Kind == fmtspec.KindBasedInteger && strings.ContainsRune("qxX", rune(spec.Verb))
}

func validateRegularTriceFormatSpecifierCount(typeName string, formatSpecifierCount int) error {
	fullType, err := ConstructFullTriceInfo(typeName, formatSpecifierCount)
	if err != nil || fullType == "" {
		return fmt.Errorf("%s does not match format specifier count %d", typeName, formatSpecifierCount)
	}
	return nil
}

func isSAliasEncodedString(s string) bool {
	return strings.HasPrefix(s, SAliasStrgPrefix) && strings.HasSuffix(s, SAliasStrgSuffix)
}

// evaluateTriceParameterCount analyzes rest, if it has the correct parameter count according to t.
// rest starts immediately after the trice format string and can be very long (the remaining file context),
// but we check only until the trice end and return nil if the evaluation is ok.
func evaluateTriceParameterCount(t TriceFmt, line int, rest string) (err error) {
	specs := formatSpecifierSpecs(t.Strg)
	fsc := len(specs)
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
		return fmt.Errorf("malformed Trice parameter list after format string: %w", err)
	}
	category := triceTypeCategory(t.Type)
	switch category {
	case "S": // expect one colon followed by a string and a closing bracket
		if fsc != 1 {
			return fmt.Errorf("line %d %v should have exactly one format specifier and not %d", line, t, fsc)
		}
		if err := validateTriceFormatSpecifierKinds(t, specs); err != nil {
			return fmt.Errorf("line %d %w", line, err)
		}
		if cnt != 1 {
			return fmt.Errorf("line %d %v should have exactly one parameter and not %d", line, t, cnt)
		}
	case "N":
		if fsc != 1 {
			return fmt.Errorf("line %d %v should have exactly one format specifier and not %d", line, t, fsc)
		}
		if err := validateTriceFormatSpecifierKinds(t, specs); err != nil {
			return fmt.Errorf("line %d %w", line, err)
		}
		if cnt != 2 {
			return fmt.Errorf("line %d %v should have exactly two parameters and not %d", line, t, cnt)
		}
	case "B":
		if fsc != 1 {
			return fmt.Errorf("line %d %v should have exactly one format specifier and not %d", line, t, fsc)
		}
		if err := validateTriceFormatSpecifierKinds(t, specs); err != nil {
			return fmt.Errorf("line %d %w", line, err)
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
		if category == "Assert" { // matches triceAssert*
			if fsc != 0 || cnt < 1 {
				return fmt.Errorf("line %d %v should have no format specifiers and one or more parameters, the boolean value is need (fsc=%d, cnt=%d)", line, t, fsc, cnt)
			}
			return
		}
		if fsc > 12 || cnt > 12 {
			return fmt.Errorf("line %d %v should have no more than 12 parameters (fsc=%d, cnt=%d)", line, t, fsc, cnt)
		}
		if err := validateTriceFormatSpecifierKinds(t, specs); err != nil {
			return fmt.Errorf("line %d %w", line, err)
		}
		if err := validateRegularTriceFormatSpecifierCount(t.Type, fsc); err != nil {
			return fmt.Errorf("line %d %w", line, err)
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
				return count, errors.New("invalid string literal before closing bracket")
			}
			s = s[i:]
			s = s[loc[1]:]
			goto restart
		case '(':
			s = s[i+1:] // cut off including '('
			pos, e := matchBracketLiteral(s)
			if e != nil {
				return count, errors.New("invalid nested parentheses before closing bracket")
			}
			s = s[pos+1:] // cut off including ')'
			goto restart
		case ',':
			count++
		case ')':
			return count, nil
		}
	}
	return count, errors.New("no matching closing bracket found")
}

func formatTriceInsertParseError(sourcePath, source string, line, contextStartLine int, err error) string {
	if line < 1 {
		line = 1
	}
	from, to, context := sourceLineContext(source, contextStartLine, triceDiagnosticContextLines)
	if context == "" {
		return fmt.Sprintf("%s:%d: %v", sourcePath, line, err)
	}
	if from == to {
		return fmt.Sprintf("%s:%d: %v\n%s:%d:\n%s", sourcePath, line, err, sourcePath, from, context)
	}
	return fmt.Sprintf("%s:%d: %v\n%s:%d-%d:\n%s", sourcePath, line, err, sourcePath, from, to, context)
}

func sourceLineContext(source string, startLine, maxLines int) (from, to int, context string) {
	if source == "" || maxLines <= 0 {
		return 0, 0, ""
	}
	if startLine < 1 {
		startLine = 1
	}
	lines := strings.Split(source, "\n")
	if len(lines) > 1 && lines[len(lines)-1] == "" {
		lines = lines[:len(lines)-1]
	}
	if len(lines) == 0 {
		return 0, 0, ""
	}
	if startLine > len(lines) {
		startLine = len(lines)
	}
	endLine := startLine + maxLines - 1
	if endLine > len(lines) {
		endLine = len(lines)
	}
	width := len(strconv.Itoa(endLine))
	var b strings.Builder
	for line := startLine; line <= endLine; line++ {
		if b.Len() > 0 {
			b.WriteByte('\n')
		}
		fmt.Fprintf(&b, "%*d | %s", width, line, shortenDiagnosticLine(lines[line-1], triceDiagnosticMaxLineRunes))
	}
	return startLine, endLine, b.String()
}

func shortenDiagnosticLine(s string, maxRunes int) string {
	if maxRunes <= 0 {
		return ""
	}
	runes := []rune(s)
	if len(runes) <= maxRunes {
		return s
	}
	return string(runes[:maxRunes]) + " ..."
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
			switch DefaultStampSize {
			case 32:
				idName = "ID("
			case 16:
				idName = "Id("
			default:
				idName = "id("
			}
		}
	}
	idName += gap
	first := s[:offset+loc[2]]                      // first is the not touched s part before the replacement space.
	idSiz := loc[5] - loc[2]                        // idSiz is the size of the replaced ID space inside the source code.
	last := s[offset+loc[5]:]                       // last is the not touched s part after the replacement space.
	prefix := keptOpeningWhitespace(s, offset, loc) // prefix preserves user whitespace after the opening parenthesis.
	idIns := prefix + idName + strconv.Itoa(int(id)) + gap + "), "
	result = first + idIns + last //
	delta = len(idIns) - idSiz    // delta is the offset change.
	return
}

func keptOpeningWhitespace(s string, offset int, loc []int) string {
	start := offset + loc[2]
	end := offset + loc[5]
	if loc[3] != loc[4] {
		end = offset + loc[3]
	}
	prefix := s[start:end]
	if prefix == "" && SpaceInsideParenthesis {
		return " "
	}
	return prefix
}

// cleanID inserts id 0 into s or removes ID statement according to loc information and returns the result together with the changed len.
func cleanID(s string, offset int, loc []int, t TriceFmt) (result string, delta int) {
	// code is similar to writeiD code for clarity.
	if t.Type[2] == 'I' { // Upper case letter (s.th. like TRICE*...), we set id just to 0.
		return writeID(s, offset, loc, t, 0)
	}
	first := s[:offset+loc[2]]                     // first is the not touched s part before the replacement space.
	idSiz := loc[5] - loc[2]                       // idSiz is the size of the replaced ID space inside the source code.
	last := s[offset+loc[5]:]                      // last is the not touched s part after the replacement space.
	idIns := keptOpeningWhitespace(s, offset, loc) // replacement string
	result = first + idIns + last                  //
	delta = len(idIns) - idSiz                     // delta is the offset change.
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
