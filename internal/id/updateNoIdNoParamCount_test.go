// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id_test

import (
	"testing"

	"github.com/rokath/trice/internal/id"
)

func TestLegacyUpdateTrice0(t *testing.T) {
	id.SearchMethod = "legacy"
	sOri := []string{`
	TRICE0( "AA" );
	trice0( "BB" );
	`}
	sExp := []string{`
	TRICE0( Id(43274), "AA" );
	trice0( Id( 9089), "BB" );
	`}
	listExp := `[
	{
		"id": 43274,
		"fmtType": "TRICE0",
		"fmtStrg": "AA",
		"created": 0,
		"removed": 0
	},
	{
		"id": 9089,
		"fmtType": "trice0",
		"fmtStrg": "BB",
		"created": 0,
		"removed": 0
	}
]`
	doUpdate1(t, sOri, sExp, listExp)
}

// func TestUpdateTrice0Downward0(t *testing.T) {
// 	id.SearchMethod = "downward"
// 	sOri := []string{`
// 	TRICE0( "AA" );
// 	trice0( "CC" );
// 	`}
// 	sExp := []string{`
// 	TRICE0( Id(65535), "AA" );
// 	trice0( Id(65534), "CC" );
// 	`}
// 	listExp := `[
// 	{
// 		"id": 65535,
// 		"fmtType": "TRICE0",
// 		"fmtStrg": "AA",
// 		"created": 0,
// 		"removed": 0
// 	},
// 	{
// 		"id": 65534,
// 		"fmtType": "trice0",
// 		"fmtStrg": "CC",
// 		"created": 0,
// 		"removed": 0
// 	}
// ]`
// 	doUpdate1(t, sOri, sExp, listExp)
// }

func TestUpdateTrice0Downward1(t *testing.T) {
	id.SearchMethod = "downward"
	sOri := []string{`
	TRICE0( Id(65535), "AA" );
	trice0( "AA" );
	`}
	sExp := []string{`
	TRICE0( Id(65535), "AA" );
	trice0( Id(65534), "AA" );
	`}
	listExp := `[
	{
		"id": 65535,
		"fmtType": "TRICE0",
		"fmtStrg": "AA",
		"created": 0,
		"removed": 0
	},
	{
		"id": 65534,
		"fmtType": "trice0",
		"fmtStrg": "AA",
		"created": 0,
		"removed": 0
	}
]`
	doUpdate1(t, sOri, sExp, listExp)
}

// func _TestUpdateTrice0Downward2(t *testing.T) {
// 	id.SearchMethod = "downward"
// 	sOri := []string{`
// 	TRICE0( "AA" );
// 	trice0( Id(65535), "BB" );
// 	`}
// 	sExp := []string{`
// 	TRICE0( Id(65534), "AA" );
// 	trice0( Id(65535), "BB" );
// 	`}
// 	listExp := `[
// 	{
// 		"id": 65534,
// 		"fmtType": "TRICE0",
// 		"fmtStrg": "AA",
// 		"created": 0,
// 		"removed": 0
// 	},
// 	{
// 		"id": 65535,
// 		"fmtType": "trice0",
// 		"fmtStrg": "BB",
// 		"created": 0,
// 		"removed": 0
// 	}
// ]`
// 	doUpdate1(t, sOri, sExp, listExp)
// }

func TestLegacyUpdateNoIDNoParamCount(t *testing.T) {
	id.SearchMethod = "legacy"
	sOri := []string{`
	TRICE16( "tst:%d\n", v0 );
	TRICE16( "tst:%d, %d\n", v0v1 ); // The fmt string is important, not the param count.
	`, `
	TRICE16( "tst:%3d, %04d\n", v0, v1 ); // The fmt string is important, not the param count.
	TRICE16( "tst:%3u, %04u\n", v0, v1 ); // The fmt string is important, not the param count.
	TRICE16( "tst:%3u, %04d, %u\n", v0, v1, v2 ); // The fmt string is important, not the param count.
	TRICE16( "tst:%3u, %04d, %u, %33d\n", v0, v1, v2, v3 ); // The fmt string is important, not the param count.	
	`, `
	TRICE32( "tst:%3d, %04d\n", v0, v1 ); // The fmt string is important, not the param count.
	TRICE32( "tst:%3u, %04u\n", v0, v1 ); // The fmt string is important, not the param count.
	TRICE32( "tst:%3u, %04d, %u\n", v0, v1, v2 ); // The fmt string is important, not the param count.
	TRICE32( "tst:%3u, %04d, %u, %33d\n", v0, v1, v2, v3 ); // The fmt string is important, not the param count.	
	`, `
	TRICE64( "tst:%d\n", v0 );
	TRICE64( "tst:%3d, %04u\n", v0, v1 ); 
	`}

	sExp := []string{`
	TRICE16_1( Id(43274), "tst:%d\n", v0 );
	TRICE16_2( Id( 9089), "tst:%d, %d\n", v0v1 ); // The fmt string is important, not the param count.
	`, `
	TRICE16_2( Id(46083), "tst:%3d, %04d\n", v0, v1 ); // The fmt string is important, not the param count.
	TRICE16_2( Id(50226), "tst:%3u, %04u\n", v0, v1 ); // The fmt string is important, not the param count.
	TRICE16_3( Id(18460), "tst:%3u, %04d, %u\n", v0, v1, v2 ); // The fmt string is important, not the param count.
	TRICE16_4( Id(55041), "tst:%3u, %04d, %u, %33d\n", v0, v1, v2, v3 ); // The fmt string is important, not the param count.
	`, `
	TRICE32_2( Id(60612), "tst:%3d, %04d\n", v0, v1 ); // The fmt string is important, not the param count.
	TRICE32_2( Id(16507), "tst:%3u, %04u\n", v0, v1 ); // The fmt string is important, not the param count.
	TRICE32_3( Id(59378), "tst:%3u, %04d, %u\n", v0, v1, v2 ); // The fmt string is important, not the param count.
	TRICE32_4( Id(62543), "tst:%3u, %04d, %u, %33d\n", v0, v1, v2, v3 ); // The fmt string is important, not the param count.
	`, `
	TRICE64_1( Id(34028), "tst:%d\n", v0 );
	TRICE64_2( Id(  472), "tst:%3d, %04u\n", v0, v1 ); 
	`}

	listExp := `[
	{
		"id": 43274,
		"fmtType": "TRICE16_1",
		"fmtStrg": "tst:%d\\n",
		"created": 0,
		"removed": 0
	},
	{
		"id": 9089,
		"fmtType": "TRICE16_2",
		"fmtStrg": "tst:%d, %d\\n",
		"created": 0,
		"removed": 0
	},
	{
		"id": 46083,
		"fmtType": "TRICE16_2",
		"fmtStrg": "tst:%3d, %04d\\n",
		"created": 0,
		"removed": 0
	},
	{
		"id": 50226,
		"fmtType": "TRICE16_2",
		"fmtStrg": "tst:%3u, %04u\\n",
		"created": 0,
		"removed": 0
	},
	{
		"id": 18460,
		"fmtType": "TRICE16_3",
		"fmtStrg": "tst:%3u, %04d, %u\\n",
		"created": 0,
		"removed": 0
	},
	{
		"id": 55041,
		"fmtType": "TRICE16_4",
		"fmtStrg": "tst:%3u, %04d, %u, %33d\\n",
		"created": 0,
		"removed": 0
	},
	{
		"id": 60612,
		"fmtType": "TRICE32_2",
		"fmtStrg": "tst:%3d, %04d\\n",
		"created": 0,
		"removed": 0
	},
	{
		"id": 16507,
		"fmtType": "TRICE32_2",
		"fmtStrg": "tst:%3u, %04u\\n",
		"created": 0,
		"removed": 0
	},
	{
		"id": 59378,
		"fmtType": "TRICE32_3",
		"fmtStrg": "tst:%3u, %04d, %u\\n",
		"created": 0,
		"removed": 0
	},
	{
		"id": 62543,
		"fmtType": "TRICE32_4",
		"fmtStrg": "tst:%3u, %04d, %u, %33d\\n",
		"created": 0,
		"removed": 0
	},
	{
		"id": 34028,
		"fmtType": "TRICE64_1",
		"fmtStrg": "tst:%d\\n",
		"created": 0,
		"removed": 0
	},
	{
		"id": 472,
		"fmtType": "TRICE64_2",
		"fmtStrg": "tst:%3d, %04u\\n",
		"created": 0,
		"removed": 0
	}
]`
	doUpdate(t, sOri, sExp, listExp)
}

func TestLegacyUpdateT8NoParamCount(t *testing.T) {
	id.SearchMethod = "legacy"
	sOri := []string{`
	TRICE8( "tst:%d\n", v0 );
	TRICE8( "tst:%d, %d\n", v0v1 ); // The fmt string is important, not the param count.
	`, `
	TRICE8( "tst:%3d, %04d\n", v0, v1 ); // The fmt string is important, not the param count.
	TRICE8( "tst:%3u, %04u\n", v0, v1 ); // The fmt string is important, not the param count.
	TRICE8( "tst:%3u, %04d, %u\n", v0, v1, v2 ); // The fmt string is important, not the param count.
	TRICE8( "tst:%3u, %04d, %u, %33d\n", v0, v1, v2, v3 ); // The fmt string is important, not the param count.	
	`, `
	TRICE8( "tst:%3u, %04d, %3u, %04d, %d\n", v0, v1, v2, v3, v4 );
	TRICE8( "tst:%04d, %3u, %06d, %d, %05u, %u\n", v0, v1, v2, v3, v4, v5 ); 
	TRICE8( "tst:%04d, %3u, %04d, %d, %05u, %04d, %3u\n", v0, v1, v2, v3, v4, v5, v6 );
	TRICE8( "tst:%04d, %3u, %04d, %d, %05u, %04d, %3u, %04d\n", v0, v1, v2, v3, v4, v5, v6, v7 );	
	`, `
	TRICE8( "tst:%66u\n", v0 );
	TRICE_S( "tst:%32s\n", v0 ); 
	`}

	sExp := []string{`
	TRICE8_1( Id(43274), "tst:%d\n", v0 );
	TRICE8_2( Id( 9089), "tst:%d, %d\n", v0v1 ); // The fmt string is important, not the param count.
	`, `
	TRICE8_2( Id(46083), "tst:%3d, %04d\n", v0, v1 ); // The fmt string is important, not the param count.
	TRICE8_2( Id(50226), "tst:%3u, %04u\n", v0, v1 ); // The fmt string is important, not the param count.
	TRICE8_3( Id(18460), "tst:%3u, %04d, %u\n", v0, v1, v2 ); // The fmt string is important, not the param count.
	TRICE8_4( Id(55041), "tst:%3u, %04d, %u, %33d\n", v0, v1, v2, v3 ); // The fmt string is important, not the param count.
	`, `
	TRICE8_5( Id(60612), "tst:%3u, %04d, %3u, %04d, %d\n", v0, v1, v2, v3, v4 ); 
	TRICE8_6( Id(16507), "tst:%04d, %3u, %06d, %d, %05u, %u\n", v0, v1, v2, v3, v4, v5 ); 
	TRICE8_7( Id(59378), "tst:%04d, %3u, %04d, %d, %05u, %04d, %3u\n", v0, v1, v2, v3, v4, v5, v6 ); 
	TRICE8_8( Id(62543), "tst:%04d, %3u, %04d, %d, %05u, %04d, %3u, %04d\n", v0, v1, v2, v3, v4, v5, v6, v7 ); 
	`, `
	TRICE8_1( Id(34028), "tst:%66u\n", v0 );
	TRICE_S( Id(  472), "tst:%32s\n", v0 ); 
	`}

	listExp := `[
	{
		"id": 43274,
		"fmtType": "TRICE8_1",
		"fmtStrg": "tst:%d\\n",
		"created": 0,
		"removed": 0
	},
	{
		"id": 9089,
		"fmtType": "TRICE8_2",
		"fmtStrg": "tst:%d, %d\\n",
		"created": 0,
		"removed": 0
	},
	{
		"id": 46083,
		"fmtType": "TRICE8_2",
		"fmtStrg": "tst:%3d, %04d\\n",
		"created": 0,
		"removed": 0
	},
	{
		"id": 50226,
		"fmtType": "TRICE8_2",
		"fmtStrg": "tst:%3u, %04u\\n",
		"created": 0,
		"removed": 0
	},
	{
		"id": 18460,
		"fmtType": "TRICE8_3",
		"fmtStrg": "tst:%3u, %04d, %u\\n",
		"created": 0,
		"removed": 0
	},
	{
		"id": 55041,
		"fmtType": "TRICE8_4",
		"fmtStrg": "tst:%3u, %04d, %u, %33d\\n",
		"created": 0,
		"removed": 0
	},
	{
		"id": 60612,
		"fmtType": "TRICE8_5",
		"fmtStrg": "tst:%3u, %04d, %3u, %04d, %d\\n",
		"created": 0,
		"removed": 0
	},
	{
		"id": 16507,
		"fmtType": "TRICE8_6",
		"fmtStrg": "tst:%04d, %3u, %06d, %d, %05u, %u\\n",
		"created": 0,
		"removed": 0
	},
	{
		"id": 59378,
		"fmtType": "TRICE8_7",
		"fmtStrg": "tst:%04d, %3u, %04d, %d, %05u, %04d, %3u\\n",
		"created": 0,
		"removed": 0
	},
	{
		"id": 62543,
		"fmtType": "TRICE8_8",
		"fmtStrg": "tst:%04d, %3u, %04d, %d, %05u, %04d, %3u, %04d\\n",
		"created": 0,
		"removed": 0
	},
	{
		"id": 34028,
		"fmtType": "TRICE8_1",
		"fmtStrg": "tst:%66u\\n",
		"created": 0,
		"removed": 0
	},
	{
		"id": 472,
		"fmtType": "TRICE_S",
		"fmtStrg": "tst:%32s\\n",
		"created": 0,
		"removed": 0
	}
]`
	doUpdate(t, sOri, sExp, listExp)
}

func TestUpwardUpdateT8NoParamCount(t *testing.T) {
	id.SearchMethod = "upward"
	id.LowerBound = 7
	sOri := []string{`
	TRICE8( "tst:%d\n", v0 );
	`, `

	`, `
	
	`, `
	TRICE_S( "tst:%32s\n", v0 ); 
	`}

	sExp := []string{`
	TRICE8_1( Id(    7), "tst:%d\n", v0 );
	`, `
	
	`, `

	`, `
	TRICE_S( Id(    8), "tst:%32s\n", v0 ); 
	`}

	listExp := `[
	{
		"id": 7,
		"fmtType": "TRICE8_1",
		"fmtStrg": "tst:%d\\n",
		"created": 0,
		"removed": 0
	},
	{
		"id": 8,
		"fmtType": "TRICE_S",
		"fmtStrg": "tst:%32s\\n",
		"created": 0,
		"removed": 0
	}
]`
	doUpdate(t, sOri, sExp, listExp)
}

func TestDownwardUpdateT8NoParamCount(t *testing.T) {
	id.SearchMethod = "downward"
	id.UpperBound = 999999
	sOri := []string{`
	TRICE8( "tst:%d\n", v0 );
	`, `

	`, `
	
	`, `
	TRICE_S( "tst:%32s\n", v0 ); 
	`}

	sExp := []string{`
	TRICE8_1( Id(999999), "tst:%d\n", v0 );
	`, `
	
	`, `

	`, `
	TRICE_S( Id(999998), "tst:%32s\n", v0 ); 
	`}

	listExp := `[
	{
		"id": 999999,
		"fmtType": "TRICE8_1",
		"fmtStrg": "tst:%d\\n",
		"created": 0,
		"removed": 0
	},
	{
		"id": 999998,
		"fmtType": "TRICE_S",
		"fmtStrg": "tst:%32s\\n",
		"created": 0,
		"removed": 0
	}
]`
	doUpdate(t, sOri, sExp, listExp)
}

func TestRandomUpdateT8NoParamCount(t *testing.T) {
	id.SearchMethod = "random"
	id.UpperBound = 999999
	id.LowerBound = 100000
	sOri := []string{`
	TRICE8( "tst:%d\n", v0 );
	`, `

	`, `
	
	`, `
	TRICE_S( "tst:%32s\n", v0 ); 
	`}

	sExp := []string{`
	TRICE8_1( Id(395529), "tst:%d\n", v0 );
	`, `
	
	`, `

	`, `
	TRICE_S( Id(559098), "tst:%32s\n", v0 ); 
	`}

	listExp := `[
	{
		"id": 395529,
		"fmtType": "TRICE8_1",
		"fmtStrg": "tst:%d\\n",
		"created": 0,
		"removed": 0
	},
	{
		"id": 559098,
		"fmtType": "TRICE_S",
		"fmtStrg": "tst:%32s\\n",
		"created": 0,
		"removed": 0
	}
]`
	doUpdate(t, sOri, sExp, listExp)
}
