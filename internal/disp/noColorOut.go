// +build nocolor
// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// USE THIS FILE instead of dispAnsiOut.go, when color output is never used.

package disp

import (
	"fmt"
	"log"
	"strings"
	"sync"
)

var (
	// Writeline is an exported function pointer, which can be redirected for example to a client call
	WriteLine = writeLine

	// mux is for syncing line output
	mux sync.Mutex
)

// writeLine displays ss and sets color.
// ss is a slice containing all substring parts of one line.
// Each substring inside ss is result of Trice or contains prefix,
// timestamp or postfix and can have its own color prefix.
// The last substring inside the slice is definitly the last in
// the line and has already trimmed its newline.
// Linebreaks inside the substrings are not handled separately (yet).
func writeLine(ss []string) error {
	var line string

	mux.Lock()
	for _, s := range ss {
		c := noColorize(s)
		line += fmt.Sprint(c)
	}

	log.SetFlags(0)
	log.Print(line) // here better use simply fmt.Print, but then the io.Writer to os.file assignment issue must be solved

	mux.Unlock()
	return nil
}

// noColorize does: if col: is begin of string remove col:
func noColorize(s string) string {
	sc := strings.SplitN(s, ":", 2)
	switch sc[0] {
	case "e", "err", "error",
		"w", "wrn", "warning",
		"m", "msg", "message",
		"rd", "rd_",
		"wr", "wr_",
		"tim", "time",
		"att", "attention",
		"d", "db", "dbg", "debug",
		"dia", "diag",
		"isr", "interrupt",
		"s", "sig", "signal",
		"t", "tst", "test",
		"i", "inf", "info", "informal":
		s = sc[1] // remove channel info
	}
	return s
}
