// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package emitter

// TODO: Now the color is resettet after each string. This is needed only after the last string in a line.

import (
	"strings"
	"unicode"

	"github.com/mgutz/ansi"
)

// lineTranslatorANSI imnplements a lineWriter interface.
// It uses an internal lineWriter lw to write to.
// It converts the channel information to color data using colorPalette.
// In case of a remote display the lineTranslator should be used there.
type lineTranslatorANSI struct {
	lw           lineWriter
	colorPalette string
}

// newLineTranslatorANSI translates lines to ANSI colors according to colorPalette.
// It provides a lineWriter interface and uses internally a lineWriter.
func newLineTranslatorANSI(lw lineWriter, colorPalette string) *lineTranslatorANSI {
	p := &lineTranslatorANSI{lw, colorPalette}
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
func (p *lineTranslatorANSI) colorize(s string) string {
	if "off" == p.colorPalette { // do nothing
		return s
	}
	sc := strings.SplitN(s, ":", 2)
	if len(sc) < 2 { // no color separator
		return s
	}
	if "none" == p.colorPalette { // remove channel info
		if isLower(s) {
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
		}
		return s
	}

	switch sc[0] {
	case "e", "err", "error":
		s = sc[1] // remove channel info
		fallthrough
	case "E", "ERR", "ERROR":
		return colorizeERROR(s)
	case "w", "wrn", "warning":
		s = sc[1] // remove channel info
		fallthrough
	case "W", "WRN", "WARNING":
		return colorizeWARNING(s)
	case "m", "msg", "message":
		s = sc[1] // remove channel info
		fallthrough
	case "M", "MSG", "MESSAGE":
		return colorizeMESSAGE(s)
	case "rd", "rd_":
		s = sc[1] // remove channel info
		fallthrough
	case "RD", "RD_":
		return colorizeREAD(s)
	case "wr", "wr_":
		s = sc[1] // remove channel info
		fallthrough
	case "WR", "WR_":
		return colorizeWRITE(s)
	case "tim", "time":
		s = sc[1] // remove channel info
		fallthrough
	case "TIM", "TIME":
		return colorizeTIME(s)
	case "att", "attention":
		s = sc[1] // remove channel info
		fallthrough
	case "ATT", "ATTENTION":
		return colorizeATTENTION(s)
	case "d", "db", "dbg", "debug":
		s = sc[1] // remove channel info
		fallthrough
	case "D", "DB", "DBG", "DEBUG":
		return colorizeDEBUG(s)
	case "dia", "diag":
		s = sc[1] // remove channel info
		fallthrough
	case "DIA", "DIAG":
		return colorizeDIAG(s)
	case "isr", "interrupt":
		s = sc[1] // remove channel info
		fallthrough
	case "ISR", "INTERRUPT":
		return colorizeDIAG(s)
	case "s", "sig", "signal":
		s = sc[1] // remove channel info
		fallthrough
	case "S", "SIG", "SIGNAL":
		return colorizeSIGNAL(s)
	case "t", "tst", "test":
		s = sc[1] // remove channel info
		fallthrough
	case "T", "TST", "TEST":
		return colorizeTEST(s)
	case "i", "inf", "info", "informal":
		s = sc[1] // remove channel info
		fallthrough
	case "I", "INF", "INFO", "INFORMAL":
		return colorizeINFO(s)
	}
	return s
}

// writeLine consumes a full line, translates it and writes it to the internal lineWriter.
// It adds ANSI color Codes and replaces col: channel information.
// It treats each sub string separately and and a color reset code at the end.
func (p *lineTranslatorANSI) writeLine(line []string) {
	l := make([]string, 0, 10)
	for _, s := range line {
		l = append(l, p.colorize(s))
	}
	if "default" == p.colorPalette {
		l = append(l, ansi.Reset)
	}
	p.lw.writeLine(l)
}
