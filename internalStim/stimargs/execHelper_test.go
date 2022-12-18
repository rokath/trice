// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package stimargs

import (
	"bytes"
	"fmt"
	"sync"
	"testing"

	"github.com/rokath/trice/pkg/tst"
)

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
