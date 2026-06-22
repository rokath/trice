// SPDX-License-Identifier: MIT

package abc_rx_host

import "testing"

// TestTriceAbcReceiveRuntime verifies ABC dispatch, payload validation, and stamp context restoration.
func TestTriceAbcReceiveRuntime(t *testing.T) {
	for i := 1; i <= 15; i++ {
		if got := triceAbcRxHostCheck(i); got != 0 {
			t.Fatalf("TriceAbcRxHostCheck(%d) returned %d", i, got)
		}
	}
}
