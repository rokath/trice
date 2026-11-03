// SPDX-License-Identifier: MIT

package id

import (
	"bytes"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"slices"
	"sort"
	"strconv"
	"strings"

	"github.com/rokath/trice/internal/fmtspec"
	"github.com/rokath/trice/pkg/ant"
	"github.com/spf13/afero"
)

// sourceContextExtension joins every selected rule in application order. The
// physical spelling, including anonymous fields, defines a complete match.
func sourceContextExtension(rules []contextRule) (string, []string) {
	var format strings.Builder
	var expressions []string
	for _, rule := range rules {
		format.WriteString(rule.sourceFormat)
		expressions = append(expressions, rule.expressions...)
	}
	return format.String(), expressions
}

// sourceContextMatch compares the complete format and argument suffix together.
// Comments and outer argument whitespace are ignored, just as in the existing
// C argument parser; different expressions or field spellings are not guessed
// equivalent. The returned format retains any original final newline.
func sourceContextMatch(format, extension string, args, expressions []string, rules []contextRule, macro ...string) (string, bool) {
	if len(args) < len(expressions) {
		return "", false
	}
	for i, expression := range expressions {
		if args[len(args)-len(expressions)+i] != strings.TrimSpace(stripCComments(expression)) {
			return "", false
		}
	}
	full, err := fmtspec.ParseTemplate(format, args)
	if err != nil {
		return "", false
	}
	body, newline := contextNewlineSuffix(format)
	// Try before the original newline first. Also allow an extension that
	// itself ends in \n on a source that originally had no final newline.
	for _, ending := range [][2]string{{body, newline}, {format, ""}} {
		if !strings.HasSuffix(ending[0], extension) {
			continue
		}
		original := strings.TrimSuffix(ending[0], extension) + ending[1]
		// A textual suffix inside %%d or the tail of a conversion is not
		// the rule's complete format: its arguments belong to other text.
		prefix, err := fmtspec.ParseTemplate(original, args[:len(args)-len(expressions)])
		if err != nil || len(prefix.Specs)+len(expressions) != len(full.Specs) {
			continue
		}
		_, remaining, _ := selectContextRules(original, rules)
		if len(macro) > 0 {
			_, remaining, _ = selectTriceContextRules(TriceFmt{Type: macro[0], Strg: original}, rules)
		}
		// A text-only extension must not consume its own selecting prefix.
		if slices.EqualFunc(remaining, rules, func(a, b contextRule) bool {
			return a.selector == b.selector && a.sourceFormat == b.sourceFormat && slices.Equal(a.expressions, b.expressions)
		}) {
			return original, true
		}
	}
	return "", false
}

// sourceContextCommas locates argument separators in a comment-masked tail.
// Skipping literals and nested parentheses preserves the original source bytes
// before the first removed CE argument, including its preceding whitespace.
func sourceContextCommas(tail string) []int {
	var commas []int
	var quote byte
	escaped := false
	for i := 0; i < len(tail); i++ {
		c := tail[i]
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
			i = contextClosingParen(tail, i+1)
			if i < 0 {
				return nil
			}
		case ')':
			return commas
		case ',':
			commas = append(commas, i)
		}
	}
	return commas
}

// sourceContextReducedMacro adjusts a fixed arity after removing CE arguments.
// Without provenance, the zero-argument generic spelling is normalized to
// trice0/TRICE0; generic and special record families otherwise keep their name.
func sourceContextReducedMacro(macro string, count int) string {
	entry := TriceFmt{Type: macro}
	resolveTriceAlias(&entry)
	if entry.Type != macro || triceTypeCategory(macro) != "" {
		return macro
	}
	if base, _, fixed := strings.Cut(macro, "_"); fixed {
		if count == 0 && strings.EqualFold(base, "trice") {
			return base + "0"
		}
		return base + "_" + strconv.Itoa(count)
	}
	return macro
}

// transformSourceContext applies the current rules using suffix matching only.
// A full match is retained by insert and removed by clean regardless of origin.
// Partial matches are ordinary source: insert appends the whole extension and
// clean leaves it alone. The preflight-only clean pass preserves enriched TIL
// entries until ordinary ID cleanup has consumed the still-enriched calls.
func transformSourceContext(w io.Writer, path, source string, rules []contextRule, clean, restore bool, originalTags ...*[]string) (string, error) {
	if hasTriceBindSidecarInclude(source) {
		return source, nil
	}
	sites, diagnostics := scanSourceSites(path, source, false)
	if len(diagnostics) != 0 {
		return "", fmt.Errorf("%s", formatBindDiagnostic(diagnostics[0]))
	}
	var edits []sourceEdit
	masked := stripCComments(source)
	for _, site := range sites {
		entry := bindSiteFormat(site)
		buildTag := site.buildTag
		if len(originalTags) > 0 {
			*originalTags[0] = append(*originalTags[0], buildTag)
		}
		if strings.TrimSpace(masked[site.loc[0]:site.loc[1]]) == "" {
			continue // Commented examples retain their ordinary ID-only behavior.
		}
		format, selected, duplicates := selectTriceContextRules(entry, rules, buildTag)
		if err := validateAliasContext(entry, selected); err != nil {
			return "", fmt.Errorf("%s:%d: CE: %w", path, site.line, err)
		}
		payload, _ := PayloadFormat(entry)
		if payload.Alias != "" && payload.Type == "triceS" {
			continue
		}
		if len(selected) == 0 && payload.Alias == "" {
			continue
		}
		physical := source[site.loc[5]+1 : site.loc[6]-1]
		args, err := splitTriceParametersUntilClosingBracket(masked[site.loc[6]:])
		if err != nil {
			return "", fmt.Errorf("%s:%d: CE: %w", path, site.line, err)
		}
		extension, expressions := sourceContextExtension(selected)
		original, matches := "", false
		if len(selected) > 0 {
			original, matches = sourceContextMatch(physical, extension, args, expressions, selected, site.macro)
		}
		// CE can itself introduce the first prefix of an originally empty alias
		// format. Recover only a complete suffix whose reconstructed source
		// selects exactly that group; never select again from the stored host tag.
		if !matches && payload.Alias != "" {
			seen := map[string]bool{}
			for _, rule := range rules {
				if seen[rule.selector] {
					continue
				}
				seen[rule.selector] = true
				if _, known := contextSelector(rule.selector); !known {
					continue
				}
				candidateFormat, candidates, candidateDuplicates := selectTriceContextRules(entry, rules, rule.selector)
				candidateExtension, candidateExpressions := sourceContextExtension(candidates)
				if reconstructed, ok := sourceContextMatch(physical, candidateExtension, args, candidateExpressions, candidates, site.macro); ok {
					format, selected, duplicates = candidateFormat, candidates, candidateDuplicates
					extension, expressions = candidateExtension, candidateExpressions
					original, matches = reconstructed, true
					break
				}
			}
		}
		if len(selected) == 0 {
			continue
		}
		if len(expressions) != 0 && triceTypeCategory(payload.Type) != "" {
			matches = false // Buffer/string arguments are not scalar CE arguments.
		}
		if matches {
			buildTag = triceTag(TriceFmt{Type: site.macro, Strg: original})
			if len(originalTags) > 0 {
				tags := *originalTags[0]
				tags[len(tags)-1] = buildTag
			}
		}
		if !restore && (!clean || matches) {
			effective := entry
			if matches {
				if err := canonicalizeSourceTemplate(&effective, masked[site.loc[6]:]); err != nil {
					return "", fmt.Errorf("%s:%d: CE: %w", path, site.line, err)
				}
				effective.Strg, _, _ = selectTriceContextRules(effective, rules, buildTag)
			} else {
				prepared := site
				if err := enrichBindSite(source, masked, &prepared, format, selected); err != nil {
					return "", fmt.Errorf("%s:%d: CE: %w", path, site.line, err)
				}
				effective = bindSiteFormat(prepared)
			}
			warnContextFallback(w, path, site.line, buildTag, effective)
		}
		// Preflight validates and diagnoses matching Clean transformations before
		// ID/cache processing; only the later restore pass edits source bytes.
		if clean && !restore {
			continue
		}
		if clean && !matches || !clean && matches {
			continue
		}
		close := contextClosingParen(masked, site.loc[2])
		if close < 0 {
			return "", fmt.Errorf("%s:%d: CE call has no closing parenthesis", path, site.line)
		}
		if clean {
			edits = append(edits, sourceEdit{start: site.loc[5], end: site.loc[6], replacement: `"` + original + `"`})
			if len(expressions) != 0 {
				commas := sourceContextCommas(masked[site.loc[6] : close+1])
				first := commas[len(commas)-len(expressions)]
				edits = append(edits, sourceEdit{start: site.loc[6] + first, end: close})
				macro := sourceContextReducedMacro(site.macro, len(args)-len(expressions))
				if macro != site.macro {
					edits = append(edits, sourceEdit{start: site.loc[0], end: site.loc[1], replacement: macro})
				}
			}
			continue
		}
		if err := enrichBindSite(source, masked, &site, format, selected); err != nil {
			return "", fmt.Errorf("%s:%d: CE: %w", path, site.line, err)
		}
		body, newline := contextNewlineSuffix(physical)
		edits = append(edits, sourceEdit{start: site.loc[5], end: site.loc[6], replacement: `"` + body + extension + newline + `"`})
		if source[site.loc[0]:site.loc[1]] != site.macro {
			edits = append(edits, sourceEdit{start: site.loc[0], end: site.loc[1], replacement: site.macro})
		}
		if len(expressions) != 0 {
			edits = append(edits, sourceEdit{start: close, end: close, replacement: ", " + strings.Join(expressions, ", ")})
		}
		if len(duplicates) != 0 {
			fmt.Fprintf(w, "%s:%d: warning: duplicate CE selector(s) %s; each selector is applied once\n", path, site.line, strings.Join(duplicates, ", "))
		}
	}
	sort.Slice(edits, func(i, j int) bool { return edits[i].start < edits[j].start })
	return applySourceEdits(source, edits)
}

// switchSourceContext stages both CE and normal ID processing in memory. No
// source, dictionary or field file changes until every selected file succeeds.
// Existing atomic rollback handles a failure while publishing the final set.
func switchSourceContext(w io.Writer, fs *afero.Afero, clean bool) error {
	rules, err := parseContextRules(ContextEnrichment)
	if err != nil {
		return err
	}
	private := &afero.Afero{Fs: afero.NewCopyOnWriteFs(fs.Fs, afero.NewMemMapFs())}
	previousTags := IDData.sourceBuildTags
	IDData.sourceBuildTags = make(map[string][]string)
	defer func() { IDData.sourceBuildTags = previousTags }()
	var paths []string
	admin := &ant.Admin{Trees: Srcs, ExcludeTrees: ExcludeSrcs, MatchingFileName: isSourceFile}
	admin.Action = func(_ io.Writer, _ *afero.Afero, path string, _ os.FileInfo, a *ant.Admin) error {
		a.Mutex.Lock()
		defer a.Mutex.Unlock()
		paths = append(paths, path)
		return nil
	}
	if err := admin.Walk(w, fs); err != nil {
		return err
	}
	sort.Strings(paths)
	paths = slices.Compact(paths)
	for _, path := range paths {
		content, err := fs.ReadFile(path)
		if err != nil {
			return err
		}
		var buildTags []string
		prepared, err := transformSourceContext(w, path, string(content), rules, clean, false, &buildTags)
		if err != nil {
			return err
		}
		IDData.sourceBuildTags[filepath.Clean(path)] = buildTags
		if prepared != string(content) {
			if err := atomicWriteFile(private, path, []byte(prepared), fileWritePerm(fs, path, 0o644)); err != nil {
				return err
			}
		}
	}
	if clean {
		err = IDData.cmdSwitchTriceIDs(w, private, IDData.processTriceIDCleaning)
	} else {
		err = insertIDs(w, private, IDData.processTriceIDInsertion)
	}
	if err != nil {
		return err
	}
	if clean {
		for _, path := range paths {
			content, err := private.ReadFile(path)
			if err != nil {
				return err
			}
			restored, err := transformSourceContext(w, path, string(content), rules, true, true)
			if err != nil {
				return err
			}
			if restored != string(content) {
				if err := atomicWriteFile(private, path, []byte(restored), fileWritePerm(fs, path, 0o644)); err != nil {
					return err
				}
			}
		}
	}
	if DryRun {
		return nil
	}
	paths = append(paths, FnJSON, LIFnJSON)
	fieldsPath := filepath.Join(FieldsDir, "trice-fields.txt")
	if !clean {
		paths = append(paths, fieldsPath)
	}
	sort.Strings(paths)
	var writes []bindWrite
	for _, path := range slices.Compact(paths) {
		content, err := private.ReadFile(path)
		if os.IsNotExist(err) {
			continue // Disabled or unused dictionaries do not become new files.
		}
		if err != nil {
			return err
		}
		original, err := fs.ReadFile(path)
		if err != nil && !os.IsNotExist(err) {
			return err
		}
		if err == nil && bytes.Equal(content, original) {
			continue
		}
		kind := "source"
		if path == fieldsPath {
			kind = "fields"
		}
		writes = append(writes, bindWrite{path: path, data: content, perm: fileWritePerm(fs, path, 0o644), kind: kind})
	}
	return commitBindWrites(fs, writes)
}
