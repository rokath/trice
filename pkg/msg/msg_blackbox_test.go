// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package msg_test

import (
	"errors"
	"log"

	"github.com/rokath/trice/pkg/msg"
)

func ExampleInfo() {
	msg.Info("code issue")
	// Output:
	// Error in msg_blackbox_test.go:14: func 'github.com/rokath/trice/pkg/msg_test.ExampleInfo' -> code issue
}

func ExampleOnErr() {
	var e error
	msg.OnErr(e)
	e = errors.New("s.th. went wrong")
	msg.OnErr(e)
	// Output:
	// Error in msg_blackbox_test.go:23: func 'github.com/rokath/trice/pkg/msg_test.ExampleOnErr' -> s.th. went wrong
}

func ExampleFatalOnErr() {
	log.SetFlags(0)
	var e error
	msg.FatalOnErr(e)
	// Output:
}

func ExampleInfoOnErr() {
	var e error
	msg.InfoOnErr(e, "just in case")
	e = errors.New("s.th. went wrong")
	msg.InfoOnErr(e, "just in case")
	// Output:
	// just in case
	// Error in msg_blackbox_test.go:39: func 'github.com/rokath/trice/pkg/msg_test.ExampleInfoOnErr' -> s.th. went wrong
}

func ExampleFatalInfoOnErr() {
	var e error
	msg.FatalInfoOnErr(e, "just in case")
	// Output:
}

func ExampleOnTrue() {
	msg.OnTrue(false)
	msg.OnTrue(true)
	// Output:
	// Error in msg_blackbox_test.go:53: func 'github.com/rokath/trice/pkg/msg_test.ExampleOnTrue' -> <nil>
}

func ExampleFatalOnTrue() {
	msg.FatalOnTrue(false)
	// Output:
}

func ExampleInfoOnTrue() {
	msg.InfoOnTrue(false, "just in case")
	msg.InfoOnTrue(true, "just in case")
	// Output:
	// Error in msg_blackbox_test.go:65: func 'github.com/rokath/trice/pkg/msg_test.ExampleInfoOnTrue' -> just in case
}

func ExampleFatalInfoOnTrue() {
	msg.FatalInfoOnTrue(false, "just in case")
	// Output:
}

func ExampleOnFalse() {
	msg.OnFalse(true)
	msg.OnFalse(false)
	// Output:
	// Error in msg_blackbox_test.go:77: func 'github.com/rokath/trice/pkg/msg_test.ExampleOnFalse' -> <nil>
}

func ExampleFatalOnFalse() {
	msg.FatalOnFalse(true)
	// Output:
}

func ExampleInfoOnFalse() {
	msg.InfoOnFalse(true, "just in case")
	msg.InfoOnFalse(false, "just in case")
	// Output:
	// Error in msg_blackbox_test.go:89: func 'github.com/rokath/trice/pkg/msg_test.ExampleInfoOnFalse' -> just in case
}

func ExampleFatalInfoOnFalse() {
	msg.FatalInfoOnFalse(true, "just in case")
	// Output:
}
