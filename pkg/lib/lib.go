// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// helper functions for trice tool
package lib

import (
	"fmt"
	"path/filepath"
)

// ArrayFlag is a slice type for multi flag
type ArrayFlag []string

var (
	Srcs ArrayFlag // gets multiple files or directories
)

func (i *ArrayFlag) String() string {
	return "my string representation"
}

func (i *ArrayFlag) Set(value string) error {
	*i = append(*i, value)
	return nil
}

func Assign(fn string) string {
	if "none" == fn {
		return fn
	}
	s, err := filepath.Abs(fn)
	if nil != err {
		_ = fmt.Errorf("failed to parse %s: %v", fn, err)
	}
	return s
}
