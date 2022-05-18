// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

import (
	"path/filepath"
	"testing"

	"github.com/rokath/trice/pkg/msg"

	"github.com/stretchr/testify/assert"
)

// TestArrayFlag checks if method (*arrayFlag).Set works as expected.
// works, but not parallel
func _TestArrayFlag(t *testing.T) {
	p := &Srcs
	assert.Equal(t, "", p.String())

	msg.OnErr(p.Set("ab"))
	msg.OnErr(p.Set("xyz"))
	af := arrayFlag([]string{"ab", "xyz"})
	assert.Equal(t, af, Srcs)
	assert.Equal(t, af.String(), Srcs.String())
}

// TestConditionalFilePath checks if ConditionalFilePath works as expected.
func TestConditionalFilePath(t *testing.T) {
	s := ConditionalFilePath("off")
	assert.Equal(t, "off", s)
	s = ConditionalFilePath("none")
	assert.Equal(t, "none", s)
	s = ConditionalFilePath("/tatue/tata")
	b := filepath.Base(s)
	assert.Equal(t, b, "tata")
}
