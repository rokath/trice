package id_test

import (
	"testing"
	"time"

	"github.com/rokath/trice/internal/args"
	. "github.com/rokath/trice/internal/id"
	"github.com/tj/assert"
)

// TestInsertOnCleanedWithoutCacheFolder checks if no cache folder is created automatically.
func TestInsertOnCleanedWithoutCacheFolder(t *testing.T) {
	defer Setup(t)()                                                  // This executes Setup(t) and puts the returned function into the defer list.
	assertFileCreate(t, FSys, SFName, `trice("msg:value=%d\n", -1);`) // cleaned, edit

	FSys.RemoveAll(Cache) // no cache folder
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-IDMin=999", "-IDMax=999", "-src", SFName}))

	assertFileContent(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -1);`) // check file content
	assertFileNotExists(t, FSys, CCache)                                        // check for not existing cCache
	assertFileNotExists(t, FSys, ICache)                                        // check for not existing iCache
}

// TestInsertOnInsertedWithoutCacheFolder checks if no cache folder is created automatically.
func TestInsertOnInsertedWithoutCacheFolder(t *testing.T) {
	defer Setup(t)()                                                           // This executes Setup(t) and puts the returned function into the defer list.
	assertFileCreate(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -1);`) // inserted, edit

	FSys.RemoveAll(Cache) // no cache folder
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-IDMin=999", "-IDMax=999", "-src", SFName}))

	assertFileContent(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -1);`) // check file content
	assertFileNotExists(t, FSys, CCache)                                        // check for not existing cCache
	assertFileNotExists(t, FSys, ICache)                                        // check for not existing iCache
}

func Test_16_17_1000X_insert_on_invalid_cCache_invalid_iCache_cleaned_file(t *testing.T) {
	defer Setup(t)()                                                  // This executes Setup(t) and puts the returned function into the defer list.
	assertFileCreate(t, FSys, SFName, `trice("msg:value=%d\n", -1);`) // cleaned, edit
	sT0 := mTime(t, FSys, SFName)                                     // keep mtime
	time.Sleep(100 * time.Millisecond)
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-IDMin", "999", "-IDMax", "999", "-src", SFName}))
	assertFileContent(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -1);`) // check file content
	assertNotMTime(t, FSys, sT0, SFName)                                        // check for touched sFname
	assertFileNotExists(t, FSys, CCache)                                        // check for not existing cCache
	assertEqualMTimes(t, FSys, SFName, ICache)                                  // check for correct updated iCache
}

func Test_18_19_1001X_insert_on_invalid_cCache_invalid_iCache_inserted_edited_file(t *testing.T) {
	defer Setup(t)()                                                           // This executes Setup(t) and puts the returned function into the defer list.
	assertFileCreate(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -1);`) // inserted, edit
	sT0 := mTime(t, FSys, SFName)                                              // keep mtime

	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-IDMin", "999", "-IDMax", "999", "-src", SFName}))

	assertFileContent(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -1);`) // check file content
	assertMTime(t, FSys, sT0, SFName)                                           // check for untouched sFname
	assertFileNotExists(t, FSys, CCache)                                        // check for not existing cCache
	assertEqualMTimes(t, FSys, SFName, ICache)                                  // check for correct updated iCache
}

func Test_20_21_1010X_insert_on_invalid_cCache_valid_iCache_cleaned_file(t *testing.T) {
	defer Setup(t)()                                                           // This executes Setup(t) and puts the returned function into the defer list.
	assertFileCreate(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -1);`) // create inserted file
	CopyFileWithMTime(FSys, ICache, SFName)                                    // create valid iCache
	iT0 := mTime(t, FSys, ICache)                                              // keep mtime
	time.Sleep(100 * time.Millisecond)                                         // This time is needed to sync sFname to disc before changing it again.
	assertFileCreate(t, FSys, SFName, `trice("msg:value=%d\n", -1);`)          // create cleaned file
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-IDMin", "999", "-IDMax", "999", "-src", SFName}))

	assertFileContent(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -1);`) // check for modified src file
	assertFileContent(t, FSys, ICache, `trice(iD(999), "msg:value=%d\n", -1);`) // check for unchanged iCache
	assertFileNotExists(t, FSys, CCache)                                        // check for not existing cCache
	assert.NotEqual(t, iT0, mTime(t, FSys, ICache))                             // check for updated iCache, because the file is edited, the iCache was invalidated and re-created
}

func Test_22_10100_insert_on_invalid_cCache_valid_iCache_inserted_not_edited_file(t *testing.T) {
	defer Setup(t)()                                                           // This executes Setup(t) and puts the returned function into the defer list.
	assertFileCreate(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -1);`) // create inserted file
	CopyFileWithMTime(FSys, ICache, SFName)                                    // create valid iCache
	iT0 := mTime(t, FSys, ICache)                                              // keep mtime
	time.Sleep(100 * time.Millisecond)                                         // This time is needed to sync sFname to disc before changing it again.

	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-IDMin", "999", "-IDMax", "999", "-src", SFName}))

	assertFileContent(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -1);`) // check for modified src file
	assertFileContent(t, FSys, ICache, `trice(iD(999), "msg:value=%d\n", -1);`) // check for unchanged iCache
	assertFileNotExists(t, FSys, CCache)                                        // check for not existing cCache
	assert.Equal(t, iT0, mTime(t, FSys, ICache))                                // check for untouched iCache
}

func Test_23_10101_insert_on_invalid_cCache_valid_iCache_inserted_edited_file(t *testing.T) {
	defer Setup(t)()                                                           // This executes Setup(t) and puts the returned function into the defer list.
	assertFileCreate(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -1);`) // create inserted file
	CopyFileWithMTime(FSys, ICache, SFName)                                    // create valid iCache
	iT0 := mTime(t, FSys, ICache)                                              // keep mtime
	time.Sleep(100 * time.Millisecond)                                         // This time is needed to sync sFname to disc before changing it again.
	assertFileCreate(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -2);`) // edit file
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-IDMin", "999", "-IDMax", "999", "-src", SFName}))

	assertFileContent(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -2);`) // check for modified src file
	assertFileContent(t, FSys, ICache, `trice(iD(999), "msg:value=%d\n", -2);`) // check for changed iCache
	assertFileNotExists(t, FSys, CCache)                                        // check for not existing cCache
	assert.NotEqual(t, iT0, mTime(t, FSys, ICache))                             // check for updated iCache, because the file is edited, the iCache was invalidated and re-created
}

func Test_24_11000_insert_on_valid_cCache_invalid_iCache_cleaned_not_edited_file(t *testing.T) {
	defer Setup(t)()                                                  // This executes Setup(t) and puts the returned function into the defer list.
	assertFileCreate(t, FSys, SFName, `trice("msg:value=%d\n", -1);`) // create src file
	sT0 := mTime(t, FSys, SFName)                                     // keep mtime
	CopyFileWithMTime(FSys, CCache, SFName)                           // create valid cCache
	time.Sleep(100 * time.Millisecond)                                // This time is needed to sync to disc before changing it again.

	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-IDMin", "999", "-IDMax", "999", "-src", SFName}))

	assertFileContent(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -1);`) // check for modified src file
	assert.NotEqual(t, sT0, mTime(t, FSys, SFName))                             // check for touched sFname
	assert.Equal(t, sT0, mTime(t, FSys, CCache))                                // check for untouched cCache
	assertFileContent(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -1);`) // check for correct iCache
	assertEqualMTimes(t, FSys, SFName, ICache)                                  // check for correct iCache
}

func Test_25_11001_insert_on_valid_cCache_invalid_iCache_cleaned_edited_file(t *testing.T) {
	defer Setup(t)()                                                  // This executes Setup(t) and puts the returned function into the defer list.
	assertFileCreate(t, FSys, SFName, `trice("msg:value=%d\n", -1);`) // create src file
	sT0 := mTime(t, FSys, SFName)                                     // keep mtime
	CopyFileWithMTime(FSys, CCache, SFName)                           // create valid cCache
	assertFileCreate(t, FSys, SFName, `trice("msg:value=%d\n", -2);`) // edit src file
	//	sT1 := mTime(t, FSys, sFname)                                     // keep mtime
	time.Sleep(100 * time.Millisecond) // This time is needed to sync to disc before changing it again.

	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-IDMin", "999", "-IDMax", "999", "-src", SFName}))

	assertFileContent(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -2);`) // check for modified src file
	assert.NotEqual(t, sT0, mTime(t, FSys, SFName))                             // check for touched sFname
	assertMTime(t, FSys, sT0, CCache)                                           // check for untouched cCache
	assertFileContent(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -2);`) // check for correct iCache
	assertEqualMTimes(t, FSys, SFName, ICache)                                  // check for correct iCache
}

func Test_26_27_1010X_insert_on_invalid_cCache_valid_iCache_cleaned_file(t *testing.T) {
	defer Setup(t)()                                                           // This executes Setup(t) and puts the returned function into the defer list.
	assertFileCreate(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -1);`) // create inserted file
	CopyFileWithMTime(FSys, ICache, SFName)                                    // create valid iCache
	time.Sleep(100 * time.Millisecond)                                         // This time is needed to sync to disc
	iT0 := mTime(t, FSys, ICache)                                              // keep mtime
	time.Sleep(100 * time.Millisecond)                                         // This time is needed to sync sFname to disc before changing it again.
	assertFileCreate(t, FSys, SFName, `trice( "msg:value=%d\n", -1);`)         // create cleaned file

	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-IDMin", "999", "-IDMax", "999", "-src", SFName}))

	assertFileContent(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -1);`) // check for modified src file
	assertFileContent(t, FSys, ICache, `trice(iD(999), "msg:value=%d\n", -1);`) // check for unchanged iCache
	assertFileNotExists(t, FSys, CCache)                                        // check for not existing cCache
	assert.NotEqual(t, iT0, mTime(t, FSys, ICache))                             // check for touched iCache
}

func Test_28_11100_insert_on_valid_cCache_valid_iCache_cleaned_not_edited_file(t *testing.T) {
	defer Setup(t)()                                                           // This executes Setup(t) and puts the returned function into the defer list.
	assertFileCreate(t, FSys, SFName, `trice("msg:value=%d\n", -1);`)          // create cleaned file
	CopyFileWithMTime(FSys, CCache, SFName)                                    // create valid cCache
	time.Sleep(100 * time.Millisecond)                                         // This time is needed to sync to disc to ensure mtime of cCache and iCache are not equal.
	assertFileCreate(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -1);`) // create inserted file
	CopyFileWithMTime(FSys, ICache, SFName)                                    // create valid iCache

	CopyFileWithMTime(FSys, SFName, CCache) // create cleaned not edited file

	cT0 := mTime(t, FSys, CCache) // keep mtime
	iT0 := mTime(t, FSys, ICache) // keep mtime

	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-IDMin", "999", "-IDMax", "999", "-src", SFName}))

	assertFileContent(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -1);`) // check for modified src file
	assert.Equal(t, iT0, mTime(t, FSys, ICache))                                // check for equal mtime with iCache
	assertFileContent(t, FSys, CCache, `trice("msg:value=%d\n", -1);`)          // check for unchanged cCache
	assert.Equal(t, cT0, mTime(t, FSys, CCache))                                // check for touched iCache
	assertFileContent(t, FSys, ICache, `trice(iD(999), "msg:value=%d\n", -1);`) // check for unchanged iCache
	assert.Equal(t, iT0, mTime(t, FSys, ICache))                                // check for touched iCache
}

func Test_29_11100_insert_on_valid_cCache_valid_iCache_cleaned_edited_file(t *testing.T) {
	defer Setup(t)()                                                           // This executes Setup(t) and puts the returned function into the defer list.
	assertFileCreate(t, FSys, SFName, `trice("msg:value=%d\n", -1);`)          // create cleaned file
	CopyFileWithMTime(FSys, CCache, SFName)                                    // create valid cCache
	time.Sleep(100 * time.Millisecond)                                         // This time is needed to sync to disc to ensure mtime of cCache and iCache are not equal.
	assertFileCreate(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -1);`) // create inserted file
	CopyFileWithMTime(FSys, ICache, SFName)                                    // create valid iCache

	time.Sleep(100 * time.Millisecond)                                // This time is needed to sync to disc to ensure mtime of cCache and iCache are not equal.
	assertFileCreate(t, FSys, SFName, `trice("msg:value=%d\n", -2);`) // create cleaned edited file
	time.Sleep(100 * time.Millisecond)                                // This time is needed to sync to disc to ensure mtime of cCache and iCache are not equal.

	iT0 := mTime(t, FSys, ICache) // keep mtime

	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-IDMin", "999", "-IDMax", "999", "-src", SFName}))

	assertFileContent(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -2);`) // check for modified src file
	assertFileContent(t, FSys, ICache, `trice(iD(999), "msg:value=%d\n", -2);`) // check for changed iCache
	assert.NotEqual(t, iT0, mTime(t, FSys, ICache))                             // check for touched iCache
	assertFileNotExists(t, FSys, CCache)                                        // check for invalid cCache
}

func Test_30_11110_insert_on_valid_cCache_valid_iCache_inserted_not_edited_file(t *testing.T) {
	defer Setup(t)()                                                           // This executes Setup(t) and puts the returned function into the defer list.
	assertFileCreate(t, FSys, SFName, `trice("msg:value=%d\n", -1);`)          // create cleaned file
	CopyFileWithMTime(FSys, CCache, SFName)                                    // create valid cCache
	time.Sleep(100 * time.Millisecond)                                         // This time is needed to sync to disc to ensure mtime of cCache and iCache are not equal.
	assertFileCreate(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -1);`) // create inserted file
	CopyFileWithMTime(FSys, ICache, SFName)                                    // create valid iCache

	CopyFileWithMTime(FSys, SFName, CCache) // create cleaned not edited file

	cT0 := mTime(t, FSys, CCache) // keep mtime
	iT0 := mTime(t, FSys, ICache) // keep mtime

	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-IDMin", "999", "-IDMax", "999", "-src", SFName}))

	assertFileContent(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -1);`) // check for modified src file
	assert.Equal(t, iT0, mTime(t, FSys, ICache))                                // check for equal mtime with iCache
	assertFileContent(t, FSys, CCache, `trice("msg:value=%d\n", -1);`)          // check for unchanged cCache
	assert.Equal(t, cT0, mTime(t, FSys, CCache))                                // check for touched iCache
	assertFileContent(t, FSys, ICache, `trice(iD(999), "msg:value=%d\n", -1);`) // check for unchanged iCache
	assert.Equal(t, iT0, mTime(t, FSys, ICache))                                // check for touched iCache
}

func Test_31_11111_insert_on_valid_cCache_valid_iCache_inserted_edited_file(t *testing.T) {
	defer Setup(t)()                                                           // This executes Setup(t) and puts the returned function into the defer list.
	assertFileCreate(t, FSys, SFName, `trice("msg:value=%d\n", -1);`)          // create cleaned file
	CopyFileWithMTime(FSys, CCache, SFName)                                    // create valid cCache
	time.Sleep(100 * time.Millisecond)                                         // This time is needed to sync to disc to ensure mtime of cCache and iCache are not equal.
	assertFileCreate(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -1);`) // create inserted file
	CopyFileWithMTime(FSys, ICache, SFName)                                    // create valid iCache

	time.Sleep(100 * time.Millisecond)                                         // This time is needed to sync to disc to ensure mtime of cCache and iCache are not equal.
	assertFileCreate(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -2);`) // create inserted edited file
	time.Sleep(100 * time.Millisecond)                                         // This time is needed to sync to disc to ensure mtime of cCache and iCache are not equal.

	iT0 := mTime(t, FSys, ICache) // keep mtime

	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-IDMin", "999", "-IDMax", "999", "-src", SFName}))

	assertFileContent(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -2);`) // check for modified src file
	assertFileContent(t, FSys, ICache, `trice(iD(999), "msg:value=%d\n", -2);`) // check for changed iCache
	assert.NotEqual(t, iT0, mTime(t, FSys, ICache))                             // check for touched iCache
	assertFileNotExists(t, FSys, CCache)                                        // check for invalid cCache
}

// Legacy tests

func TestInsertOnInsertedFileWithValidCCache(t *testing.T) {
	defer Setup(t)()                                                           // This executes Setup(t) and puts the returned function into the defer list.
	assertFileCreate(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -1);`) // create src file
	sFNameMTime := mTime(t, FSys, SFName)                                      // keep mtime

	// status: invalid cCache && invalid iCache here && file edited
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-IDMin", "999", "-IDMax", "999", "-src", SFName})) // trice insert first time

	// status: invalid cCache && valid iCache here && file NOT edited
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-IDMin", "999", "-IDMax", "999", "-src", SFName})) // trice insert second time

	assertFileContent(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -1);`)
	assert.Equal(t, sFNameMTime, mTime(t, FSys, SFName)) // check for untouched sFname
	assertEqualMTimes(t, FSys, SFName, ICache)           // check for correct updated iCache
	assertFileNotExists(t, FSys, CCache)                 // check for not existing cCache

	assertFileCreate(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -2);`)                                                                                     // "edit" src file
	sFNameMTime = mTime(t, FSys, SFName)                                                                                                                           // keep mtime
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-IDMin", "999", "-IDMax", "999", "-src", SFName})) // trice insert again

	assertFileContent(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -2);`) // check for not modified src file
	assert.Equal(t, sFNameMTime, mTime(t, FSys, SFName))                        // check for untouched sFname
	assertEqualMTimes(t, FSys, SFName, ICache)                                  // check for correct updated iCache
	assertFileNotExists(t, FSys, CCache)                                        // check for not existing cCache
}

func TestInsert_On_valid_iCache_valid_cCache_inserted_file_not_edited(t *testing.T) {
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

	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-IDMin", "999", "-IDMax", "999", "-src", SFName}))

	assertMTime(t, FSys, sT, SFName)           // check for untouched sFname
	assertMTime(t, FSys, cT, CCache)           // check for untouched cCache
	assertMTime(t, FSys, iT, ICache)           // check for untouched iCache
	assertEqualMTimes(t, FSys, ICache, SFName) // check for valid iCache
}

func TestInsert_On_valid_iCache_valid_cCache_clean_file_not_edited(t *testing.T) {
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	assertFileCreate(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -1);`) // inserted file
	assert.Nil(t, CopyFileWithMTime(FSys, ICache, SFName))                     // not edited
	iT := mTime(t, FSys, ICache)

	for iT == mTime(t, FSys, SFName) { // Loop, to ensure the file mtimes are different.
		time.Sleep((10 * time.Microsecond))
		assertFileCreate(t, FSys, SFName, `trice("msg:value=%d\n", -1);`) // inserted file
	}

	assert.Nil(t, CopyFileWithMTime(FSys, CCache, SFName)) // not edited
	cT := mTime(t, FSys, CCache)

	sT := mTime(t, FSys, SFName)
	assert.NotEqual(t, cT, iT)
	assertNotEqualMTimes(t, FSys, CCache, ICache)

	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-IDMin", "999", "-IDMax", "999", "-src", SFName}))
	assertNotMTime(t, FSys, sT, SFName)        // check for touched sFname
	assertMTime(t, FSys, cT, CCache)           // check for untouched cCache
	assertMTime(t, FSys, iT, ICache)           // check for untouched iCache
	assertEqualMTimes(t, FSys, ICache, SFName) // check for valid iCache
}

func TestInsert_On_valid_iCache_valid_cCache_inserted_file_edited(t *testing.T) {
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

	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-IDMin", "999", "-IDMax", "999", "-src", SFName}))

	assertFileContent(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -2);`) // check src file content
	assertMTime(t, FSys, sT, SFName)                                            // check for untouched sFname
	assertFileNotExists(t, FSys, CCache)                                        // check for invalid cCache
	assertNotMTime(t, FSys, iT, ICache)                                         // check for touched iCache
}

func TestInsert_On_valid_iCache_valid_cCache_clean_file_edited(t *testing.T) {
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	assertFileCreate(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -1);`) // inserted file
	assert.Nil(t, CopyFileWithMTime(FSys, ICache, SFName))                     // not edited
	iT := mTime(t, FSys, ICache)

	for iT == mTime(t, FSys, SFName) { // Loop, to ensure the file mtimes are different.
		time.Sleep((10 * time.Microsecond))
		assertFileCreate(t, FSys, SFName, `trice("msg:value=%d\n", -1);`) // inserted file
	}

	assert.Nil(t, CopyFileWithMTime(FSys, CCache, SFName)) // not edited
	cT := mTime(t, FSys, CCache)
	assert.NotEqual(t, cT, iT)
	assertNotEqualMTimes(t, FSys, CCache, ICache)

	sT0 := mTime(t, FSys, SFName)
	// When editing, the old content is still buffered and not synced to disk, so we need to wait until the new mtime.
	sT := sT0
	for sT0 == sT { // Wait for the file system ...
		assertFileCreate(t, FSys, SFName, `trice(i"msg:value=%d\n", -2);`) // edit file
		sT = mTime(t, FSys, SFName)
		time.Sleep(10 * time.Millisecond)
	}
	assertNotEqualMTimes(t, FSys, ICache, SFName)

	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-cache", "-til", FnJSON, "-li", LIFnJSON, "-IDMin", "999", "-IDMax", "999", "-src", SFName}))

	assertFileContent(t, FSys, SFName, `trice(iD(999), "msg:value=%d\n", -2);`) // check modified src file
	assertNotMTime(t, FSys, sT, SFName)                                         // check for touched sFname
	assertFileNotExists(t, FSys, CCache)                                        // check for invalid cCache
	assertEqualMTimes(t, FSys, ICache, SFName)                                  // check for valid iCache
}
