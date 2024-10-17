package id

import (
	"io"
	"os"
	"path/filepath"
	"runtime"
	"strings"

	"github.com/rokath/trice/pkg/ant"
	"github.com/rokath/trice/pkg/msg"
	"github.com/spf13/afero"
)

// triceIDInsertion reads file, processes it and writes it back, if needed and uses cache if possible.
func (p *idData) triceIDInsertion(w io.Writer, fSys *afero.Afero, path string, fileInfo os.FileInfo, a *ant.Admin) error {
	if p.err != nil {
		return p.err
	}
	///////////////////////////////////////////////////////////////////////////////
	// cache stuff:
	//
	var err error
	var cacheExists bool
	var insertedCachePath string
	if TriceCacheEnabled {
		cache := filepath.Join(UserHomeDir, ".trice/cache")

		if _, err = fSys.Stat(cache); err == nil { // cache folder exists
			// This cache code works in conjunction with the cache code in function triceIDCleaning.
			cacheExists = true
			fullPath, err := filepath.Abs(path)
			p.join(err)

			// The drive letter of filepath.Abs(path) could be e but of os.UserHomeDir() could be c.
			// Remove first colon, if exists (Windows).
			before, after, found := strings.Cut(fullPath, ":")

			// Throw away drive letter, when testing on windows.
			if found && runtime.GOOS == "windows" && len(before) == 1 {
				home, err := os.UserHomeDir()
				if err != nil {
					return err
				}
				if home != UserHomeDir { // A test is running. (We modify UserHomeDir during tests.)
					before = ""
				}
			}
			fullPath = before + after // Remove colon if there is one.

			// Construct insertedCachePath.
			insertedCachePath := filepath.Join(cache, insertedCacheFolderName, fullPath)
			// construct cleanedCachePath
			cleanedCachePath := filepath.Join(cache, cleanedCacheFolderName, fullPath)

			// If no insertedCachePath, execute insert operation
			iCache, err := fSys.Stat(insertedCachePath)
			if err != nil {
				msg.Tell(w, "no inserted Cache file")
				goto insert
			}

			// If path content equals insertedCachePath content, we are nearly done.
			if fileInfo.ModTime() == iCache.ModTime() {
				msg.Tell(w, "trice i was executed before, nothing to do")
				return msg.OnErrFv(w, p.err) // `trice i File`: File == iCache ? done
			}

			// If no cleanedCachePath, execute insert operation.
			cCache, err := fSys.Stat(cleanedCachePath)
			if err != nil {
				msg.Tell(w, "no cleaned Cache file")
				goto insert
			}

			// If path content equals cleanedCachePath content, we can copy insertedCachePath to path.
			// We know here, that insertedCachePath exists and path was not edited.
			if fileInfo.ModTime() == cCache.ModTime() && fileExists(fSys, insertedCachePath) {
				// trice i File: File == cCache ? iCache -> F (trice i was executed before)

				msg.Tell(w, "trice c was executed before, copy iCache into file")
				err = CopyFileWithMTime(fSys, path, insertedCachePath)
				p.join(err)
				return msg.OnErrFv(w, p.err) // That's it.
			}
			msg.Tell(w, "File was edited, invalidate cache")
			fSys.Remove(insertedCachePath)
			fSys.Remove(cleanedCachePath)
		}
	}
	//
	///////////////////////////////////////////////////////////////////////////////

insert:
	err = p.processTriceIDInsertion(w, fSys, path, fileInfo, a)
	p.join(err)

	///////////////////////////////////////////////////////////////////////////////
	// cache stuff:
	//
	if TriceCacheEnabled && cacheExists && p.err == nil {
		// The file could have been modified by the user but if IDs are not touched, modified is false.
		// So we need to update the cache also when modified is false.
		msg.Tell(w, "Copy file into the inserted-cache.")
		err = fSys.MkdirAll(filepath.Dir(insertedCachePath), os.ModeDir)
		p.join(err)
		err = CopyFileWithMTime(fSys, insertedCachePath, path)
		p.join(err)
	}
	//
	///////////////////////////////////////////////////////////////////////////////

	return msg.OnErrFv(w, p.err)
}
