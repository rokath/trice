// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

import (
	"fmt"
	"path/filepath"
	"reflect"
	"testing"

	"github.com/rokath/trice/pkg/msg"
	"github.com/spf13/afero"

	"github.com/stretchr/testify/assert"
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
