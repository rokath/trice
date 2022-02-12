// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package receiver_test

import (
	"io"
	"os"
	"testing"

	"github.com/rokath/trice/internal/receiver"
	"github.com/tj/assert"
)

func Test1(t *testing.T) {
	var verbose bool
	rc, err := receiver.NewReadCloser(os.Stdout, verbose, "BUFFER", "7")
	assert.Nil(t, err)
	b := make([]byte, 100)
	n, err := rc.Read(b)
	assert.Nil(t, err)
	assert.True(t, 1 == n)
	assert.True(t, 7 == b[0])
	n, err = rc.Read(b)
	assert.True(t, 0 == n)
	assert.True(t, io.EOF == err)
	assert.Nil(t, rc.Close())
}
