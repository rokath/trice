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

func TestNewLineWriter(t *testing.T) {
	o := msg.OsExitDisallow()
	defer msg.OsExitAllow(o)
	DisplayRemote = true
	var out bytes.Buffer
	_ = newLineWriter(&out)
}

//  func TestNewLineWriter2(t *testing.T) {
//  	o := msg.OsExitDisallow()
//  	defer msg.OsExitAllow(o)
//  	DisplayRemote = true
//  	Autostart = true
//  	_ = newLineWriter()
//  }

//  func TestNewRemoteDisplay(t *testing.T) {
//  	o := msg.OsExitDisallow()
//  	defer msg.OsExitAllow(o)
//  	_ = NewRemoteDisplay(os.Stdout,os.Args)
//  }

/*
func ExampleNewRemoteDisplay() {
	o := msg.OsExitDisallow()
	defer msg.OsExitAllow(o)
	_ = NewRemoteDisplay(os.Stdout, "", "-lg off", "localhost", "11111")
	// Output:
	// Error in [remoteDisplay.go %!s(int=110) github.com/rokath/trice/internal/emitter.(*RemoteDisplay).Connect dial tcp [::1]:11111: connectex: No connection could be made because the target machine actively refused it.]:%!d(MISSING): func '%!s(MISSING)' -> %!v(MISSING)
}
*/
