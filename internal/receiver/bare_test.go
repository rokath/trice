// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// blackbox test.
package receiver

import (
	"bytes"
	"testing"

	"github.com/rokath/trice/pkg/assert"
)

func TestNewTricesfromBare(t *testing.T) {
	r := bytes.NewReader([]byte{'j', 'a', 'r', 1, 1, 1, 1, 0x89, 0xab, 0xef, 0xcd, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4}) // todo: byteorder in sync
	p := NewTricesfromBare(r)
	var bFlag, tFlag bool
	for {
		select {
		case bAct := <-p.ignored:
			bExp := []byte{106, 97, 114}
			assert.Equal(t, bExp, bAct)
			bFlag = true
		case tAct := <-p.atoms:
			tExp := []Trice{
				{ID: 257, Value: [2]byte{1, 1}},
				{ID: 0x89ab, Value: [2]byte{0xef, 0xcd}}, // endianess!
				{ID: 514, Value: [2]byte{2, 2}},
				{ID: 771, Value: [2]byte{3, 3}},
			}
			assert.Equal(t, tExp, tAct)
			tFlag = true
		default:
			if bFlag && tFlag {
				assert.Equal(t, []byte{4, 4}, p.syncArray[:2]) // todo: syncBuffer vs. syncArray
				return
			}
		}
	}
}
