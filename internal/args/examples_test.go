// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package args

import (
	"fmt"

	"github.com/rokath/trice/pkg/tst"
)

func Example_handlerNone() {
	fn := func() {
		err := Handler([]string{"trice"})
		if nil != err {
			fmt.Print(err)
		}
	}
	act := tst.CaptureStdOut(fn)
	fmt.Print(act)
	// Output:
	// no args, try: 'trice help'
}

func Example_wrongSubcommand() {
	fn := func() {
		err := Handler([]string{"trice", "xyz"})
		if nil != err {
			fmt.Print(err)
		}
	}
	act := tst.CaptureStdOut(fn)
	fmt.Print(act)
	// Output:
	// unknown sub-command 'xyz'. try: 'trice help|h'
}

func Example_renew() {
	fn := func() {
		m.Lock()
		err := Handler([]string{"trice", "renew"})
		m.Unlock()
		if nil != err {
			fmt.Print(err)
		}
	}
	act := tst.CaptureStdOut(fn)
	fmt.Print(act)
	// Output:
}

func Example_refresh() {
	fn := func() {
		m.Lock()
		err := Handler([]string{"trice", "refresh"})
		m.Unlock()
		if nil != err {
			fmt.Print(err)
		}
	}
	act := tst.CaptureStdOut(fn)
	fmt.Print(act)
	// Output:
}

func Example_update() {
	fn := func() {
		m.Lock()
		err := Handler([]string{"trice", "update"})
		m.Unlock()
		if nil != err {
			fmt.Print(err)
		}
	}
	act := tst.CaptureStdOut(fn)
	fmt.Print(act)
	// Output:
}

func Example_log() {
	fn := func() {
		m.Lock()
		err := Handler([]string{"trice", "log", "-p", "COMX"})
		m.Unlock()
		if nil != err {
			fmt.Print(err)
		}
	}
	act := tst.CaptureStdOut(fn)
	fmt.Print(act)
	// Output:
	// can not open COMX
}

func Example_help_a() {
	fn := func() {
		err := Handler([]string{"trice", "help"})
		if nil != err {
			fmt.Print(err)
		}
	}
	h.Lock()
	m.Lock()
	act := tst.CaptureStdOut(fn)
	m.Unlock()
	h.Unlock()
	fmt.Print(act)
	// Output:
	// syntax: 'trice sub-command' [params]
	// example 'trice h -help': Print help for help.
}

func Example_help_b() {
	fn := func() {
		err := Handler([]string{"trice", "help", "-sd", "-v"})
		if nil != err {
			fmt.Print(err)
		}
	}
	h.Lock()
	m.Lock()
	act := tst.CaptureStdOut(fn)
	m.Unlock()
	h.Unlock()
	fmt.Print(act)
	// Output:
	// *** https://github.com/rokath/trice ***
	//
	// If a non-multi parameter is used more than one times the last value wins.
	// No logfile writing...
	// syntax: 'trice sub-command' [params]
	// sub-command 'sd|shutdown': Ends display server at IPA:IPP, works also on a remote machine.
	//   -ipa string
	//     	IP address like '127.0.0.1'.
	//     	You can specify this switch if you intend to use the remote display option to show the output on a different PC in the network.
	//     	 (default "localhost")
	//   -ipp string
	//     	16 bit IP port number.
	//     	You can specify this switch if you want to change the used port number for the remote display functionality.
	//     	 (default "61497")
	// example: 'trice sd': Shut down remote display server.
	// No logfile writing...done
}
