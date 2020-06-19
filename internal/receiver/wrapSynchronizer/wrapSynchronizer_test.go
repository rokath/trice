// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package wrapSynchronizer_test is a blackbox test.
package wrapSynchronizer_test

import (
	"testing"

	"github.com/rokath/trice/internal/receiver"
	"github.com/rokath/trice/internal/receiver/wrapSynchronizer"
	"github.com/rokath/trice/pkg/inputDummy"
	"github.com/rokath/trice/pkg/lib"
)

// check if read & sync works
func Test1(t *testing.T) {
	i := []byte{'g', 'a', 'r', 'b', 'a', 'g', 'e', 235, 96, 96, 235 ^ 10 ^ 172, 10, 172, 0, 0}
	s := inputDummy.New(i, 0)
	ws := wrapSynchronizer.New(s)
	receiver.DiscardByte = receiver.DiscardSilent
	b := make([]byte, 8)
	ws.Read(b)
	c := []byte{235, 96, 96, 235 ^ 10 ^ 172, 10, 172, 0, 0}
	lib.Equals(t, b, c)
}
