// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package baresynchronizer_test is a blackbox test.
package baresynchronizer_test

import (
	"errors"
	"fmt"
	"testing"

	"github.com/rokath/trice/internal/receiver"
	"github.com/rokath/trice/internal/receiver/baresynchronizer"
	"github.com/rokath/trice/pkg/inputdummy"
	"github.com/rokath/trice/pkg/lib"
)

const endless = 0

// TestRead checks if read works.
func TestRead(t *testing.T) {
	i := []byte{1, 2, 3, 10, 172, 0, 1, 0x16, 0x16, 0x16, 0x16, 22, 44, 33, 11}
	s := inputdummy.New(i, 0, endless)
	bs := baresynchronizer.New(s)
	receiver.DiscardByte = func(b byte) { fmt.Println(b) }
	act := make([]byte, 4)

	bs.Read(act)
	exp := []byte{1, 2, 3, 10}
	//exp := []byte{10, 172, 0, 1} // todo bareSync
	lib.Equals(t, exp, act)

	bs.Read(act)
	exp = []byte{172, 0, 1, 0x16}
	//exp := []byte{22, 44, 33, 11}  // todo bareSync
	lib.Equals(t, exp, act)
}

// TestTooShort checks if read reacts right on too small input buffer.
func TestTooShort(t *testing.T) {
	i := []byte{1, 2, 3, 10, 172, 0, 1, 0x16, 0x16, 0x16, 0x16, 22, 44, 33, 11}
	s := inputdummy.New(i, 0, endless)
	bs := baresynchronizer.New(s)
	receiver.DiscardByte = func(b byte) { fmt.Println(b) }
	act := make([]byte, 3)

	n, err := bs.Read(act)
	exp := make([]byte, 3)
	//exp := []byte{10, 172, 0, 1} // todo bareSync
	lib.Equals(t, exp, act)
	lib.Equals(t, 0, n)
	lib.Equals(t, errors.New("too short for bare"), err)
}

// TestEOF checks if read reacts right on too small input buffer.
func TestEOF(t *testing.T) {
	i := []byte{1, 2, 3, 10, 172, 0, 1, 0x16, 0x16, 0x16, 0x16, 22, 44, 33, 11}
	s := inputdummy.New(i, 0, 2)
	bs := baresynchronizer.New(s)
	receiver.DiscardByte = func(b byte) { fmt.Println(b) }
	b := make([]byte, 4)
	n, err := bs.Read(b)
	b = b[:n]
	lib.Equals(t, 2, n)
	lib.Equals(t, errors.New("unexpected EOF"), err)
	lib.Equals(t, []byte{0, 0}, b)
}

/*
// TestSync checks if sync works.
func TestSync(t *testing.T) {
	i := []byte{1, 2, 3, 10, 172, 0, 1, 0x16, 0x16, 0x16, 0x16, 22, 44, 33, 11}
	s := inputdummy.New(i, 0, endless)
	bs := baresynchronizer.New(s)
	receiver.DiscardByte = func(b byte) { fmt.Println(b) }
	act := make([]byte, 4)

	bs.Read(act)
	//exp := []byte{1, 2, 3, 10}
	exp := []byte{10, 172, 0, 1} // todo bareSync
	lib.Equals(t,  exp, act)

	bs.Read(act)
	//exp = []byte{172, 0, 1, 0x16}
	exp = []byte{22, 44, 33, 11}  // todo bareSync
	lib.Equals(t,  exp, act)
}
*/
