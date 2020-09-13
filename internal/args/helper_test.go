// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package args

import (
	"bytes"
	"io"
	"os"
)

// captureStdout captures stdout and stderr.
func captureStdout(f func()) string {

	// keep backup of the real stdout
	old := os.Stdout

	// re-direct stdout
	r, w, err := os.Pipe()
	if nil != err {
		return err.Error()
	}
	os.Stdout = w
	defer func() {
		// restoring the real stdout
		os.Stdout = old
	}()

	// copy the output in a separate goroutine so printing can't block indefinitely
	outC := make(chan string)
	go func() {
		var buf bytes.Buffer
		io.Copy(&buf, r)
		outC <- buf.String()
	}()

	// run the function
	f()

	// back to normal state
	err = w.Close()
	if nil != err {
		return err.Error()
	}

	// read output
	return <-outC
}
