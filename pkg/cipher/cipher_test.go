// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// blackbox test
package cipher_test

import (
	"testing"

	"github.com/rokath/trice/pkg/cipher"
	"github.com/rokath/trice/pkg/tst"
)

func TestPwdNone(t *testing.T) {
	cipher.Password = "none"
	cipher.ShowKey = false
	tst.AssertNoErr(t, cipher.SetUp())

	b := []byte{1, 2, 3, 4, 5, 6, 7, 8}
	c := cipher.Encrypt8(b)
	tst.Equal(t, b, c)

	d := cipher.Decrypt8(c)
	tst.Equal(t, c, d)
}

func Test8PwdXXZ(t *testing.T) {
	cipher.Password = "XYZ"
	cipher.ShowKey = true
	tst.AssertNoErr(t, cipher.SetUp())

	b := []byte{1, 2, 3, 4, 5, 6, 7, 8}
	c := cipher.Encrypt8(b)
	exp := []byte{0x32, 0xa8, 0x10, 0x11, 0x93, 0x8e, 0x64, 0x51}
	tst.Equal(t, exp, c)

	d := cipher.Decrypt8(c)
	tst.Equal(t, b, d)
}

func TestNullKey(t *testing.T) {
	cipher.Password = "0000000000000000"
	cipher.ShowKey = true
	tst.AssertNoErr(t, cipher.SetUp())

	b := []byte{1, 2, 3, 4, 5, 6, 7, 8}
	c := cipher.Encrypt8(b)
	exp := []byte{97, 66, 52, 142, 135, 70, 64, 5} // target ok
	tst.Equal(t, exp, c)

	d := cipher.Decrypt8(c)
	b8 := []byte{1, 2, 3, 4, 5, 6, 7, 8}
	tst.Equal(t, b8, d)
}
func Test1000Key(t *testing.T) {
	cipher.Password = "1000000000000000"
	cipher.ShowKey = true
	tst.AssertNoErr(t, cipher.SetUp())

	b := []byte{1, 2, 3, 4, 5, 6, 7, 8}
	c := cipher.Encrypt8(b)
	exp := []byte{139, 222, 125, 187, 64, 191, 248, 252} // target ok
	tst.Equal(t, exp, c)

	d := cipher.Decrypt8(c)
	b8 := []byte{1, 2, 3, 4, 5, 6, 7, 8}
	tst.Equal(t, b8, d)
}

func Test0001Key(t *testing.T) {
	cipher.Password = "0001000000000000"
	cipher.ShowKey = true
	tst.AssertNoErr(t, cipher.SetUp())

	b := []byte{1, 2, 3, 4, 5, 6, 7, 8}
	c := cipher.Encrypt8(b)
	exp := []byte{69, 244, 234, 92, 255, 153, 88, 152}
	tst.Equal(t, exp, c)

	d := cipher.Decrypt8(c)
	b8 := []byte{1, 2, 3, 4, 5, 6, 7, 8}
	tst.Equal(t, b8, d)
}

func TestMySecret(t *testing.T) {
	cipher.Password = "MySecret"
	cipher.ShowKey = true
	tst.AssertNoErr(t, cipher.SetUp())

	b := []byte{1, 2, 3, 4, 5, 6, 7, 8}
	c := cipher.Encrypt8(b)
	exp := []byte{129, 255, 91, 1, 64, 150, 3, 232} // target ok
	tst.Equal(t, exp, c)

	d := cipher.Decrypt8(c)
	b8 := []byte{1, 2, 3, 4, 5, 6, 7, 8}
	tst.Equal(t, b8, d)
}

func TestMySecret2(t *testing.T) {
	cipher.Password = "MySecret"
	cipher.ShowKey = true
	tst.AssertNoErr(t, cipher.SetUp())

	b := []byte{4, 3, 2, 1, 7, 6, 5, 4}
	c := cipher.Encrypt8(b)
	exp := []byte{145, 70, 10, 155, 79, 56, 249, 249}
	tst.Equal(t, exp, c)

	d := cipher.Decrypt8(c)
	b8 := []byte{4, 3, 2, 1, 7, 6, 5, 4}
	tst.Equal(t, b8, d)
}
