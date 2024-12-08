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

func ShowAllColors() {
	var i int
	fgStyles := []string{"", "+b", "+B", "+u", "+i", "+s", "+h"}
	bgStyles := []string{"", "+h"}
	colors := []string{"black", "red", "green", "yellow", "blue", "magenta", "cyan", "white", "default"}
	for _, b := range colors {
		for _, bgStyle := range bgStyles {
			bg := b + bgStyle
			for _, f := range colors {
				for _, fgStyle := range fgStyles {
					fg := f + fgStyle
					colorCode := fmt.Sprintf("%s:%s", fg, bg)
					colorize := ansi.ColorFunc(colorCode)
					colorized := colorize(colorCode)
					fmt.Printf("%4d:%24s:%s\n", i, colorCode, colorized)
					i++
				}
			}
		}
	}
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
	colorizeFATAL     = ansi.ColorFunc("magenta+b:red")
	colorizeCRITICAL  = ansi.ColorFunc("red+i:default+h")
	colorizeEMERGENCY = ansi.ColorFunc("red+i:blue")
	colorizeERROR     = ansi.ColorFunc("11:red")
	colorizeWARNING   = ansi.ColorFunc("11+i:red")
	colorizeATTENTION = ansi.ColorFunc("11:green")
	colorizeINFO      = ansi.ColorFunc("cyan+b:default+h")
	colorizeDEBUG     = ansi.ColorFunc("130+i")
	colorizeTRACE     = ansi.ColorFunc("default+i:default+h")

	// user mode colors
	colorizeTIME      = ansi.ColorFunc("blue+i:yellow+h")
	colorizeMESSAGE   = ansi.ColorFunc("green+h:black")
	colorizeREAD      = ansi.ColorFunc("black+i:yellow+h")
	colorizeWRITE     = ansi.ColorFunc("black+u:yellow+h")
	colorizeRECEIVE   = ansi.ColorFunc("black+h:black")
	colorizeTRANSMIT  = ansi.ColorFunc("black:black+h")
	colorizeDIAG      = ansi.ColorFunc("yellow+i:default+h")
	colorizeINTERRUPT = ansi.ColorFunc("magenta+i:default+h")
	colorizeSIGNAL    = ansi.ColorFunc("118+i")
	colorizeTEST      = ansi.ColorFunc("yellow+h:black")

	colorizeDEFAULT = ansi.ColorFunc("off")
	colorizeNOTICE  = ansi.ColorFunc("blue:white+h")
	colorizeALERT   = ansi.ColorFunc("magenta:magenta+h")
	colorizeASSERT  = ansi.ColorFunc("yellow+i:blue")
	colorizeALARM   = ansi.ColorFunc("red+i:white+h")
	colorizeCYCLE   = ansi.ColorFunc("magenta+i:yellow+h")
	colorizeVERBOSE = ansi.ColorFunc("blue:default")

	AllStatistics bool // Keep the complete statistics when Trice is closed.
	TagStatistics bool // Print the occured count for each Trice log when Trice is closed.
)

func isLower(s string) bool {
	for _, r := range s {
		if !unicode.IsLower(r) && unicode.IsLetter(r) {
			return false
		}
	}
	return true
}

type tag struct {
	count    int                 // count counts each occurance of the trice tag.
	Names    []string            // name contains all usable names fo a specific tag.
	colorize func(string) string // colorize is the function called for each tag.
}

// Tags contains all usable trice Tags and their possible names.
var Tags = []tag{
	// log level
	{0, []string{"Fatal", "fatal", "FATAL"}, colorizeFATAL},
	{0, []string{"crit", "Critical", "critical", "CRITICAL", "Crit", "CRIT"}, colorizeCRITICAL},
	{0, []string{"em", "Emergency", "emergency", "EMERGENCY"}, colorizeEMERGENCY},
	{0, []string{"e", "Error", "err", "error", "E", "ERR", "ERROR"}, colorizeERROR},
	{0, []string{"w", "wrn", "Warning", "warning", "W", "WRN", "WARNING", "Warn", "warn", "WARN"}, colorizeWARNING},
	{0, []string{"att", "attention", "Attention", "ATT", "ATTENTION"}, colorizeATTENTION},
	{0, []string{"i", "inf", "info", "Info", "informal", "I", "INF", "INFO", "INFORMAL"}, colorizeINFO},
	{0, []string{"d", "db", "Debug", "dbg", "deb", "debug", "D", "DB", "DBG", "DEBUG"}, colorizeDEBUG},
	{0, []string{"tr", "Trace", "trace", "TRACE"}, colorizeTRACE},

	// user modes
	{0, []string{"tim", "time", "TIM", "TIME", "TIMESTAMP", "timestamp", "Timestamp"}, colorizeTIME},
	{0, []string{"m", "msg", "message", "M", "MSG", "MESSAGE", "OK"}, colorizeMESSAGE},
	{0, []string{"r", "rx", "rd", "read", "rd_", "RD", "RD_", "READ"}, colorizeREAD},
	{0, []string{"w", "tx", "wr", "write", "wr_", "WR", "WR_", "WRITE"}, colorizeWRITE},
	{0, []string{"rx", "receive", "RECEIVE", "Receive", "RX"}, colorizeRECEIVE},
	{0, []string{"tx", "transmit", "TRANSMIT", "Transmit", "TX"}, colorizeTRANSMIT},
	{0, []string{"dia", "diag", "Diag", "DIA", "DIAG"}, colorizeDIAG},
	{0, []string{"int", "isr", "ISR", "INT", "interrupt", "Interrupt", "INTERRUPT"}, colorizeINTERRUPT},
	{0, []string{"s", "sig", "signal", "S", "SIG", "SIGNAL"}, colorizeSIGNAL},
	{0, []string{"t", "tst", "test", "T", "TST", "TEST"}, colorizeTEST},

	{0, []string{"def", "Default", "DEFAULT", "default"}, colorizeDEFAULT},
	{0, []string{"note", "Notice", "NOTICE", "notice", "Note", "NOTE"}, colorizeNOTICE},
	{0, []string{"Alert", "alert", "ALERT"}, colorizeALERT},
	{0, []string{"Assert", "assert", "ASSERT"}, colorizeASSERT},
	{0, []string{"a", "Alarm", "alarm", "ALARM"}, colorizeALARM},
	{0, []string{"CYCLE_ERROR"}, colorizeCYCLE}, // not for user code!
	{0, []string{"v", "Verbose", "verbose", "VERBOSE"}, colorizeVERBOSE},
}

func FindTagName(name string) (tagName string, err error) {
	for _, t := range Tags {
		for _, tn := range t.Names {
			if tn == name {
				tagName = t.Names[0] // take the first tag name as reference.
				return
			}
		}
	}
	return "", fmt.Errorf("no tagName found for name %s", name)
}

// TagEvents returns count of occurred channel events.
// If ch is unknown, the returned value is -1.
func TagEvents(ch string) int {
	for _, s := range Tags {
		for _, c := range s.Names {
			if c == ch {
				return s.count
			}
		}
	}
	return -1
}

// PrintTagStatistics shows the amount of occurred tag events.
func PrintTagStatistics(w io.Writer) {
	if !TagStatistics && !AllStatistics {
		return
	}
	fmt.Fprintf(w, "\nTag Statistics:\n\n")
	for _, s := range Tags {
		if s.count != 0 {
			fmt.Fprintf(w, "%6d times: ", s.count)
			for _, c := range s.Names {
				if ColorPalette != "off" && ColorPalette != "none" {
					c = s.colorize(c)
				}
				fmt.Fprint(w, c, " ")
			}
			fmt.Fprintln(w, "")
		}
	}
}

// tagVariants returns all variants of ch as string slice.
// If ch is not inside ansiSel nil is returned.
func tagVariants(ch string) []string {
	for _, s := range Tags {
		for _, c := range s.Names {
			if c == ch {
				return s.Names
			}
		}
	}
	return nil
}

// isTag returns true if tag is any tag variant string.
func isTag(tag string) bool {
	cv := tagVariants(tag)
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
func (p *lineTransformerANSI) colorize(s string) (r string, show bool) {
	if LogLevel == "off" {
		return // do not log at all, return empty string
	}
	var logLev int       // numeric log level
	var logThreshold int // numeric log threshold
	r = s
	sc := strings.SplitN(s, ":", 2)
	if len(sc) < 2 { // no color separator (no log level)
		return r, true // do nothing, return unchanged string
	}
	for i, cc := range Tags {
		for _, c := range cc.Names {
			if c == sc[0] {
				Tags[i].count++ // count event
				logLev = i
			}
			if c == LogLevel {
				logThreshold = i
			}
		}
	}

	if LogLevel != "all" && logLev > logThreshold {
		r = "" // suppress unwanted logs
		return r, false
	}

	if p.colorPalette == "off" {
		return r, true // do nothing (despite event counting)
	}
	if isTag(sc[0]) && isLower(sc[0]) {
		r = sc[1] // remove channel info
	}
	if p.colorPalette == "none" {
		return r, true
	}
	for _, cs := range Tags {
		for _, c := range cs.Names {
			if c == sc[0] {
				return cs.colorize(r), true
			}
		}
	}
	return r, true
}

// WriteLine consumes a full line, translates it and writes it to the internal Linewriter.
// It adds ANSI color Codes and replaces col: channel information.
// It treats each sub string separately and a color reset code at the end.
func (p *lineTransformerANSI) WriteLine(line []string) {
	var colored bool
	showLine := true
	l := make([]string, 0, 10)
	for i, s := range line {
		cs, show := p.colorize(s)
		// The relevant channel information is probably in the last string in the line slice before the suffix.
		// If we have `Trice( "msg:Hello");` and `Trice( att:World\n");` and `-logLevel att`, then
		// the channel "att" is relevant because it contains the newline and "msg:Hello" is shown too.
		// But a `Trice("msg:Hi!\n");` would be suppressed.
		// For the applied CLI switch "-addNL" this needs more finetuning.
		if !show && i == len(line)-2 {
			showLine = false
		}
		l = append(l, cs)
		if cs != s {
			colored = true
		}
	}
	if (p.colorPalette == "default" || p.colorPalette == "color") && 1 < len(l) && colored {
		l = append(l, ansi.Reset)
	}
	if showLine { // suppress empty lines when logLevel == "off"
		p.lw.WriteLine(l)
	}
}
