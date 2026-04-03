// SPDX-License-Identifier: MIT

//go:build jlink
// +build jlink

package link_test

import (
	"testing"

	"github.com/rokath/trice/internal/link"
	"github.com/stretchr/testify/assert"
)

// TestJlink verifies the expected behavior.
func TestJlink(t *testing.T) {
	link.Port = "JLINK"
	link.SetLinkCommandName()
	p := link.NewReadCloser()
	if nil == p {
		t.Fail()
		return
	}
	assert.Nil(t, p.Open())
	assert.Nil(t, p.Close())
}
