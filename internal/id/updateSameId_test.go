// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id_test

/*
func TestUpdateSameId(t *testing.T) {
	id.SearchMethod = "legacy"
	sOri := []string{`
		TRICE32_2( Id(10000), "rd_:  A { (uint32_t*) 0x%08x, 0x%08xu  },\r\n", pAddress, Value );
		`, `
		TRICE32_2( Id(10000), "rd_:  A { (uint32_t*) 0x%08x, 0x%08xu  },\r\n", pAddress, Value );
		TRICE32_2( Id(10000), "rd_:  B { (uint32_t*) 0x%08x, 0x%08xu  },\r\n", pAddress, Value );
		TRICE32_2( Id(10000), "rd_:  C { (uint32_t*) 0x%08x, 0x%08xu  },\r\n", pAddress, Value );
		`, `
		TRICE32_2( Id(10000), "rd_:  A { (uint32_t*) 0x%08x, 0x%08xu  },\r\n", pAddress, Value );
		TRICE32_2( Id(10000), "rd_:  B { (uint32_t*) 0x%08x, 0x%08xu  },\r\n", pAddress, Value );
		`, `
		TRICE32_2( Id(10000), "rd_:  C { (uint32_t*) 0x%08x, 0x%08xu  },\r\n", pAddress, Value );
		`}

	sExp := []string{`
		TRICE32_2( Id(10000), "rd_:  A { (uint32_t*) 0x%08x, 0x%08xu  },\r\n", pAddress, Value );
		`, `
		TRICE32_2( Id(10000), "rd_:  A { (uint32_t*) 0x%08x, 0x%08xu  },\r\n", pAddress, Value );
		TRICE32_2( Id(43274), "rd_:  B { (uint32_t*) 0x%08x, 0x%08xu  },\r\n", pAddress, Value );
		TRICE32_2( Id( 9089), "rd_:  C { (uint32_t*) 0x%08x, 0x%08xu  },\r\n", pAddress, Value );
		`, `
		TRICE32_2( Id(10000), "rd_:  A { (uint32_t*) 0x%08x, 0x%08xu  },\r\n", pAddress, Value );
		TRICE32_2( Id(46083), "rd_:  B { (uint32_t*) 0x%08x, 0x%08xu  },\r\n", pAddress, Value );
		`, `
		TRICE32_2( Id(50226), "rd_:  C { (uint32_t*) 0x%08x, 0x%08xu  },\r\n", pAddress, Value );
		`}

	listExp := `[
		{
			"id": 10000,
			"fmtType": "TRICE32_2",
			"fmtStrg": "rd_:  A { (uint32_t*) 0x%08x, 0x%08xu  },\\r\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 43274,
			"fmtType": "TRICE32_2",
			"fmtStrg": "rd_:  B { (uint32_t*) 0x%08x, 0x%08xu  },\\r\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 9089,
			"fmtType": "TRICE32_2",
			"fmtStrg": "rd_:  C { (uint32_t*) 0x%08x, 0x%08xu  },\\r\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 46083,
			"fmtType": "TRICE32_2",
			"fmtStrg": "rd_:  B { (uint32_t*) 0x%08x, 0x%08xu  },\\r\\n",
			"created": 0,
			"removed": 0
		},
		{
			"id": 50226,
			"fmtType": "TRICE32_2",
			"fmtStrg": "rd_:  C { (uint32_t*) 0x%08x, 0x%08xu  },\\r\\n",
			"created": 0,
			"removed": 0
		}
	]`
	doUpdate(t, sOri, sExp, listExp)
}

// TestUpdateSameId2 does not use 43274 a 2nd time here because
func TestUpdateSameId2(t *testing.T) {
	id.SearchMethod = "legacy"
	sOri := []string{`
		TRICE_S( Id(10000), "rd_: Example a %s", x );
		`, `
		TRICE_S( Id(10000), "rd_: Example a %s", x );
		TRICE_S( Id(10000), "rd_: Example b %s", x );
		TRICE_S( Id(10000), "rd_: Example c %s", x );
		`, `
		TRICE_S( Id(10000), "rd_: Example a %s", x );
		TRICE_S( Id(10000), "rd_: Example b %s", x ); // This becomes a new generated ID, because the 10000 is replaced only.
		`, `
		TRICE_S( Id(10000), "rd_: Example a %s", x );
		`}

	sExp := []string{`
		TRICE_S( Id(10000), "rd_: Example a %s", x );
		`, `
		TRICE_S( Id(10000), "rd_: Example a %s", x );
		TRICE_S( Id(43274), "rd_: Example b %s", x );
		TRICE_S( Id( 9089), "rd_: Example c %s", x );
		`, `
		TRICE_S( Id(10000), "rd_: Example a %s", x );
		TRICE_S( Id(46083), "rd_: Example b %s", x ); // This becomes a new generated ID, because the 10000 is replaced only.
		`, `
		TRICE_S( Id(10000), "rd_: Example a %s", x );
		`}

	listExp := `[
	{
		"id": 10000,
		"fmtType": "TRICE_S",
		"fmtStrg": "rd_: Example a %s",
		"created": 0,
		"removed": 0
	},
	{
		"id": 43274,
		"fmtType": "TRICE_S",
		"fmtStrg": "rd_: Example b %s",
		"created": 0,
		"removed": 0
	},
	{
		"id": 9089,
		"fmtType": "TRICE_S",
		"fmtStrg": "rd_: Example c %s",
		"created": 0,
		"removed": 0
	},
	{
		"id": 46083,
		"fmtType": "TRICE_S",
		"fmtStrg": "rd_: Example b %s",
		"created": 0,
		"removed": 0
	}
]`
	doUpdate(t, sOri, sExp, listExp)
}

func TestUpdateSameId3(t *testing.T) {
	id.SearchMethod = "legacy"
	sOri := []string{`
		TRICE_S( Id(10000), "rd_: Example a %s", x ); // stays 10000
		`, `
		TRICE_S( Id(10000), "rd_: Example a %s", x ); // stays 10000
		TRICE_S( Id(20000), "rd_: Example b %s", x ); // stays 20000
		TRICE_S( Id(10000), "rd_: Example c %s", x );
		`, `
		TRICE_S( Id(10000), "rd_: Example a %s", x ); // stays 10000
		TRICE_S( Id(20000), "rd_: Example b %s", x ); // stays 20000
		`, `
		TRICE_S( Id(10000), "rd_: Example a %s", x ); // stays 10000
		`}

	sExp := []string{`
		TRICE_S( Id(10000), "rd_: Example a %s", x ); // stays 10000
		`, `
		TRICE_S( Id(10000), "rd_: Example a %s", x ); // stays 10000
		TRICE_S( Id(20000), "rd_: Example b %s", x ); // stays 20000
		TRICE_S( Id(43274), "rd_: Example c %s", x );
		`, `
		TRICE_S( Id(10000), "rd_: Example a %s", x ); // stays 10000
		TRICE_S( Id(20000), "rd_: Example b %s", x ); // stays 20000
		`, `
		TRICE_S( Id(10000), "rd_: Example a %s", x ); // stays 10000
		`}

	listExp := `[
	{
		"id": 10000,
		"fmtType": "TRICE_S",
		"fmtStrg": "rd_: Example a %s",
		"created": 0,
		"removed": 0
	},
	{
		"id": 20000,
		"fmtType": "TRICE_S",
		"fmtStrg": "rd_: Example b %s",
		"created": 0,
		"removed": 0
	},
	{
		"id": 43274,
		"fmtType": "TRICE_S",
		"fmtStrg": "rd_: Example c %s",
		"created": 0,
		"removed": 0
	}
]`
	doUpdate(t, sOri, sExp, listExp)
}
*/
