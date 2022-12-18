// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package args

import (
	"bytes"
	"fmt"
	"sync"
	"testing"

	"github.com/rokath/trice/pkg/tst"
)

/*
var (

	m *sync.RWMutex

)

	func init() {
		m = new(sync.RWMutex)
	}

	func execHelper1(t *testing.T, expect string) {
		m.Lock()
		defer m.Unlock()
		args.FlagsInit() // maybe needed for clearance of previous tests (global vars)
		var out bytes.Buffer
		doit(&out)
		act := out.String()
		tst.EqualLines(t, expect, act)
	}
*/
var x *sync.RWMutex

func init() {
	x = new(sync.RWMutex)
}

func execHelper(t *testing.T, input []string, expect string) {
	x.Lock()
	defer x.Unlock()
	var out bytes.Buffer
	FlagsInit() // maybe needed for clearance of previous tests (global vars) // todo: is already in init() called
	err := Handler(&out, input)
	if err != nil {
		fmt.Fprint(&out, err)
	}
	act := out.String()
	tst.EqualLines(t, expect, act)
}
