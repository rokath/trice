// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

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

// toLIPath converts path according to global variable LIPathKind into a base or relative or absolute path.
func toLIPath(path string) string {
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

// triceIDAdding reads file, processes it and writes it NOT back.
func triceIDAdding(w io.Writer, fSys *afero.Afero, path string, fileInfo os.FileInfo, a *ant.Admin) error {

	in, err := fSys.ReadFile(path)
	if err != nil {
		return err
	}
	if Verbose {
		fmt.Fprintln(w, path)
	}
	_, _, err = zeroTriceIDs(w, toLIPath(path), in, a) // just to get the IDs, no write back
	return err
}
