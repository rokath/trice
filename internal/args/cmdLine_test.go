// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package args

import (
	"testing"
)

// needs to work in Linux and Windows!
func _TestCOBSLog(t *testing.T) {
	args := []string{"trice", "log", "-idList", "til.json", "-port", "FILEBUFFER", "-args", "C:/Users/T.Hoehenleitner/AppData/Local/Temp/trice-317444282.bin", "-ts", "off"}
	expect := ""
	execHelper(t, args, expect)
}

func containsAny(s, chars string) bool {
	for i := 0; i < len(s); i++ {
		for j := 0; j < len(chars); j++ {
			if s[i] == chars[j] {
				return true
			}
		}
	}
	return false
}

//  func Test2(t *testing.T) {
//  	var e error
//  	var fn string
//  	act := make([]byte, 1000)
//
//  	// G. 1.16 option A
//  	fSys := os.DirFS("/repos/trice/")
//  	fn = "GoInfos.txt"
//  	expA, e := fs.ReadFile(fSys, fn)
//  	assert.Nil(t, e)
//
//  	// G. 1.16 option B
//  	fSys = os.DirFS("/repos")
//  	fn = "trice/GoInfos.txt"
//  	expB, e := fs.ReadFile(fSys, fn)
//  	assert.Nil(t, e)
//  	assert.Equal(t, expB, expA)
//
//  	// G. 1.16 option C
//  	fSys = os.DirFS("")
//  	fn = "repos/trice/GoInfos.txt"
//  	expC, e := fs.ReadFile(fSys, fn)
//  	assert.Nil(t, e)
//  	assert.Equal(t, expC, expA)
//
//  	// legacy method
//  	fn = "/repos/trice/GoInfos.txt"
//  	fh, e := os.Open(fn)
//  	assert.Nil(t, e)
//  	n, e := fh.Read(act)
//  	assert.Nil(t, e)
//  	act = act[:n]
//  	e = fh.Close()
//  	assert.Nil(t, e)
//  	assert.Equal(t, expA, act)
//
//  	// depreciated
//  	fn = "/repos/trice/GoInfos.txt"
//  	act, e = ioutil.ReadFile(fn)
//  	assert.Nil(t, e)
//  	assert.Equal(t, expA, act)
//  }
//
//  type myFS struct {
//  	vfs.Filesystem // Embed the Filesystem interface and fill it with vfs.Dummy on creation
//  }
//
//  func MyFS() *myFS {
//  	return &myFS{
//  		vfs.Dummy(errors.New("Not implemented yet!")),
//  	}
//  }
//
//  func (fs myFS) Mkdir(name string, perm os.FileMode) error {
//  	// Create a directory
//  	// ...
//  	return nil
//  }
//
//  func Test1(t *testing.T) {
//  // Simply bootstrap your filesystem
//  var fs vfs.Filesystem = MyFS()
//
//  // Your mkdir implementation
//  fs.Mkdir("/tmp", 0777)
//
//  // All necessary methods like OpenFile (therefor Create) are stubbed
//  // and return the dummys error
//  _, err := vfs.Create(fs, "/tmp/vfs/example.txt")
//  }
