// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package lib contains helper functions for trice tool
package lib

import (
	"fmt"
	"path/filepath"
	"time"
)

// ArrayFlag is a slice type for multi flag
type ArrayFlag []string

var (
	Srcs ArrayFlag // gets multiple files or directories

	// TimeStampFormat is the PC timestamp format
	TimeStampFormat = "off"
)

func (i *ArrayFlag) String() string {
	return "my string representation"
}

func (i *ArrayFlag) Set(value string) error {
	*i = append(*i, value)
	return nil
}

func Assign(fn string) string {
	if "none" == fn || "off" == fn {
		return fn
	}
	s, err := filepath.Abs(fn)
	if nil != err {
		_ = fmt.Errorf("failed to parse %s: %v", fn, err)
	}
	return s
}

// Timestamp returns local time as string according var TimeStampFormat
// https://www.alexedwards.net/blog/an-overview-of-go-tooling#managing-dependencies
func Timestamp() string {
	var s string
	switch TimeStampFormat {
	case "LOCmicro":
		s = time.Now().Format(time.StampMicro) + "  "
	case "UTCmicro":
		s = "UTC " + time.Now().UTC().Format(time.StampMicro) + "  "
	case "off", "none":
		s = ""
	}
	return s
}
