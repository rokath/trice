// Copyright 2020 Thomas.Hoehenleitner [at] seerose.netid_test
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id_test

import (
	"fmt"
	"testing"

	"github.com/rokath/trice/internal/args"
	. "github.com/rokath/trice/internal/id"
	"github.com/tj/assert"
)

func TestClean(t *testing.T) {
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	// create src file
	sFn := t.Name() + "file.c"
	src := `break; case __LINE__: trice(iD(999), "msg:value=%d\n", -1  );`
	assert.Nil(t, FSys.WriteFile(sFn, []byte(src), 0777))

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "clean", "-src", sFn, "-til", FnJSON, "-li", LIFnJSON}))

	// check modified src file
	expSrc := `break; case __LINE__: trice("msg:value=%d\n", -1  );`

	actSrc, e := FSys.ReadFile(sFn)
	assert.Nil(t, e)

	assert.Equal(t, expSrc, string(actSrc))
}

func TestCleanWithSpace(t *testing.T) {
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	// create src file
	sFn := t.Name() + "file.c"
	src := `break; case __LINE__: trice(iD(999), "msg:value=%d\n", -1);`
	assert.Nil(t, FSys.WriteFile(sFn, []byte(src), 0777))

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "clean", "-spaceInsideParenthesis", "-src", sFn, "-til", FnJSON, "-li", LIFnJSON}))

	// check modified src file
	expSrc := `break; case __LINE__: trice( "msg:value=%d\n", -1);`

	actSrc, e := FSys.ReadFile(sFn)
	assert.Nil(t, e)

	assert.Equal(t, expSrc, string(actSrc))
}

func TestCleanWithSpace2(t *testing.T) {
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	// create src file
	sFn := t.Name() + "file.c"
	src := `break; case __LINE__: trice(iD(999), "msg:value=%d\n", -1  );`
	assert.Nil(t, FSys.WriteFile(sFn, []byte(src), 0777))

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "clean", "-w", "-src", sFn, "-til", FnJSON, "-li", LIFnJSON}))

	// check modified src file
	expSrc := `break; case __LINE__: trice( "msg:value=%d\n", -1  );`

	actSrc, e := FSys.ReadFile(sFn)
	assert.Nil(t, e)

	assert.Equal(t, expSrc, string(actSrc))
}

func TestCleanWithLIExtension(t *testing.T) {
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
	src := `break; case __LINE__: trice(iD(999), "msg:value=%d\n", -1  );`
	assert.Nil(t, FSys.WriteFile(sFn, []byte(src), 0777))

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "clean", "-src", sFn, "-til", FnJSON, "-li", LIFnJSON}))

	// check modified src file
	expSrc := `break; case __LINE__: trice("msg:value=%d\n", -1  );`

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
		"File": "TestCleanWithLIExtensionfile.c",
		"Line": 1
	}
}`
	actLI, e := FSys.ReadFile(LIFnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expLI, string(actLI))
}

// TestCleanTriceALias is with the alias functionality and a check for the json files.
func TestCleanTriceALias(t *testing.T) {
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	// create existing li.json file
	exsLI := `{
	"55": {
		"File": "file1.c",
		"Line": 3
	},
	"66": {
		"File": "file1.c",
		"Line": 4
	},
	"77": {
		"File": "file1.c",
		"Line": 2
	},
	"999": {
		"File": "fileX.c",
		"Line": 2
	}
}`
	assert.Nil(t, FSys.WriteFile(LIFnJSON, []byte(exsLI), 0777))

	// create existing til.json file
	exsTIL := `{
	"55": {
		"Type": "trice",
		"Strg": "msg:value=%d\\n"
	},
	"66": {
		"Type": "triceS",
		"Strg": "` + SAliasStrgPrefix + `0 == 1, \"that is wrong\"` + SAliasStrgSuffix + `"
	},
	"77": {
		"Type": "trice",
		"Strg": "%x"
	}
}`
	assert.Nil(t, FSys.WriteFile(FnJSON, []byte(exsTIL), 0777))

	// create src file1
	src1 := `
	printk(iD(77), "%x", 123 );
	log(iD(55), "msg:value=%d\n", 123 );
	AST(iD(66), 0 == 1, "that is wrong" )
	`
	assert.Nil(t, FSys.WriteFile("file1.c", []byte(src1), 0777))

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "clean", "-alias", "log", "-alias", "printk", "-salias", "AST", "-src", "file1.c", "-til", FnJSON, "-li", LIFnJSON}))

	// check modified src file1
	expSrc1 := `
	printk("%x", 123 );
	log("msg:value=%d\n", 123 );
	AST(0 == 1, "that is wrong" )
	`

	fmt.Println(B.String())
	//assert.Equal(t, B.String(), "")

	actSrc1, e := FSys.ReadFile("file1.c")
	assert.Nil(t, e)
	assert.Equal(t, expSrc1, string(actSrc1))

	// check til.json
	actTIL, e := FSys.ReadFile(FnJSON)
	assert.Nil(t, e)
	assert.Equal(t, exsTIL, string(actTIL))

	// check li.json
	actLI, e := FSys.ReadFile(LIFnJSON)
	assert.Nil(t, e)
	assert.Equal(t, exsLI, string(actLI))

	// cleanup
	FSys.Remove(FnJSON)
	FSys.Remove(LIFnJSON)
	FSys.RemoveAll(UserHomeDir)
}
