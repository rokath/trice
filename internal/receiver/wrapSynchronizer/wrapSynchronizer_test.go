// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package wrapsynchronizer_test is a blackbox test.
package wrapsynchronizer_test

import (
	"errors"
	"fmt"
	"testing"

	"github.com/rokath/trice/internal/receiver"
	"github.com/rokath/trice/internal/receiver/wrapsynchronizer"
	"github.com/rokath/trice/pkg/inputdummy"
	"github.com/rokath/trice/pkg/lib"
)

const endless = 0

// TestInSync checks if read works
func TestInSync(t *testing.T) {
	i := []byte{235, 96, 96, 235 ^ 10 ^ 172, 10, 172, 0, 0}
	s := inputdummy.New(i, inputdummy.NoDelay, endless)
	ws := wrapsynchronizer.New(s)
	receiver.DiscardByte = receiver.DiscardSilent
	act := make([]byte, 8)
	ws.Read(act)
	exp := []byte{235, 96, 96, 235 ^ 10 ^ 172, 10, 172, 0, 0}
	lib.Equals(t, exp, act)
}

// TestSync checks if read & sync works
func TestSync(t *testing.T) {
	i := []byte{'g', 'a', 'r', 'b', 'a', 'g', 'e', 235, 96, 96, 235 ^ 10 ^ 172, 10, 172, 0, 0}
	s := inputdummy.New(i, inputdummy.NoDelay, endless)
	ws := wrapsynchronizer.New(s)
	receiver.DiscardByte = receiver.DiscardSilent
	act := make([]byte, 8)
	ws.Read(act)
	exp := []byte{235, 96, 96, 235 ^ 10 ^ 172, 10, 172, 0, 0}
	lib.Equals(t, exp, act)
}

// TestTooShort checks if read & sync works
func TestTooShort(t *testing.T) {
	i := []byte{'g', 'a', 'r', 'b', 'a', 'g', 'e', 235, 96, 96, 235 ^ 10 ^ 172, 10, 172, 0, 0}
	s := inputdummy.New(i, inputdummy.NoDelay, endless)
	ws := wrapsynchronizer.New(s)
	receiver.DiscardByte = receiver.DiscardSilent
	b := make([]byte, 6)
	n, err := ws.Read(b)
	lib.Equals(t, 0, n)
	lib.Equals(t, errors.New("too short for wrap"), err)
}

// TestEOF checks if read reacts right on too small input buffer.
func TestEOF(t *testing.T) {
	i := []byte{'g', 'a', 'r', 'b', 'a', 'g', 'e', 235, 96, 96, 235 ^ 10 ^ 172, 10, 172, 0, 0}
	s := inputdummy.New(i, inputdummy.NoDelay, 2)
	bs := wrapsynchronizer.New(s)
	receiver.DiscardByte = func(b byte) { fmt.Println(b) }
	b := make([]byte, 8)
	n, err := bs.Read(b)
	b = b[:n]
	lib.Equals(t, 2, n)
	lib.Equals(t, errors.New("EOF"), err)
	lib.Equals(t, []byte{'g', 'a'}, b)
}

// TestEOF checks if read reacts right on EOF when out of sync.
func TestOutOfSyncEOF(t *testing.T) {
	i := []byte{'g', 'a', 'r', 'b', 'a', 'g', 'e', 235, 96, 96, 235 ^ 10 ^ 172, 10, 172, 0, 0}
	s := inputdummy.New(i, inputdummy.NoDelay, 11)
	bs := wrapsynchronizer.New(s)
	receiver.DiscardByte = func(b byte) { fmt.Println(b) }
	b := make([]byte, 8)
	n, err := bs.Read(b)
	lib.Equals(t, 0, n)
	lib.Equals(t, errors.New("EOF"), err)
}
