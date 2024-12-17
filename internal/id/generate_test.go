package id

import (
	"fmt"
	"testing"

	"github.com/tj/assert"
)

func TestConstructFullTriceInfo(t *testing.T) {
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
