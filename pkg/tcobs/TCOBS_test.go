package src

import (
	"testing"

	"github.com/tj/assert"
)

func TestTCOBSEncode(t *testing.T) {
	i := []byte{0, 0, 0}
	o := make([]byte, 100)
	n := TCOBSEncodeC(o, i)
	assert.Equal(n, 1)
}
