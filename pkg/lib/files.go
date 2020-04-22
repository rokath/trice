// Package tst contains no runtime code, only universal helper for tests
package lib

import (
	"bufio"
	"fmt"
	"io"
	"io/ioutil"
	"os"
	"path/filepath"
	"testing"
)

// https://stackoverflow.com/questions/7424340/read-in-lines-in-a-text-file-sort-then-overwrite-file
//

// WriteLines writes string slice containing lines to file
func ReadLines(file string) ([]string, error) {
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

/* SortLines reads inFile, sorts the lines and writes to outFile
// inFile and outFile can be the same
func SortLines(inFile, outFile string) {
    lines, err := ReadLines(inFile)
    if err != nil {
        fmt.Println(err)
        os.Exit(1)
    }
    sort.Strings(lines)
    err = WriteLines(OutFile, lines)
    if err != nil {
        fmt.Println(err)
        os.Exit(1)
    }
}
*/

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

// UniqLines reads inFile, removes duplicate lines and writes to outFile
// inFile and outFile can be the same
// it returns count of remaining lines plus 1 (io.EOF ?)
func UniqLines(t *testing.T, inFile, outFile string) int {
	il, err := ReadLines(inFile)
	Ok(t, err)
	ol := uniqueString(il)
	err = WriteLines(outFile, ol)
	Ok(t, err)
	return len(ol)
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
/* MIT License
 *
 * Copyright (c) 2017 Roland Singer [roland.singer@desertbit.com]
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
//

// CopyFile copies the contents of the file named src to the file named
// by dst. The file will be created if it does not already exist. If the
// destination file exists, all it's contents will be replaced by the contents
// of the source file. The file mode will be copied from the source and
// the copied data is synced/flushed to stable storage.
func CopyFile(src, dst string) (err error) {
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

			err = CopyFile(srcPath, dstPath)
			if err != nil {
				return
			}
		}
	}

	return
}

//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CleanFile accepts a string representing a path and converts \r\n into \n
// (taken from https://github.com/cgati/dos2unix/blob/master/main.go)
func CleanFile(fileName string) {
	file, err := os.Open(fileName)
	if err != nil {
		panic(err)
	}
	defer file.Close()

	wFile, err := os.Create(fileName + ".clean")
	if err != nil {
		panic(err)
	}
	defer wFile.Close()
	w := bufio.NewWriter(wFile)

	data := make([]byte, 128)
	for {
		data = data[:cap(data)]
		n, err := file.Read(data)
		if err != nil {
			if err == io.EOF {
				break
			}
		}
		data = data[:n]
		var indexesToDelete []int
		for i, b := range data {
			if b == '\r' {
				if i+1 < len(data) {
					if data[i+1] == '\n' {
						indexesToDelete = append(indexesToDelete, i)
					}
				}
				data[i] = '\n'
			}
		}
		data = removeFromSlice(data, indexesToDelete)
		w.Write(data)
	}
	w.Flush()
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

/*
func fileLine() string {
	_, fileName, fileLine, ok := runtime.Caller(1)
	var s string
	if ok {
		s = fmt.Sprintf("%s:%d", fileName, fileLine)
	} else {
		s = ""
	}
	return s
}
*/
