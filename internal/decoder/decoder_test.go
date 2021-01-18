// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package decoder

import (
	"bytes"
	"fmt"
	"io"
	"io/ioutil"
	"strings"
	"testing"

	"github.com/rokath/trice/pkg/tst"
)

// testTable ist a slice of structs generatable by the trice tool -testTable option.
type testTable []struct {
	in  []byte // byte buffer sequence
	exp string // output
}

// doTableTest is the universal decoder test sequence.
func doTableTest(t *testing.T, f newDecoder, endianness bool, teta testTable, inputDataType string) {
	list, err := UnmarshalTriceIDList([]byte(til))
	buf := make([]byte, defaultSize)
	if err != nil {
		t.Fail()
	}
	dec := f(list, nil, endianness) // p is a new decoder instance
	for _, x := range teta {
		in := ioutil.NopCloser(bytes.NewBuffer(x.in))
		if "unwrapped" == inputDataType {
			dec.setInput(in)
		} else if "wrapped" == inputDataType {
			dec.setInput(NewBareReaderFromWrap(in))
		} else {
			t.Fail()
		}
		var err error
		var n int
		var act string
		for nil == err {
			n, err = dec.Read(buf)
			if io.EOF == err && 0 == n {
				break
			}
			a := fmt.Sprint(string(buf[:n]))
			if "inf:[TRICE_SYNCPACKET 0x89abcdef]" != a {
				act += a // ignore sync packets
			}
		}
		a := strings.TrimSuffix(act, "\\n")
		ab := strings.TrimSuffix(a, "\n")
		tst.EqualLines(t, x.exp, ab)
	}
}

var (
	// til is the trace id list content for tests
	til = `[
		{
			"id": 18,
			"fmtType": "TRICE_S",
			"fmtStrg": "%s",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 19,
			"fmtType": "TRICE_S",
			"fmtStrg": "sig:%s",
			"created": 1610101603,
			"removed": 0
		},
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
			"fmtStrg": "--------------------------------------------------\\n",
			"created": 1610101603,
			"removed": 0
		},
		{
			"id": 13428,
			"fmtType": "TRICE0",
			"fmtStrg": "--------------------------------------------------\\n",
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
			"fmtStrg": "--------------------------------------------------\\n",
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
			"fmtStrg": "--------------------------------------------------\\n",
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
			"fmtStrg": "--------------------------------------------------\\n\\n",
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
		},
		{
			"id": 4950,
			"fmtType": "TRICE32_1",
			"fmtStrg": "tst:TRICE32_1 %08x\\n",
			"created": 1610910418,
			"removed": 0
		},
		{
			"id": 99,
			"fmtType": "TRICE_S",
			"fmtStrg": "%s\\n",
			"created": 1610914200,
			"removed": 0
		}
	]`
)
