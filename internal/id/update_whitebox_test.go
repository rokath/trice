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

// Only the first occurrence of a double used ID gets in the list.
// Only the list generation out of sources ist tested here.
func TestRefreshIDListSingle0(t *testing.T) {
	text := `
	TRICE16_3( Id(12345), "hi %2d, %13u, %64b\n",1,2,3 );	
	TRICE16_3( Id(12345), "DIFFERENT! %2d, %13u, %64b\n",1,2,3 );	
	Trice16_1( Id(12344), "hi %2d\n",1 );		
	Trice16_1( Id(12344), "DIFFERENT! %2d\n", 2 );	
`
	expJSON := `{
	"12344": {
		"Type": "Trice16_1",
		"Strg": "hi %2d\\n"
	},
	"12345": {
		"Type": "TRICE16_3",
		"Strg": "hi %2d, %13u, %64b\\n"
	}
}`
	check(t, text, expJSON)
}

func TestRefreshIDListSingle1WithNewline(t *testing.T) {
	var text string
	fmt := `hi %2d, %13u, %64b\n`
	text += `...   TRICE( Id(1234), "` + fmt + `",1,2,3 ); ...
	`
	text += `...   TRICE16_3( Id(12345), "` + fmt + `",1,2,3 ); ...
	`
	text += `...   TRICE16_3( Id(  123), "` + fmt + `",1,2,3 ); ...
	`
	text += `...   Trice16_1( Id(   13), "` + fmt + `", 3 );               ...
	`

	el := make(TriceIDLookUp)
	el[12345] = TriceFmt{Type: "TRICE16_3", Strg: fmt}
	el[1234] = TriceFmt{Type: "TRICE", Strg: fmt}
	el[123] = TriceFmt{Type: "TRICE16_3", Strg: fmt}
	el[13] = TriceFmt{Type: "Trice16_1", Strg: fmt}
	checkTil(t, text, el)
}

func TestRefreshIDListSingle1WithoutNewline(t *testing.T) {
	var text string
	fmt := `hi %2d, %13u, %64b\n`
	text += `...   TRICE( Id(1234), "` + fmt + `",1,2,3 ); ...`
	text += `...   TRICE16_3( Id(12345), "` + fmt + `",1,2,3 ); ...`
	text += `...   TRICE16_3( Id(  123), "` + fmt + `",1,2,3 ); ...`
	text += `...   Trice16_1( Id(   13), "` + fmt + `", 3 );               ...`

	el := make(TriceIDLookUp)
	el[12345] = TriceFmt{Type: "TRICE16_3", Strg: fmt}
	el[1234] = TriceFmt{Type: "TRICE", Strg: fmt}
	el[123] = TriceFmt{Type: "TRICE16_3", Strg: fmt}
	el[13] = TriceFmt{Type: "Trice16_1", Strg: fmt}
	checkTil(t, text, el)
}

func TestRefreshIDListSingle2(t *testing.T) {
	text := `
	TRICE16_3( Id(12345), "hi %2d, %13u, %64b\n",1,2,3 );
	trice16_3( Id(12345), "hi %2d, %13u, %64b\n",1,2,3 );
	Trice16_1( Id(12344), "ho %64b\n",1 );
`
	expJSON := `{
	"12344": {
		"Type": "Trice16_1",
		"Strg": "ho %64b\\n"
	},
	"12345": {
		"Type": "trice16_3",
		"Strg": "hi %2d, %13u, %64b\\n"
	}
}`
	check(t, text, expJSON)
}

func checkTil(t *testing.T, text string, exp TriceIDLookUp) {
	lu := make(TriceIDLookUp)
	tflu := lu.reverse()
	refreshIDs(text, lu, tflu)
	assert.True(t, reflect.DeepEqual(lu, exp))
}

func check(t *testing.T, text, expJSON string) {
	lu := make(TriceIDLookUp)
	tflu := lu.reverse()
	refreshIDs(text, lu, tflu)
	b, err := lu.toJSON()
	assert.Equal(t, nil, err)
	assert.Equal(t, expJSON, string(b))
}

func TestInsertSharedIDsInvalid0(t *testing.T) {
	SearchMethod = "downward"
	listModified := false
	fileModified := false
	tt := testTable{
		{
			`trice8_1( Id(0x0), "Hi %d", 5); // this gets not changed because no decimal number`,
			`trice8_1( Id(0x0), "Hi %d", 5); // this gets not changed because no decimal number`, fileModified, listModified},
		{
			`trice8_1( Id(-0), "hi %d", 5); // minus id's are not touched`,
			`trice8_1( Id(-0), "hi %d", 5); // minus id's are not touched`, fileModified, listModified},
	}
	eList := `map[]
`
	checkList(t, SharedIDs, 10000, 20000, 40000, 50000, tt, eList, true)
	checkList(t, SharedIDs, 10000, 20000, 40000, 50000, tt, eList, false)
}

var tryOkSet = []idCheck{
	//	{`Trice0i(id(   59), "tt" )`, "id(   59)", 59, true, TriceFmt{"Trice0i", "tt"}},
	{`Trice0(Id(   59), "tt" )`, "Id(   59)", 59, true, TriceFmt{"Trice0", "tt"}}, // should be false
	//{`Trice0(id(   59), "tt" )`, "id(   59)", 59, true, TriceFmt{"Trice0", "tt"}},
	{`TRICE0(Id(   59), "tt" )`, "Id(   59)", 59, true, TriceFmt{"TRICE0", "tt"}},
	//	{`TRICE0(id(   59), "tt" )`, "id(   59)", 59, true, TriceFmt{"TRICE0", "tt"}}, // should be false
	{`TRICE0(Id(59   ), "tt" )`, "Id(59   )", 59, true, TriceFmt{"TRICE0", "tt"}},
	{`TRICE0(Id(59), "tt" )`, "Id(59)", 59, true, TriceFmt{"TRICE0", "tt"}},
	{`TRICE0(Id( 59 ), "tt" )`, "Id( 59 )", 59, true, TriceFmt{"TRICE0", "tt"}},
	{`trice0(Id(59), "tt" )`, "Id(59)", 59, true, TriceFmt{"trice0", "tt"}},
	{`trice64_2(Id(59), "%d,%x", -3, -4 )`, "Id(59)", 59, true, TriceFmt{"trice64_2", "%d,%x"}},
}

func TestTriceIDParseOK(t *testing.T) {
	set := tryOkSet
	for i := range set {
		nbID, id, ok := triceIDParse(set[i].nbTRICE)
		assert.True(t, ok == set[i].ok)
		if ok {
			checkID(t, set, i, id)
			checkNbID(t, set, i, nbID)
		}
	}
}

func TestTriceFmtParse(t *testing.T) {
	for i := range tryOkSet {
		tf, ok := triceFmtParse(tryOkSet[i].nbTRICE)
		assert.True(t, ok == tryOkSet[i].ok)
		assert.True(t, tf == tryOkSet[i].tf, fmt.Sprint(tf, tryOkSet[i].tf))
	}
}

func TestTriceIDParseNotOK(t *testing.T) {
	var tryNotOkSetID = []idCheck{
		{`TRICE0(Id(0x5), "tt" )`, "Id(0x5)", 5, false, TriceFmt{"TRICE0", "tt"}},
		{`TRICE0(id(0x5 ), "tt" )`, "Id(0x59)", 0x59, false, TriceFmt{"TRICE0", "tt"}},
	}
	set := tryNotOkSetID
	for i := range set {
		nbID, id, ok := triceIDParse(set[i].nbTRICE)
		assert.True(t, ok == set[i].ok, fmt.Sprint(i))
		if ok {
			checkID(t, set, i, id)
			checkNbID(t, set, i, nbID)
		}
	}
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
func checkList(t *testing.T, sharedIDs bool, mins, maxs, min, max TriceID, tt testTable, eList string, extend bool) {
	lu := make(TriceIDLookUp)
	tflu := lu.reverse()
	Verbose = true
	for _, x := range tt {
		act0, _ := updateParamCountAndID0(x.text, extend)
		listModified := false
		act, fileModified := updateIDsUniqOrShared(sharedIDs, mins, maxs, min, max, act0, lu, tflu, &listModified)
		assert.Equal(t, x.fileMod, fileModified)
		assert.Equal(t, x.listMod, listModified)
		assert.Equal(t, x.exp, act)
	}
	lu.AddFmtCount()
	aListN := fmt.Sprintln(lu)
	assert.Equal(t, eList, aListN)
}

// checkList2 is a helper function for update tests.
//
// sharedIDs, if true, reuses IDs for identical format strings.
// min, max is the ID pool for finding new IDs
// ...
func checkList2(t *testing.T, sharedIDs bool, min, max TriceID, tt testTable, extendMacroName bool, inJSON, expJSON string) {
	lu := make(TriceIDLookUp)
	err := lu.FromJSON([]byte(inJSON))
	assert.Nil(t, err)
	tflu := lu.reverse()
	Verbose = true
	for _, x := range tt {
		act0, _ := updateParamCountAndID0(x.text, extendMacroName)
		listModified := false
		act, fileModified := updateIDsUniqOrShared(sharedIDs, 0, 0, min, max, act0, lu, tflu, &listModified)
		assert.Equal(t, x.fileMod, fileModified)
		assert.Equal(t, x.listMod, listModified)
		assert.Equal(t, x.exp, act)
	}
	lu.AddFmtCount()
	aListN := fmt.Sprintln(lu)
	assert.Equal(t, expJSON, aListN)
}

// Because of the parallel test execution the global variables must be equal for all tests
func TestInsertSharedIDs0ZeroParam1(t *testing.T) {
	SearchMethod = "downward"
	listModified := true
	fileModified := true
	tt := testTable{
		{`... TRICE0( "hi"); ...`, `... TRICE0( Id(   99), "hi"); ...`, fileModified, listModified},
		{`... TRICE0( "hi"); ...`, `... TRICE0( Id(   98), "hi"); ...`, fileModified, listModified},
	}
	eList := `map[98:{TRICE0 hi} 99:{TRICE0 hi}]
`
	sharedIDs := false
	checkList2(t, sharedIDs, 10, 99, tt, true, "", eList)
}

// Because of the parallel test execution the global variables must be equal for all tests
func TestInsertSharedIDs0ZeroParam2(t *testing.T) {
	SearchMethod = "downward"
	listModified := true
	fileModified := true
	tt := testTable{
		{`... TRICE( "hi"); ...`, `... TRICE( Id(   99), "hi"); ...`, fileModified, listModified},
		{`... TRICE( "hi"); ...`, `... TRICE( Id(   98), "hi"); ...`, fileModified, listModified},
	}
	eList := `map[98:{TRICE hi} 99:{TRICE hi}]
`
	sharedIDs := false
	extendMacroName := true
	checkList2(t, sharedIDs, 10, 99, tt, extendMacroName, "", eList)
}

// Because of the parallel test execution the global variables must be equal for all tests
func TestInsertSharedIDs0ZeroParam3(t *testing.T) {
	SearchMethod = "downward"
	listModified := true
	fileModified := true
	tt := testTable{
		{`... TRICE( "hi %d", 7); ...`, `... TRICE( Id(   99), "hi %d", 7); ...`, fileModified, listModified},
		{`... TRICE( "hi %u %b", 6, 6); ...`, `... TRICE( Id(   98), "hi %u %b", 6, 6); ...`, fileModified, listModified},
	}
	eList := `map[98:{TRICE_2 hi %u %b} 99:{TRICE_1 hi %d}]
`
	sharedIDs := false
	extendMacroName := false
	checkList2(t, sharedIDs, 10, 99, tt, extendMacroName, "", eList)
}

// Because of the parallel test execution the global variables must be equal for all tests
func TestInsertSharedIDs0ZeroParam3032(t *testing.T) {
	SearchMethod = "downward"
	listModified := true
	fileModified := true
	tt := testTable{
		{`... TRICE32( "hi %d", 7); ...`, `... TRICE32( Id(   99), "hi %d", 7); ...`, fileModified, listModified},
		{`... TRICE32( "hi %u %b", 6, 6); ...`, `... TRICE32( Id(   98), "hi %u %b", 6, 6); ...`, fileModified, listModified},
	}
	eList := `map[98:{TRICE32_2 hi %u %b} 99:{TRICE32_1 hi %d}]
`
	sharedIDs := false
	extendMacroName := false
	checkList2(t, sharedIDs, 10, 99, tt, extendMacroName, "", eList)
}

// Because of the parallel test execution the global variables must be equal for all tests
func TestInsertSharedIDs0ZeroParam332(t *testing.T) {
	SearchMethod = "downward"
	listModified := true
	fileModified := true
	tt := testTable{
		{`... TRICE32( "hi %d", 7); ...`, `... TRICE32_1( Id(   99), "hi %d", 7); ...`, fileModified, listModified},
		{`... TRICE32( "hi %u %b", 6, 6); ...`, `... TRICE32_2( Id(   98), "hi %u %b", 6, 6); ...`, fileModified, listModified},
	}
	eList := `map[98:{TRICE32_2 hi %u %b} 99:{TRICE32_1 hi %d}]
`
	sharedIDs := false
	extendMacroName := true
	checkList2(t, sharedIDs, 10, 99, tt, extendMacroName, "", eList)
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Diese Vierergruppe muss gehen.
//  - til soll immer mit text passen.
//  - Bei _n Erweiterung -> andere ID (ggf. til-Erweiterung)
//  - Bei _n Entfernung -> andere ID (ggf. til-Erweiterung)

// Because of the parallel test execution the global variables must be equal for all tests
func TestInsertSharedIDs0ZeroParam4noExtend(t *testing.T) {
	SearchMethod = "downward"
	listModified := false
	fileModified := true
	tt := testTable{
		{`... TRICE( "hi %d", 7); ...`, `... TRICE( Id(   99), "hi %d", 7); ...`, fileModified, listModified},
		{`... TRICE( "hi %u %b", 6, 6); ...`, `... TRICE( Id(   98), "hi %u %b", 6, 6); ...`, fileModified, listModified},
	}
	il := make(TriceIDLookUp)
	il[99] = TriceFmt{Type: "TRICE", Strg: "hi %d"}
	il[98] = TriceFmt{Type: "TRICE", Strg: "hi %u %b"}
	sharedIDs := true
	extendMacroName := false
	checkList3(t, sharedIDs, 10, 99, tt, extendMacroName, il, il)
}

// Because of the parallel test execution the global variables must be equal for all tests
func TestInsertSharedIDs0ZeroParam4extend(t *testing.T) {
	SearchMethod = "downward"
	listModified := false
	fileModified := true
	tt := testTable{
		{`... TRICE( "hi %d", 7); ...`, `... TRICE_1( Id(   99), "hi %d", 7); ...`, fileModified, listModified},
		{`... TRICE( "hi %u %b", 6, 6); ...`, `... TRICE_2( Id(   98), "hi %u %b", 6, 6); ...`, fileModified, listModified},
	}
	il := make(TriceIDLookUp)
	il[99] = TriceFmt{Type: "TRICE_1", Strg: "hi %d"}
	il[98] = TriceFmt{Type: "TRICE_2", Strg: "hi %u %b"}
	sharedIDs := true
	extendMacroName := true
	checkList3(t, sharedIDs, 10, 99, tt, extendMacroName, il, il)
}

// Because of the parallel test execution the global variables must be equal for all tests
func TestInsertSharedIDs0ZeroParam16noExtend(t *testing.T) {
	SearchMethod = "downward"
	listModified := false
	fileModified := true
	tt := testTable{
		{`... TRICE16( "hi %d", 7); ...`, `... TRICE16( Id(   99), "hi %d", 7); ...`, fileModified, listModified},
		{`... TRICE16( "hi %u %b", 6, 6); ...`, `... TRICE16( Id(   98), "hi %u %b", 6, 6); ...`, fileModified, listModified},
	}
	il := make(TriceIDLookUp)
	il[99] = TriceFmt{Type: "TRICE16", Strg: "hi %d"}
	il[98] = TriceFmt{Type: "TRICE16", Strg: "hi %u %b"}
	sharedIDs := true
	extendMacroName := false
	checkList3(t, sharedIDs, 10, 99, tt, extendMacroName, il, il)
}

// Because of the parallel test execution the global variables must be equal for all tests
func TestInsertSharedIDs0ZeroParam16extend(t *testing.T) {
	SearchMethod = "downward"
	fileModified := true
	listModified := false
	tt := testTable{
		{`... TRICE16( "hi %d", 7); ...`, `... TRICE16_1( Id(   99), "hi %d", 7); ...`, fileModified, listModified},
		{`... TRICE16( "hi %u %b", 6, 6); ...`, `... TRICE16_2( Id(   98), "hi %u %b", 6, 6); ...`, fileModified, listModified},
	}
	il := make(TriceIDLookUp)
	il[99] = TriceFmt{Type: "TRICE16_1", Strg: "hi %d"}
	il[98] = TriceFmt{Type: "TRICE16_2", Strg: "hi %u %b"}
	sharedIDs := true
	extendMacroName := true
	checkList3(t, sharedIDs, 10, 99, tt, extendMacroName, il, il)
}

//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// checkList3 is a helper function for update tests.
//
// sharedIDs, if true, reuses IDs for identical format strings.
// min, max is the ID pool for finding new IDs
// ...
func checkList3(t *testing.T, sharedIDs bool, min, max TriceID, tt testTable, extendMacroName bool, inMap, expMap TriceIDLookUp) {
	lu := inMap
	tflu := lu.reverse()
	Verbose = true
	for _, x := range tt {
		act0, _ := updateParamCountAndID0(x.text, extendMacroName)
		listModified := false
		act, fileModified := updateIDsUniqOrShared(sharedIDs, 0, 0, min, max, act0, lu, tflu, &listModified)
		assert.Equal(t, x.fileMod, fileModified)
		assert.Equal(t, x.listMod, listModified)
		assert.Equal(t, x.exp, act)
	}
	lu.AddFmtCount()
	eq := reflect.DeepEqual(lu, expMap)
	assert.True(t, eq)
}

// checkList4 is a helper function for update tests.
//
// sharedIDs, if true, reuses IDs for identical format strings.
// min, max is the ID pool for finding new IDs
// ...
func checkList4(t *testing.T, sharedIDs bool, min, max TriceID, tt testTable, extendMacroName bool, inMap TriceIDLookUp) TriceIDLookUp {
	lu := inMap
	tflu := lu.reverse()
	Verbose = true
	for _, x := range tt {
		act0, _ := updateParamCountAndID0(x.text, extendMacroName)
		listModified := false
		_, fileModified := updateIDsUniqOrShared(sharedIDs, 0, 0, min, max, act0, lu, tflu, &listModified)
		assert.Equal(t, x.fileMod, fileModified)
		assert.Equal(t, x.listMod, listModified)
	}
	lu.AddFmtCount()
	return lu
}

// Because of the parallel test execution the global variables must be equal for all tests
func TestInsertSharedIDs0ZeroParam(t *testing.T) {
	SearchMethod = "downward"
	fileModified := true
	listModified := false
	sharedIDs := true
	extendMacroName := false
	im := make(TriceIDLookUp)
	im[99] = TriceFmt{Type: "TRICE0", Strg: "hi"}
	im[98] = TriceFmt{Type: "TRICE0", Strg: "hi"}

	// When at least 2 IDs in til with same information, it is not determined, which ID is used again.
	// Therefore all cases must be checked.
	tt := testTable{ // case one
		{`... TRICE0 ( "hi"); ...`, `... TRICE0 ( Id(   99), "hi"); ...`, fileModified, listModified},
		{`... TRICE0( "hi"); ...`, `... TRICE0( Id(   99), "hi"); ...`, fileModified, listModified},
		{`... trice0 ( "hi"); ...`, `... trice0 ( Id(   99), "hi"); ...`, fileModified, listModified},
		{`... trice0( "hi"); ...`, `... trice0( Id(   99), "hi"); ...`, fileModified, listModified},
		{`... Trice0 ( "hi"); ...`, `... Trice0 ( Id(   99), "hi"); ...`, fileModified, listModified},
		{`... Trice0( "hi"); ...`, `... Trice0( Id(   99), "hi"); ...`, fileModified, listModified},
	}
	om := checkList4(t, sharedIDs, 10, 99, tt, extendMacroName, im)
	eq := reflect.DeepEqual(om, im)
	if eq {
		return
	}

	// next try
	tt = testTable{ // case two
		{`... TRICE0 ( "hi"); ...`, `... TRICE0 ( Id(   98), "hi"); ...`, fileModified, listModified},
		{`... TRICE0( "hi"); ...`, `... TRICE0( Id(   98), "hi"); ...`, fileModified, listModified},
		{`... trice0 ( "hi"); ...`, `... trice0 ( Id(   98), "hi"); ...`, fileModified, listModified},
		{`... trice0( "hi"); ...`, `... trice0( Id(   98), "hi"); ...`, fileModified, listModified},
		{`... Trice0 ( "hi"); ...`, `... Trice0 ( Id(   98), "hi"); ...`, fileModified, listModified},
		{`... Trice0( "hi"); ...`, `... Trice0( Id(   98), "hi"); ...`, fileModified, listModified},
	}
	om = checkList4(t, sharedIDs, 10, 99, tt, extendMacroName, im)
	assert.True(t, reflect.DeepEqual(om, im))
}

// Because of the parallel test execution the global variables must be equal for all tests
func TestInsertSharedIDs1WithExtendN(t *testing.T) {
	SearchMethod = "upward"
	tt := testTable{
		{`...  Trice8 ( "hi %03u", 5); ...`, `...  Trice8_1 ( Id(10000), "hi %03u", 5); ...`, true, false},
		{`...  TRICE8 ( "hi %03u", 5); ...`, `...  TRICE8_1 ( Id(10000), "hi %03u", 5); ...`, true, false},
		{`...  trice8 ( "hi %03u", 5); ...`, `...  trice8_1 ( Id(10000), "hi %03u", 5); ...`, true, false},

		{`... Trice16 ( "hi %03u", 5); ...`, `... Trice16_1 ( Id(10001), "hi %03u", 5); ...`, true, false},
		{`... TRICE16 ( "hi %03u", 5); ...`, `... TRICE16_1 ( Id(10001), "hi %03u", 5); ...`, true, false},
		{`... trice16 ( "hi %03u", 5); ...`, `... trice16_1 ( Id(10001), "hi %03u", 5); ...`, true, false},

		{`... Trice32 ( "hi %03u", 5); ...`, `... Trice32_1 ( Id(10002), "hi %03u", 5); ...`, true, false},
		{`... TRICE32 ( "hi %03u", 5); ...`, `... TRICE32_1 ( Id(10002), "hi %03u", 5); ...`, true, false},
		{`... trice32 ( "hi %03u", 5); ...`, `... trice32_1 ( Id(10002), "hi %03u", 5); ...`, true, false},

		{`... Trice64 ( "hi %03u", 5); ...`, `... Trice64_1 ( Id(10003), "hi %03u", 5); ...`, true, false},
		{`... TRICE64 ( "hi %03u", 5); ...`, `... TRICE64_1 ( Id(10003), "hi %03u", 5); ...`, true, false},
		{`... trice64 ( "hi %03u", 5); ...`, `... trice64_1 ( Id(10003), "hi %03u", 5); ...`, true, false},
	}
	im := make(TriceIDLookUp)
	im[10000] = TriceFmt{Type: "TRICE8_1", Strg: "hi %03u"}
	im[10001] = TriceFmt{Type: "TRICE16_1", Strg: "hi %03u"}
	im[10002] = TriceFmt{Type: "TRICE32_1", Strg: "hi %03u"}
	im[10003] = TriceFmt{Type: "TRICE64_1", Strg: "hi %03u"}
	em := im
	sharedIDs := true
	extendMacroName := true
	checkList3(t, sharedIDs, 10, 99, tt, extendMacroName, im, em)
}

// Because of the parallel test execution the global variables must be equal for all tests.
// The trice map does distinguish between TRICE8 and TRICE8_2 for example, so even "sameID" is selected,
// there are 2 different IDs used when the format string is identical.
func TestInsertSharedIDs2NoExtendN(t *testing.T) {
	SearchMethod = "upward"
	sharedIDs := true
	extendMacroName := false
	tt := []struct {
		text, exp        string
		fileMod, listMod bool
	}{
		{`...  TRICE8 ( "hi %03u, %5x", 5, 7); ...`, `...  TRICE8 ( Id(10000), "hi %03u, %5x", 5, 7); ...`, true, false},
		{`...  trice8 ( "hi %03u, %5x", 5, 7); ...`, `...  trice8 ( Id(10000), "hi %03u, %5x", 5, 7); ...`, true, false},
		{`...  Trice8 ( "hi %03u, %5x", 5, 7); ...`, `...  Trice8 ( Id(10000), "hi %03u, %5x", 5, 7); ...`, true, false},

		{`... Trice16 ( "hi %03u, %5x", 5, 7); ...`, `... Trice16 ( Id(10001), "hi %03u, %5x", 5, 7); ...`, true, false},
		{`... TRICE16 ( "hi %03u, %5x", 5, 7); ...`, `... TRICE16 ( Id(10001), "hi %03u, %5x", 5, 7); ...`, true, false},
		{`... trice16 ( "hi %03u, %5x", 5, 7); ...`, `... trice16 ( Id(10001), "hi %03u, %5x", 5, 7); ...`, true, false},

		{`... Trice32 ( "hi %03u, %5x", 5, 7); ...`, `... Trice32 ( Id(10002), "hi %03u, %5x", 5, 7); ...`, true, false}, // does not exist but allowed to match
		{`... TRICE32 ( "hi %03u, %5x", 5, 7); ...`, `... TRICE32 ( Id(10002), "hi %03u, %5x", 5, 7); ...`, true, false},
		{`... trice32 ( "hi %03u, %5x", 5, 7); ...`, `... trice32 ( Id(10002), "hi %03u, %5x", 5, 7); ...`, true, false},

		{`... Trice64 ( "hi %03u, %5x", 5, 7); ...`, `... Trice64 ( Id(10003), "hi %03u, %5x", 5, 7); ...`, true, false}, // does not exist but allowed to match
		{`... TRICE64 ( "hi %03u, %5x", 5, 7); ...`, `... TRICE64 ( Id(10003), "hi %03u, %5x", 5, 7); ...`, true, false},
		{`... trice64 ( "hi %03u, %5x", 5, 7); ...`, `... trice64 ( Id(10003), "hi %03u, %5x", 5, 7); ...`, true, false},

		{`... Trice( "hi %03u, %5x", 5, 7); ...`, `... Trice( Id(10004), "hi %03u, %5x", 5, 7); ...`, true, false}, // does not exist but allowed to match
		{`... TRICE( "hi %03u, %5x", 5, 7); ...`, `... TRICE( Id(10004), "hi %03u, %5x", 5, 7); ...`, true, false},
		{`... trice( "hi %03u, %5x", 5, 7); ...`, `... trice( Id(10004), "hi %03u, %5x", 5, 7); ...`, true, false},

		{`...  Trice8_2 ( "hi %03u, %5x", 5, 7); ...`, `...  Trice8_2 ( Id(10005), "hi %03u, %5x", 5, 7); ...`, true, false},
		{`...  Trice8_2 ( "hi %03u, %5x", 5, 7); ...`, `...  Trice8_2 ( Id(10005), "hi %03u, %5x", 5, 7); ...`, true, false},
		{`...  Trice8_2 ( Id(10005), "hi %03u, %5x", 5, 7); ...`, `...  Trice8_2 ( Id(10005), "hi %03u, %5x", 5, 7); ...`, false, false},
	}

	im := make(TriceIDLookUp)
	im[10000] = TriceFmt{Type: "TRICE8", Strg: "hi %03u, %5x"}
	im[10001] = TriceFmt{Type: "TRICE16", Strg: "hi %03u, %5x"}
	im[10002] = TriceFmt{Type: "TRICE32", Strg: "hi %03u, %5x"}
	im[10003] = TriceFmt{Type: "TRICE64", Strg: "hi %03u, %5x"}
	im[10004] = TriceFmt{Type: "TRICE", Strg: "hi %03u, %5x"}
	im[10005] = TriceFmt{Type: "TRICE8_2", Strg: "hi %03u, %5x"}
	em := im
	checkList3(t, sharedIDs, 10000, 10099, tt, extendMacroName, im, em)
}

func _TestTrice(t *testing.T) { // wip
	tt := testTable{
		{
			`TRICE( "Go is fun");`,
			`TRICE(id(20000), "Go is fun");`, true, true},
	}
	eList := `map[20000:{TRICE Trallala}]
`
	checkList(t, true, 10000, 20000, 10, 99, tt, eList, true)
	checkList(t, true, 10000, 20000, 10, 99, tt, eList, false)
}

func _TestTriceParseOK(t *testing.T) {
	set := tryOkSet
	for i := range set {
		nbID, id, tf, ok := triceParse(set[i].nbTRICE)
		assert.True(t, ok == set[i].ok)
		if ok {
			checkID(t, set, i, id)
			checkNbID(t, set, i, nbID)
			assert.True(t, tf == tryOkSet[i].tf, fmt.Sprint(tf, tryOkSet[i].tf))
		}
	}
}

// Because of the parallel test execution the global variables must be equal for all tests
func _TestInsertSharedIDs0WithParamCount(t *testing.T) {
	SearchMethod = "downward"

	tt := testTable{
		{
			`Trice0(Id(0), "Trallala");`,
			`Trice0(Id(20000), "Trallala");`, true, true},
		//		{
		//			`trice8_1( Id(0), "hi %d", 5); // first id`,
		//			`trice8_1( Id(    99), "hi %d", 5); // first id`, true, true},
		//		{
		//			`trice8_1( Id(0), "Hi %d", 5); // different format string needs a new id`,
		//			`trice8_1( Id(    98), "Hi %d", 5); // different format string needs a new id`, true, true},
		//		{
		//			`TRICE8_1( Id(       0   ), "Hi %d", 5); // different type case gets same id`,
		//			`TRICE8_1( Id(    98), "Hi %d", 5); // different type case gets same id`, true, false},
		//		{
		//			`TRICE8_1( Id(0), "hi %d", 5); // same format string gets same id`,
		//			`TRICE8_1( Id(    99), "hi %d", 5); // same format string gets same id`, true, false},
		//		{
		//			`trice8_1(  Id( 0       ),  "hi %d", 5); // same format string gets same id`,
		//			`trice8_1(  Id(    99),  "hi %d", 5); // same format string gets same id`, true, false},
		//		{
		//			`Trice8_1( Id(0), "hi %d", 5); // first id`,
		//			`Trice8_1( id(19999), "hi %d", 5); // first id`, true, true},
		//		{
		//			`Trice8_2( Id(0), "hi %d %u", 5, 7); // first id`,
		//			`Trice8_2( Id(19998), "hi %d %u", 5, 7); // first id`, true, true},
		//		{
		//			`Trice16_1( Id(0), "hi %d", 5); // first id`,
		//			`Trice16_1( id(19997), "hi %d", 5); // first id`, true, true},
	}
	eList := `map[98:{TRICE8_1 Hi %d} 99:{trice8_1 hi %d} 19997:{Trice16_1 hi %d} 19998:{Trice8_2 hi %d %u} 19999:{Trice8_1 hi %d} 20000:{Trice0 Trallala}]
`
	//	checkList(t, true, 10000, 20000, 10, 99, tt, eList, true)
	checkList(t, true, 10000, 20000, 10, 99, tt, eList, false)
}
