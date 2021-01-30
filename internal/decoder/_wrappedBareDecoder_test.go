// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package decoder

import (
	"testing"

	"github.com/rokath/trice/pkg/cipher"
	"github.com/rokath/trice/pkg/tst"
)

func TestEncryptedWrappedBare(t *testing.T) {
	cipher.Password = "MySecret"
	tst.AssertNoErr(t, cipher.SetUp())
	doTableTest(t, NewBareDecoder, bigEndian, encryptedWrappedBareTestTable, "wrapped")
}

func TestWrappedBare1(t *testing.T) {
	cipher.Password = "none"
	tst.AssertNoErr(t, cipher.SetUp())
	doTableTest(t, NewBareDecoder, bigEndian, wrappedBareTestTable, "wrapped")
}

func TestWrappedBare2(t *testing.T) {
	doTableTest(t, NewBareDecoder, bigEndian, wrappedBareTestTable, "wrapped")
}

var encryptedWrappedBareTestTable = testTable{
	{[]byte{73, 30, 134, 219, 229, 12, 175, 39}, ``}, // sync trice
	{[]byte{32, 219, 85, 219, 178, 71, 211, 103, 240, 140, 22, 140, 15, 12, 178, 101}, `MSG: triceFifoMaxDepth = 4, select = 0`},
	{[]byte{234, 96, 214, 236, 57, 193, 7, 235}, `--------------------------------------------------`},
	{[]byte{15, 1, 167, 97, 54, 253, 62, 129}, `--------------------------------------------------`},
	{[]byte{22, 207, 0, 100, 192, 241, 245, 43}, `dbg:12345 as 16bit is 0b0011000000111001`},
	{[]byte{194, 189, 182, 68, 174, 241, 225, 67}, `--------------------------------------------------`},
	{[]byte{242, 42, 201, 210, 96, 41, 118, 213}, `sig:This ASSERT error is just a demo and no real error:`},
	{[]byte{145, 165, 154, 156, 144, 134, 51, 187}, `--------------------------------------------------`},
}

var wrappedBareTestTable = testTable{
	{[]byte{235, 128, 129, 235, 0, 0, 255, 254, 235, 128, 129, 235, 0, 0, 253, 252,
		235, 128, 129, 235, 0, 0, 251, 250, 235, 128, 129, 218, 90, 147, 0, 249}, `tst:TRICE8_7 -1 -2 -3 -4 -5 -6 -7`},
	{[]byte{235, 128, 129, 235, 0, 0, 255, 254, 235, 128, 129, 235, 0, 0, 253, 252,
		235, 128, 129, 235, 0, 0, 251, 250, 235, 128, 129, 96, 20, 159, 249, 248}, `tst:TRICE8_8 -1 -2 -3 -4 -5 -6 -7 -8`},
	{[]byte{235, 128, 129, 234, 137, 171, 205, 239}, ``}, // sync
	{[]byte{235, 128, 129, 78, 0, 0, 0, 164, 235, 128, 129, 239, 70, 69, 0, 6}, `MSG: triceFifoMaxDepth = 164, select = 6`},
}
