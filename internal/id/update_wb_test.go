// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.
package id

import (
	"fmt"
	"testing"

	"github.com/rokath/trice/pkg/tst"
)

type idCheck struct {
	nbTRICE string
	nbID    string
	id      TriceID
	ok      bool
	tf      TriceFmt
}

func TestOptionallyExtendLenAndInsertID0(t *testing.T) {
	text := `
	_TRICE8( "hi %d", 5); // to not touch
	TRICE8( Id(0), "hi %2d, %13u, %64b, %8x %02d, %013u, %032b, %016x",1,2,3,4,5,6,7,8 );
	TRICE8( "hi %2d, %13u, %64b, %8x %02d, %013u, %032b, %016x",1,2,3,4,5,6,7,8 );
	TRICE8_3( "hi %2d, %13u, %64b, %8x %02d, %013u, %032b, %016x",1,2,3,4,5,6,7,8 ); // do not change wrong count
	trice_s  ( "%s\n", rts )
	trice_s  ( "%s\n", "rts" );
`
	exp := `
	_TRICE8( "hi %d", 5); // to not touch
	TRICE8_8( Id(0), "hi %2d, %13u, %64b, %8x %02d, %013u, %032b, %016x",1,2,3,4,5,6,7,8 );
	TRICE8_8( Id(0), "hi %2d, %13u, %64b, %8x %02d, %013u, %032b, %016x",1,2,3,4,5,6,7,8 );
	TRICE8_3( Id(0), "hi %2d, %13u, %64b, %8x %02d, %013u, %032b, %016x",1,2,3,4,5,6,7,8 ); // do not change wrong count
	trice_s  ( Id(0), "%s\n", rts )
	trice_s  ( Id(0), "%s\n", "rts" );
`
	act, _ := updateParamCountAndID0(text)
	tst.Equal(t, exp, act)
}

// Only the first occurrence of a double used ID gets in the list.
// Only the list generation out of sources ist tested here.
func TestRefreshIDListSingle0(t *testing.T) {
	text := `
	TRICE16_3( Id(12345), "hi %2d, %13u, %64b\n",1,2,3 );	
	TRICE16_3( Id(12345), "DIFFERENT! %2d, %13u, %64b\n",1,2,3 );	
	Trice16_1( Id(12344), "hi %2d\n",1 );	
	Trice16_1i( Id(12344), "hi %2d\n",1 );	
	Trice16_1( Id(12344), "DIFFERENT! %2d\n", 2 );	
`
	expText := `
	TRICE16_3( Id(12345), "hi %2d, %13u, %64b\n",1,2,3 );	
	TRICE16_3( Id(12345), "DIFFERENT! %2d, %13u, %64b\n",1,2,3 );	
	Trice16_1( Id(12344), "hi %2d\n",1 );	
	Trice16_1i( Id(12344), "hi %2d\n",1 );	
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
	lu := make(TriceIDLookUp)
	tflu := lu.reverse()
	refreshIDs(text, lu, tflu)

	tst.Equal(t, expText, text)
	b, err := lu.toJSON()
	tst.Equal(t, nil, err)
	tst.Equal(t, expJSON, string(b))
}

func TestRefreshIDListSingle1(t *testing.T) {
	text := `
	TRICE16_3( Id(12345), "hi %2d, %13u, %64b\n",1,2,3 );	
	TRICE16_3( Id(  123), "hi %2d, %13u, %64b\n",1,2,3 );	
	Trice16_1( Id(   13), "hi %13u\n", 3 );	
	Trice16_1i( Id(   13), "hi %13u\n", 3 );	
`
	expJSON := `{
	"123": {
		"Type": "TRICE16_3",
		"Strg": "hi %2d, %13u, %64b\\n"
	},
	"12345": {
		"Type": "TRICE16_3",
		"Strg": "hi %2d, %13u, %64b\\n"
	},
	"13": {
		"Type": "Trice16_1",
		"Strg": "hi %13u\\n"
	}
}`
	lu := make(TriceIDLookUp)
	tflu := lu.reverse()
	refreshIDs(text, lu, tflu)

	b, err := lu.toJSON()
	tst.Equal(t, nil, err)
	tst.Equal(t, expJSON, string(b))
}

func TestRefreshIDListSingle2(t *testing.T) {
	text := `
	TRICE16_3( Id(12345), "hi %2d, %13u, %64b\n",1,2,3 );
	trice16_3( Id(12345), "hi %2d, %13u, %64b\n",1,2,3 );
	Trice16_1i( Id(12344), "ho %64b\n",1 );
	Trice16_1( Id(12344), "ho %64b\n",1 );
`
	expJSON := `{
	"12344": {
		"Type": "Trice16_1i",
		"Strg": "ho %64b\\n"
	},
	"12345": {
		"Type": "trice16_3",
		"Strg": "hi %2d, %13u, %64b\\n"
	}
}`
	lu := make(TriceIDLookUp)
	tflu := lu.reverse()
	refreshIDs(text, lu, tflu)

	b, err := lu.toJSON()
	tst.Equal(t, nil, err)
	tst.Equal(t, expJSON, string(b))
}

// A wrong parameter count should not be corrected! THe compiler will complain and a decision should be made.
func TestDoNotCorrectWrongParamCountSingle(t *testing.T) {
	text := `
	Trice8_2( Id(0), "hi %2d",1  );
	TRICE8_2( Id(0), "hi %2d, %13u, %64b, %8x %02d, %013u, %032b, %016x",1,2,3,4,5,6,7,8 );
`
	exp := `
	Trice8_2( Id(0), "hi %2d",1  );
	TRICE8_2( Id(0), "hi %2d, %13u, %64b, %8x %02d, %013u, %032b, %016x",1,2,3,4,5,6,7,8 );
`
	act, _ := updateParamCountAndID0(text)
	tst.Equal(t, exp, act)
}

func TestInsertSharedIDs0Param(t *testing.T) {
	SearchMethod = "downward"
	Min = 10
	Max = 99
	lu := make(TriceIDLookUp)
	tflu := lu.reverse()
	listModified := false

	tt := []insertParamCountAndID{
		{`... Trice0 ( "hi"); ...`, `... Trice0 ( id(32767), "hi"); ...`},
		{`... TRICE0 ( "hi"); ...`, `... TRICE0 ( Id(    99), "hi"); ...`},
		{`... trice0 ( "hi"); ...`, `... trice0 ( Id(    99), "hi"); ...`},
		{`... Trice0i( "hi"); ...`, `... Trice0i( id(32766), "hi"); ...`},
		{`... TRICE0i( "hi"); ...`, `... TRICE0i( Id(    98), "hi"); ...`},
		{`... trice0i( "hi"); ...`, `... trice0i( Id(    98), "hi"); ...`},
	}
	for _, x := range tt {
		act0, _ := updateParamCountAndID0(x.text)
		act, fileModified := updateIDsShared(act0, lu, tflu, &listModified)
		fmt.Println(fileModified)
		tst.Equal(t, true, fileModified)
		tst.Equal(t, true, listModified)
		tst.Equal(t, x.exp, act)
	}
	aList := fmt.Sprintln(lu)
	eList := `map[98:{trice0i hi} 99:{trice0 hi} 32766:{Trice0i hi} 32767:{Trice0 hi}]
`
	tst.Equal(t, eList, aList)
}

func TestInsertSharedIDs1Param(t *testing.T) {
	SearchMethod = "upward"
	Min = 40000
	Max = 50000
	MinShort = 10000
	MaxShort = 20000
	lu := make(TriceIDLookUp)
	tflu := lu.reverse()
	listModified := false

	tt := []insertParamCountAndID{
		{`...  Trice8 ( "hi %03u", 5); ...`, `...  Trice8_1 ( id(10000), "hi %03u", 5); ...`},
		{`...  TRICE8 ( "hi %03u", 5); ...`, `...  TRICE8_1 ( Id( 40000), "hi %03u", 5); ...`},
		{`...  trice8 ( "hi %03u", 5); ...`, `...  trice8_1 ( Id( 40000), "hi %03u", 5); ...`},
		{`... Trice16 ( "hi %03u", 5); ...`, `... Trice16_1 ( id(10001), "hi %03u", 5); ...`},
		{`... TRICE16 ( "hi %03u", 5); ...`, `... TRICE16_1 ( Id( 40001), "hi %03u", 5); ...`},
		{`... trice16 ( "hi %03u", 5); ...`, `... trice16_1 ( Id( 40001), "hi %03u", 5); ...`},
		{`... Trice32 ( "hi %03u", 5); ...`, `... Trice32_1 ( id(10002), "hi %03u", 5); ...`}, // does not exist but allowed to match
		{`... TRICE32 ( "hi %03u", 5); ...`, `... TRICE32_1 ( Id( 40002), "hi %03u", 5); ...`},
		{`... trice32 ( "hi %03u", 5); ...`, `... trice32_1 ( Id( 40002), "hi %03u", 5); ...`},
		{`... Trice64 ( "hi %03u", 5); ...`, `... Trice64_1 ( id(10003), "hi %03u", 5); ...`},
		{`... TRICE64 ( "hi %03u", 5); ...`, `... TRICE64_1 ( Id( 40003), "hi %03u", 5); ...`},
		{`... trice64 ( "hi %03u", 5); ...`, `... trice64_1 ( Id( 40003), "hi %03u", 5); ...`},
		{`...  Trice8i( "hi %03u", 5); ...`, `...  Trice8_1i( id(10004), "hi %03u", 5); ...`},
		{`...  TRICE8i( "hi %03u", 5); ...`, `...  TRICE8_1i( Id( 40004), "hi %03u", 5); ...`},
		{`...  trice8i( "hi %03u", 5); ...`, `...  trice8_1i( Id( 40004), "hi %03u", 5); ...`},
		{`... Trice16i( "hi %03u", 5); ...`, `... Trice16_1i( id(10005), "hi %03u", 5); ...`},
		{`... TRICE16i( "hi %03u", 5); ...`, `... TRICE16_1i( Id( 40005), "hi %03u", 5); ...`},
		{`... trice16i( "hi %03u", 5); ...`, `... trice16_1i( Id( 40005), "hi %03u", 5); ...`},
		{`... Trice32i( "hi %03u", 5); ...`, `... Trice32_1i( id(10006), "hi %03u", 5); ...`},
		{`... TRICE32i( "hi %03u", 5); ...`, `... TRICE32_1i( Id( 40006), "hi %03u", 5); ...`},
		{`... trice32i( "hi %03u", 5); ...`, `... trice32_1i( Id( 40006), "hi %03u", 5); ...`},
		{`... Trice64i( "hi %03u", 5); ...`, `... Trice64_1i( id(10007), "hi %03u", 5); ...`},
		{`... TRICE64i( "hi %03u", 5); ...`, `... TRICE64_1i( Id( 40007), "hi %03u", 5); ...`},
		{`... trice64i( "hi %03u", 5); ...`, `... trice64_1i( Id( 40007), "hi %03u", 5); ...`},
	}
	for _, x := range tt {
		act0, _ := updateParamCountAndID0(x.text)
		act, fileModified := updateIDsShared(act0, lu, tflu, &listModified)
		fmt.Println(fileModified)
		tst.Equal(t, true, fileModified)
		tst.Equal(t, true, listModified)
		tst.Equal(t, x.exp, act)
	}
	aList := fmt.Sprintln(lu)
	eList := `map[10000:{Trice8_1 hi %03u} 10001:{Trice16_1 hi %03u} 10002:{Trice32_1 hi %03u} 10003:{Trice64_1 hi %03u} 10004:{Trice8_1i hi %03u} 10005:{Trice16_1i hi %03u} 10006:{Trice32_1i hi %03u} 10007:{Trice64_1i hi %03u} 40000:{trice8_1 hi %03u} 40001:{trice16_1 hi %03u} 40002:{trice32_1 hi %03u} 40003:{trice64_1 hi %03u} 40004:{trice8_1i hi %03u} 40005:{trice16_1i hi %03u} 40006:{trice32_1i hi %03u} 40007:{trice64_1i hi %03u}]
`
	tst.Equal(t, eList, aList)
}

func _TestInsertSharedIDs0(t *testing.T) {
	text := `
	Trice0(id(0), "Trallala");
	trice8_1( Id(0), "hi %d", 5); // first id
	trice8_1( Id(0), "Hi %d", 5); // different format string needs a new id
	TRICE8_1( Id(       0   ), "Hi %d", 5); // different type case gets same id
	trice8_1( Id(0x0), "Hi %d", 5); // this gets not changed because no decimal number
	TRICE8_1( Id(0), "hi %d", 5); // same format string gets same id
	trice8_1(  Id( 0       ),  "hi %d", 5); // same format string gets same id
	trice8_1( Id(-0), "hi %d", 5); // minus id's are not touched
	Trice8_1( Id(0), "hi %d", 5); // first id
	Trice8_2( id(0), "hi %d %u", 5, 7); // first id
	Trice16_1( Id(0), "hi %d", 5); // first id
	Trice16_1i( Id(0), "hi %d", 5); // first id
	`
	exp := `
	Trice0(id(32767), "Trallala");
	trice8_1( Id(    99), "hi %d", 5); // first id
	trice8_1( Id(    98), "Hi %d", 5); // different format string needs a new id
	TRICE8_1( Id(    98), "Hi %d", 5); // different type case gets same id
	trice8_1( Id(0x0), "Hi %d", 5); // this gets not changed because no decimal number
	TRICE8_1( Id(    99), "hi %d", 5); // same format string gets same id
	trice8_1(  Id(    99),  "hi %d", 5); // same format string gets same id
	trice8_1( Id(-0), "hi %d", 5); // minus id's are not touched
	Trice8_1( id(32766), "hi %d", 5); // first id
	Trice8_2( id(32765), "hi %d %u", 5, 7); // first id
	Trice16_1( id(32764), "hi %d", 5); // first id
	Trice16_1i( id(32763), "hi %d", 5); // first id
	`
	SearchMethod = "downward"
	Min = 10
	Max = 99
	lu := make(TriceIDLookUp)
	tflu := lu.reverse()
	listModified := false

	act, fileModified := updateIDsShared(text, lu, tflu, &listModified)
	tst.Equal(t, true, fileModified)
	tst.Equal(t, true, listModified)
	tst.Equal(t, exp, act)
}

var tryOkSet = []idCheck{
	{`Trice0i(id(   59), "tt" )`, "id(   59)", 59, true, TriceFmt{"Trice0i", "tt"}},
	{`Trice0(Id(   59), "tt" )`, "Id(   59)", 59, true, TriceFmt{"Trice0", "tt"}}, // should be false
	{`Trice0(id(   59), "tt" )`, "id(   59)", 59, true, TriceFmt{"Trice0", "tt"}},
	{`TRICE0(Id(   59), "tt" )`, "Id(   59)", 59, true, TriceFmt{"TRICE0", "tt"}},
	{`TRICE0(id(   59), "tt" )`, "id(   59)", 59, true, TriceFmt{"TRICE0", "tt"}}, // should be false
	{`TRICE0(Id(59   ), "tt" )`, "Id(59   )", 59, true, TriceFmt{"TRICE0", "tt"}},
	{`TRICE0(Id(59), "tt" )`, "Id(59)", 59, true, TriceFmt{"TRICE0", "tt"}},
	{`TRICE0(Id( 59 ), "tt" )`, "Id( 59 )", 59, true, TriceFmt{"TRICE0", "tt"}},
	{`trice0(Id(59), "tt" )`, "Id(59)", 59, true, TriceFmt{"trice0", "tt"}},
	{`trice64_2(Id(59), "%d,%x", -3, -4 )`, "Id(59)", 59, true, TriceFmt{"trice64_2", "%d,%x"}},
}

var tryNotOkSetID = []idCheck{
	{`TRICE0(Id(0x5), "tt" )`, "Id(0x5)", 5, false, TriceFmt{"TRICE0", "tt"}},
	{`TRICE0(id(0x5 ), "tt" )`, "id(0x59)", 0x59, false, TriceFmt{"TRICE0", "tt"}},
}

func TestTriceParseOK(t *testing.T) {
	set := tryOkSet
	for i := range set {
		nbID, id, tf, ok := triceParse(set[i].nbTRICE)
		tst.AssertTrue(t, ok == set[i].ok)
		if ok {
			checkID(t, set, i, id)
			checkNbID(t, set, i, nbID)
			tst.AssertTrueElseInfo(t, tf == tryOkSet[i].tf, fmt.Sprint(tf, tryOkSet[i].tf))
		}
	}
}

func TestTriceIDParseOK(t *testing.T) {
	set := tryOkSet
	for i := range set {
		nbID, id, ok := triceIDParse(set[i].nbTRICE)
		tst.AssertTrue(t, ok == set[i].ok)
		if ok {
			checkID(t, set, i, id)
			checkNbID(t, set, i, nbID)
		}
	}
}

func TestTriceIDParseNotOK(t *testing.T) {
	set := tryNotOkSetID
	for i := range set {
		nbID, id, ok := triceIDParse(set[i].nbTRICE)
		tst.AssertTrueElseInfo(t, ok == set[i].ok, fmt.Sprint(i))
		if ok {
			checkID(t, set, i, id)
			checkNbID(t, set, i, nbID)
		}
	}
}

func TestTriceFmtParse(t *testing.T) {
	for i := range tryOkSet {
		tf, ok := triceFmtParse(tryOkSet[i].nbTRICE)
		tst.AssertTrue(t, ok == tryOkSet[i].ok)
		tst.AssertTrueElseInfo(t, tf == tryOkSet[i].tf, fmt.Sprint(tf, tryOkSet[i].tf))
	}
}

func checkID(t *testing.T, set []idCheck, i int, id TriceID) {
	tst.AssertTrueElseInfo(t, id == set[i].id, fmt.Sprint(i, id))
}

func checkNbID(t *testing.T, set []idCheck, i int, nbID string) {
	tst.AssertTrueElseInfo(t, nbID == set[i].nbID, fmt.Sprint(i, nbID))
}
