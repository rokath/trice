// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

//go:build stlink
// +build stlink

package link_test

import (
	"testing"

	"github.com/rokath/trice/internal/link"
	"github.com/stretchr/testify/assert"
)

func TestStLink(t *testing.T) {
	link.Port = "STLINK"
	link.SetLinkCommandName()
	p := link.NewReadCloser()
	if nil == p {
		t.Fail()
		return
	}
	assert.Nil(t, p.Open())
	assert.Nil(t, p.Close())
}
