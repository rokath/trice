package id_test

import (
	"testing"
	"time"

	"github.com/rokath/trice/internal/args"
	. "github.com/rokath/trice/internal/id"
	"github.com/tj/assert"
)

// TestCleanOnCleanedWithoutCacheFolder checks if no cache folder is created automatically.
func TestCleanOnCleanedWithoutCacheFolder(t *testing.T) {
	defer Setup(t)()                                                  // This executes Setup(t) and puts the returned function into the defer list.
	assertFileCreate(t, FSys, SFName, `trice("msg:value=%d\n", -1);`) // cleaned, edit

	FSys.RemoveAll(Cache) // no cache folder
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "clean", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-src", SFName}))

	assertFileContent(t, FSys, SFName, `trice("msg:value=%d\n", -1);`) // check file content
	assertFileNotExists(t, FSys, CCache)                               // check for not existing cCache
	assertFileNotExists(t, FSys, ICache)                               // check for not existing iCache
}

// TestCleanOnInsertedWithoutCacheFolder checks if no cache folder is created automatically.
func TestCleanOnInsertedWithoutCacheFolder(t *testing.T) {
	defer Setup(t)()                                                           // This executes Setup(t) and puts the returned function into the defer list.
	assertFileCreate(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -1);`) // inserted, edit

	FSys.RemoveAll(Cache)                                                                                                       // no cache folder                                                                                                                        // no cache folder
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "clean", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-src", SFName})) // trice clean

	assertFileContent(t, FSys, SFName, `trice("msg:value=%d\n", -1);`) // check file content
	assertFileNotExists(t, FSys, CCache)                               // check for not existing cCache
	assertFileNotExists(t, FSys, ICache)                               // check for not existing iCache
}

func Test_0_1_0000X_clean_on_invalid_cCache_invalid_iCache_cleaned_file(t *testing.T) {
	defer Setup(t)()                                                  // This executes Setup(t) and puts the returned function into the defer list.
	assertFileCreate(t, FSys, SFName, `trice("msg:value=%d\n", -1);`) // cleaned
	sT0 := mTime(t, FSys, SFName)                                     // keep mtime

	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "clean", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-src", SFName}))
	assertFileContent(t, FSys, SFName, `trice("msg:value=%d\n", -1);`) // check file content
	assertMTime(t, FSys, sT0, SFName)                                  // check for untouched sFname
	assertEqualMTimes(t, FSys, SFName, CCache)                         // check for correct updated cCache
	assertFileNotExists(t, FSys, ICache)                               // check for not existing iCache
}

func Test_2_3_0001X_clean_on_inalid_cCache_invalid_iCache_inserted_X_file(t *testing.T) {
	defer Setup(t)()                                                           // This executes Setup(t) and puts the returned function into the defer list.
	assertFileCreate(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -1);`) // inserted

	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "clean", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-src", SFName}))

	assertFileContent(t, FSys, SFName, `trice("msg:value=%d\n", -1);`) // check file content
	assertFileNotExists(t, FSys, ICache)                               // check for not existing iCache
	assertEqualMTimes(t, FSys, SFName, CCache)                         // check for correct updated cCache
}

func Test_4_5_0010X_clean_on_invalid_cCache_valid_iCache_cleaned_file(t *testing.T) {
	defer Setup(t)()                                                          // This executes Setup(t) and puts the returned function into the defer list.
	assertFileCreate(t, FSys, SFName, `trice(iD999), "msg:value=%d\n", -1);`) // create inserted file
	sT0 := mTime(t, FSys, SFName)                                             // keep mtime
	CopyFileWithMTime(FSys, ICache, SFName)                                   // create valid iCache
	assertFileCreate(t, FSys, SFName, `trice("msg:value=%d\n", -2);`)         // edit cleaned file
	sT1 := mTime(t, FSys, SFName)                                             // keep mtime

	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "clean", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-src", SFName}))

	assertFileContent(t, FSys, SFName, `trice("msg:value=%d\n", -2);`) // check for not modified src file
	assert.Equal(t, sT1, mTime(t, FSys, SFName))                       // check for untouched sFname
	assertEqualMTimes(t, FSys, SFName, CCache)                         // check for correct cCache
	assert.Equal(t, sT0, mTime(t, FSys, ICache))                       // check for correct iCache
}

func Test_6_00110_clean_on_invalid_cCache_valid_iCache_inserted_not_edited_file(t *testing.T) {
	defer Setup(t)()                                                           // This executes Setup(t) and puts the returned function into the defer list.
	assertFileCreate(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -1);`) // create inserted file
	sT0 := mTime(t, FSys, SFName)                                              // keep mtime
	CopyFileWithMTime(FSys, ICache, SFName)                                    // create valid iCache

	time.Sleep(100 * time.Millisecond)
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "clean", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-src", SFName}))
	time.Sleep(100 * time.Millisecond)

	assertFileContent(t, FSys, SFName, `trice("msg:value=%d\n", -1);`) // check src file
	assert.NotEqual(t, sT0, mTime(t, FSys, SFName))                    // check for touched sFname
	assertEqualMTimes(t, FSys, SFName, CCache)                         // check for correct cCache
	assert.Equal(t, sT0, mTime(t, FSys, ICache))                       // check for correct iCache
}

func Test_7_00111_clean_on_invalid_cCache_valid_iCache_inserted_edited_file(t *testing.T) {
	defer Setup(t)()                                                           // This executes Setup(t) and puts the returned function into the defer list.
	assertFileCreate(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -1);`) // create inserted file
	sT0 := mTime(t, FSys, SFName)                                              // keep mtime
	CopyFileWithMTime(FSys, ICache, SFName)                                    // create valid iCache

	assertFileCreate(t, FSys, SFName, `trice(iD(999),"msg:value=%d\n", -2);`) // "edit" src file
	sT1 := mTime(t, FSys, SFName)                                             // keep mtime

	time.Sleep(100 * time.Millisecond)
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "clean", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-src", SFName}))
	time.Sleep(100 * time.Millisecond)

	assertFileContent(t, FSys, SFName, `trice("msg:value=%d\n", -2);`) // check  src file
	assert.NotEqual(t, sT1, mTime(t, FSys, SFName))                    // check for touched sFname
	assertEqualMTimes(t, FSys, SFName, CCache)                         // check for correct cCache
	assert.Equal(t, sT0, mTime(t, FSys, ICache))                       // check for correct iCache
}

func Test_8_01000_clean_on_valid_cCache_invalid_iCache_cleaned_not_edited_file(t *testing.T) {
	defer Setup(t)()                                                  // This executes Setup(t) and puts the returned function into the defer list.
	assertFileCreate(t, FSys, SFName, `trice("msg:value=%d\n", -1);`) // create src file
	sT0 := mTime(t, FSys, SFName)                                     // keep mtime
	CopyFileWithMTime(FSys, CCache, SFName)                           // create valid cCache

	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "clean", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-src", SFName}))

	assertFileContent(t, FSys, SFName, `trice("msg:value=%d\n", -1);`) // check for not modified src file
	assert.Equal(t, sT0, mTime(t, FSys, SFName))                       // check for untouched sFname
	assertEqualMTimes(t, FSys, SFName, CCache)                         // check for correct cCache
	assertFileNotExists(t, FSys, ICache)                               // check for not existing iCache
}

func Test_9_01001_clean_on_valid_cCache_invalid_iCache_cleaned_edited_file(t *testing.T) {
	defer Setup(t)()                                                  // This executes Setup(t) and puts the returned function into the defer list.
	assertFileCreate(t, FSys, SFName, `trice("msg:value=%d\n", -1);`) // create src file
	CopyFileWithMTime(FSys, CCache, SFName)                           // create valid cCache

	assertFileCreate(t, FSys, SFName, `trice("msg:value=%d\n", -2);`) // "edit" src file
	time.Sleep(100 * time.Millisecond)

	sT1 := mTime(t, FSys, SFName) // keep mtime
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "clean", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-src", SFName}))

	assertFileContent(t, FSys, SFName, `trice("msg:value=%d\n", -2);`) // check for not modified src file
	assert.Equal(t, sT1, mTime(t, FSys, SFName))                       // check for untouched sFname
	assertEqualMTimes(t, FSys, SFName, CCache)                         // check for correct updated cCache
	assertFileNotExists(t, FSys, ICache)                               // check for not existing iCache
}

func Test_10_01011_clean_on_valid_cCache_invalid_iCache_inserted_not_edited_file(t *testing.T) {
	defer Setup(t)()                                                  // This executes Setup(t) and puts the returned function into the defer list.
	assertFileCreate(t, FSys, SFName, `trice("msg:value=%d\n", -1);`) // create src file
	sT0 := mTime(t, FSys, SFName)                                     // keep mtime
	CopyFileWithMTime(FSys, CCache, SFName)                           // create valid cCache
	time.Sleep(100 * time.Millisecond)

	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "clean", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-src", SFName}))
	time.Sleep(100 * time.Millisecond)

	assertFileContent(t, FSys, SFName, `trice("msg:value=%d\n", -1);`) // check for unchanged src file
	assert.Equal(t, sT0, mTime(t, FSys, SFName))                       // check for untouched sFname
	assertEqualMTimes(t, FSys, SFName, CCache)                         // check for untouched cCache
	assertFileContent(t, FSys, CCache, `trice("msg:value=%d\n", -1);`) // check for correct cCache
	assertFileNotExists(t, FSys, ICache)                               // check for not existing iCache
}

func Test_11_01011_clean_on_valid_cCache_invalid_iCache_inserted_edited_file(t *testing.T) {
	defer Setup(t)()                                                  // This executes Setup(t) and puts the returned function into the defer list.
	assertFileCreate(t, FSys, SFName, `trice("msg:value=%d\n", -1);`) // create src file
	CopyFileWithMTime(FSys, CCache, SFName)                           // create valid cCache
	time.Sleep(100 * time.Millisecond)

	assertFileCreate(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -2);`) // inserted, edit file
	time.Sleep(100 * time.Millisecond)
	sT1 := mTime(t, FSys, SFName) // keep mtime
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "clean", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-src", SFName}))
	time.Sleep(100 * time.Millisecond)

	assertFileContent(t, FSys, SFName, `trice("msg:value=%d\n", -2);`) // check for cleaned src file
	assert.NotEqual(t, sT1, mTime(t, FSys, SFName))                    // check for touched sFname
	assertEqualMTimes(t, FSys, SFName, CCache)                         // check for correct updated cCache
	assertFileContent(t, FSys, CCache, `trice("msg:value=%d\n", -2);`) // check for correct cCache
	assertFileNotExists(t, FSys, ICache)                               // check for not existing iCache
}

func Test_14_01110_clean_on_valid_iCache_valid_cCache_inserted_file_not_edited(t *testing.T) {
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	assertFileCreate(t, FSys, SFName, `trice("msg:value=%d\n", -1);`) // cleaned file
	assert.Nil(t, CopyFileWithMTime(FSys, CCache, SFName))            // not edited
	cT := mTime(t, FSys, CCache)

	for cT == mTime(t, FSys, SFName) { // Loop, to ensure the file mtimes are different.
		time.Sleep((10 * time.Microsecond))
		assertFileCreate(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -1);`) // inserted file
	}
	sT := mTime(t, FSys, SFName)
	assert.Nil(t, CopyFileWithMTime(FSys, ICache, SFName)) // not edited
	iT := mTime(t, FSys, ICache)

	assert.NotEqual(t, cT, iT)
	assertNotEqualMTimes(t, FSys, CCache, SFName) // check for not touched sFname
	assertEqualMTimes(t, FSys, ICache, SFName)    // check for equal iCache

	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "clean", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-src", SFName}))

	assertNotMTime(t, FSys, sT, SFName)        // check for touched sFname
	assertMTime(t, FSys, cT, CCache)           // check for untouched cCache
	assertMTime(t, FSys, iT, ICache)           // check for untouched iCache
	assertEqualMTimes(t, FSys, CCache, SFName) // check for valid cCache
}

func Test_12_01100_clean_on_valid_iCache_valid_cCache_clean_file_not_edited(t *testing.T) {
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	assertFileCreate(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -1);`) // inserted file
	assert.Nil(t, CopyFileWithMTime(FSys, ICache, SFName))                     // not edited
	iT := mTime(t, FSys, ICache)

	for iT == mTime(t, FSys, SFName) { // Loop, to ensure the file mtimes are different.
		time.Sleep((10 * time.Microsecond))
		assertFileCreate(t, FSys, SFName, ` "msg:value=%d\n", -1);`) // inserted file
	}

	assert.Nil(t, CopyFileWithMTime(FSys, CCache, SFName)) // not edited
	cT := mTime(t, FSys, CCache)

	sT := mTime(t, FSys, SFName)
	assert.NotEqual(t, cT, iT)
	assertNotEqualMTimes(t, FSys, CCache, ICache)

	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "clean", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-src", SFName}))
	assertMTime(t, FSys, sT, SFName)           // check for untouched sFname
	assertMTime(t, FSys, cT, CCache)           // check for untouched cCache
	assertMTime(t, FSys, iT, ICache)           // check for untouched iCache
	assertEqualMTimes(t, FSys, CCache, SFName) // check for valid cCache
}

func Test_15_01111_clean_on_valid_iCache_valid_cCache_inserted_file_edited(t *testing.T) {
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	assertFileCreate(t, FSys, SFName, `trice("msg:value=%d\n", -1);`) // cleaned file
	assert.Nil(t, CopyFileWithMTime(FSys, CCache, SFName))            // not edited
	cT := mTime(t, FSys, CCache)

	for cT == mTime(t, FSys, SFName) { // Loop, to ensure the file mtimes are different.
		time.Sleep((10 * time.Microsecond))
		assertFileCreate(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -1);`) // inserted file
	}
	assert.Nil(t, CopyFileWithMTime(FSys, ICache, SFName)) // not edited
	iT := mTime(t, FSys, ICache)
	assert.NotEqual(t, cT, iT)

	sT0 := mTime(t, FSys, SFName)
	// When editing, the old content is still buffered and not synced to disk, so we need to wait until the new mtime.
	sT := sT0
	for sT0 == sT { // Wait for the file system ...
		assertFileCreate(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -2);`) // edit file
		sT = mTime(t, FSys, SFName)
		time.Sleep(10 * time.Millisecond)
	}
	assertNotEqualMTimes(t, FSys, ICache, SFName)

	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "clean", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-src", SFName}))

	assertFileContent(t, FSys, SFName, `trice("msg:value=%d\n", -2);`) // check src file content
	assertNotMTime(t, FSys, sT, SFName)                                // check for touched sFname
	assertFileNotExists(t, FSys, ICache)                               // check for invalid iCache
	assertEqualMTimes(t, FSys, CCache, SFName)                         // check for valid cCache
}

func Test_13_01101_clean_on_valid_iCache_valid_cCache_clean_file_edited(t *testing.T) {
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	assertFileCreate(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -1);`) // inserted file
	assert.Nil(t, CopyFileWithMTime(FSys, ICache, SFName))                     // not edited
	iT := mTime(t, FSys, ICache)

	for iT == mTime(t, FSys, SFName) { // Loop, to ensure the file mtimes are different.
		time.Sleep((10 * time.Microsecond))
		assertFileCreate(t, FSys, SFName, ` "msg:value=%d\n", -1);`) // inserted file
	}

	assert.Nil(t, CopyFileWithMTime(FSys, CCache, SFName)) // not edited
	cT := mTime(t, FSys, CCache)
	assert.NotEqual(t, cT, iT)
	assertNotEqualMTimes(t, FSys, CCache, ICache)

	sT0 := mTime(t, FSys, SFName)
	// When editing, the old content is still buffered and not synced to disk, so we need to wait until the new mtime.
	sT := sT0
	for sT0 == sT { // Wait for the file system ...
		assertFileCreate(t, FSys, SFName, `trice("msg:value=%d\n", -2);`) // edit file
		sT = mTime(t, FSys, SFName)
		time.Sleep(10 * time.Millisecond)
	}
	assertNotEqualMTimes(t, FSys, ICache, SFName)

	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "clean", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-src", SFName}))

	assertFileContent(t, FSys, SFName, `trice("msg:value=%d\n", -2);`) // check modified src file
	assertMTime(t, FSys, sT, SFName)                                   // check for untouched sFname
	assertFileNotExists(t, FSys, ICache)                               // check for invalid iCache
	assertEqualMTimes(t, FSys, CCache, SFName)                         // check for valid cCache
}
