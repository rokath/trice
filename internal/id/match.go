// SPDX-License-Identifier: MIT

package id

// triceMatchIssue records a recognized macro start that the shared parser cannot resolve.
type triceMatchIssue struct {
	offset  int
	message string
}

// matchTrice searches in s for the next trice statement. If not found loc is nil.
// When found, s[loc[0]:loc[1]] is the typeName and at s[loc[2] is the opening parenthesis behind the typeName.
// If the found trice statement contains an ID statement, it is s[loc[3]:loc[4]]. Otherwise is loc[3]==loc[4].
// The associated format string is s[loc[5]:loc[6]].
//
// Examples:
// - TRice( "a" )
// - 0   12 5 6
//
// - TRice   ("a" )
// - 0   1   25 6
//
// - TRice ( iD(999) ,  "a" )
// - 0   1 2 3     4    5 6
//
// - TRice(example,example); "string" )
// -                     `)`
// - nil
// - TRice(id,string); "string" )
// -               `)`
// - nil
func matchTrice(s string) (loc []int) {
	loc, _ = matchTriceWithIssues(s)
	return
}

// matchTriceWithIssues exposes skipped recognized starts without implementing a second parser.
func matchTriceWithIssues(s string) (loc []int, issues []triceMatchIssue) {
	var offset int
	var clpIndex = -1

	for {
		triceStartloc := matchAnyTriceStart.FindStringIndex(s)
		if triceStartloc == nil { // not found
			return
		}

		typeNameLoc := matchTypNameTRICE.FindStringIndex(s[triceStartloc[0]:triceStartloc[1]])
		var trice TriceFmt
		trice.Type = s[triceStartloc[0] : triceStartloc[0]+typeNameLoc[1]]
		resolveTriceAlias(&trice)
		clpIndex = findClosingParentis(s, triceStartloc[1])
		if clpIndex == -1 {
			issues = append(issues, triceMatchIssue{offset: offset + triceStartloc[0], message: "recognized Trice call has no matching closing parenthesis"})
			cut := triceStartloc[1]
			offset += cut
			s = s[cut:]
			continue
		}

		args := s[triceStartloc[1]:clpIndex]
		fmtLoc := matchStringLiteral(args)

		if fmtLoc == nil { // not found
			if trice.isAlias() {
				// Custom macros are flexible and might not have a format string: MyAssert(2>2)
				// At this point, to ensure proper down-the-code matching, let's use the clp (closing parenthesis)
				// and call its position (location) as an empty format location (zero-length format string)
				fmtLoc = []int{clpIndex - triceStartloc[1], clpIndex - triceStartloc[1]}
			} else {
				issues = append(issues, triceMatchIssue{offset: offset + triceStartloc[0], message: "recognized Trice call has no supported format string"})
				cut := triceStartloc[1]
				offset += cut
				s = s[cut:]
				continue
			}
		}

		//typeNameLoc := matchTypNameTRICE.FindStringIndex(s[triceStartloc[0]:triceStartloc[1]])
		// formatString starts after typeName (normal case)
		// now we have: triceStartloc[0], triceStartloc[0]+typeNameLoc[1], triceStartloc[1]:
		//              TR________________ice                              (
		rest := args[:fmtLoc[0]]
		idLoc := matchNbID.FindStringIndex(rest)

		actualFormatStart := fmtLoc[0]
		// Calculate the leftmost position of the first semantic argument, skipping
		// trailing spaces and the comma after ID when present.
		fmtLoc[0] = 0
		if idLoc != nil {
			fmtLoc[0] += idLoc[1]
		}

		skipSpacesBeforeFmtLoc := matchSpacesWithOptionalComma.FindStringIndex(args[fmtLoc[0]:])
		if skipSpacesBeforeFmtLoc != nil {
			fmtLoc[0] += skipSpacesBeforeFmtLoc[1]
		}
		if !trice.isAlias() && actualFormatStart > fmtLoc[0] {
			prefix := args[fmtLoc[0]:actualFormatStart]
			if isTriceFormatContinuationWhitespace(prefix) {
				// A physical line continuation is still leading whitespace, so the
				// shared insert parser should expose the actual literal boundary.
				fmtLoc[0] = actualFormatStart
			} else {
				issues = append(issues, triceMatchIssue{offset: offset + triceStartloc[0], message: "Trice site uses an unsupported composite or non-leading format string"})
			}
		}

		if idLoc == nil { // no ids
			idLoc = []int{0, 0}
		} else {
			idLoc = []int{triceStartloc[1] + idLoc[0], triceStartloc[1] + idLoc[1]}
		}

		// For custom macros, the format string isn't always the first arg after Trice ID
		// (assert macros put the condition first). So loc[5] tracks where the actual
		// first arg starts, not where the format string is
		loc = append(loc,
			triceStartloc[0], triceStartloc[0]+typeNameLoc[1], triceStartloc[1], idLoc[0], idLoc[1], fmtLoc[0]+triceStartloc[1], fmtLoc[1]+triceStartloc[1])

		if offset != 0 {
			for i := range loc {
				loc[i] = loc[i] + offset
			}
		}
		return
	}
}

// isTriceFormatContinuationWhitespace accepts only whitespace and physical C line continuations.
func isTriceFormatContinuationWhitespace(s string) bool {
	for index := 0; index < len(s); index++ {
		switch s[index] {
		case ' ', '\t', '\n', '\r', '\f', '\v':
			continue
		case '\\':
			if index+1 < len(s) && s[index+1] == '\n' {
				index++
				continue
			}
			if index+2 < len(s) && s[index+1] == '\r' && s[index+2] == '\n' {
				index += 2
				continue
			}
			return false
		default:
			return false
		}
	}
	return true
}
