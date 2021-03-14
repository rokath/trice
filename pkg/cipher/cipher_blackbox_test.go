// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// blackbox test
package cipher_test

import (
	"testing"

	"github.com/rokath/trice/pkg/cipher"
	"github.com/rokath/trice/pkg/tst"
)

type testTable struct {
	passWord string
	pattern  []byte
}

func Test12345678(t *testing.T) {
	list := []testTable{
		{"", []byte{1, 2, 3, 4, 5, 6, 7, 8}},
		{"XYZ", []byte{0x32, 0xa8, 0x10, 0x11, 0x93, 0x8e, 0x64, 0x51}},
		{"0000000000000000", []byte{97, 66, 52, 142, 135, 70, 64, 5}},
		{"1000000000000000", []byte{139, 222, 125, 187, 64, 191, 248, 252}},
		{"0001000000000000", []byte{69, 244, 234, 92, 255, 153, 88, 152}},
		{"MySecret", []byte{129, 255, 91, 1, 64, 150, 3, 232}},
	}
	for _, x := range list {
		check12345678(t, x.passWord, x.pattern)
	}
}

func check12345678(t *testing.T, password string, exp []byte) {
	cipher.Password = password
	cipher.ShowKey = true
	tst.AssertNoErr(t, cipher.SetUp())
	b := []byte{1, 2, 3, 4, 5, 6, 7, 8}
	c := cipher.Encrypt8(b)
	tst.Equal(t, exp, c)
	d := cipher.Decrypt8(c)
	tst.Equal(t, b, d)
}
