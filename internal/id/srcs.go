// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

// ArrayFlag is a slice type for multi flag
type ArrayFlag []string

// String method is the needed for interface satisfaction.
func (i *ArrayFlag) String() string {
	return ""
}

// Set is a needed method for multi flags.
func (i *ArrayFlag) Set(value string) error {
	*i = append(*i, value)
	return nil
}
