// SPDX-License-Identifier: MIT

package id

import (
	"errors"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"strconv"

	"github.com/spf13/afero"
)

// selectCurrentLogEntries builds the exact TIL subset referenced by the
// current -src selection. It deliberately accepts only IDs already made
// authoritative by trice insert or trice bind; source text and format strings
// are never used to guess an ID.
func selectCurrentLogEntries(w io.Writer, fSys *afero.Afero, til TriceIDLookUp) (TriceIDLookUp, error) {
	inputs, diagnostics := collectBindInputs(w, fSys)
	plans := analyzeBindInputs(inputs)
	for index := range plans {
		diagnostics = append(diagnostics, plans[index].diagnostics...)
		plans[index].diagnostics = nil
	}

	// Project analysis assigns stable source ordinals to multiple direct sites
	// on one line and definition ordinals to sites in supported wrapper macros.
	// The generated sidecar uses those same logical categories. The selector
	// still consumes sidecar descriptors in source order so generated rebase
	// lines may move without becoming source identity.
	// A successfully generated sidecar already proves that advanced bind
	// constructs passed bind's semantic checks. Read-only selection therefore
	// needs ordinals and source ordering, but must not impose those bind-only
	// diagnostics on legacy insert-owned files.
	_, _ = analyzeBindProject(plans, false)
	for index := range plans {
		diagnostics = append(diagnostics, plans[index].diagnostics...)
		plans[index].diagnostics = nil
	}

	selected := make(TriceIDLookUp)
	for planIndex := range plans {
		plan := &plans[planIndex]
		// Project analysis operates on the logical source with generated rebase
		// includes removed. Scan the same view here: physical helper lines shift
		// every following line and would otherwise make active sites appear to be
		// unrelated ID-free calls. Ordinary comments remain present in cleanSource
		// and therefore retain the legacy-insert diagnostics required below.
		cleanSource, _, artifactDiagnostics, _ := stripBindRebaseArtifacts(plan.path, plan.original)
		diagnostics = append(diagnostics, artifactDiagnostics...)
		allSites, _ := scanGenerateSites(plan.path, string(cleanSource))
		switch plan.class {
		case bindFileInsert:
			for _, site := range allSites {
				if site.wasExplicit {
					diagnostics = append(diagnostics, addCurrentLogSite(selected, til, plan.path, site, site.id)...)
				} else {
					diagnostics = append(diagnostics, unresolvedCurrentLogSite(plan.path, site))
				}
			}
		case bindFileBound:
			if len(plan.sites) != 0 {
				ids, sidecarDiagnostics := readCurrentBindIDs(fSys, plan)
				diagnostics = append(diagnostics, sidecarDiagnostics...)
				if len(sidecarDiagnostics) == 0 {
					if len(ids) != len(plan.sites) {
						diagnostics = append(diagnostics, bindDiagnostic{
							path: plan.path,
							message: fmt.Sprintf(
								"current Trice source has %d active site(s), but sidecar %s has %d numeric descriptor(s); run trice bind before trice generate -logC",
								len(plan.sites), plan.sidecarName, len(ids),
							),
						})
					} else {
						for siteIndex, site := range plan.sites {
							diagnostics = append(diagnostics, addCurrentLogSite(selected, til, plan.path, site, ids[siteIndex])...)
						}
					}
				}
			}
			// A bound source can still contain Trice calls in ordinary C comments.
			// Explicit calls carry an independent, authoritative numeric ID. An
			// ID-free commented call cannot be mapped by bind because it has no
			// active preprocessor site; report it instead of silently omitting a
			// source-visible format from the requested current table.
			for _, site := range allSites {
				if site.wasExplicit {
					diagnostics = append(diagnostics, addCurrentLogSite(selected, til, plan.path, site, site.id)...)
				} else if !bindSiteIsActive(site, plan.sites) && !currentLogSiteIsInherentlyUnsupported(site) {
					diagnostics = append(diagnostics, unresolvedCurrentLogSite(plan.path, site))
				}
			}
		case bindFileMixed:
			// analyzeBindFile already emitted the precise ownership diagnostic.
		default:
			for _, site := range allSites {
				if site.wasExplicit {
					diagnostics = append(diagnostics, addCurrentLogSite(selected, til, plan.path, site, site.id)...)
				} else {
					diagnostics = append(diagnostics, unresolvedCurrentLogSite(plan.path, site))
				}
			}
		}
	}

	if len(diagnostics) != 0 {
		return nil, currentLogDiagnosticsError(w, diagnostics)
	}
	if len(selected) == 0 {
		return nil, currentLogDiagnosticsError(w, []bindDiagnostic{{
			message: "no resolved current Trice sites were found; check -src and run trice insert or trice bind before trice generate -logC",
		}})
	}
	return selected, nil
}

// currentLogSiteIsInherentlyUnsupported permits an ID-free call in an ordinary
// C comment to remain documentation when -logC cannot represent its format or
// Trice family at all. This exception belongs only to local table generation:
// bind and legacy insert keep their existing comment behavior, and a supported
// commented Trice still needs an authoritative explicit ID.
func currentLogSiteIsInherentlyUnsupported(site bindSite) bool {
	entry := TriceFmt{Type: site.macro, Strg: site.format}
	resolveTriceAlias(&entry)
	defaultBitWidth, err := strconv.Atoi(DefaultTriceBitWidth)
	if err != nil {
		return false
	}
	_, bitWidth, _ := computeLogValues(entry, defaultBitWidth)
	return logFeatureCondition(entry, bitWidth) == "0"
}

// bindSiteIsActive distinguishes an ID-free source site already covered by a
// freshly generated sidecar from an ID-free call seen only in a C comment.
// Physical line, column, macro, and format are all parser-derived, so no new
// identity or format-based ID heuristic is introduced here.
func bindSiteIsActive(candidate bindSite, active []bindSite) bool {
	for _, site := range active {
		if candidate.line == site.line && candidate.column == site.column && candidate.macro == site.macro && candidate.format == site.format {
			return true
		}
	}
	return false
}

// unresolvedCurrentLogSite reports a textual current site for which neither
// source nor a bind sidecar supplies a numeric identity.
func unresolvedCurrentLogSite(path string, site bindSite) bindDiagnostic {
	return bindDiagnostic{
		path:    path,
		line:    site.line,
		message: "Trice site has no resolved non-zero ID; run trice insert or trice bind before trice generate -logC",
	}
}

// readCurrentBindIDs validates the owner sidecar and returns its numeric site
// descriptors in logical source order. Rebase helper files are build details;
// the owner sidecar remains the sole numeric authority needed by -logC.
func readCurrentBindIDs(fSys *afero.Afero, plan *bindFilePlan) ([]TriceID, []bindDiagnostic) {
	if plan.key == "" || plan.sidecarName == "" {
		return nil, []bindDiagnostic{{
			path:    plan.path,
			message: "ID-free Trice sites have no File Key include; run trice bind before trice generate -logC",
		}}
	}

	sidecarPath := filepath.Join(BindDir, plan.sidecarName)
	content, err := fSys.ReadFile(sidecarPath)
	if errors.Is(err, os.ErrNotExist) {
		return nil, []bindDiagnostic{{
			path: plan.path,
			message: fmt.Sprintf(
				"Trice bind sidecar %s is missing; run trice bind or pass its directory with -bindDir before trice generate -logC",
				sidecarPath,
			),
		}}
	}
	if err != nil {
		return nil, []bindDiagnostic{{path: sidecarPath, message: "cannot read Trice bind sidecar: " + err.Error()}}
	}
	if diagnostics := validateExistingSidecar(sidecarPath, plan.key, content); len(diagnostics) != 0 {
		return nil, diagnostics
	}

	historical := parseBindHistoricalSites(content, plan.key)
	ids := make([]TriceID, 0, len(historical))
	for _, site := range historical {
		ids = append(ids, site.id)
	}
	return ids, nil
}

// addCurrentLogSite verifies both parts of a source site's semantic identity
// against the primary TIL before adding it. A duplicate ID is harmless only
// when it denotes the same TIL entry.
func addCurrentLogSite(selected, til TriceIDLookUp, path string, site bindSite, id TriceID) []bindDiagnostic {
	if id <= 0 {
		return []bindDiagnostic{{path: path, line: site.line, message: "Trice site has no resolved non-zero ID; run trice insert or trice bind before trice generate -logC"}}
	}
	want := TriceFmt{Type: site.macro, Strg: site.format}
	resolveTriceAlias(&want)
	got, ok := til[id]
	if !ok {
		return []bindDiagnostic{{path: path, line: site.line, message: fmt.Sprintf("Trice ID %d is absent from %s; run trice insert or trice bind before trice generate -logC", id, FnJSON)}}
	}
	if got.Type != want.Type || got.Strg != want.Strg {
		return []bindDiagnostic{{
			path: path,
			line: site.line,
			message: fmt.Sprintf(
				"Trice ID %d does not match %s (source %s %q, TIL %s %q); run trice insert or trice bind before trice generate -logC",
				id, FnJSON, want.Type, want.Strg, got.Type, got.Strg,
			),
		}}
	}
	if previous, exists := selected[id]; exists && (previous.Type != got.Type || previous.Strg != got.Strg) {
		return []bindDiagnostic{{path: path, line: site.line, message: fmt.Sprintf("Trice ID %d resolves to conflicting current entries", id)}}
	}
	selected[id] = got
	return nil
}

// currentLogDiagnosticsError prints every deterministic compiler-style
// diagnostic and returns one summary error. No generated file has been opened
// at this point, which guarantees that fatal selection errors leave no partial
// regular output.
func currentLogDiagnosticsError(w io.Writer, diagnostics []bindDiagnostic) error {
	diagnostics = sortedBindDiagnostics(diagnostics)
	for _, diagnostic := range diagnostics {
		fmt.Fprintln(w, formatBindDiagnostic(diagnostic))
	}
	return fmt.Errorf("trice generate -logC failed with %d error(s)", len(diagnostics))
}
