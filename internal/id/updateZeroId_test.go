// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id_test

import (
	"testing"

	"github.com/rokath/trice/internal/id"
)

func TestUpdateZeroId(t *testing.T) {
	id.SearchMethod = "legacy"
	sOri := []string{`
	TRICE0 (Id(0), "---------------------------------------\n" );
	TRICE0 (Id(0), "---------------------------------------\n" );
	`, `
	TRICE8_1( Id(0), "tst:TRICE8 %d\n", 1 );
	TRICE8_2( Id(0), "tst:TRICE8 %d %d\n", 1, 2 );
	TRICE8_3( Id(0), "tst:TRICE8 %d %d %d\n", 1, 2, 3 );
	TRICE8_4( Id(0), "tst:TRICE8 %d %d %d %d\n", 1, 2, 3, 4 );
	TRICE8_5( Id(0), "tst:TRICE8 %d %d %d %d %d\n", 1, 2, 3, 4, 5 );
	TRICE8_6( Id(0), "tst:TRICE8 %d %d %d %d %d %d \n", 1, 2, 3, 4, 5, 6 );
	TRICE8_7( Id(0), "tst:TRICE8 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7 );
	TRICE8_8( Id(0), "tst:TRICE8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
	`, `
	TRICE16_1( Id(0), "tst:TRICE16 %d\n", 1 );
	TRICE16_2( Id(0), "tst:TRICE16 %d %d\n", 1, 2 );
	TRICE16_3( Id(0), "tst:TRICE16 %d %d %d\n", 1, 2, 3 );
	TRICE16_4( Id(0), "tst:TRICE16 %d %d %d %d\n", 1, 2, 3, 4 );
	`, `
	TRICE32_2( Id(0), "tst:TRICE32   %%09x ->     %09x     %09x", 1, 0x7fffffff );
	TRICE32_2( Id(0), "tst:     %09x     %09x\n", 0x80000000, 0xffffffff );
	TRICE32_2( Id(0), "tst:TRICE32   %%11d ->   %11d   %11d", 1, 0x7fffffff );
	TRICE32_2( Id(0), "tst:   %11d   %11d\n", 0x80000000, 0xffffffff  );
	TRICE32_2( Id(0), "tst:TRICE32   %%12o ->  %12o  %12o", 1, 0x7fffffff );
	TRICE32_2( Id(0), "tst:  %12o  %12o\n", 0x80000000, 0xffffffff );
	TRICE_S( Id(0), "sig:generated=%s\n", x );
	TRICE_S  (   Id(0),   "sig:generated=%s\n", x );
	`}

	sExp := []string{`
	TRICE0 (Id(43274), "---------------------------------------\n" );
	TRICE0 (Id( 9089), "---------------------------------------\n" );
	`, `
	TRICE8_1( Id(46083), "tst:TRICE8 %d\n", 1 );
	TRICE8_2( Id(50226), "tst:TRICE8 %d %d\n", 1, 2 );
	TRICE8_3( Id(18460), "tst:TRICE8 %d %d %d\n", 1, 2, 3 );
	TRICE8_4( Id(55041), "tst:TRICE8 %d %d %d %d\n", 1, 2, 3, 4 );
	TRICE8_5( Id(60612), "tst:TRICE8 %d %d %d %d %d\n", 1, 2, 3, 4, 5 );
	TRICE8_6( Id(16507), "tst:TRICE8 %d %d %d %d %d %d \n", 1, 2, 3, 4, 5, 6 );
	TRICE8_7( Id(59378), "tst:TRICE8 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7 );
	TRICE8_8( Id(62543), "tst:TRICE8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
	`, `
	TRICE16_1( Id(34028), "tst:TRICE16 %d\n", 1 );
	TRICE16_2( Id(  472), "tst:TRICE16 %d %d\n", 1, 2 );
	TRICE16_3( Id( 1669), "tst:TRICE16 %d %d %d\n", 1, 2, 3 );
	TRICE16_4( Id(21923), "tst:TRICE16 %d %d %d %d\n", 1, 2, 3, 4 );
	`, `
	TRICE32_2( Id(42242), "tst:TRICE32   %%09x ->     %09x     %09x", 1, 0x7fffffff );
	TRICE32_2( Id(60761), "tst:     %09x     %09x\n", 0x80000000, 0xffffffff );
	TRICE32_2( Id(16891), "tst:TRICE32   %%11d ->   %11d   %11d", 1, 0x7fffffff );
	TRICE32_2( Id(53650), "tst:   %11d   %11d\n", 0x80000000, 0xffffffff  );
	TRICE32_2( Id(15285), "tst:TRICE32   %%12o ->  %12o  %12o", 1, 0x7fffffff );
	TRICE32_2( Id(61124), "tst:  %12o  %12o\n", 0x80000000, 0xffffffff );
	TRICE_S( Id(63805), "sig:generated=%s\n", x );
	TRICE_S  (   Id(44213),   "sig:generated=%s\n", x );
	`}

	listExp := `[
		{
			"id": 43274,
			"fmtType": "TRICE0",
			"fmtStrg": "---------------------------------------\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 9089,
			"fmtType": "TRICE0",
			"fmtStrg": "---------------------------------------\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 46083,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst:TRICE8 %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 50226,
			"fmtType": "TRICE8_2",
			"fmtStrg": "tst:TRICE8 %d %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 18460,
			"fmtType": "TRICE8_3",
			"fmtStrg": "tst:TRICE8 %d %d %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 55041,
			"fmtType": "TRICE8_4",
			"fmtStrg": "tst:TRICE8 %d %d %d %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 60612,
			"fmtType": "TRICE8_5",
			"fmtStrg": "tst:TRICE8 %d %d %d %d %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 16507,
			"fmtType": "TRICE8_6",
			"fmtStrg": "tst:TRICE8 %d %d %d %d %d %d \\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 59378,
			"fmtType": "TRICE8_7",
			"fmtStrg": "tst:TRICE8 %d %d %d %d %d %d %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 62543,
			"fmtType": "TRICE8_8",
			"fmtStrg": "tst:TRICE8 %d %d %d %d %d %d %d %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 34028,
			"fmtType": "TRICE16_1",
			"fmtStrg": "tst:TRICE16 %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 472,
			"fmtType": "TRICE16_2",
			"fmtStrg": "tst:TRICE16 %d %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 1669,
			"fmtType": "TRICE16_3",
			"fmtStrg": "tst:TRICE16 %d %d %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 21923,
			"fmtType": "TRICE16_4",
			"fmtStrg": "tst:TRICE16 %d %d %d %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 42242,
			"fmtType": "TRICE32_2",
			"fmtStrg": "tst:TRICE32   %%09x -\u003e     %09x     %09x",
			"created": 0,
			"removed": 0
		},
		{
			"id": 60761,
			"fmtType": "TRICE32_2",
			"fmtStrg": "tst:     %09x     %09x\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 16891,
			"fmtType": "TRICE32_2",
			"fmtStrg": "tst:TRICE32   %%11d -\u003e   %11d   %11d",
			"created": 0,
			"removed": 0
		},
		{
			"id": 53650,
			"fmtType": "TRICE32_2",
			"fmtStrg": "tst:   %11d   %11d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 15285,
			"fmtType": "TRICE32_2",
			"fmtStrg": "tst:TRICE32   %%12o -\u003e  %12o  %12o",
			"created": 0,
			"removed": 0
		},
		{
			"id": 61124,
			"fmtType": "TRICE32_2",
			"fmtStrg": "tst:  %12o  %12o\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 63805,
			"fmtType": "TRICE_S",
			"fmtStrg": "sig:generated=%s\\n",
			"created": 0,
			"removed": 0
		},	
		{
			"id": 44213,
			"fmtType": "TRICE_S",
			"fmtStrg": "sig:generated=%s\\n",
			"created": 0,
			"removed": 0
		}	
	]`
	doUpdate(t, sOri, sExp, listExp)
}

func TestUpdateZeroIds(t *testing.T) {
	id.SearchMethod = "legacy"
	sOri := []string{`
	TRICE_S( Id(0), "tst:runtime string %s.\n", x)
	`, `

	`, `

	`, `
	
	`}

	sExp := []string{`
	TRICE_S( Id(43274), "tst:runtime string %s.\n", x)
	`, `

	`, `

	`, `
	
	`}

	listExp := `[
	{
		"id": 43274,
		"fmtType": "TRICE_S",
		"fmtStrg": "tst:runtime string %s.\\n",
		"created": 0,
		"removed": 0
	}
]`
	doUpdate(t, sOri, sExp, listExp)
}
