// SPDX-License-Identifier: MIT

package cipher

import (
	"bytes"
	"io"
	"testing"
)

// resetCipherGlobals restores cipher package globals after a test.
func resetCipherGlobals() func() {
	prevPassword := Password
	prevShow := ShowKey
	deferCI := ci
	return func() {
		Password = prevPassword
		ShowKey = prevShow
		enabled = false
		key = nil
		ci = deferCI
	}
}

// newCipherForTest configures the cipher package for a test and returns a reset function.
func newCipherForTest(t *testing.T, pass string) func() {
	t.Helper()
	reset := resetCipherGlobals()
	Password = pass
	ShowKey = false
	if err := SetUp(io.Discard); err != nil {
		t.Fatalf("SetUp failed: %v", err)
	}
	return reset
}

// TestSwap8Bytes verifies endian reordering used for encrypt/decrypt helpers.
func TestSwap8Bytes(t *testing.T) {
	input := []byte{1, 2, 3, 4, 5, 6, 7, 8}
	got := swap8Bytes(input)
	want := []byte{4, 3, 2, 1, 8, 7, 6, 5}
	if !bytes.Equal(got, want) {
		t.Fatalf("swap8Bytes failed: got %v want %v", got, want)
	}
}

// TestEncryptDecryptRoundTrip ensures Encrypt8/Decrypt8 form a reversible transformation with static key.
func TestEncryptDecryptRoundTrip(t *testing.T) {
	reset := newCipherForTest(t, "0000000000000000")
	defer reset()

	input := []byte{0, 1, 2, 3, 4, 5, 6, 7}
	en := Encrypt8(input)
	if bytes.Equal(en, input) {
		t.Fatalf("Encrypt8 did not change payload")
	}
	out := Decrypt8(en)
	if !bytes.Equal(out, input) {
		t.Fatalf("Decrypt8 round trip failed: got %v want %v", out, input)
	}
}

// TestDecryptCopyLimits checks that Decrypt copies up to the minimum of dst/src lengths even when encryption is disabled.
func TestDecryptCopyLimits(t *testing.T) {
	reset := resetCipherGlobals()
	defer reset()

	Password = ""
	enabled = false
	key = nil
	ci = nil

	src := make([]byte, 16)
	for i := range src {
		src[i] = byte(i)
	}
	dst := make([]byte, 9)
	n := Decrypt(dst, src)
	if n != 8 {
		t.Fatalf("Decrypt returned %d, want 8", n)
	}
	if !bytes.Equal(dst[:8], src[:8]) {
		t.Fatalf("Decrypt copied wrong bytes: %v", dst[:8])
	}
}
