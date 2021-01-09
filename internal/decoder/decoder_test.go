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
	testTable := []struct {
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

		{[]byte{239, 205, 171, 137, 4, 0, 69, 70, 29, 0, 116, 2}, `MSG: triceFifoMaxDepth = 628, select = 29\n`},
		{[]byte{239, 205, 171, 137, 4, 0, 69, 70, 0, 0, 116, 2}, `MSG: triceFifoMaxDepth = 628, select = 0\n`},
		//{[]byte{0, 0, 161, 95}, `--------------------------------------------------\n`},
		//{[]byte{0, 0, 116, 52}, `--------------------------------------------------\n`},
		{[]byte{2, 0, 213, 70, 57, 48, 0, 0}, `dbg:12345 as 16bit is 0b0011000000111001\n`},
		//{[]byte{0, 0, 207, 14}, `--------------------------------------------------\n`},
		{[]byte{0, 0, 229, 66}, `sig:This ASSERT error is just a demo and no real error:\n`},
		//{[]byte{0, 0, 173, 104}, `--------------------------------------------------\n`},
		{[]byte{239, 205, 171, 137, 4, 0, 69, 70, 1, 0, 116, 2}, `MSG: triceFifoMaxDepth = 628, select = 1\n`},
		{[]byte{2, 0, 193, 70, 184, 179, 0, 0}, `ERR:error       message, SysTick is -19528\n`},
		{[]byte{2, 0, 129, 110, 178, 177, 0, 0}, `WRN:warning     message, SysTick is -20046\n`},
		{[]byte{2, 0, 56, 209, 172, 175, 0, 0}, `ATT:attension   message, SysTick is -20564\n`},
		{[]byte{2, 0, 32, 65, 166, 173, 0, 0}, `DIA:diagnostics message, SysTick is -21082\n`},
		{[]byte{2, 0, 222, 164, 160, 171, 0, 0}, `TIM:timing      message, SysTick is -21600\n`},
		{[]byte{2, 0, 165, 93, 154, 169, 0, 0}, `DBG:debug       message, SysTick is -22118\n`},
		{[]byte{239, 205, 171, 137, 4, 0, 69, 70, 2, 0, 116, 2}, `MSG: triceFifoMaxDepth = 628, select = 2\n`},
		{[]byte{2, 0, 162, 53, 200, 179, 0, 0}, `SIG:signal      message, SysTick is -19512\n`},
		{[]byte{2, 0, 238, 94, 194, 177, 0, 0}, `RD:read         message, SysTick is -20030\n`},
		{[]byte{2, 0, 58, 55, 188, 175, 0, 0}, `WR:write        message, SysTick is -20548\n`},
		{[]byte{2, 0, 149, 216, 182, 173, 0, 0}, `ISR:interrupt   message, SysTick is -21066\n`},
		{[]byte{2, 0, 23, 162, 176, 171, 0, 0}, `TST:test        message, SysTick is -21584\n`},
		{[]byte{2, 0, 129, 178, 170, 169, 0, 0}, `MSG:normal      message, SysTick is -22102\n`},
		{[]byte{2, 0, 13, 96, 164, 167, 0, 0}, `INFO:informal   message, SysTick is -22620\n`},
		{[]byte{4, 0, 128, 216, 158, 165, 0, 0}, `TST:test        message, SysTick is  42398\n`},
		{[]byte{4, 0, 128, 216, 159, 163, 0, 0}, `TST:test        message, SysTick is  41887\n`},
		{[]byte{4, 0, 128, 216, 159, 161, 0, 0}, `TST:test        message, SysTick is  41375\n`},
		{[]byte{4, 0, 128, 216, 160, 159, 0, 0}, `TST:test        message, SysTick is  40864\n`},
		{[]byte{4, 0, 128, 216, 160, 157, 0, 0}, `TST:test        message, SysTick is  40352\n`},
		{[]byte{4, 0, 128, 216, 156, 155, 0, 0}, `TST:test        message, SysTick is  39836\n`},
		{[]byte{4, 0, 128, 216, 157, 153, 0, 0}, `TST:test        message, SysTick is  39325\n`},
		{[]byte{239, 205, 171, 137, 4, 0, 69, 70, 3, 0, 116, 2}, `MSG: triceFifoMaxDepth = 628, select = 3\n`},
		{[]byte{4, 0, 223, 9, 255, 128, 127, 1}, `tst:TRICE8  %03x ->  001  07f  -80  -01\n`},
		{[]byte{4, 0, 191, 247, 255, 128, 127, 1}, `tst:TRICE8   %4d ->    1  127 -128   -1\n`},
		{[]byte{4, 0, 127, 156, 255, 128, 127, 1}, `tst:TRICE8   %4o ->    1  177 -200   -1\n`},
		{[]byte{8, 0, 226, 33, 255, 127, 1, 0, 255, 255, 0, 128}, `tst:TRICE16  %05x ->   00001   07fff   -8000   -0001\n`},
		{[]byte{8, 0, 221, 233, 255, 127, 1, 0, 255, 255, 0, 128}, `tst:TRICE16   %6d ->       1   32767  -32768      -1\n`},
		{[]byte{8, 0, 205, 96, 255, 127, 1, 0, 255, 255, 0, 128}, `tst:TRICE16   %7o ->       1   77777 -100000      -1\n`},
		{[]byte{239, 205, 171, 137, 4, 0, 69, 70, 4, 0, 116, 2}, `MSG: triceFifoMaxDepth = 628, select = 4\n`},
		{[]byte{16, 0, 171, 65, 1, 0, 0, 0, 255, 255, 255, 127, 0, 0, 0, 128, 255, 255, 255, 255}, `tst:TRICE32_4 %09x ->      000000001      07fffffff       -80000000     -00000001\n`},
		{[]byte{16, 0, 16, 113, 1, 0, 0, 0, 255, 255, 255, 127, 0, 0, 0, 128, 255, 255, 255, 255}, `tst:TRICE32_4 %10d ->              1     2147483647     -2147483648            -1\n`},
		{[]byte{8, 0, 105, 74, 68, 51, 34, 17, 136, 119, 102, 85}, `att:64bit 0b1000100100010001100110100010001010101011001100111011110001000\n`},
		{[]byte{239, 205, 171, 137, 4, 0, 69, 70, 6, 0, 116, 2}, `MSG: triceFifoMaxDepth = 628, select = 6\n`},
		{[]byte{239, 205, 171, 137, 4, 0, 69, 70, 7, 0, 116, 2}, `MSG: triceFifoMaxDepth = 628, select = 7\n`},
		{[]byte{2, 0, 33, 199, 255, 255, 0, 0}, `tst:TRICE16_1 -1\n`},
		{[]byte{4, 0, 112, 254, 254, 255, 255, 255}, `tst:TRICE16_2 -1 -2\n`},
		{[]byte{6, 0, 35, 230, 254, 255, 255, 255, 253, 255, 0, 0}, `tst:TRICE16_3 -1 -2 -3\n`},
		{[]byte{8, 0, 61, 176, 254, 255, 255, 255, 252, 255, 253, 255}, `tst:TRICE16_4 -1 -2 -3 -4\n`},
		{[]byte{239, 205, 171, 137, 4, 0, 69, 70, 8, 0, 116, 2}, `MSG: triceFifoMaxDepth = 628, select = 8\n`},
		{[]byte{4, 0, 240, 24, 255, 255, 255, 255}, `tst:TRICE32_1 -1\n`},
		{[]byte{4, 0, 240, 24, 255, 255, 255, 255}, `tst:TRICE32_1 -1\n`},
		{[]byte{8, 0, 241, 44, 255, 255, 255, 255, 254, 255, 255, 255}, `tst:TRICE32_2 -1 -2\n`},
		{[]byte{12, 0, 125, 219, 255, 255, 255, 255, 254, 255, 255, 255, 253, 255, 255, 255}, `tst:TRICE32_3 -1 -2 -3\n`},
		{[]byte{16, 0, 221, 142, 255, 255, 255, 255, 254, 255, 255, 255, 253, 255, 255, 255, 252, 255, 255, 255}, `tst:TRICE32_4 -1 -2 -3 -4\n`},
		{[]byte{239, 205, 171, 137, 4, 0, 69, 70, 10, 0, 116, 2}, `MSG: triceFifoMaxDepth = 628, select = 10\n`},
		{[]byte{0, 0, 146, 174, 0, 0, 191, 5, 0, 0, 83, 205, 0, 0, 164, 240, 0, 0, 163, 154}, `e:Aw:Ba:cwr:drd:e\n`},
		{[]byte{0, 0, 188, 208, 0, 0, 166, 127, 0, 0, 19, 53, 0, 0, 123, 31, 0, 0, 245, 102, 0, 0, 252, 10}, `diag:fd:Gt:Htime:imessage:Jdbg:k\n`},
		{[]byte{239, 205, 171, 137, 4, 0, 69, 70, 12, 0, 116, 2}, `MSG: triceFifoMaxDepth = 628, select = 12\n`},
		{[]byte{1, 0, 51, 161, 1, 0, 0, 0}, `tst:TRICE8_1 1\n`},
		{[]byte{2, 0, 201, 112, 2, 1, 0, 0}, `tst:TRICE8_2 1 2\n`},
		{[]byte{3, 0, 69, 72, 3, 2, 1, 0}, `tst:TRICE8_3 1 2 3\n`},
		{[]byte{4, 0, 76, 136, 4, 3, 2, 1}, `tst:TRICE8_4 1 2 3 4\n`},
		{[]byte{5, 0, 251, 7, 4, 3, 2, 1, 5, 0, 0, 0}, `tst:TRICE8_5 1 2 3 4 5\n`},
		{[]byte{6, 0, 146, 141, 4, 3, 2, 1, 6, 5, 0, 0}, `tst:TRICE8_6 1 2 3 4 5 6\n`},
		{[]byte{7, 0, 147, 90, 4, 3, 2, 1, 7, 6, 5, 0}, `tst:TRICE8_7 1 2 3 4 5 6 7\n`},
		{[]byte{8, 0, 159, 20, 4, 3, 2, 1, 8, 7, 6, 5}, `tst:TRICE8_8 1 2 3 4 5 6 7 8\n`},
		{[]byte{239, 205, 171, 137, 4, 0, 69, 70, 13, 0, 116, 2}, `MSG: triceFifoMaxDepth = 628, select = 13\n`},

		//{[]byte{8, 0, 26, 207, 101, 95, 110, 97, 112, 109, 97, 120, 8, 0, 26, 207, 115, 95, 101, 108, 110, 105, 114, 116, 2, 0, 198, 194, 10, 103, 0, 0}, `an_example_string\n`},
		//{[]byte{1, 0, 56, 141, 10, 0, 0, 0}, `\n`},
		//{[]byte{2, 0, 198, 194, 10, 97, 0, 0}, `a\n`},
		//{[]byte{3, 0, 226, 237, 10, 110, 97, 0}, `an\n`},
		//{[]byte{4, 0, 114, 226, 10, 95, 110, 97}, `an_\n`},
		//{[]byte{5, 0, 149, 194, 101, 95, 110, 97, 10, 0, 0, 0}, `an_e\n`},
		//{[]byte{6, 0, 217, 39, 101, 95, 110, 97, 10, 120, 0, 0}, `an_ex\n`},
		//{[]byte{7, 0, 95, 224, 101, 95, 110, 97, 10, 97, 120, 0}, `an_exa\n`},
		//{[]byte{8, 0, 26, 207, 101, 95, 110, 97, 10, 109, 97, 120}, `an_exam\n`},
		//{[]byte{8, 0, 26, 207, 101, 95, 110, 97, 112, 109, 97, 120, 1, 0, 56, 141, 10, 0, 0, 0}, `an_examp\n`},
		//{[]byte{8, 0, 26, 207, 101, 95, 110, 97, 112, 109, 97, 120, 2, 0, 198, 194, 10, 108, 0, 0}, `an_exampl\n`},
		//{[]byte{8, 0, 26, 207, 101, 95, 110, 97, 112, 109, 97, 120, 3, 0, 226, 237, 10, 101, 108, 0}, `an_example\n`},
		//{[]byte{8, 0, 26, 207, 101, 95, 110, 97, 112, 109, 97, 120, 4, 0, 114, 226, 10, 95, 101, 108}, `an_example_\n`},
		//{[]byte{8, 0, 26, 207, 101, 95, 110, 97, 112, 109, 97, 120, 5, 0, 149, 194, 115, 95, 101, 108, 10, 0, 0, 0}, `an_example_s\n`},
		//{[]byte{8, 0, 26, 207, 101, 95, 110, 97, 112, 109, 97, 120, 6, 0, 217, 39, 115, 95, 101, 108, 10, 116, 0, 0}, `an_example_st\n`},
		//{[]byte{8, 0, 26, 207, 101, 95, 110, 97, 112, 109, 97, 120, 7, 0, 95, 224, 115, 95, 101, 108, 10, 114, 116, 0}, `an_example_str\n`},
		//{[]byte{8, 0, 26, 207, 101, 95, 110, 97, 112, 109, 97, 120, 8, 0, 26, 207, 115, 95, 101, 108, 10, 105, 114, 116}, `an_example_stri\n`},
		//{[]byte{8, 0, 26, 207, 101, 95, 110, 97, 112, 109, 97, 120, 8, 0, 26, 207, 115, 95, 101, 108, 110, 105, 114, 116, 1, 0, 56, 141, 10, 0, 0, 0}, `an_example_strin\n`},
		//{[]byte{8, 0, 26, 207, 101, 95, 110, 97, 112, 109, 97, 120, 8, 0, 26, 207, 115, 95, 101, 108, 110, 105, 114, 116, 2, 0, 198, 194, 10, 103, 0, 0}, `an_example_string\n`},
		//{[]byte{239, 205, 171, 137, 4, 0, 69, 70, 14, 0, 116, 2}, `MSG: triceFifoMaxDepth = 628, select = 14\n`},
		//{[]byte{0, 0, 18, 0, 1, 0, 18, 0, 10, 0, 0, 0}, `\n`},
		//{[]byte{2, 0, 18, 0, 49, 10, 0, 0}, `1\n`},
		//{[]byte{3, 0, 18, 0, 49, 50, 10, 0}, `12\n`},
		//{[]byte{4, 0, 18, 0, 49, 50, 51, 10}, `123\n`},
		//{[]byte{5, 0, 18, 0, 49, 50, 51, 52, 10, 0, 0, 0}, `1234\n`},
		//{[]byte{6, 0, 18, 0, 49, 50, 51, 52, 53, 10, 0, 0}, `12345\n`},
		//{[]byte{7, 0, 18, 0, 49, 50, 51, 52, 53, 54, 10, 0}, `123456\n`},
		//{[]byte{8, 0, 18, 0, 49, 50, 51, 52, 53, 54, 55, 10}, `1234567\n`},
		//{[]byte{9, 0, 18, 0, 49, 50, 51, 52, 53, 54, 55, 56, 10, 0, 0, 0}, `12345678\n`},
		//{[]byte{10, 0, 18, 0, 49, 50, 51, 52, 53, 54, 55, 56, 57, 10, 0, 0}, `123456789\n`},
		//{[]byte{11, 0, 18, 0, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 10, 0}, `123456789A\n`},
		//{[]byte{12, 0, 18, 0, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 10}, `123456789AB\n`},
		//{[]byte{13, 0, 18, 0, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 10, 0, 0, 0}, `123456789ABC\n`},
		//{[]byte{14, 0, 18, 0, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 10, 0, 0}, `123456789ABCD\n`},
		//{[]byte{15, 0, 18, 0, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 10, 0}, `123456789ABCDE\n`},
		//{[]byte{16, 0, 18, 0, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 10}, `123456789ABCDEF\n`},
		//{[]byte{17, 0, 18, 0, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 10, 0, 0, 0}, `123456789ABCDEFG\n`},
		//{[]byte{239, 205, 171, 137, 4, 0, 69, 70, 15, 0, 116, 2}, `MSG: triceFifoMaxDepth = 628, select = 15\n`},
		//{[]byte{18, 0, 18, 0, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 10, 0, 0}, `123456789ABCDEFGH\n`},
		//{[]byte{19, 0, 18, 0, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 10, 0}, `123456789ABCDEFGHI\n`},
		//{[]byte{20, 0, 18, 0, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 10}, `123456789ABCDEFGHIJ\n`},
		//{[]byte{21, 0, 18, 0, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 48, 10, 0, 0, 0}, `123456789ABCDEFGHIJ0\n`},
		//{[]byte{22, 0, 18, 0, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 48, 49, 10, 0, 0}, `123456789ABCDEFGHIJ01\n`},
		//{[]byte{23, 0, 18, 0, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 48, 49, 50, 10, 0}, `123456789ABCDEFGHIJ012\n`},
		//{[]byte{24, 0, 18, 0, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 48, 49, 50, 51, 10}, `123456789ABCDEFGHIJ0123\n`},
		//{[]byte{25, 0, 18, 0, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 48, 49, 50, 51, 52, 10, 0, 0, 0}, `123456789ABCDEFGHIJ01234\n`},
		//{[]byte{26, 0, 18, 0, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 48, 49, 50, 51, 52, 53, 10, 0, 0}, `123456789ABCDEFGHIJ012345\n`},
		//{[]byte{27, 0, 18, 0, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 48, 49, 50, 51, 52, 53, 54, 10, 0}, `123456789ABCDEFGHIJ0123456\n`},
		//{[]byte{28, 0, 18, 0, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 48, 49, 50, 51, 52, 53, 54, 55, 10}, `123456789ABCDEFGHIJ01234567\n`},
		//{[]byte{29, 0, 18, 0, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 48, 49, 50, 51, 52, 53, 54, 55, 56, 10, 0, 0, 0}, `123456789ABCDEFGHIJ012345678\n`},
		//{[]byte{30, 0, 18, 0, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 10, 0, 0}, `123456789ABCDEFGHIJ0123456789\n`},
		//{[]byte{31}, `error: error:padding bytes not zero ignoring first byte [31 0 18 0]\n`},
		//{[]byte{0}, `error: unknown triceID 12544 ignoring first byte [0 18 0 49]\n`},
		//{[]byte{18}, `error: unknown triceID 12849 ignoring first byte [18 0 49 50]\n`},
		//{[]byte{0}, `error: unknown triceID 13106 ignoring first byte [0 49 50 51]\n`},
		//{[]byte{49}, `error: unknown triceID 13363 ignoring first byte [49 50 51 52]\n`},
		//{[]byte{50}, `error: unknown triceID 13620 ignoring first byte [50 51 52 53]\n`},
		//{[]byte{51}, `error: unknown triceID 13877 ignoring first byte [51 52 53 54]\n`},
		//{[]byte{52}, `error: unknown triceID 14134 ignoring first byte [52 53 54 55]\n`},
		//{[]byte{53}, `error: unknown triceID 14391 ignoring first byte [53 54 55 56]\n`},
		//{[]byte{54}, `error: unknown triceID 14648 ignoring first byte [54 55 56 57]\n`},
		//{[]byte{55}, `error: unknown triceID 16697 ignoring first byte [55 56 57 65]\n`},
		//{[]byte{56}, `error: unknown triceID 16961 ignoring first byte [56 57 65 66]\n`},
		//{[]byte{57}, `error: unknown triceID 17218 ignoring first byte [57 65 66 67]\n`},
		//{[]byte{65}, `error: unknown triceID 17475 ignoring first byte [65 66 67 68]\n`},
		//{[]byte{66}, `error: unknown triceID 17732 ignoring first byte [66 67 68 69]\n`},
		//{[]byte{67}, `error: unecpected byteCount, it is not 17475 ignoring first byte [67 68 69 70]\n`},
		//{[]byte{68}, `error: unknown triceID 18246 ignoring first byte [68 69 70 71]\n`},
		//{[]byte{69}, `error: unknown triceID 18503 ignoring first byte [69 70 71 72]\n`},
		//{[]byte{70}, `error: unknown triceID 18760 ignoring first byte [70 71 72 73]\n`},
		//{[]byte{71}, `error: unknown triceID 19017 ignoring first byte [71 72 73 74]\n`},
		//{[]byte{72}, `error: unknown triceID 12362 ignoring first byte [72 73 74 48]\n`},
		//{[]byte{73}, `error: unknown triceID 61232 ignoring first byte [73 74 48 239]\n`},
		//{[]byte{74}, `error: unknown triceID 52719 ignoring first byte [74 48 239 205]\n`},
		//{[]byte{48}, `error: unknown triceID 43981 ignoring first byte [48 239 205 171]\n`},
	}
	list, err := UnmarshalTriceIDList([]byte(til))
	buf := make([]byte, defaultSize)
	if err != nil {
		t.Fail()
	}
	for _, tt := range testTable {
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
