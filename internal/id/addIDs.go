// SPDX-License-Identifier: MIT

package id

// source tree management

import (
	"fmt"
	"io"
	"os"
	"path"
	"path/filepath"
	"strings"

	"github.com/rokath/trice/pkg/ant"
	"github.com/spf13/afero"
)

// SubCmdIdAdd performs sub-command add, setting getting trice IDs from source tree to til.json.
func SubCmdIdAdd(w io.Writer, fSys *afero.Afero) error {
	return IDData.cmdSwitchTriceIDs(w, fSys, triceIDAdding)
}

// ToLIFile returns a portable source path relative to the configured location root.
// If the source and root cannot be made relative, as on different Windows
// volumes, the normalized absolute source path is retained.
func ToLIFile(sourcePath string) string {
	absoluteSourcePath, err := filepath.Abs(sourcePath)
	if err != nil {
		return normalizeLocationPath(sourcePath)
	}
	relativePath, err := filepath.Rel(locationRoot(), absoluteSourcePath)
	if err != nil {
		return normalizeLocationPath(absoluteSourcePath)
	}
	return normalizeLocationPath(relativePath)
}

// locationRoot resolves an explicit root from the current working directory.
// Without an explicit root, the directory containing li.json is the anchor.
func locationRoot() string {
	root := LIRoot
	if root == "" {
		root = filepath.Dir(LIFnJSON)
	}
	absoluteRoot, err := filepath.Abs(root)
	if err != nil {
		return filepath.Clean(root)
	}
	return filepath.Clean(absoluteRoot)
}

// normalizeLocationPath makes a cleaned path portable without resolving symlinks.
func normalizeLocationPath(sourcePath string) string {
	return filepath.ToSlash(filepath.Clean(sourcePath))
}

// newTriceLI creates location information with one canonical source path.
func newTriceLI(sourcePath string, line int) TriceLI {
	return TriceLI{
		File: ToLIFile(sourcePath),
		Line: line,
	}
}

// LocationFile limits the displayed path to the requested trailing directories.
// Leading parent components only describe the storage root and are never shown.
func LocationFile(li TriceLI) string {
	cleanPath := path.Clean(strings.ReplaceAll(li.File, `\`, "/"))
	parts := strings.Split(cleanPath, "/")
	components := parts[:0]
	for _, component := range parts {
		if component == "" || component == "." {
			continue
		}
		components = append(components, component)
	}
	for len(components) > 1 && components[0] == ".." {
		components = components[1:]
	}
	if len(components) == 0 {
		return path.Base(cleanPath)
	}
	maxDirs := LIMaxDirs
	if maxDirs < 0 {
		maxDirs = 0
	}
	first := len(components) - 1 - maxDirs
	if first < 0 {
		first = 0
	}
	return strings.Join(components[first:], "/")
}

// triceIDAdding reads file, processes it and writes it NOT back.
func triceIDAdding(w io.Writer, fSys *afero.Afero, path string, fileInfo os.FileInfo, a *ant.Admin) error {

	in, err := fSys.ReadFile(path)
	if err != nil {
		return err
	}
	if Verbose {
		fmt.Fprintln(w, path)
	}
	_, _, err = zeroTriceIDs(w, ToLIFile(path), path, in, a) // just to get the IDs, no write back
	return err
}
