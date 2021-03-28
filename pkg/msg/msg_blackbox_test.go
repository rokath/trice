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
	e = errors.New("s.th. went wrong")
	// msg.FatalOnErr(e)
	// Output:
	// Error in msg_blackbox_test.go:33: func 'github.com/rokath/trice/pkg/msg_test.ExampleFatalOnErr' -> s.th. went wrong
}

//  func ExamplePanicInfoOnErr() {
//  	log.SetFlags(0)
//  	var e error
//  	msg.PanicInfoOnErr(e, "just in case")
//  	e = errors.New("s.th. went wrong")
//  	// msg.PanicInfoOnErr(e, "just in case")
//  	// Output:
//  	// Error in msg_blackbox_test.go:33: func 'github.com/rokath/trice/pkg/msg_test.ExampleFatalOnErr' -> s.th. went wrong
//  }

func ExampleInfoOnErr() {
	var e error
	msg.InfoOnErr(e, "just in case")
	e = errors.New("s.th. went wrong")
	msg.InfoOnErr(e, "just in case")
	// Output:
	// just in case
	// Error in msg_blackbox_test.go:52: func 'github.com/rokath/trice/pkg/msg_test.ExampleInfoOnErr' -> s.th. went wrong
}

func ExampleFatalInfoOnErr() {
	var e error
	msg.FatalInfoOnErr(e, "just in case")
	e = errors.New("s.th. went wrong")
	// msg.FatalInfoOnErr(e, "just in case")
	// Output:
	// just in case
	// Error in msg_blackbox_test.go:52: func 'github.com/rokath/trice/pkg/msg_test.ExampleInfoOnErr' -> s.th. went wrong
}

func ExampleOnTrue() {
	var f bool
	msg.OnTrue(f)
	msg.OnTrue(!f)
	// Output:
	// Error in msg_blackbox_test.go:71: func 'github.com/rokath/trice/pkg/msg_test.ExampleOnTrue' -> <nil>
}

func ExampleFatalOnTrue() {
	log.SetFlags(0)
	var f bool
	msg.FatalOnTrue(f)
	//msg.FatalOnTrue(!f)
	// Output:
	// Error in msg_blackbox_test.go:71: func 'github.com/rokath/trice/pkg/msg_test.ExampleOnTrue' -> <nil>
}

func ExampleInfoOnTrue() {
	var f bool
	msg.InfoOnTrue(f, "just in case")
	msg.InfoOnTrue(!f, "just in case")
	// Output:
	// Error in msg_blackbox_test.go:88: func 'github.com/rokath/trice/pkg/msg_test.ExampleInfoOnTrue' -> just in case
}

func ExampleFatalInfoOnTrue() {
	log.SetFlags(0)
	var f bool
	msg.FatalInfoOnTrue(f, "just in case")
	//msg.FatalInfoOnTrue(!f, "just in case")
	// Output:
	// Error in msg_blackbox_test.go:71: func 'github.com/rokath/trice/pkg/msg_test.ExampleOnTrue' -> <nil>
}

func ExampleOnFalse() {
	var f bool
	msg.OnFalse(f)
	msg.OnFalse(!f)
	// Output:
	// Error in msg_blackbox_test.go:104: func 'github.com/rokath/trice/pkg/msg_test.ExampleOnFalse' -> <nil>
}

func ExampleFatalOnFalse() {
	log.SetFlags(0)
	var f bool
	msg.FatalOnFalse(f)
	//msg.FatalOnFalse(!f)
	// Output:
	// Error in msg_blackbox_test.go:71: func 'github.com/rokath/trice/pkg/msg_test.ExampleOnFalse' -> <nil>
}

func ExampleInfoOnFalse() {
	var f bool
	msg.InfoOnFalse(f, "just in case")
	msg.InfoOnFalse(!f, "just in case")
	// Output:
	// Error in msg_blackbox_test.go:121: func 'github.com/rokath/trice/pkg/msg_test.ExampleInfoOnFalse' -> just in case
}

func ExampleFatalInfoOnFalse() {
	log.SetFlags(0)
	var f bool
	msg.FatalInfoOnFalse(f, "just in case")
	//msg.FatalInfoOnFalse(!f, "just in case")
	// Output:
	// Error in msg_blackbox_test.go:71: func 'github.com/rokath/trice/pkg/msg_test.ExampleOnFalse' -> <nil>
}
