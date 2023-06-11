// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

// arrayFlag is a slice type for multi flag
type arrayFlag []string

var (
	// Srcs gets multiple files or directories.
	Srcs arrayFlag
)

// String method is the needed for interface satisfaction.
func (i *arrayFlag) String() string {
	return ""
}

// Set is a needed method for multi flags.
func (i *arrayFlag) Set(value string) error {
	*i = append(*i, value)
	return nil
}

//  // FullFilePath returns absolute file path if fn is not "off" or "none".
//  func FullFilePath(fn string) string {
//  	if fn == "none" || fn == "off" {
//  		return fn
//  	}
//  	s, err := filepath.Abs(fn)
//  	msg.InfoOnErr(err, fmt.Sprintf("failed to parse %s\n", fn))
//  	return s
//  }
//
