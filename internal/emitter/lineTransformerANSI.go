// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package emitter

// TODO: Now the color is reset after each string. This is needed only after the last string in a line.

import (
	"fmt"
	"io"
	"strings"
	"unicode"

	"github.com/mgutz/ansi"
)

// lineTransformerANSI implements a Linewriter interface.
// It uses an internal Linewriter lw to write to.
// It converts the channel information to color data using colorPalette.
// In case of a remote display the lineTranslator should be used there.
type lineTransformerANSI struct {
	lw           LineWriter
	colorPalette string
}

// newLineTransformerANSI translates lines to ANSI colors according to colorPalette.
// It provides a Linewriter interface and uses internally a Linewriter.
func newLineTransformerANSI(lw LineWriter, colorPalette string) *lineTransformerANSI {
	p := &lineTransformerANSI{lw, colorPalette}
	return p
}

var (
	// LogLevel is usable to suppress less important logs.
	LogLevel = "all"

	// log level colors
	colorizeFATAL     = ansi.ColorFunc("121+i")
	colorizeCRITICAL  = ansi.ColorFunc("121+i")
	colorizeEMERGENCY = ansi.ColorFunc("121+i")
	colorizeERROR     = ansi.ColorFunc("11:red")
	colorizeWARNING   = ansi.ColorFunc("11+i:red")
	colorizeATTENTION = ansi.ColorFunc("11:green")
	colorizeINFO      = ansi.ColorFunc("121+i")
	colorizeDEBUG     = ansi.ColorFunc("130+i")
	colorizeTRACE     = ansi.ColorFunc("121+i")

	// user mode colors
	colorizeTIME      = ansi.ColorFunc("108:blue")
	colorizeMESSAGE   = ansi.ColorFunc("green+h:black")
	colorizeREAD      = ansi.ColorFunc("off") //  ansi.ColorFunc("101:black")
	colorizeWRITE     = ansi.ColorFunc("off") //  ansi.ColorFunc("101+i:black")
	colorizeDIAG      = ansi.ColorFunc("161+B")
	colorizeINTERRUPT = ansi.ColorFunc("13+i")
	colorizeSIGNAL    = ansi.ColorFunc("118+i")
	colorizeTEST      = ansi.ColorFunc("yellow+h:black")

	//  colorizeDEFAULT   = ansi.ColorFunc("121+i")
	//  colorizeNOTICE    = ansi.ColorFunc("121+i")
	//  colorizeALERT     = ansi.ColorFunc("121+i")
	//  colorizeASSERT    = ansi.ColorFunc("121+i")
	//  colorizeALARM     = ansi.ColorFunc("121+i")
	//  colorizeCYCLE     = ansi.ColorFunc("11:red")
	//  colorizeVERBOSE   = ansi.ColorFunc("121+i")
)

func isLower(s string) bool {
	for _, r := range s {
		if !unicode.IsLower(r) && unicode.IsLetter(r) {
			return false
		}
	}
	return true
}

type colorChannel struct {
	events   int
	channel  []string
	colorize func(string) string
}

var colorChannels = []colorChannel{
	// log level
	{0, []string{"Fatal", "fatal", "FATAL"}, colorizeFATAL},
	{0, []string{"Critical", "critical", "CRITICAL", "crit", "Crit", "CRIT"}, colorizeCRITICAL},
	{0, []string{"Emergency", "emergency", "EMERGENCY"}, colorizeEMERGENCY},
	{0, []string{"Error", "e", "err", "error", "E", "ERR", "ERROR"}, colorizeERROR},
	{0, []string{"Warning", "w", "wrn", "warning", "W", "WRN", "WARNING", "Warn", "warn", "WARN"}, colorizeWARNING},
	{0, []string{"att", "attention", "Attention", "ATT", "ATTENTION"}, colorizeATTENTION},
	{0, []string{"Info", "i", "inf", "info", "informal", "I", "INF", "INFO", "INFORMAL"}, colorizeINFO},
	{0, []string{"Debug", "d", "db", "dbg", "deb", "debug", "D", "DB", "DBG", "DEBUG"}, colorizeDEBUG},
	{0, []string{"Trace", "trace", "TRACE"}, colorizeTRACE},

	// user modes
	{0, []string{"Timestamp", "tim", "time", "TIM", "TIME", "TIMESTAMP", "timestamp"}, colorizeTIME},
	{0, []string{"m", "msg", "message", "M", "MSG", "MESSAGE"}, colorizeMESSAGE},
	{0, []string{"rd", "rd_", "RD", "RD_"}, colorizeREAD},
	{0, []string{"wr", "wr_", "WR", "WR_"}, colorizeWRITE},
	{0, []string{"dia", "diag", "Diag", "DIA", "DIAG"}, colorizeDIAG},
	{0, []string{"int", "isr", "ISR", "INT", "interrupt", "Interrupt", "INTERRUPT"}, colorizeINTERRUPT},
	{0, []string{"s", "sig", "signal", "S", "SIG", "SIGNAL"}, colorizeSIGNAL},
	{0, []string{"t", "tst", "test", "T", "TST", "TEST"}, colorizeTEST},

	//  {0, []string{"Default", "DEFAULT", "default"}, colorizeDEFAULT},
	//  {0, []string{"Notice", "NOTICE", "notice", "Note", "note", "NOTE"}, colorizeNOTICE},
	//  {0, []string{"Alert", "alert", "ALERT"}, colorizeALERT},
	//  {0, []string{"Assert", "assert", "ASSERT"}, colorizeASSERT},
	//  {0, []string{"Alarm", "alarm", "ALARM"}, colorizeALARM},
	//  {0, []string{"cycle", "CYCLE"}, colorizeCYCLE},
	//  {0, []string{"Verbose", "verbose", "VERBOSE"}, colorizeVERBOSE},
}

// ColorChannelEvents returns count of occurred channel events.
// If ch is unknown, the returned value is -1.
func ColorChannelEvents(ch string) int {
	for _, s := range colorChannels {
		for _, c := range s.channel {
			if c == ch {
				return s.events
			}
		}
	}
	return -1
}

// PrintColorChannelEvents shows the amount of occurred channel events.
func PrintColorChannelEvents(w io.Writer) {
	for _, s := range colorChannels {
		if s.events != 0 {
			fmt.Fprintf(w, "%6d times: ", s.events)
			for _, c := range s.channel {
				fmt.Fprint(w, c, " ")
			}
			fmt.Fprintln(w, "")
		}
	}
}

// channelVariants returns all variants of ch as string slice.
// If ch is not inside ansiSel nil is returned.
func channelVariants(ch string) []string {
	for _, s := range colorChannels {
		for _, c := range s.channel {
			if c == ch {
				return s.channel
			}
		}
	}
	return nil
}

// isChannel returns true if ch is any ansiSel string.
func isChannel(ch string) bool {
	cv := channelVariants(ch)
	return cv != nil
}

// colorize prefixes s with an ansi color code according to these conditions:
// If p.colorPalette is "off", do nothing.
// If p.colorPalette is "none" remove only lower case channel info "col:"
// If "COL:" is start of string add ANSI color code according to COL:
// If "col:" is start of string replace "col:" with ANSI color code according to col:
// Additionally, if global variable LogLevel is not the default "all", but found inside
// ColorChannels, logs with higher index positions are suppressed.
// As special case LogLevel == "off" does not output anything.
func (p *lineTransformerANSI) colorize(s string) (r string) {
	if LogLevel == "off" {
		return // do not log at all
	}
	var logLev int       // numeric log level
	var logThreshold int // numeric log threshold
	r = s
	sc := strings.SplitN(s, ":", 2)
	if len(sc) < 2 { // no color separator (no log level)
		return // do nothing, return unchanged string
	}
	for i, cc := range colorChannels {
		for _, c := range cc.channel {
			if c == sc[0] {
				colorChannels[i].events++ // count event
				logLev = i
			}
			if c == LogLevel {
				logThreshold = i
			}
		}
	}

	if LogLevel != "all" && logLev > logThreshold {
		r = "" // suppress unwanted logs
		return
	}

	if p.colorPalette == "off" {
		return // do nothing (despite event counting)
	}
	if isChannel(sc[0]) && isLower(sc[0]) {
		r = sc[1] // remove channel info
	}
	if p.colorPalette == "none" {
		return
	}
	for _, cs := range colorChannels {
		for _, c := range cs.channel {
			if c == sc[0] {
				return cs.colorize(r)
			}
		}
	}
	return
}

// WriteLine consumes a full line, translates it and writes it to the internal Linewriter.
// It adds ANSI color Codes and replaces col: channel information.
// It treats each sub string separately and a color reset code at the end.
func (p *lineTransformerANSI) WriteLine(line []string) {
	var colored bool
	l := make([]string, 0, 10)
	for _, s := range line {
		cs := p.colorize(s)
		l = append(l, cs)
		if cs != s {
			colored = true
		}
	}
	if (p.colorPalette == "default" || p.colorPalette == "color") && 1 < len(l) && colored {
		l = append(l, ansi.Reset)
	}
	p.lw.WriteLine(l)
}
