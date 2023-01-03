// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// white-box test
package id

import (
	"fmt"
	"testing"

	"github.com/spf13/afero"
	"github.com/tj/assert"
)

// TestLutFileTransfer checks lut file transfer.
func TestLutFileTransfer(t *testing.T) {
	t.Parallel()
	wr := sampleLut0()
	exp := sampleLutMap0

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	//fSys := &afero.Afero{Fs: afero.NewOsFs()}

	fn := "TestWriteLutToFile.JSON"
	assert.Nil(t, wr.toFile(fSys, fn))
	rd := make(TriceIDLookUp)
	assert.Nil(t, rd.fromFile(fSys, fn))
	act := fmt.Sprint(rd)
	assert.Equal(t, exp, act)
}

func TestBasePath(t *testing.T) {
	baseFs := &afero.MemMapFs{}
	baseFs.MkdirAll("/base/path/tmp", 0777)
	bp := afero.NewBasePathFs(baseFs, "/base/path")

	if _, err := bp.Create("/tmp/foo"); err != nil {
		t.Errorf("Failed to set real path")
	}

	if fh, err := bp.Create("../tmp/bar"); err == nil {
		t.Errorf("succeeded in creating %s ...", fh.Name())
	}
}
