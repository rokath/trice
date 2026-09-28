// SPDX-License-Identifier: MIT

package id

import (
	"bytes"
	"crypto/sha256"
	"encoding/base64"
	"encoding/json"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"slices"
	"sort"
	"strings"
	"unicode"

	"github.com/rokath/trice/pkg/ant"
	"github.com/spf13/afero"
)

// insertContextPrefix reserves an adjacent generated comment as ownership proof.
// Base64 keeps original C comments, quotes and macro continuations inert in C.
const insertContextPrefix = "/* trice-ce: "

// insertContextMetadata travels with the call, independent of line numbers,
// IDs, build directories and caches. Only proven generated calls are reversed.
type insertContextMetadata struct {
	Version  int      `json:"version"`
	Original string   `json:"original"`
	Rules    []string `json:"rules"`
	Config   string   `json:"config"`         // Fingerprint the complete ordered CLI list without repeating it at every site.
	Tags     []string `json:"tags,omitempty"` // Preserve tag policy independently of later commands.
}

// readInsertContext returns the complete owned range relative to the text just
// after a format literal. Whitespace before the comment may be formatter-owned.
func readInsertContext(rest string) (*insertContextMetadata, int, error) {
	close := contextClosingParen(rest, 0)
	if close < 0 {
		return nil, 0, nil // The normal source parser diagnoses malformed calls.
	}
	tail := strings.TrimLeftFunc(rest[close+1:], unicode.IsSpace)
	if !strings.HasPrefix(tail, insertContextPrefix) {
		return nil, 0, nil
	}
	end := strings.Index(tail, "*/")
	if end < 0 {
		return nil, 0, fmt.Errorf("CE ownership comment is incomplete; restore it before insert/clean")
	}
	encoded := strings.TrimSpace(tail[len(insertContextPrefix):end])
	data, err := base64.RawStdEncoding.DecodeString(encoded)
	if err != nil {
		return nil, 0, fmt.Errorf("invalid CE ownership comment: %w", err)
	}
	var metadata insertContextMetadata
	if err := json.Unmarshal(data, &metadata); err != nil {
		return nil, 0, fmt.Errorf("invalid CE ownership comment: %w", err)
	}
	if metadata.Version != 1 || metadata.Original == "" || len(metadata.Rules) == 0 || len(metadata.Config) != 64 {
		return nil, 0, fmt.Errorf("invalid CE ownership comment: expected version 1, original call, rules and configuration fingerprint")
	}
	return &metadata, len(rest) - len(tail) + end + 2, nil
}

// sourceContextConfig distinguishes complete ordered rule lists, including
// currently unmatched rules, while keeping each ownership comment compact.
func sourceContextConfig(values []string) string {
	encoded, _ := json.Marshal(values)
	return fmt.Sprintf("%x", sha256.Sum256(encoded))
}

// expandInsertContext replays the small, self-contained ownership record. The
// original call must be complete and cannot itself carry another CE record.
func expandInsertContext(metadata insertContextMetadata) (string, bindSite, error) {
	source := metadata.Original
	// Verify the enclosing call before scanning: this also prevents recursive
	// ownership records from entering canonicalizeSourceTemplate below.
	opening := strings.IndexByte(source, '(')
	if opening < 0 || contextClosingParen(stripCComments(source), opening+1) != len(source)-1 {
		return "", bindSite{}, fmt.Errorf("invalid CE original call")
	}
	sites, diagnostics := scanSourceSites("CE original", source, false)
	if len(diagnostics) != 0 {
		return "", bindSite{}, fmt.Errorf("%s", formatBindDiagnostic(diagnostics[0]))
	}
	if len(sites) != 1 || sites[0].loc[0] != 0 {
		return "", bindSite{}, fmt.Errorf("CE ownership requires exactly one original Trice call")
	}
	rules, err := parseContextRules(metadata.Rules)
	if err != nil {
		return "", bindSite{}, err
	}
	site := sites[0]
	registered := make(map[string]bool, len(metadata.Tags))
	for _, tag := range metadata.Tags {
		registered[tag] = true
	}
	format, selected, _ := selectContextRulesWithTags(site.format, rules, registered)
	if len(selected) == 0 {
		return "", bindSite{}, fmt.Errorf("CE ownership rules do not select the original call")
	}
	if err := enrichBindSite(source, stripCComments(source), &site, format, selected); err != nil {
		return "", bindSite{}, err
	}
	// Keep the selector and all original field spelling in the physical source.
	// The shared canonicalization hook supplies the selector-free final schema.
	physical, suffix := contextNewlineSuffix(source[site.loc[5]+1 : site.loc[6]-1])
	for _, rule := range selected {
		physical += rule.sourceFormat
	}
	edits := []sourceEdit{{start: site.loc[5], end: site.loc[6], replacement: `"` + physical + suffix + `"`}}
	originalType := TriceFmt{Type: source[site.loc[0]:site.loc[1]]}
	resolveTriceAlias(&originalType)
	if originalType.Type != site.macro {
		edits = append(edits, sourceEdit{start: site.loc[0], end: site.loc[1], replacement: site.macro})
	}
	if len(site.ce.expressions) != 0 {
		edits = append(edits, sourceEdit{start: site.ce.end - 1, end: site.ce.end - 1, replacement: ", " + strings.Join(site.ce.expressions, ", ")})
	}
	sort.Slice(edits, func(i, j int) bool { return edits[i].start < edits[j].start })
	expanded, err := applySourceEdits(source, edits)
	return expanded, site, err
}

// canonicalizeInsertContext verifies the actual call against its replayed
// provenance and supplies the final schema to every ID/table reader. IDs and
// surrounding source layout are deliberately not part of CE ownership.
func canonicalizeInsertContext(entry *TriceFmt, rest string) error {
	metadata, _, err := readInsertContext(rest)
	if err != nil || metadata == nil {
		return err
	}
	expanded, final, err := expandInsertContext(*metadata)
	if err != nil {
		return fmt.Errorf("CE ownership: %w", err)
	}
	expected, diagnostics := scanSourceSites("CE expansion", expanded, false)
	if len(diagnostics) != 0 || len(expected) != 1 {
		return fmt.Errorf("CE ownership: expanded call is invalid")
	}
	close := contextClosingParen(rest, 0)
	args, err := splitTriceParametersUntilClosingBracket(stripCComments(rest[:close+1]))
	if err != nil {
		return err
	}
	expectedArgs, err := splitTriceParametersUntilClosingBracket(contextArgumentTail(final.ce.arguments))
	if err != nil {
		return err
	}
	// The splitter trims outer whitespace. Comments do not change C semantics;
	// expressions themselves must still match, so edits never get overwritten.
	if entry.Type != final.macro || entry.Strg != expected[0].format || !slices.Equal(args, expectedArgs) {
		return fmt.Errorf("CE-expanded call was edited; restore it, then run clean with the original -ce options before editing")
	}
	entry.Strg = final.format
	return nil
}

// transformSourceContext either creates owned expansions or restores them.
// Existing ownership is validated even during clean's preflight-only pass.
func transformSourceContext(w io.Writer, path, source string, rules []contextRule, clean, restore bool) (string, error) {
	if hasTriceBindSidecarInclude(source) {
		return source, nil
	}
	sites, diagnostics := scanSourceSites(path, source, false)
	if len(diagnostics) != 0 {
		return "", fmt.Errorf("%s", formatBindDiagnostic(diagnostics[0]))
	}
	var edits []sourceEdit
	active := stripCComments(source)
	owned := 0 // Every reserved comment must remain attached to a parsed call.
	for _, site := range sites {
		metadata, end, err := readInsertContext(source[site.loc[6]:])
		if err != nil {
			return "", fmt.Errorf("%s:%d: %w", path, site.line, err)
		}
		if metadata != nil {
			owned++
			if metadata.Config != sourceContextConfig(ContextEnrichment) {
				return "", fmt.Errorf("%s:%d: CE rules differ; run clean with the original -ce options in the original order first", path, site.line)
			}
			if restore {
				original := metadata.Original
				// Source ownership restores spelling and arity; ordinary ID clean
				// still removes lowercase IDs and zeroes uppercase ID wrappers.
				originalSites, _ := scanSourceSites(path, original, false)
				old := originalSites[0]
				if old.loc[3] != old.loc[4] {
					original, _ = cleanID(original, 0, old.loc[:], bindSiteFormat(old))
				}
				edits = append(edits, sourceEdit{start: site.loc[0], end: site.loc[6] + end, replacement: original})
			}
			continue
		}
		if clean {
			continue // Never remove an unmarked, possibly hand-written suffix.
		}
		if strings.TrimSpace(active[site.loc[0]:site.loc[1]]) == "" {
			continue // Do not introduce nested C comments into documented calls.
		}
		_, selected, duplicates := selectContextRules(site.format, rules)
		if len(selected) == 0 {
			continue
		}
		// Preserve the exact original call, including comments in its arguments.
		close := contextClosingParen(source, site.loc[2])
		if close < 0 {
			return "", fmt.Errorf("%s:%d: CE call has no closing parenthesis", path, site.line)
		}
		end = close + 1
		metadata = &insertContextMetadata{Version: 1, Original: source[site.loc[0]:end], Config: sourceContextConfig(ContextEnrichment)}
		for _, rule := range selected {
			metadata.Rules = append(metadata.Rules, rule.option)
			if _, known := contextSelector(rule.selector); known && !slices.Contains(metadata.Tags, rule.selector) {
				metadata.Tags = append(metadata.Tags, rule.selector)
			}
		}
		expanded, _, err := expandInsertContext(*metadata)
		if err != nil {
			return "", fmt.Errorf("%s:%d: CE: %w", path, site.line, err)
		}
		encoded, _ := json.Marshal(metadata) // This struct contains only JSON-safe primitive values.
		expanded += " " + insertContextPrefix + base64.RawStdEncoding.EncodeToString(encoded) + " */"
		edits = append(edits, sourceEdit{start: site.loc[0], end: end, replacement: expanded})
		if len(duplicates) != 0 {
			fmt.Fprintf(w, "%s:%d: warning: duplicate CE selector(s) %s; each selector is applied once\n", path, site.line, strings.Join(duplicates, ", "))
		}
	}
	// A detached marker must fail before a fresh insert can accidentally append
	// another extension. Ignore marker-like text inside C string literals and
	// explicitly excluded instrumentation regions.
	visible := maskTriceInsertDisabledRegions(source)
	markers := 0
	for offset := 0; offset < len(visible); {
		index := strings.Index(visible[offset:], insertContextPrefix)
		if index < 0 {
			break
		}
		offset += index
		if strings.TrimSpace(active[offset:offset+len(insertContextPrefix)]) == "" {
			markers++
		}
		offset += len(insertContextPrefix)
	}
	if markers != owned {
		return "", fmt.Errorf("%s: detached CE ownership comment; restore its position immediately after the generated call before insert/clean", path)
	}
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
		prepared, err := transformSourceContext(w, path, string(content), rules, clean, false)
		if err != nil {
			return err
		}
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
