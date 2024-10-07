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
	assertFileCreate(t, fSys, sFname, `trice("msg:value=%d\n", -1);`) // cleaned, edit

	fSys.RemoveAll(cache) // no cache folder
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "clean", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-src", sFname}))

	assertFileContent(t, fSys, sFname, `trice("msg:value=%d\n", -1);`) // check file content
	assertFileNotExists(t, fSys, cCache)                               // check for not existing cCache
	assertFileNotExists(t, fSys, iCache)                               // check for not existing iCache
}

// TestCleanOnInsertedWithoutCacheFolder checks if no cache folder is created automatically.
func TestCleanOnInsertedWithoutCacheFolder(t *testing.T) {
	defer setupCacheTest(t)()
	assertFileCreate(t, fSys, sFname, `trice(iD(999), "msg:value=%d\n", -1);`) // inserted, edit

	fSys.RemoveAll(cache)                                                                                                                         // no cache folder                                                                                                                        // no cache folder
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "clean", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-src", sFname})) // trice clean

	assertFileContent(t, fSys, sFname, `trice("msg:value=%d\n", -1);`) // check file content
	assertFileNotExists(t, fSys, cCache)                               // check for not existing cCache
	assertFileNotExists(t, fSys, iCache)                               // check for not existing iCache
}

func Test_0_1_0000X_clean_on_invalid_cCache_invalid_iCache_cleaned_file(t *testing.T) {
	defer setupCacheTest(t)()
	assertFileCreate(t, fSys, sFname, `trice("msg:value=%d\n", -1);`) // cleaned
	sT0 := mTime(t, fSys, sFname)                                     // keep mtime

	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "clean", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-src", sFname}))
	assertFileContent(t, fSys, sFname, `trice("msg:value=%d\n", -1);`) // check file content
	assertMTime(t, fSys, sT0, sFname)                                  // check for untouched sFname
	assertEqualMTimes(t, fSys, sFname, cCache)                         // check for correct updated cCache
	assertFileNotExists(t, fSys, iCache)                               // check for not existing iCache
}

func Test_2_3_0001X_clean_on_inalid_cCache_invalid_iCache_inserted_X_file(t *testing.T) {
	defer setupCacheTest(t)()
	assertFileCreate(t, fSys, sFname, `trice(iD(999), "msg:value=%d\n", -1);`) // inserted

	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "clean", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-src", sFname}))

	assertFileContent(t, fSys, sFname, `trice("msg:value=%d\n", -1);`) // check file content
	assertFileNotExists(t, fSys, iCache)                               // check for not existing iCache
	assertEqualMTimes(t, fSys, sFname, cCache)                         // check for correct updated cCache
}

func Test_4_5_0010X_clean_on_invalid_cCache_valid_iCache_cleaned_file(t *testing.T) {
	defer setupCacheTest(t)()
	assertFileCreate(t, fSys, sFname, `trice(iD999), "msg:value=%d\n", -1);`) // create inserted file
	sT0 := mTime(t, fSys, sFname)                                             // keep mtime
	id.CopyFileWithMTime(fSys, iCache, sFname)                                // create valid iCache
	assertFileCreate(t, fSys, sFname, `trice("msg:value=%d\n", -2);`)         // edit cleaned file
	sT1 := mTime(t, fSys, sFname)                                             // keep mtime

	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "clean", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-src", sFname}))

	assertFileContent(t, fSys, sFname, `trice("msg:value=%d\n", -2);`) // check for not modified src file
	assert.Equal(t, sT1, mTime(t, fSys, sFname))                       // check for untouched sFname
	assertEqualMTimes(t, fSys, sFname, cCache)                         // check for correct cCache
	assert.Equal(t, sT0, mTime(t, fSys, iCache))                       // check for correct iCache
}

func Test_6_00110_clean_on_invalid_cCache_valid_iCache_inserted_not_edited_file(t *testing.T) {
	defer setupCacheTest(t)()
	assertFileCreate(t, fSys, sFname, `trice(iD(999), "msg:value=%d\n", -1);`) // create inserted file
	sT0 := mTime(t, fSys, sFname)                                              // keep mtime
	id.CopyFileWithMTime(fSys, iCache, sFname)                                 // create valid iCache

	time.Sleep(100 * time.Millisecond)
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "clean", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-src", sFname}))
	time.Sleep(100 * time.Millisecond)

	assertFileContent(t, fSys, sFname, `trice("msg:value=%d\n", -1);`) // check src file
	assert.NotEqual(t, sT0, mTime(t, fSys, sFname))                    // check for touched sFname
	assertEqualMTimes(t, fSys, sFname, cCache)                         // check for correct cCache
	assert.Equal(t, sT0, mTime(t, fSys, iCache))                       // check for correct iCache
}

func Test_7_00111_clean_on_invalid_cCache_valid_iCache_inserted_edited_file(t *testing.T) {
	defer setupCacheTest(t)()
	assertFileCreate(t, fSys, sFname, `trice(iD(999), "msg:value=%d\n", -1);`) // create inserted file
	sT0 := mTime(t, fSys, sFname)                                              // keep mtime
	id.CopyFileWithMTime(fSys, iCache, sFname)                                 // create valid iCache

	assertFileCreate(t, fSys, sFname, `trice(iD(999),"msg:value=%d\n", -2);`) // "edit" src file
	sT1 := mTime(t, fSys, sFname)                                             // keep mtime

	time.Sleep(100 * time.Millisecond)
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "clean", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-src", sFname}))
	time.Sleep(100 * time.Millisecond)

	assertFileContent(t, fSys, sFname, `trice("msg:value=%d\n", -2);`) // check  src file
	assert.NotEqual(t, sT1, mTime(t, fSys, sFname))                    // check for touched sFname
	assertEqualMTimes(t, fSys, sFname, cCache)                         // check for correct cCache
	assert.Equal(t, sT0, mTime(t, fSys, iCache))                       // check for correct iCache
}

func Test_8_01000_clean_on_valid_cCache_invalid_iCache_cleaned_not_edited_file(t *testing.T) {
	defer setupCacheTest(t)()
	assertFileCreate(t, fSys, sFname, `trice("msg:value=%d\n", -1);`) // create src file
	sT0 := mTime(t, fSys, sFname)                                     // keep mtime
	id.CopyFileWithMTime(fSys, cCache, sFname)                        // create valid cCache

	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "clean", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-src", sFname}))

	assertFileContent(t, fSys, sFname, `trice("msg:value=%d\n", -1);`) // check for not modified src file
	assert.Equal(t, sT0, mTime(t, fSys, sFname))                       // check for untouched sFname
	assertEqualMTimes(t, fSys, sFname, cCache)                         // check for correct cCache
	assertFileNotExists(t, fSys, iCache)                               // check for not existing iCache
}

func Test_9_01001_clean_on_valid_cCache_invalid_iCache_cleaned_edited_file(t *testing.T) {
	defer setupCacheTest(t)()
	assertFileCreate(t, fSys, sFname, `trice("msg:value=%d\n", -1);`) // create src file
	id.CopyFileWithMTime(fSys, cCache, sFname)                        // create valid cCache

	assertFileCreate(t, fSys, sFname, `trice("msg:value=%d\n", -2);`) // "edit" src file
	time.Sleep(100 * time.Millisecond)

	sT1 := mTime(t, fSys, sFname) // keep mtime
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "clean", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-src", sFname}))

	assertFileContent(t, fSys, sFname, `trice("msg:value=%d\n", -2);`) // check for not modified src file
	assert.Equal(t, sT1, mTime(t, fSys, sFname))                       // check for untouched sFname
	assertEqualMTimes(t, fSys, sFname, cCache)                         // check for correct updated cCache
	assertFileNotExists(t, fSys, iCache)                               // check for not existing iCache
}

func Test_10_01011_clean_on_valid_cCache_invalid_iCache_inserted_not_edited_file(t *testing.T) {
	defer setupCacheTest(t)()
	assertFileCreate(t, fSys, sFname, `trice("msg:value=%d\n", -1);`) // create src file
	sT0 := mTime(t, fSys, sFname)                                     // keep mtime
	id.CopyFileWithMTime(fSys, cCache, sFname)                        // create valid cCache
	time.Sleep(100 * time.Millisecond)

	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "clean", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-src", sFname}))
	time.Sleep(100 * time.Millisecond)

	assertFileContent(t, fSys, sFname, `trice("msg:value=%d\n", -1);`) // check for unchanged src file
	assert.Equal(t, sT0, mTime(t, fSys, sFname))                       // check for untouched sFname
	assertEqualMTimes(t, fSys, sFname, cCache)                         // check for untouched cCache
	assertFileContent(t, fSys, cCache, `trice("msg:value=%d\n", -1);`) // check for correct cCache
	assertFileNotExists(t, fSys, iCache)                               // check for not existing iCache
}

func Test_11_01011_clean_on_valid_cCache_invalid_iCache_inserted_edited_file(t *testing.T) {
	defer setupCacheTest(t)()
	assertFileCreate(t, fSys, sFname, `trice("msg:value=%d\n", -1);`) // create src file
	id.CopyFileWithMTime(fSys, cCache, sFname)                        // create valid cCache
	time.Sleep(100 * time.Millisecond)

	assertFileCreate(t, fSys, sFname, `trice(iD(999), "msg:value=%d\n", -2);`) // inserted, edit file
	time.Sleep(100 * time.Millisecond)
	sT1 := mTime(t, fSys, sFname) // keep mtime
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "clean", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-src", sFname}))
	time.Sleep(100 * time.Millisecond)

	assertFileContent(t, fSys, sFname, `trice("msg:value=%d\n", -2);`) // check for cleaned src file
	assert.NotEqual(t, sT1, mTime(t, fSys, sFname))                    // check for touched sFname
	assertEqualMTimes(t, fSys, sFname, cCache)                         // check for correct updated cCache
	assertFileContent(t, fSys, cCache, `trice("msg:value=%d\n", -2);`) // check for correct cCache
	assertFileNotExists(t, fSys, iCache)                               // check for not existing iCache
}

func Test_14_01110_clean_on_valid_iCache_valid_cCache_inserted_file_not_edited(t *testing.T) {
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

func Test_12_01100_clean_on_valid_iCache_valid_cCache_clean_file_not_edited(t *testing.T) {
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

func Test_15_01111_clean_on_valid_iCache_valid_cCache_inserted_file_edited(t *testing.T) {
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

func Test_13_01101_clean_on_valid_iCache_valid_cCache_clean_file_edited(t *testing.T) {
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
