// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package args

import (
	"fmt"
	"github.com/rokath/trice/pkg/msg"
	"os"
	"testing"

	"github.com/rokath/trice/pkg/tst"
	"github.com/stretchr/testify/assert"
)

func TestVersion(t *testing.T) {
	fi, err := os.Stat(os.Args[0])
	assert.Nil(t, err)
	buildTime := fi.ModTime().String()
	exp := "version=devel, built " + buildTime + "\n"

	fn := func() {
		msg.InfoOnErr("", Handler([]string{"trice", "v"}))
	}
	act := tst.CaptureStdOut(fn)
	assert.Equal(t, exp, act)

	fn = func() {
		msg.InfoOnErr("", Handler([]string{"trice", "ver"}))
	}
	act = tst.CaptureStdOut(fn)
	assert.Equal(t, exp, act)

	fn = func() {
		msg.InfoOnErr("", Handler([]string{"trice", "version"}))
	}
	act = tst.CaptureStdOut(fn)
	assert.Equal(t, exp, act)
}

func Example_handlerNone() {
	fn := func() {
		msg.InfoOnErr("", Handler([]string{"trice", ""}))
	}
	act := tst.CaptureStdOut(fn)
	fmt.Print(act)
	// Output:
	// try: 'trice help|h'
}

func Example_wrongSubcommand() {
	fn := func() {
		msg.InfoOnErr("", Handler([]string{"trice", "xyz"}))
	}
	act := tst.CaptureStdOut(fn)
	fmt.Print(act)
	// Output:
	// try: 'trice help|h'
}
