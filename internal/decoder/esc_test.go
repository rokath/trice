// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package decoder_test

import (
	"encoding/json"
	"fmt"
	"testing"

	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/id"
)

var (
	byteStream string = string([]byte{
		236, 228, 113, 16, 0, 0, 0, 1, 127, 255, 255, 255, 128, 0, 0, 0, 255, 255, 255, 255, // TRICE32_4 %10d ->              1     2147483647     -2147483648            -1
		236, 227, 74, 105, 17, 34, 51, 68, 85, 102, 119, 136, // 64bit 0b1000100100010001100110100010001010101011001100111011110001000
		236, 228, 177, 183, 0, 0, 0, 0, 0, 0, 0, 129, 0, 0, 0, 0, 0, 0, 0, 3, // MSG: triceEscFifoMaxDepth = 129, index = 3
		236, 224, 161, 51, 255, // TRICE8_1 -1
		236, 228, 177, 183, 0, 0, 0, 0, 0, 0, 0, 98, 0, 0, 0, 0, 0, 0, 0, 2,
		236, 226, 9, 223, 1, 127, 128, 255,
		236, 226, 247, 191, 1, 127, 128, 255,
		236, 226, 156, 127, 1, 127, 128, 255,
	})

	til string = `
[
	{
		"id": 28944,
		"fmtType": "TRICE32_4",
		"fmtStrg": "tst:TRICE32_4 %%10d -\u003e     %10d     %10d     %10d    %10x\\n",
		"created": 1601240704,
		"removed": 0
	},
	{
		"id": 19049,
		"fmtType": "TRICE64_1",
		"fmtStrg": "att:64bit %#b\\n",
		"created": 1601240704,
		"removed": 0
	},
	{
		"id": 41267,
		"fmtType": "TRICE8_1",
		"fmtStrg": "tst:TRICE8_1 %d\\n",
		"created": 1601240704,
		"removed": 0
	},
	{
		"id": 45495,
		"fmtType": "TRICE64_2",
		"fmtStrg": "MSG: triceEscFifoMaxDepth = %d, index = %d\\n",
		"created": 1601240705,
		"removed": 0
	},
]`
	itemList = make([]id.Item, 1<<16)
) // var

/*

	itemlist = []id.Item{
		{ID: 258, FmtType: "TRICE8_2", FmtStrg: "att:Hello, %d*%d=", Created: 0, Removed: 0}, // 1, 2, 3, 4,
		{ID: 515, FmtType: "TRICE16_1", FmtStrg: "att:%d, ok", Created: 0, Removed: 0},       // 2, 3, 0, 0xc,
		{ID: 259, FmtType: "TRICE0", FmtStrg: "?\n", Created: 0, Removed: 0},                 // 1, 3, 0, 0,
		{ID: 771, FmtType: "TRICE0", FmtStrg: "msg:Yes!\n", Created: 0, Removed: 0},          // 3, 3, 3, 3,
	}

	list = &id.List{
		FnJSON:   "",
		ItemList: itemlist,
		//savedErr: nil,
	}
)
*/

func Test1(t *testing.T) {
	var data = []byte(til)
	json.Unmarshal(data, &itemList)
	r, err := decoder.NewInputPort("BUFFER", byteStream)
	p := decoder.NewEsc(itemList, r)
	i := id.List{}
	i.ItemList := itemList
	p.list = i
	if err != nil {
		fmt.Println(p)
		t.Fail()
	}
	ss := make([]string, 100)
	n, err := p.StringsRead(ss)
	if err != nil {
		fmt.Println(p)
		t.Fail()
	}
	ss = ss[:n]
	fmt.Println(ss)
	//t.Fail()
}
