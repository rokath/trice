// SPDX-License-Identifier: MIT

package abc_tx_host

// #cgo CFLAGS: -Wshadow -Wno-format-security -I. -I../../src -I../../src/default_conf
// int TriceAbcTxHostCheck(int n);
import "C"

// triceAbcTxHostCheck executes one C-side transmit macro fixture case.
func triceAbcTxHostCheck(n int) int {
	return int(C.TriceAbcTxHostCheck(C.int(n)))
}
