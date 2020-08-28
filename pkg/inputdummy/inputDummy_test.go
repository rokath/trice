// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package inputdummy_test is a blsckbox test.
package inputdummy_test

var ()

/*
// Test1 checks if a first read works
func Test1(t *testing.T) {
	i := []byte{'g', 'a', 'r', 'b', 'a', 'g', 'e', 235, 96, 96, 0, 10, 172, 0, 0}
	s := inputdummy.New(i, inputdummy.NoDelay, inputdummy.NoLimit)
	o := make([]byte, 7)
	r := []byte{'g', 'a', 'r', 'b', 'a', 'g', 'e'}
	s.Read(o)
	notEqualFail(t, o, r)
}

// Test2 checks if a shorter internal buffer repeats
func Test2(t *testing.T) {
	i := []byte{'A'}
	s := inputdummy.New(i, inputdummy.NoDelay, inputdummy.NoLimit)
	o := make([]byte, 3)
	r := []byte{'A', 'A', 'A'}
	s.Read(o)
	notEqualFail(t, o, r)
}

// Test 3 checks if a longer internal buffer repeats
func Test3(t *testing.T) {
	i := []byte{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}
	s := inputdummy.New(i, inputdummy.NoDelay, inputdummy.NoLimit)
	o := make([]byte, 7)
	exp := []byte{7, 8, 9, 10, 0, 1, 2}
	s.Read(o)
	s.Read(o)
	notEqualFail(t, exp, o)
}

// Test4 checks if a zero internal buffer works
func Test4(t *testing.T) {
	i := []byte{}
	s := inputdummy.New(i, inputdummy.NoDelay, inputdummy.NoLimit)
	o := make([]byte, 3)
	n, err := s.Read(o)
	notEqualFail(t, n, 0)
	notEqualFail(t, err, io.EOF)
}

// Test5 checks if a nil internal buffer works
func Test5(t *testing.T) {
	s := inputdummy.New(nil, inputdummy.NoDelay, inputdummy.NoLimit)
	o := make([]byte, 3)
	n, err := s.Read(o)
	notEqualFail(t, n, 0)
	notEqualFail(t, err, io.EOF)
}

// Test6 checks if a zero external buffer works with internal nil
func Test6(t *testing.T) {
	s := inputdummy.New(nil, inputdummy.NoDelay, inputdummy.NoLimit)
	o := []byte{}
	n, err := s.Read(o)
	notEqualFail(t, n, 0)
	notEqualFail(t, err, io.EOF)
}

// Test7 checks if a zero external buffer works with internal zero
func Test7(t *testing.T) {
	var i []byte
	s := inputdummy.New(i, inputdummy.NoDelay, inputdummy.NoLimit)
	o := []byte{}
	n, err := s.Read(o)
	notEqualFail(t, n, 0)
	notEqualFail(t, err, io.EOF)
}

// Test8 checks if a zero external buffer works with internal non-zero
func Test8(t *testing.T) {
	i := []byte{'g', 'a', 'r', 'b', 'a', 'g', 'e'}
	s := inputdummy.New(i, inputdummy.NoDelay, inputdummy.NoLimit)
	var o []byte
	n, err := s.Read(o)
	notEqualFail(t, n, 0)
	notEqualFail(t, err, nil)
}

// TestLimit checks if a zero external buffer works with internal non-zero
func TestLimit(t *testing.T) {
	i := []byte{1, 2, 3, 4}
	s := inputdummy.New(i, inputdummy.NoDelay, 6)
	o := make([]byte, 8)
	n, err := s.Read(o)
	notEqualFail(t, byte(2), o[5])
	notEqualFail(t, 6, n)
	notEqualFail(t, io.EOF, err)
}
*/
/* works but ReadString not needed now
// Test0 checks if a first read works
func Test0(t *testing.T) {
	i := []byte{'g', 'a', 'r', 'b', 'a', 'g', 'e', '\n', 96, 96, 0, 10, 172, 0, 0}
	s := inputdummy.New(i, inputdummy.NoDelay, inputdummy.NoLimit)

	act, err := s.ReadString('\n')
	notEqualFail(t, "garbage\n", act)
	notEqualFail(t, nil, err)
}

// Test01 checks if a first read works
func Test01(t *testing.T) {
	i := []byte{'g', 'a', 'r', 'b', 'a', 'g', 'e', '\n', 96, 96, 0, 10, 172, 0, 0}
	s := inputdummy.New(i, inputdummy.NoDelay, 3)

	act, err := s.ReadString('\n')
	notEqualFail(t, "gar", act)
	notEqualFail(t, io.EOF, err)
}
*/
