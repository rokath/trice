// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package main

import (
	"os"
	"testing"
)

func TestMain(m *testing.M) {
	// do stuff before tests
	exitVal := m.Run()
	// do stuff after tests
	os.Exit(exitVal)
}
