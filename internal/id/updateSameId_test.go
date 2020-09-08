// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id_test

import "testing"

// Todo: avoid different IDs for same strings
func TestUpdateSameId(t *testing.T) {

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
