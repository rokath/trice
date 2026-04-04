// SPDX-License-Identifier: MIT

// blackbox test
package cipher

import (
	"os"
	"testing"

	"github.com/stretchr/testify/assert"
)

// TestMySecret1 verifies the expected behavior.
func TestMySecret1(t *testing.T) {
	Password = ""
	checkBig(t)
}

// TestMySecret2 verifies the expected behavior.
func TestMySecret2(t *testing.T) {
	Password = "aSecret"
	checkBig(t)
}

// TestMySecret3 verifies the expected behavior.
func TestMySecret3(t *testing.T) {
	Password = ""
	checkMix(t)
}

// TestMySecret4 verifies the expected behavior.
func TestMySecret4(t *testing.T) {
	Password = "aSecret"
	checkMix(t)
}

// TestMySecret5 verifies the expected behavior.
func TestMySecret5(t *testing.T) {
	Password = ""
	checkSmall(t)
}

// TestMySecret6 verifies the expected behavior.
func TestMySecret6(t *testing.T) {
	Password = "aSecret"
	checkSmall(t)
}

// checkBig verifies decryption for the large white-box cipher test vector.
func checkBig(t *testing.T) {
	assert.Nil(t, SetUp(os.Stdout))
	src := []byte{0, 1, 2, 3, 4, 5, 6, 7}
	enc := Encrypt8(src)
	dst := Decrypt8(enc)
	assert.Equal(t, src, dst)
}

// checkMix verifies decryption for the mixed white-box cipher test vector.
func checkMix(t *testing.T) {
	assert.Nil(t, SetUp(os.Stdout))
	src := []byte{0, 1, 2, 3, 4, 5, 6, 7}
	enc := Encrypt8(src)
	dst := make([]byte, 8)
	decrypt8(dst, enc)
	assert.Equal(t, src, dst)
}

// checkSmall verifies decryption for the small white-box cipher test vector.
func checkSmall(t *testing.T) {
	assert.Nil(t, SetUp(os.Stdout))
	src := []byte{0, 1, 2, 3, 4, 5, 6, 7}
	enc := make([]byte, 8)
	encrypt8(enc, src)
	dst := make([]byte, 8)
	decrypt8(dst, enc)
	assert.Equal(t, src, dst)
}

// TestDecryptBufferBoundaries verifies the expected behavior.
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
