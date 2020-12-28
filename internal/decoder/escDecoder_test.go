// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package decoder_test

import (
	"fmt"
	"testing"

	"github.com/stretchr/testify/assert"

	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/internal/receiver"
)

var (
	// byteStreamEsc are the raw read input bytes for esc format tests
	byteStreamEsc string = string([]byte{
		236, 228, 113, 16, 0, 0, 0, 1, 127, 255, 255, 255, 128, 0, 0, 0, 255, 255, 255, 255, // TRICE32_4 %10d ->              1     2147483647     -2147483648            -1
		236, 227, 74, 105, 17, 34, 51, 68, 85, 102, 119, 136, // 64bit 0b1000100100010001100110100010001010101011001100111011110001000
		236, 228, 177, 183, 0, 0, 0, 0, 0, 0, 0, 129, 0, 0, 0, 0, 0, 0, 0, 3, // MSG: triceEscFifoMaxDepth = 129, index = 3
		236, 224, 161, 51, 255, // TRICE8_1 -1
		236, 228, 177, 183, 0, 0, 0, 0, 0, 0, 0, 98, 0, 0, 0, 0, 0, 0, 0, 2, // MSG: triceEscFifoMaxDepth = 98, index = 2
	})
)

func TestEsc(t *testing.T) {

	// rc is created ReadCloser
	rc, err := receiver.NewReader("BUFFER", byteStreamEsc)
	if err != nil {
		t.Fail()
	}

	list, err := decoder.UnmarshalTriceIDList([]byte(til))
	if err != nil {
		t.Fail()
	}

	p := decoder.NewEscFormat(list, rc) // p is a new decoder instance

	ss := make([]string, 100)
	n, err := p.StringsRead(ss)
	if err != nil {
		t.Fail()
	}
	ss = ss[:n]
	act := fmt.Sprintln(ss)
	exp := "[tst:TRICE32_4 %10d ->              1     2147483647     -2147483648            -1\\n att:64bit 0b1000100100010001100110100010001010101011001100111011110001000\\n MSG: triceEscFifoMaxDepth = 129, index = 3\\n tst:TRICE8_1 -1\\n MSG: triceEscFifoMaxDepth = 98, index = 2\\n]\n"
	assert.Equal(t, exp, act)
}

func checkDynString(t *testing.T, list []id.Item, in, exp string) {
	// rc is created ReadCloser
	rc, err := receiver.NewReader("BUFFER", in)
	if err != nil {
		t.Fail()
	}

	p := decoder.NewEscFormat(list, rc) // p is a new decoder instance

	ss := make([]string, 100)
	n, err := p.StringsRead(ss)
	if err != nil {
		t.Fail()
	}
	ss = ss[:n]
	act := fmt.Sprintln(ss)

	assert.Equal(t, exp, act)
}

func TestEscDynStrings(t *testing.T) {
	list, err := decoder.UnmarshalTriceIDList([]byte(til))
	if err != nil {
		t.Fail()
	}

	var in, exp string

	in = string([]byte{236, 224, 0, 18, 0})
	exp = "[]\n"
	checkDynString(t, list, in, exp)

	in = string([]byte{236, 225, 0, 18, 10, 0})
	exp = "[\n]\n"
	checkDynString(t, list, in, exp)

	in = string([]byte{236, 226, 0, 18, 49, 10, 0, 0})
	exp = "[1\n]\n"
	checkDynString(t, list, in, exp)

	in = string([]byte{236, 226, 0, 18, 49, 50, 10, 0})
	exp = "[12\n]\n"
	checkDynString(t, list, in, exp)

	in = string([]byte{236, 227, 0, 18, 49, 50, 51, 10, 0, 0, 0, 0})
	exp = "[123\n]\n"
	checkDynString(t, list, in, exp)

	in = string([]byte{236, 227, 0, 18, 49, 50, 51, 52, 10, 0, 0, 0})
	exp = "[1234\n]\n"
	checkDynString(t, list, in, exp)

	in = string([]byte{236, 227, 0, 18, 49, 50, 51, 52, 53, 10, 0, 0})
	exp = "[12345\n]\n"
	checkDynString(t, list, in, exp)

	in = string([]byte{236, 227, 0, 18, 49, 50, 51, 52, 53, 54, 10, 0})
	exp = "[123456\n]\n"
	checkDynString(t, list, in, exp)

	in = string([]byte{236, 228, 0, 18, 49, 50, 51, 52, 53, 54, 55, 10, 0, 0, 0, 0, 0, 0, 0, 0})
	exp = "[1234567\n]\n"
	checkDynString(t, list, in, exp)

	in = string([]byte{236, 228, 0, 18, 49, 50, 51, 52, 53, 54, 55, 56, 10, 0, 0, 0, 0, 0, 0, 0})
	exp = "[12345678\n]\n"
	checkDynString(t, list, in, exp)

	in = string([]byte{236, 228, 0, 18, 49, 50, 51, 52, 53, 54, 55, 56, 57, 10, 0, 0, 0, 0, 0, 0})
	exp = "[123456789\n]\n"
	checkDynString(t, list, in, exp)

	in = string([]byte{236, 228, 0, 18, 49, 50, 51, 52, 53, 54, 55, 56, 57, 97, 10, 0, 0, 0, 0, 0})
	exp = "[123456789a\n]\n"
	checkDynString(t, list, in, exp)

	in = string([]byte{236, 228, 0, 18, 49, 50, 51, 52, 53, 54, 55, 56, 57, 97, 98, 10, 0, 0, 0, 0})
	exp = "[123456789ab\n]\n"
	checkDynString(t, list, in, exp)

	in = string([]byte{236, 228, 0, 18, 49, 50, 51, 52, 53, 54, 55, 56, 57, 97, 98, 99, 10, 0, 0, 0})
	exp = "[123456789abc\n]\n"
	checkDynString(t, list, in, exp)

	in = string([]byte{236, 228, 0, 18, 49, 50, 51, 52, 53, 54, 55, 56, 57, 97, 98, 99, 100, 10, 0, 0})
	exp = "[123456789abcd\n]\n"
	checkDynString(t, list, in, exp)

	in = string([]byte{236, 228, 0, 18, 49, 50, 51, 52, 53, 54, 55, 56, 57, 97, 98, 99, 100, 101, 10, 0})
	exp = "[123456789abcde\n]\n"
	checkDynString(t, list, in, exp)

	in = string([]byte{236, 228, 0, 18, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 10, 0})
	exp = "[123456789ABCDE\n]\n"
	checkDynString(t, list, in, exp)

	in = string([]byte{236, 229, 0, 18, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0})
	exp = "[123456789ABCDEF\n]\n"
	checkDynString(t, list, in, exp)

	in = string([]byte{236, 229, 0, 18, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 10, 0})
	exp = "[123456789ABCDEFGHIJKLMNOPQRSTU\n]\n"
	checkDynString(t, list, in, exp)

	in = string([]byte{236, 230, 0, 18,
		49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 10,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0})
	exp = "[123456789ABCDEFGHIJKLMNOPQRSTUV\n]\n"
	checkDynString(t, list, in, exp)

	in = string([]byte{236, 230, 0, 18,
		49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
		10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0})
	exp = "[123456789ABCDEFGHIJKLMNOPQRSTUVW\n]\n"
	checkDynString(t, list, in, exp)

	in = string([]byte{236, 231, 0, 18,
		49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
		49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
		49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
		10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0})
	exp = "[123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW\n]\n"
	checkDynString(t, list, in, exp)

	in = string([]byte{236, 231, 0, 18,
		49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
		49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
		49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
		49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 0, 0})
	exp = "[123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTU]\n"
	checkDynString(t, list, in, exp)

	in = string([]byte{236, 231, 0, 18,
		49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
		49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
		49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
		49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 0})
	exp = "[123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUV]\n"
	checkDynString(t, list, in, exp)

	in = string([]byte{236, 232, 0, 18,
		49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
		49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
		49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
		49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
		49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
		49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
		49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
		49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 0})
	exp = "[123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUV]\n"
	checkDynString(t, list, in, exp)

	in = string([]byte{236, 232, 0, 18,
		49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
		49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
		49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
		49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
		49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
		49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
		49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
		49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 0, 0})
	exp = "[123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTU]\n"
	checkDynString(t, list, in, exp)

}
