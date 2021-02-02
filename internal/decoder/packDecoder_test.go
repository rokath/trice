// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package decoder

import (
	"testing"
)

func TestPackl(t *testing.T) {
	var packlTestTable = testTable{
		{[]byte{0, 0, 24, 255}, `--------------------------------------------------`},
	}

	doTableTest(t, NewPackDecoder, littleEndian, packlTestTable, "unwrapped")
}

func TestPack(t *testing.T) {
	var packTestTable = testTable{
		{[]byte{255, 24, 0, 0}, `--------------------------------------------------`},
	}
	doTableTest(t, NewPackDecoder, bigEndian, packTestTable, "unwrapped")
}
