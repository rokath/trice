// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package args

import (
	"fmt"
	"io/fs"
	"os"
	"testing"

	"github.com/psanford/memfs"
)

func Test1(t *testing.T) {
	rootFS := memfs.New()

	err := rootFS.MkdirAll("dir1/dir2", 0777)
	if err != nil {
		panic(err)
	}

	err = rootFS.WriteFile("dir1/dir2/f1.txt", []byte("incinerating-unsubstantial"), 0755)
	if err != nil {
		panic(err)
	}

	err = fs.WalkDir(rootFS, ".", func(path string, d fs.DirEntry, err error) error {
		fmt.Println(path)
		return nil
	})
	if err != nil {
		panic(err)
	}

	os.ReadFile("dir1/dir2/f1.txt")

	content, err := fs.ReadFile(rootFS, "dir1/dir2/f1.txt")
	if err != nil {
		panic(err)
	}
	fmt.Printf("%s\n", content)
}
