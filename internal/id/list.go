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
)

// ScZero does replace all ID's in source tree with 0
func ScZero(w io.Writer, SrcZ string, cmd *flag.FlagSet) error {
	if SrcZ == "" {
		cmd.PrintDefaults()
		return errors.New("no source tree root specified")
	}
	ZeroSourceTreeIds(w, SrcZ, !DryRun)
	return nil
}

// SubCmdReNewList renews the trice id list parsing the source tree without changing any source file.
// It creates a new FnJSON and tries to add id:tf pairs from the source tree.
// If equal tf are found with different ids they are all added.
// If the same id is found with different tf only one is added. The others are reported as warning.
// If any TRICE* is found without Id(n) or with Id(0) it is ignored.
// SubCmdUpdate needs to know which IDs are used in the source tree to reliable add new IDs.
func SubCmdReNewList(w io.Writer) (err error) {
	lu := make(TriceIDLookUp)
	return updateList(w, lu)
}

// SubCmdRefreshList refreshes the trice id list parsing the source tree without changing any source file.
// It only reads FnJSON and tries to add id:tf pairs from the source tree.
// If equal tf are found with different ids they are all added.
// If the same id is found with different tf only one is added. The others are reported as warning.
// If any TRICE* is found without Id(n) or with Id(0) it is ignored.
// SubCmdUpdate needs to know which IDs are used in the source tree to reliable add new IDs.
func SubCmdRefreshList(w io.Writer) (err error) {
	lu := NewLut(w, FnJSON)
	return updateList(w, lu)
}

func refreshListAdapter(w io.Writer, root string, lu TriceIDLookUp, tflu TriceFmtLookUp, _ *bool) {
	refreshList(w, root, lu, tflu)
}

func updateList(w io.Writer, lu TriceIDLookUp) error {
	tflu := lu.reverse()

	// keep a copy
	lu0 := make(TriceIDLookUp)
	for k, v := range lu {
		lu0[k] = v
	}
	var listModified bool
	walkSrcs(w, refreshListAdapter, lu, tflu, &listModified)

	// listModified does not help here, because it indicates that some sources are updated and therefore the list needs an update too.
	// But here we are only scanning the source tree, so if there would be some changes they are not relevant because sources are not changed here.
	// And if all
	eq := reflect.DeepEqual(lu0, lu)

	if Verbose {
		fmt.Fprintln(w, len(lu0), " -> ", len(lu), "ID's in List", FnJSON)
	}
	if !eq && !DryRun {
		msg.FatalOnErr(lu.toFile(FnJSON))
	}

	return nil // SubCmdUpdate() // to do
}

// SubCmdUpdate is sub-command update
func SubCmdUpdate(w io.Writer) error {
	lu := NewLut(w, FnJSON)
	tflu := lu.reverse()
	var listModified bool
	o := len(lu)
	walkSrcs(w, IDsUpdate, lu, tflu, &listModified)
	if Verbose {
		fmt.Fprintln(w, len(lu), "ID's in List", FnJSON, "listModified=", listModified)
	}

	if (len(lu) != o || listModified) && !DryRun {
		msg.FatalOnErr(lu.toFile(FnJSON))
	}
	return nil
}

func walkSrcs(w io.Writer, f func(w io.Writer, root string, lu TriceIDLookUp, tflu TriceFmtLookUp, pListModified *bool), lu TriceIDLookUp, tflu TriceFmtLookUp, pListModified *bool) {
	if 0 == len(Srcs) {
		Srcs = append(Srcs, "./") // default value
	}
	for i := range Srcs {
		s := Srcs[i]
		srcU := ConditionalFilePath(s)
		if _, err := os.Stat(srcU); err == nil { // path exists
			f(w, srcU, lu, tflu, pListModified)
		} else if os.IsNotExist(err) { // path does *not* exist
			fmt.Fprintln(w, s, " -> ", srcU, "does not exist!")
		} else {
			fmt.Fprintln(w, s, "Schrodinger: file may or may not exist. See err for details.")
			// Therefore, do *NOT* use !os.IsNotExist(err) to test for file existence
			// https://stackoverflow.com/questions/12518876/how-to-check-if-a-file-exists-in-go
		}
	}
}
