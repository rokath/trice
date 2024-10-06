package id_test

import (
	"io"
	"testing"
	"time"

	"github.com/rokath/trice/internal/args"
	"github.com/rokath/trice/internal/id"
	"github.com/tj/assert"
)

// TestInsertOnCleanedWithoutCacheFolder checks if no cache folder is created automatically.
func TestInsertOnCleanedWithoutCacheFolder(t *testing.T) {
	defer setupCacheTest(t)()
	assertFileCreate(t, fSys, sFname, `trice("msg:value=%d\n", -1);`)

	fSys.RemoveAll(cache)
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "insert", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-IDMin=999", "-IDMax=999", "-src", sFname}))

	assertFileContent(t, fSys, sFname, `trice(iD(999), "msg:value=%d\n", -1);`) // check modified src file
	assertFileNotExists(t, fSys, cCache)                                        // check for not existing cCache
	assertFileNotExists(t, fSys, iCache)                                        // check for not existing iCache
}

// TestInsertOnCleanedWithCacheFolder checks if iCache is created and no cCache is crreated.
func TestInsertOnCleanedWithCacheFolder(t *testing.T) {
	defer setupCacheTest(t)()
	assertFileCreate(t, fSys, sFname, `trice("msg:value=%d\n", -1);`)

	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "insert", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-IDMin", "999", "-IDMax", "999", "-src", sFname}))

	assertFileContent(t, fSys, sFname, `trice(iD(999), "msg:value=%d\n", -1);`) // check modified src file
	assertFileNotExists(t, fSys, cCache)                                        // check for not existing cCache
	assertEqualMTimes(t, fSys, sFname, iCache)                                  // check for correct updated iCache
}

// TestInsertOnInsertedWithoutCacheFolder checks if no cache folder is created automatically.
func TestInsertOnInsertedWithoutCacheFolder(t *testing.T) {
	defer setupCacheTest(t)()
	assertFileCreate(t, fSys, sFname, `trice(iD(999), "msg:value=%d\n", -1);`)

	fSys.RemoveAll(cache)
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "insert", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-IDMin=999", "-IDMax=999", "-src", sFname}))

	assertFileContent(t, fSys, sFname, `trice(iD(999), "msg:value=%d\n", -1);`) // check modified src file
	assertFileNotExists(t, fSys, cCache)                                        // check for not existing cCache
	assertFileNotExists(t, fSys, iCache)                                        // check for not existing iCache
}

// TestInsertOnInsertedWithCacheFolder checks if iCache is created and no cCache is crreated.
func TestInseredInsertWithCacheFolder(t *testing.T) {
	defer setupCacheTest(t)()
	assertFileCreate(t, fSys, sFname, `trice(iD(999), "msg:value=%d\n", -1);`)

	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "insert", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-IDMin", "999", "-IDMax", "999", "-src", sFname}))

	assertFileContent(t, fSys, sFname, `trice(iD(999), "msg:value=%d\n", -1);`) // check modified src file
	assertFileNotExists(t, fSys, cCache)                                        // check for not existing cCache
	assertEqualMTimes(t, fSys, sFname, iCache)                                  // check for correct updated iCache
}

func TestInsertOnCleanedFileWithButInvalidCache(t *testing.T) {
	defer setupCacheTest(t)()
	assertFileCreate(t, fSys, sFname, `trice("msg:value=%d\n", -1);`)

	// status: invalid cCache && invalid iCache here && file edited
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "insert", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-IDMin", "999", "-IDMax", "999", "-src", sFname})) // trice insert

	assertFileContent(t, fSys, sFname, `trice(iD(999), "msg:value=%d\n", -1);`) // check modified src file
	assertEqualMTimes(t, fSys, sFname, iCache)                                  // check for correct updated iCache
	assertFileNotExists(t, fSys, cCache)                                        // check for not existing cCache
}

func TestInsertOnInsertedFileWithInvalidCache(t *testing.T) {
	defer setupCacheTest(t)()
	assertFileCreate(t, fSys, sFname, `trice(iD(999), "msg:value=%d\n", -1);`) // create src file
	sMTime0 := mTime(t, fSys, sFname)                                 // keep mtime

	// status: invalid cCache && invalid iCache here && file edited
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "insert", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-IDMin", "999", "-IDMax", "999", "-src", sFname})) // trice insert

	assertFileContent(t, fSys, sFname, `trice(iD(999), "msg:value=%d\n", -1);`)
	sMTime1 := mTime(t, fSys, sFname)
	assert.Equal(t, sMTime0, sMTime1)          // check for untouched sFname
	assertEqualMTimes(t, fSys, sFname, iCache)           // check for correct updated iCache
	assertFileNotExists(t, fSys, cCache)                 // check for not existing cCache
}

func TestInsertOnInsertedFileWithValidCCache(t *testing.T) {
	defer setupCacheTest(t)()
	assertFileCreate(t, fSys, sFname, `trice(iD(999), "msg:value=%d\n", -1);`) // create src file
	sFNameMTime := mTime(t, fSys, sFname)                                      // keep mtime

	// status: invalid cCache && invalid iCache here && file edited
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "insert", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-IDMin", "999", "-IDMax", "999", "-src", sFname})) // trice insert first time

	// status: invalid cCache && valid iCache here && file NOT edited
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "insert", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-IDMin", "999", "-IDMax", "999", "-src", sFname})) // trice insert second time

	assertFileContent(t, fSys, sFname, `trice(iD(999), "msg:value=%d\n", -1);`)
	assert.Equal(t, sFNameMTime, mTime(t, fSys, sFname)) // check for untouched sFname
	assertEqualMTimes(t, fSys, sFname, iCache)           // check for correct updated iCache
	assertFileNotExists(t, fSys, cCache)                 // check for not existing cCache

	assertFileCreate(t, fSys, sFname, `trice(iD(999), "msg:value=%d\n", -2);`)                                                                                                       // "edit" src file
	sFNameMTime = mTime(t, fSys, sFname)                                                                                                                                             // keep mtime
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "insert", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-IDMin", "999", "-IDMax", "999", "-src", sFname})) // trice insert again

	assertFileContent(t, fSys, sFname, `trice(iD(999), "msg:value=%d\n", -2);`) // check for not modified src file
	assert.Equal(t, sFNameMTime, mTime(t, fSys, sFname))                        // check for untouched sFname
	assertEqualMTimes(t, fSys, sFname, iCache)                                  // check for correct updated iCache
	assertFileNotExists(t, fSys, cCache)                                        // check for not existing cCache
}

func TestInsert_On_valid_iCache_valid_cCache_inserted_file_not_edited(t *testing.T) {
	defer setupCacheTest(t)()

	assertFileCreate(t, fSys, sFname, `trice("msg:value=%d\n", -1);`) // cleaned file
	assert.Nil(t, id.CopyFileWithMTime(fSys, cCache, sFname))         // not edited
	cT := mTime(t, fSys, cCache)

	for cT == mTime(t, fSys, sFname) { // Loop, to ensure the file mtimes are different.
		time.Sleep((10 * time.Microsecond))
		assertFileCreate(t, fSys, sFname, `trice(iD(999), "msg:value=%d\n", -1);`) // inserted file
	}
	sT := mTime(t, fSys, sFname)
	assert.Nil(t, id.CopyFileWithMTime(fSys, iCache, sFname)) // not edited
	iT := mTime(t, fSys, iCache)

	assert.NotEqual(t, cT, iT)
	assertNotEqualMTimes(t, fSys, cCache, sFname) // check for not touched sFname
	assertEqualMTimes(t, fSys, iCache, sFname)    // check for equal iCache

	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "insert", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-IDMin", "999", "-IDMax", "999", "-src", sFname}))

	assertMTime(t, fSys, sT, sFname)           // check for untouched sFname
	assertMTime(t, fSys, cT, cCache)           // check for untouched cCache
	assertMTime(t, fSys, iT, iCache)           // check for untouched iCache
	assertEqualMTimes(t, fSys, iCache, sFname) // check for valid iCache
}

func TestInsert_On_valid_iCache_valid_cCache_clean_file_not_edited(t *testing.T) {
	defer setupCacheTest(t)()

	assertFileCreate(t, fSys, sFname, `trice(iD(999), "msg:value=%d\n", -1);`) // inserted file
	assert.Nil(t, id.CopyFileWithMTime(fSys, iCache, sFname))                  // not edited
	iT := mTime(t, fSys, iCache)

	for iT == mTime(t, fSys, sFname) { // Loop, to ensure the file mtimes are different.
		time.Sleep((10 * time.Microsecond))
		assertFileCreate(t, fSys, sFname, `trice("msg:value=%d\n", -1);`) // inserted file
	}

	assert.Nil(t, id.CopyFileWithMTime(fSys, cCache, sFname)) // not edited
	cT := mTime(t, fSys, cCache)

	sT := mTime(t, fSys, sFname)
	assert.NotEqual(t, cT, iT)
	assertNotEqualMTimes(t, fSys, cCache, iCache)

	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "insert", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-IDMin", "999", "-IDMax", "999", "-src", sFname}))
	assertNotMTime(t, fSys, sT, sFname)        // check for touched sFname
	assertMTime(t, fSys, cT, cCache)           // check for untouched cCache
	assertMTime(t, fSys, iT, iCache)           // check for untouched iCache
	assertEqualMTimes(t, fSys, iCache, sFname) // check for valid iCache
}

func TestInsert_On_valid_iCache_valid_cCache_inserted_file_edited(t *testing.T) {
	defer setupCacheTest(t)()

	assertFileCreate(t, fSys, sFname, `trice("msg:value=%d\n", -1);`) // cleaned file
	assert.Nil(t, id.CopyFileWithMTime(fSys, cCache, sFname))         // not edited
	cT := mTime(t, fSys, cCache)

	for cT == mTime(t, fSys, sFname) { // Loop, to ensure the file mtimes are different.
		time.Sleep((10 * time.Microsecond))
		assertFileCreate(t, fSys, sFname, `trice(iD(999), "msg:value=%d\n", -1);`) // inserted file
	}
	assert.Nil(t, id.CopyFileWithMTime(fSys, iCache, sFname)) // not edited
	iT := mTime(t, fSys, iCache)
	assert.NotEqual(t, cT, iT)

	sT0 := mTime(t, fSys, sFname)
	// When editing, the old content is still buffered and not synced to disk, so we need to wait until the new mtime.
	sT := sT0
	for sT0 == sT { // Wait for the file system ...
		assertFileCreate(t, fSys, sFname, `trice(iD(999), "msg:value=%d\n", -2);`) // edit file
		sT = mTime(t, fSys, sFname)
		time.Sleep(10 * time.Millisecond)
	}
	assertNotEqualMTimes(t, fSys, iCache, sFname)

	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "insert", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-IDMin", "999", "-IDMax", "999", "-src", sFname}))

	assertFileContent(t, fSys, sFname, `trice(iD(999), "msg:value=%d\n", -2);`) // check src file content
	assertMTime(t, fSys, sT, sFname)                                            // check for untouched sFname
	assertFileNotExists(t, fSys, cCache)                                        // check for invalid cCache
	assertNotMTime(t, fSys, iT, iCache)                                         // check for touched iCache
}

func TestInsert_On_valid_iCache_valid_cCache_clean_file_edited(t *testing.T) {
	defer setupCacheTest(t)()

	assertFileCreate(t, fSys, sFname, `trice(iD(999), "msg:value=%d\n", -1);`) // inserted file
	assert.Nil(t, id.CopyFileWithMTime(fSys, iCache, sFname))                  // not edited
	iT := mTime(t, fSys, iCache)

	for iT == mTime(t, fSys, sFname) { // Loop, to ensure the file mtimes are different.
		time.Sleep((10 * time.Microsecond))
		assertFileCreate(t, fSys, sFname, `trice("msg:value=%d\n", -1);`) // inserted file
	}

	assert.Nil(t, id.CopyFileWithMTime(fSys, cCache, sFname)) // not edited
	cT := mTime(t, fSys, cCache)
	assert.NotEqual(t, cT, iT)
	assertNotEqualMTimes(t, fSys, cCache, iCache)

	sT0 := mTime(t, fSys, sFname)
	// When editing, the old content is still buffered and not synced to disk, so we need to wait until the new mtime.
	sT := sT0
	for sT0 == sT { // Wait for the file system ...
		assertFileCreate(t, fSys, sFname, `trice(i"msg:value=%d\n", -2);`) // edit file
		sT = mTime(t, fSys, sFname)
		time.Sleep(10 * time.Millisecond)
	}
	assertNotEqualMTimes(t, fSys, iCache, sFname)

	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "insert", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-IDMin", "999", "-IDMax", "999", "-src", sFname}))

	assertFileContent(t, fSys, sFname, `trice(iD(999), "msg:value=%d\n", -2);`) // check modified src file
	assertNotMTime(t, fSys, sT, sFname)                                         // check for touched sFname
	assertFileNotExists(t, fSys, cCache)                                        // check for invalid cCache
	assertEqualMTimes(t, fSys, iCache, sFname)                                  // check for valid iCache
}
