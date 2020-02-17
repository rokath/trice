// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// All rights reserved.
// Use of this source code is governed by a
// license that can be found in the LICENSE file.

package emit

import (
	"bytes"
	"fmt"
	"io"
	"io/ioutil"
	"os"
	"strings"
	"testing"

	"github.com/rokath/trice/pkg/id"
)

func TestPosition(t *testing.T) {
	fnIDList := "./testdata/til.json"        // file with test input
	fnIDListOut := "./testdata/position.out" // file with expected test output

	list := make(id.List, 0, 65536) // for 16 bit IDs enough
	pList := &list

	err := pList.Read(fnIDList)
	if nil != err {
		fmt.Println("ID list " + fnIDList + " not found, exit")
		t.Fail()
	}

	// https://stackoverflow.com/questions/10473800/in-go-how-do-i-capture-stdout-of-a-function-into-a-string
	old := os.Stdout // keep backup of the real stdout
	r, w, _ := os.Pipe()
	os.Stdout = w

	Check(list, "position", "off") // color "off"

	outC := make(chan string)
	// copy the output in a separate goroutine so printing can't block indefinitely
	go func() {
		var buf bytes.Buffer
		io.Copy(&buf, r)
		outC <- buf.String()
	}()

	// back to normal state
	w.Close()
	os.Stdout = old // restoring the real stdout
	out := <-outC

	os.Stdout = old

	// read expected output
	b, err := ioutil.ReadFile(fnIDListOut)
	if nil != err {
		fmt.Println("ID list " + fnIDListOut + " not found, exit")
		t.Fail()
	}
	s := string(b)
	s = strings.Replace(s, "\r", "", -1) // for Windows needed
	s = strings.Replace(s, "\n", "", -1)

	if out == s {
		return
	}
	fmt.Println("expected:")
	fmt.Println(s)
	fmt.Println("got:")
	fmt.Println(out)
	t.Fail()
}

func TestNegative(t *testing.T) {
	fnIDList := "./testdata/til.json"        // file with test input
	fnIDListOut := "./testdata/negative.out" // file with expected test output

	list := make(id.List, 0, 65536) // for 16 bit IDs enough
	pList := &list

	err := pList.Read(fnIDList)
	if nil != err {
		fmt.Println("ID list " + fnIDList + " not found, exit")
		t.Fail()
	}

	// https://stackoverflow.com/questions/10473800/in-go-how-do-i-capture-stdout-of-a-function-into-a-string
	old := os.Stdout // keep backup of the real stdout
	r, w, _ := os.Pipe()
	os.Stdout = w

	Check(list, "negative", "off") // color "off"

	outC := make(chan string)
	// copy the output in a separate goroutine so printing can't block indefinitely
	go func() {
		var buf bytes.Buffer
		io.Copy(&buf, r)
		outC <- buf.String()
	}()

	// back to normal state
	w.Close()
	os.Stdout = old // restoring the real stdout
	out := <-outC

	os.Stdout = old

	// read expected output
	b, err := ioutil.ReadFile(fnIDListOut)
	if nil != err {
		fmt.Println("ID list " + fnIDListOut + " not found, exit")
		t.Fail()
	}
	s := string(b)
	s = strings.Replace(s, "\r", "", -1) // for Windows needed
	s = strings.Replace(s, "\n", "", -1)

	if out == s {
		return
	}
	fmt.Println("expected:")
	fmt.Println(s)
	fmt.Println("got:")
	fmt.Println(out)
	t.Fail()
}
