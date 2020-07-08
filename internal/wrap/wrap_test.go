// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package wrap_test is a blackbox test.
package wrap_test

import (
	"bytes"
	"errors"
	"testing"

	"github.com/rokath/trice/internal/bare"
	"github.com/rokath/trice/internal/wrap"
	"github.com/rokath/trice/pkg/lib"
)

func Test1(t *testing.T) {
	r := bytes.NewReader([]byte{})
	_, err := wrap.NewReader(r, "xxx")
	lib.Equals(t, errors.New("unknown encoding"), err)

}

// TestWrapInputWithSync checks disturbed wrapped input data reading
func TestWrapInputWithSync(t *testing.T) {
	r := bytes.NewReader([]byte{'j', 'a', 'r', 0xC0, 0x60, 0x60, 0xC0, 1, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 0xC0, 0x60, 0x60, 0xC0, 8, 8, 5, 5})
	x, err := wrap.NewReader(r, "wrap")
	lib.Equals(t, nil, err)

	// read 3 baretrice items
	act := make([]wrap.Item, 5)
	n, err := x.Read(act)
	lib.Equals(t, nil, err)
	lib.Equals(t, 2, n)
	act = act[:n]

	exp := []wrap.Item{
		{
			Start:  0xC0,
			Dest:   0x60,
			Source: 0x60,
			Crc:    0xC0,
			BareV: bare.Item{
				ID:    0x0101,
				Value: [2]byte{0x02, 0x02},
			},
		},
		{
			Start:  0xC0,
			Dest:   0x60,
			Source: 0x60,
			Crc:    0xC0,
			BareV: bare.Item{
				ID:    0x0808,
				Value: [2]byte{0x05, 0x05},
			},
		},
	}

	lib.Equals(t, exp, act)
}
