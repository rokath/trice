// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id_test

import (
	"fmt"
	"testing"

	"github.com/rokath/trice/internal/args"
	. "github.com/rokath/trice/internal/id"
	"github.com/tj/assert"
)

func TestCleanAndInsert(t *testing.T) {
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	// create src file1
	sFn1 := "file1.c"
	src1 := `			// line 1
		TRice(iD(992), "%x", 2 );	// line 2
		TRice(iD(993), "%x", 3 );	// line 3
		TRice(iD(994), "%x", 4 );	// line 4
		TRice(iD(995), "%x", 5 );	// line 5
		TRice(iD(996), "%x", 6 );	// line 6
		TRice(iD(997), "%x", 7 );	// line 7
		TRice(iD(998), "%x", 8 );	// line 8
		TRice(iD(999), "%x", 9 );	// line 9
	`
	assert.Nil(t, FSys.WriteFile(sFn1, []byte(src1), 0777))

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "clean", "-src", "file1.c", "-til", FnJSON, "-li", LIFnJSON}))

	// check modified src file1
	expSrc0 := `			// line 1
		TRice("%x", 2 );	// line 2
		TRice("%x", 3 );	// line 3
		TRice("%x", 4 );	// line 4
		TRice("%x", 5 );	// line 5
		TRice("%x", 6 );	// line 6
		TRice("%x", 7 );	// line 7
		TRice("%x", 8 );	// line 8
		TRice("%x", 9 );	// line 9
	`
	actSrc0, e := FSys.ReadFile(sFn1)
	assert.Nil(t, e)
	assert.Equal(t, expSrc0, string(actSrc0))

	fmt.Println("LIFnJSON=", LIFnJSON)

	li, e := FSys.ReadFile(LIFnJSON)
	assert.Nil(t, e)

	fmt.Println(string(li))

	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-src", "file1.c", "-til", FnJSON, "-li", LIFnJSON}))

	// check modified src file1
	expSrc1 := `			// line 1
		TRice(iD(992), "%x", 2 );	// line 2
		TRice(iD(993), "%x", 3 );	// line 3
		TRice(iD(994), "%x", 4 );	// line 4
		TRice(iD(995), "%x", 5 );	// line 5
		TRice(iD(996), "%x", 6 );	// line 6
		TRice(iD(997), "%x", 7 );	// line 7
		TRice(iD(998), "%x", 8 );	// line 8
		TRice(iD(999), "%x", 9 );	// line 9
	`
	actSrc1, e := FSys.ReadFile(sFn1)
	assert.Nil(t, e)
	assert.Equal(t, expSrc1, string(actSrc1))
}

func TestCleanAndInsertPackedTrices(t *testing.T) {
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	// create src file1
	sFn1 := "file1.c"
	src1 := `TRice(iD(2),"");
TRice(iD(3),"");
TRice(iD(4),"");
TRice(iD(5),"");
TRice(iD(6),"");
TRice(iD(7),"");
TRice(iD(8),"");
TRice(iD(9),"");`
	assert.Nil(t, FSys.WriteFile(sFn1, []byte(src1), 0777))

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "clean", "-src", "file1.c", "-til", FnJSON, "-li", LIFnJSON}))

	// check modified src file1
	expSrc0 := `TRice("");
TRice("");
TRice("");
TRice("");
TRice("");
TRice("");
TRice("");
TRice("");`
	actSrc0, e := FSys.ReadFile(sFn1)
	assert.Nil(t, e)
	assert.Equal(t, expSrc0, string(actSrc0))

	fmt.Println("LIFnJSON=", LIFnJSON)

	li, e := FSys.ReadFile(LIFnJSON)
	assert.Nil(t, e)

	fmt.Println(string(li))

	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-src", "file1.c", "-til", FnJSON, "-li", LIFnJSON}))

	// check modified src file1
	expSrc1 := `TRice(iD(2), "");
TRice(iD(3), "");
TRice(iD(4), "");
TRice(iD(5), "");
TRice(iD(6), "");
TRice(iD(7), "");
TRice(iD(8), "");
TRice(iD(9), "");`
	actSrc1, e := FSys.ReadFile(sFn1)
	assert.Nil(t, e)
	assert.Equal(t, expSrc1, string(actSrc1))
}

func TestCleanAndInsert2Files(t *testing.T) {
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	// create src file1
	sFn1 := "file1.c"
	src1 := `			// line 1
		TRice(iD(1992), "%x", 2 );	// line 2
		TRice(iD(1993), "%x", 3 );	// line 3
		TRice(iD(1994), "%x", 4 );	// line 4
		TRice(iD(1995), "%x", 5 );	// line 5
		TRice(iD(1996), "%x", 6 );	// line 6
		TRice(iD(1997), "%x", 7 );	// line 7
		TRice(iD(1998), "%x", 8 );	// line 8
		TRice(iD(1999), "%x", 9 );	// line 9
	`
	sFn2 := "file2.c"
	src2 := `			// line 1
		TRice(iD(2992), "%x", 2 );	// line 2
		TRice(iD(2993), "%x", 3 );	// line 3
		TRice(iD(2994), "%x", 4 );	// line 4
		TRice(iD(2995), "%x", 5 );	// line 5
		TRice(iD(2996), "%x", 6 );	// line 6
		TRice(iD(2997), "%x", 7 );	// line 7
		TRice(iD(2998), "%x", 8 );	// line 8
		TRice(iD(2999), "%x", 9 );	// line 9
	`
	assert.Nil(t, FSys.WriteFile(sFn1, []byte(src1), 0777))
	assert.Nil(t, FSys.WriteFile(sFn2, []byte(src2), 0777))

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "clean", "-src", "file1.c", "-src", "file2.c", "-til", FnJSON, "-li", LIFnJSON}))

	// check modified src file1
	expSrc10 := `			// line 1
		TRice("%x", 2 );	// line 2
		TRice("%x", 3 );	// line 3
		TRice("%x", 4 );	// line 4
		TRice("%x", 5 );	// line 5
		TRice("%x", 6 );	// line 6
		TRice("%x", 7 );	// line 7
		TRice("%x", 8 );	// line 8
		TRice("%x", 9 );	// line 9
	`
	actSrc10, e := FSys.ReadFile(sFn1)
	assert.Nil(t, e)
	assert.Equal(t, expSrc10, string(actSrc10))

	// check modified src file2
	expSrc20 := `			// line 1
		TRice("%x", 2 );	// line 2
		TRice("%x", 3 );	// line 3
		TRice("%x", 4 );	// line 4
		TRice("%x", 5 );	// line 5
		TRice("%x", 6 );	// line 6
		TRice("%x", 7 );	// line 7
		TRice("%x", 8 );	// line 8
		TRice("%x", 9 );	// line 9
	`
	actSrc20, e := FSys.ReadFile(sFn2)
	assert.Nil(t, e)
	assert.Equal(t, expSrc20, string(actSrc20))

	fmt.Println("LIFnJSON=", LIFnJSON)

	li, e := FSys.ReadFile(LIFnJSON)
	assert.Nil(t, e)

	fmt.Println(string(li))

	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-src", "file1.c", "-src", "file2.c", "-til", FnJSON, "-li", LIFnJSON}))

	// check modified src file1
	expSrc11 := `			// line 1
		TRice(iD(1992), "%x", 2 );	// line 2
		TRice(iD(1993), "%x", 3 );	// line 3
		TRice(iD(1994), "%x", 4 );	// line 4
		TRice(iD(1995), "%x", 5 );	// line 5
		TRice(iD(1996), "%x", 6 );	// line 6
		TRice(iD(1997), "%x", 7 );	// line 7
		TRice(iD(1998), "%x", 8 );	// line 8
		TRice(iD(1999), "%x", 9 );	// line 9
	`
	actSrc11, e := FSys.ReadFile(sFn1)
	assert.Nil(t, e)
	assert.Equal(t, expSrc11, string(actSrc11))

	// check modified src file2
	expSrc21 := `			// line 1
		TRice(iD(2992), "%x", 2 );	// line 2
		TRice(iD(2993), "%x", 3 );	// line 3
		TRice(iD(2994), "%x", 4 );	// line 4
		TRice(iD(2995), "%x", 5 );	// line 5
		TRice(iD(2996), "%x", 6 );	// line 6
		TRice(iD(2997), "%x", 7 );	// line 7
		TRice(iD(2998), "%x", 8 );	// line 8
		TRice(iD(2999), "%x", 9 );	// line 9
	`
	actSrc21, e := FSys.ReadFile(sFn2)
	assert.Nil(t, e)
	assert.Equal(t, expSrc21, string(actSrc21))
}
