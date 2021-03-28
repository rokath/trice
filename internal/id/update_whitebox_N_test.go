// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.
package id

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

// A wrong parameter count should not be corrected! THe compiler will complain and a decision should be made.
func TestDoNotCorrectWrongParamCountSingle(t *testing.T) {
	tt := []struct{ text, exp string }{
		{
			`Trice8_2( Id(0), "hi %2d",1  );`,
			`Trice8_2( Id(0), "hi %2d",1  );`},
		{
			`TRICE8_2( Id(0), "hi %2d, %13u, %64b, %8x %02d, %013u, %032b, %016x",1,2,3,4,5,6,7,8 );`,
			`TRICE8_2( Id(0), "hi %2d, %13u, %64b, %8x %02d, %013u, %032b, %016x",1,2,3,4,5,6,7,8 );`},
	}
	checkTestTable(t, tt, true)
}

func TestInsertParamCountAndIDNoParam(t *testing.T) {
	tt := []struct{ text, exp string }{
		{`... Trice0 ( "hi"); ...`, `... Trice0 ( Id(0), "hi"); ...`},
		{`... TRICE0 ( "hi"); ...`, `... TRICE0 ( Id(0), "hi"); ...`},
		{`... trice0 ( "hi"); ...`, `... trice0 ( Id(0), "hi"); ...`},
		{`... Trice0i ( "hi"); ...`, `... Trice0i ( Id(0), "hi"); ...`},
		{`... TRICE0i ( "hi"); ...`, `... TRICE0i ( Id(0), "hi"); ...`},
		{`... trice0i ( "hi"); ...`, `... trice0i ( Id(0), "hi"); ...`},
	}
	checkTestTable(t, tt, true)
}

func TestInsertParamCountAndIDOneParamN(t *testing.T) {
	tt := []struct{ text, exp string }{
		{`... Trice8 ( "hi %03u", 5); ...`, `... Trice8_1 ( Id(0), "hi %03u", 5); ...`},
		{`... TRICE8 ( "hi %03u", 5); ...`, `... TRICE8_1 ( Id(0), "hi %03u", 5); ...`},
		{`... trice8 ( "hi %03u", 5); ...`, `... trice8_1 ( Id(0), "hi %03u", 5); ...`},
		{`... Trice16 ( "hi %03u", 5); ...`, `... Trice16_1 ( Id(0), "hi %03u", 5); ...`},
		{`... TRICE16 ( "hi %03u", 5); ...`, `... TRICE16_1 ( Id(0), "hi %03u", 5); ...`},
		{`... trice16 ( "hi %03u", 5); ...`, `... trice16_1 ( Id(0), "hi %03u", 5); ...`},
		{`... Trice32 ( "hi %03u", 5); ...`, `... Trice32_1 ( Id(0), "hi %03u", 5); ...`},
		{`... TRICE32 ( "hi %03u", 5); ...`, `... TRICE32_1 ( Id(0), "hi %03u", 5); ...`},
		{`... trice32 ( "hi %03u", 5); ...`, `... trice32_1 ( Id(0), "hi %03u", 5); ...`},
		{`... Trice64 ( "hi %03u", 5); ...`, `... Trice64_1 ( Id(0), "hi %03u", 5); ...`},
		{`... TRICE64 ( "hi %03u", 5); ...`, `... TRICE64_1 ( Id(0), "hi %03u", 5); ...`},
		{`... trice64 ( "hi %03u", 5); ...`, `... trice64_1 ( Id(0), "hi %03u", 5); ...`},
		{`... Trice8i ( "hi %03u", 5); ...`, `... Trice8_1i ( Id(0), "hi %03u", 5); ...`},
		{`... TRICE8i ( "hi %03u", 5); ...`, `... TRICE8_1i ( Id(0), "hi %03u", 5); ...`},
		{`... trice8i ( "hi %03u", 5); ...`, `... trice8_1i ( Id(0), "hi %03u", 5); ...`},
		{`... Trice16i ( "hi %03u", 5); ...`, `... Trice16_1i ( Id(0), "hi %03u", 5); ...`},
		{`... TRICE16i ( "hi %03u", 5); ...`, `... TRICE16_1i ( Id(0), "hi %03u", 5); ...`},
		{`... trice16i ( "hi %03u", 5); ...`, `... trice16_1i ( Id(0), "hi %03u", 5); ...`},
		{`... Trice32i ( "hi %03u", 5); ...`, `... Trice32_1i ( Id(0), "hi %03u", 5); ...`},
		{`... TRICE32i ( "hi %03u", 5); ...`, `... TRICE32_1i ( Id(0), "hi %03u", 5); ...`},
		{`... trice32i ( "hi %03u", 5); ...`, `... trice32_1i ( Id(0), "hi %03u", 5); ...`},
		{`... Trice64i ( "hi %03u", 5); ...`, `... Trice64_1i ( Id(0), "hi %03u", 5); ...`},
		{`... TRICE64i ( "hi %03u", 5); ...`, `... TRICE64_1i ( Id(0), "hi %03u", 5); ...`},
		{`... trice64i ( "hi %03u", 5); ...`, `... trice64_1i ( Id(0), "hi %03u", 5); ...`},
	}
	checkTestTable(t, tt, true)
}

func TestInsertParamCountAndIDOneParam(t *testing.T) {
	tt := []struct{ text, exp string }{
		{`... Trice8 ( "hi %03u", 5); ...`, `... Trice8 ( Id(0), "hi %03u", 5); ...`},
		{`... TRICE8 ( "hi %03u", 5); ...`, `... TRICE8 ( Id(0), "hi %03u", 5); ...`},
		{`... trice8 ( "hi %03u", 5); ...`, `... trice8 ( Id(0), "hi %03u", 5); ...`},
		{`... Trice16 ( "hi %03u", 5); ...`, `... Trice16 ( Id(0), "hi %03u", 5); ...`},
		{`... TRICE16 ( "hi %03u", 5); ...`, `... TRICE16 ( Id(0), "hi %03u", 5); ...`},
		{`... trice16 ( "hi %03u", 5); ...`, `... trice16 ( Id(0), "hi %03u", 5); ...`},
		{`... Trice32 ( "hi %03u", 5); ...`, `... Trice32 ( Id(0), "hi %03u", 5); ...`},
		{`... TRICE32 ( "hi %03u", 5); ...`, `... TRICE32 ( Id(0), "hi %03u", 5); ...`},
		{`... trice32 ( "hi %03u", 5); ...`, `... trice32 ( Id(0), "hi %03u", 5); ...`},
		{`... Trice64 ( "hi %03u", 5); ...`, `... Trice64 ( Id(0), "hi %03u", 5); ...`},
		{`... TRICE64 ( "hi %03u", 5); ...`, `... TRICE64 ( Id(0), "hi %03u", 5); ...`},
		{`... trice64 ( "hi %03u", 5); ...`, `... trice64 ( Id(0), "hi %03u", 5); ...`},
		{`... Trice8i ( "hi %03u", 5); ...`, `... Trice8i ( Id(0), "hi %03u", 5); ...`},
		{`... TRICE8i ( "hi %03u", 5); ...`, `... TRICE8i ( Id(0), "hi %03u", 5); ...`},
		{`... trice8i ( "hi %03u", 5); ...`, `... trice8i ( Id(0), "hi %03u", 5); ...`},
		{`... Trice16i ( "hi %03u", 5); ...`, `... Trice16i ( Id(0), "hi %03u", 5); ...`},
		{`... TRICE16i ( "hi %03u", 5); ...`, `... TRICE16i ( Id(0), "hi %03u", 5); ...`},
		{`... trice16i ( "hi %03u", 5); ...`, `... trice16i ( Id(0), "hi %03u", 5); ...`},
		{`... Trice32i ( "hi %03u", 5); ...`, `... Trice32i ( Id(0), "hi %03u", 5); ...`},
		{`... TRICE32i ( "hi %03u", 5); ...`, `... TRICE32i ( Id(0), "hi %03u", 5); ...`},
		{`... trice32i ( "hi %03u", 5); ...`, `... trice32i ( Id(0), "hi %03u", 5); ...`},
		{`... Trice64i ( "hi %03u", 5); ...`, `... Trice64i ( Id(0), "hi %03u", 5); ...`},
		{`... TRICE64i ( "hi %03u", 5); ...`, `... TRICE64i ( Id(0), "hi %03u", 5); ...`},
		{`... trice64i ( "hi %03u", 5); ...`, `... trice64i ( Id(0), "hi %03u", 5); ...`},
	}
	checkTestTable(t, tt, false)
}

func TestInsertParamCountAndIDTwoParamN(t *testing.T) {
	tt := []struct{ text, exp string }{
		{`...   Trice8 ( "hi %03u %03d", 5, 7 ); ...`, `...   Trice8_2 ( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...   TRICE8 ( "hi %03u %03d", 5, 7 ); ...`, `...   TRICE8_2 ( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...   trice8 ( "hi %03u %03d", 5, 7 ); ...`, `...   trice8_2 ( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  Trice16 ( "hi %03u %03d", 5, 7 ); ...`, `...  Trice16_2 ( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  TRICE16 ( "hi %03u %03d", 5, 7 ); ...`, `...  TRICE16_2 ( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  trice16 ( "hi %03u %03d", 5, 7 ); ...`, `...  trice16_2 ( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  Trice32 ( "hi %03u %03d", 5, 7 ); ...`, `...  Trice32_2 ( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  TRICE32 ( "hi %03u %03d", 5, 7 ); ...`, `...  TRICE32_2 ( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  trice32 ( "hi %03u %03d", 5, 7 ); ...`, `...  trice32_2 ( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  Trice64 ( "hi %03u %03d", 5, 7 ); ...`, `...  Trice64_2 ( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  TRICE64 ( "hi %03u %03d", 5, 7 ); ...`, `...  TRICE64_2 ( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  trice64 ( "hi %03u %03d", 5, 7 ); ...`, `...  trice64_2 ( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...   Trice8i( "hi %03u %03d", 5, 7 ); ...`, `...   Trice8_2i( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...   TRICE8i( "hi %03u %03d", 5, 7 ); ...`, `...   TRICE8_2i( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...   trice8i( "hi %03u %03d", 5, 7 ); ...`, `...   trice8_2i( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  Trice16i( "hi %03u %03d", 5, 7 ); ...`, `...  Trice16_2i( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  TRICE16i( "hi %03u %03d", 5, 7 ); ...`, `...  TRICE16_2i( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  trice16i( "hi %03u %03d", 5, 7 ); ...`, `...  trice16_2i( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  Trice32i( "hi %03u %03d", 5, 7 ); ...`, `...  Trice32_2i( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  TRICE32i( "hi %03u %03d", 5, 7 ); ...`, `...  TRICE32_2i( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  trice32i( "hi %03u %03d", 5, 7 ); ...`, `...  trice32_2i( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  Trice64i( "hi %03u %03d", 5, 7 ); ...`, `...  Trice64_2i( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  TRICE64i( "hi %03u %03d", 5, 7 ); ...`, `...  TRICE64_2i( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  trice64i( "hi %03u %03d", 5, 7 ); ...`, `...  trice64_2i( Id(0), "hi %03u %03d", 5, 7 ); ...`},
	}
	checkTestTable(t, tt, true)
}

func TestInsertParamCountAndIDTwoParam(t *testing.T) {
	tt := []struct{ text, exp string }{
		{`...   Trice8 ( "hi %03u %03d", 5, 7 ); ...`, `...   Trice8 ( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...   TRICE8 ( "hi %03u %03d", 5, 7 ); ...`, `...   TRICE8 ( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...   trice8 ( "hi %03u %03d", 5, 7 ); ...`, `...   trice8 ( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  Trice16 ( "hi %03u %03d", 5, 7 ); ...`, `...  Trice16 ( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  TRICE16 ( "hi %03u %03d", 5, 7 ); ...`, `...  TRICE16 ( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  trice16 ( "hi %03u %03d", 5, 7 ); ...`, `...  trice16 ( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  Trice32 ( "hi %03u %03d", 5, 7 ); ...`, `...  Trice32 ( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  TRICE32 ( "hi %03u %03d", 5, 7 ); ...`, `...  TRICE32 ( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  trice32 ( "hi %03u %03d", 5, 7 ); ...`, `...  trice32 ( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  Trice64 ( "hi %03u %03d", 5, 7 ); ...`, `...  Trice64 ( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  TRICE64 ( "hi %03u %03d", 5, 7 ); ...`, `...  TRICE64 ( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  trice64 ( "hi %03u %03d", 5, 7 ); ...`, `...  trice64 ( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...   Trice8i( "hi %03u %03d", 5, 7 ); ...`, `...   Trice8i( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...   TRICE8i( "hi %03u %03d", 5, 7 ); ...`, `...   TRICE8i( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...   trice8i( "hi %03u %03d", 5, 7 ); ...`, `...   trice8i( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  Trice16i( "hi %03u %03d", 5, 7 ); ...`, `...  Trice16i( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  TRICE16i( "hi %03u %03d", 5, 7 ); ...`, `...  TRICE16i( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  trice16i( "hi %03u %03d", 5, 7 ); ...`, `...  trice16i( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  Trice32i( "hi %03u %03d", 5, 7 ); ...`, `...  Trice32i( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  TRICE32i( "hi %03u %03d", 5, 7 ); ...`, `...  TRICE32i( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  trice32i( "hi %03u %03d", 5, 7 ); ...`, `...  trice32i( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  Trice64i( "hi %03u %03d", 5, 7 ); ...`, `...  Trice64i( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  TRICE64i( "hi %03u %03d", 5, 7 ); ...`, `...  TRICE64i( Id(0), "hi %03u %03d", 5, 7 ); ...`},
		{`...  trice64i( "hi %03u %03d", 5, 7 ); ...`, `...  trice64i( Id(0), "hi %03u %03d", 5, 7 ); ...`},
	}
	checkTestTable(t, tt, false)
}

func TestInsertParamCountAndIDThreeParamN(t *testing.T) {
	tt := []struct{ text, exp string }{
		{`...   Trice8 ( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...   Trice8_3 ( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...   TRICE8 ( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...   TRICE8_3 ( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...   trice8 ( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...   trice8_3 ( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...  Trice16 ( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  Trice16_3 ( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...  TRICE16 ( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  TRICE16_3 ( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...  trice16 ( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  trice16_3 ( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...  Trice32 ( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  Trice32_3 ( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...  TRICE32 ( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  TRICE32_3 ( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...  trice32 ( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  trice32_3 ( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...  Trice64 ( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  Trice64_3 ( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`}, // does not exist but allowed to match
		{`...  TRICE64 ( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  TRICE64_3 ( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`}, // does not exist but allowed to match
		{`...  trice64 ( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  trice64_3 ( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`}, // does not exist but allowed to match
		{`...   Trice8i( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...   Trice8_3i( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...   TRICE8i( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...   TRICE8_3i( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...   trice8i( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...   trice8_3i( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...  Trice16i( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  Trice16_3i( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...  TRICE16i( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  TRICE16_3i( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...  trice16i( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  trice16_3i( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...  Trice32i( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  Trice32_3i( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...  TRICE32i( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  TRICE32_3i( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...  trice32i( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  trice32_3i( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...  Trice64i( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  Trice64_3i( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`}, // does not exist but allowed to match
		{`...  TRICE64i( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  TRICE64_3i( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`}, // does not exist but allowed to match
		{`...  trice64i( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  trice64_3i( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`}, // does not exist but allowed to match
	}
	checkTestTable(t, tt, true)
}

func TestInsertParamCountAndIDThreeParam(t *testing.T) {
	tt := []struct{ text, exp string }{
		{`...   Trice8 ( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...   Trice8 ( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...   TRICE8 ( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...   TRICE8 ( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...   trice8 ( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...   trice8 ( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...  Trice16 ( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  Trice16 ( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...  TRICE16 ( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  TRICE16 ( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...  trice16 ( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  trice16 ( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...  Trice32 ( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  Trice32 ( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...  TRICE32 ( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  TRICE32 ( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...  trice32 ( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  trice32 ( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...  Trice64 ( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  Trice64 ( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`}, // does not exist but allowed to match
		{`...  TRICE64 ( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  TRICE64 ( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`}, // does not exist but allowed to match
		{`...  trice64 ( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  trice64 ( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`}, // does not exist but allowed to match
		{`...   Trice8i( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...   Trice8i( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...   TRICE8i( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...   TRICE8i( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...   trice8i( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...   trice8i( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...  Trice16i( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  Trice16i( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...  TRICE16i( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  TRICE16i( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...  trice16i( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  trice16i( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...  Trice32i( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  Trice32i( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...  TRICE32i( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  TRICE32i( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...  trice32i( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  trice32i( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`},
		{`...  Trice64i( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  Trice64i( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`}, // does not exist but allowed to match
		{`...  TRICE64i( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  TRICE64i( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`}, // does not exist but allowed to match
		{`...  trice64i( "hi %03u %03d %16b", 5, 7, 9 ); ...`, `...  trice64i( Id(0), "hi %03u %03d %16b", 5, 7, 9 ); ...`}, // does not exist but allowed to match
	}
	checkTestTable(t, tt, false)
}

func TestInsertParamCountAndIDFourParamN(t *testing.T) {
	tt := []struct{ text, exp string }{
		{`...   Trice8 ( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...   Trice8_4 ( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...   TRICE8 ( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...   TRICE8_4 ( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...   trice8 ( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...   trice8_4 ( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...  Trice16 ( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  Trice16_4 ( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...  TRICE16 ( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  TRICE16_4 ( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...  trice16 ( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  trice16_4 ( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...  Trice32 ( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  Trice32_4 ( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...  TRICE32 ( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  TRICE32_4 ( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...  trice32 ( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  trice32_4 ( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...  Trice64 ( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  Trice64_4 ( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`}, // does not exist but allowed to match
		{`...  TRICE64 ( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  TRICE64_4 ( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`}, // does not exist but allowed to match
		{`...  trice64 ( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  trice64_4 ( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`}, // does not exist but allowed to match
		{`...   Trice8i( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...   Trice8_4i( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...   TRICE8i( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...   TRICE8_4i( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...   trice8i( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...   trice8_4i( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...  Trice16i( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  Trice16_4i( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...  TRICE16i( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  TRICE16_4i( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...  trice16i( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  trice16_4i( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...  Trice32i( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  Trice32_4i( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...  TRICE32i( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  TRICE32_4i( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...  trice32i( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  trice32_4i( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...  Trice64i( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  Trice64_4i( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`}, // does not exist but allowed to match
		{`...  TRICE64i( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  TRICE64_4i( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`}, // does not exist but allowed to match
		{`...  trice64i( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  trice64_4i( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`}, // does not exist but allowed to match
	}
	checkTestTable(t, tt, true)
}

func TestInsertParamCountAndIDFourParam(t *testing.T) {
	tt := []struct{ text, exp string }{
		{`...   Trice8 ( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...   Trice8 ( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...   TRICE8 ( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...   TRICE8 ( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...   trice8 ( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...   trice8 ( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...  Trice16 ( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  Trice16 ( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...  TRICE16 ( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  TRICE16 ( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...  trice16 ( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  trice16 ( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...  Trice32 ( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  Trice32 ( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...  TRICE32 ( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  TRICE32 ( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...  trice32 ( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  trice32 ( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...  Trice64 ( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  Trice64 ( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`}, // does not exist but allowed to match
		{`...  TRICE64 ( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  TRICE64 ( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`}, // does not exist but allowed to match
		{`...  trice64 ( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  trice64 ( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`}, // does not exist but allowed to match
		{`...   Trice8i( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...   Trice8i( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...   TRICE8i( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...   TRICE8i( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...   trice8i( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...   trice8i( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...  Trice16i( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  Trice16i( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...  TRICE16i( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  TRICE16i( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...  trice16i( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  trice16i( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...  Trice32i( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  Trice32i( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...  TRICE32i( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  TRICE32i( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...  trice32i( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  trice32i( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`},
		{`...  Trice64i( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  Trice64i( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`}, // does not exist but allowed to match
		{`...  TRICE64i( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  TRICE64i( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`}, // does not exist but allowed to match
		{`...  trice64i( "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`, `...  trice64i( Id(0), "hi %03u %03d %16b 0x%08x", 5, 7, 9, 3 ); ...`}, // does not exist but allowed to match
	}
	checkTestTable(t, tt, false)
}

func TestInsertParamCountAndIDFiveParamN(t *testing.T) {
	tt := []struct{ text, exp string }{
		{`...   Trice8 ( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...   Trice8_5 ( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`},
		{`...   TRICE8 ( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...   TRICE8_5 ( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`},
		{`...   trice8 ( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...   trice8_5 ( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`},
		{`...  Trice16 ( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  Trice16_5 ( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
		{`...  TRICE16 ( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  TRICE16_5 ( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
		{`...  trice16 ( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  trice16_5 ( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
		{`...  Trice32 ( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  Trice32_5 ( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
		{`...  TRICE32 ( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  TRICE32_5 ( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
		{`...  trice32 ( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  trice32_5 ( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
		{`...  Trice64 ( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  Trice64_5 ( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
		{`...  TRICE64 ( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  TRICE64_5 ( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
		{`...  trice64 ( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  trice64_5 ( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
		{`...   Trice8i( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...   Trice8_5i( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`},
		{`...   TRICE8i( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...   TRICE8_5i( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`},
		{`...   trice8i( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...   trice8_5i( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`},
		{`...  Trice16i( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  Trice16_5i( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
		{`...  TRICE16i( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  TRICE16_5i( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
		{`...  trice16i( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  trice16_5i( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
		{`...  Trice32i( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  Trice32_5i( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
		{`...  TRICE32i( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  TRICE32_5i( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
		{`...  trice32i( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  trice32_5i( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
		{`...  Trice64i( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  Trice64_5i( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
		{`...  TRICE64i( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  TRICE64_5i( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
		{`...  trice64i( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  trice64_5i( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
	}
	checkTestTable(t, tt, true)
}

func TestInsertParamCountAndIDFiveParam(t *testing.T) {
	tt := []struct{ text, exp string }{
		{`...   Trice8 ( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...   Trice8 ( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`},
		{`...   TRICE8 ( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...   TRICE8 ( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`},
		{`...   trice8 ( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...   trice8 ( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`},
		{`...  Trice16 ( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  Trice16 ( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
		{`...  TRICE16 ( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  TRICE16 ( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
		{`...  trice16 ( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  trice16 ( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
		{`...  Trice32 ( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  Trice32 ( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
		{`...  TRICE32 ( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  TRICE32 ( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
		{`...  trice32 ( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  trice32 ( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
		{`...  Trice64 ( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  Trice64 ( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
		{`...  TRICE64 ( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  TRICE64 ( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
		{`...  trice64 ( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  trice64 ( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
		{`...   Trice8i( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...   Trice8i( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`},
		{`...   TRICE8i( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...   TRICE8i( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`},
		{`...   trice8i( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...   trice8i( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`},
		{`...  Trice16i( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  Trice16i( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
		{`...  TRICE16i( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  TRICE16i( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
		{`...  trice16i( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  trice16i( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
		{`...  Trice32i( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  Trice32i( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
		{`...  TRICE32i( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  TRICE32i( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
		{`...  trice32i( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  trice32i( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
		{`...  Trice64i( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  Trice64i( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
		{`...  TRICE64i( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  TRICE64i( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
		{`...  trice64i( "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`, `...  trice64i( Id(0), "hi %03u %03d %16b 0x%08x %X", 5, 7, 9, 3, 2 ); ...`}, // does not exist but allowed to match
	}
	checkTestTable(t, tt, false)
}

func TestInsertParamCountAndIDSixParamN(t *testing.T) {
	tt := []struct{ text, exp string }{
		{`...   Trice8 ( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...   Trice8_6 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`},
		{`...   TRICE8 ( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...   TRICE8_6 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`},
		{`...   trice8 ( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...   trice8_6 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`},
		{`...  Trice16 ( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  Trice16_6 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
		{`...  TRICE16 ( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  TRICE16_6 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
		{`...  trice16 ( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  trice16_6 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
		{`...  Trice32 ( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  Trice32_6 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
		{`...  TRICE32 ( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  TRICE32_6 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
		{`...  trice32 ( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  trice32_6 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
		{`...  Trice64 ( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  Trice64_6 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
		{`...  TRICE64 ( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  TRICE64_6 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
		{`...  trice64 ( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  trice64_6 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
		{`...   Trice8i( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...   Trice8_6i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`},
		{`...   TRICE8i( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...   TRICE8_6i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`},
		{`...   trice8i( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...   trice8_6i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`},
		{`...  Trice16i( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  Trice16_6i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
		{`...  TRICE16i( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  TRICE16_6i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
		{`...  trice16i( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  trice16_6i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
		{`...  Trice32i( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  Trice32_6i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
		{`...  TRICE32i( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  TRICE32_6i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
		{`...  trice32i( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  trice32_6i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
		{`...  Trice64i( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  Trice64_6i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
		{`...  TRICE64i( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  TRICE64_6i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
		{`...  trice64i( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  trice64_6i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
	}
	checkTestTable(t, tt, true)
}

func TestInsertParamCountAndIDSixParam(t *testing.T) {
	tt := []struct{ text, exp string }{
		{`...   Trice8 ( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...   Trice8 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`},
		{`...   TRICE8 ( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...   TRICE8 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`},
		{`...   trice8 ( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...   trice8 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`},
		{`...  Trice16 ( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  Trice16 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
		{`...  TRICE16 ( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  TRICE16 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
		{`...  trice16 ( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  trice16 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
		{`...  Trice32 ( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  Trice32 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
		{`...  TRICE32 ( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  TRICE32 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
		{`...  trice32 ( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  trice32 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
		{`...  Trice64 ( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  Trice64 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
		{`...  TRICE64 ( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  TRICE64 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
		{`...  trice64 ( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  trice64 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
		{`...   Trice8i( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...   Trice8i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`},
		{`...   TRICE8i( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...   TRICE8i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`},
		{`...   trice8i( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...   trice8i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`},
		{`...  Trice16i( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  Trice16i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
		{`...  TRICE16i( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  TRICE16i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
		{`...  trice16i( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  trice16i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
		{`...  Trice32i( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  Trice32i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
		{`...  TRICE32i( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  TRICE32i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
		{`...  trice32i( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  trice32i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
		{`...  Trice64i( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  Trice64i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
		{`...  TRICE64i( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  TRICE64i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
		{`...  trice64i( "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`, `...  trice64i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d", 5, 7, 9, 3, 2, 4 ); ...`}, // does not exist but allowed to match
	}
	checkTestTable(t, tt, false)
}

func TestInsertParamCountAndIDSevenParamN(t *testing.T) {
	tt := []struct{ text, exp string }{
		{`...   Trice8 ( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...   Trice8_7 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`},
		{`...   TRICE8 ( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...   TRICE8_7 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`},
		{`...   trice8 ( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...   trice8_7 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`},
		{`...  Trice16 ( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  Trice16_7 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
		{`...  TRICE16 ( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  TRICE16_7 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
		{`...  trice16 ( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  trice16_7 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
		{`...  Trice32 ( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  Trice32_7 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
		{`...  TRICE32 ( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  TRICE32_7 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
		{`...  trice32 ( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  trice32_7 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
		{`...  Trice64 ( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  Trice64_7 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
		{`...  TRICE64 ( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  TRICE64_7 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
		{`...  trice64 ( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  trice64_7 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
		{`...   Trice8i( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...   Trice8_7i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`},
		{`...   TRICE8i( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...   TRICE8_7i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`},
		{`...   trice8i( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...   trice8_7i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`},
		{`...  Trice16i( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  Trice16_7i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
		{`...  TRICE16i( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  TRICE16_7i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
		{`...  trice16i( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  trice16_7i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
		{`...  Trice32i( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  Trice32_7i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
		{`...  TRICE32i( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  TRICE32_7i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
		{`...  trice32i( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  trice32_7i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
		{`...  Trice64i( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  Trice64_7i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
		{`...  TRICE64i( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  TRICE64_7i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
		{`...  trice64i( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  trice64_7i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
	}
	checkTestTable(t, tt, true)
}

func TestInsertParamCountAndIDSevenParam(t *testing.T) {
	tt := []struct{ text, exp string }{
		{`...   Trice8 ( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...   Trice8 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`},
		{`...   TRICE8 ( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...   TRICE8 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`},
		{`...   trice8 ( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...   trice8 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`},
		{`...  Trice16 ( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  Trice16 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
		{`...  TRICE16 ( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  TRICE16 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
		{`...  trice16 ( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  trice16 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
		{`...  Trice32 ( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  Trice32 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
		{`...  TRICE32 ( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  TRICE32 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
		{`...  trice32 ( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  trice32 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
		{`...  Trice64 ( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  Trice64 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
		{`...  TRICE64 ( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  TRICE64 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
		{`...  trice64 ( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  trice64 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
		{`...   Trice8i( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...   Trice8i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`},
		{`...   TRICE8i( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...   TRICE8i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`},
		{`...   trice8i( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...   trice8i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`},
		{`...  Trice16i( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  Trice16i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
		{`...  TRICE16i( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  TRICE16i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
		{`...  trice16i( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  trice16i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
		{`...  Trice32i( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  Trice32i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
		{`...  TRICE32i( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  TRICE32i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
		{`...  trice32i( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  trice32i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
		{`...  Trice64i( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  Trice64i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
		{`...  TRICE64i( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  TRICE64i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
		{`...  trice64i( "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`, `...  trice64i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u", 5, 7, 9, 3, 2, 4, 6 ); ...`}, // does not exist but allowed to match
	}
	checkTestTable(t, tt, false)
}

func TestInsertParamCountAndIDEightParamN(t *testing.T) {
	tt := []struct{ text, exp string }{
		{`...   Trice8 ( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...   Trice8_8 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`},
		{`...   TRICE8 ( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...   TRICE8_8 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`},
		{`...   trice8 ( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...   trice8_8 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`},
		{`...  Trice16 ( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  Trice16_8 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
		{`...  TRICE16 ( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  TRICE16_8 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
		{`...  trice16 ( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  trice16_8 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
		{`...  Trice32 ( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  Trice32_8 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
		{`...  TRICE32 ( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  TRICE32_8 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
		{`...  trice32 ( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  trice32_8 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
		{`...  Trice64 ( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  Trice64_8 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
		{`...  TRICE64 ( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  TRICE64_8 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
		{`...  trice64 ( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  trice64_8 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
		{`...   Trice8i( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...   Trice8_8i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`},
		{`...   TRICE8i( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...   TRICE8_8i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`},
		{`...   trice8i( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...   trice8_8i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`},
		{`...  Trice16i( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  Trice16_8i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
		{`...  TRICE16i( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  TRICE16_8i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
		{`...  trice16i( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  trice16_8i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
		{`...  Trice32i( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  Trice32_8i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
		{`...  TRICE32i( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  TRICE32_8i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
		{`...  trice32i( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  trice32_8i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
		{`...  Trice64i( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  Trice64_8i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
		{`...  TRICE64i( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  TRICE64_8i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
		{`...  trice64i( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  trice64_8i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
	}
	checkTestTable(t, tt, true)
}

func TestInsertParamCountAndIDEightParam(t *testing.T) {
	tt := []struct{ text, exp string }{
		{`...   Trice8 ( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...   Trice8 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`},
		{`...   TRICE8 ( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...   TRICE8 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`},
		{`...   trice8 ( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...   trice8 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`},
		{`...  Trice16 ( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  Trice16 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
		{`...  TRICE16 ( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  TRICE16 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
		{`...  trice16 ( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  trice16 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
		{`...  Trice32 ( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  Trice32 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
		{`...  TRICE32 ( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  TRICE32 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
		{`...  trice32 ( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  trice32 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
		{`...  Trice64 ( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  Trice64 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
		{`...  TRICE64 ( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  TRICE64 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
		{`...  trice64 ( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  trice64 ( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
		{`...   Trice8i( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...   Trice8i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`},
		{`...   TRICE8i( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...   TRICE8i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`},
		{`...   trice8i( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...   trice8i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`},
		{`...  Trice16i( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  Trice16i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
		{`...  TRICE16i( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  TRICE16i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
		{`...  trice16i( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  trice16i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
		{`...  Trice32i( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  Trice32i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
		{`...  TRICE32i( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  TRICE32i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
		{`...  trice32i( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  trice32i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
		{`...  Trice64i( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  Trice64i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
		{`...  TRICE64i( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  TRICE64i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
		{`...  trice64i( "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`, `...  trice64i( Id(0), "hi %03u %03d %16b 0x%08x %X %17d %99u %04b", 5, 7, 9, 3, 2, 4, 6, 8 ); ...`}, // does not exist but allowed to match
	}
	checkTestTable(t, tt, false)
}

func TestInsertParamCountAndIDAll0(t *testing.T) {
	tt := []struct{ text, exp string }{
		{
			`... TRICE0 ( "hi"); ...`,
			`... TRICE0 ( Id(0), "hi"); ...`},
		{
			`... TRICE0( "hi"); ...`,
			`... TRICE0( Id(0), "hi"); ...`},
		{
			`... TRICE8( "hi %d", 5); ...`,
			`... TRICE8_1( Id(0), "hi %d", 5); ...`},
		{
			`... TRICE8( "hi %d, %u", 5, h); ...`,
			`... TRICE8_2( Id(0), "hi %d, %u", 5, h); ...`},
		{
			`... TRICE8( "hi %d, %u, %b", d, u, b); ...`,
			`... TRICE8_3( Id(0), "hi %d, %u, %b", d, u, b); ...`},
		{
			`... TRICE8( "hi %d, %u, %b, %x", d, u, b, h); ...`,
			`... TRICE8_4( Id(0), "hi %d, %u, %b, %x", d, u, b, h); ...`},
		{
			`... TRICE8( "hi %d, %u, %b, %x %d, %u, %b, %x", d, u, b, h, d, u, b, h); ...`,
			`... TRICE8_8( Id(0), "hi %d, %u, %b, %x %d, %u, %b, %x", d, u, b, h, d, u, b, h); ...`},
		{
			`... TRICE8( "hi %2d, %13u, %64b, %8x %02d, %013u, %032b, %016x", d, u, b, h, d, u, b, h); ...`,
			`... TRICE8_8( Id(0), "hi %2d, %13u, %64b, %8x %02d, %013u, %032b, %016x", d, u, b, h, d, u, b, h); ...`},
		{
			`... TRICE16( "hi %d", 5); ...`,
			`... TRICE16_1( Id(0), "hi %d", 5); ...`},
		{
			`... TRICE16( "hi %d, %u", 5, h); ...`,
			`... TRICE16_2( Id(0), "hi %d, %u", 5, h); ...`},
		{
			`... TRICE16( "hi %d, %u, %b", d, u, b); ...`,
			`... TRICE16_3( Id(0), "hi %d, %u, %b", d, u, b); ...`},
		{
			`... TRICE16( "hi %d, %u, %b, %x", d, u, b, h); ...`,
			`... TRICE16_4( Id(0), "hi %d, %u, %b, %x", d, u, b, h); ...`},
		{
			`... TRICE32( "hi %d", 5); ...`,
			`... TRICE32_1( Id(0), "hi %d", 5); ...`},
		{
			`... TRICE32( "hi %d, %u", 5, h); ...`,
			`... TRICE32_2( Id(0), "hi %d, %u", 5, h); ...`},
		{
			`... TRICE32( "hi %d, %u, %b", d, u, b); ...`,
			`... TRICE32_3( Id(0), "hi %d, %u, %b", d, u, b); ...`},
		{
			`... TRICE32( "hi %d, %u, %b, %x", d, u, b, h); ...`,
			`... TRICE32_4( Id(0), "hi %d, %u, %b, %x", d, u, b, h); ...`},
		{
			`... TRICE64( "hi %d", 5); ...`,
			`... TRICE64_1( Id(0), "hi %d", 5); ...`},
		{
			`... TRICE64( "hi %d, %u", 5, h); ...`,
			`... TRICE64_2( Id(0), "hi %d, %u", 5, h); ...`},
		{
			`... trice0 ( "hi"); ...`,
			`... trice0 ( Id(0), "hi"); ...`},
		{
			`... trice0( "hi"); ...`,
			`... trice0( Id(0), "hi"); ...`},
		{
			`... trice8( "hi %d", 5); ...`,
			`... trice8_1( Id(0), "hi %d", 5); ...`},
		{
			`... trice8( "hi %d, %u", 5, h); ...`,
			`... trice8_2( Id(0), "hi %d, %u", 5, h); ...`},
		{
			`... trice8( "hi %d, %u, %b", d, u, b); ...`,
			`... trice8_3( Id(0), "hi %d, %u, %b", d, u, b); ...`},
		{
			`... trice8( "hi %d, %u, %b, %x", d, u, b, h); ...`,
			`... trice8_4( Id(0), "hi %d, %u, %b, %x", d, u, b, h); ...`},
		{
			`... trice8( "hi %d, %u, %b, %x %d, %u, %b, %x", d, u, b, h, d, u, b, h); ...`,
			`... trice8_8( Id(0), "hi %d, %u, %b, %x %d, %u, %b, %x", d, u, b, h, d, u, b, h); ...`},
		{
			`... trice8( "hi %2d, %13u, %64b, %8x %02d, %013u, %032b, %016x", d, u, b, h, d, u, b, h); ...`,
			`... trice8_8( Id(0), "hi %2d, %13u, %64b, %8x %02d, %013u, %032b, %016x", d, u, b, h, d, u, b, h); ...`},
		{
			`... trice16( "hi %d", 5); ...`,
			`... trice16_1( Id(0), "hi %d", 5); ...`},
		{
			`... trice16( "hi %d, %u", 5, h); ...`,
			`... trice16_2( Id(0), "hi %d, %u", 5, h); ...`},
		{
			`... trice16( "hi %d, %u, %b", d, u, b); ...`,
			`... trice16_3( Id(0), "hi %d, %u, %b", d, u, b); ...`},
		{
			`... trice16( "hi %d, %u, %b, %x", d, u, b, h); ...`,
			`... trice16_4( Id(0), "hi %d, %u, %b, %x", d, u, b, h); ...`},
		{
			`... trice32( "hi %d", 5); ...`,
			`... trice32_1( Id(0), "hi %d", 5); ...`},
		{
			`... trice32( "hi %d, %u", 5, h); ...`,
			`... trice32_2( Id(0), "hi %d, %u", 5, h); ...`},
		{
			`... trice32( "hi %d, %u, %b", d, u, b); ...`,
			`... trice32_3( Id(0), "hi %d, %u, %b", d, u, b); ...`},
		{
			`... trice32( "hi %d, %u, %b, %x", d, u, b, h); ...`,
			`... trice32_4( Id(0), "hi %d, %u, %b, %x", d, u, b, h); ...`},
		{
			`... trice64( "hi %d", 5); ...`,
			`... trice64_1( Id(0), "hi %d", 5); ...`},
		{
			`... trice64( "hi %d, %u", 5, h); ...`,
			`... trice64_2( Id(0), "hi %d, %u", 5, h); ...`},
		{
			`... TRICE0i ( "hi"); ...`,
			`... TRICE0i ( Id(0), "hi"); ...`},
		{
			`... TRICE0i( "hi"); ...`,
			`... TRICE0i( Id(0), "hi"); ...`},
		{
			`... TRICE8i( "hi %d", 5); ...`,
			`... TRICE8_1i( Id(0), "hi %d", 5); ...`},
		{
			`... TRICE8i( "hi %d, %u", 5, h); ...`,
			`... TRICE8_2i( Id(0), "hi %d, %u", 5, h); ...`},
		{
			`... TRICE8i( "hi %d, %u, %b", d, u, b); ...`,
			`... TRICE8_3i( Id(0), "hi %d, %u, %b", d, u, b); ...`},
		{
			`... TRICE8i( "hi %d, %u, %b, %x", d, u, b, h); ...`,
			`... TRICE8_4i( Id(0), "hi %d, %u, %b, %x", d, u, b, h); ...`},
		{
			`... TRICE8i( "hi %d, %u, %b, %x %d, %u, %b, %x", d, u, b, h, d, u, b, h); ...`,
			`... TRICE8_8i( Id(0), "hi %d, %u, %b, %x %d, %u, %b, %x", d, u, b, h, d, u, b, h); ...`},
		{
			`... TRICE8i( "hi %2d, %13u, %64b, %8x %02d, %013u, %032b, %016x", d, u, b, h, d, u, b, h); ...`,
			`... TRICE8_8i( Id(0), "hi %2d, %13u, %64b, %8x %02d, %013u, %032b, %016x", d, u, b, h, d, u, b, h); ...`},
		{
			`... TRICE16i( "hi %d", 5); ...`,
			`... TRICE16_1i( Id(0), "hi %d", 5); ...`},
		{
			`... TRICE16i( "hi %d, %u", 5, h); ...`,
			`... TRICE16_2i( Id(0), "hi %d, %u", 5, h); ...`},
		{
			`... TRICE16i( "hi %d, %u, %b", d, u, b); ...`,
			`... TRICE16_3i( Id(0), "hi %d, %u, %b", d, u, b); ...`},
		{
			`... TRICE16i( "hi %d, %u, %b, %x", d, u, b, h); ...`,
			`... TRICE16_4i( Id(0), "hi %d, %u, %b, %x", d, u, b, h); ...`},
		{
			`... TRICE32i( "hi %d", 5); ...`,
			`... TRICE32_1i( Id(0), "hi %d", 5); ...`},
		{
			`... TRICE32i( "hi %d, %u", 5, h); ...`,
			`... TRICE32_2i( Id(0), "hi %d, %u", 5, h); ...`},
		{
			`... TRICE32i( "hi %d, %u, %b", d, u, b); ...`,
			`... TRICE32_3i( Id(0), "hi %d, %u, %b", d, u, b); ...`},
		{
			`... TRICE32i( "hi %d, %u, %b, %x", d, u, b, h); ...`,
			`... TRICE32_4i( Id(0), "hi %d, %u, %b, %x", d, u, b, h); ...`},
		{
			`... TRICE64i( "hi %d", 5); ...`,
			`... TRICE64_1i( Id(0), "hi %d", 5); ...`},
		{
			`... TRICE64i( "hi %d, %u", 5, h); ...`,
			`... TRICE64_2i( Id(0), "hi %d, %u", 5, h); ...`},
		{
			`... trice0i ( "hi"); ...`,
			`... trice0i ( Id(0), "hi"); ...`},
		{
			`... trice0i( "hi"); ...`,
			`... trice0i( Id(0), "hi"); ...`},
		{
			`... trice8i( "hi %d", 5); ...`,
			`... trice8_1i( Id(0), "hi %d", 5); ...`},
		{
			`... trice8i( "hi %d, %u", 5, h); ...`,
			`... trice8_2i( Id(0), "hi %d, %u", 5, h); ...`},
		{
			`... trice8i( "hi %d, %u, %b", d, u, b); ...`,
			`... trice8_3i( Id(0), "hi %d, %u, %b", d, u, b); ...`},
		{
			`... trice8i( "hi %d, %u, %b, %x", d, u, b, h); ...`,
			`... trice8_4i( Id(0), "hi %d, %u, %b, %x", d, u, b, h); ...`},
		{
			`... trice8i( "hi %d, %u, %b, %x %d, %u, %b, %x", d, u, b, h, d, u, b, h); ...`,
			`... trice8_8i( Id(0), "hi %d, %u, %b, %x %d, %u, %b, %x", d, u, b, h, d, u, b, h); ...`},
		{
			`... trice8i( "hi %2d, %13u, %64b, %8x %02d, %013u, %032b, %016x", d, u, b, h, d, u, b, h); ...`,
			`... trice8_8i( Id(0), "hi %2d, %13u, %64b, %8x %02d, %013u, %032b, %016x", d, u, b, h, d, u, b, h); ...`},
		{
			`... trice16i( "hi %d", 5); ...`,
			`... trice16_1i( Id(0), "hi %d", 5); ...`},
		{
			`... trice16i( "hi %d, %u", 5, h); ...`,
			`... trice16_2i( Id(0), "hi %d, %u", 5, h); ...`},
		{
			`... trice16i( "hi %d, %u, %b", d, u, b); ...`,
			`... trice16_3i( Id(0), "hi %d, %u, %b", d, u, b); ...`},
		{
			`... trice16i( "hi %d, %u, %b, %x", d, u, b, h); ...`,
			`... trice16_4i( Id(0), "hi %d, %u, %b, %x", d, u, b, h); ...`},
		{
			`... trice32i( "hi %d", 5); ...`,
			`... trice32_1i( Id(0), "hi %d", 5); ...`},
		{
			`... trice32i( "hi %d, %u", 5, h); ...`,
			`... trice32_2i( Id(0), "hi %d, %u", 5, h); ...`},
		{
			`... trice32i( "hi %d, %u, %b", d, u, b); ...`,
			`... trice32_3i( Id(0), "hi %d, %u, %b", d, u, b); ...`},
		{
			`... trice32i( "hi %d, %u, %b, %x", d, u, b, h); ...`,
			`... trice32_4i( Id(0), "hi %d, %u, %b, %x", d, u, b, h); ...`},
		{
			`... trice64i( "hi %d", 5); ...`,
			`... trice64_1i( Id(0), "hi %d", 5); ...`},
		{
			`... trice64i( "hi %d, %u", 5, h); ...`,
			`... trice64_2i( Id(0), "hi %d, %u", 5, h); ...`},
	}
	checkTestTable(t, tt, true)
}

func TestOptionallyExtendLenAndInsertID0(t *testing.T) {
	tt := []struct{ text, exp string }{
		{
			`_TRICE8( "hi %d", 5); // to not touch`,
			`_TRICE8( "hi %d", 5); // to not touch`},
		{
			`TRICE8( Id(0), "hi %2d, %13u, %64b, %8x %02d, %013u, %032b, %016x",1,2,3,4,5,6,7,8 );`,
			`TRICE8_8( Id(0), "hi %2d, %13u, %64b, %8x %02d, %013u, %032b, %016x",1,2,3,4,5,6,7,8 );`},
		{
			`TRICE8( "hi %2d, %13u, %64b, %8x %02d, %013u, %032b, %016x",1,2,3,4,5,6,7,8 );`,
			`TRICE8_8( Id(0), "hi %2d, %13u, %64b, %8x %02d, %013u, %032b, %016x",1,2,3,4,5,6,7,8 );`},
		{
			`TRICE8_3( "hi %2d, %13u, %64b, %8x %02d, %013u, %032b, %016x",1,2,3,4,5,6,7,8 ); // do not change wrong count`,
			`TRICE8_3( Id(0), "hi %2d, %13u, %64b, %8x %02d, %013u, %032b, %016x",1,2,3,4,5,6,7,8 ); // do not change wrong count`},
		{
			`trice_s  ( "%s\n", rts ) \\ no semicolon`,
			`trice_s  ( Id(0), "%s\n", rts ) \\ no semicolon`},
		{
			`trice_s  ( "%s\n", "rts" );`,
			`trice_s  ( Id(0), "%s\n", "rts" );`},
	}
	checkTestTable(t, tt, true)
}

func TestVariadicInsertId0(t *testing.T) {
	tt := []struct{ text, exp string }{
		{
			`Trice8( "hi %2d",1  );`,
			`Trice8( Id(0), "hi %2d",1  );`},
		{
			`TRICE8( "hi %2d, %13u, %64b, %8x %02d, %013u, %032b, %016x",1,2,3,4,5,6,7,8 );`,
			`TRICE8( Id(0), "hi %2d, %13u, %64b, %8x %02d, %013u, %032b, %016x",1,2,3,4,5,6,7,8 );`},
		{
			`trice16i( "hi %2d, %13u, %64b, %8x %02d, %013u, %032b, %016x",1,2,3,4,5,6,7,8 ); // does not exist but is allowed`,
			`trice16i( Id(0), "hi %2d, %13u, %64b, %8x %02d, %013u, %032b, %016x",1,2,3,4,5,6,7,8 ); // does not exist but is allowed`},
	}
	checkTestTable(t, tt, false)
}

func checkTestTable(t *testing.T, tt []struct{ text, exp string }, extend bool) {
	for _, x := range tt {
		act, _ := updateParamCountAndID0(x.text, extend)
		assert.Equal(t, x.exp, act)
	}
}
