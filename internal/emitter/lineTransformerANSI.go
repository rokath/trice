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
	lw           LineWriter
	colorPalette string
}

// NewLineTransformerANSI translates lines to ANSI colors according to colorPalette.
// It provides a Linewriter interface and uses internally a Linewriter.
func NewLineTransformerANSI(lw LineWriter, colorPalette string) *LineTransformerANSI {
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

type ansiSelector struct {
	channel  []string
	colorize func(string) string
}

var ansiSel = []ansiSelector{
	{[]string{"e", "err", "error", "E", "ERR", "ERROR"}, colorizeERROR},
	{[]string{"w", "wrn", "warning", "W", "WRN", "WARNING"}, colorizeWARNING},
	{[]string{"m", "msg", "message", "M", "MSG", "MESSAGE"}, colorizeMESSAGE},
	{[]string{"rd", "rd_", "RD", "RD_"}, colorizeREAD},
	{[]string{"wr", "wr_", "WR", "WR_"}, colorizeWRITE},
	{[]string{"tim", "time", "TIM", "TIME"}, colorizeTIME},
	{[]string{"att", "attention", "ATT", "ATTENTION"}, colorizeATTENTION},
	{[]string{"d", "db", "dbg", "debug", "D", "DB", "DBG", "DEBUG"}, colorizeDEBUG},
	{[]string{"dia", "diag", "DIA", "DIAG"}, colorizeDIAG},
	{[]string{"int", "isr", "ISR", "INT", "interrupt", "INTERRUPT"}, colorizeINTERRUPT},
	{[]string{"s", "sig", "signal", "S", "SIG", "SIGNAL"}, colorizeSIGNAL},
	{[]string{"t", "tst", "test", "T", "TST", "TEST"}, colorizeTEST},
	{[]string{"i", "inf", "info", "informal", "I", "INF", "INFO", "INFORMAL"}, colorizeINFO},
}

func isChannel(ch string) bool {
	for _, s := range ansiSel {
		for _, c := range s.channel {
			if c == ch {
				return true
			}
		}
	}
	return false
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
	if isChannel(sc[0]) && isLower(sc[0]) {
		r = sc[1] // remove channel info
	}
	if "none" == p.colorPalette {
		return
	}
	for _, s := range ansiSel {
		for _, c := range s.channel {
			if c == sc[0] {
				return s.colorize(r)
			}
		}
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
	if ("default" == p.colorPalette || "color" == p.colorPalette) && 1 < len(l) && colored {
		l = append(l, ansi.Reset)
	}
	p.lw.writeLine(l)
}
