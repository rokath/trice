// SPDX-License-Identifier: MIT

package id

import (
	"bytes"
	cryptorand "crypto/rand"
	"encoding/hex"
	"errors"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"sort"
	"strings"
	"sync"

	"github.com/rokath/trice/pkg/ant"
	"github.com/spf13/afero"
)

var (
	// bindRandomReader is replaceable inside package tests while production always uses crypto/rand.Reader.
	bindRandomReader io.Reader = cryptorand.Reader
)

// SubCmdIdBind generates stable sidecars while keeping bind-owned user Trice calls ID-free.
func SubCmdIdBind(w io.Writer, fSys *afero.Afero) error {
	if w == nil {
		w = io.Discard
	}
	if fSys == nil || fSys.Fs == nil {
		return errors.New("trice bind: nil filesystem")
	}
	if err := validateBindOptions(); err != nil {
		return err
	}

	inputs, diagnostics := collectBindInputs(w, fSys)
	plans := analyzeBindInputs(inputs)
	for i := range plans {
		diagnostics = append(diagnostics, plans[i].diagnostics...)
		plans[i].diagnostics = nil
	}
	diagnostics = append(diagnostics, prepareBindPlans(fSys, plans)...)

	IDData.err = nil
	IDData.PreProcessing(w, fSys)
	initialIDs := make(map[TriceID]struct{}, len(IDData.idToTrice))
	for id := range IDData.idToTrice {
		initialIDs[id] = struct{}{}
	}
	diagnostics = append(diagnostics, assignBindIDs(w, plans, initialIDs)...)

	for i := range plans {
		if plans[i].class == bindFileBound && plans[i].key != "" {
			plans[i].sidecarContent = renderBindSidecar(&plans[i])
		}
	}
	if len(diagnostics) > 0 {
		return reportBindDiagnostics(w, diagnostics)
	}

	writes, err := buildBindWrites(fSys, plans)
	if err != nil {
		return err
	}
	printBindSummary(w, plans, writes)
	if DryRun {
		return nil
	}
	return commitBindWrites(fSys, writes)
}

// validateBindOptions rejects invalid semantic choices before any source or generated file can change.
func validateBindOptions() error {
	if DefaultStampSize != 0 && DefaultStampSize != 16 && DefaultStampSize != 32 {
		return fmt.Errorf("trice bind: invalid -defaultStampSize %d; expected 0, 16, or 32", DefaultStampSize)
	}
	switch SearchMethod {
	case "random", "upward", "downward":
	default:
		return fmt.Errorf("trice bind: invalid -IDMethod %q; expected random, upward, or downward", SearchMethod)
	}
	if strings.TrimSpace(BindDir) == "" {
		return errors.New("trice bind: -bindDir must not be empty")
	}
	return nil
}

// collectBindInputs uses the same concurrent ant walker and source-file predicate as insert.
func collectBindInputs(w io.Writer, fSys *afero.Afero) ([]bindFileInput, []bindDiagnostic) {
	var (
		inputs      = make(map[string]bindFileInput)
		diagnostics []bindDiagnostic
		mutex       sync.Mutex
	)
	exclusions := append([]string(nil), ExcludeSrcs...)
	exclusions = append(exclusions, filepath.Clean(BindDir))
	if absoluteBindDir, err := filepath.Abs(BindDir); err == nil {
		exclusions = append(exclusions, absoluteBindDir)
	}
	admin := &ant.Admin{
		Trees:            append([]string(nil), Srcs...),
		ExcludeTrees:     exclusions,
		Verbose:          Verbose,
		MatchingFileName: isSourceFile,
	}
	admin.Action = func(_ io.Writer, fileSystem *afero.Afero, path string, info os.FileInfo, _ *ant.Admin) error {
		content, err := fileSystem.ReadFile(path)
		mutex.Lock()
		defer mutex.Unlock()
		if err != nil {
			diagnostics = append(diagnostics, bindDiagnostic{path: path, message: "cannot read source: " + err.Error()})
			return nil
		}
		cleanPath := filepath.Clean(path)
		inputs[cleanPath] = bindFileInput{path: cleanPath, info: info, data: content}
		return nil
	}
	if err := admin.Walk(w, fSys); err != nil {
		diagnostics = append(diagnostics, bindDiagnostic{message: "source walk failed: " + err.Error()})
	}
	paths := make([]string, 0, len(inputs))
	for path := range inputs {
		paths = append(paths, path)
	}
	sort.Strings(paths)
	result := make([]bindFileInput, 0, len(paths))
	for _, path := range paths {
		result = append(result, inputs[path])
	}
	return result, diagnostics
}

// analyzeBindInputs performs independent classification in parallel and retains sorted plan order.
func analyzeBindInputs(inputs []bindFileInput) []bindFilePlan {
	plans := make([]bindFilePlan, len(inputs))
	var workers sync.WaitGroup
	workers.Add(len(inputs))
	for i := range inputs {
		i := i
		go func() {
			defer workers.Done()
			plans[i] = analyzeBindFile(inputs[i])
		}()
	}
	workers.Wait()
	return plans
}

// prepareBindPlans resolves stable keys, plans includes, validates ownership, and reads old sidecars.
func prepareBindPlans(fSys *afero.Afero, plans []bindFilePlan) []bindDiagnostic {
	var diagnostics []bindDiagnostic
	keyOwners := make(map[string][]string)
	for i := range plans {
		plan := &plans[i]
		if plan.key != "" {
			keyOwners[plan.key] = append(keyOwners[plan.key], plan.path)
		}
	}
	for key, owners := range keyOwners {
		owners = compactSortedStrings(owners)
		keyOwners[key] = owners
		if len(owners) > 1 {
			diagnostics = append(diagnostics, bindDiagnostic{message: fmt.Sprintf("file key %s belongs to multiple owner files: %s", key, strings.Join(owners, ", "))})
		}
	}

	for i := range plans {
		plan := &plans[i]
		if plan.class == bindFileBound && plan.key == "" && len(plan.sites) > 0 {
			key, err := newBindFileKey(keyOwners)
			if err != nil {
				diagnostics = append(diagnostics, bindDiagnostic{path: plan.path, message: "cannot generate file key: " + err.Error()})
				continue
			}
			plan.key = key
			plan.sidecarName = bindSidecarFilename(plan.path, key)
			keyOwners[key] = []string{plan.path}
		}
		if plan.key != "" {
			if plan.sidecarName == "" {
				plan.sidecarName = bindSidecarFilename(plan.path, plan.key)
			}
			plan.sidecarPath = filepath.Join(BindDir, plan.sidecarName)
			addBindInclude(plan)
			if content, err := fSys.ReadFile(plan.sidecarPath); err == nil {
				diagnostics = append(diagnostics, validateExistingSidecar(plan.sidecarPath, plan.key, content)...)
			} else if !errors.Is(err, os.ErrNotExist) {
				diagnostics = append(diagnostics, bindDiagnostic{path: plan.sidecarPath, message: "cannot read existing sidecar: " + err.Error()})
			}
		}
		if filepath.Base(plan.path) == "triceConfig.h" {
			updated := bytes.Replace(plan.final, []byte("#define TRICE_CLEAN 1"), []byte("#define TRICE_CLEAN 0"), 1)
			if !bytes.Equal(updated, plan.final) {
				plan.final = updated
				plan.configChanged = true
			}
		}
		diagnostics = append(diagnostics, plan.diagnostics...)
		plan.diagnostics = nil
	}
	return diagnostics
}

// newBindFileKey returns a cryptographically random key that is unique in the current plan set.
func newBindFileKey(owners map[string][]string) (string, error) {
	var random [8]byte
	for attempt := 0; attempt < 128; attempt++ {
		if _, err := io.ReadFull(bindRandomReader, random[:]); err != nil {
			return "", err
		}
		key := "K" + strings.ToUpper(hex.EncodeToString(random[:]))
		if _, exists := owners[key]; !exists {
			return key, nil
		}
	}
	return "", errors.New("128 random file keys collided")
}

// assignBindIDs runs the established insert pass in memory in deterministic source-path order.
func assignBindIDs(w io.Writer, plans []bindFilePlan, initialIDs map[TriceID]struct{}) []bindDiagnostic {
	var diagnostics []bindDiagnostic
	admin := new(ant.Admin)
	classes := []bindFileClass{bindFileInsert, bindFileBound}
	for _, class := range classes {
		for i := range plans {
			plan := &plans[i]
			if plan.class != class {
				continue
			}
			var insertOutput bytes.Buffer
			virtual, modified, err := IDData.insertTriceIDs(&insertOutput, plan.path, ToLIFile(plan.path), plan.final, admin)
			if err != nil {
				message := strings.TrimSpace(insertOutput.String())
				if message == "" {
					message = err.Error()
				}
				diagnostics = append(diagnostics, bindDiagnostic{path: plan.path, message: message})
				continue
			}
			if plan.class == bindFileInsert {
				if modified {
					diagnostics = append(diagnostics, bindDiagnostic{path: plan.path, message: "insert validation would change this insert-owned file; run trice insert and resolve its ID conflicts first"})
				}
				continue
			}

			assigned, parseDiagnostics := scanBindSites(plan.path, string(virtual))
			for _, diagnostic := range parseDiagnostics {
				if strings.Contains(diagnostic.message, "multiple bindable") {
					continue
				}
				diagnostics = append(diagnostics, diagnostic)
			}
			if len(assigned) != len(plan.sites) {
				diagnostics = append(diagnostics, bindDiagnostic{path: plan.path, message: fmt.Sprintf("in-memory insert returned %d sites for %d parsed bind sites", len(assigned), len(plan.sites))})
				continue
			}
			for siteIndex := range plan.sites {
				assignedSite := assigned[siteIndex]
				if assignedSite.id <= 0 || assignedSite.wrapper == "" {
					diagnostics = append(diagnostics, bindDiagnostic{path: plan.path, line: plan.sites[siteIndex].line, message: "in-memory insert did not produce a valid site ID"})
					continue
				}
				plan.sites[siteIndex].id = assignedSite.id
				if plan.sites[siteIndex].mode == bindSiteAuto {
					plan.sites[siteIndex].wrapper = assignedSite.wrapper
				}
				IDData.idToLocNew[assignedSite.id] = newTriceLI(plan.path, plan.sites[siteIndex].line)
				if _, existed := initialIDs[assignedSite.id]; existed {
					plan.reusedIDs++
				} else {
					plan.newIDs++
				}
			}
		}
	}
	return diagnostics
}

// buildBindWrites renders JSON in memory and omits every destination whose bytes are unchanged.
func buildBindWrites(fSys *afero.Afero, plans []bindFilePlan) ([]bindWrite, error) {
	writes := make([]bindWrite, 0, len(plans)*2+2)
	for i := range plans {
		plan := &plans[i]
		if !bytes.Equal(plan.original, plan.final) {
			writes = append(writes, bindWrite{path: plan.path, data: plan.final, perm: plan.info.Mode(), kind: "source"})
		}
		if len(plan.sidecarContent) > 0 {
			unchanged, err := bindFileHasContent(fSys, plan.sidecarPath, plan.sidecarContent)
			if err != nil {
				return nil, err
			}
			if !unchanged {
				writes = append(writes, bindWrite{path: plan.sidecarPath, data: plan.sidecarContent, perm: fileWritePerm(fSys, plan.sidecarPath, 0o644), kind: "sidecar"})
			}
		}
	}

	if len(IDData.idToTrice) > IDData.idInitialCount {
		til, err := IDData.idToTrice.toJSON()
		if err != nil {
			return nil, fmt.Errorf("cannot render %s: %w", FnJSON, err)
		}
		unchanged, err := bindFileHasContent(fSys, FnJSON, til)
		if err != nil {
			return nil, err
		}
		if !unchanged {
			writes = append(writes, bindWrite{path: FnJSON, data: til, perm: fileWritePerm(fSys, FnJSON, 0o666), kind: "til"})
		}
	}

	if LIFnJSON != "off" && LIFnJSON != "none" && (len(IDData.idToLocNew) > 0 || IDData.liNeedsRewrite) {
		for id, location := range IDData.idToLocNew {
			IDData.idToLocRef[id] = location
		}
		li, err := IDData.idToLocRef.toJSON()
		if err != nil {
			return nil, fmt.Errorf("cannot render %s: %w", LIFnJSON, err)
		}
		unchanged, err := bindFileHasContent(fSys, LIFnJSON, li)
		if err != nil {
			return nil, err
		}
		if !unchanged {
			writes = append(writes, bindWrite{path: LIFnJSON, data: li, perm: fileWritePerm(fSys, LIFnJSON, 0o666), kind: "li"})
		}
	}

	order := map[string]int{"sidecar": 0, "til": 1, "li": 2, "source": 3}
	sort.SliceStable(writes, func(i, j int) bool {
		if order[writes[i].kind] != order[writes[j].kind] {
			return order[writes[i].kind] < order[writes[j].kind]
		}
		return writes[i].path < writes[j].path
	})
	return writes, nil
}

// bindFileHasContent compares bytes without treating an absent generated destination as an error.
func bindFileHasContent(fSys *afero.Afero, path string, expected []byte) (bool, error) {
	content, err := fSys.ReadFile(path)
	if errors.Is(err, os.ErrNotExist) {
		return false, nil
	}
	if err != nil {
		return false, fmt.Errorf("cannot read %s before commit: %w", path, err)
	}
	return bytes.Equal(content, expected), nil
}

// commitBindWrites creates only required parent directories and rolls back earlier replacements on failure.
func commitBindWrites(fSys *afero.Afero, writes []bindWrite) error {
	originals, err := snapshotBindWrites(fSys, writes)
	if err != nil {
		return err
	}
	for index, write := range writes {
		if write.kind == "sidecar" {
			if err := fSys.MkdirAll(filepath.Dir(write.path), 0o755); err != nil {
				return rollbackBindWrites(fSys, writes[:index], originals[:index], fmt.Errorf("cannot create bind directory for %s: %w", write.path, err))
			}
		}
		if err := atomicWriteFile(fSys, write.path, write.data, write.perm); err != nil {
			return rollbackBindWrites(fSys, writes[:index], originals[:index], fmt.Errorf("cannot write %s: %w", write.path, err))
		}
	}
	return nil
}

// snapshotBindWrites captures every old destination before the first regular output is replaced.
func snapshotBindWrites(fSys *afero.Afero, writes []bindWrite) ([]bindOriginalFile, error) {
	originals := make([]bindOriginalFile, len(writes))
	for index, write := range writes {
		info, err := fSys.Stat(write.path)
		if errors.Is(err, os.ErrNotExist) {
			continue
		}
		if err != nil {
			return nil, fmt.Errorf("cannot inspect %s before commit: %w", write.path, err)
		}
		content, err := fSys.ReadFile(write.path)
		if err != nil {
			return nil, fmt.Errorf("cannot snapshot %s before commit: %w", write.path, err)
		}
		originals[index] = bindOriginalFile{existed: true, data: content, perm: info.Mode()}
	}
	return originals, nil
}

// rollbackBindWrites restores every already-applied destination in reverse commit order.
func rollbackBindWrites(fSys *afero.Afero, writes []bindWrite, originals []bindOriginalFile, cause error) error {
	var rollbackErrors []error
	for index := len(writes) - 1; index >= 0; index-- {
		write := writes[index]
		original := originals[index]
		if original.existed {
			if err := atomicWriteFile(fSys, write.path, original.data, original.perm); err != nil {
				rollbackErrors = append(rollbackErrors, fmt.Errorf("cannot restore %s: %w", write.path, err))
			}
			continue
		}
		if err := fSys.Remove(write.path); err != nil && !errors.Is(err, os.ErrNotExist) {
			rollbackErrors = append(rollbackErrors, fmt.Errorf("cannot remove newly created %s: %w", write.path, err))
		}
	}
	if len(rollbackErrors) == 0 {
		return cause
	}
	return errors.Join(cause, fmt.Errorf("bind commit rollback failed: %w", errors.Join(rollbackErrors...)))
}

// printBindSummary emits the requested classification and plan details only in verbose mode.
func printBindSummary(w io.Writer, plans []bindFilePlan, writes []bindWrite) {
	if !Verbose {
		return
	}
	writeKinds := make(map[string]string, len(writes))
	for _, write := range writes {
		writeKinds[write.path] = write.kind
	}
	for i := range plans {
		plan := &plans[i]
		fmt.Fprintf(w, "%s: %s\n", plan.path, plan.class)
		if plan.class == bindFileInsert {
			fmt.Fprintln(w, "  skipped: insert-owned")
			continue
		}
		if plan.includeAdded {
			fmt.Fprintf(w, "  include planned: %s\n", plan.sidecarName)
		} else if plan.sidecarName != "" {
			fmt.Fprintf(w, "  include recognized: %s\n", plan.sidecarName)
		}
		if writeKinds[plan.sidecarPath] == "sidecar" {
			fmt.Fprintf(w, "  sidecar generated or updated: %s\n", plan.sidecarPath)
		}
		if plan.newIDs+plan.reusedIDs > 0 {
			fmt.Fprintf(w, "  IDs: %d reused, %d new\n", plan.reusedIDs, plan.newIDs)
		}
	}
	if DryRun {
		fmt.Fprintf(w, "dry-run: %d file write(s) suppressed\n", len(writes))
	}
}

// reportBindDiagnostics prints every stable diagnostic and returns one non-zero command error.
func reportBindDiagnostics(w io.Writer, diagnostics []bindDiagnostic) error {
	diagnostics = sortedBindDiagnostics(diagnostics)
	for _, diagnostic := range diagnostics {
		fmt.Fprintln(w, formatBindDiagnostic(diagnostic))
	}
	return fmt.Errorf("trice bind failed with %d error(s)", len(diagnostics))
}

// compactSortedStrings returns a deterministic unique owner list.
func compactSortedStrings(values []string) []string {
	result := append([]string(nil), values...)
	sort.Strings(result)
	if len(result) == 0 {
		return result
	}
	write := 1
	for read := 1; read < len(result); read++ {
		if result[read] != result[write-1] {
			result[write] = result[read]
			write++
		}
	}
	return result[:write]
}
