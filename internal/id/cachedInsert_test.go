package id_test

import (
	"bytes"
	"io"
	"os"
	"path/filepath"
	"testing"

	"github.com/rokath/trice/internal/args"
	"github.com/rokath/trice/internal/id"
	"github.com/spf13/afero"
	"github.com/tj/assert"
)

// TestCleanWithoutCacheFolder checks if no cache folder is created automatically.
func TestInsertWithoutCacheFolder(t *testing.T) {
	var err error
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer id.ResetGlobalVars(t)

	// create home and project folder
	home := "/home/tst/"
	assert.Nil(t, fSys.MkdirAll(home, os.ModeDir))
	proj := "/my/project/"
	assert.Nil(t, fSys.MkdirAll(proj, os.ModeDir))

	// create src file
	sFnm := proj + t.Name() + "_file.c"
	src := `trice("msg:value=%d\n", -1);`
	assert.Nil(t, fSys.WriteFile(sFnm, []byte(src), 0777))

	// create til.json and li.json
	id.FnJSON = proj + t.Name() + "_til.json"
	assert.Nil(t, fSys.WriteFile(id.FnJSON, nil, 0777))
	id.LIFnJSON = proj + t.Name() + "_li.json"
	assert.Nil(t, fSys.WriteFile(id.LIFnJSON, nil, 0777))

	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "insert", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-IDMin=999", "-IDMax=999", "-src", sFnm}))

	// check modified src file
	expSrc := `trice(iD(999), "msg:value=%d\n", -1);`

	actSrc, e := fSys.ReadFile(sFnm)
	assert.Nil(t, e)
	assert.Equal(t, expSrc, string(actSrc))

	// check for not existing cache
	cCache := filepath.Join(home, ".trice/cache/cleaned", sFnm)
	_, err = fSys.Stat(cCache)
	assert.Error(t, err)
	iCache := filepath.Join(home, ".trice/cache/inserted", sFnm)
	_, err = fSys.Stat(iCache)
	assert.Error(t, err)
}

func TestInsertWithCacheFolder(t *testing.T) {
	var err error
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer id.ResetGlobalVars(t)

	// create home, cache and project folder
	id.UserHomeDir = "/home/tst/" // overwrite global variable for this test
	cache := filepath.Join(id.UserHomeDir, ".trice/cache/")
	assert.Nil(t, fSys.MkdirAll(cache, os.ModeDir))
	proj := "/my/project/"
	assert.Nil(t, fSys.MkdirAll(proj, os.ModeDir))

	// create src file
	sFnm := proj + t.Name() + "_file.c"
	src := `trice("msg:value=%d\n", -1);`
	assert.Nil(t, fSys.WriteFile(sFnm, []byte(src), 0777))

	// create til.json and li.json
	id.FnJSON = proj + t.Name() + "_til.json"
	assert.Nil(t, fSys.WriteFile(id.FnJSON, nil, 0777))
	id.LIFnJSON = proj + t.Name() + "_li.json"
	assert.Nil(t, fSys.WriteFile(id.LIFnJSON, nil, 0777))

	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "insert", "-cache", "-til", id.FnJSON, "-li", id.LIFnJSON, "-IDMin", "999", "-IDMax", "999", "-src", sFnm}))

	// check modified src file
	expSrc := `trice(iD(999), "msg:value=%d\n", -1);`
	actSrc, e := fSys.ReadFile(sFnm)
	assert.Nil(t, e)
	assert.Equal(t, expSrc, string(actSrc))

	// check for not existing cCache
	cCache := filepath.Join(cache, "/cleaned", sFnm)
	_, err = fSys.Stat(cCache)
	assert.Error(t, err)
	// check for existing iCache
	iCache := filepath.Join(cache, "/inserted", sFnm)
	_, err = fSys.Stat(iCache)
	assert.Nil(t, err)
}
