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
func _TestLutFileTransfer(t *testing.T) {
	t.Parallel()
	wr := sampleLut0()
	exp := sampleLutMap0

	//fSys := afero.NewMemMapFs()
	//fSys := &afero.Afero{Fs: ifSys}
	//fSys := afero.NewOsFs()

	fSys := &afero.MemMapFs{}
	//fSys.MkdirAll("src/a", 0755)

	fn := "TestWriteLutToFile.JSON"
	assert.Nil(t, wr.toFile(fSys, fn))
	rd := make(TriceIDLookUp)
	assert.Nil(t, rd.fromFile(fSys, fn))
	act := fmt.Sprint(rd)
	assert.Equal(t, exp, act)
	fmt.Println(fSys, wr, exp, fn, rd)
}

func _TestBasePath(t *testing.T) {
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
