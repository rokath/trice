// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// white-box test
package id

/*
// TestLutFileTransfer checks lut file transfer.
func TestLutFileTransfer(t *testing.T) {
	wr := sampleLut0()
	exp := sampleLutMap0
	ramFS, _ := memfs.NewFS()
	fn := "TestWriteLutToFile.JSON"
	fh, e := ramFS.Create(fn) //  .WriteFile(fn, []byte(""), 0755))
	assert.Nil(t, e)
	assert.Nil(t, wr.toFile(ramFS, fn))
	rd := make(TriceIDLookUp)
	assert.Nil(t, rd.fromFile(fn))
	act := fmt.Sprint(rd)
	assert.Equal(t, exp, act)
}
*/
