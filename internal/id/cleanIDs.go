// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

// source tree management

import (
	"fmt"
	"io"
	"os"

	"github.com/rokath/trice/pkg/ant"
	"github.com/spf13/afero"
)

// SubCmdIdClean performs sub-command clear, removing trice IDs from source tree.
func SubCmdIdClean(w io.Writer, fSys *afero.Afero) error {
	return cmdManageTriceIDs(w, fSys, triceIDCleaning)
}

// triceIDCleaning reads file, processes it and writes it back, if needed.
func triceIDCleaning(w io.Writer, fSys *afero.Afero, path string, fileInfo os.FileInfo, a *ant.Admin) error {

	in, err := fSys.ReadFile(path)
	if err != nil {
		return err
	}
	if Verbose {
		fmt.Fprintln(w, path)
	}

	out, fileModified, err := cleanTriceIDs(w, path, in, a)
	if err != nil {
		return err
	}

	if fileModified && !DryRun {
		if Verbose {
			fmt.Fprintln(w, "Changed: ", path)
		}
		err = fSys.WriteFile(path, out, fileInfo.Mode())
	}
	return err
}

// cleanTriceIDs does the ID clearance task on in, uses local pointer idd and returns the result in out with modified==true when any changes where made.
// For reference look into file TriceUserGuide.md part "The `trice insert` Algorithm".
func cleanTriceIDs(w io.Writer, path string, in []byte, a *ant.Admin) (out []byte, modified bool, err error) {

	a.Mutex.RLock()
	tfmt, found := idd.idToFmt[TriceID(1)]
	_, err = fmt.Fprintln(w, "cleaning in", path, found, tfmt)
	a.Mutex.RUnlock()

	return in, modified, err
}
