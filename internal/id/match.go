// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

import (
	"fmt"
	"strings"
)

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
	var clpIndex int
start:
	for {
		triceStartloc := matchAnyTriceStart.FindStringIndex(s)
		if triceStartloc == nil { // not found
			return
		}
		for {
			fmtLoc := matchStringLiteral(s)
			if fmtLoc == nil { // not found
				return
			}
			// Check, if there is a closing parenthesis after the format string
			clpIndex = strings.Index(s[fmtLoc[1]:], `)`)
			if clpIndex == -1 {
				if Verbose {
					fmt.Println("No closing parenthesis found after format string:", s)
				}
				return
			}

			if fmtLoc[1] < triceStartloc[1] {
				cut := fmtLoc[1]
				if Verbose {
					fmt.Println("The formatString ends before typeName, continue with reduced string:", s, "-->", s[cut:], "and look for next format string")
				}
				s = s[cut:]
				triceStartloc[0] -= cut
				triceStartloc[1] -= cut
				offset += cut
				continue
			}

			if fmtLoc[0] < triceStartloc[0] { //
				cut := fmtLoc[1]
				if Verbose {
					fmt.Println("The typeName is inside format string, start over with reduced string:", s, "-->", s[cut:], "and look for next trice start")
				}
				s = s[cut:]
				triceStartloc[0] -= cut
				triceStartloc[1] -= cut
				offset += cut
				goto start
			}

			// formatString starts after typeName (normal case)
			typeNameLoc := matchTypNameTRICE.FindStringIndex(s[triceStartloc[0]:triceStartloc[1]])
			// now we have: triceStartloc[0], triceStartloc[0]+typeNameLoc[1], triceStartloc[1]:
			//              TR________________ice                              (
			rest := s[triceStartloc[1]:fmtLoc[0]]
			idLoc := matchNbID.FindStringIndex(rest)
			if idLoc == nil { // no ID statement
				clpIndex = strings.Index(rest, `)`)
				// - if `)` is located before format string starts, discard trice
				// - ExampleY: trice(tid, fmt, ...) ... "y"   ... (y)
				//  -          0   12 00                5 6
				//  -                            `)`                       -> discard
				if clpIndex != -1 { // a closing parenthesis was found before format string
					return
				}
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
