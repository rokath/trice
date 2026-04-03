// SPDX-License-Identifier: MIT

package args

import (
	"bytes"
	"fmt"
	"testing"

	"github.com/rokath/trice/pkg/tst"
	"github.com/spf13/afero"
)

// execHelper runs the exec helper with the provided arguments and expected output.
func execHelper(t *testing.T, input []string, expect string) {
	var out bytes.Buffer
	FlagsInit()                                   // maybe needed for clearance of previous tests (global vars)
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()} // fSys := &afero.Afero{Fs: afero.NewOsFs()},  osFs := os.DirFS(""),
	err := Handler(&out, fSys, input)
	if err != nil {
		fmt.Fprint(&out, err)
	}
	act := out.String()
	tst.EqualLines(t, expect, act)
}
