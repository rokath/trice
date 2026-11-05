// SPDX-License-Identifier: MIT

package id

// source tree management

import (
	"os"
	"strings"

	"github.com/rokath/trice/internal/fmtspec"
)

const (

	// patSourceFile is a regex pattern matching any source file for patching (https://stackoverflow.com/questions/1545080/c-code-file-extension-what-is-the-difference-between-cc-and-cpp)
	patSourceFile = "(\\.c|\\.cc|\\.cp|\\.cxx|\\.cpp|\\.CPP|\\.c\\+\\+|\\.C|\\.h|\\.hh|\\.hp|\\.hxx|\\.hpp|\\.HPP|\\.h\\+\\+|\\.H|\\.ixx|\\.inc)$"

	// patNbID is a regex pattern matching any first "Id(n)" and usable in matches after patTypNameTRICE. It works also over line breaks.
	patNbID = `(?Ui)\b(i)(D)\b\s*\(\s*\d+\s*\)` // https://regex101.com/r/2BlNSv/1

	// patFmtString is a regex matching the first format string inside trice even containing newlines and \"
	patFmtString = `"(?s)(.*)"` // https://stackoverflow.com/questions/159118/how-do-i-match-any-character-across-multiple-lines-in-a-regular-expression

	patID = `\s*\b(i|I)(d|D)\b\s*` // `\s*\b(I|i)d\b\s*`

	patNb = `\d+` // // `[0-9]*`

	// patSpacesWithOptionalComma matches trailing spaces and optional comma after trice ID: trice(ID(111)>>  ,   <<"%format")
	patSpacesWithOptionalComma = `^\s*,?\s*`
)

// formatSpecifierCount parses s for format specifier and returns the found count.
func formatSpecifierCount(s string) (count int) {
	// Keep source scanning aligned with decoder-side normalization. This shared
	// parser was added after Issue #649 because valid C modifiers such as %zu
	// were not counted here and later reconstructed as TRICE_0 during decoding.
	return len(formatSpecifierSpecs(s))
}

func formatSpecifierSpecs(s string) []fmtspec.Spec {
	if strings.ContainsAny(s, "{}") && !isSAliasEncodedString(s) {
		if template, err := fmtspec.ParseTemplate(s, nil); err == nil {
			return template.Specs
		}
	}
	_, specs := fmtspec.Normalize(s)
	return specs
}

func isSourceFile(fi os.FileInfo) bool {
	return matchSourceFile.MatchString(fi.Name())
}
