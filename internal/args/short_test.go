// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package args

import (
	"fmt"
	"log"
	"testing"

	"github.com/rokath/trice/pkg/msg"
	"github.com/rokath/trice/pkg/tst"
	"github.com/stretchr/testify/assert"
)

func TestVersion(t *testing.T) {
	verbose = false
	v := []string{"", ""}
	testVersion(t, v)
	m.Lock()
	verbose = true
	v[0] = "No logfile writing...\nhttps://github.com/rokath/trice\n"
	v[1] = "No logfile writing...done\n"
	testVersion(t, v)
	verbose = false
	m.Unlock()
}

func TestScan(t *testing.T) {
	fn := func() {
		err := Handler([]string{"trice", "scan"})
		if nil != err {
			fmt.Print(err)
		}
	}
	fn()
	act := tst.CaptureStdOut(fn)
	fmt.Print(act)
	s := "Found port:"
	assert.Equal(t, s, act[:len(s)])
}

func TestHelpScan(t *testing.T) {
	fn := func() {
		err := Handler([]string{"trice", "help", "-s"})
		if nil != err {
			fmt.Print(err)
		}
	}
	h.Lock()
	m.Lock()
	act := tst.CaptureStdOut(fn)
	allHelp = false
	m.Unlock()
	h.Unlock()
	fmt.Print(act)
	exp := `syntax: 'trice sub-command' [params]
      sub-command 's|scan': Shows available serial ports)
      example: 'trice s': Show COM ports.          
      `
	tst.EqualLines(t, exp, act)
}

func TestHelpZeroSourceTreeIds(t *testing.T) {
	fn := func() {
		err := Handler([]string{"trice", "help", "-z"})
		if nil != err {
			fmt.Print(err)
		}
	}
	h.Lock()
	m.Lock()
	act := tst.CaptureStdOut(fn)
	allHelp = false
	m.Unlock()
	h.Unlock()
	fmt.Print(act)
	exp := `syntax: 'trice sub-command' [params]
      sub-command 'zeroSourceTreeIds': Set all Id(n) inside source tree dir to Id(0).
              Avoid using this sub-command normally. The switch "-src" is mandatory and no multi-flag here.
              This sub-command is mainly for testing. For several source directories you need several runs.
        -dry-run
              No changes applied but output shows what would happen.
              "trice zeroSourceTreeIds -dry-run" will change nothing but show changes it would perform without the "-dry-run" switch.
              This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.
        -src string
              Zero all Id(n) inside source tree dir, required.
      example: 'trice zeroSourceTreeIds -src ../A': Sets all TRICE IDs to 0 in ../A. Use with care!          
      `
	tst.EqualLines(t, exp, act)
}

func _TestShutdown(t *testing.T) { // crashes

	//defer func() {
	//	msg.OsExitAllow(o)
	//}()
	log.SetFlags(0)
	fn := func() {
		o := msg.OsExitDisallow()
		err := Handler([]string{"trice", "sd"})
		if nil != err {
			fmt.Print(err)
		}
		msg.OsExitAllow(o)
	}
	act := tst.CaptureStdOut(fn)
	fmt.Print(act)
	s := "Found port:"
	assert.Equal(t, s, act[:len(s)])

}
