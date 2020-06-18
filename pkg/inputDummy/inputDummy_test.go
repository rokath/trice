// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package inputDummy_test is a blsckbox test.
package inputDummy_test

import (
	"io"
	"testing"

	"github.com/rokath/trice/pkg/inputDummy"
	"github.com/rokath/trice/pkg/lib"
)

func Test1(t *testing.T) {
	i := []byte{'g', 'a', 'r', 'b', 'a', 'g', 'e', 235, 96, 96, 0, 10, 172, 0, 0}
	s := inputDummy.New(i, 0)
	o := make([]byte, 7)
	r := []byte{'g', 'a', 'r', 'b', 'a', 'g', 'e'}
	s.Read(o)
	lib.Equals(t, o, r)
}

// Test2 checks if a shorter internal buffer repeats
func Test2(t *testing.T) {
	i := []byte{'A'}
	s := inputDummy.New(i, 0)
	o := make([]byte, 3)
	r := []byte{'A', 'A', 'A'}
	s.Read(o)
	lib.Equals(t, o, r)
}

// Test 3 checks if a longer internal buffer repeats
func Test3(t *testing.T) {
	i := []byte{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}
	s := inputDummy.New(i, 0)
	o := make([]byte, 7)
	exp := []byte{7, 8, 9, 10, 0, 1, 2}
	s.Read(o)
	s.Read(o)
	lib.Equals(t, exp, o)
}

// Test4 checks if a zero internal buffer works
func Test4(t *testing.T) {
	i := []byte{}
	s := inputDummy.New(i, 0)
	o := make([]byte, 3)
	n, err := s.Read(o)
	lib.Equals(t, n, 0)
	lib.Equals(t, err, io.EOF)
}

// Test5 checks if a nil internal buffer works
func Test5(t *testing.T) {
	s := inputDummy.New(nil, 0)
	o := make([]byte, 3)
	n, err := s.Read(o)
	lib.Equals(t, n, 0)
	lib.Equals(t, err, io.EOF)
}

// Test6 checks if a zero external buffer works with internal nil
func Test6(t *testing.T) {
	s := inputDummy.New(nil, 0)
	o := []byte{}
	n, err := s.Read(o)
	lib.Equals(t, n, 0)
	lib.Equals(t, err, io.EOF)
}

// Test7 checks if a zero external buffer works with internal zero
func Test7(t *testing.T) {
	var i []byte
	s := inputDummy.New(i, 0)
	o := []byte{}
	n, err := s.Read(o)
	lib.Equals(t, n, 0)
	lib.Equals(t, err, io.EOF)
}

// Test8 checks if a zero external buffer works with internal non-zero
func Test8(t *testing.T) {
	i := []byte{'g', 'a', 'r', 'b', 'a', 'g', 'e'}
	s := inputDummy.New(i, 0)
	var o []byte
	n, err := s.Read(o)
	lib.Equals(t, n, 0)
	lib.Equals(t, err, nil)
}
