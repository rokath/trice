// SPDX-License-Identifier: MIT

package id

import (
	"fmt"
	"io"
	"os"
	"path/filepath"
	"sort"
	"strings"

	"github.com/rokath/trice/internal/fmtspec"
	"github.com/rokath/trice/pkg/ant"
	"github.com/spf13/afero"
)

// FieldsDir is insert's build output directory for the current-run field list.
// Bind uses its existing BindDir so each build keeps its generated facts together.
var FieldsDir = DefaultGenDir

// canonicalizeSourceTemplate resolves names without rewriting user-owned string
// literals. Keeping the source spelling is what makes insert/clean reversible.
func canonicalizeSourceTemplate(t *TriceFmt, rest string) error {
	if t.isSAlias() || !strings.ContainsAny(t.Strg, "{}") {
		return nil
	}
	args, err := splitTriceParametersUntilClosingBracket(rest)
	if err != nil {
		return fmt.Errorf("structured template arguments: %w", err)
	}
	template, err := fmtspec.ParseTemplate(t.Strg, args)
	if err != nil {
		return err
	}
	if err := ValidateStructuredFields(*t, template); err != nil {
		return err
	}
	// A float32 wrapper supplies different IEEE bits than the float64 decoder
	// expects, even when C can widen the wrapper's integer transport value.
	for _, field := range template.Fields {
		if field.Argument < len(args) && field.Spec.Kind == fmtspec.KindFloat && triceValueBitWidth(t.Type, len(template.Specs)) == 64 && isTopLevelCall(args[field.Argument], "aFloat") {
			return fmt.Errorf("field %q in a 64-bit Trice requires aDouble(), not aFloat()", field.Name)
		}
	}
	t.Strg = template.Canonical
	return nil
}

// ValidateStructuredFields enforces the same supported record families during
// instrumentation and when loading externally supplied or historical TIL data.
// Dynamic buffers have repeated elements, not the scalar schema promised by M19.
func ValidateStructuredFields(t TriceFmt, template fmtspec.Template) error {
	if len(template.Fields) == 0 {
		return nil
	}
	switch triceTypeCategory(t.Type) {
	case "B", "F", "C", "Assert":
		return fmt.Errorf("%s does not support named structured fields; use scalar Trices or triceS/triceN, or keep the classic message format", t.Type)
	case "S", "N":
		if len(template.Specs) != 1 {
			return fmt.Errorf("%s requires exactly one string conversion", t.Type)
		}
		if template.Fields[0].Spec.Kind != fmtspec.KindString {
			return fmt.Errorf("%s named structured string fields require %%s; other string presentations remain available in classic messages", t.Type)
		}
	}
	return validateTriceFormatSpecifierKinds(t, template.Specs)
}

// collectInsertFields validates all templates before the first source write.
// It deliberately runs even with -cache: the registry describes this invocation,
// including unchanged sites, rather than just cache misses or historical IDs.
func collectInsertFields(w io.Writer, fSys *afero.Afero) (map[string]int, error) {
	if strings.TrimSpace(FieldsDir) == "" {
		return nil, fmt.Errorf("trice insert: -genDir must not be empty")
	}
	counts := make(map[string]int)
	admin := &ant.Admin{Trees: Srcs, ExcludeTrees: ExcludeSrcs, MatchingFileName: isSourceFile}
	admin.Action = func(_ io.Writer, fs *afero.Afero, path string, _ os.FileInfo, a *ant.Admin) error {
		content, err := fs.ReadFile(path)
		if err != nil {
			return err
		}
		if hasTriceBindSidecarInclude(string(content)) {
			return nil
		}
		sites, diagnostics := scanSourceSites(path, string(content), false)
		if len(diagnostics) != 0 {
			return fmt.Errorf("%s", formatBindDiagnostic(diagnostics[0]))
		}
		local := make(map[string]int)
		for _, site := range sites {
			if isSAliasEncodedString(site.format) {
				continue
			}
			if strings.ContainsAny(site.format, "{}") && !SkipAdditionalChecks {
				trice := TriceFmt{Type: site.macro, Strg: site.format}
				resolveTriceAlias(&trice)
				if err := evaluateTriceParameterCount(trice, site.line, string(content)[site.loc[6]:]); err != nil {
					return fmt.Errorf("%s:%d: %w", path, site.line, err)
				}
			}
			template, err := fmtspec.ParseTemplate(site.format, nil)
			if err != nil {
				return fmt.Errorf("%s:%d: %w", path, site.line, err)
			}
			for _, field := range template.Fields {
				local[field.Name]++
			}
		}
		a.Mutex.Lock()
		defer a.Mutex.Unlock()
		for name, count := range local {
			counts[name] += count
		}
		return nil
	}
	if err := admin.Walk(w, fSys); err != nil {
		return nil, err
	}
	return counts, nil
}

// renderFieldRegistry sorts rare names first and never includes host metadata.
// A zero-site invocation yields an empty file, removing stale prior-run counts.
func renderFieldRegistry(counts map[string]int) []byte {
	names := make([]string, 0, len(counts))
	for name := range counts {
		names = append(names, name)
	}
	sort.Slice(names, func(i, j int) bool {
		if counts[names[i]] != counts[names[j]] {
			return counts[names[i]] < counts[names[j]]
		}
		return names[i] < names[j]
	})
	var result strings.Builder
	for _, name := range names {
		fmt.Fprintf(&result, "%8d %s\n", counts[name], name)
	}
	return []byte(result.String())
}

// writeInsertFields replaces the registry only after successful instrumentation.
func writeInsertFields(fSys *afero.Afero, counts map[string]int) error {
	if DryRun {
		return nil
	}
	if err := fSys.MkdirAll(FieldsDir, 0o755); err != nil {
		return err
	}
	path := filepath.Join(FieldsDir, "trice-fields.txt")
	return atomicWriteFile(fSys, path, renderFieldRegistry(counts), fileWritePerm(fSys, path, 0o644))
}
