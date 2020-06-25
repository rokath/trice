// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package main

import (
	"math/rand"
	"os"
	"time"
)

var (
	version  string // do not initialize, goreleaser will handle that
	commit   string // do not initialize, goreleaser will handle that
	date     string // do not initialize, goreleaser will handle that
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
		os.Exit(1)
	}
	os.Exit(0)
}
