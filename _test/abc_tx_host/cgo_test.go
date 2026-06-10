// SPDX-License-Identifier: MIT

package abc_tx_host

import "testing"

// TestTriceAbcTransmitMacros verifies selected ABC macro spellings emit the expected raw Trice bytes.
func TestTriceAbcTransmitMacros(t *testing.T) {
	for i := 1; i <= 6; i++ {
		if got := triceAbcTxHostCheck(i); got != 0 {
			t.Fatalf("TriceAbcTxHostCheck(%d) returned %d", i, got)
		}
	}
}
