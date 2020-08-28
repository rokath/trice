// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package jlink_test is a blackbox test
package jlink_test

import (
	"testing"

	"github.com/rokath/trice/internal/jlink"
)

func Test1(t *testing.T) {
	p := jlink.New("-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0")
	if nil == p {
		t.Fail()
		return
	}
	err := p.Open()
	notEqualFail(t, nil, err)
	err = p.Close()
	notEqualFail(t, nil, err)
}
