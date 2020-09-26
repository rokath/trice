// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// +build jlink

package link_test

import (
	"testing"

	"github.com/rokath/trice/internal/link"
	"github.com/stretchr/testify/assert"
)

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
