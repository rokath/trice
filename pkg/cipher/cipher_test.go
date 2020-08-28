// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// blackbox test
package cipher_test

import (
	"testing"

	"github.com/rokath/trice/pkg/cipher"
)

func TestPwdNone(t *testing.T) {
	cipher.Password = "none"
	cipher.ShowKey = false
	e := cipher.SetUp()
	assertEqual(t, nil, e)

	b := []byte{1, 2, 3, 4, 5, 6, 7, 8}
	c := cipher.Encrypt8(b)
	assertEqual(t, b, c)

	d := cipher.Decrypt8(c)
	assertEqual(t, c, d)
}

func Test8PwdXXZ(t *testing.T) {
	cipher.Password = "XYZ"
	cipher.ShowKey = true
	e := cipher.SetUp()
	assertEqual(t, nil, e)

	b := []byte{1, 2, 3, 4, 5, 6, 7, 8}
	c := cipher.Encrypt8(b)
	exp := []byte{0x32, 0xa8, 0x10, 0x11, 0x93, 0x8e, 0x64, 0x51}
	assertEqual(t, exp, c)

	d := cipher.Decrypt8(c)
	assertEqual(t, b, d)
}

func Test4PwdXXZ(t *testing.T) {
	cipher.Password = "XYZ"
	cipher.ShowKey = false
	e := cipher.SetUp()
	assertEqual(t, nil, e)

	b := []byte{1, 2, 3, 4}
	c := cipher.Encrypt8(b)
	exp := []byte{0xd0, 0x4a, 0x20, 0x8a, 0x62, 0x49, 0xdd, 0x5f}
	assertEqual(t, exp, c)

	d := cipher.Decrypt8(c)
	b8 := []byte{1, 2, 3, 4, 0x16, 0x16, 0x16, 0x16}
	assertEqual(t, b8, d)
}
