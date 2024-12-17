package decoder

import (
	"fmt"
	"testing"

	"github.com/tj/assert"
)

func Test_UReplaceN(t *testing.T) {
	tt := []struct {
		fmt string // fmt is the Trice format string written by the user in the target source code.
		exp string // exp is the modified fmt.
		u   []int  // u is the format specifiers modification information.
	}{
		{"a %s b", "a %s b", []int{5}},
		{"%x", "%x", []int{1}},
		{"% x", "% x", []int{1}},
		{"%u", "%d", []int{0}},
		{"%d", "%d", []int{1}},
		{"%d %u %f", "%d %d %f", []int{1, 0, 2}},
	}
	for i, x := range tt {
		fmt.Println(i, x)
		act, uct := UReplaceN(x.fmt)
		assert.Equal(t, x.exp, act)
		assert.Equal(t, x.u, uct)
	}
}
