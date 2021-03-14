// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// blackbox test
package cipher

import (
	"testing"

	"github.com/rokath/trice/pkg/tst"
)

func TestMySecret1(t *testing.T) {
	Password = ""
	checkBig(t)
}

func TestMySecret2(t *testing.T) {
	Password = "aSecret"
	checkBig(t)
}

func TestMySecret3(t *testing.T) {
	Password = ""
	checkMix(t)
}

func TestMySecret4(t *testing.T) {
	Password = "aSecret"
	checkMix(t)
}

func TestMySecret5(t *testing.T) {
	Password = ""
	checkSmall(t)
}
func TestMySecret6(t *testing.T) {
	Password = "aSecret"
	checkSmall(t)
}

func checkBig(t *testing.T) {
	tst.AssertNoErr(t, SetUp())
	src := []byte{0, 1, 2, 3, 4, 5, 6, 7}
	enc := Encrypt8(src)
	dst := Decrypt8(enc)
	tst.Equal(t, src, dst)
}

func checkMix(t *testing.T) {
	tst.AssertNoErr(t, SetUp())
	src := []byte{0, 1, 2, 3, 4, 5, 6, 7}
	enc := Encrypt8(src)
	dst := make([]byte, 8)
	decrypt8(dst, enc)
	tst.Equal(t, src, dst)
}

func checkSmall(t *testing.T) {
	tst.AssertNoErr(t, SetUp())
	src := []byte{0, 1, 2, 3, 4, 5, 6, 7}
	enc := make([]byte, 8)
	encrypt8(enc, src)
	dst := make([]byte, 8)
	decrypt8(dst, enc)
	tst.Equal(t, src, dst)
}
