// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package receiver_test

import (
	"io"
	"testing"

	"github.com/rokath/trice/internal/receiver"
	"github.com/rokath/trice/pkg/tst"
)

func Test1(t *testing.T) {
	buf := string([]byte{7})
	rc, err := receiver.NewReadCloser("BUFFER", buf)
	tst.AssertNoErr(t, err)
	b := make([]byte, 100)
	n, err := rc.Read(b)
	tst.AssertNoErr(t, err)
	tst.AssertTrue(t, 1 == n)
	tst.AssertTrue(t, 7 == b[0])
	n, err = rc.Read(b)
	tst.AssertTrue(t, 0 == n)
	tst.AssertTrue(t, io.EOF == err)
	tst.AssertNoErr(t, rc.Close())
}
