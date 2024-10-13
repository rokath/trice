package id_test

/*

TODO:

// TestCacheLogic tests several common variants occuring in cache usage.
func _TestCacheLogic(t *testing.T) {
	defer setupCacheTest(t)()


		// check for unchanged cCache
		cCacheStat, err = FSys.Stat(cCache)
		assert.Nil(t, err)
		assert.True(t, cCacheMTime_0 == cCacheStat.ModTime())

		// check for unchanged iCache
		iCacheStat, err = FSys.Stat(iCache)
		assert.Nil(t, err)
		assert.True(t, iCacheMTime_1 == iCacheStat.ModTime())

		// check for unchanged sFname
		sFnameStat, err = FSys.Stat(sFname)
		assert.Nil(t, err)
		assert.True(t, sFnameModTime_0 == sFnameStat.ModTime())

		// "edit" src file
		src = `trice("msg:value=%d\n", -3);`
		assert.Nil(t, FSys.WriteFile(sFname, []byte(src), 0777))

		// status: valid iCache && valid cCache here && file edited

		// trice insert
		assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-IDMin", "999", "-IDMax", "999", "-src", sFname}))

		// check for correct updated iCache
		iCacheStat, err = FSys.Stat(iCache)
		assert.Nil(t, err)
		iCacheMTime_0 := iCacheStat.ModTime()
		sFnameStat, err = FSys.Stat(sFname)
		assert.Nil(t, err)
		sFnameModTime_0 = sFnameStat.ModTime()
		assert.True(t, iCacheMTime_0 == sFnameModTime_0)

		// check for invalid iCache
		_, err = FSys.Stat(iCache)
		assert.Error(t, err)


func _Test_CleanOn_valid_iCache_invalid_cCache_clean_file_edited(t *testing.T) {

	// trice clean
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "clean", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-src", sFname}))

	// check src file
	expSrc = `trice("msg:value=%d\n", -3);`
	actSrc, e = FSys.ReadFile(sFname)
	assert.Nil(t, e)
	assert.Equal(t, expSrc, string(actSrc))

	// check for correct updated cCache
	cCacheStat, err = FSys.Stat(cCache)
	assert.Nil(t, err)
	cCacheMTime_0 = cCacheStat.ModTime()
	sFnameStat, err = FSys.Stat(sFname)
	assert.Nil(t, err)
	sFnameModTime_0 = sFnameStat.ModTime()
	fmt.Println("cCacheMTime_0:", cCacheMTime_0)
	fmt.Println("sFnameModTime_0:", sFnameModTime_0)
	assert.True(t, cCacheMTime_0 == sFnameModTime_0)

	// check for unchanged iCache
	iCacheStat, err = FSys.Stat(iCache)
	assert.Nil(t, err)
	assert.True(t, iCacheMTime_0 == iCacheStat.ModTime())

	// cleanup
	FSys.Remove(FnJSON)
	FSys.Remove(LIFnJSON)
	FSys.RemoveAll(id.UserHomeDir)
}
*/
