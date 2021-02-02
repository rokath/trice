// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package id List is responsible for id List managing
package id

// List management

import (
	"errors"
	"flag"
	"fmt"
	"os"

	"github.com/rokath/trice/pkg/msg"
)

// ScZero does replace all ID's in source tree with 0
func ScZero(SrcZ string, cmd *flag.FlagSet) error {
	if SrcZ == "" {
		cmd.PrintDefaults()
		return errors.New("no source tree root specified")
	}
	ZeroSourceTreeIds(SrcZ, !DryRun)
	return nil
}

// SubCmdUpdate is subcommand update
func SubCmdUpdate() error {
	lu := NewLut(FnJSON)
	tflu := lu.reverse()
	var listModified bool

	if 0 == len(Srcs) {
		Srcs = append(Srcs, "./") // default value
	}

	if SharedIDs {
		update = sharedIDsUpdate
	} else {
		update = separatedIDsUpdate
	}

	for i := range Srcs {
		s := Srcs[i]
		srcU := ConditionalFilePath(s)
		if _, err := os.Stat(srcU); err == nil { // path exists
			update(srcU, lu, tflu, &listModified)
		} else if os.IsNotExist(err) { // path does *not* exist
			fmt.Println(s, " -> ", srcU, "does not exist!")
		} else {
			fmt.Println(s, "Schrodinger: file may or may not exist. See err for details.")
			// Therefore, do *NOT* use !os.IsNotExist(err) to test for file existence
			// https://stackoverflow.com/questions/12518876/how-to-check-if-a-file-exists-in-go
		}
	}

	if Verbose {
		fmt.Println(len(lu), "ID's in List", FnJSON, "listModified=", listModified)
	}
	if listModified && !DryRun {
		msg.FatalOnErr(lu.toFile(FnJSON))
	}
	return nil
}

var update func(string, TriceIDLookUp, TriceFmtLookUp, *bool)

// SubCmdRefreshList refreshes the trice id list parsing the source tree without changing any source file.
// It only reads FnJSON and tries to add id:tf pairs from the source tree.
// If equal tf are found with different ids they are all added.
// If the same id is found with different tf only one is added. The others are reported as warning.
// If any TRICE* is found without Id(n) or with Id(0) it is ignored.
// SubCmdUpdate needs to know which IDs are used in the source tree to reliable add new IDs.
func SubCmdRefreshList() (err error) {
	fmt.Println("To do: not omplemented yet!")
	return nil // SubCmdUpdate() // to do
}
