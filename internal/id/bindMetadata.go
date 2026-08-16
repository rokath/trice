// SPDX-License-Identifier: MIT

package id

import (
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
	"sync"

	"github.com/spf13/afero"
)

var (
	// bindHistoricalOrdinaryID extracts a numeric ID from an ordinary generated
	// line descriptor. Descriptors referring to a wrapper definition are handled
	// by bindHistoricalDefinitionID instead because they contain no numeric TID.
	bindHistoricalOrdinaryID = regexp.MustCompile(`(?m)^[\t ]*#[\t ]*define[\t ]+TRICE_BIND_SITE_(K[0-9A-F]{16})_L([0-9]+)[\t ]+[^\r\n]*\b(?:iD|id|Id|ID)\([\t ]*([0-9]+)u?[\t ]*\)`)
	// bindHistoricalDefinitionID extracts the persistent ID of a Trice site in
	// a supported logging-wrapper definition.
	bindHistoricalDefinitionID = regexp.MustCompile(`(?m)^[\t ]*#[\t ]*define[\t ]+TRICE_BIND_ID_DEFINITION_(K[0-9A-F]{16})_L([0-9]+)_O([0-9]+)[\t ]+([0-9]+)u?\b`)
	// bindHistoricalLocationID extracts a direct counter-rebase site ID. The
	// source ordinal distinguishes multiple direct calls on one physical line.
	bindHistoricalLocationID = regexp.MustCompile(`(?m)^[\t ]*#[\t ]*define[\t ]+TRICE_BIND_ID_LOCATION_(K[0-9A-F]{16})_L([0-9]+)_O([0-9]+)[\t ]+([0-9]+)u?\b`)
)

// bindMetadataKind identifies a read-only JSON file by its data shape rather
// than by its filename. The primary files keep their roles from the CLI.
type bindMetadataKind uint8

const (
	bindMetadataUnknown bindMetadataKind = iota
	bindMetadataTIL
	bindMetadataLI
)

// bindDiscoveredJSON is one validated, read-only metadata source. Exactly one
// of til or li is populated for a non-empty recognized document.
type bindDiscoveredJSON struct {
	path string
	dir  string
	kind bindMetadataKind
	til  TriceIDLookUp
	li   TriceIDLookUpLI
}

// bindRankedJSON adds source-local search precedence without duplicating the
// parsed JSON document in every file plan.
type bindRankedJSON struct {
	data *bindDiscoveredJSON
	rank int
}

// bindHistoricalSite is non-authoritative evidence recovered from an old
// generated sidecar. Its old line and ordinal are ordering hints only.
type bindHistoricalSite struct {
	line    int
	ordinal int
	kind    string
	id      TriceID
}

// bindHistoricalSidecar contains validated ID evidence for the File Key owned
// by one current source file. Its bytes are never copied to the new bind tree.
type bindHistoricalSidecar struct {
	path    string
	rank    int
	primary bool
	sites   []bindHistoricalSite
}

// bindPlanMetadata is the source-specific view of all bounded, read-only
// metadata. Search order is deterministic and nearest-source-first.
type bindPlanMetadata struct {
	json     []bindRankedJSON
	sidecars []bindHistoricalSidecar
}

// bindIDCandidate describes one recoverable site ID. Lower priority and rank
// values are stronger; lineDistance breaks ties without trusting old lines as
// permanent identity.
type bindIDCandidate struct {
	id           TriceID
	priority     int
	rank         int
	lineDistance int
	source       string
}

// bindMetadataResolver caches directory listings and parsed JSON while each
// source receives its own proximity order. All cached data remains read-only.
type bindMetadataResolver struct {
	w           io.Writer
	fSys        *afero.Afero
	directories map[string][]os.FileInfo
	json        map[string]*bindDiscoveredJSON
	reported    map[string]bool
	primaryTIL  string
	primaryLI   string
}

// collectBindInputs enumerates source files deterministically, skips every
// hidden directory, and reads the resulting files concurrently. A -src entry
// may name either one file or a complete directory tree.
func collectBindInputs(w io.Writer, fSys *afero.Afero) ([]bindFileInput, []bindDiagnostic) {
	if w == nil {
		w = io.Discard
	}
	exclusions := append([]string(nil), ExcludeSrcs...)
	exclusions = append(exclusions, BindDir)

	files := make(map[string]os.FileInfo)
	var diagnostics []bindDiagnostic
	roots := append([]string(nil), Srcs...)
	sort.Strings(roots)
	for _, root := range roots {
		info, err := fSys.Stat(root)
		if errors.Is(err, os.ErrNotExist) {
			// Preserve the established source-walker behavior for a missing CLI
			// path without turning the remaining valid roots into a fatal error.
			fmt.Fprintln(w, root, "does not exist!")
			continue
		}
		if err != nil {
			diagnostics = append(diagnostics, bindDiagnostic{path: root, message: "cannot inspect source root: " + err.Error()})
			continue
		}
		if bindSourceRootIsHidden(root, info) {
			if Verbose {
				fmt.Fprintf(w, "trice bind: ignore hidden source root %s\n", root)
			}
			continue
		}

		err = fSys.Walk(root, func(path string, fileInfo os.FileInfo, walkErr error) error {
			if walkErr != nil {
				return walkErr
			}
			if fileInfo.IsDir() && bindHiddenBase(fileInfo.Name()) {
				if Verbose {
					fmt.Fprintf(w, "trice bind: ignore hidden directory %s\n", path)
				}
				return filepath.SkipDir
			}
			if bindPathExcluded(path, exclusions) {
				if fileInfo.IsDir() {
					return filepath.SkipDir
				}
				return nil
			}
			if fileInfo.IsDir() || !isSourceFile(fileInfo) {
				return nil
			}
			files[filepath.Clean(path)] = fileInfo
			return nil
		})
		if err != nil {
			diagnostics = append(diagnostics, bindDiagnostic{path: root, message: "source walk failed: " + err.Error()})
		}
	}

	paths := make([]string, 0, len(files))
	for path := range files {
		paths = append(paths, path)
	}
	sort.Strings(paths)
	inputs := make([]bindFileInput, len(paths))
	readDiagnostics := make([]*bindDiagnostic, len(paths))
	var workers sync.WaitGroup
	workers.Add(len(paths))
	for index, path := range paths {
		index, path := index, path
		go func() {
			defer workers.Done()
			content, err := fSys.ReadFile(path)
			if err != nil {
				diagnostic := bindDiagnostic{path: path, message: "cannot read source: " + err.Error()}
				readDiagnostics[index] = &diagnostic
				return
			}
			inputs[index] = bindFileInput{path: path, info: files[path], data: content}
		}()
	}
	workers.Wait()

	result := make([]bindFileInput, 0, len(inputs))
	for index := range inputs {
		if readDiagnostics[index] != nil {
			diagnostics = append(diagnostics, *readDiagnostics[index])
			continue
		}
		result = append(result, inputs[index])
	}
	return result, diagnostics
}

// bindHiddenBase reports the portable hidden-directory convention used by the
// bind walker and implicit metadata search. The special path elements . and ..
// are navigation syntax rather than hidden directory names.
func bindHiddenBase(name string) bool {
	return len(name) > 1 && strings.HasPrefix(name, ".") && name != ".."
}

// bindSourceRootIsHidden applies hidden-directory exclusion even when the user
// explicitly passed a hidden directory or a file below one as -src.
func bindSourceRootIsHidden(path string, info os.FileInfo) bool {
	directory := path
	if !info.IsDir() {
		directory = filepath.Dir(path)
	}
	return bindPathHasHiddenDirectory(directory)
}

// bindPathHasHiddenDirectory checks directory components without classifying
// a hidden regular file as a hidden folder.
func bindPathHasHiddenDirectory(path string) bool {
	clean := filepath.Clean(path)
	for {
		base := filepath.Base(clean)
		if bindHiddenBase(base) {
			return true
		}
		parent := filepath.Dir(clean)
		if parent == clean {
			return false
		}
		clean = parent
	}
}

// bindPathExcluded gives bind the same subtree semantics as the established
// ant walker while also handling relative and absolute spellings consistently.
func bindPathExcluded(path string, exclusions []string) bool {
	for _, exclusion := range exclusions {
		if strings.TrimSpace(exclusion) == "" {
			continue
		}
		if bindPathWithin(path, exclusion) {
			return true
		}
	}
	return false
}

// bindPathWithin reports whether path is tree itself or one of its descendants.
func bindPathWithin(path, tree string) bool {
	path = bindComparablePath(path)
	tree = bindComparablePath(tree)
	relative, err := filepath.Rel(tree, path)
	if err != nil {
		return false
	}
	return relative == "." || relative != ".." && !strings.HasPrefix(relative, ".."+string(os.PathSeparator))
}

// bindComparablePath creates a stable lexical comparison path without
// resolving symlinks or consulting a filesystem other than the process CWD.
func bindComparablePath(path string) string {
	absolute, err := filepath.Abs(path)
	if err == nil {
		return filepath.Clean(absolute)
	}
	return filepath.Clean(path)
}

// newBindMetadataResolver initializes one command-local, read-only discovery
// cache. Explicit primary paths are recorded so they cannot be rediscovered as
// secondary metadata.
func newBindMetadataResolver(w io.Writer, fSys *afero.Afero) *bindMetadataResolver {
	if w == nil {
		w = io.Discard
	}
	primaryLI := LIFnJSON
	if primaryLI == "off" || primaryLI == "none" {
		primaryLI = ""
	}
	comparablePrimaryLI := ""
	if primaryLI != "" {
		comparablePrimaryLI = bindComparablePath(primaryLI)
	}
	return &bindMetadataResolver{
		w:           w,
		fSys:        fSys,
		directories: make(map[string][]os.FileInfo),
		json:        make(map[string]*bindDiscoveredJSON),
		reported:    make(map[string]bool),
		primaryTIL:  bindComparablePath(FnJSON),
		primaryLI:   comparablePrimaryLI,
	}
}

// metadataForPlan discovers only metadata reachable through the current
// source's bounded search corridor and the explicitly selected metadata roots.
func (resolver *bindMetadataResolver) metadataForPlan(plan *bindFilePlan) bindPlanMetadata {
	directories := bindMetadataSearchDirectories(resolver.fSys, plan.path)
	view := bindPlanMetadata{}
	seenJSON := make(map[string]bool)
	for rank, directory := range directories {
		for _, metadata := range resolver.metadataInDirectory(directory) {
			key := bindComparablePath(metadata.path)
			if seenJSON[key] {
				continue
			}
			seenJSON[key] = true
			view.json = append(view.json, bindRankedJSON{data: metadata, rank: rank})
		}
	}
	view.sidecars = resolver.sidecarsForPlan(plan, directories)
	return view
}

// bindMetadataSearchDirectories returns nearest-source-first directories. It
// climbs only to the owning -src anchor, adds one parent, and then adds the
// explicit primary TIL/LI directories and one parent for each.
func bindMetadataSearchDirectories(fSys *afero.Afero, sourcePath string) []string {
	anchor := bindOwningSourceAnchor(fSys, sourcePath)
	var directories []string
	current := filepath.Dir(sourcePath)
	if bindPathWithin(current, anchor) {
		for {
			directories = append(directories, current)
			if bindComparablePath(current) == bindComparablePath(anchor) {
				break
			}
			parent := filepath.Dir(current)
			if parent == current {
				break
			}
			current = parent
		}
	} else {
		directories = append(directories, current, anchor)
	}
	directories = appendBindDirectoryAndParent(directories, anchor)
	directories = appendBindDirectoryAndParent(directories, filepath.Dir(FnJSON))
	if LIFnJSON != "off" && LIFnJSON != "none" {
		directories = appendBindDirectoryAndParent(directories, filepath.Dir(LIFnJSON))
	}

	seen := make(map[string]bool)
	result := make([]string, 0, len(directories))
	for _, directory := range directories {
		if directory == "" || bindPathHasHiddenDirectory(directory) {
			continue
		}
		key := bindComparablePath(directory)
		if seen[key] {
			continue
		}
		seen[key] = true
		result = append(result, filepath.Clean(directory))
	}
	return result
}

// bindOwningSourceAnchor selects the most specific -src entry containing the
// source. A file-valued -src anchors discovery at that file's parent directory.
func bindOwningSourceAnchor(fSys *afero.Afero, sourcePath string) string {
	best := filepath.Dir(sourcePath)
	bestLength := -1
	for _, sourceRoot := range Srcs {
		info, err := fSys.Stat(sourceRoot)
		if err != nil {
			continue
		}
		candidate := filepath.Clean(sourceRoot)
		if !info.IsDir() {
			if bindComparablePath(candidate) != bindComparablePath(sourcePath) {
				continue
			}
			candidate = filepath.Dir(candidate)
		} else if !bindPathWithin(sourcePath, candidate) {
			continue
		}
		length := len(bindComparablePath(candidate))
		if length > bestLength {
			best = candidate
			bestLength = length
		}
	}
	return filepath.Clean(best)
}

// appendBindDirectoryAndParent appends one anchor and at most one additional
// parent. De-duplication and hidden-directory filtering happen afterwards.
func appendBindDirectoryAndParent(directories []string, directory string) []string {
	if strings.TrimSpace(directory) == "" {
		return directories
	}
	directory = filepath.Clean(directory)
	directories = append(directories, directory)
	parent := filepath.Dir(directory)
	if parent != directory {
		directories = append(directories, parent)
	}
	return directories
}

// metadataInDirectory identifies every immediate *.json file by content. It
// deliberately performs no recursive or sibling-directory search.
func (resolver *bindMetadataResolver) metadataInDirectory(directory string) []*bindDiscoveredJSON {
	entries, ok := resolver.directories[directory]
	if !ok {
		readEntries, err := resolver.fSys.ReadDir(directory)
		if err != nil {
			if !errors.Is(err, os.ErrNotExist) {
				resolver.verboseOnce("dir:"+directory, "trice bind: ignore unreadable metadata directory %s: %v\n", directory, err)
			}
			resolver.directories[directory] = nil
			return nil
		}
		entries = readEntries
		sort.Slice(entries, func(i, j int) bool { return entries[i].Name() < entries[j].Name() })
		resolver.directories[directory] = entries
	}

	var result []*bindDiscoveredJSON
	for _, entry := range entries {
		if entry.IsDir() || !strings.EqualFold(filepath.Ext(entry.Name()), ".json") {
			continue
		}
		path := filepath.Join(directory, entry.Name())
		comparable := bindComparablePath(path)
		if comparable == resolver.primaryTIL || resolver.primaryLI != "" && comparable == resolver.primaryLI {
			continue
		}
		metadata, cached := resolver.json[comparable]
		if !cached {
			metadata = resolver.readMetadataJSON(path)
			resolver.json[comparable] = metadata
		}
		if metadata != nil {
			result = append(result, metadata)
		}
	}
	return result
}

// readMetadataJSON parses one secondary candidate without allowing malformed
// optional data to abort or mutate the primary project.
func (resolver *bindMetadataResolver) readMetadataJSON(path string) *bindDiscoveredJSON {
	content, err := resolver.fSys.ReadFile(path)
	if err != nil {
		resolver.verboseOnce("json-read:"+path, "trice bind: ignore unreadable secondary JSON %s: %v\n", path, err)
		return nil
	}
	kind, til, li, err := classifyBindMetadataJSON(content)
	if err != nil {
		resolver.verboseOnce("json-shape:"+path, "trice bind: ignore secondary JSON %s: %v\n", path, err)
		return nil
	}
	if kind == bindMetadataUnknown {
		return nil
	}
	return &bindDiscoveredJSON{path: path, dir: filepath.Dir(path), kind: kind, til: til, li: li}
}

// classifyBindMetadataJSON recognizes non-empty TIL and LI maps solely from
// their value fields. Empty secondary maps contain no evidence and need no role.
func classifyBindMetadataJSON(content []byte) (bindMetadataKind, TriceIDLookUp, TriceIDLookUpLI, error) {
	var values map[string]json.RawMessage
	if err := json.Unmarshal(content, &values); err != nil {
		return bindMetadataUnknown, nil, nil, err
	}
	if len(values) == 0 {
		return bindMetadataUnknown, nil, nil, nil
	}
	allTIL, allLI := true, true
	for key, raw := range values {
		id, err := strconv.Atoi(key)
		if err != nil || id <= 0 {
			return bindMetadataUnknown, nil, nil, fmt.Errorf("key %q is not a positive Trice ID", key)
		}
		var fields map[string]json.RawMessage
		if err := json.Unmarshal(raw, &fields); err != nil {
			return bindMetadataUnknown, nil, nil, fmt.Errorf("ID %s is not an object", key)
		}
		_, hasType := fields["Type"]
		_, hasString := fields["Strg"]
		_, hasFile := fields["File"]
		_, hasLine := fields["Line"]
		allTIL = allTIL && hasType && hasString
		allLI = allLI && hasFile && hasLine
	}
	if allTIL == allLI {
		return bindMetadataUnknown, nil, nil, errors.New("content is neither an unambiguous TIL nor an unambiguous LI")
	}
	if allTIL {
		til := make(TriceIDLookUp)
		if err := json.Unmarshal(content, &til); err != nil {
			return bindMetadataUnknown, nil, nil, err
		}
		return bindMetadataTIL, til, nil, nil
	}
	li := make(TriceIDLookUpLI)
	if err := json.Unmarshal(content, &li); err != nil {
		return bindMetadataUnknown, nil, nil, err
	}
	return bindMetadataLI, nil, li, nil
}

// sidecarsForPlan finds the exact owner filename only. Historical generated
// bytes are parsed as evidence and are never copied into the primary BindDir.
func (resolver *bindMetadataResolver) sidecarsForPlan(plan *bindFilePlan, directories []string) []bindHistoricalSidecar {
	if plan.key == "" || plan.sidecarName == "" {
		return nil
	}
	type sidecarDirectory struct {
		path    string
		primary bool
	}
	candidates := []sidecarDirectory{{path: BindDir, primary: true}}
	for _, directory := range directories {
		candidates = append(candidates, sidecarDirectory{path: filepath.Join(directory, "build", "triceIDs")})
	}
	seen := make(map[string]bool)
	var result []bindHistoricalSidecar
	for rank, directory := range candidates {
		if !directory.primary && bindPathHasHiddenDirectory(directory.path) {
			continue
		}
		key := bindComparablePath(directory.path)
		if seen[key] {
			continue
		}
		seen[key] = true
		path := filepath.Join(directory.path, plan.sidecarName)
		content, err := resolver.fSys.ReadFile(path)
		if errors.Is(err, os.ErrNotExist) {
			continue
		}
		if err != nil {
			resolver.verboseOnce("sidecar-read:"+path, "trice bind: ignore unreadable historical sidecar %s: %v\n", path, err)
			continue
		}
		if !bindHistoricalSidecarIsPlausible(content, plan.key) {
			resolver.verboseOnce("sidecar-shape:"+path, "trice bind: ignore historical sidecar %s because its File Key or route is inconsistent\n", path)
			continue
		}
		sites := parseBindHistoricalSites(content, plan.key)
		if len(sites) == 0 {
			continue
		}
		result = append(result, bindHistoricalSidecar{path: path, rank: rank, primary: directory.primary, sites: sites})
	}
	return result
}

// bindHistoricalSidecarIsPlausible accepts only a sidecar that declares the
// same File Key and normal BIND route as its current source owner.
func bindHistoricalSidecarIsPlausible(content []byte, key string) bool {
	keyDefinitions := bindSidecarKeyDefinition.FindAllSubmatch(content, -1)
	if len(keyDefinitions) == 0 {
		return false
	}
	for _, definition := range keyDefinitions {
		if string(definition[1]) != key {
			return false
		}
	}
	routes := bindSidecarRouteDefinition.FindAllSubmatch(content, -1)
	if len(routes) == 0 {
		return false
	}
	for _, route := range routes {
		if string(route[1]) != key {
			return false
		}
	}
	return true
}

// parseBindHistoricalSites extracts all directly numeric descriptors and sorts
// them by their former source position. Old line numbers guide matching but are
// never emitted without a fresh analysis of the current source.
func parseBindHistoricalSites(content []byte, key string) []bindHistoricalSite {
	var sites []bindHistoricalSite
	for _, match := range bindHistoricalOrdinaryID.FindAllSubmatch(content, -1) {
		if string(match[1]) != key {
			continue
		}
		line, lineErr := strconv.Atoi(string(match[2]))
		id, idErr := strconv.Atoi(string(match[3]))
		if lineErr == nil && idErr == nil && id > 0 {
			sites = append(sites, bindHistoricalSite{line: line, kind: "ordinary", id: TriceID(id)})
		}
	}
	for _, pattern := range []struct {
		kind       string
		expression *regexp.Regexp
	}{{kind: "definition", expression: bindHistoricalDefinitionID}, {kind: "location", expression: bindHistoricalLocationID}} {
		for _, match := range pattern.expression.FindAllSubmatch(content, -1) {
			if string(match[1]) != key {
				continue
			}
			line, lineErr := strconv.Atoi(string(match[2]))
			ordinal, ordinalErr := strconv.Atoi(string(match[3]))
			id, idErr := strconv.Atoi(string(match[4]))
			if lineErr == nil && ordinalErr == nil && idErr == nil && id > 0 {
				sites = append(sites, bindHistoricalSite{line: line, ordinal: ordinal, kind: pattern.kind, id: TriceID(id)})
			}
		}
	}
	sort.SliceStable(sites, func(i, j int) bool {
		if sites[i].line != sites[j].line {
			return sites[i].line < sites[j].line
		}
		if sites[i].ordinal != sites[j].ordinal {
			return sites[i].ordinal < sites[j].ordinal
		}
		if sites[i].kind != sites[j].kind {
			return sites[i].kind < sites[j].kind
		}
		return sites[i].id < sites[j].id
	})
	return sites
}

// preferredBindIDs resolves read-only history into virtual per-site ID hints.
// The returned IDs are fed through insertTriceIDs, so existing parsing,
// allocation, range handling, and conflict behavior remain authoritative.
func preferredBindIDs(w io.Writer, plans []bindFilePlan, resolver *bindMetadataResolver) map[bindSiteReference]TriceID {
	preferred := make(map[bindSiteReference]TriceID)
	claimed := make(map[TriceID]bindSiteReference)
	for planIndex := range plans {
		for siteIndex, site := range plans[planIndex].sites {
			if site.wasExplicit && site.id > 0 {
				claimed[site.id] = bindSiteReference{plan: planIndex, site: siteIndex}
			}
		}
	}

	for planIndex := range plans {
		plan := &plans[planIndex]
		if plan.class != bindFileBound {
			continue
		}
		metadata := resolver.metadataForPlan(plan)
		sidecarCandidates := bindSidecarCandidates(plan, metadata, IDData.idToTrice)
		for siteIndex := range plan.sites {
			site := plan.sites[siteIndex]
			if site.wasExplicit {
				continue
			}
			ref := bindSiteReference{plan: planIndex, site: siteIndex}
			format := bindSiteFormat(site)
			candidates := append([]bindIDCandidate(nil), sidecarCandidates[siteIndex]...)
			candidates = append(candidates, bindCatalogCandidates(plan, site, format, metadata)...)
			candidates = compactBindIDCandidates(candidates)
			for _, candidate := range candidates {
				if candidate.id <= 0 {
					continue
				}
				primaryFormat, primaryExists := IDData.idToTrice[candidate.id]
				if primaryExists {
					if primaryFormat != format {
						resolver.verboseOnce(
							fmt.Sprintf("collision:%d:%s:%d", candidate.id, plan.path, site.line),
							"trice bind: %s:%d: read-only ID %d from %s yields to the primary TIL mapping %+v\n",
							plan.path, site.line, candidate.id, candidate.source, primaryFormat,
						)
						continue
					}
					if location, hasLocation := IDData.idToLocRef[candidate.id]; hasLocation && !bindLocationMatches(location.File, LIFnJSON, plan.path) {
						resolver.verboseOnce(
							fmt.Sprintf("location:%d:%s:%d", candidate.id, plan.path, site.line),
							"trice bind: %s:%d: ID %d remains associated with primary location %s; trying the next candidate\n",
							plan.path, site.line, candidate.id, location.File,
						)
						continue
					}
				}
				if owner, used := claimed[candidate.id]; used && owner != ref {
					resolver.verboseOnce(
						fmt.Sprintf("claimed:%d:%s:%d", candidate.id, plan.path, site.line),
						"trice bind: %s:%d: read-only ID %d from %s is already assigned in this bind plan; trying the next candidate\n",
						plan.path, site.line, candidate.id, candidate.source,
					)
					continue
				}
				if !primaryExists {
					// Import only the selected active mapping. Historical entries not
					// used by the current source set remain in their read-only TIL.
					IDData.idToTrice[candidate.id] = format
					IDData.removeIDFromIDSpace(candidate.id)
				}
				claimed[candidate.id] = ref
				preferred[ref] = candidate.id
				if Verbose && bindComparablePath(candidate.source) != bindComparablePath(FnJSON) {
					fmt.Fprintf(w, "trice bind: %s:%d: reuse ID %d from read-only metadata %s\n", plan.path, site.line, candidate.id, candidate.source)
				}
				break
			}
		}
	}

	// Rebuild and sort the shared reverse lookup after importing selected IDs.
	// Sorting removes Go map iteration order from otherwise ambiguous equal-format
	// fallbacks before the established insert engine consumes the candidates.
	IDData.triceToId = IDData.idToTrice.reverseS()
	for format := range IDData.triceToId {
		sort.Slice(IDData.triceToId[format], func(i, j int) bool {
			return IDData.triceToId[format][i] < IDData.triceToId[format][j]
		})
	}
	return preferred
}

// bindSiteFormat reconstructs the established TriceFmt from parser-derived
// site data. Alias resolution is shared with insert and no second parser exists.
func bindSiteFormat(site bindSite) TriceFmt {
	format := TriceFmt{Type: site.macro, Strg: site.format}
	resolveTriceAlias(&format)
	format.Alias = ""
	return format
}

// bindSidecarCandidates aligns old sidecar descriptors with current sites by
// TriceFmt and source order. This survives shifted lines while keeping repeated
// identical formats deterministic.
func bindSidecarCandidates(plan *bindFilePlan, metadata bindPlanMetadata, primary TriceIDLookUp) map[int][]bindIDCandidate {
	result := make(map[int][]bindIDCandidate)
	currentByFormat := make(map[TriceFmt][]int)
	for siteIndex, site := range plan.sites {
		if !site.wasExplicit {
			format := bindSiteFormat(site)
			currentByFormat[format] = append(currentByFormat[format], siteIndex)
		}
	}
	for _, sidecar := range metadata.sidecars {
		historicalByFormat := make(map[TriceFmt][]bindHistoricalSite)
		for _, historical := range sidecar.sites {
			format, ok := bindHistoricalFormat(historical.id, sidecar.primary, metadata, primary)
			if ok {
				historicalByFormat[format] = append(historicalByFormat[format], historical)
			}
		}
		for format, currentIndexes := range currentByFormat {
			historicalSites := historicalByFormat[format]
			count := len(currentIndexes)
			if len(historicalSites) < count {
				count = len(historicalSites)
			}
			for index := 0; index < count; index++ {
				currentSite := plan.sites[currentIndexes[index]]
				oldSite := historicalSites[index]
				result[currentIndexes[index]] = append(result[currentIndexes[index]], bindIDCandidate{
					id:           oldSite.id,
					priority:     0,
					rank:         sidecar.rank,
					lineDistance: bindLineDistance(currentSite.line, oldSite.line),
					source:       sidecar.path,
				})
			}
		}
	}
	return result
}

// bindHistoricalFormat finds the format that made a historical sidecar ID
// meaningful. The primary TIL is preferred for a primary sidecar; otherwise the
// nearest read-only TIL is preferred before falling back to the primary map.
func bindHistoricalFormat(id TriceID, primarySidecar bool, metadata bindPlanMetadata, primary TriceIDLookUp) (TriceFmt, bool) {
	if primarySidecar {
		if format, ok := primary[id]; ok {
			return format, true
		}
	}
	for _, ranked := range metadata.json {
		if ranked.data.kind != bindMetadataTIL {
			continue
		}
		if format, ok := ranked.data.til[id]; ok {
			return format, true
		}
	}
	format, ok := primary[id]
	return format, ok
}

// bindCatalogCandidates returns primary and read-only TIL candidates for one
// exact TriceFmt. Existing LI ownership in another file prevents format-only
// reuse; a matching file and nearby line improve the candidate priority.
func bindCatalogCandidates(plan *bindFilePlan, site bindSite, format TriceFmt, metadata bindPlanMetadata) []bindIDCandidate {
	var candidates []bindIDCandidate
	primaryIDs := bindSortedFormatIDs(IDData.idToTrice, format)
	for _, id := range primaryIDs {
		if location, ok := IDData.idToLocRef[id]; ok {
			if !bindLocationMatches(location.File, LIFnJSON, plan.path) {
				continue
			}
			candidates = append(candidates, bindIDCandidate{id: id, priority: 1, lineDistance: bindLineDistance(site.line, location.Line), source: FnJSON})
			continue
		}
		candidates = append(candidates, bindIDCandidate{id: id, priority: 2, source: FnJSON})
	}

	for _, ranked := range metadata.json {
		if ranked.data.kind != bindMetadataTIL {
			continue
		}
		for _, id := range bindSortedFormatIDs(ranked.data.til, format) {
			location, hasLocation, locationMatches := bindSecondaryLocation(id, ranked.data.dir, plan.path, metadata)
			if hasLocation && !locationMatches {
				continue
			}
			candidate := bindIDCandidate{id: id, priority: 3, rank: ranked.rank, source: ranked.data.path}
			if hasLocation {
				candidate.priority = 1
				candidate.lineDistance = bindLineDistance(site.line, location.Line)
			}
			candidates = append(candidates, candidate)
		}
	}
	return candidates
}

// bindSortedFormatIDs lists exact Type-and-Strg matches in numeric order.
func bindSortedFormatIDs(lookup TriceIDLookUp, format TriceFmt) []TriceID {
	var ids []TriceID
	for id, candidate := range lookup {
		if candidate == format {
			ids = append(ids, id)
		}
	}
	sort.Slice(ids, func(i, j int) bool { return ids[i] < ids[j] })
	return ids
}

// bindSecondaryLocation pairs a secondary TIL only with LI documents in the
// same directory. This avoids treating an equal numeric ID from another
// subproject as location evidence for the current catalog.
func bindSecondaryLocation(id TriceID, tilDirectory, sourcePath string, metadata bindPlanMetadata) (TriceLI, bool, bool) {
	var first TriceLI
	found := false
	for _, ranked := range metadata.json {
		if ranked.data.kind != bindMetadataLI || bindComparablePath(ranked.data.dir) != bindComparablePath(tilDirectory) {
			continue
		}
		location, ok := ranked.data.li[id]
		if !ok {
			continue
		}
		if !found {
			first = location
			found = true
		}
		if bindLocationMatches(location.File, ranked.data.path, sourcePath) {
			return location, true, true
		}
	}
	return first, found, false
}

// bindLocationMatches compares a stored portable LI path against several
// representations derived from the metadata file and current source. Component
// suffix matching accommodates a project bound from a different parent root.
func bindLocationMatches(stored, metadataPath, sourcePath string) bool {
	stored = normalizeLocationPath(stored)
	if stored == "" || stored == "." {
		return false
	}
	candidates := []string{normalizeLocationPath(sourcePath), ToLIFile(sourcePath)}
	if metadataPath != "" && metadataPath != "off" && metadataPath != "none" {
		if relative, err := filepath.Rel(filepath.Dir(metadataPath), sourcePath); err == nil {
			candidates = append(candidates, normalizeLocationPath(relative))
		}
	}
	for _, candidate := range candidates {
		if candidate == stored || strings.HasSuffix(candidate, "/"+strings.TrimPrefix(stored, "./")) {
			return true
		}
	}
	return !strings.Contains(stored, "/") && filepath.Base(filepath.FromSlash(sourcePath)) == stored
}

// bindLineDistance returns a stable non-negative tie breaker for relocated
// source sites.
func bindLineDistance(current, historical int) int {
	if current >= historical {
		return current - historical
	}
	return historical - current
}

// compactBindIDCandidates keeps the strongest occurrence of each numeric ID
// and sorts the remaining alternatives deterministically.
func compactBindIDCandidates(candidates []bindIDCandidate) []bindIDCandidate {
	sort.SliceStable(candidates, func(i, j int) bool {
		if candidates[i].priority != candidates[j].priority {
			return candidates[i].priority < candidates[j].priority
		}
		if candidates[i].rank != candidates[j].rank {
			return candidates[i].rank < candidates[j].rank
		}
		if candidates[i].lineDistance != candidates[j].lineDistance {
			return candidates[i].lineDistance < candidates[j].lineDistance
		}
		if candidates[i].id != candidates[j].id {
			return candidates[i].id < candidates[j].id
		}
		return candidates[i].source < candidates[j].source
	})
	seen := make(map[TriceID]bool)
	result := make([]bindIDCandidate, 0, len(candidates))
	for _, candidate := range candidates {
		if seen[candidate.id] {
			continue
		}
		seen[candidate.id] = true
		result = append(result, candidate)
	}
	return result
}

// applyBindPreferredIDs inserts recovered IDs into the in-memory parser view
// only. Reverse-order writes preserve all parser offsets and never touch source.
func applyBindPreferredIDs(source []byte, planIndex int, plan *bindFilePlan, preferred map[bindSiteReference]TriceID) []byte {
	result := string(source)
	for siteIndex := len(plan.sites) - 1; siteIndex >= 0; siteIndex-- {
		id, ok := preferred[bindSiteReference{plan: planIndex, site: siteIndex}]
		if !ok {
			continue
		}
		location := plan.sites[siteIndex].loc
		result, _ = writeID(result, 0, location[:], bindSiteFormat(plan.sites[siteIndex]), id)
	}
	return []byte(result)
}

// preserveBindRebaseFormatting restores formatter-owned horizontal whitespace
// when a regenerated compact boundary has the same complete semantic identity.
// New, removed, or renamed boundaries retain freshly generated text.
func preserveBindRebaseFormatting(plan *bindFilePlan) {
	original := bindRebaseLinesByIdentity(plan.original)
	if len(original) == 0 {
		return
	}
	text := string(plan.final)
	var builder strings.Builder
	for position := 0; position < len(text); {
		end := lineEndIncludingNewline(text, position)
		lineWithEnding := text[position:end]
		line := strings.TrimRight(lineWithEnding, "\r\n")
		ending := lineWithEnding[len(line):]
		identity := bindRebaseLineIdentity(line)
		if preserved := original[identity]; identity != "" && len(preserved) > 0 {
			builder.WriteString(preserved[0])
			builder.WriteString(ending)
			original[identity] = preserved[1:]
		} else {
			builder.WriteString(lineWithEnding)
		}
		position = end
	}
	plan.final = []byte(builder.String())
}

// bindRebaseLinesByIdentity records exact generated line text without newline
// bytes so the current source newline remains authoritative.
func bindRebaseLinesByIdentity(source []byte) map[string][]string {
	result := make(map[string][]string)
	text := string(source)
	for position := 0; position < len(text); {
		end := lineEndIncludingNewline(text, position)
		line := strings.TrimRight(text[position:end], "\r\n")
		if identity := bindRebaseLineIdentity(line); identity != "" {
			result[identity] = append(result[identity], line)
		}
		position = end
	}
	return result
}

// bindRebaseLineIdentity includes every field validated by the compact parser;
// whitespace alone is intentionally absent from the identity.
func bindRebaseLineIdentity(line string) string {
	name, key, scope, kind, ok := parseBindCompactRebaseInclude(line)
	if !ok {
		return ""
	}
	return strings.Join([]string{name, key, scope, kind}, "\x00")
}

// verboseOnce keeps normal bind silent and prevents repeated optional-metadata
// messages when several source files share the same search corridor.
func (resolver *bindMetadataResolver) verboseOnce(key, format string, arguments ...interface{}) {
	if !Verbose || resolver.reported[key] {
		return
	}
	resolver.reported[key] = true
	fmt.Fprintf(resolver.w, format, arguments...)
}
