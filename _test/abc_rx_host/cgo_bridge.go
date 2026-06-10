// SPDX-License-Identifier: MIT

package abc_rx_host

// #cgo CFLAGS: -I../../src -I. -Wshadow -Wno-format-security
// int TriceAbcRxHostCheck(int n);
import "C"

// triceAbcRxHostCheck executes one C-side receive-runtime fixture case.
func triceAbcRxHostCheck(n int) int {
	return int(C.TriceAbcRxHostCheck(C.int(n)))
}
