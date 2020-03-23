// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// All rights reserved.
// Use of this source code is governed by a
// license that can be found in the LICENSE file.

package main

import (
	"os"
	"testing"

	"github.com/rokath/trice/pkg/emit"
)

func TestMain(m *testing.M) {
	// do stuff before tests
	emit.Tee = os.Stdout
	exitVal := m.Run()
	// do stuff after tests
	os.Exit(exitVal)
}
