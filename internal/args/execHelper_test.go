// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package args

import (
	"bytes"
	"fmt"
	"sync"
	"testing"

	"github.com/rokath/trice/pkg/tst"
	"github.com/spf13/afero"
)

var m *sync.RWMutex

func init() {
	m = new(sync.RWMutex)
}

func execHelper(t *testing.T, input []string, expect string) {
	m.Lock()
	defer m.Unlock()
	var out bytes.Buffer
	FlagsInit()                               // maybe needed for clearance of previous tests (global vars) // todo: is already in init() called
	fSys := &afero.Afero{Fs: afero.NewOsFs()} // osFs := os.DirFS("")
	err := Handler(&out, fSys, input)
	if err != nil {
		fmt.Fprint(&out, err)
	}
	act := out.String()
	tst.EqualLines(t, expect, act)
}
