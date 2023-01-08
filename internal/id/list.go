// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package id List is responsible for id List managing
package id

// List management

import (
	"errors"
	"flag"
	"fmt"
	"io"
	"os"
	"reflect"

	"github.com/rokath/trice/pkg/msg"
	"github.com/spf13/afero"
)

// _ScZero does replace all ID's in source tree with 0
func _ScZero(w io.Writer, fSys *afero.Afero, SrcZ string, cmd *flag.FlagSet) error {
	if SrcZ == "" {
		cmd.PrintDefaults()
		return errors.New("no source tree root specified")
	}
	zeroSourceTreeIds(w, fSys, SrcZ, !DryRun)
	return nil
}

// SubCmdReNewList renews the trice id list parsing the source tree without changing any source file.
// It creates a new FnJSON and tries to add id:tf pairs from the source tree.
// If equal tf are found with different ids they are all added.
// If the same id is found with different tf only one is added. The others are reported as warning.
// If any TRICE* is found without Id(n) or with Id(0), it is ignored.
// SubCmdUpdate needs to know which IDs are used in the source tree, to reliably add new IDs.
func SubCmdReNewList(w io.Writer, fSys *afero.Afero) (err error) {
	lu := make(TriceIDLookUp)
	lim := make(TriceIDLookUpLI, 4000)
	msg.OnErr(updateList(w, fSys, lu, lim))
	return lim.toFile(fSys, LIFnJSON)
}

// SubCmdRefreshList refreshes the trice id list parsing the source tree without changing any source file.
// It only reads FnJSON and tries to add id:tf pairs from the source tree.
// If equal tf are found with different ids they are all added.
// If the same id is found with different tf only one is added. The others are reported as warning.
// If any TRICE* is found without Id(n) or with Id(0), it is ignored.
// SubCmdUpdate needs to know which IDs are used in the source tree, to reliably add new IDs.
func SubCmdRefreshList(w io.Writer, fSys *afero.Afero) (err error) {
	lu := NewLut(w, fSys, FnJSON)
	lim := make(TriceIDLookUpLI, 4000)
	msg.OnErr(updateList(w, fSys, lu, lim))
	return lim.toFile(fSys, LIFnJSON)
}

func refreshListAdapter(w io.Writer, fSys *afero.Afero, root string, lu TriceIDLookUp, tflus triceFmtLookUpS, _ *bool, lim TriceIDLookUpLI) {
	refreshList(w, fSys, root, lu, tflus, lim)
}

func updateList(w io.Writer, fSys *afero.Afero, lu TriceIDLookUp, lim TriceIDLookUpLI) error {
	tflus := lu.reverseS()

	// keep a copy
	lu0 := make(TriceIDLookUp)
	for k, v := range lu {
		lu0[k] = v
	}
	var listModified bool
	walkSrcs(w, fSys, lu, tflus, &listModified, lim, refreshListAdapter)

	// listModified does not help here, because it indicates that some sources are updated and therefore the list needs an update too.
	// But here we are only scanning the source tree, so if there would be some changes they are not relevant because sources are not changed here.
	eq := reflect.DeepEqual(lu0, lu)

	if Verbose {
		fmt.Fprintln(w, len(lu0), " -> ", len(lu), "ID's in List", FnJSON)
	}
	if !eq && !DryRun {
		msg.FatalOnErr(lu.toFile(fSys, FnJSON))
	}

	return nil // SubCmdUpdate() // todo?
}

// SubCmdUpdate is sub-command update
func SubCmdUpdate(w io.Writer, fSys *afero.Afero) error {
	lim := make(TriceIDLookUpLI, 4000)
	lu := NewLut(w, fSys, FnJSON)
	tflus := lu.reverseS()
	var listModified bool
	o := len(lu)
	walkSrcs(w, fSys, lu, tflus, &listModified, lim, idsUpdate)
	if Verbose {
		fmt.Fprintln(w, len(lu), "ID's in List", FnJSON, "listModified=", listModified)
	}

	if (len(lu) != o || listModified) && !DryRun {
		msg.FatalOnErr(lu.toFile(fSys, FnJSON))
	}
	return lim.toFile(fSys, LIFnJSON)
}

func walkSrcs(w io.Writer, fSys *afero.Afero, lu TriceIDLookUp, tflus triceFmtLookUpS, pListModified *bool, lim TriceIDLookUpLI, f func(w io.Writer, fSys *afero.Afero, root string, lu TriceIDLookUp, tflus triceFmtLookUpS, pListModified *bool, lim TriceIDLookUpLI)) {
	if len(Srcs) == 0 {
		Srcs = append(Srcs, "./") // default value
	}
	for i := range Srcs {
		s := Srcs[i]
		srcU := s                                  // FullFilePath2(fSys, s)
		if _, err := fSys.Stat(srcU); err == nil { // path exists
			f(w, fSys, srcU, lu, tflus, pListModified, lim)
		} else if os.IsNotExist(err) { // path does *not* exist
			fmt.Fprintln(w, s, " -> ", srcU, "does not exist!")
		} else {
			fmt.Fprintln(w, s, "Schrodinger: file may or may not exist. See err for details.")
			// Therefore, do *NOT* use !os.IsNotExist(err) to test for file existence
			// https://stackoverflow.com/questions/12518876/how-to-check-if-a-file-exists-in-go
		}
	}
}
