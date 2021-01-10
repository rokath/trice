// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package msg_test

import (
	"errors"

	"github.com/rokath/trice/pkg/msg"
)

func ExampleInfoOnErr_a() {
	var e error
	msg.InfoOnErr("", e)
	e = errors.New("s.th. went wrong")
	msg.InfoOnErr("", e)
	// Output:
	// Error in msg_test.go:16: func 'github.com/rokath/trice/pkg/msg_test.ExampleInfoOnErr_a' -> s.th. went wrong
}

func ExampleInfoOnErr_b() {
	var e error
	msg.InfoOnErr("", e)
	e = errors.New("s.th. went wrong")
	msg.InfoOnErr("info", e)
	// Output:
	// info
	// Error in msg_test.go:25: func 'github.com/rokath/trice/pkg/msg_test.ExampleInfoOnErr_b' -> s.th. went wrong
}
