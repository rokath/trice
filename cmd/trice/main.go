// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package main

import (
	"math/rand"
	"os"
	"time"

	"github.com/rokath/trice/internal/global"
)

var (
	// do not initialize, goreleaser will handle that
	version string

	// do not initialize, goreleaser will handle that
	commit string

	// do not initialize, goreleaser will handle that
	date string

	// linkTime ...
	linkTime = "testTime"
)

func main() {
	fi, err := os.Stat(os.Args[0])
	if nil == err {
		linkTime = fi.ModTime().String()
	} else {
		linkTime = "unknown" // on running main tests fileinfo is invalid
	}
	rand.Seed(time.Now().UnixNano())
	err = HandleArgs(os.Args)
	if nil != err {
		global.OsExit(1)
	}
	global.OsExit(0)
}
