// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// blackbox test.
package receiver

import (
	"bytes"
	"testing"
)

func TestNewTricesfromBare(t *testing.T) {
	r := bytes.NewReader([]byte{'j', 'a', 'r', 1, 1, 1, 1, 0x16, 0x16, 0x16, 0x16, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4})
	p := NewTricesfromBare(r)
	var bFlag, tFlag bool
	for {
		select {
		case bAct := <-p.ignored:
			bExp := []byte{106, 97, 114}
			assertEqual(t, bExp, bAct)
			bFlag = true
		case tAct := <-p.atoms:
			tExp := []Trice{
				{ID: 257, Value: [2]byte{1, 1}},
				{ID: 5654, Value: [2]byte{22, 22}},
				{ID: 514, Value: [2]byte{2, 2}},
				{ID: 771, Value: [2]byte{3, 3}},
			}
			assertEqual(t, tExp, tAct)
			tFlag = true
		default:
			if bFlag && tFlag {
				assertEqual(t, []byte{4, 4}, p.syncBuffer)
				return
			}
		}
	}
}
