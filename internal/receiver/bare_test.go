// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// blackbox test.
package receiver

import (
	"bytes"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestNewTricesfromBare(t *testing.T) {
	r := bytes.NewReader([]byte{'j', 'a', 'r', 1, 1, 1, 1, 0x89, 0xab, 0xcd, 0xef, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4}) // todo: byteorder in sync
	hardReadError := make(chan bool)
	p := NewTricesfromBare(r, hardReadError)
	for {
		select {
		case bAct := <-p.ignoredCh:
			bExp := []byte{106, 97, 114, 1, 1, 1, 1, 0x89, 0xab, 0xcd, 0xef}
			assert.Equal(t, bExp, bAct)
		case tAct := <-p.atomsCh:
			tExp := []Trice{
				{ID: 514, Value: 0x0202},
				{ID: 771, Value: 0x0303},
			}
			assert.Equal(t, tExp, tAct)
			return
		}
	}
}
