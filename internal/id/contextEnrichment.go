// SPDX-License-Identifier: MIT

package id

import (
	"crypto/sha256"
	"encoding/json"
	"fmt"
	"io"
	"strconv"
	"strings"
	"unicode"

	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/fmtspec"
)

// ContextEnrichment holds the repeatable, command-local bind -ce options.
var ContextEnrichment ArrayFlag

// bindLimitsHint keeps unsupported-construct diagnostics short and searchable.
const bindLimitsHint = `Search UM for "bind-limits".`

// contextRule retains CLI order within a canonical selector group. Formats
// keep their C escapes, just like source templates and the serialized TIL.
type contextRule struct {
	selector    string
	format      string
	expressions []string
}

// bindEnrichment separates the original call signature from its final schema.
// Only the generated adapter evaluates expressions; the source remains intact.
type bindEnrichment struct {
	arguments         []string
	expressions       []string
	originalArguments int
	sourceHash        string
	end               int
	lastLine          int
}

// bindContextMetadata ties the final schema to one exact source call and ID.
// generate -logC reads it without needing the original CLI rules or guessing IDs.
type bindContextMetadata struct {
	Site   int     `json:"site"`
	Source string  `json:"source"`
	ID     TriceID `json:"id"`
	Type   string  `json:"type"`
	Format string  `json:"format"`
}

// contextSelector accepts a nonempty prefix token, not arbitrary message text.
// Registered tag aliases share a group; free selectors only ignore case.
func contextSelector(name string) (key string, known bool) {
	if name == "" || strings.ContainsAny(name, ":\"\\{}%") || strings.ContainsFunc(name, unicode.IsSpace) {
		return "", false
	}
	if tag, err := emitter.FindTagNameFold(name); err == nil {
		return strings.ToLower(tag), true
	}
	return strings.ToLower(name), false
}

// parseContextRules validates every rule before source discovery or writes,
// including rules that do not select a site in this particular invocation.
func parseContextRules(values []string) ([]contextRule, error) {
	var rules []contextRule
	for _, value := range values {
		name, rest, found := strings.Cut(value, ":")
		key, _ := contextSelector(name)
		rest = strings.TrimSpace(rest)
		if !found || key == "" || len(rest) < 2 || rest[0] != '"' || strings.ContainsAny(value, "\r\n\x00") {
			return nil, fmt.Errorf("invalid -ce %q: expected selector:\"format-extension\"[, comma-free C-expression]...", value)
		}
		// Locate the closing quote without interpreting C escapes a second time.
		end := 1
		for end < len(rest) {
			if rest[end] == '\\' {
				end += 2
				continue
			}
			if rest[end] == '"' {
				break
			}
			end++
		}
		if end >= len(rest) {
			return nil, fmt.Errorf("invalid -ce %q: unclosed format string", value)
		}
		rule := contextRule{selector: key, format: rest[1:end]}
		tail := strings.TrimSpace(rest[end+1:])
		if tail != "" {
			if tail[0] != ',' {
				return nil, fmt.Errorf("invalid -ce %q: expected comma before C-expression", value)
			}
			for _, expression := range strings.Split(tail[1:], ",") {
				expression = strings.TrimSpace(expression)
				wrapped := "(" + expression + ")"
				if expression == "" || contextClosingParen(wrapped, 1) != len(wrapped)-1 || strings.Contains(expression, "//") {
					return nil, fmt.Errorf("invalid -ce %q: expected nonempty, comma-free C-expressions with balanced parentheses", value)
				}
				rule.expressions = append(rule.expressions, expression)
			}
		}
		template, err := fmtspec.ParseTemplate(rule.format, rule.expressions)
		if err != nil {
			return nil, fmt.Errorf("invalid -ce %q: %w", value, err)
		}
		if len(template.Specs) != len(rule.expressions) {
			return nil, fmt.Errorf("invalid -ce %q: %d format conversion(s) for %d expression(s)", value, len(template.Specs), len(rule.expressions))
		}
		rule.format = template.Canonical
		rules = append(rules, rule)
	}
	return rules, nil
}

// selectContextRules walks the prefix chain in source order. Only configured
// lowercase free selectors disappear; known tags retain normal color behavior.
func selectContextRules(format string, rules []contextRule) (string, []contextRule, []string) {
	var prefix strings.Builder
	var selected []contextRule
	var duplicates []string
	seen := make(map[string]bool)
	for {
		name, rest, found := strings.Cut(format, ":")
		key, known := contextSelector(name)
		if !found || key == "" {
			break
		}
		var matching []contextRule
		for _, rule := range rules {
			if rule.selector == key {
				matching = append(matching, rule)
			}
		}
		if len(matching) != 0 {
			if seen[key] {
				duplicates = append(duplicates, name)
			} else {
				selected = append(selected, matching...)
				seen[key] = true
			}
		}
		if len(matching) == 0 || known || name != strings.ToLower(name) {
			prefix.WriteString(name + ":")
		}
		format = rest
	}
	return prefix.String() + format, selected, duplicates
}

// prepareBindContext enriches only direct sites after all ordinary Bind source
// layout planning, but before historical ID selection and the shared allocator.
func prepareBindContext(w io.Writer, plans []bindFilePlan, rules []contextRule) []bindDiagnostic {
	if len(rules) == 0 {
		return nil
	}
	var diagnostics []bindDiagnostic
	for planIndex := range plans {
		plan := &plans[planIndex]
		// Mask once per file, preserving offsets without rescanning the entire
		// source for every selected call in a large instrumentation matrix.
		source := string(plan.final)
		masked := stripCComments(source)
		for siteIndex := range plan.sites {
			site := &plan.sites[siteIndex]
			format, selected, duplicates := selectContextRules(site.format, rules)
			if len(selected) == 0 {
				continue
			}
			if !directContextSite(plan, planIndex, siteIndex, masked) {
				diagnostics = append(diagnostics, bindDiagnostic{path: plan.path, line: site.line, message: "CE requires a direct, line-addressable bind site"})
				continue
			}
			if err := enrichBindSite(source, masked, site, format, selected); err != nil {
				diagnostics = append(diagnostics, bindDiagnostic{path: plan.path, line: site.line, message: "CE: " + err.Error()})
				continue
			}
			if len(duplicates) != 0 {
				fmt.Fprintf(w, "%s:%d: warning: duplicate CE selector(s) %s; each selector is applied once\n", plan.path, site.line, strings.Join(duplicates, ", "))
			}
		}
	}
	return diagnostics
}

// contextClosingParen respects both C string and character literals. Source
// callers mask comments first; CLI expressions must fit one generated line.
func contextClosingParen(text string, start int) int {
	depth, quote, escaped := 1, byte(0), false
	for i := start; i < len(text); i++ {
		c := text[i]
		if quote != 0 {
			if escaped {
				escaped = false
			} else if c == '\\' {
				escaped = true
			} else if c == quote {
				quote = 0
			}
			continue
		}
		switch c {
		case '\'', '"':
			quote = c
		case '(':
			depth++
		case ')':
			depth--
			if depth == 0 {
				return i
			}
		}
	}
	return -1
}

// directContextSite permits a multiline call only when every occupied line
// identifies that same site. This works with preprocessors reporting either
// the first or last line, without changing ordinary unselected Bind sites.
func directContextSite(plan *bindFilePlan, planIndex, siteIndex int, masked string) bool {
	site := plan.sites[siteIndex]
	if plan.class != bindFileBound || site.definitionName != "" || site.counterSelected {
		return false
	}
	end := contextClosingParen(masked, site.loc[2])
	if end < site.loc[6] {
		return false
	}
	lastLine := site.line + strings.Count(masked[site.loc[0]:end], "\n")
	for _, descriptor := range plan.descriptors {
		if descriptor.line >= site.line && descriptor.line <= lastLine && descriptor.ref != (bindSiteReference{plan: planIndex, site: siteIndex}) {
			return false
		}
	}
	return true
}

// enrichBindSite validates both the original signature and the final template.
// Fixed-arity macros gain a matching implementation; generic macros keep theirs.
func enrichBindSite(source, masked string, site *bindSite, format string, rules []contextRule) error {
	entry := bindSiteFormat(*site)
	args, err := splitTriceParametersUntilClosingBracket(masked[site.loc[6]:])
	if err != nil {
		return err
	}
	if err := evaluateTriceParameterCount(entry, site.line, contextArgumentTail(args)); err != nil {
		return err
	}
	end := contextClosingParen(masked, site.loc[2]) + 1
	ce := &bindEnrichment{
		originalArguments: len(args), sourceHash: bindContextSourceHash(source, masked, *site),
		end: end, lastLine: site.line + strings.Count(source[site.loc[0]:end], "\n"),
	}
	// Insert all extensions before the source's final newline, preserving C
	// escape spelling. An escaped backslash followed by n is literal text.
	suffix := ""
	if strings.HasSuffix(format, `\n`) {
		backslashes := 0
		for i := len(format) - 2; i >= 0 && format[i] == '\\'; i-- {
			backslashes++
		}
		if backslashes%2 == 1 {
			format, suffix = format[:len(format)-2], `\n`
		}
	}
	for _, rule := range rules {
		format += rule.format
		ce.expressions = append(ce.expressions, rule.expressions...)
	}
	ce.arguments = append(append([]string{}, args...), ce.expressions...)
	category := triceTypeCategory(entry.Type)
	if category != "" && len(ce.expressions) != 0 {
		return fmt.Errorf("%s cannot append runtime CE arguments; use a scalar Trice", entry.Type)
	}
	if category == "" && len(ce.expressions) != 0 {
		if before, _, fixed := strings.Cut(entry.Type, "_"); fixed {
			entry.Type = before + "_" + strconv.Itoa(len(ce.arguments))
		} else if strings.HasSuffix(entry.Type, "0") {
			entry.Type = strings.TrimSuffix(entry.Type, "0") + "_" + strconv.Itoa(len(ce.arguments))
		}
	}
	entry.Strg = format + suffix
	template, err := fmtspec.ParseTemplate(entry.Strg, ce.arguments)
	if err != nil {
		return err
	}
	entry.Strg = template.Canonical
	if err := ValidateStructuredFields(entry, template); err != nil {
		return err
	}
	if err := evaluateTriceParameterCount(entry, site.line, contextArgumentTail(ce.arguments)); err != nil {
		return err
	}
	// Classic %f and named float fields share the same explicit wrapper rule.
	for i, spec := range template.Specs {
		if spec.Kind == fmtspec.KindFloat && triceValueBitWidth(entry.Type, len(template.Specs)) == 64 && isTopLevelCall(ce.arguments[i], "aFloat") {
			return fmt.Errorf("64-bit CE float argument %d requires aDouble(), not aFloat()", i+1)
		}
	}
	site.macro, site.format, site.ce = entry.Type, entry.Strg, ce
	return nil
}

// contextArgumentTail supplies the existing parser's post-format input shape.
func contextArgumentTail(args []string) string {
	if len(args) == 0 {
		return ")"
	}
	return ", " + strings.Join(args, ", ") + ")"
}

// bindContextSourceHash detects stale generated schemas even when only source
// arguments changed. Physical shifts outside the call do not invalidate it.
func bindContextSourceHash(source, masked string, site bindSite) string {
	end := contextClosingParen(masked, site.loc[2]) + 1
	if end <= site.loc[0] {
		return ""
	}
	return fmt.Sprintf("%x", sha256.Sum256([]byte(source[site.loc[0]:end])))
}

// contextInsertEdit feeds the final schema to the existing allocator in a
// private source view. Preserved newline counts keep subsequent locations valid.
func contextInsertEdit(source string, site bindSite, preferred TriceID) sourceEdit {
	end := site.ce.end
	call := site.macro + "("
	if preferred != 0 || site.mode == bindSiteReplace {
		call += fmt.Sprintf("%s(%d), ", site.wrapper, preferred)
	}
	// Names and float displays are already canonical, and original arguments
	// were validated before enrichment. The allocator needs only their shape,
	// not arbitrary C expressions that its lightweight lexer may misinterpret.
	// Actual expressions remain exclusively in the source and generated adapter.
	virtualArgs := make([]string, len(site.ce.arguments))
	for i, expression := range site.ce.arguments {
		virtualArgs[i] = "0"
		for _, wrapper := range []string{"aFloat", "aDouble"} {
			if isTopLevelCall(expression, wrapper) {
				virtualArgs[i] = wrapper + "(0)"
			}
		}
	}
	call += `"` + site.format + `"` + contextArgumentTail(virtualArgs)
	if missing := strings.Count(source[site.loc[0]:end], "\n") - strings.Count(call, "\n"); missing > 0 {
		call += strings.Repeat("\n", missing)
	}
	return sourceEdit{start: site.loc[0], end: end, replacement: call}
}

// bindContextMode names the per-call adapter without changing numeric IDs.
func bindContextMode(key string, site bindSite) string {
	if site.ce != nil {
		return fmt.Sprintf("TRICE_BIND_CE_%s_L%d", key, site.line)
	}
	return string(site.mode)
}

// renderBindContext emits one adapter per enriched site. Original and injected
// expressions occur exactly once; no runtime context or allocation is introduced.
func renderBindContext(builder *strings.Builder, plan *bindFilePlan) {
	for index, site := range plan.sites {
		if site.ce == nil {
			continue
		}
		metadata := bindContextMetadata{index, site.ce.sourceHash, site.id, site.macro, site.format}
		encoded, _ := json.Marshal(metadata) // This fixed struct contains only JSON-safe primitive values.
		fmt.Fprintf(builder, "// trice-ce: %s\n", encoded)
		// Macro parameter names must never capture an identifier in a user's
		// injected expression (including a local named tid, format, or arg0).
		prefix := "trice_ce_"
		for strings.Contains(strings.Join(site.ce.expressions, " "), prefix) {
			prefix += "_"
		}
		parameters := []string{prefix + "implementation", prefix + "tid"}
		if site.mode == bindSiteReplace {
			parameters = append(parameters, prefix+"ignoredTid")
		}
		parameters = append(parameters, prefix+"format")
		arguments := []string{prefix + "tid", prefix + "format"}
		for i := 0; i < site.ce.originalArguments; i++ {
			name := fmt.Sprintf("%sarg%d", prefix, i)
			parameters = append(parameters, name)
			arguments = append(arguments, name)
		}
		for _, expression := range site.ce.expressions {
			arguments = append(arguments, "("+expression+")")
		}
		fmt.Fprintf(builder, "#define %s(%s) TRICE_INSERT_%s(%s)\n", bindContextMode(plan.key, site), strings.Join(parameters, ", "), site.macro, strings.Join(arguments, ", "))
		// Aliases carry no additional numeric descriptors, so source ordering,
		// historical IDs, and logC still see exactly one logical site.
		for line := site.line + 1; line <= site.ce.lastLine; line++ {
			fmt.Fprintf(builder, "#define %s %s\n", bindSiteMacroName(plan.key, line), bindSiteMacroName(plan.key, site.line))
		}
	}
}

// readBindContextMetadata refuses stale or malformed CE facts before logC
// output. Metadata is supplemental; numeric descriptors remain authoritative.
func readBindContextMetadata(content []byte, plan *bindFilePlan, ids []TriceID) (map[int]bindContextMetadata, error) {
	metadata := make(map[int]bindContextMetadata)
	source := string(plan.final)
	masked := stripCComments(source)
	for _, line := range strings.Split(string(content), "\n") {
		if !strings.HasPrefix(line, "// trice-ce: ") {
			continue
		}
		var item bindContextMetadata
		if err := json.Unmarshal([]byte(strings.TrimPrefix(line, "// trice-ce: ")), &item); err != nil {
			return nil, fmt.Errorf("invalid CE metadata: %w", err)
		}
		if _, exists := metadata[item.Site]; exists || item.Site < 0 || item.Site >= len(plan.sites) || item.Site >= len(ids) || item.ID != ids[item.Site] || item.Type == "" || item.Source == "" || item.Source != bindContextSourceHash(source, masked, plan.sites[item.Site]) {
			return nil, fmt.Errorf("stale or inconsistent CE metadata; run trice bind with the current -ce rules before trice generate -logC")
		}
		metadata[item.Site] = item
	}
	return metadata, nil
}
