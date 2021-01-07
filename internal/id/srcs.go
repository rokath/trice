// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

import (
	"fmt"
	"path/filepath"

	"github.com/rokath/trice/pkg/msg"
)

// ArrayFlag is a slice type for multi flag
type ArrayFlag []string

var (
	// Srcs gets multiple files or directories.
	Srcs ArrayFlag
)

// String method is the needed for interface satisfaction.
func (i *ArrayFlag) String() string {
	return ""
}

// Set is a needed method for multi flags.
func (i *ArrayFlag) Set(value string) error {
	*i = append(*i, value)
	return nil
}

// ConditionalFilePath returns absolute file path if fn is not "off" or "none".
func ConditionalFilePath(fn string) string {
	if "none" == fn || "off" == fn {
		return fn
	}
	s, err := filepath.Abs(fn)
	msg.InfoOnErr(fmt.Sprintf("failed to parse %s\n", fn), err)
	return s
}
