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

// Processing is a function type, executed for each file inside the visit function.
type Processing func(w io.Writer, fSys *afero.Afero, path string, fileInfo os.FileInfo, p *WalkData) error

// WalkData holds the walk specific data.
type WalkData struct {
	walkTreeData interface{}
	wg           sync.WaitGroup // wg is sync medium for parallel processing.
	action       Processing     // action is the function executed on each file in passed source tree.
	errorCount   int
}

// Walk performs f on each file in source tree.
func (p *WalkData) Walk(w io.Writer, fSys *afero.Afero) error {

	// processing Srcs list ...
	for _, path := range Srcs {
		if _, err := fSys.Stat(path); err == nil { // path exists
			path := path // make a copy for each go routine
			p.wg.Add(1)
			go func() {
				defer p.wg.Done()
				msg.FatalInfoOnErr(fSys.Walk(path, visit(w, fSys, p)), "failed to walk tree")
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
	p.wg.Wait()
	return nil
}

// visit is passed to fSys.Walk and executed for each file found in the processed root folder.
// To speed processing up, for each file a go routine is started.
// Error handling is done through abort.
func visit(w io.Writer, fSys *afero.Afero, wd *WalkData) filepath.WalkFunc {
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

		wd.wg.Add(1)
		go func() {
			defer wd.wg.Done()
			err := wd.action(w, fSys, path, fileInfo, wd)
			if err != nil {
				fmt.Fprintln(w, path, err)
				wd.errorCount++
			}
		}()

		return nil
	}
}
