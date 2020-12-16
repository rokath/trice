// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package file helps with file handling.
package file

import (
	"bufio"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"os"
	"path/filepath"
	"runtime"
)

// BigReadLines reads all lines of a file into lines.
// https://stackoverflow.com/questions/7424340/read-in-lines-in-a-text-file-sort-then-overwrite-file
func BigReadLines(file string) ([]string, error) {
	var lines []string
	f, err := os.Open(file)
	if err != nil {
		return nil, err
	}
	defer f.Close()
	r := bufio.NewReader(f)
	for {
		const delim = '\n'
		line, err := r.ReadString(delim)
		if err == nil || len(line) > 0 {
			if err != nil {
				line += string(delim)
			}
			lines = append(lines, line)
		}
		if err != nil {
			if err == io.EOF {
				break
			}
			return nil, err
		}
	}
	return lines, nil
}

// ReadLines reads all lines of a file into lines.
var ReadLines = BigReadLines

// WriteLines writes string slice containing lines to file
func WriteLines(file string, lines []string) error {
	f, err := os.Create(file)
	if err != nil {
		return err
	}
	defer f.Close()
	w := bufio.NewWriter(f)
	defer w.Flush()
	for _, line := range lines {
		_, err := w.WriteString(line)
		if err != nil {
			return err
		}
	}
	return nil
}

// uniqueString removes string duplicates in string slice
// https://www.golangprograms.com/remove-duplicate-values-from-slice.html
func uniqueString(ss []string) []string {
	keys := make(map[string]bool)
	//list := []string{}
	var list []string
	for _, entry := range ss {
		if _, value := keys[entry]; !value {
			keys[entry] = true
			list = append(list, entry)
		}
	}
	return list
}

// errorFatal ends in osExit(1) if p.Err not nil.
func errorFatal(err error) {
	if nil == err {
		return
	}
	_, file, line, _ := runtime.Caller(1)
	log.Fatal(err, filepath.Base(file), line)
}

// removeFromSlice accepts a byte slice and an integer slice
// and deletes each index from the integer slice from the
// byte slice
// (taken from https://github.com/cgati/dos2unix/blob/master/main.go)
func removeFromSlice(data []byte, indexes []int) []byte {
	for _, i := range indexes {
		data = append(data[:i], data[i+1:]...)
	}
	return data
}

// Copy copies the contents of the file named src to the file named
// by dst. The file will be created if it does not already exist. If the
// destination file exists, all it's contents will be replaced by the contents
// of the source file. The file mode will be copied from the source and
// the copied data is synced/flushed to stable storage.
func Copy(src, dst string) (err error) {
	in, err := os.Open(src)
	if err != nil {
		return
	}
	defer in.Close()

	out, err := os.Create(dst)
	if err != nil {
		return
	}
	defer func() {
		if e := out.Close(); e != nil {
			err = e
		}
	}()

	_, err = io.Copy(out, in)
	if err != nil {
		return
	}

	err = out.Sync()
	if err != nil {
		return
	}

	si, err := os.Stat(src)
	if err != nil {
		return
	}
	err = os.Chmod(dst, si.Mode())
	if err != nil {
		return
	}

	return
}

// CopyDir recursively copies a directory tree, attempting to preserve permissions.
// Source directory must exist, destination directory must *not* exist.
// Symlinks are ignored and skipped.
func CopyDir(src string, dst string) (err error) {
	src = filepath.Clean(src)
	dst = filepath.Clean(dst)

	si, err := os.Stat(src)
	if err != nil {
		return err
	}
	if !si.IsDir() {
		return fmt.Errorf("source is not a directory")
	}

	_, err = os.Stat(dst)
	if err != nil && !os.IsNotExist(err) {
		return
	}
	if err == nil {
		return fmt.Errorf("destination already exists")
	}

	err = os.MkdirAll(dst, si.Mode())
	if err != nil {
		return
	}

	entries, err := ioutil.ReadDir(src)
	if err != nil {
		return
	}

	for _, entry := range entries {
		srcPath := filepath.Join(src, entry.Name())
		dstPath := filepath.Join(dst, entry.Name())

		if entry.IsDir() {
			err = CopyDir(srcPath, dstPath)
			if err != nil {
				return
			}
		} else {
			// Skip symlinks.
			if entry.Mode()&os.ModeSymlink != 0 {
				continue
			}

			err = Copy(srcPath, dstPath)
			if err != nil {
				return
			}
		}
	}

	return
}

// ReadString returns filename content as string
func ReadString(filename string) (s string) {
	b, err := ioutil.ReadFile(filename)
	errorFatal(err)
	s = string(b)
	return
}

// Random creates a random file containing s and returns its name.
// See ioutil.Tempfile() for dir and pattern.
func Random(s, dir, pattern string) string {
	fd, err := ioutil.TempFile(dir, pattern)
	errorFatal(err)
	fd.WriteString(s)
	fn := fd.Name()
	fd.Close()
	return fn
}
