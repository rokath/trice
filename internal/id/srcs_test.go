// SPDX-License-Identifier: MIT

package id

import (
	"bytes"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"reflect"
	"testing"

	"github.com/rokath/trice/pkg/msg"
	"github.com/spf13/afero"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// TestArrayFlag checks if method (*arrayFlag).Set works as expected.
// works, but not parallel
func TestArrayFlag(t *testing.T) {
	p := &Srcs
	assert.Equal(t, "", p.String())

	msg.OnErr(p.Set("ab"))
	msg.OnErr(p.Set("xyz"))
	af := ArrayFlag([]string{"ab", "xyz"})
	assert.Equal(t, af, Srcs)
	assert.Equal(t, af.String(), Srcs.String())
}

// TestConditionalFilePath checks if ConditionalFilePath works as expected.
func TestConditionalFilePathAfero(t *testing.T) {
	FSys := &afero.Afero{Fs: afero.NewOsFs()}
	s := fullFilePath2(FSys, "/tatue/tata")
	b := filepath.Base(s)
	assert.Equal(t, b, "tata")
}

// TestConditionalFilePath checks if ConditionalFilePath works as expected.
func TestConditionalFilePathOs(t *testing.T) {

	s := fullFilePath2(FSys, "/tatue/tata")
	b := filepath.Base(s)
	assert.Equal(t, b, "tata")
}

// TestCompactSrcs verifies the expected behavior.
func TestCompactSrcs(t *testing.T) {
	defer Setup(t)()

	Srcs = nil
	CompactSrcs()
	assert.Equal(t, ArrayFlag{"."}, Srcs)

	Srcs = ArrayFlag{"./b", "./a", "./a", "sub/../b"}
	CompactSrcs()
	assert.Equal(t, ArrayFlag{"a", "b"}, Srcs)
}

// TestWalkSrcs verifies the expected behavior.
func TestWalkSrcs(t *testing.T) {
	defer Setup(t)()

	require.NoError(t, FSys.MkdirAll(Proj+"src", 0o777))
	Srcs = ArrayFlag{Proj + "src", Proj + "missing"}

	var visited []string
	var out bytes.Buffer
	walkSrcs(&out, FSys, nil, nil, nil, nil, func(_ io.Writer, _ *afero.Afero, root string, _ TriceIDLookUp, _ triceFmtLookUp, _ *bool, _ TriceIDLookUpLI) {
		visited = append(visited, root)
	})

	assert.Equal(t, []string{Proj + "src"}, visited)
	assert.Contains(t, out.String(), "does not exist!")
}

// TestAddToListAdapter verifies the expected behavior.
func TestAddToListAdapter(t *testing.T) {
	defer Setup(t)()

	tempDir := t.TempDir()
	fs := &afero.Afero{Fs: afero.NewOsFs()}
	sourceFile := filepath.Join(tempDir, "demo.c")
	const src = `TRice(iD(1234), "msg:%d", 7);`
	require.NoError(t, os.WriteFile(sourceFile, []byte(src), 0o644))

	ilu := make(TriceIDLookUp)
	flu := make(triceFmtLookUp)
	lim := make(TriceIDLookUpLI)
	modified := false

	addToListAdapter(W, fs, sourceFile, ilu, flu, &modified, lim)

	require.Contains(t, ilu, TriceID(1234))
	assert.Equal(t, "TRice", ilu[1234].Type)
	assert.Equal(t, "msg:%d", ilu[1234].Strg)
	assert.Contains(t, lim, TriceID(1234))
	assert.Equal(t, filepath.Base(sourceFile), lim[1234].File)
	assert.Equal(t, 1, lim[1234].Line)
}

// fullFilePath2 returns absolute file path if fn is not "off" or "none".
func fullFilePath2(FSys *afero.Afero, fn string) string {
	xType := reflect.TypeOf(FSys)
	xValue := reflect.ValueOf(FSys)
	if Verbose {
		fmt.Println(xType, xValue) // Os: *afero.Afero &{0x85d228} // MemMap: *afero.Afero &{0xc00007bb60}
	}
	if fn == "none" || fn == "off" {
		return fn
	}
	var e error
	var s string
	s, e = filepath.Abs(fn)
	msg.InfoOnErr(e, fmt.Sprintf("failed to parse %s\n", fn))
	return s
}
