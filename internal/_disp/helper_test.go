// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// blackbox test
package disp_test

import "testing"

// test helper ///////////////////////////////////////////////////////////////////////
//

// uniqueString removes string duplicates in string slice
// https://www.golangprograms.com/remove-duplicate-values-from-slice.html
func uniqueString(ss []string) []string {
	keys := make(map[string]bool)
	//list := []string{}
	var list []string
	for _, entry := range ss {
		if _, value := keys[entry]; !value {
			keys[entry] = true
			list = append(list, entry)
		}
	}
	return list
}

// uniqLines reads inFile, removes duplicate lines and writes to outFile
// inFile and outFile can be the same
// it returns count of remaining lines plus 1 (io.EOF ?)
func uniqLines(t *testing.T, inFile, outFile string) int {
	il, err := ReadLines(inFile)
	Ok(t, err)
	ol := uniqueString(il)
	err = WriteLines(outFile, ol)
	Ok(t, err)
	return len(ol)
}

//

// test helper ///////////////////////////////////////////////////////////////////////
