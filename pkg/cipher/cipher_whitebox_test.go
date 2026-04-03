// SPDX-License-Identifier: MIT

// blackbox test
package cipher

import (
	"os"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestMySecret1(t *testing.T) {
	Password = ""
	checkBig(t)
}

func TestMySecret2(t *testing.T) {
	Password = "aSecret"
	checkBig(t)
}

func TestMySecret3(t *testing.T) {
	Password = ""
	checkMix(t)
}

func TestMySecret4(t *testing.T) {
	Password = "aSecret"
	checkMix(t)
}

func TestMySecret5(t *testing.T) {
	Password = ""
	checkSmall(t)
}
func TestMySecret6(t *testing.T) {
	Password = "aSecret"
	checkSmall(t)
}

func checkBig(t *testing.T) {
	assert.Nil(t, SetUp(os.Stdout))
	src := []byte{0, 1, 2, 3, 4, 5, 6, 7}
	enc := Encrypt8(src)
	dst := Decrypt8(enc)
	assert.Equal(t, src, dst)
}

func checkMix(t *testing.T) {
	assert.Nil(t, SetUp(os.Stdout))
	src := []byte{0, 1, 2, 3, 4, 5, 6, 7}
	enc := Encrypt8(src)
	dst := make([]byte, 8)
	decrypt8(dst, enc)
	assert.Equal(t, src, dst)
}

func checkSmall(t *testing.T) {
	assert.Nil(t, SetUp(os.Stdout))
	src := []byte{0, 1, 2, 3, 4, 5, 6, 7}
	enc := make([]byte, 8)
	encrypt8(enc, src)
	dst := make([]byte, 8)
	decrypt8(dst, enc)
	assert.Equal(t, src, dst)
}

func TestDecryptBufferBoundaries(t *testing.T) {
	Password = "aSecret"
	assert.Nil(t, SetUp(os.Stdout))

	src := []byte{0, 1, 2, 3, 4, 5, 6, 7, 9, 9, 9}
	enc := make([]byte, len(src))
	encrypt8(enc[:8], src[:8])
	copy(enc[8:], src[8:]) // trailing bytes should stay untouched by Decrypt

	dst := make([]byte, 5) // smaller than one block
	n := Decrypt(dst, enc)
	assert.Equal(t, 0, n)

	dst = make([]byte, 8)
	n = Decrypt(dst, enc)
	assert.Equal(t, 8, n)
	assert.Equal(t, src[:8], dst)
}
