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

func _TestCleanAndInsert(t *testing.T) {
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	// create src file1
	sFn1 := "file1.c"
	src1 := `
	TRice(iD(992), "%x", 2 ); 
	TRice(iD(993), "%x", 3 ); 
	TRice(iD(994), "%x", 4 ); 
	TRice(iD(995), "%x", 5 ); 
	TRice(iD(996), "%x", 6 ); 
	TRice(iD(997), "%x", 7 ); 
	TRice(iD(998), "%x", 8 ); 
	TRice(iD(999), "%x", 9 ); 
	`
	assert.Nil(t, FSys.WriteFile(sFn1, []byte(src1), 0777))

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "clean", "-src", "file1.c", "-til", FnJSON, "-li", LIFnJSON}))

	// check modified src file1
	expSrc0 := `
	TRice("%x", 2 ); 
	TRice("%x", 3 ); 
	TRice("%x", 4 ); 
	TRice("%x", 5 ); 
	TRice("%x", 6 ); 
	TRice("%x", 7 ); 
	TRice("%x", 8 ); 
	TRice("%x", 9 ); 
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
	expSrc1 := `
	TRice(iD(992), "%x", 2 ); 
	TRice(iD(993), "%x", 3 ); 
	TRice(iD(994), "%x", 4 ); 
	TRice(iD(995), "%x", 5 ); 
	TRice(iD(996), "%x", 6 ); 
	TRice(iD(997), "%x", 7 ); 
	TRice(iD(998), "%x", 8 ); 
	TRice(iD(999), "%x", 9 ); 
	`
	actSrc1, e := FSys.ReadFile(sFn1)
	assert.Nil(t, e)
	assert.Equal(t, expSrc1, string(actSrc1))
}
