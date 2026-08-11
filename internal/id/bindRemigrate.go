// SPDX-License-Identifier: MIT

package id

import (
	"bytes"
	"encoding/json"
	"errors"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"regexp"
	"sort"
	"strconv"
	"strings"

	"github.com/spf13/afero"
)

var (
	// bindRemigrationSiteDefinition extracts the generated key, physical line,
	// and stable ID needed to validate and relocate li.json entries.
	bindRemigrationSiteDefinition = regexp.MustCompile(`(?m)^[\t ]*#[\t ]*define[\t ]+TRICE_BIND_SITE_(K[0-9A-F]{16})_L([0-9]+)[\t ]+TRICE_BIND_(AUTO|REPLACE),[\t ]+(iD|id|Id|ID)\(([0-9]+)u\)`)
	// bindRemigrationAdvancedDefinition extracts stable IDs owned by wrapper
	// definitions and direct counter-selected locations.
	bindRemigrationAdvancedDefinition = regexp.MustCompile(`(?m)^[\t ]*#[\t ]*define[\t ]+TRICE_BIND_ID_(?:DEFINITION|LOCATION)_(K[0-9A-F]{16})_L([0-9]+)_O[0-9]+[\t ]+([0-9]+)u\b`)
)

// bindRemigratePlan holds the exact source replacement and generated sidecar
// deletion that return one bind-owned physical file to the clean state.
type bindRemigratePlan struct {
	path         string
	info         os.FileInfo
	original     []byte
	final        []byte
	sidecarPath  string
	removedLines []int           // removedLines identifies every generated line removed from the Bound source.
	siteLines    map[TriceID]int // siteLines maps each generated ID to its validated Bound line.
}

// SubCmdIdRemigrateBindToClean removes validated bind ownership artifacts while
// preserving every ID-free Trice call and zero placeholder byte-for-byte.
//
// The operation deliberately reuses the bind source walker, comment-aware
// include scanner, sidecar validator, deterministic diagnostics, and
// transactional commit path. It is exported only for the repository workflow
// helper; it is not a public trice CLI subcommand.
func SubCmdIdRemigrateBindToClean(w io.Writer, fSys *afero.Afero) error {
	if w == nil {
		w = io.Discard
	}
	if fSys == nil || fSys.Fs == nil {
		return errors.New("trice bind remigration: nil filesystem")
	}
	if strings.TrimSpace(BindDir) == "" {
		return errors.New("trice bind remigration: -bindDir must not be empty")
	}

	inputs, diagnostics := collectBindInputs(w, fSys)
	plans, planDiagnostics := prepareBindRemigration(fSys, inputs)
	diagnostics = append(diagnostics, planDiagnostics...)
	if len(diagnostics) > 0 {
		return reportBindRemigrationDiagnostics(w, diagnostics)
	}

	writes, err := buildBindRemigrationWrites(fSys, plans)
	if err != nil {
		return err
	}
	if Verbose {
		for _, plan := range plans {
			fmt.Fprintf(w, "%s: remove bind include(s) and sidecar %s\n", plan.path, plan.sidecarPath)
		}
	}
	if DryRun {
		if Verbose {
			fmt.Fprintf(w, "dry-run: %d file change(s) suppressed\n", len(writes))
		}
		return nil
	}
	return commitBindWrites(fSys, writes)
}

// prepareBindRemigration validates complete source-to-sidecar ownership before
// planning any edit, so one ambiguous file prevents all regular output writes.
func prepareBindRemigration(fSys *afero.Afero, inputs []bindFileInput) ([]bindRemigratePlan, []bindDiagnostic) {
	var (
		plans       []bindRemigratePlan
		diagnostics []bindDiagnostic
		owners      = make(map[string][]string)
	)

	for _, input := range inputs {
		includes := scanBindIncludes(string(input.data))
		blocks, blockDiagnostics := scanBindRebaseArtifacts(input.path, input.data)
		if len(blockDiagnostics) > 0 {
			diagnostics = append(diagnostics, blockDiagnostics...)
			continue
		}
		identities := make(map[string]string)
		for _, include := range includes {
			if include.isSidecar {
				identities[include.name] = include.key
			}
		}
		if len(identities) == 0 {
			continue
		}

		names := make([]string, 0, len(identities))
		for name := range identities {
			names = append(names, name)
		}
		sort.Strings(names)
		if len(names) != 1 {
			diagnostics = append(diagnostics, bindDiagnostic{
				path:    input.path,
				message: "cannot remigrate a file that references multiple distinct Trice bind sidecars: " + strings.Join(names, ", "),
			})
			continue
		}

		name := names[0]
		key := identities[name]
		sidecarPath := filepath.Join(BindDir, name)
		content, err := fSys.ReadFile(sidecarPath)
		if errors.Is(err, os.ErrNotExist) {
			diagnostics = append(diagnostics, bindDiagnostic{path: input.path, message: fmt.Sprintf("referenced Trice bind sidecar %s does not exist", sidecarPath)})
			continue
		}
		if err != nil {
			diagnostics = append(diagnostics, bindDiagnostic{path: sidecarPath, message: "cannot read referenced Trice bind sidecar: " + err.Error()})
			continue
		}
		if sidecarDiagnostics := validateExistingSidecar(sidecarPath, key, content); len(sidecarDiagnostics) > 0 {
			diagnostics = append(diagnostics, sidecarDiagnostics...)
			continue
		}
		siteLines, err := parseBindRemigrationSiteLines(content, key)
		if err != nil {
			diagnostics = append(diagnostics, bindDiagnostic{path: sidecarPath, message: err.Error()})
			continue
		}
		removedLineSet := make(map[int]bool)
		for _, include := range includes {
			if include.isSidecar {
				removedLineSet[include.line] = true
			}
		}
		for _, block := range blocks {
			for position := block.start; position < block.end; position = lineEndIncludingNewline(string(input.data), position) {
				removedLineSet[sourceLine(string(input.data), position)] = true
			}
		}
		removedLines := make([]int, 0, len(removedLineSet))
		for line := range removedLineSet {
			removedLines = append(removedLines, line)
		}
		sort.Ints(removedLines)

		cleaned, _, _ := stripBindRebaseArtifacts(input.path, input.data)
		cleanIncludes := scanBindIncludes(string(cleaned))

		owners[sidecarPath] = append(owners[sidecarPath], input.path)
		plans = append(plans, bindRemigratePlan{
			path:         input.path,
			info:         input.info,
			original:     input.data,
			final:        removeBindSidecarIncludes(cleaned, cleanIncludes),
			sidecarPath:  sidecarPath,
			removedLines: removedLines,
			siteLines:    siteLines,
		})
	}

	for sidecarPath, ownerPaths := range owners {
		ownerPaths = compactSortedStrings(ownerPaths)
		if len(ownerPaths) > 1 {
			diagnostics = append(diagnostics, bindDiagnostic{
				path:    sidecarPath,
				message: "cannot remigrate sidecar with multiple owner files: " + strings.Join(ownerPaths, ", "),
			})
		}
	}
	if len(diagnostics) > 0 {
		return nil, diagnostics
	}
	return plans, nil
}

// parseBindRemigrationSiteLines reads only generator-owned descriptor syntax.
// It does not parse user Trice calls or duplicate the shared source parser.
func parseBindRemigrationSiteLines(content []byte, expectedKey string) (map[TriceID]int, error) {
	result := make(map[TriceID]int)
	for _, match := range bindRemigrationSiteDefinition.FindAllSubmatch(content, -1) {
		if string(match[1]) != expectedKey {
			return nil, fmt.Errorf("site descriptor uses file key %s instead of %s", match[1], expectedKey)
		}
		line, err := strconv.Atoi(string(match[2]))
		if err != nil {
			return nil, fmt.Errorf("invalid site line %q", match[2])
		}
		value, err := strconv.Atoi(string(match[5]))
		if err != nil {
			return nil, fmt.Errorf("invalid site ID %q", match[5])
		}
		id := TriceID(value)
		if previous, exists := result[id]; exists && previous != line {
			return nil, fmt.Errorf("site ID %d occurs on multiple physical lines", id)
		}
		result[id] = line
	}
	for _, match := range bindRemigrationAdvancedDefinition.FindAllSubmatch(content, -1) {
		if string(match[1]) != expectedKey {
			return nil, fmt.Errorf("advanced descriptor uses file key %s instead of %s", match[1], expectedKey)
		}
		line, err := strconv.Atoi(string(match[2]))
		if err != nil {
			return nil, fmt.Errorf("invalid advanced descriptor line %q", match[2])
		}
		value, err := strconv.Atoi(string(match[3]))
		if err != nil {
			return nil, fmt.Errorf("invalid advanced descriptor ID %q", match[3])
		}
		id := TriceID(value)
		if previous, exists := result[id]; exists && previous != line {
			return nil, fmt.Errorf("site ID %d occurs on multiple physical lines", id)
		}
		result[id] = line
	}
	return result, nil
}

// removeBindSidecarIncludes deletes complete active sidecar include lines in
// reverse byte order, preserving every other byte and the original line endings.
func removeBindSidecarIncludes(source []byte, includes []bindInclude) []byte {
	result := append([]byte(nil), source...)
	for index := len(includes) - 1; index >= 0; index-- {
		include := includes[index]
		if !include.isSidecar {
			continue
		}
		result = append(result[:include.start], result[include.afterLine:]...)
	}
	return result
}

// buildBindRemigrationWrites plans source and location-list replacements before
// sidecar deletions. The shared commit helper snapshots every destination first
// and rolls earlier changes back if any later operation fails.
func buildBindRemigrationWrites(fSys *afero.Afero, plans []bindRemigratePlan) ([]bindWrite, error) {
	writes := make([]bindWrite, 0, len(plans)*2+1)
	for _, plan := range plans {
		if !bytes.Equal(plan.original, plan.final) {
			writes = append(writes, bindWrite{path: plan.path, data: plan.final, perm: plan.info.Mode(), kind: "source"})
		}
	}
	liWrite, err := buildBindRemigrationLocationWrite(fSys, plans)
	if err != nil {
		return nil, err
	}
	if liWrite != nil {
		writes = append(writes, *liWrite)
	}
	for _, plan := range plans {
		writes = append(writes, bindWrite{path: plan.sidecarPath, kind: "sidecar-delete", remove: true})
	}
	return writes, nil
}

// buildBindRemigrationLocationWrite restores Clean-source line numbers only
// when every generated descriptor still matches its current li.json entry.
func buildBindRemigrationLocationWrite(fSys *afero.Afero, plans []bindRemigratePlan) (*bindWrite, error) {
	if len(plans) == 0 || LIFnJSON == "" || LIFnJSON == "off" || LIFnJSON == "none" {
		return nil, nil
	}
	original, err := fSys.ReadFile(LIFnJSON)
	if errors.Is(err, os.ErrNotExist) {
		return nil, nil
	}
	if err != nil {
		return nil, fmt.Errorf("cannot read %s before bind remigration: %w", LIFnJSON, err)
	}
	locations := make(TriceIDLookUpLI)
	if len(bytes.TrimSpace(original)) > 0 {
		if err := json.Unmarshal(original, &locations); err != nil {
			return nil, fmt.Errorf("cannot parse %s before bind remigration: %w", LIFnJSON, err)
		}
	}

	changed := false
	for _, plan := range plans {
		locationFile := ToLIFile(plan.path)
		// Map iteration must not decide which stale-location diagnostic is
		// reported first, so descriptor IDs are validated in numeric order.
		siteIDs := make([]TriceID, 0, len(plan.siteLines))
		for id := range plan.siteLines {
			siteIDs = append(siteIDs, id)
		}
		sort.Slice(siteIDs, func(i, j int) bool { return siteIDs[i] < siteIDs[j] })
		for _, id := range siteIDs {
			boundLine := plan.siteLines[id]
			location, exists := locations[id]
			if !exists {
				return nil, fmt.Errorf("cannot remigrate %s: ID %d is missing from %s", plan.path, id, LIFnJSON)
			}
			if normalizeLocationPath(location.File) != locationFile || location.Line != boundLine {
				return nil, fmt.Errorf(
					"cannot remigrate %s: ID %d location is %s:%d, expected %s:%d",
					plan.path,
					id,
					location.File,
					location.Line,
					locationFile,
					boundLine,
				)
			}
			removedBefore := 0
			for _, removedLine := range plan.removedLines {
				if removedLine < boundLine {
					removedBefore++
				}
			}
			if removedBefore == 0 {
				continue
			}
			location.Line -= removedBefore
			if location.Line < 1 {
				return nil, fmt.Errorf("cannot remigrate %s: ID %d would receive invalid line %d", plan.path, id, location.Line)
			}
			locations[id] = location
			changed = true
		}
	}
	if !changed {
		return nil, nil
	}
	rendered, err := locations.toJSON()
	if err != nil {
		return nil, fmt.Errorf("cannot render %s after bind remigration: %w", LIFnJSON, err)
	}
	if bytes.Equal(original, rendered) {
		return nil, nil
	}
	return &bindWrite{
		path: LIFnJSON,
		data: rendered,
		perm: fileWritePerm(fSys, LIFnJSON, 0o666),
		kind: "li",
	}, nil
}

// reportBindRemigrationDiagnostics emits all stable diagnostics and returns one
// summary error suitable for the repository workflow command.
func reportBindRemigrationDiagnostics(w io.Writer, diagnostics []bindDiagnostic) error {
	diagnostics = sortedBindDiagnostics(diagnostics)
	for _, diagnostic := range diagnostics {
		fmt.Fprintln(w, formatBindDiagnostic(diagnostic))
	}
	return fmt.Errorf("trice bind remigration failed with %d error(s)", len(diagnostics))
}
