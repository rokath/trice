// SPDX-License-Identifier: MIT

package receiver

import (
	"bytes"
	"testing"
)

// TestSpaceStringsBuilder verifies the expected behavior.
func TestSpaceStringsBuilder(t *testing.T) {
	got := spaceStringsBuilder(" a b\tc\n")
	if got != "abc" {
		t.Fatalf("unexpected result: %q", got)
	}
}

// TestRemoveWhitespaces verifies the expected behavior.
func TestRemoveWhitespaces(t *testing.T) {
	if removeWhitespaces("x y z") != "xyz" {
		t.Fatalf("removeWhitespaces failed")
	}
}

// TestScanHexDump verifies the expected behavior.
func TestScanHexDump(t *testing.T) {
	data, err := scanHexDump("09 a1 fe")
	if err != nil {
		t.Fatalf("scanHexDump: %v", err)
	}
	if !bytes.Equal(data, []byte{0x09, 0xa1, 0xfe}) {
		t.Fatalf("unexpected data: %#v", data)
	}
}

// TestScanBytes verifies the expected behavior.
func TestScanBytes(t *testing.T) {
	got := scanBytes("10 11,12\n13")
	expected := []byte{10, 11, 12, 13}
	if !bytes.Equal(got, expected) {
		t.Fatalf("unexpected bytes: %#v", got)
	}
}
