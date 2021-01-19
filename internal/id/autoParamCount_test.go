// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id_test

import (
	"testing"

	"github.com/rokath/trice/internal/id"
)

func TestAutoParamCount(t *testing.T) {
	id.SearchMethod = "legacy"
	sOri := []string{`
    // with count, with id, more in one line, optional semi-colon
	TRICE8_1( Id(0), "tst: %d\n", n );
	TRICE8_1( Id(0), "tst: %d\n", n ); TRICE8_1( Id(0), "tst: %d\n", n );
	TRICE8_1( Id(0), "tst: %d\n", n ); TRICE8_1( Id(0), "tst: %d\n", n ); TRICE8_1( Id(0), "tst: %d\n", n );
	TRICE8_1( Id(0), "tst: %d\n", n )
	TRICE8_1( Id(0), "tst: %d\n", n )  TRICE8_1( Id(0), "tst: %d\n", n ) 
	TRICE8_1( Id(0), "tst: %d\n", n )  TRICE8_1( Id(0), "tst: %d\n", n )  TRICE8_1( Id(0), "tst: %d\n", n ) 
	TRICE8_1( Id(0), "tst: %d\n", n ); TRICE8_1( Id(0), "tst: %d\n", n ); TRICE8_1( Id(0), "tst: %d\n", n ) 
	// without count, without id, more in one line, optional semi-colon
	TRICE8( "tst: %d\n", n );
	TRICE8( "tst: %d\n", n ); TRICE8( "tst: %d\n", n );
	TRICE8( "tst: %d\n", n ); TRICE8( "tst: %d\n", n ); TRICE8( "tst: %d\n", n );
	TRICE8( "tst: %d\n", n ) 
	TRICE8( "tst: %d\n", n )  TRICE8( "tst: %d\n", n ) 
	TRICE8( "tst: %d\n", n )  TRICE8( "tst: %d\n", n )  TRICE8( "tst: %d\n", n ) 
	TRICE8( "tst: %d\n", n ); TRICE8( "tst: %d\n", n ); TRICE8( "tst: %d\n", n ) 
	`, `
    // mixed
     TRICE8( "tst: %d\n", n );  TRICE8_1( Id(0), "tst: %d\n", n )
     TRICE8_1( Id(0), "tst: %d\n", n );  TRICE8( "tst: %d\n", n )
	`, `
    // param count
    TRICE0( "tst: Hi" );
    TRICE8( "tst:TRICE8 %d\n", 1 );
    TRICE8( "tst:TRICE8 %d %d\n", 1, 2 );
    TRICE8( "tst:TRICE8 %d %d %d\n", 1, 2, 3 );
    TRICE8( "tst:TRICE8 %d %d %d %d\n", 1, 2, 3, 4 ); 
    TRICE8( "tst:TRICE8 %d %d %d %d %d\n", 1, 2, 3, 4, 5 );
    TRICE8( "tst:TRICE8 %d %d %d %d %d %d \n", 1, 2, 3, 4, 5, 6 );
    TRICE8( "tst:TRICE8 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7 ); 
    TRICE8( "tst:TRICE8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8 ); 
    TRICE16( "tst:TRICE16 %d\n", 1 );
    TRICE16( "tst:TRICE16 %d %d\n", 1, 2 );
    TRICE16( "tst:TRICE16 %d %d %d\n", 1, 2, 3 );
    TRICE16( "tst:TRICE16 %d %d %d %d\n", 1, 2, 3, 4 );
    TRICE32( "tst:TRICE32 %d\n", 1 );
    TRICE32( "tst:TRICE32 %d %d\n", 1, 2 );
    TRICE32( "tst:TRICE32 %d %d %d\n", 1, 2, 3 );
    TRICE32( "tst:TRICE32 %d %d %d %d\n", 1, 2, 3, 4 );
    TRICE64( "tst:TRICE64 %d %d %d\n", 1, 2, 3 );
    TRICE64( "tst:TRICE64 %d %d %d %d\n", 1, 2, 3, 4 );
	`, `
	// special chars
    TRICE0( "tst: Hi>" );
    TRICE0( "tst: Hi<" );
    TRICE0( "tst: Hi|" );
    TRICE0( "tst: Hi)" );
    TRICE0( "tst: Hi(" );
    TRICE0( "tst: Hi>>" );
    TRICE0( "tst: Hi<<" );
	`}

	sExp := []string{`
    // with count, with id, more in one line, optional semi-colon
	TRICE8_1( Id(43274), "tst: %d\n", n );
	TRICE8_1( Id( 9089), "tst: %d\n", n ); TRICE8_1( Id(46083), "tst: %d\n", n );
	TRICE8_1( Id(50226), "tst: %d\n", n ); TRICE8_1( Id(18460), "tst: %d\n", n ); TRICE8_1( Id(55041), "tst: %d\n", n );
	TRICE8_1( Id(60612), "tst: %d\n", n )
	TRICE8_1( Id(16507), "tst: %d\n", n )  TRICE8_1( Id(59378), "tst: %d\n", n ) 
	TRICE8_1( Id(62543), "tst: %d\n", n )  TRICE8_1( Id(34028), "tst: %d\n", n )  TRICE8_1( Id(  472), "tst: %d\n", n ) 
	TRICE8_1( Id( 1669), "tst: %d\n", n ); TRICE8_1( Id(21923), "tst: %d\n", n ); TRICE8_1( Id(42242), "tst: %d\n", n ) 
	// without count, without id, more in one line, optional semi-colon
	TRICE8_1( Id(60761), "tst: %d\n", n );
	TRICE8_1( Id(16891), "tst: %d\n", n ); TRICE8_1( Id(53650), "tst: %d\n", n );
	TRICE8_1( Id(15285), "tst: %d\n", n ); TRICE8_1( Id(61124), "tst: %d\n", n ); TRICE8_1( Id(63805), "tst: %d\n", n );
	TRICE8_1( Id(44213), "tst: %d\n", n ) 
	TRICE8_1( Id(57421), "tst: %d\n", n )  TRICE8_1( Id(38870), "tst: %d\n", n ) 
	TRICE8_1( Id(15196), "tst: %d\n", n )  TRICE8_1( Id(56756), "tst: %d\n", n )  TRICE8_1( Id(38757), "tst: %d\n", n ) 
	TRICE8_1( Id(16985), "tst: %d\n", n ); TRICE8_1( Id(57756), "tst: %d\n", n ); TRICE8_1( Id(28737), "tst: %d\n", n ) 
	`, `
    // mixed
     TRICE8_1( Id(26897), "tst: %d\n", n );  TRICE8_1( Id(53353), "tst: %d\n", n )
     TRICE8_1( Id(43570), "tst: %d\n", n );  TRICE8_1( Id(64044), "tst: %d\n", n )
	`, `
    // param count
    TRICE0( Id(51808), "tst: Hi" );
    TRICE8_1( Id(23823), "tst:TRICE8 %d\n", 1 );
    TRICE8_2( Id(23845), "tst:TRICE8 %d %d\n", 1, 2 );
    TRICE8_3( Id(63690), "tst:TRICE8 %d %d %d\n", 1, 2, 3 );
    TRICE8_4( Id(52219), "tst:TRICE8 %d %d %d %d\n", 1, 2, 3, 4 ); 
    TRICE8_5( Id(21209), "tst:TRICE8 %d %d %d %d %d\n", 1, 2, 3, 4, 5 );
    TRICE8_6( Id(37812), "tst:TRICE8 %d %d %d %d %d %d \n", 1, 2, 3, 4, 5, 6 );
    TRICE8_7( Id(20324), "tst:TRICE8 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7 ); 
    TRICE8_8( Id(32876), "tst:TRICE8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8 ); 
    TRICE16_1( Id( 8353), "tst:TRICE16 %d\n", 1 );
    TRICE16_2( Id(11349), "tst:TRICE16 %d %d\n", 1, 2 );
    TRICE16_3( Id(30672), "tst:TRICE16 %d %d %d\n", 1, 2, 3 );
    TRICE16_4( Id(18803), "tst:TRICE16 %d %d %d %d\n", 1, 2, 3, 4 );
    TRICE32_1( Id(19070), "tst:TRICE32 %d\n", 1 );
    TRICE32_2( Id(52577), "tst:TRICE32 %d %d\n", 1, 2 );
    TRICE32_3( Id(10007), "tst:TRICE32 %d %d %d\n", 1, 2, 3 );
    TRICE32_4( Id( 2470), "tst:TRICE32 %d %d %d %d\n", 1, 2, 3, 4 );
    TRICE64_3( Id(23733), "tst:TRICE64 %d %d %d\n", 1, 2, 3 );
    TRICE64_4( Id(26384), "tst:TRICE64 %d %d %d %d\n", 1, 2, 3, 4 );
	`, `
	// special chars
    TRICE0( Id(58062), "tst: Hi>" );
    TRICE0( Id(52779), "tst: Hi<" );
    TRICE0( Id(52400), "tst: Hi|" );
    TRICE0( Id(20867), "tst: Hi)" );
    TRICE0( Id(63379), "tst: Hi(" );
    TRICE0( Id(60436), "tst: Hi>>" );
    TRICE0( Id(22429), "tst: Hi<<" );
	`}

	listExp := `[
		{
			"id": 43274,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst: %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 9089,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst: %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 46083,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst: %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 50226,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst: %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 18460,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst: %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 55041,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst: %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 60612,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst: %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 16507,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst: %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 59378,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst: %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 62543,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst: %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 34028,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst: %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 472,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst: %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 1669,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst: %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 21923,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst: %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 42242,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst: %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 60761,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst: %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 16891,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst: %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 53650,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst: %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 15285,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst: %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 61124,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst: %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 63805,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst: %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 44213,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst: %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 57421,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst: %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 38870,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst: %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 15196,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst: %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 56756,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst: %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 38757,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst: %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 16985,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst: %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 57756,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst: %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 28737,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst: %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 26897,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst: %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 53353,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst: %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 43570,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst: %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 64044,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst: %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 51808,
			"fmtType": "TRICE0",
			"fmtStrg": "tst: Hi",
			"created": 0,
			"removed": 0
		},
		{
			"id": 23823,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst:TRICE8 %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 23845,
			"fmtType": "TRICE8_2",
			"fmtStrg": "tst:TRICE8 %d %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 63690,
			"fmtType": "TRICE8_3",
			"fmtStrg": "tst:TRICE8 %d %d %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 52219,
			"fmtType": "TRICE8_4",
			"fmtStrg": "tst:TRICE8 %d %d %d %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 21209,
			"fmtType": "TRICE8_5",
			"fmtStrg": "tst:TRICE8 %d %d %d %d %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 37812,
			"fmtType": "TRICE8_6",
			"fmtStrg": "tst:TRICE8 %d %d %d %d %d %d \\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 20324,
			"fmtType": "TRICE8_7",
			"fmtStrg": "tst:TRICE8 %d %d %d %d %d %d %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 32876,
			"fmtType": "TRICE8_8",
			"fmtStrg": "tst:TRICE8 %d %d %d %d %d %d %d %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 8353,
			"fmtType": "TRICE16_1",
			"fmtStrg": "tst:TRICE16 %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 11349,
			"fmtType": "TRICE16_2",
			"fmtStrg": "tst:TRICE16 %d %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 30672,
			"fmtType": "TRICE16_3",
			"fmtStrg": "tst:TRICE16 %d %d %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 18803,
			"fmtType": "TRICE16_4",
			"fmtStrg": "tst:TRICE16 %d %d %d %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 19070,
			"fmtType": "TRICE32_1",
			"fmtStrg": "tst:TRICE32 %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 52577,
			"fmtType": "TRICE32_2",
			"fmtStrg": "tst:TRICE32 %d %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 10007,
			"fmtType": "TRICE32_3",
			"fmtStrg": "tst:TRICE32 %d %d %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 2470,
			"fmtType": "TRICE32_4",
			"fmtStrg": "tst:TRICE32 %d %d %d %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 23733,
			"fmtType": "TRICE64_3",
			"fmtStrg": "tst:TRICE64 %d %d %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 26384,
			"fmtType": "TRICE64_4",
			"fmtStrg": "tst:TRICE64 %d %d %d %d\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 58062,
			"fmtType": "TRICE0",
			"fmtStrg": "tst: Hi\u003e",
			"created": 0,
			"removed": 0
		},
		{
			"id": 52779,
			"fmtType": "TRICE0",
			"fmtStrg": "tst: Hi\u003c",
			"created": 0,
			"removed": 0
		},
		{
			"id": 52400,
			"fmtType": "TRICE0",
			"fmtStrg": "tst: Hi|",
			"created": 0,
			"removed": 0
		},
		{
			"id": 20867,
			"fmtType": "TRICE0",
			"fmtStrg": "tst: Hi)",
			"created": 0,
			"removed": 0
		},
		{
			"id": 63379,
			"fmtType": "TRICE0",
			"fmtStrg": "tst: Hi(",
			"created": 0,
			"removed": 0
		},
		{
			"id": 60436,
			"fmtType": "TRICE0",
			"fmtStrg": "tst: Hi\u003e\u003e",
			"created": 0,
			"removed": 0
		},
		{
			"id": 22429,
			"fmtType": "TRICE0",
			"fmtStrg": "tst: Hi\u003c\u003c",
			"created": 0,
			"removed": 0
		}
	]`
	doUpdate(t, sOri, sExp, listExp)
}
