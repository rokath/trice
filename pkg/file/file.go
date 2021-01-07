// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package file helps with file handling.
package file

import (
	"io/ioutil"

	"github.com/rokath/trice/pkg/msg"
)

// ReadString returns filename content as string
func ReadString(filename string) (s string) {
	b, err := ioutil.ReadFile(filename)
	msg.FatalErr(err)
	s = string(b)
	return
}

// Random creates a random file containing s and returns its name.
// See ioutil.Tempfile() for dir and pattern.
func Random(s, dir, pattern string) string {
	fd, err := ioutil.TempFile(dir, pattern)
	msg.FatalErr(err)
	_, err = fd.WriteString(s)
	msg.OnErr(err)
	fn := fd.Name()
	msg.OnErr(fd.Close())
	return fn
}
