// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id_test

/*
func TestUpdateAllEqual(t *testing.T) {
	sOri := []string{`
	TRICE32_2( Id(100), "rd_:    { (uint32_t*) 0x%08x, 0x%08xu  },\r\n", pAddress, Value );
	`, `
	TRICE32_2( Id(100), "rd_:    { (uint32_t*) 0x%08x, 0x%08xu  },\r\n", pAddress, Value );
	TRICE32_2( Id(100), "rd_:    { (uint32_t*) 0x%08x, 0x%08xu  },\r\n", pAddress, Value );
	TRICE32_2( Id(100), "rd_:    { (uint32_t*) 0x%08x, 0x%08xu  },\r\n", pAddress, Value );
	`, `
	TRICE32_2( Id(100), "rd_:    { (uint32_t*) 0x%08x, 0x%08xu  },\r\n", pAddress, Value );
	TRICE32_2( Id(100), "rd_:    { (uint32_t*) 0x%08x, 0x%08xu  },\r\n", pAddress, Value );
	TRICE_S( Id(200), "sig:generated=%s\n", x );
	TRICE_S( Id(200), "sig:generated=%s\n", x );
	TRICE_S( Id(200), "sig:generated=%s\n", x );
	`, `
	TRICE32_2( Id(100), "rd_:    { (uint32_t*) 0x%08x, 0x%08xu  },\r\n", pAddress, Value );
	TRICE_S( Id(200), "sig:generated=%s\n", x );
	TRICE_S( Id(200), "sig:generated=%s\n", x );
	`}

	listExp := `[
	{
		"id": 100,
		"fmtType": "TRICE32_2",
		"fmtStrg": "rd_:    { (uint32_t*) 0x%08x, 0x%08xu  },\\r\\n",
		"created": 0,
		"removed": 0
	},
	{
		"id": 200,
		"fmtType": "TRICE_S",
		"fmtStrg": "sig:generated=%s\\n",
		"created": 0,
		"removed": 0
	}
]`
	doUpdate(t, sOri, sOri, listExp)
}

// Currently a TRICE macro must be complete with Id(0) or Id(12345) or can be without parameter count specification but must not contain an Id(0) or Id(12345) in that case.
// Examples:
// TRICE8( "%d", v) // ok
// TRICE8_1( Id(0), "%d", v) // ok
// TRICE8_1( Id(7), "%d", v) // ok
// TRICE8_1( "%d", v ) // NOT ok: remove _1 or add Id(0)
// TRICE8( Id(0), "%d", v ) // NOT ok: add _1 or remove Id(0)
func TestUpdateWithIdAndNoParamCount(t *testing.T) {
	id.SearchMethod = "legacy"
	sOri := []string{`
	TRICE32_2( Id(100), "rd_:    { (uint32_t*) 0x%08x, 0x%08xu  },\r\n", pAddress, Value ); // to do: remove _2
	`, `
	TRICE_S( "wr:fmtSting %s", x )
	`, `

	`, `

	`}

	sExp := []string{`
	TRICE32_2( Id(100), "rd_:    { (uint32_t*) 0x%08x, 0x%08xu  },\r\n", pAddress, Value ); // to do: remove _2
	`, `
	TRICE_S( Id(43274), "wr:fmtSting %s", x )
	`, `

	`, `

	`}

	listExp := `[
		{
			"id": 100,
			"fmtType": "TRICE32_2",
			"fmtStrg": "rd_:    { (uint32_t*) 0x%08x, 0x%08xu  },\\r\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 43274,
			"fmtType": "TRICE_S",
			"fmtStrg": "wr:fmtSting %s",
			"created": 0,
			"removed": 0
		}
]`
	doUpdate(t, sOri, sExp, listExp)
}
*/
