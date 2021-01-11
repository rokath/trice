// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

import (
	"fmt"
	"github.com/rokath/trice/pkg/msg"
	"path/filepath"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestArrayFlag(t *testing.T) {
	p := &Srcs
	assert.Equal(t, "", p.String())

	msg.InfoOnErr("", p.Set("ab"))
	msg.InfoOnErr("", p.Set("xyz"))
	fmt.Println(Srcs)
	var af ArrayFlag
	af = []string{"ab", "xyz"}
	assert.Equal(t, af, Srcs)
	assert.Equal(t, af.String(), Srcs.String())
}

func TestConditionalFilePath(t *testing.T) {
	s := ConditionalFilePath("off")
	assert.Equal(t, "off", s)
	s = ConditionalFilePath("none")
	assert.Equal(t, "none", s)
	s = ConditionalFilePath("/tatue/tata")
	b := filepath.Base(s)
	assert.Equal(t, b, "tata")
}
