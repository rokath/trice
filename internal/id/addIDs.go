// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

// source tree management

import (
	"fmt"
	"io"
	"os"
	"path/filepath"

	"github.com/rokath/trice/pkg/ant"
	"github.com/spf13/afero"
)

// SubCmdIdZero performs sub-command zero, setting trice IDs in source tree to 0.
func SubCmdIdAdd(w io.Writer, fSys *afero.Afero) error {
	return IDData.cmdSwitchTriceIDs(w, fSys, triceIDAdding)
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

	var liPath string

	if LiPathIsRelative {
		liPath = filepath.ToSlash(path)
	} else {
		liPath = filepath.Base(path)
	}

	_, _, err = zeroTriceIDs(w, liPath, in, a)
	return err
}
