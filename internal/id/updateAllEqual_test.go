// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id_test

import "testing"

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
	`, `
	TRICE32_2( Id(100), "rd_:    { (uint32_t*) 0x%08x, 0x%08xu  },\r\n", pAddress, Value );
	`}

	listExp := `[
		{
			"id": 100,
			"fmtType": "TRICE32_2",
			"fmtStrg": "rd_:    { (uint32_t*) 0x%08x, 0x%08xu  },\\r\\n",
			"created": 0,
			"removed": 0
		}
	]`
	doUpdate(t, sOri, sOri, listExp)
}
