// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package msg_test

import (
	"errors"

	"github.com/rokath/trice/pkg/msg"
)

func ExampleOnErr() {
	var e error
	msg.OnErr(e)
	e = errors.New("s.th. went wrong")
	msg.OnErr(e)
	// output:
	// [error] in github.com/rokath/trice/pkg/msg_test.ExampleOnErr[c:/repos/trice/pkg/msg/msg_test.go:16] s.th. went wrong
}

func ExampleInfoOnErr() {
	var e error
	msg.OnErr(e)
	e = errors.New("s.th. went wrong")
	msg.InfoOnErr("info", e)
	// output:
	// info
	// [error] in github.com/rokath/trice/pkg/msg_test.ExampleInfoOnErr[c:/repos/trice/pkg/msg/msg_test.go:25] s.th. went wrong
}
