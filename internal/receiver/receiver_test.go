// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// blackbox test.
package receiver

// func TestTriceItem(t *testing.T) {
// 	id.FnJSON = "./testdata/til.json"
// 	id.ReadListFile()
// 	r := bytes.NewReader([]byte{
// 		'j', 'a', 'r',
// 		0, 0, 11, 22, 0x10, 0x72, 33, 44,
// 		0x16, 0x16, 0x16, 0x16,
// 		4, 4})
// 	p := NewTriceReceiverfromBare(r)
// 	for 0 != bytes.Compare([]byte{4, 4}, p.syncbuffer) {
// 	} // wait until all is read
// 	time.Sleep(100 * time.Millisecond)
// 	tExp := id.Item{
// 		ID:      29200, //      int    `json:"id"`      // identifier
// 		FmtType: "",    // string `json:"fmtType"` // format type (bitsize and number of fmt string parameters)
// 		FmtStrg: "",    // string `json:"fmtStrg"` // format string
// 		Created: 0,     // int32  `json:"created"` // utc unix time of creation
// 		Removed: 0,     // int32  `json:"removed"` // utc unix time of disappearing in processed src directory
// 	}
// 	assertEqual(t, tExp, p.item)
// 	assertEqual(t, []byte{11, 22, 33, 44}, p.values)
// 	//	tExp := id.Item{
// 	//		ID:      29200,             //      int    `json:"id"`      // identifier
// 	//		FmtType: "TRICE32_1",       // string `json:"fmtType"` // format type (bitsize and number of fmt string parameters)
// 	//		FmtStrg: "time:ms = %d\\n", // string `json:"fmtStrg"` // format string
// 	//		Created: 1590414933,        // int32  `json:"created"` // utc unix time of creation
// 	//		Removed: 0,                 // int32  `json:"removed"` // utc unix time of disappearing in processed src directory
// 	//	}
// }
