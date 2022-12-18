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
