// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package jlink_test is a blackbox test
package jlink_test

import (
	"testing"

	"github.com/rokath/trice/internal/receiver/jlink"
	"github.com/rokath/trice/pkg/lib"
)

func Test1(t *testing.T) {
	p := jlink.New("-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0")
	if nil == p {
		t.Fail()
		return
	}
	err := p.Open()
	lib.Equals(t, nil, err)
	err = p.Close()
	lib.Equals(t, nil, err)
}
