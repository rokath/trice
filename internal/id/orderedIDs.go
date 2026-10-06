// SPDX-License-Identifier: MIT

package id

import (
	"fmt"
	"io"
	"os"
	"path"
	"path/filepath"
	"sort"
	"strings"

	"github.com/rokath/trice/pkg/ant"
	"github.com/spf13/afero"
)

// orderedIDSite is one writable textual site, independent of worker completion
// order. Its index points back into an Insert source or a Bind plan.
type orderedIDSite struct {
	file, site   int
	path         string
	key          string
	line, column int
	id           TriceID
}

// orderedIDPath uses the location root, never the absolute checkout prefix.
// Go's string comparison is ordinal and does not depend on the host locale.
func orderedIDPath(source string) string {
	return path.Clean(strings.ReplaceAll(ToLIFile(source), `\`, "/"))
}

// reserveIDsOutside protects primary LI entries not conclusively owned by the
// selected files. Ambiguous legacy basenames are deliberately not guessed.
func (p *idData) reserveIDsOutside(paths []string, protected map[TriceID]bool) {
	selected := make(map[string]bool, len(paths))
	for _, source := range paths {
		selected[orderedIDPath(source)] = true
		// Legacy LI may store the exact CLI-relative spelling rather than a
		// path relative to li.json. Accept that spelling, not a basename guess.
		selected[path.Clean(strings.ReplaceAll(source, `\`, "/"))] = true
	}
	p.reservedIDs = make(map[TriceID]bool)
	for id := range protected {
		p.reservedIDs[id] = true
	}
	for id, location := range p.idToLocRef {
		stored := path.Clean(strings.ReplaceAll(location.File, `\`, "/"))
		if filepath.IsAbs(location.File) {
			stored = orderedIDPath(location.File)
		}
		if !selected[stored] {
			p.reservedIDs[id] = true
		}
	}
	for format, ids := range p.triceToId {
		usable := make(TriceIDs, 0, len(ids))
		for _, id := range ids {
			if !p.reservedIDs[id] {
				usable = append(usable, id)
			}
		}
		sort.Slice(usable, func(i, j int) bool { return usable[i] < usable[j] })
		p.triceToId[format] = usable
	}
}

// orderIdenticalIDs permutes only interchangeable IDs. The existing allocator
// still selects new IDs; TIL entries and IDs outside the scope are retained.
func (p *idData) orderIdenticalIDs(sites []orderedIDSite) error {
	groups := make(map[TriceFmt][]int)
	for index, site := range sites {
		format, ok := p.idToTrice[site.id]
		if !ok || site.id <= 0 || p.reservedIDs[site.id] {
			return fmt.Errorf("cannot order ID %d at %s:%d: missing or reserved mapping", site.id, site.path, site.line)
		}
		groups[format] = append(groups[format], index)
		sites[index].key = orderedIDPath(site.path)
	}
	for format, indexes := range groups {
		pool := make(map[TriceID]bool)
		for _, index := range indexes {
			pool[sites[index].id] = true
			delete(p.idToLocNew, sites[index].id)
		}
		ids := make(TriceIDs, 0, len(pool))
		for id := range pool {
			if p.idAllowedForTrice(id, format) {
				ids = append(ids, id)
			}
		}
		sort.Slice(ids, func(i, j int) bool { return ids[i] < ids[j] })
		if len(ids) < len(indexes) {
			return fmt.Errorf("cannot order %d identical Trices: only %d eligible IDs", len(indexes), len(ids))
		}
		sort.Slice(indexes, func(i, j int) bool {
			a, b := sites[indexes[i]], sites[indexes[j]]
			if a.key != b.key {
				return a.key < b.key
			}
			if a.line != b.line {
				return a.line < b.line
			}
			return a.column < b.column
		})
		for rank, index := range indexes {
			sites[index].id = ids[rank]
			p.idToLocNew[ids[rank]] = newTriceLI(sites[index].path, sites[index].line)
		}
	}
	return nil
}

// orderBindIDs changes only generated Bind descriptors. Insert-owned sources
// participate in validation but their IDs cannot enter the interchangeable pool.
func orderBindIDs(plans []bindFilePlan) error {
	var sites []orderedIDSite
	var paths []string
	protected := make(map[TriceID]bool)
	for file, plan := range plans {
		if plan.class != bindFileBound {
			for _, site := range plan.sites {
				if site.id > 0 {
					protected[site.id] = true
				}
			}
			continue
		}
		paths = append(paths, plan.path)
		for ordinal, site := range plan.sites {
			sites = append(sites, orderedIDSite{file: file, site: ordinal, path: plan.path, line: site.line, column: site.column, id: site.id})
		}
	}
	IDData.reserveIDsOutside(paths, protected)
	if err := IDData.orderIdenticalIDs(sites); err != nil {
		return err
	}
	for _, site := range sites {
		plans[site.file].sites[site.site].id = site.id
	}
	return nil
}

// insertIDsInOrder reads concurrently, deduplicates overlapping roots, and
// allocates in source order in memory. No source is written until all files
// validate and the complete identical-site mapping is known. Cache handling
// and source/configuration publication continue through their existing paths.
func (p *idData) insertIDsInOrder(w io.Writer, fs *afero.Afero, action ant.Processing) error {
	p.err = nil
	inputs := make(map[string]bindFileInput)
	admin := &ant.Admin{Trees: Srcs, ExcludeTrees: ExcludeSrcs, MatchingFileName: isSourceFile}
	admin.Action = func(_ io.Writer, fs *afero.Afero, source string, info os.FileInfo, a *ant.Admin) error {
		absolute, err := filepath.Abs(source)
		if err != nil {
			return err
		}
		data, err := fs.ReadFile(source)
		if err != nil {
			return err
		}
		a.Mutex.Lock()
		defer a.Mutex.Unlock()
		// Overlapping absolute/relative roots must also choose a stable spelling
		// for diagnostics and publication, not whichever reader finishes last.
		spelling := filepath.Clean(source)
		if existing, found := inputs[absolute]; !found || spelling < existing.path {
			inputs[absolute] = bindFileInput{path: spelling, info: info, data: data}
		}
		return nil
	}
	if err := admin.Walk(w, fs); err != nil {
		return err
	}
	files := make([]bindFileInput, 0, len(inputs))
	var paths []string
	for _, input := range inputs {
		files = append(files, input)
		if !hasTriceBindSidecarInclude(string(input.data)) {
			paths = append(paths, input.path)
		}
	}
	sort.Slice(files, func(i, j int) bool { return orderedIDPath(files[i].path) < orderedIDPath(files[j].path) })
	p.PreProcessing(w, fs)
	p.reserveIDsOutside(paths, nil)
	defer func() { p.preparedInsert = nil; p.reservedIDs = nil }()
	outputs := make([][]byte, len(files))
	parsed := make([][]bindSite, len(files))
	var sites []orderedIDSite
	for index, input := range files {
		if hasTriceBindSidecarInclude(string(input.data)) {
			continue
		}
		out, _, err := p.insertTriceIDs(w, input.path, ToLIFile(input.path), input.data, admin)
		if err != nil {
			return err
		}
		outputs[index] = out
		parsed[index], _ = scanSourceSites(input.path, string(out), false)
		for ordinal, site := range parsed[index] {
			if site.id > 0 {
				sites = append(sites, orderedIDSite{file: index, site: ordinal, path: input.path, line: site.line, column: site.column, id: site.id})
			}
		}
	}
	if err := p.orderIdenticalIDs(sites); err != nil {
		return err
	}
	edits := make([][]sourceEdit, len(files))
	for _, site := range sites {
		original := parsed[site.file][site.site]
		if original.id != site.id {
			edits[site.file] = append(edits[site.file], idSourceEdit(string(outputs[site.file]), 0, original.loc[:], bindSiteFormat(original), site.id))
		}
	}
	p.preparedInsert = make(map[string][]byte)
	for index, input := range files {
		if outputs[index] == nil {
			continue
		}
		sort.Slice(edits[index], func(i, j int) bool { return edits[index][i].start < edits[index][j].start })
		out, err := applySourceEdits(string(outputs[index]), edits[index])
		if err != nil {
			return err
		}
		if filepath.Base(input.path) == "triceConfig.h" {
			out = strings.Replace(out, "#define TRICE_CLEAN 1", "#define TRICE_CLEAN 0", 1)
		}
		p.preparedInsert[input.path] = []byte(out)
	}
	for _, input := range files {
		if err := action(w, fs, input.path, input.info, admin); err != nil {
			return err
		}
	}
	if p.err != nil {
		return p.err
	}
	return p.postProcessing(w, fs)
}
