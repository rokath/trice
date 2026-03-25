// SPDX-License-Identifier: MIT

// white-box test
package keybcmd

import (
	"bufio"
	"bytes"
	"strings"
	"testing"
)

func TestDummy(t *testing.T) {
}

// stimulate injects keys to the loop action and returns the captured output as byte slice.
func stimulate(keys, ipa, ipp string) []byte {

	// simulate input
	ioReader := strings.NewReader(keys)
	bufioReader := bufio.NewReader(ioReader)

	var buf bytes.Buffer
	loopAction(bufioReader, &buf, ipa, ipp)

	return buf.Bytes()
}
