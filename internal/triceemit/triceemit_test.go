// +build x
// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package triceemit_test is a blackbox test.
package triceemit

import (
	"bytes"
	"testing"

	"github.com/rokath/trice/pkg/lib"
)

//
//func Test1(t *testing.T) {
//	c := com.New("COM4")
//	if !c.Open() {
//		return
//	}
//	p := triceemit.NewTriceReceiver(c, "bare")
//	tr := make([]triceemit.Trice, 1000)
//	n, _ := p.TriceRead(tr)
//	tr = tr[:n]
//	fmt.Println(tr)
//	t.Fail()
//}

func TestNewTriceReceiverfromBare(t *testing.T) {
	r := bytes.NewReader([]byte{'j', 'a', 'r', 1, 1, 1, 1, 0x16, 0x16, 0x16, 0x16, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4})
	p := NewTriceReceiverfromBare(r)
	var bFlag, tFlag bool
	for {
		select {
		case bAct := <-p.ignored:
			bExp := []byte{106, 97, 114}
			lib.Equals(t, bExp, bAct)
			bFlag = true
		case tAct := <-p.atoms:
			tExp := []Trice{
				{ID: 257, Value: [2]byte{1, 1}},
				{ID: 5654, Value: [2]byte{22, 22}},
				{ID: 514, Value: [2]byte{2, 2}},
				{ID: 771, Value: [2]byte{3, 3}},
			}
			lib.Equals(t, tExp, tAct)
			tFlag = true
		default:
			if bFlag && tFlag {
				lib.Equals(t, []byte{4, 4}, p.syncbuffer)
				return
			}
		}
	}
}

/*
func TestTriceItem(t *testing.T) {
	id.FnJSON = "./testdata/til.json"
	id.ReadListFile()
	r := bytes.NewReader([]byte{
		'j', 'a', 'r',
		0, 0, 11, 22, 0x10, 0x72, 33, 44,
		0x16, 0x16, 0x16, 0x16,
		4, 4})
	p := NewTriceReceiverfromBare(r)
	for 0 != bytes.Compare([]byte{4, 4}, p.syncbuffer) {
	} // wait until all is read
	time.Sleep(100 * time.Millisecond)
	tExp := id.Item{
		ID:      29200, //      int    `json:"id"`      // identifier
		FmtType: "",    // string `json:"fmtType"` // format type (bitsize and number of fmt string parameters)
		FmtStrg: "",    // string `json:"fmtStrg"` // format string
		Created: 0,     // int32  `json:"created"` // utc unix time of creation
		Removed: 0,     // int32  `json:"removed"` // utc unix time of disappearing in processed src directory
	}
	lib.Equals(t, tExp, p.item)
	lib.Equals(t, []byte{11, 22, 33, 44}, p.values)
	//	tExp := id.Item{
	//		ID:      29200,             //      int    `json:"id"`      // identifier
	//		FmtType: "TRICE32_1",       // string `json:"fmtType"` // format type (bitsize and number of fmt string parameters)
	//		FmtStrg: "time:ms = %d\\n", // string `json:"fmtStrg"` // format string
	//		Created: 1590414933,        // int32  `json:"created"` // utc unix time of creation
	//		Removed: 0,                 // int32  `json:"removed"` // utc unix time of disappearing in processed src directory
	//	}

}
*/
