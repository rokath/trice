// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

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
	var offset int
	var clpIndex int = -1

	for {
		triceStartloc := matchAnyTriceStart.FindStringIndex(s)
		if triceStartloc == nil { // not found
			return
		}

		typeNameLoc := matchTypNameTRICE.FindStringIndex(s[triceStartloc[0]:triceStartloc[1]])
		clpIndex = findClosingParentis(s, triceStartloc[1])
		if clpIndex == -1 {
			// Skip this occurrence
			offset += triceStartloc[1]
			cut := triceStartloc[1]
			s = s[cut:]
			continue
		}

		args := s[triceStartloc[1]:clpIndex]
		fmtLoc := matchStringLiteral(args)

		if fmtLoc == nil { // not found
			var t TriceFmt
			t.Type = s[triceStartloc[0] : triceStartloc[0]+typeNameLoc[1]]
			resolveTriceAlias(&t)

			if t.isAlias() {
				// Custom macros are flexible and might not have a format string: MyAssert(2>2)
				// At this point, to ensure proper down-the-code matching, let's use the clp (closing parenthesis)
				// and call its position (location) as an empty format location (zero-length format string)
				fmtLoc = []int{clpIndex - triceStartloc[1], clpIndex - triceStartloc[1]}
			} else {
				// if it is a built-in macro

				// Skip this occurrence
				cut := triceStartloc[1]
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

		// Calculate the leftmost position of the format string, skipping trailing spaces
		// and the comma after ID (if present)
		fmtLoc[0] = 0
		if idLoc != nil {
			fmtLoc[0] += idLoc[1]
		}

		skipSpacesBeforeFmtLoc := matchSpacesWithOptionalComma.FindStringIndex(args[fmtLoc[0]:])
		if skipSpacesBeforeFmtLoc != nil {
			fmtLoc[0] += skipSpacesBeforeFmtLoc[1]
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
