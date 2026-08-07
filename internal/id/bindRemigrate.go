// SPDX-License-Identifier: MIT

package id

import (
	"bytes"
	"errors"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"sort"
	"strings"

	"github.com/spf13/afero"
)

// bindRemigratePlan holds the exact source replacement and generated sidecar
// deletion that return one bind-owned physical file to the clean state.
type bindRemigratePlan struct {
	path        string
	info        os.FileInfo
	original    []byte
	final       []byte
	sidecarPath string
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

	writes := buildBindRemigrationWrites(plans)
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

		owners[sidecarPath] = append(owners[sidecarPath], input.path)
		plans = append(plans, bindRemigratePlan{
			path:        input.path,
			info:        input.info,
			original:    input.data,
			final:       removeBindSidecarIncludes(input.data, includes),
			sidecarPath: sidecarPath,
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

// buildBindRemigrationWrites orders source replacements before sidecar
// deletions. The shared commit helper snapshots every destination first and
// rolls earlier changes back if any later operation fails.
func buildBindRemigrationWrites(plans []bindRemigratePlan) []bindWrite {
	writes := make([]bindWrite, 0, len(plans)*2)
	for _, plan := range plans {
		if !bytes.Equal(plan.original, plan.final) {
			writes = append(writes, bindWrite{path: plan.path, data: plan.final, perm: plan.info.Mode(), kind: "source"})
		}
	}
	for _, plan := range plans {
		writes = append(writes, bindWrite{path: plan.sidecarPath, kind: "sidecar-delete", remove: true})
	}
	return writes
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
