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
func matchTrice(s string) (loc []int) {
	var offset int
start:
	for {
		triceStartloc := matchAnyTriceStart.FindStringIndex(s)
		if triceStartloc == nil { // not found
			return
		}
		for {
			fmtLoc := matchFormatString(s)
			if fmtLoc == nil { // not found
				return
			}

			if fmtLoc[1] < triceStartloc[1] { // formatString ends before typeName, continue with reduced string
				cut := fmtLoc[1]
				s = s[cut:]
				triceStartloc[0] -= cut
				triceStartloc[1] -= cut
				offset += cut
				continue // look for next format string
			}

			if fmtLoc[0] < triceStartloc[0] { // typeName is inside format string, start over with reduced string
				cut := fmtLoc[1]
				s = s[cut:]
				triceStartloc[0] -= cut
				triceStartloc[1] -= cut
				offset += cut
				goto start // look for next trice start
			}

			// formatString starts after typeName (normal case)
			typeNameLoc := matchTypNameTRICE.FindStringIndex(s[triceStartloc[0]:triceStartloc[1]])
			// now we have: triceStartloc[0], triceStartloc[0]+typeNameLoc[1], triceStartloc[1]:
			//              TR________________ice                              (
			rest := s[triceStartloc[1]:fmtLoc[0]]
			idLoc := matchNbID.FindStringIndex(rest)
			if idLoc == nil { // no ID statement
				loc = append(loc, triceStartloc[0], triceStartloc[0]+typeNameLoc[1], triceStartloc[1], 0, 0, fmtLoc[0], fmtLoc[1])
			} else {
				loc = append(loc, triceStartloc[0], triceStartloc[0]+typeNameLoc[1], triceStartloc[1], triceStartloc[1]+idLoc[0], triceStartloc[1]+idLoc[1], fmtLoc[0], fmtLoc[1])
			}
			if offset != 0 {
				for i := range loc {
					loc[i] = loc[i] + offset
				}
			}
			return
		}
	}
}
