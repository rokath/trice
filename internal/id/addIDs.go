// SPDX-License-Identifier: MIT

package id

// source tree management

import (
	"fmt"
	"io"
	"log"
	"os"
	"path/filepath"

	"github.com/rokath/trice/pkg/ant"
	"github.com/spf13/afero"
)

// SubCmdIdAdd performs sub-command add, setting getting trice IDs from source tree to til.json.
func SubCmdIdAdd(w io.Writer, fSys *afero.Afero) error {
	return IDData.cmdSwitchTriceIDs(w, fSys, triceIDAdding)
}

// ToLIPath converts path according to global variable LIPathKind into a base or relative or absolute path.
func ToLIPath(path string) string {
	liPathK := filepath.Base(LIPathKind) // strip leading path info
	switch liPathK[:1] {
	case "r": // relative
		basePath := LIPathKind[:len(LIPathKind)-len(liPathK)]
		reloc, err := filepath.Rel(basePath, path)
		if err != nil {
			log.Fatal(err)
		}
		reloc = filepath.ToSlash(reloc)
		return reloc //filepath.Clean(reloc + path)
	case "f": // full
		full, err := filepath.Abs(path)
		if err != nil {
			fmt.Println("could not get absolute path of", path, "- using base name")
			return filepath.Base(path)
		}
		return full
	default:
		fmt.Println("ignoring unknown liPath", LIPathKind, "- using base name")
		fallthrough
	case "b": // base
		return filepath.Base(path)
	}
}

// newTriceLI creates location information without losing the complete source path.
func newTriceLI(sourcePath string, line int) TriceLI {
	return TriceLI{
		File: ToLIPath(sourcePath),
		Path: sourcePathRelativeToLI(sourcePath),
		Line: line,
	}
}

// sourcePathRelativeToLI normalizes sourcePath relative to the directory containing li.json.
func sourcePathRelativeToLI(sourcePath string) string {
	liRoot, err := filepath.Abs(filepath.Dir(LIFnJSON))
	if err != nil {
		return filepath.ToSlash(filepath.Clean(sourcePath))
	}
	absoluteSourcePath, err := filepath.Abs(sourcePath)
	if err != nil {
		return filepath.ToSlash(filepath.Clean(sourcePath))
	}
	relativePath, err := filepath.Rel(liRoot, absoluteSourcePath)
	if err != nil {
		return filepath.ToSlash(filepath.Clean(sourcePath))
	}
	return filepath.ToSlash(filepath.Clean(relativePath))
}

// LocationFile selects the source path representation requested for log output.
func LocationFile(li TriceLI) string {
	switch LIDisplayPathKind {
	case "base":
		if li.Path != "" {
			return filepath.Base(filepath.FromSlash(li.Path))
		}
		return filepath.Base(li.File)
	case "relative":
		if li.Path != "" {
			return li.Path
		}
		return li.File
	case "full":
		return fullLocationFile(li)
	case "legacy":
		fallthrough
	default:
		return li.File
	}
}

// fullLocationFile resolves portable location paths against the configured local root.
func fullLocationFile(li TriceLI) string {
	path := li.Path
	if path == "" {
		cleanFile := filepath.Clean(li.File)
		if filepath.IsAbs(cleanFile) {
			return cleanFile
		}
		if cleanFile == filepath.Base(cleanFile) {
			return li.File
		}
		path = filepath.ToSlash(cleanFile)
	}
	if filepath.IsAbs(filepath.FromSlash(path)) {
		return filepath.Clean(filepath.FromSlash(path))
	}
	root := LIRoot
	if root == "" {
		root = filepath.Dir(LIFnJSON)
	}
	absoluteRoot, err := filepath.Abs(root)
	if err != nil {
		return li.File
	}
	return filepath.Clean(filepath.Join(absoluteRoot, filepath.FromSlash(path)))
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
	_, _, err = zeroTriceIDs(w, ToLIPath(path), path, in, a) // just to get the IDs, no write back
	return err
}
