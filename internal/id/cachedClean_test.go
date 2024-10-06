package id_test

import (
	"io"
	"testing"
	"time"

	"github.com/rokath/trice/internal/args"
	"github.com/rokath/trice/internal/id"
	"github.com/tj/assert"
)

// TestCleanOnCleanedWithoutCacheFolder checks if no cache folder is created automatically.
func TestCleanOnCleanedWithoutCacheFolder(t *testing.T) {
	defer setupCacheTest(t)()
	assertFileCreate(t, fSys, sFname, `trice("msg:value=%d\n", -1);`)

	fSys.RemoveAll(cache)
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "clean", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-src", sFname}))

	assertFileContent(t, fSys, sFname, `trice("msg:value=%d\n", -1);`) // check modified src file
	assertFileNotExists(t, fSys, cCache)                               // check for not existing cCache
	assertFileNotExists(t, fSys, iCache)                               // check for not existing iCache
}

// TestCleanOnCleanedWithCacheFolder checks if cCache is created and no iCache is crreated.
func TestCleanOnCleanedWithCacheFolder(t *testing.T) {
	defer setupCacheTest(t)()
	assertFileCreate(t, fSys, sFname, `trice("msg:value=%d\n", -1);`)

	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "clean", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-src", sFname}))

	assertFileContent(t, fSys, sFname, `trice("msg:value=%d\n", -1);`) // check modified src file
	assertFileNotExists(t, fSys, iCache)                               // check for not existing iCache
	assertEqualMTimes(t, fSys, sFname, cCache)                         // check for correct updated cCache
}

// TestCleanOnInsertedWithoutCacheFolder checks if no cache folder is created automatically.
func TestCleanOnInsertedWithoutCacheFolder(t *testing.T) {
	defer setupCacheTest(t)()
	assertFileCreate(t, fSys, sFname, `trice(iD(999), "msg:value=%d\n", -1);`)

	fSys.RemoveAll(cache)
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "clean", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-src", sFname})) // trice clean

	assertFileContent(t, fSys, sFname, `trice("msg:value=%d\n", -1);`) // check modified src file
	assertFileNotExists(t, fSys, cCache)                               // check for not existing cCache
	assertFileNotExists(t, fSys, iCache)                               // check for not existing iCache
}

// TestCleanOnInsertedWithCacheFolder checks if cCache is created and no iCache is crreated.
func TestCleanOnInsertedWithCacheFolder(t *testing.T) {
	defer setupCacheTest(t)()
	assertFileCreate(t, fSys, sFname, `trice(iD(999), "msg:value=%d\n", -1);`)

	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "clean", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-src", sFname}))

	assertFileContent(t, fSys, sFname, `trice("msg:value=%d\n", -1);`) // check modified src file
	assertFileNotExists(t, fSys, iCache)                               // check for not existing iCache
	assertEqualMTimes(t, fSys, sFname, cCache)                         // check for correct updated cCache
}

func TestCleanOnInsertedFileWithButInvalidCache(t *testing.T) {
	defer setupCacheTest(t)()
	assertFileCreate(t, fSys, sFname, `trice(iD(999), "msg:value=%d\n", -1);`)

	// status: invalid iCache && invalid cCache here && file edited
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "clean", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-src", sFname})) // trice clean

	assertFileContent(t, fSys, sFname, `trice("msg:value=%d\n", -1);`) // check modified src file
	assertEqualMTimes(t, fSys, sFname, cCache)                         // check for correct updated cCache
	assertFileNotExists(t, fSys, iCache)                               // check for not existing iCache
}

func TestCleanOnCleanedFileWithInvalidCache(t *testing.T) {
	defer setupCacheTest(t)()
	assertFileCreate(t, fSys, sFname, `trice("msg:value=%d\n", -1);`) // create src file
	sMTime0 := mTime(t, fSys, sFname)                                 // keep mtime

	// status: invalid iCache && invalid cCache here && file edited
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "clean", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-src", sFname})) // trice clean

	assertFileContent(t, fSys, sFname, `trice("msg:value=%d\n", -1);`)
	sMTime1 := mTime(t, fSys, sFname)
	assert.Equal(t, sMTime0, sMTime1)          // check for untouched sFname
	assertEqualMTimes(t, fSys, sFname, cCache) // check for correct updated cCache
	assertFileNotExists(t, fSys, iCache)       // check for not existing iCache
}

func TestCleanOnCleanedFileWithValidCCache(t *testing.T) {
	defer setupCacheTest(t)()
	assertFileCreate(t, fSys, sFname, `trice("msg:value=%d\n", -1);`) // create src file
	sFNameMTime := mTime(t, fSys, sFname)                             // keep mtime

	// status: invalid iCache && invalid cCache here && file edited
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "clean", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-src", sFname})) // trice clean first time

	// status: invalid iCache && valid cCache here && file NOT edited
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "clean", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-src", sFname})) // trice clean second time

	assertFileContent(t, fSys, sFname, `trice("msg:value=%d\n", -1);`)
	assert.Equal(t, sFNameMTime, mTime(t, fSys, sFname)) // check for untouched sFname
	assertEqualMTimes(t, fSys, sFname, cCache)           // check for correct updated cCache
	assertFileNotExists(t, fSys, iCache)                 // check for not existing iCache

	assertFileCreate(t, fSys, sFname, `trice("msg:value=%d\n", -2);`)                                                                             // "edit" src file
	sFNameMTime = mTime(t, fSys, sFname)                                                                                                          // keep mtime
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "clean", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-src", sFname})) // trice clean again

	assertFileContent(t, fSys, sFname, `trice("msg:value=%d\n", -2);`) // check for not modified src file
	assert.Equal(t, sFNameMTime, mTime(t, fSys, sFname))               // check for untouched sFname
	assertEqualMTimes(t, fSys, sFname, cCache)                         // check for correct updated cCache
	assertFileNotExists(t, fSys, iCache)                               // check for not existing iCache
}

func TestClean_On_valid_iCache_valid_cCache_inserted_file_not_edited(t *testing.T) {
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

	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "clean", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-src", sFname}))

	assertNotMTime(t, fSys, sT, sFname)        // check for touched sFname
	assertMTime(t, fSys, cT, cCache)           // check for untouched cCache
	assertMTime(t, fSys, iT, iCache)           // check for untouched iCache
	assertEqualMTimes(t, fSys, cCache, sFname) // check for valid cCache
}

func TestClean_On_valid_iCache_valid_cCache_clean_file_not_edited(t *testing.T) {
	defer setupCacheTest(t)()

	assertFileCreate(t, fSys, sFname, `trice(iD(999), "msg:value=%d\n", -1);`) // inserted file
	assert.Nil(t, id.CopyFileWithMTime(fSys, iCache, sFname))                  // not edited
	iT := mTime(t, fSys, iCache)

	for iT == mTime(t, fSys, sFname) { // Loop, to ensure the file mtimes are different.
		time.Sleep((10 * time.Microsecond))
		assertFileCreate(t, fSys, sFname, ` "msg:value=%d\n", -1);`) // inserted file
	}

	assert.Nil(t, id.CopyFileWithMTime(fSys, cCache, sFname)) // not edited
	cT := mTime(t, fSys, cCache)

	sT := mTime(t, fSys, sFname)
	assert.NotEqual(t, cT, iT)
	assertNotEqualMTimes(t, fSys, cCache, iCache)

	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "clean", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-src", sFname}))
	assertMTime(t, fSys, sT, sFname)           // check for untouched sFname
	assertMTime(t, fSys, cT, cCache)           // check for untouched cCache
	assertMTime(t, fSys, iT, iCache)           // check for untouched iCache
	assertEqualMTimes(t, fSys, cCache, sFname) // check for valid cCache
}

func TestClean_On_valid_iCache_valid_cCache_inserted_file_edited(t *testing.T) {
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

	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "clean", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-src", sFname}))

	assertFileContent(t, fSys, sFname, `trice("msg:value=%d\n", -2);`) // check src file content
	assertNotMTime(t, fSys, sT, sFname)                                // check for touched sFname
	assertFileNotExists(t, fSys, iCache)                               // check for invalid iCache
	assertEqualMTimes(t, fSys, cCache, sFname)                         // check for valid cCache
}

func TestClean_On_valid_iCache_valid_cCache_clean_file_edited(t *testing.T) {
	defer setupCacheTest(t)()

	assertFileCreate(t, fSys, sFname, `trice(iD(999), "msg:value=%d\n", -1);`) // inserted file
	assert.Nil(t, id.CopyFileWithMTime(fSys, iCache, sFname))                  // not edited
	iT := mTime(t, fSys, iCache)

	for iT == mTime(t, fSys, sFname) { // Loop, to ensure the file mtimes are different.
		time.Sleep((10 * time.Microsecond))
		assertFileCreate(t, fSys, sFname, ` "msg:value=%d\n", -1);`) // inserted file
	}

	assert.Nil(t, id.CopyFileWithMTime(fSys, cCache, sFname)) // not edited
	cT := mTime(t, fSys, cCache)
	assert.NotEqual(t, cT, iT)
	assertNotEqualMTimes(t, fSys, cCache, iCache)

	sT0 := mTime(t, fSys, sFname)
	// When editing, the old content is still buffered and not synced to disk, so we need to wait until the new mtime.
	sT := sT0
	for sT0 == sT { // Wait for the file system ...
		assertFileCreate(t, fSys, sFname, `trice("msg:value=%d\n", -2);`) // edit file
		sT = mTime(t, fSys, sFname)
		time.Sleep(10 * time.Millisecond)
	}
	assertNotEqualMTimes(t, fSys, iCache, sFname)

	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "clean", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-src", sFname}))

	assertFileContent(t, fSys, sFname, `trice("msg:value=%d\n", -2);`) // check modified src file
	assertMTime(t, fSys, sT, sFname)                                   // check for untouched sFname
	assertFileNotExists(t, fSys, iCache)                               // check for invalid iCache
	assertEqualMTimes(t, fSys, cCache, sFname)                         // check for valid cCache
}
