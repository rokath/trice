// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

import (
	"fmt"
	"path/filepath"
)

// ArrayFlag is a slice type for multi flag
type ArrayFlag []string

var (
	// Srcs gets multiple files or directories.
	Srcs ArrayFlag

	// TimeStampFormat is the PC timestamp format.
	TimeStampFormat = "off"
)

// String is a needed method for some reason.
func (i *ArrayFlag) String() string {
	return "my string representation"
}

// Set is a needed method for multi flags.
func (i *ArrayFlag) Set(value string) error {
	*i = append(*i, value)
	return nil
}

// ConditinalFilePath returns absolute file path if fn is not "off" or "none".
func ConditinalFilePath(fn string) string {
	if "none" == fn || "off" == fn {
		return fn
	}
	s, err := filepath.Abs(fn)
	if nil != err {
		_ = fmt.Errorf("failed to parse %s: %v", fn, err)
	}
	return s
}
