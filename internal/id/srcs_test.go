// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

import (
	"testing"

	"github.com/rokath/trice/pkg/msg"

	"github.com/stretchr/testify/assert"
)

// TestArrayFlag checks if method (*arrayFlag).Set works as expected.
// works, but not parallel
func TestArrayFlag(t *testing.T) {
	p := &Srcs
	assert.Equal(t, "", p.String())

	msg.OnErr(p.Set("ab"))
	msg.OnErr(p.Set("xyz"))
	af := arrayFlag([]string{"ab", "xyz"})
	assert.Equal(t, af, Srcs)
	assert.Equal(t, af.String(), Srcs.String())
}

//  // TestConditionalFilePath checks if ConditionalFilePath works as expected.
//  func TestConditionalFilePathAfero(t *testing.T) {
//  	fSys := &afero.Afero{Fs: afero.NewOsFs()} //os.DirFS("")
//  	//s := ConditionalFilePath2(fSys, "off")
//  	//assert.Equal(t, "off", s)
//  	//s = ConditionalFilePath2(fSys, "none")
//  	//assert.Equal(t, "none", s)
//  	s := FullFilePath2(fSys, "/tatue/tata")
//  	b := filepath.Base(s)
//  	assert.Equal(t, b, "tata")
//  }
//
//  // TestConditionalFilePath checks if ConditionalFilePath works as expected.
//  func TestConditionalFilePathOs(t *testing.T) {
//  	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
//  	//s := ConditionalFilePath2(fSys, "off")
//  	//assert.Equal(t, "off", s)
//  	//s = ConditionalFilePath2(fSys, "none")
//  	//assert.Equal(t, "none", s)
//  	s := FullFilePath2(fSys, "/tatue/tata")
//  	b := filepath.Base(s)
//  	assert.Equal(t, b, "tata")
//  }
