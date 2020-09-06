// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package assert provides hepler functions for testing.
package assert_test

import (
	"os"
	"testing"

	"github.com/rokath/trice/pkg/assert"
)

// test helper ///////////////////////////////////////////////////////////////////////
//

func TestTrue(t *testing.T) {
	assert.True(t, true, "")
}

func TestFalse(t *testing.T) {
	assert.False(t, false, "")
}

func TestErrorNil(t *testing.T) {
	assert.ErrorNil(t, nil)
}

func TestEqual(t *testing.T) {
	assert.Equal(t, 33, 33)
}

func TestEqualLines(t *testing.T) {
	exp := "Hello\r\nWorld\r\n"
	act := "Hello\nWorld\n"
	assert.EqualLines(t, exp, act)
}

func TestEqualFiles(t *testing.T) {
	assert.EqualLines(t, os.Args[0], os.Args[0])
}
