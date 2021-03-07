package src

import (
	"testing"

	"github.com/tj/assert"
)

type ttt struct {
	id  int
	fmt string
	b   []byte
}

func Test1(t *testing.T) {
	tt := []ttt{
		{34481, "", []byte{0, 136, 53, 132}},
		{34481, "", []byte{1, 136, 53, 132}},
	}
	for _, x := range tt {
		b := trice0i(len(x.b), x.id, x.fmt)
		assert.Equal(t, b, x.b)
	}
}
