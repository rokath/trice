// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

// source tree management

import (
	"fmt"
	"io"
	"os"
	"path/filepath"
	"sync"

	"github.com/rokath/trice/pkg/msg"
	"github.com/spf13/afero"
)

// actionOnFile is a function type, executed for each file inside the visit function.
type actionOnFile func(w io.Writer, fSys *afero.Afero, path string, fileInfo os.FileInfo, idd *idData)

// idData holds the Id specific data.
type idData struct {
	idToFmt    TriceIDLookUp     // idToFmt is a trice ID lookup map and is generated from existing til.json file at the begin of SubCmdIdInsert. This map is only extended during SubCmdIdInsert and goes back into til.json afterwards.
	fmtToId    triceFmtLookUp    // fmtToId is a trice fmt lookup map (reversed idToFmt for faster operation) and kept in sync with idToFmt. Each fmt can have several trice IDs (slice).
	idToLocRef TriceIDLookUpLI   // idToLocInf is the trice ID location information as reference generated from li.json (if exists) at the begin of SubCmdIdInsert and is not modified at all. At the end of SubCmdIdInsert a new li.json is generated from itemToId.
	itemToId   TriceItemLookUpID // itemToId is a trice item lookup ID map, extended from source tree during SubCmdIdInsert after each found and maybe modified trice item.
	idToItem   TriceIDLookupItem // idToItem is a trice ID lookup item map (reversed itemToId for faster operation) and kept in sync with itemToId.
	wg         sync.WaitGroup    // wg is sync medium for parallel processing.
	action     actionOnFile      // action is the function executed on each file in passed source tree.
}

// ActionCmd performs f on each file in source tree.
func ActionCmd(w io.Writer, fSys *afero.Afero, f actionOnFile) error {
	var idd idData

	// get state
	idd.idToFmt = NewLut(w, fSys, FnJSON)
	idd.fmtToId = idd.idToFmt.reverseS()
	oLen := len(idd.idToFmt)
	idd.idToLocRef = make(TriceIDLookUpLI, 4000)

	// prepare
	idd.itemToId = make(TriceItemLookUpID, 4000)
	idd.idToItem = make(TriceIDLookupItem, 4000)
	if len(Srcs) == 0 {
		Srcs = append(Srcs, "./") // default value
	}
	idd.action = f

	// processing Srcs list ...
	for _, path := range Srcs {
		if _, err := fSys.Stat(path); err == nil { // path exists
			path := path // make a copy for each go routine
			idd.wg.Add(1)
			go func() {
				defer idd.wg.Done()
				msg.FatalInfoOnErr(fSys.Walk(path, visit(w, fSys, &idd)), "failed to walk tree")
			}()
		} else if os.IsNotExist(err) { // path does *not* exist
			fmt.Fprintln(w, path, "does not exist!")
		} else {
			fmt.Fprintln(w, path, "Schrodinger: file may or may not exist. See err for details.")
			// Therefore, do *NOT* use !os.IsNotExist(err) to test for file existence
			// https://stackoverflow.com/questions/12518876/how-to-check-if-a-file-exists-in-go
		}
	}

	// ...waiting
	idd.wg.Wait()

	// finalize
	if Verbose {
		fmt.Fprintln(w, len(idd.idToFmt), "ID's in List", FnJSON)
	}
	if (len(idd.idToFmt) != oLen) && !DryRun {
		msg.FatalOnErr(idd.idToFmt.toFile(fSys, FnJSON))
	}
	if LIFnJSON == "off" || LIFnJSON == "none" {
		return nil
	}
	li := idd.idToLocRef // todo: derive from iid.idToItem
	return li.toFile(fSys, LIFnJSON)
}

// visit is passed to fSys.Walk and executed for each file found in the processed root folder.
// To speed processing up, for each file a go routine is started.
// Error handling is done through abort.
func visit(w io.Writer, fSys *afero.Afero, idd *idData) filepath.WalkFunc {
	// WalkFunc is the type of the function called for each file or directory
	// visited by Walk. The path argument contains the argument to Walk as a
	// prefix; that is, if Walk is called with "dir", which is a directory
	// containing the file "a", the walk function will be called with argument
	// "dir/a". The info argument is the os.FileInfo for the named path.
	//
	// If there was a problem walking to the file or directory named by path, the
	// incoming error will describe the problem and the function can decide how
	// to handle that error (and Walk will not descend into that directory). In the
	// case of an error, the info argument will be nil. If an error is returned,
	// processing stops. The sole exception is when the function returns the special
	// value SkipDir. If the function returns SkipDir when invoked on a directory,
	// Walk skips the directory's contents entirely. If the function returns SkipDir
	// when invoked on a non-directory file, Walk skips the remaining files in the
	// containing directory.
	return func(path string, fileInfo os.FileInfo, err error) error {

		if err != nil || fileInfo.IsDir() || !isSourceFile(fileInfo) {
			return err // forward any error and do nothing
		}

		idd.wg.Add(1)
		go func() {
			defer idd.wg.Done()
			idd.action(w, fSys, path, fileInfo, idd)
		}()

		return nil
	}
}
