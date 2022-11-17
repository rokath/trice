// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// whitebox test for package emitter.
package emitter

import (
	"bytes"
	"testing"

	"github.com/rokath/trice/internal/receiver"
	"github.com/rokath/trice/pkg/msg"
	"github.com/stretchr/testify/assert"
)

func TestSetPrefixCOMn(t *testing.T) {
	Prefix = "source:"
	receiver.Port = "COM3"
	setPrefix()
	assert.Equal(t, "COM3:", Prefix)
}

func TestSetPrefix2(t *testing.T) {
	Prefix = "MySpecialDevice:"
	receiver.Port = "COM3"
	setPrefix()
	assert.Equal(t, "MySpecialDevice:", Prefix)
}

func TestSetPrefixNone(t *testing.T) {
	Prefix = "none"
	receiver.Port = "COM3"
	setPrefix()
	assert.Equal(t, "", Prefix)
}

func TestNew(t *testing.T) {
	var out bytes.Buffer
	_ = New(&out)
	act := out.String()
	assert.Equal(t, "", act)
}

// TestNewLineWriter works, but not concurrently
func TestNewLineWriter(t *testing.T) {
	o := msg.OsExitDisallow()
	defer msg.OsExitAllow(o)
	DisplayRemote = true
	var out bytes.Buffer
	_ = newLineWriter(&out)
}
