// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package main

import (
	"math/rand"
	"os"
	"time"
)

var ( // do not initialize these, goreleaser will handle that
	version string
	commit  string
	date    string
)

func main() {
	rand.Seed(time.Now().UnixNano())
	wd, _ := os.Getwd() // for usage
	wd += "/"
	err := HandleArgs(wd, os.Args)
	if nil != err {
		os.Exit(1)
	}
	os.Exit(0)
}
