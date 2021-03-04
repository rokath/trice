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
	var encryptedWrappedBareTestTable = testTable{
		{[]byte{73, 30, 134, 219, 229, 12, 175, 39}, ``}, // sync trice
		{[]byte{32, 219, 85, 219, 178, 71, 211, 103, 160, 147, 226, 170, 105, 114, 144, 134}, `MSG: triceFifoMaxDepth = 4, select = 0`},
		{[]byte{189, 10, 70, 10, 214, 21, 57, 186}, `dbg:12345 as 16bit is 0b0011000000111001`},
		{[]byte{149, 196, 93, 131, 165, 54, 19, 186}, `sig:This ASSERT error is just a demo and no real error:`},
		{[]byte{68, 58, 51, 8, 236, 73, 188, 234}, `--------------------------------------------------`},
	}
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

var wrappedBareTestTable = testTable{
	{[]byte{235, 128, 129, 17, 255, 84, 165, 245}, `MSG:normal      message, SysTick is -23051`},
	{[]byte{235, 128, 129, 96, 254, 75, 164, 155}, `INFO:informal   message, SysTick is -23397`},
}
