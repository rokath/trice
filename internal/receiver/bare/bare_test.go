// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package bare_test is a blackbox test.
package bare_test

import (
	"bytes"
	"errors"
	"testing"

	"github.com/rokath/trice/internal/receiver/bare"
	"github.com/rokath/trice/pkg/lib"
)

func Test1(t *testing.T) {
	r := bytes.NewReader([]byte{})
	_, err := bare.NewReader(r, "xxx")
	lib.Equals(t, errors.New("unknown encoding"), err)

}

// TestRawInputWithSync checks raw input data with sync package in 2 sequential reads
func TestRawInputWithSync(t *testing.T) {
	r := bytes.NewReader([]byte{'j', 'a', 'r', 1, 1, 1, 1, 0x16, 0x16, 0x16, 0x16, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4})
	x, err := bare.NewReader(r, "raw")
	lib.Equals(t, nil, err)

	// read 3 bare items
	act := make([]bare.Item, 3)
	n, err := x.Read(act)
	lib.Equals(t, nil, err)
	lib.Equals(t, 3, n)
	act = act[:n]

	exp := []bare.Item{{0x0101, 0x0101}, {0x1616, 0x1616}, {0x0202, 0x0202}}
	lib.Equals(t, exp, act)

	// read 3 baretrice items, but get only 1
	act = make([]bare.Item, 3)
	n, err = x.Read(act)
	lib.Equals(t, nil, err)
	lib.Equals(t, 1, n)
	act = act[:n]

	exp = []bare.Item{{0x0303, 0x0303}}
	lib.Equals(t, exp, act)

	// at this point only the last 2 bytes {4,4} in the internal buffer
}

// TestWrapInputWithSync checks disturbed wrapped input data reading
func TestWrapInputWithSync(t *testing.T) {
	r := bytes.NewReader([]byte{'j', 'a', 'r', 0xC0, 0x60, 0x60, 0xC0, 1, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 0xC0, 0x60, 0x60, 0xC0, 8, 8, 5, 5})
	x, err := bare.NewReader(r, "wrap")
	lib.Equals(t, nil, err)

	// read 3 baretrice items
	act := make([]bare.Item, 5)
	n, err := x.Read(act)
	lib.Equals(t, nil, err)
	lib.Equals(t, 2, n)
	act = act[:n]

	exp := []bare.Item{{0x0101, 0x0202}, {0x0808, 0x0505}}
	lib.Equals(t, exp, act)
}
