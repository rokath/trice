package id

import (
	"io"
	"os"
	"path/filepath"
	"strings"

	"github.com/rokath/trice/pkg/ant"
	"github.com/rokath/trice/pkg/msg"
	"github.com/spf13/afero"
)

// triceIDCleaning reads file, processes it and writes it back, if needed and uses cache if possible.
func (p *idData) triceIDCleaning(w io.Writer, fSys *afero.Afero, path string, fileInfo os.FileInfo, a *ant.Admin) error {
	if p.err != nil {
		return p.err
	}
	///////////////////////////////////////////////////////////////////////////////
	// cache stuff:
	//
	var err error
	var cacheExists bool
	var cleanedCachePath string
	if TriceCacheEnabled {
		cache := filepath.Join(UserHomeDir, ".trice/cache")

		if _, err = fSys.Stat(cache); err == nil { // cache folder exists
			// This cache code works in conjunction with the cache code in function triceIDInsertion.
			cacheExists = true
			fullPath, err := filepath.Abs(path)
			p.join(err)

			// remove first colon, if exists (Windows)
			before, after, _ := strings.Cut(fullPath, ":")
			home, _ := os.UserHomeDir()
			if UserHomeDir != home {
				before = "" // Throw away windows drive letter, when running tests with afero file system.
			}
			fullPath = before + after

			// Construct insertedCachePath.
			insertedCachePath := filepath.Join(cache, insertedCacheFolderName, fullPath)
			// construct cleanedCachePath
			cleanedCachePath = filepath.Join(cache, cleanedCacheFolderName, fullPath)

			// If no cleanedCachePath, execute clean operation
			cCache, err := fSys.Stat(cleanedCachePath)
			if err != nil {
				msg.Tell(w, "no cleaned Cache file")
				goto clean
			}

			// If path content equals cleanedCachePath content, we are done.
			if fileInfo.ModTime() == cCache.ModTime() {
				msg.Tell(w, "trice c was executed before, nothing to do")
				return msg.OnErrFv(w, p.err) // `trice c File`: File == cCache ? done
			}

			// If no insertedCachePath, execute clean operation.
			iCache, err := fSys.Stat(insertedCachePath)
			if err != nil {
				msg.Tell(w, "no inserted Cache file")
				goto clean
			}

			// If path content equals insertedCachePath content, we can copy cleanedCachePath to path.
			// We know here, that cleanedCachePath exists and path was not edited.
			if fileInfo.ModTime() == iCache.ModTime() && fileExists(fSys, cleanedCachePath) {
				// trice i File: File == iCache ? cCache -> F (trice c was executed before)

				msg.Tell(w, "trice c was executed before, copy cCache into file")
				err = CopyFileWithMTime(fSys, path, cleanedCachePath)
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

clean:
	err = p.processTriceIDCleaning(w, fSys, path, fileInfo, a)
	p.join(err)

	///////////////////////////////////////////////////////////////////////////////
	// cache stuff:
	//
	if TriceCacheEnabled && cacheExists && p.err == nil {
		// The file could have been modified by the user but if IDs are not touched, modified is false.
		// So we need to update the cache also when modified is false.
		msg.Tell(w, "Copy file into the cleaned-cache.")
		err = fSys.MkdirAll(filepath.Dir(cleanedCachePath), os.ModeDir)
		p.join(err)
		err = CopyFileWithMTime(fSys, cleanedCachePath, path)
		p.join(err)
	}
	//
	///////////////////////////////////////////////////////////////////////////////

	return msg.OnErrFv(w, p.err)
}
