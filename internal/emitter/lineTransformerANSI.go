// SPDX-License-Identifier: MIT

package emitter

// TODO: Now the color is reset after each string. This is needed only after the last string in a line.

import (
	"errors"
	"fmt"
	"io"
	"strconv"
	"strings"
	"unicode"

	"github.com/mgutz/ansi"
)

const (
	minTagWeight = 0
	maxTagWeight = 999
	untaggedTag  = "untagged"
)

// userLabelSpec keeps independently supplied metadata for one -ulabel value.
type userLabelSpec struct {
	name      string
	weight    int
	hasWeight bool
	color     string
	hasColor  bool
}

// AddUserLabels rebuilds the per-command tag registry and applies all -ulabel
// specifications atomically. Unweighted new labels inherit the final INFO weight.
func AddUserLabels() error {
	tags := copyTagRegistry(defaultTags)
	// Defer unweighted new labels until INFO's final weight is known. This also
	// preserves the existing order of explicitly weighted and defaulted labels.
	pending := make(map[string]tag)
	pendingOrder := make([]string, 0, len(UserLabel))

	for _, specification := range UserLabel {
		parsed, err := parseUserLabel(specification)
		if err != nil {
			return fmt.Errorf("invalid -ulabel %q: %w", specification, err)
		}

		if i := tagIndex(tags, parsed.name); i >= 0 {
			if parsed.hasWeight {
				tags[i].weight = parsed.weight
			}
			if parsed.hasColor {
				tags[i].colorize = ansi.ColorFunc(parsed.color)
			}
			continue
		}
		newTag, wasPending := pending[parsed.name]
		if !wasPending {
			newTag = tag{Names: []string{parsed.name}, colorize: colorizeUSER}
		}
		if parsed.hasColor {
			newTag.colorize = ansi.ColorFunc(parsed.color)
		}
		if parsed.hasWeight {
			newTag.weight = parsed.weight
			tags = append(tags, newTag)
			delete(pending, parsed.name)
			continue
		}
		pending[parsed.name] = newTag
		if !wasPending {
			pendingOrder = append(pendingOrder, parsed.name)
		}
	}

	infoIndex := tagIndex(tags, "INFO")
	if infoIndex < 0 {
		return errors.New("built-in INFO tag is missing")
	}
	for _, name := range pendingOrder {
		newTag, ok := pending[name]
		if !ok {
			continue
		}
		newTag.weight = tags[infoIndex].weight
		tags = append(tags, newTag)
	}

	Tags = tags
	return nil
}

// parseUserLabel validates one name, name:weight, or name:generated-color value
// without changing the current registry.
func parseUserLabel(specification string) (parsed userLabelSpec, err error) {
	name, value, hasValue := strings.Cut(specification, ":")
	parsed.name = name
	if name == "" {
		return parsed, errors.New("tag name is empty")
	}
	if name == "all" || name == "off" || isDecimal(name) {
		return parsed, fmt.Errorf("tag name %q is reserved", name)
	}
	if !hasValue {
		return parsed, nil
	}
	if value == "" {
		return parsed, errors.New("tag weight or color is empty")
	}
	if !isDecimal(value) {
		if !isGeneratedColor(value) {
			return parsed, fmt.Errorf("unknown color %q; run trice generate -colors to list supported color strings", value)
		}
		parsed.color, parsed.hasColor = value, true
		return parsed, nil
	}
	weight, err := strconv.Atoi(value)
	if err != nil || weight < minTagWeight || weight > maxTagWeight {
		return parsed, fmt.Errorf("tag weight must be in range %d..%d", minTagWeight, maxTagWeight)
	}
	parsed.weight, parsed.hasWeight = weight, true
	return parsed, nil
}

// isDecimal reports whether s consists exclusively of ASCII decimal digits.
func isDecimal(s string) bool {
	if s == "" {
		return false
	}
	for _, r := range s {
		if r < '0' || r > '9' {
			return false
		}
	}
	return true
}

// lineTransformerANSI implements a Linewriter interface.
// It uses an internal Linewriter lw to write to.
// It converts the channel information to color data using colorPalette.
// In case of a remote display the lineTranslator should be used there.
type lineTransformerANSI struct {
	lw           LineWriter
	colorPalette string
}

// forEachGeneratedColor visits the exact color tokens displayed by -colors.
// Returning true from visit stops enumeration once a requested token is found.
func forEachGeneratedColor(visit func(string) bool) {
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
					if visit(colorCode) {
						return
					}
				}
			}
		}
	}
}

// isGeneratedColor accepts only strings from the existing -colors vocabulary.
func isGeneratedColor(value string) bool {
	found := false
	forEachGeneratedColor(func(color string) bool {
		found = color == value
		return found
	})
	return found
}

// ShowAllColors prints all foreground/background style combinations.
func ShowAllColors() {
	i := 0
	forEachGeneratedColor(func(colorCode string) bool {
		colorized := ansi.ColorFunc(colorCode)(colorCode)
		fmt.Printf("%4d:%24s:%s\n", i, colorCode, colorized)
		i++
		return false
	})
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

// colorizeUntagged preserves text without adding ANSI styling.
func colorizeUntagged(s string) string {
	return s
}

func isLower(s string) bool {
	for _, r := range s {
		if !unicode.IsLower(r) && unicode.IsLetter(r) {
			return false
		}
	}
	return true
}

type tag struct {
	count    int                 // count records successfully decoded application events in this tag group.
	weight   int                 // weight is the group priority and is independent of table order and color.
	Names    []string            // Names contains all aliases for one tag.
	colorize func(string) string // colorize is the function called for each tag.
}

// defaultTags contains the immutable built-in tag definitions used to start
// each command. CYCLE_ERROR is a tool diagnostic and its weight is not used
// for application-message selection.
var defaultTags = []tag{
	// log level
	{weight: 800, Names: []string{"FATAL", "Fatal", "fatal"}, colorize: colorizeFATAL},
	{weight: 750, Names: []string{"CRITICAL", "crit", "Critical", "critical", "Crit", "CRIT"}, colorize: colorizeCRITICAL},
	{weight: 700, Names: []string{"EMERGENCY", "em", "Emergency", "emergency"}, colorize: colorizeEMERGENCY},
	{weight: 650, Names: []string{"ERROR", "e", "Error", "err", "error", "E", "ERR"}, colorize: colorizeERROR},
	{weight: 600, Names: []string{"WARNING", "wrn", "Warning", "warning", "WRN", "Warn", "warn", "WARN"}, colorize: colorizeWARNING},
	{weight: 550, Names: []string{"ATTENTION", "att", "attention", "Attention", "ATT"}, colorize: colorizeATTENTION},
	{weight: 500, Names: []string{"INFO", "i", "inf", "info", "Info", "informal", "I", "INF", "INFORMAL"}, colorize: colorizeINFO},
	{weight: 300, Names: []string{"DEBUG", "d", "db", "Debug", "dbg", "deb", "debug", "D", "DB", "DBG"}, colorize: colorizeDEBUG},
	{weight: 200, Names: []string{"TRACE", "tr", "Trace", "trace"}, colorize: colorizeTRACE},

	// user modes
	{weight: 400, Names: []string{"TIME", "tim", "time", "Time", "TIM", "TIMESTAMP", "timestamp", "Timestamp"}, colorize: colorizeTIME},
	{weight: 400, Names: []string{"MESSAGE", "m", "msg", "message", "M", "MSG", "OK"}, colorize: colorizeMESSAGE},
	{weight: 400, Names: []string{"READ", "r", "rd", "read", "rd_", "RD", "RD_"}, colorize: colorizeREAD},
	{weight: 400, Names: []string{"WRITE", "w", "wr", "write", "wr_", "W", "WR", "WR_"}, colorize: colorizeWRITE},
	{weight: 400, Names: []string{"RECEIVE", "rx", "receive", "Receive", "RX"}, colorize: colorizeRECEIVE},
	{weight: 400, Names: []string{"TRANSMIT", "tx", "transmit", "Transmit", "TX"}, colorize: colorizeTRANSMIT},
	{weight: 400, Names: []string{"DIAG", "dia", "diag", "Diag", "DIA"}, colorize: colorizeDIAG},
	{weight: 400, Names: []string{"INTERRUPT", "int", "isr", "ISR", "INT", "interrupt", "Interrupt"}, colorize: colorizeINTERRUPT},
	{weight: 400, Names: []string{"SIGNAL", "sig", "signal", "SIG"}, colorize: colorizeSIGNAL},
	{weight: 400, Names: []string{"TEST", "t", "tst", "test", "T", "TST"}, colorize: colorizeTEST},

	{weight: 400, Names: []string{"DEFAULT", "def", "Default", "default"}, colorize: colorizeDEFAULT},
	{weight: 400, Names: []string{untaggedTag}, colorize: colorizeUntagged},
	{weight: 600, Names: []string{"NOTICE", "note", "Notice", "notice", "Note", "NOTE"}, colorize: colorizeNOTICE},
	{weight: 550, Names: []string{"ALERT", "Alert", "alert"}, colorize: colorizeALERT},
	{weight: 650, Names: []string{"ASSERT", "Assert", "assert"}, colorize: colorizeASSERT},
	{weight: 650, Names: []string{"ALARM", "a", "Alarm", "alarm"}, colorize: colorizeALARM},
	{weight: 0x0, Names: []string{"CYCLE_ERROR"}, colorize: colorizeCYCLE}, // not for user code!
	{weight: 100, Names: []string{"VERBOSE", "v", "Verbose", "verbose"}, colorize: colorizeVERBOSE},
	{weight: 500, Names: []string{"CONFIG", "cfg", "config"}, colorize: colorizeDEFAULT},
	{weight: 350, Names: []string{"MICROSECOND", "us", "µs", "uS", "µS", "uSec", "µSec", "uSEC", "µSEC", "MicroSec", "Microsecond", "Microseconds"}, colorize: colorizeTIME},
	{weight: 350, Names: []string{"MILLISECOND", "ms", "mS", "mSec", "mSEC", "MSEC", "MilliSec", "Millisecond", "Milliseconds"}, colorize: colorizeTIME},
	{weight: 350, Names: []string{"SECOND", "s", "S", "Sec", "SEC", "SECONDS", "Second", "Seconds"}, colorize: colorizeTIME},
	{weight: 350, Names: []string{"DELTATIME", "dt", "delta", "dT", "deltaTime", "delta-time"}, colorize: colorizeTIME},
}

// copyTagRegistry returns a deep copy so command-specific weights, counts, and
// user groups cannot modify the built-in definitions or a later command.
func copyTagRegistry(src []tag) []tag {
	dst := make([]tag, len(src))
	for i := range src {
		dst[i] = src[i]
		dst[i].Names = append([]string(nil), src[i].Names...)
	}
	return dst
}

// tagIndex returns the group index for any canonical name or alias.
func tagIndex(tags []tag, name string) int {
	for i, group := range tags {
		for _, alias := range group.Names {
			if alias == name {
				return i
			}
		}
	}
	return -1
}

// Tags contains all usable Trice tags for the current command.
//
// The optional target-side local-log hints in src/triceLogAnsi.c deliberately
// duplicate a subset of this host presentation policy without introducing a
// generated file or a Go dependency into the C library. Neither list is
// authoritative for the other. Maintainers may manually synchronize aliases
// and palette colors when matching local and host presentation is desired; the
// C file contains the reciprocal maintenance note.
var Tags = copyTagRegistry(defaultTags)

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

// TagWeight returns the current command's weight for a canonical tag name or alias.
func TagWeight(name string) (int, error) {
	i := tagIndex(Tags, name)
	if i < 0 {
		return 0, fmt.Errorf("no tag weight found for name %s", name)
	}
	return Tags[i].weight, nil
}

// logLevelWeight resolves a numeric threshold or a registered tag alias. The
// special values all and off are handled by the caller.
func logLevelWeight(value string) (int, error) {
	if value == "" {
		return 0, errors.New("level is empty")
	}
	if weight, err := strconv.Atoi(value); err == nil {
		if weight < minTagWeight || weight > maxTagWeight {
			return 0, fmt.Errorf("numeric level must be in range %d..%d", minTagWeight, maxTagWeight)
		}
		return weight, nil
	}
	weight, err := TagWeight(value)
	if err != nil {
		return 0, fmt.Errorf("unknown tag or numeric weight")
	}
	return weight, nil
}

// RecordTagEvent counts a successfully decoded application event before host
// selection. Missing and unknown format-string tags use the reserved untagged
// group; presentation fragments and diagnostics do not call this function.
func RecordTagEvent(candidate string) {
	if !TagStatistics && !AllStatistics {
		return
	}
	index := tagIndex(Tags, candidate)
	if index < 0 {
		index = tagIndex(Tags, untaggedTag)
	}
	if index >= 0 {
		Tags[index].count++
	}
}

// TagEvents returns count of successfully decoded application events in a group.
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

// NormalizeApplicationTag prepends the reserved untagged group when candidate
// is absent or unknown. The original text remains unchanged after the synthetic
// prefix. Existing spare buffer capacity avoids allocation in the normal path.
func NormalizeApplicationTag(text []byte, candidate string) []byte {
	if isTag(candidate) {
		return text
	}
	prefix := untaggedTag + ":"
	oldLength := len(text)
	if cap(text)-oldLength < len(prefix) {
		tagged := make([]byte, len(prefix)+oldLength)
		copy(tagged, prefix)
		copy(tagged[len(prefix):], text)
		return tagged
	}
	text = text[:oldLength+len(prefix)]
	copy(text[len(prefix):], text[:oldLength])
	copy(text, prefix)
	return text
}

// colorize transforms s according to tag and palette configuration:
// If p.colorPalette is "off", do nothing.
// If p.colorPalette is "none" remove only lower case channel info "col:"
// If "COL:" is start of string add ANSI color code according to COL:
// If "col:" is start of string replace "col:" with ANSI color code according to col:
// Event selection and counting happen before line composition so metadata and
// all lines of one application call share the same decision. This function
// only presents surviving fragments.
func (p *lineTransformerANSI) colorize(s string) (r string, show bool) {
	r = s
	sc := strings.SplitN(s, ":", 2)
	if len(sc) < 2 { // no color separator (no log level)
		return r, true // do nothing, return unchanged string
	}
	if p.colorPalette == "off" {
		return r, true // do nothing
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

// Colorize applies global tag/palette rules to a format string in the ID
// statistics report. It does not count events; LogLevel off still hides the
// formatted string, preserving the report's existing presentation behavior.
func Colorize(s string) (r string) {
	if LogLevel == "off" {
		return // do not log at all, return empty string
	}

	r = s
	sc := strings.SplitN(s, ":", 2)
	if len(sc) < 2 { // no color separator (no log level)
		return r // do nothing, return unchanged string
	}
	if ColorPalette == "off" {
		return r // do nothing
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
	l := make([]string, 0, 10)
	for _, s := range line {
		cs, _ := p.colorize(s)
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
