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
func checkList(t *testing.T, _ /*sharedIDs*/ bool, min, max TriceID, searchMethod string, tt testTable, eList string, extend bool) {
	ilu := make(TriceIDLookUp)
	flu := ilu.reverseS()
	Verbose = true
	for _, x := range tt {
		act0, _ := updateParamCountAndID0(os.Stdout, x.text, extend)
		listModified := false
		act, fileModified := updateIDsUniqOrShared(os.Stdout, false /*sharedIDs*/, min, max, searchMethod, act0, ilu, flu, &listModified)
		assert.Equal(t, x.fileMod, fileModified)
		assert.Equal(t, x.listMod, listModified)
		assert.Equal(t, x.exp, act)
	}
	ilu.AddFmtCount(os.Stdout)
	aListN := fmt.Sprintln(ilu)
	assert.Equal(t, eList, aListN)
}

// checkList2 is a helper function for update tests.
//
// sharedIDs, if true, reuses IDs for identical format strings.
// min, max is the ID pool for finding new IDs
// ...
func checkList2(t *testing.T, _ /*sharedIDs*/ bool, min, max TriceID, searchMethod string, tt testTable, extendMacroName bool, inJSON, expJSON string) {

	ilu := make(TriceIDLookUp)
	err := ilu.FromJSON([]byte(inJSON))
	assert.Nil(t, err)
	flu := ilu.reverseS()
	for _, x := range tt {
		act0, _ := updateParamCountAndID0(os.Stdout, x.text, extendMacroName)
		listModified := false
		act, fileModified := updateIDsUniqOrShared(os.Stdout, false /*sharedIDs*/, min, max, searchMethod, act0, ilu, flu, &listModified)
		assert.Equal(t, x.fileMod, fileModified)
		assert.Equal(t, x.listMod, listModified)
		assert.Equal(t, x.exp, act)
	}
	ilu.AddFmtCount(os.Stdout)
	aListN := fmt.Sprintln(ilu)
	assert.Equal(t, expJSON, aListN)
}

func checkTil(t *testing.T, text string, exp TriceIDLookUp) {
	ilu := make(TriceIDLookUp)
	flu := ilu.reverseS()
	lim := make(TriceIDLookUpLI)
	refreshIDs(os.Stdout, "", text, ilu, flu, lim)
	assert.True(t, reflect.DeepEqual(ilu, exp))
}

func testRefreshIDs(t *testing.T, text, expJSON string) {
	ilu := make(TriceIDLookUp)
	flu := ilu.reverseS()
	lim := make(TriceIDLookUpLI)
	refreshIDs(os.Stdout, "", text, ilu, flu, lim)
	b, err := ilu.toJSON()
	assert.Equal(t, nil, err)
	assert.Equal(t, expJSON, string(b))
}

// checkList3 is a helper function for update tests.
//
// sharedIDs, if true, reuses IDs for identical format strings.
// min, max is the ID pool for finding new IDs
// ...
func checkList3(t *testing.T, _ /*sharedIDs*/ bool, min, max TriceID, searchMethod string, tt testTable, extendMacroName bool, inMap, expMap TriceIDLookUp) {
	ilu := inMap
	flu := ilu.reverseS()
	for _, x := range tt {
		act0, _ := updateParamCountAndID0(os.Stdout, x.text, extendMacroName)
		listModified := false
		act, fileModified := updateIDsUniqOrShared(os.Stdout, false /*sharedIDs*/, min, max, searchMethod, act0, ilu, flu, &listModified)
		assert.Equal(t, x.exp, act)
		assert.Equal(t, x.fileMod, fileModified)
		assert.Equal(t, x.listMod, listModified)
	}
	ilu.AddFmtCount(os.Stdout)
	eq := reflect.DeepEqual(ilu, expMap)
	assert.True(t, eq)
}

// checkList4 is a helper function for update tests.
//
// sharedIDs, if true, reuses IDs for identical format strings.
// min, max is the ID pool for finding new IDs
// ...
func checkList4(t *testing.T, _ /*sharedIDs*/ bool, min, max TriceID, searchMethod string, tt testTable, extendMacroName bool, inMap TriceIDLookUp) TriceIDLookUp {
	ilu := inMap
	flu := ilu.reverseS()
	for _, x := range tt {
		act0, _ := updateParamCountAndID0(os.Stdout, x.text, extendMacroName)
		listModified := false
		_, fileModified := updateIDsUniqOrShared(os.Stdout, false /*sharedIDs*/, min, max, searchMethod, act0, ilu, flu, &listModified)
		assert.Equal(t, x.fileMod, fileModified)
		assert.Equal(t, x.listMod, listModified)
	}
	ilu.AddFmtCount(os.Stdout)
	return ilu
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// todo

//  func checkID(t *testing.T, set []idCheck, i int, id TriceID) {
//  	assert.True(t, id == set[i].id, fmt.Sprint(i, id))
//  }
//
//  func checkNbID(t *testing.T, set []idCheck, i int, nbID string) {
//  	assert.True(t, nbID == set[i].nbID, fmt.Sprint(i, nbID))
//  }
