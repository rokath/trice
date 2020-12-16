// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package args

import (
	"fmt"
	"os"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestVersion(t *testing.T) {
	fi, err := os.Stat(os.Args[0])
	assert.Nil(t, err)
	buildTime := fi.ModTime().String()
	exp := "version=devel, built " + buildTime + "\n"

	fn := func() {
		Handler([]string{"trice", "v"})
	}
	act := captureStdout(fn)
	assert.Equal(t, exp, act)

	fn = func() {
		Handler([]string{"trice", "ver"})
	}
	act = captureStdout(fn)
	assert.Equal(t, exp, act)

	fn = func() {
		Handler([]string{"trice", "version"})
	}
	act = captureStdout(fn)
	assert.Equal(t, exp, act)
}

func Example_handlerNone() {
	fn := func() {
		Handler([]string{"trice", ""})
	}
	act := captureStdout(fn)
	fmt.Print(act)
	// Output:
	// try: 'trice help|h'
}

func Example_wrongSubcommand() {
	fn := func() {
		Handler([]string{"trice", "xyz"})
	}
	act := captureStdout(fn)
	fmt.Print(act)
	// Output:
	// try: 'trice help|h'
}

func Example_vwrongSubcommand() {
	fn := func() {
		Handler([]string{"trice", "xyz"})
	}
	act := captureStdout(fn)
	fmt.Print(act)
	// Output:
	// try: 'trice help|h'
}
