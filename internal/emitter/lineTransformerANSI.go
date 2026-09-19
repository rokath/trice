// SPDX-License-Identifier: MIT

package emitter

// TODO: Now the color is reset after each string. This is needed only after the last string in a line.

import (
	"fmt"
	"io"
	"strings"
	"unicode"

	"github.com/mgutz/ansi"
)

var userLabelsAdded bool

// AddUserLabels appends user-defined labels to Tags once per process.
func AddUserLabels() {
	if userLabelsAdded {
		return
	}
	userLabelsAdded = true
	for _, v := range UserLabel {
		var t tag
		t.Names = append(t.Names, v) // User labels get only one name.
		t.colorize = colorizeUSER
		Tags = append(Tags, t)
	}
}

// lineTransformerANSI implements a Linewriter interface.
// It uses an internal Linewriter lw to write to.
// It converts the channel information to color data using colorPalette.
// In case of a remote display the lineTranslator should be used there.
type lineTransformerANSI struct {
	lw           LineWriter
	colorPalette string
}

// ShowAllColors prints all foreground/background style combinations.
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
	colorizeFATAL     = ansi.ColorFunc("magenta+b:red")       // FATAL
	colorizeCRITICAL  = ansi.ColorFunc("red+i:default+h")     //
	colorizeEMERGENCY = ansi.ColorFunc("red+i:blue")          //
	colorizeERROR     = ansi.ColorFunc("11:red")              // ERROR
	colorizeWARNING   = ansi.ColorFunc("11+i:red")            // WARN
	colorizeATTENTION = ansi.ColorFunc("11:green")            //
	colorizeINFO      = ansi.ColorFunc("cyan+b:default+h")    // INFO
	colorizeDEBUG     = ansi.ColorFunc("130+i")               // DEBUG
	colorizeTRACE     = ansi.ColorFunc("default+i:default+h") //

	// user mode colors
	colorizeTIME      = ansi.ColorFunc("blue+i:yellow+h")     //
	colorizeMESSAGE   = ansi.ColorFunc("green+h:black")       //
	colorizeREAD      = ansi.ColorFunc("black+i:yellow+h")    //
	colorizeWRITE     = ansi.ColorFunc("black+u:yellow+h")    //
	colorizeRECEIVE   = ansi.ColorFunc("black+h:black")       //
	colorizeTRANSMIT  = ansi.ColorFunc("black:black+h")       //
	colorizeDIAG      = ansi.ColorFunc("yellow+i:default+h")  //
	colorizeINTERRUPT = ansi.ColorFunc("magenta+i:default+h") //
	colorizeSIGNAL    = ansi.ColorFunc("118+i")               //
	colorizeTEST      = ansi.ColorFunc("yellow+h:black")      //

	colorizeDEFAULT = ansi.ColorFunc("off")                //
	colorizeNOTICE  = ansi.ColorFunc("blue:white+h")       //
	colorizeALERT   = ansi.ColorFunc("magenta:magenta+h")  //
	colorizeASSERT  = ansi.ColorFunc("yellow+i:blue")      //
	colorizeALARM   = ansi.ColorFunc("red+i:white+h")      //
	colorizeCYCLE   = ansi.ColorFunc("magenta+i:yellow+h") //
	colorizeVERBOSE = ansi.ColorFunc("blue:default")       // VERB
	colorizeUSER    = ansi.ColorFunc("off")                //

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
	Names    []string            // Names contains all aliases for one tag.
	colorize func(string) string // colorize is the function called for each tag.
}

// Tags contains all usable trice Tags and their possible names.
//
// The optional target-side local-log hints in src/triceLogAnsi.c deliberately
// duplicate a subset of this host presentation policy without introducing a
// generated file or a Go dependency into the C library. Neither list is
// authoritative for the other. Maintainers may manually synchronize aliases
// and palette colors when matching local and host presentation is desired; the
// C file contains the reciprocal maintenance note.
var Tags = []tag{
	// log level
	{0, []string{"FATAL", "Fatal", "fatal"}, colorizeFATAL},
	{0, []string{"CRITICAL", "crit", "Critical", "critical", "Crit", "CRIT"}, colorizeCRITICAL},
	{0, []string{"EMERGENCY", "em", "Emergency", "emergency"}, colorizeEMERGENCY},
	{0, []string{"ERROR", "e", "Error", "err", "error", "E", "ERR"}, colorizeERROR},
	{0, []string{"WARNING", "wrn", "Warning", "warning", "WRN", "Warn", "warn", "WARN"}, colorizeWARNING},
	{0, []string{"ATTENTION", "att", "attention", "Attention", "ATT"}, colorizeATTENTION},
	{0, []string{"INFO", "i", "inf", "info", "Info", "informal", "I", "INF", "INFORMAL"}, colorizeINFO},
	{0, []string{"DEBUG", "d", "db", "Debug", "dbg", "deb", "debug", "D", "DB", "DBG"}, colorizeDEBUG},
	{0, []string{"TRACE", "tr", "Trace", "trace"}, colorizeTRACE},

	// user modes
	{0, []string{"TIME", "tim", "time", "Time", "TIM", "TIMESTAMP", "timestamp", "Timestamp"}, colorizeTIME},
	{0, []string{"MESSAGE", "m", "msg", "message", "M", "MSG", "OK"}, colorizeMESSAGE},
	{0, []string{"READ", "r", "rd", "read", "rd_", "RD", "RD_"}, colorizeREAD},
	{0, []string{"WRITE", "w", "wr", "write", "wr_", "W", "WR", "WR_"}, colorizeWRITE},
	{0, []string{"RECEIVE", "rx", "receive", "Receive", "RX"}, colorizeRECEIVE},
	{0, []string{"TRANSMIT", "tx", "transmit", "Transmit", "TX"}, colorizeTRANSMIT},
	{0, []string{"DIAG", "dia", "diag", "Diag", "DIA"}, colorizeDIAG},
	{0, []string{"INTERRUPT", "int", "isr", "ISR", "INT", "interrupt", "Interrupt"}, colorizeINTERRUPT},
	{0, []string{"SIGNAL", "sig", "signal", "SIG"}, colorizeSIGNAL},
	{0, []string{"TEST", "t", "tst", "test", "T", "TST"}, colorizeTEST},

	{0, []string{"DEFAULT", "def", "Default", "default"}, colorizeDEFAULT},
	{0, []string{"NOTICE", "note", "Notice", "notice", "Note", "NOTE"}, colorizeNOTICE},
	{0, []string{"ALERT", "Alert", "alert"}, colorizeALERT},
	{0, []string{"ASSERT", "Assert", "assert"}, colorizeASSERT},
	{0, []string{"ALARM", "a", "Alarm", "alarm"}, colorizeALARM},
	{0, []string{"CYCLE_ERROR"}, colorizeCYCLE}, // not for user code!
	{0, []string{"VERBOSE", "v", "Verbose", "verbose"}, colorizeVERBOSE},
	{0, []string{"CONFIG", "cfg", "config"}, colorizeDEFAULT},
	{0, []string{"MICROSECOND", "us", "µs", "uS", "µS", "uSec", "µSec", "uSEC", "µSEC", "MicroSec", "Microsecond", "Microseconds"}, colorizeTIME},
	{0, []string{"MILLISECOND", "ms", "mS", "mSec", "mSEC", "MSEC", "MilliSec", "Millisecond", "Milliseconds"}, colorizeTIME},
	{0, []string{"SECOND", "s", "S", "Sec", "SEC", "SECONDS", "Second", "Seconds"}, colorizeTIME},
	{0, []string{"DELTATIME", "dt", "delta", "dT", "deltaTime", "delta-time"}, colorizeTIME},
}

// levelTags = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL", "FATAL", "EMERGENCY", "ATTENTION", "TRACE"}

// FindTagName maps any tag alias to its canonical name.
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

// tagVariants returns all known aliases for ch, or nil if unknown.
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

// colorize transforms s according to tag and palette configuration:
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

// colorize prefixes s with an ansi color code according to these conditions:
// If p.colorPalette is "off", do nothing.
// If p.colorPalette is "none" remove only lower case channel info "col:"
// If "COL:" is start of string add ANSI color code according to COL:
// If "col:" is start of string replace "col:" with ANSI color code according to col:
// Additionally, if global variable LogLevel is not the default "all", but found inside
// ColorChannels, logs with higher index positions are suppressed.
// As special case LogLevel == "off" does not output anything.
// Colorize applies global tag/palette rules to s.
func Colorize(s string) (r string) {
	if LogLevel == "off" {
		return // do not log at all, return empty string
	}

	r = s
	sc := strings.SplitN(s, ":", 2)
	if len(sc) < 2 { // no color separator (no log level)
		return r // do nothing, return unchanged string
	}
	for i, cc := range Tags {
		for _, c := range cc.Names {
			if c == sc[0] {
				Tags[i].count++ // count event
			}
			if c == LogLevel {
			}
		}
	}

	if ColorPalette == "off" {
		return r // do nothing (despite event counting)
	}
	if isTag(sc[0]) && isLower(sc[0]) {
		r = sc[1] // remove channel info
	}
	if ColorPalette == "none" {
		return r
	}
	for _, cs := range Tags {
		for _, c := range cs.Names {
			if c == sc[0] {
				return cs.colorize(r)
			}
		}
	}
	return r
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
