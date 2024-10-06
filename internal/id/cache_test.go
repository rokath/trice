package id_test

/*

TODO:

// TestCacheLogic tests several common variants occuring in cache usage.
func _TestCacheLogic(t *testing.T) {
	defer setupCacheTest(t)()


		// check for unchanged cCache
		cCacheStat, err = fSys.Stat(cCache)
		assert.Nil(t, err)
		assert.True(t, cCacheMTime_0 == cCacheStat.ModTime())

		// check for unchanged iCache
		iCacheStat, err = fSys.Stat(iCache)
		assert.Nil(t, err)
		assert.True(t, iCacheMTime_1 == iCacheStat.ModTime())

		// check for unchanged sFname
		sFnameStat, err = fSys.Stat(sFname)
		assert.Nil(t, err)
		assert.True(t, sFnameModTime_0 == sFnameStat.ModTime())

		// "edit" src file
		src = `trice("msg:value=%d\n", -3);`
		assert.Nil(t, fSys.WriteFile(sFname, []byte(src), 0777))

		// status: valid iCache && valid cCache here && file edited

		// trice insert
		assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "insert", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-IDMin", "999", "-IDMax", "999", "-src", sFname}))

		// check for correct updated iCache
		iCacheStat, err = fSys.Stat(iCache)
		assert.Nil(t, err)
		iCacheMTime_0 := iCacheStat.ModTime()
		sFnameStat, err = fSys.Stat(sFname)
		assert.Nil(t, err)
		sFnameModTime_0 = sFnameStat.ModTime()
		assert.True(t, iCacheMTime_0 == sFnameModTime_0)

		// check for invalid iCache
		_, err = fSys.Stat(iCache)
		assert.Error(t, err)

		// status: valid iCache && invalid cCache here && file was edited before

		// trice clean
		assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "clean", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-src", sFname}))

		// check src file
		expSrc = `trice("msg:value=%d\n", -3);`
		actSrc, e = fSys.ReadFile(sFname)
		assert.Nil(t, e)
		assert.Equal(t, expSrc, string(actSrc))

		// check for correct updated cCache
		cCacheStat, err = fSys.Stat(cCache)
		assert.Nil(t, err)
		cCacheMTime_0 = cCacheStat.ModTime()
		sFnameStat, err = fSys.Stat(sFname)
		assert.Nil(t, err)
		sFnameModTime_0 = sFnameStat.ModTime()
		fmt.Println("cCacheMTime_0:", cCacheMTime_0)
		fmt.Println("sFnameModTime_0:", sFnameModTime_0)
		assert.True(t, cCacheMTime_0 == sFnameModTime_0)

		// check for unchanged iCache
		iCacheStat, err = fSys.Stat(iCache)
		assert.Nil(t, err)
		assert.True(t, iCacheMTime_0 == iCacheStat.ModTime())

		// cleanup
		fSys.Remove(id.FnJSON)
		fSys.Remove(id.LIFnJSON)
		fSys.RemoveAll(id.UserHomeDir)
}
*/
