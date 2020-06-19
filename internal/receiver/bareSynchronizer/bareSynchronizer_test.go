// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package bareSynchronizer_test is a blackbox test.
package bareSynchronizer_test

import (
	"fmt"
	"testing"

	"github.com/rokath/trice/internal/receiver"
	"github.com/rokath/trice/internal/receiver/bareSynchronizer"
	"github.com/rokath/trice/pkg/inputDummy"
	"github.com/rokath/trice/pkg/lib"
)

// check if read & sync works
func Test1(t *testing.T) {
	i := []byte{1, 2, 3, 10, 172, 0, 1, 0x16, 0x16, 0x16, 0x16, 22, 44, 33, 11}
	s := inputDummy.New(i, 0)
	bs := bareSynchronizer.New(s)
	receiver.DiscardByte = func(b byte) { fmt.Println(b) }
	act := make([]byte, 4)

	bs.Read(act)
	exp := []byte{1, 2, 3, 10}
	//exp := []byte{10, 172, 0, 1} // todo bareSync
	lib.Equals(t, act, exp)

	bs.Read(act)
	exp = []byte{172, 0, 1, 0x16}
	//exp := []byte{22, 44, 33, 11}  // todo bareSync
	lib.Equals(t, act, exp)
}
