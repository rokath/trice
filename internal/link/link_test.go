// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package link_test is a blackbox test
package link_test

import (
	"testing"

	"github.com/rokath/trice/internal/link"
	"github.com/stretchr/testify/assert"
)

func Test1(t *testing.T) {
	p := link.NewReadCloser("-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0")
	if nil == p {
		t.Fail()
		return
	}
	assert.Nil(t, p.Open())
	assert.Nil(t, p.Close())
}
