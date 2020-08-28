// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package bare_test is a blackbox test.
package bare_test

import (
	"bytes"
	"errors"
	"testing"

	"github.com/rokath/trice/internal/bare"
)

func Test1(t *testing.T) {
	r := bytes.NewReader([]byte{})
	_, err := bare.NewReader(r, "xxx")
	assertEqual(t, errors.New("unknown encoding"), err)

}

// TestRawInputWithSync checks raw input data with sync package in 2 sequential reads
func TestRawInputWithSync(t *testing.T) {
	r := bytes.NewReader([]byte{'j', 'a', 'r', 1, 1, 1, 1, 0x16, 0x16, 0x16, 0x16, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4})
	//x, err := bare.NewReader(r, "raw")
	x, err := bare.NewRawReader(r)
	assertEqual(t, nil, err)

	// read 3 bare items
	act := make([]bare.Item, 3)
	n, err := x.Read(act)
	assertEqual(t, nil, err)
	assertEqual(t, 3, n)
	act = act[:n]

	exp := []bare.Item{
		{ID: 0x0101, Value: [2]byte{0x01, 0x01}},
		{ID: 0x1616, Value: [2]byte{0x16, 0x16}},
		{ID: 0x0202, Value: [2]byte{0x02, 0x02}},
	}

	assertEqual(t, exp, act)

	// read 3 baretrice items, but get only 1
	act = make([]bare.Item, 3)
	n, err = x.Read(act)
	assertEqual(t, nil, err)
	assertEqual(t, 1, n)
	act = act[:n]

	exp = []bare.Item{{ID: 0x0303, Value: [2]byte{0x03, 0x03}}}
	assertEqual(t, exp, act)

	// at this point only the last 2 bytes {4,4} in the internal buffer
}

// TestWrapInputWithSync checks disturbed wrapped input data reading
func TestWrapInputWithSync(t *testing.T) {
	r := bytes.NewReader([]byte{'j', 'a', 'r', 0xC0, 0x60, 0x60, 0xC0, 1, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 0xC0, 0x60, 0x60, 0xC0, 8, 8, 5, 5})
	x, err := bare.NewReader(r, "wrap")
	assertEqual(t, nil, err)

	// read 3 baretrice items
	act := make([]bare.Item, 5)
	n, err := x.Read(act)
	assertEqual(t, nil, err)
	assertEqual(t, 2, n)
	act = act[:n]

	exp := []bare.Item{
		{ID: 0x0101, Value: [2]byte{0x02, 0x02}},
		{ID: 0x0808, Value: [2]byte{0x05, 0x05}},
	}
	assertEqual(t, exp, act)
}
