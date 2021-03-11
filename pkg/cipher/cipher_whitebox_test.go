// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// blackbox test
package cipher

import (
	"testing"

	"github.com/rokath/trice/pkg/tst"
)

func TestMySecret2(t *testing.T) {
	Password = "aSecret"
	tst.AssertNoErr(t, SetUp())

	src := []byte{0, 1, 2, 3, 4, 5, 6, 7}
	enc := Encrypt8(src)
	dst := Decrypt8(enc)
	tst.Equal(t, src, dst)
}

func TestMySecret3(t *testing.T) {
	Password = ""
	tst.AssertNoErr(t, SetUp())

	src := []byte{0, 1, 2, 3, 4, 5, 6, 7}
	enc := Encrypt8(src)
	dst := make([]byte, 8)
	decrypt8(dst, enc)
	tst.Equal(t, src, dst)
}

func TestMySecret4(t *testing.T) {
	Password = "aSecret"
	tst.AssertNoErr(t, SetUp())

	src := []byte{0, 1, 2, 3, 4, 5, 6, 7}
	enc := Encrypt8(src)
	dst := make([]byte, 8)
	decrypt8(dst, enc)
	tst.Equal(t, src, dst)
}

func TestMySecret5(t *testing.T) {
	Password = ""
	tst.AssertNoErr(t, SetUp())

	src := []byte{0, 1, 2, 3, 4, 5, 6, 7}
	enc := make([]byte, 8)
	encrypt8(enc, src)
	dst := make([]byte, 8)
	decrypt8(dst, enc)
	tst.Equal(t, src, dst)
}

func TestMySecret6(t *testing.T) {
	Password = "aSecret"
	tst.AssertNoErr(t, SetUp())

	src := []byte{0, 1, 2, 3, 4, 5, 6, 7}
	enc := Encrypt8(src)
	dst := Decrypt8(enc)
	tst.Equal(t, src, dst)
}
