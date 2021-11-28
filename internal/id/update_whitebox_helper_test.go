// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.
package id

import (
	"fmt"
	"os"
	"reflect"
	"testing"

	"github.com/tj/assert"
)

func TestMain(m *testing.M) {
	ExtendMacrosWithParamCount = true
	i := m.Run()
	if i != 0 {
		os.Exit(i)
	}
	// THe 2nd call is possible, but some tests are failing - needs investigation!
	//
	// ExtendMacrosWithParamCount = false
	// os.Exit(m.Run())
}

type idCheck struct {
	nbTRICE string
	nbID    string
	id      TriceID
	ok      bool
	tf      TriceFmt
}

var tryOkSet = []idCheck{
	{`TRICE(Id(   59), "tt" )`, "Id(   59)", 59, true, TriceFmt{"TRICE", "tt"}},
	{`TRICE_12(Id(   59), "tt" )`, "Id(   59)", 59, true, TriceFmt{"TRICE_12", "tt"}},
	{`Trice0(Id(   59), "tt" )`, "Id(   59)", 59, true, TriceFmt{"Trice0", "tt"}},
	{`TRICE0(Id(   59), "tt" )`, "Id(   59)", 59, true, TriceFmt{"TRICE0", "tt"}},
	{`TRICE0(Id(59   ), "tt" )`, "Id(59   )", 59, true, TriceFmt{"TRICE0", "tt"}},
	{`TRICE0(Id(59), "tt" )`, "Id(59)", 59, true, TriceFmt{"TRICE0", "tt"}},
	{`TRICE0(Id( 59 ), "tt" )`, "Id( 59 )", 59, true, TriceFmt{"TRICE0", "tt"}},
	{`trice0(Id(59), "tt" )`, "Id(59)", 59, true, TriceFmt{"trice0", "tt"}},
	{`trice64_2(Id(59), "%d,%x", -3, -4 )`, "Id(59)", 59, true, TriceFmt{"trice64_2", "%d,%x"}},
}

func checkID(t *testing.T, set []idCheck, i int, id TriceID) {
	assert.True(t, id == set[i].id, fmt.Sprint(i, id))
}

func checkNbID(t *testing.T, set []idCheck, i int, nbID string) {
	assert.True(t, nbID == set[i].nbID, fmt.Sprint(i, nbID))
}

type testTable []struct {
	text    string // file content as input
	exp     string // file content as expected output
	fileMod bool   // expected file modification flag
	listMod bool   // expected list modification flag
}

// checkList is a helper function for update tests.
//
// sharedIDs, if true, reuses IDs for identical format strings.
// mins, maxs is the ID pool for short trices (macros "Tr") - unused
// min, max is the ID pool for finding new IDs
// ...
func checkList(t *testing.T, sharedIDs bool, min, max TriceID, searchMethod string, tt testTable, eList string, extend bool) {
	lu := make(TriceIDLookUp)
	tflu := lu.reverse()
	Verbose = true
	for _, x := range tt {
		act0, _ := updateParamCountAndID0(os.Stdout, x.text, extend)
		listModified := false
		act, fileModified := updateIDsUniqOrShared(os.Stdout, sharedIDs, min, max, searchMethod, act0, lu, tflu, &listModified)
		assert.Equal(t, x.fileMod, fileModified)
		assert.Equal(t, x.listMod, listModified)
		assert.Equal(t, x.exp, act)
	}
	lu.AddFmtCount(os.Stdout)
	aListN := fmt.Sprintln(lu)
	assert.Equal(t, eList, aListN)
}

// checkList2 is a helper function for update tests.
//
// sharedIDs, if true, reuses IDs for identical format strings.
// min, max is the ID pool for finding new IDs
// ...
func checkList2(t *testing.T, sharedIDs bool, min, max TriceID, searchMethod string, tt testTable, extendMacroName bool, inJSON, expJSON string) {
	lu := make(TriceIDLookUp)
	err := lu.FromJSON([]byte(inJSON))
	assert.Nil(t, err)
	tflu := lu.reverse()
	Verbose = true
	for _, x := range tt {
		act0, _ := updateParamCountAndID0(os.Stdout, x.text, extendMacroName)
		listModified := false
		act, fileModified := updateIDsUniqOrShared(os.Stdout, sharedIDs, min, max, searchMethod, act0, lu, tflu, &listModified)
		assert.Equal(t, x.fileMod, fileModified)
		assert.Equal(t, x.listMod, listModified)
		assert.Equal(t, x.exp, act)
	}
	lu.AddFmtCount(os.Stdout)
	aListN := fmt.Sprintln(lu)
	assert.Equal(t, expJSON, aListN)
}

func checkTil(t *testing.T, text string, exp TriceIDLookUp) {
	lu := make(TriceIDLookUp)
	tflu := lu.reverse()
	refreshIDs(os.Stdout, text, lu, tflu)
	assert.True(t, reflect.DeepEqual(lu, exp))
}

func check(t *testing.T, text, expJSON string) {
	lu := make(TriceIDLookUp)
	tflu := lu.reverse()
	refreshIDs(os.Stdout, text, lu, tflu)
	b, err := lu.toJSON()
	assert.Equal(t, nil, err)
	assert.Equal(t, expJSON, string(b))
}

// checkList3 is a helper function for update tests.
//
// sharedIDs, if true, reuses IDs for identical format strings.
// min, max is the ID pool for finding new IDs
// ...
func checkList3(t *testing.T, sharedIDs bool, min, max TriceID, searchMethod string, tt testTable, extendMacroName bool, inMap, expMap TriceIDLookUp) {
	lu := inMap
	tflu := lu.reverse()
	Verbose = true
	for _, x := range tt {
		act0, _ := updateParamCountAndID0(os.Stdout, x.text, extendMacroName)
		listModified := false
		act, fileModified := updateIDsUniqOrShared(os.Stdout, sharedIDs, min, max, searchMethod, act0, lu, tflu, &listModified)
		assert.Equal(t, x.fileMod, fileModified)
		assert.Equal(t, x.listMod, listModified)
		assert.Equal(t, x.exp, act)
	}
	lu.AddFmtCount(os.Stdout)
	eq := reflect.DeepEqual(lu, expMap)
	assert.True(t, eq)
}

// checkList4 is a helper function for update tests.
//
// sharedIDs, if true, reuses IDs for identical format strings.
// min, max is the ID pool for finding new IDs
// ...
func checkList4(t *testing.T, sharedIDs bool, min, max TriceID, searchMethod string, tt testTable, extendMacroName bool, inMap TriceIDLookUp) TriceIDLookUp {
	lu := inMap
	tflu := lu.reverse()
	Verbose = true
	for _, x := range tt {
		act0, _ := updateParamCountAndID0(os.Stdout, x.text, extendMacroName)
		listModified := false
		_, fileModified := updateIDsUniqOrShared(os.Stdout, sharedIDs, min, max, searchMethod, act0, lu, tflu, &listModified)
		assert.Equal(t, x.fileMod, fileModified)
		assert.Equal(t, x.listMod, listModified)
	}
	lu.AddFmtCount(os.Stdout)
	return lu
}
