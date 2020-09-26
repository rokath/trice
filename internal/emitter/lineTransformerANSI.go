// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package emitter

// TODO: Now the color is resettet after each string. This is needed only after the last string in a line.

import (
	"strings"
	"unicode"

	"github.com/mgutz/ansi"
)

// LineTransformerANSI imnplements a Linewriter interface.
// It uses an internal Linewriter lw to write to.
// It converts the channel information to color data using colorPalette.
// In case of a remote display the lineTranslator should be used there.
type LineTransformerANSI struct {
	lw           Linewriter
	colorPalette string
}

// NewLineTransformerANSI translates lines to ANSI colors according to colorPalette.
// It provides a Linewriter interface and uses internally a Linewriter.
func NewLineTransformerANSI(lw Linewriter, colorPalette string) *LineTransformerANSI {
	p := &LineTransformerANSI{lw, colorPalette}
	return p
}

var (
	colorizeERROR     = ansi.ColorFunc("11:red")
	colorizeWARNING   = ansi.ColorFunc("11+i:red")
	colorizeMESSAGE   = ansi.ColorFunc("green+h:black")
	colorizeREAD      = ansi.ColorFunc("101:black")
	colorizeWRITE     = ansi.ColorFunc("101+i:black")
	colorizeTIME      = ansi.ColorFunc("108:blue")
	colorizeATTENTION = ansi.ColorFunc("11:green")
	colorizeDEBUG     = ansi.ColorFunc("130+i")
	colorizeDIAG      = ansi.ColorFunc("161+B")
	colorizeINTERRUPT = ansi.ColorFunc("13+i")
	colorizeSIGNAL    = ansi.ColorFunc("118+i")
	colorizeTEST      = ansi.ColorFunc("yellow+h:black")
	colorizeINFO      = ansi.ColorFunc("121+i")
)

func isLower(s string) bool {
	for _, r := range s {
		if !unicode.IsLower(r) && unicode.IsLetter(r) {
			return false
		}
	}
	return true
}

// colorize prefixes s with an ansi color code according to this conditions:
// If p.colorPalette is "off", do nothing.
// If p.colorPalette is "none" remove only lower case channel info "col:"
// If "COL:" is begin of string add ANSI color code according to COL:
// If "col:" is begin of string replace "col:" with ANSI color code according to col:
func (p *LineTransformerANSI) colorize(s string) (r string) {
	r = s
	if "off" == p.colorPalette { // do nothing
		return
	}
	sc := strings.SplitN(s, ":", 2)
	if len(sc) < 2 { // no color separator
		return
	}
	if "none" == p.colorPalette { // remove channel info
		if isLower(sc[0]) {
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
				r = sc[1] // remove channel info
			}
		}
		return
	}

	switch sc[0] {
	case "e", "err", "error":
		r = sc[1] // remove channel info
		fallthrough
	case "E", "ERR", "ERROR":
		return colorizeERROR(r)
	case "w", "wrn", "warning":
		r = sc[1] // remove channel info
		fallthrough
	case "W", "WRN", "WARNING":
		return colorizeWARNING(r)
	case "m", "msg", "message":
		r = sc[1] // remove channel info
		fallthrough
	case "M", "MSG", "MESSAGE":
		return colorizeMESSAGE(r)
	case "rd", "rd_":
		r = sc[1] // remove channel info
		fallthrough
	case "RD", "RD_":
		return colorizeREAD(r)
	case "wr", "wr_":
		r = sc[1] // remove channel info
		fallthrough
	case "WR", "WR_":
		return colorizeWRITE(r)
	case "tim", "time":
		r = sc[1] // remove channel info
		fallthrough
	case "TIM", "TIME":
		return colorizeTIME(r)
	case "att", "attention":
		r = sc[1] // remove channel info
		fallthrough
	case "ATT", "ATTENTION":
		return colorizeATTENTION(r)
	case "d", "db", "dbg", "debug":
		r = sc[1] // remove channel info
		fallthrough
	case "D", "DB", "DBG", "DEBUG":
		return colorizeDEBUG(r)
	case "dia", "diag":
		r = sc[1] // remove channel info
		fallthrough
	case "DIA", "DIAG":
		return colorizeDIAG(r)
	case "isr", "interrupt":
		r = sc[1] // remove channel info
		fallthrough
	case "ISR", "INTERRUPT":
		return colorizeDIAG(r)
	case "s", "sig", "signal":
		r = sc[1] // remove channel info
		fallthrough
	case "S", "SIG", "SIGNAL":
		return colorizeSIGNAL(r)
	case "t", "tst", "test":
		r = sc[1] // remove channel info
		fallthrough
	case "T", "TST", "TEST":
		return colorizeTEST(r)
	case "i", "inf", "info", "informal":
		r = sc[1] // remove channel info
		fallthrough
	case "I", "INF", "INFO", "INFORMAL":
		return colorizeINFO(s)
	}
	return
}

// writeLine consumes a full line, translates it and writes it to the internal Linewriter.
// It adds ANSI color Codes and replaces col: channel information.
// It treats each sub string separately and and a color reset code at the end.
func (p *LineTransformerANSI) writeLine(line []string) {
	var colored bool
	l := make([]string, 0, 10)
	for _, s := range line {
		cs := p.colorize(s)
		l = append(l, cs)
		if cs != s {
			colored = true
		}
	}
	if "default" == p.colorPalette && 1 < len(l) && colored {
		l = append(l, ansi.Reset)
	}
	p.lw.writeLine(l)
}
