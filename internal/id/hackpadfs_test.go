// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

import (
	"fmt"
	goOS "os"
	"testing"

	"github.com/hack-pad/hackpadfs/os"
	"github.com/tj/assert"
)

func Test_h1(t *testing.T) {

	filePath := "tmp/hello.txt"
	fs := os.NewFS()
	file, _ := fs.Open(filePath)
	defer file.Close()

	buffer := make([]byte, 1024)
	n, _ := file.Read(buffer)
	fmt.Println("Contents of hello.txt:", string(buffer[:n]))
	//assert.True(t, false)
}

func Test_h2(t *testing.T) {
	//t.Parallel()
	nfs := os.NewFS()
	workingDirectory, _ := goOS.Getwd()  // Get current working directory
	fSys, e := nfs.Sub(workingDirectory) // Run all file system operations rooted at the current working directory
	assert.Nil(t, e)
	fmt.Println(fSys)
	//xfs := fs.FS(fSys)
	//ySys, e := fs.Sub(xfs, workingDirectory) // Run all file system operations rooted at the current working directory
	//assert.Nil(t, e)
	//fmt.Println(ySys)
	//assert.True(t, false)
}

/* PASS in debug mode, FAIL in run mode !
func Test_h3(t *testing.T) {
	fs, e := mem.NewFS()
	assert.Nil(t, e)
	//time.Sleep(1000 * time.Millisecond)
	e = helloWorld(fs)
	assert.Nil(t, e)
	//time.Sleep(1000 * time.Millisecond)
}

// helloWorld uses a generic hackpadfs.FS to create a file containing "world".
// Returns an error if 'fs' does not support creating or writing to files.
func helloWorld(fs hackpadfs.FS) error {
	// hackpadfs.Create(...) checks to see if 'fs' implements Create, with a few fallback interfaces as well.
	file, err := hackpadfs.Create(fs, "hello.txt")
	if err != nil {
		return err
	}
	//time.Sleep(1000 * time.Millisecond)
	// Same here for hackpadfs.WriteFile(...). If the file doesn't support writing, a "not implemented" error is returned.
	_, err = hackpadfs.WriteFile(file, []byte("world"))
	//time.Sleep(1000 * time.Millisecond)
	return err
}
*/
