// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package id List is responsible for id List managing
package id

// List management

import (
	"fmt"
	"io"
	"os"
	"reflect"

	"github.com/rokath/trice/pkg/msg"
	"github.com/spf13/afero"
)

// SubCmdAddToList extends the trice id list by parsing the source tree without changing any source file.
// It only reads FnJSON and tries to add id:tf pairs from the source tree.
// If equal tf are found with different ids they are all added.
// If the same id is found with different tf only one is added. The others are reported as warning.
// If any TRICE* is found without Id(n) or with Id(0), it is ignored.
func SubCmdAddToList(w io.Writer, fSys *afero.Afero) (err error) {
	lu := NewLut(w, fSys, FnJSON)
	lim := make(TriceIDLookUpLI, 4000)
	msg.OnErr(updateList(w, fSys, lu, lim))
	if LIFnJSON == "off" || LIFnJSON == "none" {
		return nil
	}
	// We do not want simply overwrite the legacy LI file, we want to extend it.
	li := NewLutLI(w, fSys, LIFnJSON) // Get legacy location information.
	for k, v := range lim {
		li[k] = v // Extend/modify li
	}
	return li.toFile(fSys, LIFnJSON)
}

func addToListAdapter(w io.Writer, fSys *afero.Afero, root string, ilu TriceIDLookUp, flu triceFmtLookUp, _ *bool, lim TriceIDLookUpLI) {
	refreshList(w, fSys, root, ilu, flu, lim)
}

func updateList(w io.Writer, fSys *afero.Afero, ilu TriceIDLookUp, lim TriceIDLookUpLI) error {
	flu := ilu.reverseS()

	// keep a copy
	lu0 := make(TriceIDLookUp)
	for k, v := range ilu {
		lu0[k] = v
	}
	var listModified bool
	walkSrcs(w, fSys, ilu, flu, &listModified, lim, addToListAdapter)

	// listModified does not help here, because it indicates that some sources are updated and therefore the list needs an update too.
	// But here we are only scanning the source tree, so if there would be some changes they are not relevant because sources are not changed here.
	eq := reflect.DeepEqual(lu0, ilu)

	if Verbose {
		fmt.Fprintln(w, len(lu0), " -> ", len(ilu), "ID's in List", FnJSON)
	}
	if !eq && !DryRun {
		msg.FatalOnErr(ilu.toFile(fSys, FnJSON))
	}

	return nil // SubCmdUpdate() // todo?
}

func walkSrcs(w io.Writer, fSys *afero.Afero /*******/, ilu TriceIDLookUp, flu triceFmtLookUp, pListModified *bool, lim TriceIDLookUpLI,
	f func(w io.Writer, fSys *afero.Afero, root string, ilu TriceIDLookUp, flu triceFmtLookUp, pListModified *bool, lim TriceIDLookUpLI)) {
	for i := range Srcs {
		s := Srcs[i]
		srcU := s
		if _, err := fSys.Stat(srcU); err == nil { // path exists
			f(w, fSys, srcU, ilu, flu, pListModified, lim)
		} else if os.IsNotExist(err) { // path does *not* exist
			fmt.Fprintln(w, s, " -> ", srcU, "does not exist!")
		} else {
			fmt.Fprintln(w, s, "Schrodinger: file may or may not exist. See err for details.")
			// Therefore, do *NOT* use !os.IsNotExist(err) to test for file existence
			// https://stackoverflow.com/questions/12518876/how-to-check-if-a-file-exists-in-go
		}
	}
}
