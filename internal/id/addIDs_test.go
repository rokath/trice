// SPDX-License-Identifier: MIT

package id_test

import (
	"path/filepath"
	"testing"

	"github.com/rokath/trice/internal/args"
	. "github.com/rokath/trice/internal/id"
	"github.com/stretchr/testify/assert"
)

// TestAddWithLIExtension verifies the expected behavior.
func TestAddWithLIExtension(t *testing.T) {
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	// create non-empty li.json
	li := `{
	"16200": {
		"File": "stm32f0xx_it.c",
		"Line": 160
	},
	"16202": {
		"File": "main.c",
		"Line": 71
	}
}`
	assert.Nil(t, FSys.WriteFile(LIFnJSON, []byte(li), 0777))

	// create src file
	sFn := t.Name() + "file.c"
	src := `break; case __LINE__: trice( iD(999), "msg:value=%d\n", -1  );`
	assert.Nil(t, FSys.WriteFile(sFn, []byte(src), 0777))

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "add", "-v", "-src", sFn, "-til", FnJSON, "-li", LIFnJSON}))

	// check un-modified src file
	expSrc := src
	actSrc, e := FSys.ReadFile(sFn)
	assert.Nil(t, e)
	assert.Equal(t, expSrc, string(actSrc))

	// check extended location indormation file
	expLI := `{
	"16200": {
		"File": "stm32f0xx_it.c",
		"Line": 160
	},
	"16202": {
		"File": "main.c",
		"Line": 71
	},
	"999": {
		"File": "TestAddWithLIExtensionfile.c",
		"Line": 1
	}
}`
	actLI, e := FSys.ReadFile(LIFnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expLI, string(actLI))
}

// TestToLIPath verifies the expected behavior.
func TestToLIPath(t *testing.T) {
	defer Setup(t)()

	LIPathKind = "base"
	assert.Equal(t, "demo.c", ToLIPath(filepath.Join("dir", "demo.c")))

	LIPathKind = filepath.Join("root", "relative")
	assert.Equal(t, filepath.ToSlash(filepath.Join("sub", "demo.c")), ToLIPath(filepath.Join("root", "sub", "demo.c")))

	LIPathKind = "full"
	got := ToLIPath("demo.c")
	assert.True(t, filepath.IsAbs(got))
	assert.Equal(t, "demo.c", filepath.Base(got))

	LIPathKind = "weird"
	assert.Equal(t, "demo.c", ToLIPath(filepath.Join("dir", "demo.c")))
}
