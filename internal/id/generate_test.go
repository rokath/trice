package id

import (
	"fmt"
	"testing"

	"github.com/tj/assert"
)

func Test_computeValues(t *testing.T) {
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	tt := []struct {
		defaultBitWidth    int
		trice              TriceFmt
		expectedExtType    string
		expectedBitWith    int
		expectedParamCount int
	}{
		{32, TriceFmt{"TRICE", "%d", ""}, "TRICE32_1", 32, 1},
		{32, TriceFmt{"TRICE", "%d%d%d%d%d%d%d", ""}, "TRICE32_7", 32, 7},
		{32, TriceFmt{"TRICE", "%d%d%d%d%d%d%d%d", ""}, "TRICE32_8", 32, 8},
		{32, TriceFmt{"TRICE", "%d%d%d%d%d%d%d%d%d", ""}, "TRICE32_9", 32, 9},
		{32, TriceFmt{"trice", "%d", ""}, "trice32_1", 32, 1},
		{32, TriceFmt{"trice", "%d%d%d%d%d%d%d", ""}, "trice32_7", 32, 7},
		{32, TriceFmt{"trice", "%d%d%d%d%d%d%d%d", ""}, "trice32_8", 32, 8},
		{32, TriceFmt{"trice", "%d%d%d%d%d%d%d%d%d", ""}, "trice32_9", 32, 9},

		{16, TriceFmt{"TRICE", "%d", ""}, "TRICE16_1", 16, 1},
		{16, TriceFmt{"TRICE", "%d%d%d%d%d%d%d", ""}, "TRICE16_7", 16, 7},
		{16, TriceFmt{"TRICE", "%d%d%d%d%d%d%d%d", ""}, "TRICE16_8", 16, 8},
		{16, TriceFmt{"TRICE", "%d%d%d%d%d%d%d%d%d", ""}, "TRICE16_9", 16, 9},
		{16, TriceFmt{"trice", "%d", ""}, "trice16_1", 16, 1},
		{16, TriceFmt{"trice", "%d%d%d%d%d%d%d", ""}, "trice16_7", 16, 7},
		{16, TriceFmt{"trice", "%d%d%d%d%d%d%d%d", ""}, "trice16_8", 16, 8},
		{16, TriceFmt{"trice", "%d%d%d%d%d%d%d%d%d", ""}, "trice16_9", 16, 9},

		{8, TriceFmt{"TRICE", "%d", ""}, "TRICE8_1", 8, 1},
		{8, TriceFmt{"TRICE", "%d%d%d%d%d%d%d", ""}, "TRICE8_7", 8, 7},
		{8, TriceFmt{"TRICE", "%d%d%d%d%d%d%d%d", ""}, "TRICE8_8", 8, 8},
		{8, TriceFmt{"TRICE", "%d%d%d%d%d%d%d%d%d", ""}, "TRICE8_9", 8, 9},
		{8, TriceFmt{"trice", "%d", ""}, "trice8_1", 8, 1},
		{8, TriceFmt{"trice", "%d%d%d%d%d%d%d", ""}, "trice8_7", 8, 7},
		{8, TriceFmt{"trice", "%d%d%d%d%d%d%d%d", ""}, "trice8_8", 8, 8},
		{8, TriceFmt{"trice", "%d%d%d%d%d%d%d%d%d", ""}, "trice8_9", 8, 9},
	}
	for _, v := range tt {
		extType, bitWidth, paramCount := computeValues(v.trice, v.defaultBitWidth)
		//  fmt.Println(v.defaultBitWidth, v.trice.Type, v.trice.Strg,
		//  	v.expectedExtType, extType,
		//  	v.expectedBitWith, bitWidth,
		//  	v.expectedParamCount, paramCount)
		assert.Equal(t, v.expectedExtType, extType)
		assert.Equal(t, v.expectedBitWith, bitWidth)
		assert.Equal(t, v.expectedParamCount, paramCount)
	}

}

func TestConstructFullTriceInfo(t *testing.T) {
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	tt := []struct {
		ty    string // ty is the Trice type name written by the user in the target source code.
		count int    // count is the counted number of format specifiers inside the format string.
		exp   string // exp is what ConstructFullTriceInfo is expected to generate. If empty, ConstructFullTriceInfo should return with an error.
	}{

		{"trice", 0, "trice_0"},
		{"trice0", 0, "trice_0"},
		{"trice8", 0, "trice_0"},
		{"trice8", 7, "trice8_7"},
		{"trice8", 8, "trice8_8"},
		{"trice16", 0, "trice_0"},
		{"trice32", 0, "trice_0"},
		{"trice64", 0, "trice_0"},

		{"trice_10", 10, "trice32_10"},
		{"trice10", 10, ""},
		{"trice8", 8, "trice8_8"},
		{"trice8", 7, "trice8_7"},
		{"trice_0", 0, "trice_0"},
		{"trice8_0", 0, "trice_0"},
		{"trice16_0", 0, "trice_0"},
		{"trice32_0", 0, "trice_0"},
		{"trice64_0", 0, "trice_0"},

		{"TRICE_S", 0, ""},
		{"TRICE_N", 0, ""},
		{"TRICE_B", 0, ""},
		{"TRICE_F", 0, "TRICE32F"},
		{"triceS", 0, ""},
		{"triceN", 0, ""},

		{"triceB", 0, ""},
		{"trice8B", 0, ""},
		{"trice16B", 0, ""},
		{"trice32B", 0, ""},
		{"trice64B", 0, ""},

		{"triceF", 0, "trice32F"},
		{"trice8F", 0, "trice8F"},
		{"trice16F", 0, "trice16F"},
		{"trice32F", 0, "trice32F"},
		{"trice64F", 0, "trice64F"},

		{"trice", 1, "trice32_1"},
		{"trice8", 1, "trice8_1"},
		{"trice16", 1, "trice16_1"},
		{"trice32", 1, "trice32_1"},
		{"trice64", 1, "trice64_1"},

		{"trice_1", 1, "trice32_1"},
		{"trice8_1", 1, "trice8_1"},
		{"trice16_1", 1, "trice16_1"},
		{"trice32_1", 1, "trice32_1"},
		{"trice64_1", 1, "trice64_1"},

		{"TRICE_S", 1, "TRICES"},
		{"TRICE_N", 1, "TRICEN"},
		{"TRICE_B", 1, "TRICE32B"},
		{"TRICE_F", 1, ""},
		{"triceS", 1, "triceS"},
		{"triceN", 1, "triceN"},

		{"triceB", 1, "trice32B"},
		{"trice8B", 1, "trice8B"},
		{"trice16B", 1, "trice16B"},
		{"trice32B", 1, "trice32B"},
		{"trice64B", 1, "trice64B"},

		{"triceF", 1, ""},
		{"trice8F", 1, ""},
		{"trice16F", 1, ""},
		{"trice32F", 1, ""},
		{"trice64F", 1, ""},

		{"trice", 2, "trice32_2"},
		{"trice8", 2, "trice8_2"},
		{"trice16", 2, "trice16_2"},
		{"trice32", 2, "trice32_2"},
		{"trice64", 2, "trice64_2"},

		{"trice_2", 2, "trice32_2"},
		{"trice8_2", 2, "trice8_2"},
		{"trice16_2", 2, "trice16_2"},
		{"trice32_2", 2, "trice32_2"},
		{"trice64_2", 2, "trice64_2"},

		{"TRICE_S", 2, ""},
		{"TRICE_N", 2, ""},
		{"TRICE_B", 2, ""},
		{"TRICE_F", 2, ""},
		{"triceS", 2, ""},
		{"TRiceN", 2, ""},

		{"triceB", 2, ""},
		{"TRice8B", 2, ""},
		{"trice16B", 2, ""},
		{"trice32B", 2, ""},
		{"trice64B", 2, ""},

		{"TRICE_B", 2, ""},
		{"TRICE8_B", 2, ""},
		{"TRICE16_B", 2, ""},
		{"TRICE32_B", 2, ""},
		{"TRICE64_B", 2, ""},

		{"triceF", 2, ""},
		{"trice8F", 2, ""},
		{"Trice16F", 2, ""},
		{"trice32F", 2, ""},
		{"trice64F", 2, ""},

		{"TRICE_F", 2, ""},
		{"TRICE8_F", 2, ""},
		{"TRICE16_F", 2, ""},
		{"TRICE32_F", 2, ""},
		{"TRICE64_F", 2, ""},

		{"trice", 3, "trice32_3"},
		{"trice8", 3, "trice8_3"},
		{"trice16", 3, "trice16_3"},
		{"trice32", 3, "trice32_3"},
		{"trice64", 3, "trice64_3"},

		{"trice_2", 3, ""},
		{"trice8_2", 3, ""},
		{"trice16_2", 3, ""},
		{"trice32_2", 3, ""},
		{"trice64_2", 3, ""},

		{"TRICE_S", 3, ""},
		{"TRICE_N", 3, ""},
		{"TRICE_B", 3, ""},
		{"TRICE_F", 3, ""},

		{"triceS", 3, ""},
		{"triceN", 3, ""},

		{"triceB", 3, ""},
		{"trice8B", 3, ""},
		{"trice16B", 3, ""},
		{"trice32B", 3, ""},
		{"trice64B", 3, ""},

		{"TRICE_B", 3, ""},
		{"TRICE8_B", 3, ""},
		{"TRICE16_B", 3, ""},
		{"TRICE32_B", 3, ""},
		{"TRICE64_B", 3, ""},

		{"triceF", 3, ""},
		{"trice8F", 3, ""},
		{"trice16F", 3, ""},
		{"trice32F", 3, ""},
		{"trice64F", 3, ""},

		{"TRICE_F", 3, ""},
		{"TRICE8_F", 3, ""},
		{"TRICE16_F", 3, ""},
		{"TRICE32_F", 3, ""},
		{"TRICE64_F", 3, ""},

		{"trice", 8, "trice32_8"},
		{"trice8", 8, "trice8_8"},
		{"trice16", 8, "trice16_8"},
		{"trice32", 8, "trice32_8"},
		{"trice64", 8, "trice64_8"},

		{"trice_8", 8, "trice32_8"},
		{"trice8_8", 8, "trice8_8"},
		{"trice16_8", 8, "trice16_8"},
		{"trice32_8", 8, "trice32_8"},
		{"trice64_8", 8, "trice64_8"},

		{"TRICE_S", 0, ""},
		{"TRICE_S", 1, "TRICES"},
		{"TRICE_S", 2, ""},
		{"TRICE_S", 3, ""},
		{"TRICE_S", 7, ""},
		{"TRICE_S", 8, ""},

		{"TRICE_S", 8, ""},
		{"TRICE_N", 8, ""},
		{"TRICE_B", 8, ""},
		{"TRICE_F", 8, ""},

		{"triceS", 8, ""},
		{"triceN", 8, ""},

		{"triceB", 8, ""},
		{"trice8B", 8, ""},
		{"trice16B", 8, ""},
		{"trice32B", 8, ""},
		{"trice64B", 8, ""},

		{"TRICE_B", 8, ""},
		{"TRICE8_B", 8, ""},
		{"TRICE16_B", 8, ""},
		{"TRICE32_B", 8, ""},
		{"TRICE64_B", 8, ""},

		{"triceF", 8, ""},
		{"trice8F", 8, ""},
		{"trice16F", 8, ""},
		{"trice32F", 8, ""},
		{"trice64F", 8, ""},

		{"TRICE_F", 8, ""},
		{"TRICE8_F", 8, ""},
		{"TRICE16_F", 8, ""},
		{"TRICE32_F", 8, ""},
		{"TRICE64_F", 8, ""},

		{"TRICE", 8, "TRICE32_8"},
		{"TRICE8", 8, "TRICE8_8"},
		{"TRICE16", 8, "TRICE16_8"},
		{"TRICE32", 8, "TRICE32_8"},
		{"TRICE64", 8, "TRICE64_8"},

		{"TRICE_8", 8, "TRICE32_8"},
		{"TRICE8_8", 8, "TRICE8_8"},
		{"TRICE16_8", 8, "TRICE16_8"},
		{"TRICE32_8", 8, "TRICE32_8"},
		{"TRICE64_8", 8, "TRICE64_8"},
	}
	for i, x := range tt {
		act, err := ConstructFullTriceInfo(x.ty, x.count)
		fmt.Println(i, x, act, err)
		if err != nil {
			assert.Equal(t, x.exp, "")
		} else {
			assert.Equal(t, x.exp, act)
		}
	}
}
