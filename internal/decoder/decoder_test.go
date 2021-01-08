// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package decoder

import (
	"bytes"
	"fmt"
	"io"
	"io/ioutil"
	"testing"

	"github.com/rokath/trice/pkg/tst"
)

/*
type triceTest struct {
	in []string // byte buffer sequence
	ex string   // output
}

var triceTests []struct {
	in  []string // byte buffer sequence
	exp string   // output
}
*/

func Test1(t *testing.T) {
	triceTestTable := []struct {
		in  []byte // byte buffer
		exp string // output
	}{
		{
			[]byte{
				239, 205, 171, 137,
				4, 0, 0x45, 0x46, 10, 0, 116, 2,
			},
			"MSG: triceFifoMaxDepth = 628, select = 10\\n",
		},

		//  trice l -ts off -prefix " }, ``" -suffix "\n``}," -color off

		{[]byte{239, 205, 171, 137, 4, 0, 69, 70, 29, 0, 116, 2}, `MSG: triceFifoMaxDepth = 628, select = 29\n`},
		{[]byte{239, 205, 171, 137, 4, 0, 69, 70, 0, 0, 116, 2}, `MSG: triceFifoMaxDepth = 628, select = 0\n`},
		//{[]byte{0, 0, 161, 95}, `--------------------------------------------------\n`},
		//{[]byte{0, 0, 116, 52}, `--------------------------------------------------\n`},
		{[]byte{2, 0, 213, 70, 57, 48, 0, 0}, `dbg:12345 as 16bit is 0b0011000000111001\n`},
		//{[]byte{0, 0, 207, 14}, `--------------------------------------------------\n`},
		{[]byte{0, 0, 229, 66}, `sig:This ASSERT error is just a demo and no real error:\n`},
		//{[]byte{0, 0, 173, 104}, `--------------------------------------------------\n`},
		{[]byte{239, 205, 171, 137, 4, 0, 69, 70, 1, 0, 116, 2}, `MSG: triceFifoMaxDepth = 628, select = 1\n`},
		{[]byte{2, 0, 193, 70, 204, 179, 0, 0}, `ERR:error       message, SysTick is -19508\n`},
		{[]byte{2, 0, 129, 110, 198, 177, 0, 0}, `WRN:warning     message, SysTick is -20026\n`},
		{[]byte{2, 0, 56, 209, 192, 175, 0, 0}, `ATT:attension   message, SysTick is -20544\n`},
		{[]byte{2, 0, 32, 65, 186, 173, 0, 0}, `DIA:diagnostics message, SysTick is -21062\n`},
		{[]byte{2, 0, 222, 164, 180, 171, 0, 0}, `TIM:timing      message, SysTick is -21580\n`},
		{[]byte{2, 0, 165, 93, 174, 169, 0, 0}, `DBG:debug       message, SysTick is -22098\n`},
		{[]byte{239, 205, 171, 137, 4, 0, 69, 70, 3, 0, 116, 2}, `MSG: triceFifoMaxDepth = 628, select = 3\n`},
		{[]byte{4, 0, 223, 9, 255, 128, 127, 1}, `tst:TRICE8  %03x ->  001  07f  -80  -01\n`},
		{[]byte{4, 0, 191, 247, 255, 128, 127, 1}, `tst:TRICE8   %4d ->    1  127 -128   -1\n`},
		{[]byte{4, 0, 127, 156, 255, 128, 127, 1}, `tst:TRICE8   %4o ->    1  177 -200   -1\n`},
		{[]byte{8, 0, 226, 33, 255, 127, 1, 0, 255, 255, 0, 128}, `tst:TRICE16  %05x ->   00001   07fff   -8000   -0001\n`},
		{[]byte{8, 0, 221, 233, 255, 127, 1, 0, 255, 255, 0, 128}, `tst:TRICE16   %6d ->       1   32767  -32768      -1\n`},
		{[]byte{8, 0, 205, 96, 255, 127, 1, 0, 255, 255, 0, 128}, `tst:TRICE16   %7o ->       1   77777 -100000      -1\n`},
		{[]byte{239, 205, 171, 137, 4, 0, 69, 70, 5, 0, 116, 2}, `MSG: triceFifoMaxDepth = 628, select = 5\n`},
		{[]byte{1, 0, 51, 161, 255, 0, 0, 0}, `tst:TRICE8_1 -1\n`},
		{[]byte{2, 0, 201, 112, 254, 255, 0, 0}, `tst:TRICE8_2 -1 -2\n`},
		{[]byte{3, 0, 69, 72, 253, 254, 255, 0}, `tst:TRICE8_3 -1 -2 -3\n`},
		{[]byte{4, 0, 76, 136, 252, 253, 254, 255}, `tst:TRICE8_4 -1 -2 -3 -4\n`},
		{[]byte{5, 0, 251, 7, 252, 253, 254, 255, 251, 0, 0, 0}, `tst:TRICE8_5 -1 -2 -3 -4 -5\n`},
		{[]byte{6, 0, 146, 141, 252, 253, 254, 255, 250, 251, 0, 0}, `tst:TRICE8_6 -1 -2 -3 -4 -5 -6\n`},
		{[]byte{7, 0, 147, 90, 252, 253, 254, 255, 249, 250, 251, 0}, `tst:TRICE8_7 -1 -2 -3 -4 -5 -6 -7\n`},
		{[]byte{8, 0, 159, 20, 252, 253, 254, 255, 248, 249, 250, 251}, `tst:TRICE8_8 -1 -2 -3 -4 -5 -6 -7 -8\n`},
		{[]byte{239, 205, 171, 137, 4, 0, 69, 70, 6, 0, 116, 2}, `MSG: triceFifoMaxDepth = 628, select = 6\n`},
		{[]byte{239, 205, 171, 137, 4, 0, 69, 70, 11, 0, 116, 2}, `MSG: triceFifoMaxDepth = 628, select = 11\n`},
		{[]byte{0, 0, 6, 171, 0, 0, 201, 124, 0, 0, 33, 225, 0, 0, 117, 125, 0, 0, 156, 15, 0, 0, 159, 25, 0, 0, 15, 119}, `1234e:7m:12m:123\n`},
		{[]byte{239, 205, 171, 137, 4, 0, 69, 70, 13, 0, 116, 2}, `MSG: triceFifoMaxDepth = 628, select = 13\n`},
		//{[]byte{8, 0, 26, 207, 101, 95, 110, 97, 112, 109, 97, 120, 8, 0, 26, 207, 115, 95, 101, 108, 110, 105, 114, 116, 2, 0, 198, 194, 10, 103, 0, 0}, `an_example_string\n`},
		//	{[]byte{1, 0, 56, 141, 10, 0, 0, 0}, `\n`},
		//	{[]byte{2, 0, 198, 194, 10, 97, 0, 0}, `a\n`},
		//	{[]byte{3, 0, 226, 237, 10, 110, 97, 0}, `an\n`},
		//	{[]byte{4, 0, 114, 226, 10, 95, 110, 97}, `an_\n`},
		//	{[]byte{5, 0, 149, 194, 101, 95, 110, 97, 10, 0, 0, 0}, `an_e\n`},
		//	{[]byte{6, 0, 217, 39, 101, 95, 110, 97, 10, 120, 0, 0}, `an_ex\n`},
		//	{[]byte{7, 0, 95, 224, 101, 95, 110, 97, 10, 97, 120, 0}, `an_exa\n`},
		//	{[]byte{8, 0, 26, 207, 101, 95, 110, 97, 10, 109, 97, 120}, `an_exam\n`},
		//	{[]byte{8, 0, 26, 207, 101, 95, 110, 97, 112, 109, 97, 120, 1, 0, 56, 141, 10, 0, 0, 0}, `an_examp\n`},
		//	{[]byte{8, 0, 26, 207, 101, 95, 110, 97, 112, 109, 97, 120, 2, 0, 198, 194, 10, 108, 0, 0}, `an_exampl\n`},
		//	{[]byte{8, 0, 26, 207, 101, 95, 110, 97, 112, 109, 97, 120, 3, 0, 226, 237, 10, 101, 108, 0}, `an_example\n`},
		//	{[]byte{8, 0, 26, 207, 101, 95, 110, 97, 112, 109, 97, 120, 4, 0, 114, 226, 10, 95, 101, 108}, `an_example_\n`},
		//	{[]byte{8, 0, 26, 207, 101, 95, 110, 97, 112, 109, 97, 120, 5, 0, 149, 194, 115, 95, 101, 108, 10, 0, 0, 0}, `an_example_s\n`},
		//	{[]byte{8, 0, 26, 207, 101, 95, 110, 97, 112, 109, 97, 120, 6, 0, 217, 39, 115, 95, 101, 108, 10, 116, 0, 0}, `an_example_st\n`},
		//	{[]byte{8, 0, 26, 207, 101, 95, 110, 97, 112, 109, 97, 120, 7, 0, 95, 224, 115, 95, 101, 108, 10, 114, 116, 0}, `an_example_str\n`},
		//	{[]byte{8, 0, 26, 207, 101, 95, 110, 97, 112, 109, 97, 120, 8, 0, 26, 207, 115, 95, 101, 108, 10, 105, 114, 116}, `an_example_stri\n`},
		//	{[]byte{8, 0, 26, 207, 101, 95, 110, 97, 112, 109, 97, 120, 8, 0, 26, 207, 115, 95, 101, 108, 110, 105, 114, 116, 1, 0, 56, 141, 10, 0, 0, 0}, `an_example_strin\n`},
		//	{[]byte{8, 0, 26, 207, 101, 95, 110, 97, 112, 109, 97, 120, 8, 0, 26, 207, 115, 95, 101, 108, 110, 105, 114, 116, 2, 0, 198, 194, 10, 103, 0, 0}, `an_example_string\n`},

		{
			[]byte{
				239, 205, 171, 137,
				4, 0, 0x45, 0x46, 10, 0, 116, 2,
			},
			"MSG: triceFifoMaxDepth = 628, select = 10\\n",
		},
	}
	list, err := UnmarshalTriceIDList([]byte(til))
	buf := make([]byte, defaultSize)
	if err != nil {
		t.Fail()
	}
	for _, tt := range triceTestTable {
		rc := ioutil.NopCloser(bytes.NewBuffer(tt.in))
		p := NewPackDecoder(list, rc, littleEndian) // p is a new decoder instance
		var err error
		var n int
		var act string
		for nil == err {
			n, err = p.Read(buf)
			if io.EOF == err && 0 == n {
				break
			}
			a := fmt.Sprint(string(buf[:n]))
			if "inf:[TRICE_SYNCPACKET 0x89abcdef]" != a {
				act += a // ignore sync packets
			}
		}
		tst.EqualStrings(t, tt.exp, act)
	}
}

var (
	// til is the trace id list content for tests
	til = `[
		{
			"id": 48976,
			"fmtType": "TRICE0",
			"fmtStrg": "\\ns:                                                     \\ns:   ARM-MDK_LL_UART_RTT0_ESC_STM32F030R8_NUCLEO-64    \\ns:                                                     \\n\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 17989,
			"fmtType": "TRICE16_2",
			"fmtStrg": "MSG: triceFifoMaxDepth = %d, select = %d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 44155,
			"fmtType": "TRICE0",
			"fmtStrg": "rd_:triceBareFifoToEscFifo.c",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 46957,
			"fmtType": "TRICE0",
			"fmtStrg": "s:                                                   \\ns:                     myProject                     \\ns:                                                   \\n\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 36269,
			"fmtType": "TRICE32_2",
			"fmtStrg": "sig:%2d:%6d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 28489,
			"fmtType": "TRICE0",
			"fmtStrg": "wrn:TRICES_1(id, pFmt, dynString) macro is not supported in bare encoding.\\nmsg:See TRICE_RTS macro in triceCheck.c for an alternative or use a different encoding.\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 59283,
			"fmtType": "TRICE0",
			"fmtStrg": "rd_:triceCheck.c",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 36152,
			"fmtType": "TRICE8_1",
			"fmtStrg": "%c",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 49862,
			"fmtType": "TRICE8_2",
			"fmtStrg": "%c%c",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 60898,
			"fmtType": "TRICE8_3",
			"fmtStrg": "%c%c%c",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 57970,
			"fmtType": "TRICE8_4",
			"fmtStrg": "%c%c%c%c",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 49813,
			"fmtType": "TRICE8_5",
			"fmtStrg": "%c%c%c%c%c",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 10201,
			"fmtType": "TRICE8_6",
			"fmtStrg": "%c%c%c%c%c%c",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 57439,
			"fmtType": "TRICE8_7",
			"fmtStrg": "%c%c%c%c%c%c%c",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 53018,
			"fmtType": "TRICE8_8",
			"fmtStrg": "%c%c%c%c%c%c%c%c",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 24481,
			"fmtType": "TRICE0",
			"fmtStrg": "--------------------------------------------------\\r\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 13428,
			"fmtType": "TRICE0",
			"fmtStrg": "--------------------------------------------------\\r\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 18133,
			"fmtType": "TRICE16_1",
			"fmtStrg": "dbg:12345 as 16bit is %#016b\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 3791,
			"fmtType": "TRICE0",
			"fmtStrg": "--------------------------------------------------\\r\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 17125,
			"fmtType": "TRICE0",
			"fmtStrg": "sig:This ASSERT error is just a demo and no real error:\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 26797,
			"fmtType": "TRICE0",
			"fmtStrg": "--------------------------------------------------\\r\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 18113,
			"fmtType": "TRICE16_1",
			"fmtStrg": "ERR:error       message, SysTick is %6d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 28289,
			"fmtType": "TRICE16_1",
			"fmtStrg": "WRN:warning     message, SysTick is %6d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 53560,
			"fmtType": "TRICE16_1",
			"fmtStrg": "ATT:attension   message, SysTick is %6d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 16672,
			"fmtType": "TRICE16_1",
			"fmtStrg": "DIA:diagnostics message, SysTick is %6d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 42206,
			"fmtType": "TRICE16_1",
			"fmtStrg": "TIM:timing      message, SysTick is %6d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 23973,
			"fmtType": "TRICE16_1",
			"fmtStrg": "DBG:debug       message, SysTick is %6d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 13730,
			"fmtType": "TRICE16_1",
			"fmtStrg": "SIG:signal      message, SysTick is %6d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 24302,
			"fmtType": "TRICE16_1",
			"fmtStrg": "RD:read         message, SysTick is %6d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 14138,
			"fmtType": "TRICE16_1",
			"fmtStrg": "WR:write        message, SysTick is %6d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 55445,
			"fmtType": "TRICE16_1",
			"fmtStrg": "ISR:interrupt   message, SysTick is %6d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 41495,
			"fmtType": "TRICE16_1",
			"fmtStrg": "TST:test        message, SysTick is %6d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 45697,
			"fmtType": "TRICE16_1",
			"fmtStrg": "MSG:normal      message, SysTick is %6d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 24589,
			"fmtType": "TRICE16_1",
			"fmtStrg": "INFO:informal   message, SysTick is %6d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 55424,
			"fmtType": "TRICE32_1",
			"fmtStrg": "TST:test        message, SysTick is %6d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 2527,
			"fmtType": "TRICE8_4",
			"fmtStrg": "tst:TRICE8  %%03x -\u003e  %03x  %03x  %03x  %03x\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 63423,
			"fmtType": "TRICE8_4",
			"fmtStrg": "tst:TRICE8   %%4d -\u003e %4d %4d %4d %4d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 40063,
			"fmtType": "TRICE8_4",
			"fmtStrg": "tst:TRICE8   %%4o -\u003e %4o %4o %4o %4o\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 8674,
			"fmtType": "TRICE16_4",
			"fmtStrg": "tst:TRICE16  %%05x -\u003e   %05x   %05x   %05x   %05x\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 59869,
			"fmtType": "TRICE16_4",
			"fmtStrg": "tst:TRICE16   %%6d -\u003e  %6d  %6d  %6d  %6d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 24781,
			"fmtType": "TRICE16_4",
			"fmtStrg": "tst:TRICE16   %%7o -\u003e %7o %7o %7o %7o\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 16811,
			"fmtType": "TRICE32_4",
			"fmtStrg": "tst:TRICE32_4 %%09x -\u003e      %09x      %09x       %09x     %09x\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 28944,
			"fmtType": "TRICE32_4",
			"fmtStrg": "tst:TRICE32_4 %%10d -\u003e     %10d     %10d     %10d    %10x\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 19049,
			"fmtType": "TRICE64_1",
			"fmtStrg": "att:64bit %#b\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 41267,
			"fmtType": "TRICE8_1",
			"fmtStrg": "tst:TRICE8_1 %d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 28873,
			"fmtType": "TRICE8_2",
			"fmtStrg": "tst:TRICE8_2 %d %d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 18501,
			"fmtType": "TRICE8_3",
			"fmtStrg": "tst:TRICE8_3 %d %d %d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 34892,
			"fmtType": "TRICE8_4",
			"fmtStrg": "tst:TRICE8_4 %d %d %d %d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 2043,
			"fmtType": "TRICE8_5",
			"fmtStrg": "tst:TRICE8_5 %d %d %d %d %d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 36242,
			"fmtType": "TRICE8_6",
			"fmtStrg": "tst:TRICE8_6 %d %d %d %d %d %d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 23187,
			"fmtType": "TRICE8_7",
			"fmtStrg": "tst:TRICE8_7 %d %d %d %d %d %d %d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 5279,
			"fmtType": "TRICE8_8",
			"fmtStrg": "tst:TRICE8_8 %d %d %d %d %d %d %d %d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 50977,
			"fmtType": "TRICE16_1",
			"fmtStrg": "tst:TRICE16_1 %d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 65136,
			"fmtType": "TRICE16_2",
			"fmtStrg": "tst:TRICE16_2 %d %d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 58915,
			"fmtType": "TRICE16_3",
			"fmtStrg": "tst:TRICE16_3 %d %d %d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 45117,
			"fmtType": "TRICE16_4",
			"fmtStrg": "tst:TRICE16_4 %d %d %d %d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 6384,
			"fmtType": "TRICE32_1",
			"fmtStrg": "tst:TRICE32_1 %d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 11505,
			"fmtType": "TRICE32_2",
			"fmtStrg": "tst:TRICE32_2 %d %d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 56189,
			"fmtType": "TRICE32_3",
			"fmtStrg": "tst:TRICE32_3 %d %d %d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 36573,
			"fmtType": "TRICE32_4",
			"fmtStrg": "tst:TRICE32_4 %d %d %d %d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 43171,
			"fmtType": "TRICE64_1",
			"fmtStrg": "tst:TRICE64_1 %d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 8402,
			"fmtType": "TRICE64_2",
			"fmtStrg": "tst:TRICE64_2 %d %d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 44690,
			"fmtType": "TRICE0",
			"fmtStrg": "e:A",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 1471,
			"fmtType": "TRICE0",
			"fmtStrg": "w:B",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 52563,
			"fmtType": "TRICE0",
			"fmtStrg": "a:c",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 61604,
			"fmtType": "TRICE0",
			"fmtStrg": "wr:d",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 39587,
			"fmtType": "TRICE0",
			"fmtStrg": "rd:e\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 53436,
			"fmtType": "TRICE0",
			"fmtStrg": "diag:f",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 32678,
			"fmtType": "TRICE0",
			"fmtStrg": "d:G",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 13587,
			"fmtType": "TRICE0",
			"fmtStrg": "t:H",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 8059,
			"fmtType": "TRICE0",
			"fmtStrg": "time:i",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 26357,
			"fmtType": "TRICE0",
			"fmtStrg": "message:J",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 2812,
			"fmtType": "TRICE0",
			"fmtStrg": "dbg:k\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 43782,
			"fmtType": "TRICE0",
			"fmtStrg": "1",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 31945,
			"fmtType": "TRICE0",
			"fmtStrg": "2",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 57633,
			"fmtType": "TRICE0",
			"fmtStrg": "3",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 32117,
			"fmtType": "TRICE0",
			"fmtStrg": "4",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 3996,
			"fmtType": "TRICE0",
			"fmtStrg": "e:7",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 6559,
			"fmtType": "TRICE0",
			"fmtStrg": "m:12",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 30479,
			"fmtType": "TRICE0",
			"fmtStrg": "m:123\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 2502,
			"fmtType": "TRICE8_8",
			"fmtStrg": "msg:1:%03x %03x %03x %03x %03x %03x %03x %03x\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 20625,
			"fmtType": "TRICE16_1",
			"fmtStrg": "tim: pre encryption SysTick=%d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 64368,
			"fmtType": "TRICE16_1",
			"fmtStrg": "tim: post encryption SysTick=%d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 43335,
			"fmtType": "TRICE8_8",
			"fmtStrg": "att:1:%03x %03x %03x %03x %03x %03x %03x %03x\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 56697,
			"fmtType": "TRICE16_1",
			"fmtStrg": "tim: pre decryption SysTick=%d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 50168,
			"fmtType": "TRICE16_1",
			"fmtStrg": "tim: post decryption SysTick=%d\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 990,
			"fmtType": "TRICE8_8",
			"fmtStrg": "msg:2:%03x %03x %03x %03x %03x %03x %03x %03x\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 7275,
			"fmtType": "TRICE0",
			"fmtStrg": "--------------------------------------------------\\r\\n\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 13515,
			"fmtType": "TRICE0",
			"fmtStrg": "rd_:triceFifo.c",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 30152,
			"fmtType": "TRICE0",
			"fmtStrg": "rd_:triceFifoToBytesBuffer.c",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 54823,
			"fmtType": "TRICE0",
			"fmtStrg": "s:                                                   \\ns:   MDK-ARM_LL_UART_RTT0_BARE_STM32F030_NUCLEO-64   \\ns:                                                   \\n\\n",
			"created": 1610101610,
			"removed": 0
		},
		{
			"id": 44461,
			"fmtType": "TRICE0",
			"fmtStrg": "s:                                                          \\ns:   MDK-ARM_LL_UART_WRAP_RTT0_BARE_STM32F030R8-NUCLEO-64   \\ns:                                                          \\n\\n",
			"created": 1610101611,
			"removed": 0
		},
		{
			"id": 46928,
			"fmtType": "TRICE0",
			"fmtStrg": "s:                                                   \\ns:   MDK-ARM_LL_UART_RTT0_PACK_STM32F030_NUCLEO-64   \\ns:                                                   \\n\\n",
			"created": 1610101612,
			"removed": 0
		}
	]`
)
