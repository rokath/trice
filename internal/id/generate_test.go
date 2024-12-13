package id

import (
	"fmt"
	"testing"

	"github.com/tj/assert"
)

func TestConstructFullTriceInfo(t *testing.T) {
	tt := []struct {
		ty    string
		count int
		exp   string
		ok    bool
	}{
		{"trice", 3, "trice32_3", true},
		{"TRICE0", 0, "TRICE0", true},
		{"TRICE0", 1, "TRICE0", false},
		{"trice", 0, "trice0", true},
		{"TRice16", 0, "TRice0", true},
		{"TRice16_0", 0, "TRice0", true},
		{"TRice16_1", 1, "TRice0", true},
		{"trice7", 2, "trice32_7", true},

		{"triceB", 2, "trice32B", true},
		{"trice8F", 2, "trice8F", true},
		{"TRICE16_B", 2, "TRICE16_B", true},
		{"TriceS", 1, "TriceS", true},
		{"TRiceN", 2, "TRiceN", true},
	}
	for i, x := range tt {
		act, err := ConstructFullTriceInfo(x.ty, x.count)
		fmt.Println(i, x, act, err)
		if err != nil {
			assert.False(t, x.ok)
		} else {
			assert.Equal(t, x.exp, act)
			assert.True(t, x.ok)
		}
	}
}
