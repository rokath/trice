// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// white-box test
package id

import (
	"fmt"
	"testing"

	"github.com/psanford/memfs"
	"github.com/tj/assert"
)

// TestLutFileTransfer checks lut file transfer.
func _TestLutFileTransfer_psanford(t *testing.T) {
	wr := sampleLut0()
	exp := sampleLutMap0
	ramFS := memfs.New()
	fn := "TestWriteLutToFile.JSON"
	assert.Nil(t, ramFS.WriteFile(fn, []byte(""), 0755))
	assert.Nil(t, wr.toFile(ramFS, fn))
	rd := make(TriceIDLookUp)
	assert.Nil(t, rd.fromFile(fn))
	act := fmt.Sprint(rd)
	assert.Equal(t, exp, act)
}
