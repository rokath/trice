// +build color
// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// OBSOLETE VARIANT - still working but now replaced by dispAnsiOut.go

// Package disp is for dispatching and displaying trice log lines
// This file contains the print code code.
package disp

import (
	"errors"
	"fmt"
	"strings"
	"sync"

	"github.com/fatih/color"
)

var (
	// separate for performance to avoid re-allocation
	noColor = color.New()

	// WriteLine is an exported function pointer, which can be redirected for example to a client call
	WriteLine = writeLine

	// mux is for syncing line output
	mux sync.Mutex
)

// writeLine displays ss and sets color.
// ss is a slice containing substring parts of one line.
// Each substring inside ss is result of Trice or contains prefix,
// timestamp or postfix and can have its own color prefix.
// The last substring inside the slice is definitly the last in
// the line and has already trimmed its newline.
// Linebreaks inside the substrings are not handled separately (yet).
func writeLine(ss []string) error {
	var c *color.Color
	var line string

	mux.Lock()
	for _, s := range ss {
		c, s = colorChannel(s)
		if true == color.NoColor {
			line += fmt.Sprint(s)
		} else {
			line += c.Sprint(s)
		}
	}
	o := color.NoColor
	color.NoColor = true
	c.Print(line) // here better use simply fmt.Print, but then the io.Writer to os.file assignment issue must be solved
	color.NoColor = o
	mux.Unlock()
	return nil
}

func colorSetDefault(channel string) (*color.Color, error) {
	var err error
	var c *color.Color
	switch channel {
	case "ERR", "err":
		c = color.New(color.FgYellow).Add(color.Bold).Add(color.BgRed)
	case "WRN", "wrn":
		c = color.New(color.FgRed).Add(color.BgBlack)
	case "MSG", "msg":
		c = color.New(color.FgGreen).Add(color.BgBlack)
	case "RD_", "rd_":
		c = color.New(color.FgMagenta).Add(color.BgBlack)
	case "WR_", "wr_":
		c = color.New(color.FgBlack).Add(color.BgMagenta)
	case "TIM", "tim":
		c = color.New(color.FgBlue).Add(color.BgYellow)
	case "ATT", "att":
		c = color.New(color.FgYellow).Add(color.Bold).Add(color.BgCyan)
	case "DBG", "dbg":
		c = color.New(color.FgCyan).Add(color.BgBlack)
	case "DIA", "dia":
		c = color.New(color.BgHiCyan).Add(color.BgWhite)
	case "ISR", "isr":
		c = color.New(color.FgYellow).Add(color.BgHiBlue)
	case "SIG", "sig":
		c = color.New(color.FgYellow).Add(color.Bold).Add(color.BgGreen)
	case "TST", "tst":
		c = color.New(color.FgYellow).Add(color.BgBlack)
	default:
		c = color.New(color.FgWhite).Add(color.BgBlack)
		err = errors.New("unknown channel info")
	}
	return c, err
}

func colorSetAlternate(channel string) (*color.Color, error) {
	var err error
	var c *color.Color
	switch channel {
	case "ERR", "err":
		c = color.New(color.FgRed).Add(color.Bold).Add(color.BgYellow)
	case "WRN", "wrn":
		c = color.New(color.FgBlack).Add(color.BgRed)
	case "MSG", "msg":
		c = color.New(color.FgBlack).Add(color.BgGreen)
	case "RD_", "rd_":
		c = color.New(color.FgBlack).Add(color.BgMagenta)
	case "WR_", "wr_":
		c = color.New(color.FgMagenta).Add(color.BgBlack)
	case "TIM", "tim":
		c = color.New(color.FgYellow).Add(color.BgBlue)
	case "ATT", "att":
		c = color.New(color.FgCyan).Add(color.Bold).Add(color.BgYellow)
	case "DBG", "dbg":
		c = color.New(color.FgRed).Add(color.BgCyan)
	case "DIA", "dia":
		c = color.New(color.BgHiBlack).Add(color.BgHiCyan)
	case "ISR", "isr":
		c = color.New(color.FgBlack).Add(color.BgYellow)
	case "SIG", "sig":
		c = color.New(color.FgGreen).Add(color.Bold).Add(color.BgYellow)
	case "TST", "tst":
		c = color.New(color.FgRed).Add(color.BgGreen)
	default:
		c = color.New(color.FgWhite).Add(color.BgBlack)
		err = errors.New("unknown channel info")
	}
	return c, err
}

// check for color match and remove color info
// expects s starting with "col:" or "COL:" and returns color and (modified) s
// If no match occuured it returns no color and unchanged s
// If upper case match it returns *color.Color and unchanged s
// If lower case match it returns *color.Color and s without starting pattern "col:"
// col options are: err, wrn, msg, ...
// COL options are: ERR, WRN, MSG, ...
func colorChannel(s string) (*color.Color, string) {
	c := noColor
	var err error
	sc := strings.SplitN(s, ":", 2)
	if 2 != len(sc) {
		return c, s
	}
	var r string
	if isLower(sc[0]) {
		r = sc[1] // remove channel info
	} else {
		r = s // keep channel info
	}
	switch ColorPalette {
	case "off", "none":
		color.NoColor = true // disables colorized output
	case "default":
		color.NoColor = false // to force color after some errors
		c, err = colorSetDefault(sc[0])
		if nil != err {
			r = s // keep channel info
		}
	case "alternate":
		color.NoColor = false // to force color after some errors
		c, err = colorSetAlternate(sc[0])
		if nil != err {
			r = s // keep channel info
		}
	default:
		fmt.Println("ignoring unknown color palette", ColorPalette)
	}
	return c, r
}
