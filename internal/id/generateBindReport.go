// SPDX-License-Identifier: MIT

package id

import (
	"bytes"
	"errors"
	"fmt"
	"io"
	"path/filepath"
	"regexp"
	"sort"
	"strings"

	"github.com/spf13/afero"
)

// bindReportHelperName recognizes the filename namespace, not ownership by
// itself. The helper bytes and its owner's declarations are checked separately.
var bindReportHelperName = regexp.MustCompile(`^(trice_[A-Za-z0-9_]+_(K[0-9A-F]{16}))_(R[0-9]+)_(begin|end)\.h$`)

// bindReportIncludeName accepts literal quoted and angle-bracket includes.
// Macro expressions containing a filename string are not physical references
// to that file without evaluating the preprocessor, which this report avoids.
var bindReportIncludeName = regexp.MustCompile(`(?m)^[\t ]*#[\t ]*include\b[\t ]*(?:"([^"\r\n]+)"|<([^>\r\n]+)>)`)

// bindReportArtifact retains inspection evidence until the complete scan has
// succeeded. Unrecognized files remain visible and are never cleanup candidates.
type bindReportArtifact struct {
	name, owner, key string
	kind, problem    string
}

// generateBindReport inventories only the selected directory's immediate
// entries. Source includes establish scan references, not compiler use or safe
// deletion. No Bind planning, ID assignment or dictionary loading is performed.
func generateBindReport(w io.Writer, fSys *afero.Afero) error {
	if strings.TrimSpace(BindDir) == "" {
		return errors.New("trice generate: -genDir must not be empty")
	}
	if w == nil {
		w = io.Discard
	}
	entries, err := fSys.ReadDir(BindDir)
	if err != nil {
		return fmt.Errorf("trice generate -bindReport: cannot read generated directory %s: %w", BindDir, err)
	}
	roots := append([]string(nil), Srcs...)
	if len(roots) == 0 {
		roots = []string{"."}
	}
	sort.Strings(roots)
	// Unlike Bind's permissive missing-root handling, a report must not silently
	// describe a partial scan when the user misspelled a requested source path.
	for _, root := range roots {
		if _, err := fSys.Stat(root); err != nil {
			return fmt.Errorf("trice generate -bindReport: cannot inspect source root %s: %w", root, err)
		}
	}
	// CompactSrcs normally supplies the default in the CLI. Keep direct callers
	// consistent while restoring their selection after this read-only operation.
	previous := Srcs
	Srcs = roots
	inputs, diagnostics := collectBindInputs(io.Discard, fSys)
	Srcs = previous
	if len(diagnostics) != 0 {
		var messages []string
		for _, diagnostic := range sortedBindDiagnostics(diagnostics) {
			messages = append(messages, formatBindDiagnostic(diagnostic))
		}
		return fmt.Errorf("trice generate -bindReport: incomplete source scan: %s", strings.Join(messages, "; "))
	}

	// A source may reactivate its sidecar several times. Deduplicate per file;
	// different physical sources claiming one key remain ambiguous evidence.
	references := make(map[string][]string)
	keyOwners := make(map[string][]string)
	for _, input := range inputs {
		// Mask comments before extracting names as well as finding directives:
		// an unrelated include's trailing comment is not ownership evidence.
		masked := stripCComments(string(input.data))
		for _, include := range bindReportIncludeName.FindAllStringSubmatch(masked, -1) {
			name := include[1]
			if name == "" {
				name = include[2]
			}
			quoted := `"` + name + `"`
			sidecar := bindSidecarName.FindStringSubmatch(quoted)
			if len(sidecar) == 3 && sidecar[0] == quoted {
				references[name] = append(references[name], filepath.ToSlash(input.path))
				keyOwners[sidecar[2]] = append(keyOwners[sidecar[2]], filepath.ToSlash(input.path))
			} else if bindReportHelperName.MatchString(name) {
				// Record the literal helper reference independently of ownership
				// comments; the actual helper bytes are checked below.
				references[name] = append(references[name], filepath.ToSlash(input.path))
			}
		}
	}
	for name, paths := range references {
		references[name] = compactSortedStrings(paths)
	}
	for key, paths := range keyOwners {
		keyOwners[key] = compactSortedStrings(paths)
	}

	var artifacts []bindReportArtifact
	known := make(map[string]bool)
	validOwners := make(map[string]bool)
	for _, entry := range entries {
		artifact := bindReportArtifact{name: entry.Name(), kind: "unclassified"}
		known[artifact.name] = true
		if entry.Mode().IsRegular() {
			quoted := `"` + artifact.name + `"`
			match := bindSidecarName.FindStringSubmatch(quoted)
			helper := bindReportHelperName.FindStringSubmatch(artifact.name)
			if len(match) == 3 && match[0] == quoted {
				artifact.kind, artifact.owner, artifact.key = "sidecar", artifact.name, match[2]
			} else if len(helper) == 5 {
				artifact.kind, artifact.owner, artifact.key = "rebase "+helper[4], helper[1]+".h", helper[2]
			}
			if artifact.key != "" {
				path := filepath.Join(BindDir, artifact.name)
				content, err := fSys.ReadFile(path)
				if err != nil {
					return fmt.Errorf("trice generate -bindReport: cannot read artifact %s: %w", path, err)
				}
				if artifact.kind == "sidecar" {
					problems := validateExistingSidecar(path, artifact.key, []byte(stripCComments(string(content))))
					if len(problems) != 0 {
						artifact.problem = problems[0].message
					} else {
						validOwners[artifact.name] = true
					}
				} else {
					expected := renderBindRebaseArtifact(artifact.owner, artifact.key, artifact.key+"_"+helper[3], helper[4])
					if !bytes.Equal(content, expected.content) {
						artifact.problem = "content differs from the generated helper; ownership unverified"
					}
				}
			}
		} else if entry.IsDir() {
			artifact.kind = "directory (not inspected recursively)"
		} else {
			artifact.kind = "non-regular entry (not followed)"
		}
		artifacts = append(artifacts, artifact)
	}
	// Report referenced but missing artifacts as well as existing inventory.
	for name := range references {
		if !known[name] {
			artifacts = append(artifacts, bindReportArtifact{name: name, kind: "missing", problem: "referenced artifact is absent from -genDir"})
		}
	}
	sort.Slice(artifacts, func(i, j int) bool { return artifacts[i].name < artifacts[j].name })

	var report strings.Builder
	fmt.Fprintf(&report, "Bind artifact report: %s\n", filepath.ToSlash(filepath.Clean(BindDir)))
	fmt.Fprintf(&report, "Source roots: %s\n", strings.Join(roots, ", "))
	fmt.Fprintf(&report, "Exclusions: %s (generated directory and hidden paths also excluded)\n", strings.Join(ExcludeSrcs, ", "))
	fmt.Fprintf(&report, "Scanned %d source file(s); %d directory entry/entries.\n", len(inputs), len(entries))
	report.WriteString("References are physical includes; preprocessor conditions and compiler use are not evaluated.\n")
	report.WriteString("An owner not found in this scan is not proof of an unused file. No files are changed or deleted.\n\n")
	for _, artifact := range artifacts {
		fmt.Fprintf(&report, "%s [%s]\n", artifact.name, artifact.kind)
		if artifact.key != "" {
			fmt.Fprintf(&report, "  File Key: %s; owner sidecar: %s\n", artifact.key, artifact.owner)
			if len(keyOwners[artifact.key]) > 1 {
				fmt.Fprintf(&report, "  AMBIGUOUS: File Key claimed by multiple sources: %s\n", strings.Join(keyOwners[artifact.key], ", "))
			}
			if artifact.kind != "sidecar" && !validOwners[artifact.owner] {
				report.WriteString("  UNVERIFIED: owner sidecar is missing or has invalid declarations.\n")
			}
		}
		if artifact.problem != "" {
			fmt.Fprintf(&report, "  UNVERIFIED: %s\n", artifact.problem)
		}
		if paths := references[artifact.name]; len(paths) != 0 {
			fmt.Fprintf(&report, "  Referenced in scan: %s\n", strings.Join(paths, ", "))
		} else if artifact.key != "" {
			report.WriteString("  Not referenced in selected scan; owner source not established for this artifact.\n")
		}
		if artifact.owner != "" && artifact.owner != artifact.name && len(references[artifact.owner]) != 0 {
			fmt.Fprintf(&report, "  Owner sidecar referenced by: %s\n", strings.Join(references[artifact.owner], ", "))
		}
		if artifact.kind == "unclassified" {
			report.WriteString("  Ownership not established; may be user-managed (for example an ABC selection header).\n")
		}
	}
	_, err = io.WriteString(w, report.String())
	return err
}
