// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// white-box test
package id

/*
// TestLutFileTransfer checks lut file transfer.
func _TestLutFileTransfer(t *testing.T) { // Anti-Virus issue
	wr := sampleLut0()
	exp := sampleLutMap0
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	fn := "TestWriteLutToFile.JSON"
	assert.Nil(t, wr.toFile(fSys, fn))
	rd := make(TriceIDLookUp)
	assert.Nil(t, rd.fromFile(fSys, fn))
	act := fmt.Sprint(rd)
	assert.Equal(t, exp, act)
}

func _TestBasePath(t *testing.T) { // Anti-Virus issue
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

func _TestRefresh(t *testing.T) { // Anti-Virus issue
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	fn := "file.c"
	assert.Nil(t, fSys.WriteFile(fn, []byte(CFile0), 0777))
	fh, e := fSys.Create("til.json")
	assert.Nil(t, e)
	assert.Nil(t, fh.Close())
	SubCmdRefreshList(os.Stdout, fSys)
	//fh, e := fSys.Open(fn)
	act, e := fSys.ReadFile("til.json")
	assert.Nil(t, e)
	exp := ``
	assert.Equal(t, exp, string(act))
}
*/

/*
const (
	CFile0 = `
    // no stamp:
    break; case __LINE__: trice8     (           "msg:value=%d\n", -1  ); // user written code, empty macro for patching
    break; case __LINE__: trice8_M   ( 7009,     "msg:value=%d\n", -11 ); // trice tool modified code for compilation, calls internally fuction trice8_1_fn
    break; case __LINE__: trice8_1   (           "msg:value=%d\n", -1  ); // user written code, empty macro for patching
    break; case __LINE__: trice8_1_M ( 7009,     "msg:value=%d\n", -12 ); // trice tool modified code for compilation, calls internally fuction trice8_1_fn
    break; case __LINE__: trice8_1_fn( 7009,                       -13 ); // This is a direct function call, which works, but not inteded for user code.

  //break; case __LINE__: TRICE8_1   (           "msg:value=%d\n", -1  ); // legacy default with times stamp
  //break; case __LINE__: TRICE8_1   ( id(0),    "msg:value=%d\n", -1  ); // user written code
    break; case __LINE__: TRICE8_1   ( id(7009), "msg:value=%d\n", -14 ); // trice tool modified code for compilation, inlines code

    // 16-bit stamp:
    break; case __LINE__: Trice8     (           "msg:value=%d\n", -1  ); // user written code, empty macro for patching
    break; case __LINE__: Trice8_M   ( 7009,     "msg:value=%d\n", -11 ); // trice tool modified code for compilation, calls internally fuction trice8_1_fn
    break; case __LINE__: Trice8_1   (           "msg:value=%d\n", -1  ); // user written code, empty macro for patching
    break; case __LINE__: Trice8_1_M ( 7009,     "msg:value=%d\n", -12 ); // trice tool modified code for compilation, calls internally fuction trice8_1_fn
    break; case __LINE__: Trice8_1_fn( 7009,                       -13 ); // This is a direct function call, which works, but not inteded for user code.

  //break; case __LINE__: TRICE8_1   (           "msg:value=%d\n", -1  ); // legacy default with times stamp
  //break; case __LINE__: TRICE8_1   ( Id(0),    "msg:value=%d\n", -1  ); // user written code
    break; case __LINE__: TRICE8_1   ( Id(7009), "msg:value=%d\n", -14 ); // trice tool modified code for compilation, inlines code

    // 32-bit stamp:
    break; case __LINE__: TRice8     (           "msg:value=%d\n", -1  ); // user written code, empty macro for patching
    break; case __LINE__: TRice8_M   ( 7009,     "msg:value=%d\n", -11 ); // trice tool modified code for compilation, calls internally fuction trice8_1_fn
    break; case __LINE__: TRice8_1   (           "msg:value=%d\n", -1  ); // user written code, empty macro for patching
    break; case __LINE__: TRice8_1_M ( 7009,     "msg:value=%d\n", -12 ); // trice tool modified code for compilation, calls internally fuction trice8_1_fn
    break; case __LINE__: TRice8_1_fn( 7009,                       -13 ); // This is a direct function call, which works, but not inteded for user code.

  //break; case __LINE__: TRICE8_1   (           "msg:value=%d\n", -1  ); // legacy default with times stamp
  //break; case __LINE__: TRICE8_1   ( ID(0),    "msg:value=%d\n", -1  ); // user written code
    break; case __LINE__: TRICE8_1   ( ID(7009), "msg:value=%d\n", -14 ); // trice tool modified code for compilation, inlines code
`
)
*/
