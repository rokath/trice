// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// package ant performs a function of type Processing on each file in several files, folders and sub-folders.
package ant

// source tree management

import (
	"errors"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"sync"

	"github.com/rokath/trice/pkg/msg"
	"github.com/spf13/afero"
)

// Processing is a function type, executed for each file inside the visit function.
type Processing func(w io.Writer, fSys *afero.Afero, path string, fileInfo os.FileInfo, p *Admin) error

// Admin holds the walk specific data.
type Admin struct {
	Trees            []string                  // Trees contains all files and folders to process. Sub-folders are walked automatically.
	MatchingFileName func(fi os.FileInfo) bool // MatchingFileName is a user provided function and returns true on matching user conditions. Simplest case: func(_ os.FileInfo){ return true } for all files.
	Action           Processing                // Action is the user provided function executed on each file in Trees.
	Mutex            sync.RWMutex              // A sync.RWMutex is thus preferable for data that is mostly read.
	wg               sync.WaitGroup            // wg is sync medium for parallel processing. Walk returns, when all parallel processed files done.
	errorCount       int                       // errorCount gets incremented by each started go routine on an error.
}

// Walk performs p.action on each file in passed srcs and all sub trees.
func (p *Admin) Walk(w io.Writer, fSys *afero.Afero) error {

	// processing tree list ...
	for _, path := range p.Trees {
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

	if p.errorCount > 0 {
		return errors.New(fmt.Sprint(p.errorCount, " walk errors"))
	}
	return nil
}

// visit is passed to fSys.Walk and executed for each file found in the processed root folder.
// To speed processing up, for each file a go routine is started.
// Error handling is done through abort.
func visit(w io.Writer, fSys *afero.Afero, jalan *Admin) filepath.WalkFunc {
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

		if err != nil || fileInfo.IsDir() || !jalan.MatchingFileName(fileInfo) {
			return err // forward any error and do nothing
		}

		jalan.wg.Add(1)
		go func() {
			defer jalan.wg.Done()
			err := jalan.Action(w, fSys, path, fileInfo, jalan)
			if err != nil {
				fmt.Fprintln(w, path, err)
				jalan.errorCount++
			}
		}()

		return nil
	}
}
