// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

import (
	"fmt"
	"path/filepath"
	"testing"
)

func TestArrayFlag(t *testing.T) {
	p := &Srcs
	equals(t, "my string representation", p.String())

	p.Set("ab")
	p.Set("xyz")
	fmt.Println(Srcs)
	var af ArrayFlag
	af = []string{"ab", "xyz"}
	equals(t, af, Srcs)
}

func TestConditinalFilePath(t *testing.T) {
	s := ConditinalFilePath("off")
	equals(t, "off", s)
	s = ConditinalFilePath("none")
	equals(t, "none", s)
	s = ConditinalFilePath("/tatue/tata")
	b := filepath.Base(s)
	equals(t, b, "tata")
}
