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
	// Apply the selected palette before explicit -ulabel overrides.
	for i := range tags {
		if style := tagColorStyle(tags[i].Names[0], ColorPalette); style != "" {
			tags[i].colorize = ansi.ColorFunc(style)
		}
	}
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
				tags[i].colorOverridden = true
			}
			continue
		}
		newTag, wasPending := pending[parsed.name]
		if !wasPending {
			newTag = tag{Names: []string{parsed.name}, colorize: colorizeUSER}
		}
		if parsed.hasColor {
			newTag.colorize = ansi.ColorFunc(parsed.color)
			newTag.colorOverridden = true
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
	// Show the actual tag assignments alongside the raw color vocabulary.
	fmt.Println("\nTag palettes: -color dark (default), light, or contrast")
	for _, palette := range []string{"dark", "light", "contrast"} {
		fmt.Printf("\n%s:\n", palette)
		for _, group := range defaultTags {
			name := group.Names[0]
			style := tagColorStyle(name, palette)
			if style == "" {
				style = "off"
			}
			fmt.Printf("  %-14s %-24s %s\n", name, style, ansi.ColorFunc(style)(name+": Example message"))
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
	colorizeFATAL     = ansi.ColorFunc(tagColorStyle("FATAL", "dark"))
	colorizeCRITICAL  = ansi.ColorFunc(tagColorStyle("CRITICAL", "dark"))
	colorizeEMERGENCY = ansi.ColorFunc(tagColorStyle("EMERGENCY", "dark"))
	colorizeERROR     = ansi.ColorFunc(tagColorStyle("ERROR", "dark"))
	colorizeWARNING   = ansi.ColorFunc(tagColorStyle("WARNING", "dark"))
	colorizeATTENTION = ansi.ColorFunc(tagColorStyle("ATTENTION", "dark"))
	colorizeINFO      = ansi.ColorFunc(tagColorStyle("INFO", "dark"))
	colorizeDEBUG     = ansi.ColorFunc(tagColorStyle("DEBUG", "dark"))
	colorizeTRACE     = ansi.ColorFunc(tagColorStyle("TRACE", "dark"))

	// user mode colors
	colorizeTIME      = ansi.ColorFunc(tagColorStyle("TIME", "dark"))
	colorizeMESSAGE   = ansi.ColorFunc(tagColorStyle("MESSAGE", "dark"))
	colorizeREAD      = ansi.ColorFunc(tagColorStyle("READ", "dark"))
	colorizeWRITE     = ansi.ColorFunc(tagColorStyle("WRITE", "dark"))
	colorizeRECEIVE   = ansi.ColorFunc(tagColorStyle("RECEIVE", "dark"))
	colorizeTRANSMIT  = ansi.ColorFunc(tagColorStyle("TRANSMIT", "dark"))
	colorizeDIAG      = ansi.ColorFunc(tagColorStyle("DIAG", "dark"))
	colorizeINTERRUPT = ansi.ColorFunc(tagColorStyle("INTERRUPT", "dark"))
	colorizeSIGNAL    = ansi.ColorFunc(tagColorStyle("SIGNAL", "dark"))
	colorizeTEST      = ansi.ColorFunc(tagColorStyle("TEST", "dark"))

	colorizeDEFAULT = ansi.ColorFunc("off") //
	colorizeNOTICE  = ansi.ColorFunc(tagColorStyle("NOTICE", "dark"))
	colorizeALERT   = ansi.ColorFunc(tagColorStyle("ALERT", "dark"))
	colorizeASSERT  = ansi.ColorFunc(tagColorStyle("ASSERT", "dark"))
	colorizeALARM   = ansi.ColorFunc(tagColorStyle("ALARM", "dark"))
	colorizeCYCLE   = ansi.ColorFunc(tagColorStyle("CYCLE_ERROR", "dark"))
	colorizeVERBOSE = ansi.ColorFunc(tagColorStyle("VERBOSE", "dark"))
	colorizeUSER    = ansi.ColorFunc("off") //

	AllStatistics bool // Keep the complete statistics when Trice is closed.
	TagStatistics bool // Print the occured count for each Trice log when Trice is closed.
)

// tagPaletteStyles keeps terminal-specific choices together for each tag.
// Urgent tags stand out through filled backgrounds in dark/light. Related I/O tags
// share a hue and differ by emphasis. Blinking and inverse video are avoided.
type tagPaletteStyles struct {
	dark     string // dark uses bright foregrounds on the terminal's dark background.
	light    string // light uses darker foregrounds on the terminal's light background.
	contrast string // contrast adds explicit backgrounds and emphasis for differentiation.
}

// tagPalette uses only the vocabulary shown by generate -colors.
// DEFAULT, untagged text, and unstyled user labels retain terminal defaults.
var tagPalette = map[string]tagPaletteStyles{
	"EMERGENCY":   {"white+b:red", "white+b:red", "white+b:red"},
	"FATAL":       {"white:magenta", "white:magenta", "white+b:magenta"},
	"CRITICAL":    {"white:red", "white:red", "black+b:red+h"},
	"ALARM":       {"black:red+h", "black:red+h", "black+u:red+h"},
	"ASSERT":      {"magenta+b:default", "magenta+b:default", "magenta+b:white+h"},
	"ERROR":       {"red+h:default", "red:default", "red+b:white+h"},
	"ALERT":       {"black:yellow", "black:yellow", "black+b:yellow+h"},
	"ATTENTION":   {"yellow+b:default", "black:yellow+h", "black+u:yellow+h"},
	"WARNING":     {"yellow+h:default", "yellow:black", "yellow+b:black"},
	"CONFIG":      {"cyan+u:default", "blue+u:default", "blue+u:white+h"},
	"INFO":        {"cyan+h:default", "blue:default", "cyan+h:black"},
	"MESSAGE":     {"green+h:default", "green:default", "green+h:black"},
	"NOTICE":      {"blue+h:default", "cyan+b:default", "blue+b:white+h"},
	"DEBUG":       {"magenta:default", "magenta:default", "magenta+h:black"},
	"DIAG":        {"cyan:default", "cyan:default", "black:cyan"},
	"READ":        {"green:default", "green+u:default", "black:green+h"},
	"RECEIVE":     {"green+u:default", "green+b:default", "black+u:green+h"},
	"TEST":        {"green+b:default", "green:black", "green+b:white+h"},
	"TRANSMIT":    {"yellow+u:default", "blue+b:default", "black+u:cyan+h"},
	"WRITE":       {"yellow:default", "blue+h:default", "black:cyan+h"},
	"INTERRUPT":   {"magenta+u:default", "magenta+u:default", "black:magenta+h"},
	"SIGNAL":      {"blue+b:default", "magenta:yellow+h", "black+u:magenta+h"},
	"TIME":        {"white:default", "black:default", "white:black"},
	"TRACE":       {"white+u:default", "black+u:default", "black:white"},
	"VERBOSE":     {"black+h:default", "black+h:default", "black+u:white"},
	"CYCLE_ERROR": {"red+u:default", "red+u:default", "red+u:white+h"},
}

// tagColorStyle groups time units and selects a palette. An empty result
// leaves deliberately unstyled tags unchanged. Default/color aliases and
// unknown palettes resolve to dark, matching the CLI fallback.
func tagColorStyle(name, palette string) string {
	switch name {
	case "DELTATIME", "MICROSECOND", "MILLISECOND", "SECOND":
		name = "TIME"
	}
	styles := tagPalette[name]
	switch palette {
	case "light":
		return styles.light
	case "contrast":
		return styles.contrast
	default:
		return styles.dark
	}
}

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
	count           int                 // count records successfully decoded application events in this tag group.
	weight          int                 // weight is the group priority and is independent of table order and color.
	Names           []string            // Names contains all aliases for one tag.
	caseInsensitive bool                // caseInsensitive recognizes built-in application aliases regardless of spelling.
	colorize        func(string) string // colorize is the function called for each tag.
	colorOverridden bool                // colorOverridden marks an explicit -ulabel color for presentation without a visible tag.
}

// defaultTags contains the immutable built-in tag definitions used to start
// each command. CYCLE_ERROR is a tool diagnostic and its weight is not used
// for application-message selection.
var defaultTags = builtInTags([]tag{
	// Descending weights and alphabetical peers make the policy easy to review;
	// filtering and level derivation do not depend on this presentation order.
	{weight: 900, Names: []string{"EMERGENCY", "em", "Emergency", "emergency"}, colorize: colorizeEMERGENCY},
	{weight: 900, Names: []string{"FATAL", "Fatal", "fatal"}, colorize: colorizeFATAL},
	{weight: 800, Names: []string{"CRITICAL", "crit", "Critical", "critical", "Crit", "CRIT"}, colorize: colorizeCRITICAL},
	{weight: 700, Names: []string{"ALARM", "Alarm", "alarm"}, colorize: colorizeALARM},
	{weight: 700, Names: []string{"ASSERT", "Assert", "assert"}, colorize: colorizeASSERT},
	{weight: 700, Names: []string{"ERROR", "Error", "err", "error", "ERR"}, colorize: colorizeERROR},
	{weight: 600, Names: []string{"ALERT", "Alert", "alert"}, colorize: colorizeALERT},
	{weight: 600, Names: []string{"ATTENTION", "att", "attention", "Attention", "ATT"}, colorize: colorizeATTENTION},
	{weight: 600, Names: []string{"WARNING", "wrn", "Warning", "warning", "WRN", "Warn", "warn", "WARN"}, colorize: colorizeWARNING},
	{weight: 500, Names: []string{"CONFIG", "cfg", "config"}, colorize: colorizeDEFAULT},
	{weight: 500, Names: []string{"DEFAULT", "def", "Default", "default"}, colorize: colorizeDEFAULT},
	{weight: 500, Names: []string{"INFO", "inf", "info", "Info", "informal", "INF", "INFORMAL"}, colorize: colorizeINFO},
	{weight: 500, Names: []string{"MESSAGE", "msg", "message", "MSG"}, colorize: colorizeMESSAGE},
	{weight: 500, Names: []string{"NOTICE", "note", "Notice", "notice", "Note", "NOTE"}, colorize: colorizeNOTICE},
	{weight: 500, Names: []string{untaggedTag}, colorize: colorizeUntagged},
	{weight: 300, Names: []string{"DEBUG", "db", "Debug", "dbg", "deb", "debug", "DB", "DBG"}, colorize: colorizeDEBUG},
	{weight: 300, Names: []string{"DIAG", "dia", "diag", "Diag", "DIA"}, colorize: colorizeDIAG},
	{weight: 300, Names: []string{"READ", "rd", "read", "rd_", "RD", "RD_"}, colorize: colorizeREAD},
	{weight: 300, Names: []string{"RECEIVE", "rx", "receive", "Receive", "RX"}, colorize: colorizeRECEIVE},
	{weight: 300, Names: []string{"TEST", "tst", "test", "TST"}, colorize: colorizeTEST},
	{weight: 300, Names: []string{"TRANSMIT", "tx", "transmit", "Transmit", "TX"}, colorize: colorizeTRANSMIT},
	{weight: 300, Names: []string{"WRITE", "wr", "write", "wr_", "WR", "WR_"}, colorize: colorizeWRITE},
	{weight: 100, Names: []string{"DELTATIME", "dt", "delta", "dT", "deltaTime", "delta-time"}, colorize: colorizeTIME},
	{weight: 100, Names: []string{"INTERRUPT", "int", "isr", "ISR", "INT", "interrupt", "Interrupt"}, colorize: colorizeINTERRUPT},
	{weight: 100, Names: []string{"MICROSECOND", "us", "µs", "uS", "µS", "uSec", "µSec", "uSEC", "µSEC", "MicroSec", "Microsecond", "Microseconds"}, colorize: colorizeTIME},
	{weight: 100, Names: []string{"MILLISECOND", "ms", "mS", "mSec", "mSEC", "MSEC", "MilliSec", "Millisecond", "Milliseconds"}, colorize: colorizeTIME},
	{weight: 100, Names: []string{"SECOND", "Sec", "SEC", "SECONDS", "Second", "Seconds"}, colorize: colorizeTIME},
	{weight: 100, Names: []string{"SIGNAL", "sig", "signal", "SIG"}, colorize: colorizeSIGNAL},
	{weight: 100, Names: []string{"TIME", "tim", "time", "Time", "TIM", "TIMESTAMP", "timestamp", "Timestamp"}, colorize: colorizeTIME},
	{weight: 100, Names: []string{"TRACE", "tr", "Trace", "trace"}, colorize: colorizeTRACE},
	{weight: 0, Names: []string{"VERBOSE", "Verbose", "verbose"}, colorize: colorizeVERBOSE},

	{weight: 0x0, Names: []string{"CYCLE_ERROR"}, colorize: colorizeCYCLE}, // not for user code!
})

// builtInTags marks application defaults for case-insensitive lookup once at
// initialization. Copies retain this policy; newly registered user tags do not
// acquire it. Tool diagnostics keep their exact spelling.
func builtInTags(tags []tag) []tag {
	for i := range tags {
		tags[i].caseInsensitive = tags[i].Names[0] != "CYCLE_ERROR"
		// Initialize direct emitter users with the same dark defaults as the CLI.
		if style := tagColorStyle(tags[i].Names[0], "dark"); style != "" {
			tags[i].colorize = ansi.ColorFunc(style)
		}
	}
	return tags
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

// tagIndex applies one recognition rule to metadata, weights, selectors and
// presentation. Built-in aliases ignore case; user labels remain literal so
// independently registered labels such as new and NEW cannot merge.
func tagIndex(tags []tag, name string) int {
	for i, group := range tags {
		for _, alias := range group.Names {
			if alias == name || (group.caseInsensitive && strings.EqualFold(alias, name)) {
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

// levelTags selects the canonical tags that define numeric level boundaries.
// Their immutable defaultTags weights apply to every command; changing a tag's
// effective weight with -ulabel never moves boundaries for other events.
var levelTags = []string{"FATAL", "CRITICAL", "ERROR", "WARNING", "INFO", "DEBUG", "TRACE", "VERBOSE"}

// TagLevel derives structured severity from the current weight of a canonical
// tag or registered alias. Unknown names and tool diagnostics have no level;
// application callers resolve missing/unknown format tags to untagged first.
func TagLevel(name string) string {
	index := tagIndex(Tags, name)
	if index < 0 || Tags[index].Names[0] == "CYCLE_ERROR" {
		return ""
	}
	// Keep the highest eligible named boundary, independent of either list's
	// order. Category tags and coincident category weights create no boundaries.
	weight := Tags[index].weight
	boundary := -1
	level := ""
	for _, group := range defaultTags {
		if group.weight > weight || group.weight <= boundary {
			continue
		}
		for _, candidate := range levelTags {
			if group.Names[0] == candidate {
				level, boundary = candidate, group.weight
				break
			}
		}
	}
	return level
}

// FindTagName maps any tag alias to its canonical name.
func FindTagName(name string) (tagName string, err error) {
	if index := tagIndex(Tags, name); index >= 0 {
		return Tags[index].Names[0], nil
	}
	return "", fmt.Errorf("no tagName found for name %s", name)
}

// UntaggedColorOverridden reports whether an explicit user color must still be
// applied when an untagged event has no visible tag prefix in its message.
func UntaggedColorOverridden() bool {
	index := tagIndex(Tags, untaggedTag)
	return index >= 0 && Tags[index].colorOverridden
}

// FindTagNameFold resolves all registered aliases without regard to case for
// Context Enrichment selectors, whose separate matching contract ignores case
// even for free selectors. Logging uses FindTagName to keep user tags literal.
func FindTagNameFold(name string) (string, error) {
	for _, t := range Tags {
		for _, alias := range t.Names {
			if strings.EqualFold(alias, name) {
				return t.Names[0], nil
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
	if index := tagIndex(Tags, ch); index >= 0 {
		return Tags[index].count
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
	if index := tagIndex(Tags, ch); index >= 0 {
		return Tags[index].Names
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
	if index := tagIndex(Tags, sc[0]); index >= 0 {
		return Tags[index].colorize(r), true
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
	if index := tagIndex(Tags, sc[0]); index >= 0 {
		return Tags[index].colorize(r)
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
	if p.colorPalette != "off" && p.colorPalette != "none" && 1 < len(l) && colored {
		l = append(l, ansi.Reset)
	}
	p.lw.WriteLine(l)
}
