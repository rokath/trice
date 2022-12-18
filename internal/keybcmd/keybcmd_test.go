// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// white-box test
package keybcmd

import (
	"bufio"
	"bytes"
	"strings"
	"testing"
)

func TestDummy(t *testing.T) {
}

// stimulate injects keys to the loop action and returns the captured output as byte slice.
func stimulate(keys, ipa, ipp string) []byte {

	// simulate input
	ioReader := strings.NewReader(keys)
	bufioReader := bufio.NewReader(ioReader)

	//  // capture output
	//  rescueStdout := os.Stdout
	//  r, w, _ := os.Pipe()
	//  os.Stdout = w

	var buf bytes.Buffer
	loopAction(bufioReader, &buf, ipa, ipp)

	// restore
	// msg.OnErr(w.Close())
	//captured, _ := ioutil.ReadAll(r)
	//os.Stdout = rescueStdout

	return buf.Bytes()
}

// todo: Linux

//  func TestHelp(t *testing.T) {
//  	exp := "-> h|help                   - this text\nexitServer|serverExit    - kill server\nq|quit                   - end program\n"
//  	b := stimulate("help\r\n", "", "")
//  	act := string(b)
//  	fmt.Println("*****************************************\n*****************************act=", act, "##############")
//  	fmt.Println("exp=", exp, "##############")
//  	assert.Equal(t, exp, act)
//  }

//  // baseName returns basic filename of program without extension
//  func baseName() string {
//  	s := strings.TrimSuffix(filepath.Base(os.Args[0]), filepath.Ext(os.Args[0]))
//  	return strings.TrimSuffix(s, ".test") // for Example tests only
//  }
