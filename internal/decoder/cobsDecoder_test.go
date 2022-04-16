// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package decoder

import (
	"bytes"
	"fmt"
	"io"
	"io/ioutil"
	"os"
	"strings"
	"sync"
	"testing"

	"github.com/rokath/trice/internal/id"
	"github.com/tj/assert"
)

// doTableTest is the universal decoder test sequence.
func doCOBSTableTest(t *testing.T, out io.Writer, f newDecoder, endianness bool, teTa testTable) {
	var (
		// til is the trace id list content for test
		idl = `{
			"48324": {
				"Type": "TRICE16",
				"Strg": "MSG: START select = %d, TriceDepthMax =%4u\\n"
			},
			"58755": {
				"Type": "TRICE32_1",
				"Strg": "rd:TRICE32_1 line %d (%%d)\\n"
			},
			"53709": {
				"Type": "TRICE16",
				"Strg": "MSG: STOP  select = %d, TriceDepthMax =%4u\\n"
			}
		}
	`
	)
	lu := make(id.TriceIDLookUp) // empty
	luM := new(sync.RWMutex)
	assert.Nil(t, lu.FromJSON([]byte(idl)))
	lu.AddFmtCount(os.Stdout)
	buf := make([]byte, defaultSize)
	dec := f(out, lu, luM, nil, endianness) // a new decoder instance
	for _, x := range teTa {
		in := ioutil.NopCloser(bytes.NewBuffer(x.in))
		dec.setInput(in)
		lineStart := true
		var err error
		var n int
		var act string
		for nil == err {
			n, err = dec.Read(buf)
			if n == 0 {
				break
			}
			if ShowID != "" && lineStart {
				act += fmt.Sprintf(ShowID, lastTriceID)
			}
			act += fmt.Sprint(string(buf[:n]))
			lineStart = false
		}
		act = strings.TrimSuffix(act, "\\n")
		act = strings.TrimSuffix(act, "\n")
		assert.Equal(t, x.exp, act)
	}
}

func TestCOBS(t *testing.T) {
	tt := testTable{ // little endian
		{[]byte{2, 1, 1, 1, 3, 208, 7, 1, 5, 192, 1, 196, 188, 1, 1, 1, 1, 0}, `MSG: START select = 0, TriceDepthMax =   0`},
		{[]byte{2, 1, 1, 1, 3, 209, 7, 1, 5, 193, 1, 205, 209, 1, 2, 28, 1, 0}, `MSG: STOP  select = 0, TriceDepthMax =  28`},
		{[]byte{0x02, 0x03, 0x01, 0x01, 0x02, 0x13, 0x0f, 0x38, 0xcb, 0x11, 0x11, 0x11, 0x11, 0xc0, 0x01, 0x83, 0xe5, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00}, `rd:TRICE32_1 line -1 (%d)`},
	}
	var out bytes.Buffer
	doCOBSTableTest(t, &out, newCOBSDecoder, littleEndian, tt)
	assert.Equal(t, "", out.String())
}

// used command to get sequences: "trice l -p COM1 -s -debug"
//        02 01 01 01 03 d0 07 01 05 c0 01 c4 bc 01 01 01 01 00 00 00 02 01 01 01 03 d1 07 01 05 c1 01 cd d1 01 02 1c 01 00 00 00
//  COBS: 02 01 01 01 03 d0 07 01 05 c0 01 c4 bc 01 01 01 01 00
//  -> PKG:  01 00 00 00 d0 07 00 00 c0 01 c4 bc 00 00 00 00
//  TRICE ->    00000001   timestamp c0 01 c4 bc 00 00 00 00
//  COBS:                                                       00
//  COBS:                                                          00
//  COBS:                                                             02 01 01 01 03 d1 07 01 05 c1 01 cd d1 01 02 1c 01 00
//  -> PKG:                                                              01 00 00 00 d1 07 00 00 c1 01 cd d1 00 00 1c 00
//  TRICE ->                                                                00000001   timestamp c1 01 cd d1 00 00 1c 00
//  COBS:                                                                                                                   00
//  COBS:                                                                                                                      00
//  func _TestTranslate(t *testing.T) {
//  	var (
//  		// til is the trace id list content for test
//  		idl = `{
//  			"48324": {
//  				"Type": "TRICE16",
//  				"Strg": "MSG: START select = %d, TriceDepthMax =%4u\\n"
//  			},
//  			"53709": {
//  				"Type": "TRICE16",
//  				"Strg": "MSG: STOP  select = %d, TriceDepthMax =%4u\\n"
//  			}
//  		}
//  	`
//  	)
//  	glob.Lock()
//  	defer glob.Unlock()
//  	sw := emitter.New(os.Stdout)
//  	lu := make(id.TriceIDLookUp) // empty
//  	assert.Nil(t, lu.FromJSON([]byte(idl)))
//  	m := new(sync.RWMutex) // m is a pointer to a read write mutex for lu
//  	Encoding = "COBS"
//  	TargetEndianness = "littleEndian"
//  	receiver.Port = "dumpDec"
//  	ShowID = "%d"
//  	defer func() {
//  		ShowID = "" // reset to default
//  	}()
//  	verbose := true
//
//  	buf := new(bytes.Buffer)
//  	rc, err := receiver.NewReadCloser(buf, verbose, receiver.Port, "02 01 01 01 03 d0 07 01 05 c0 01 c4 bc 01 01 01 01 00")
//  	assert.Nil(t, err)
//  	assert.Equal(t, buf.String(), "")
//  	err = Translate(buf, sw, lu, m, rc)
//  	assert.Nil(t, err)
//  	assert.Equal(t, "bla", buf.String())
//  }
//
//  var glob *sync.RWMutex // tests changing global values need to exclude each other
//
//  func init() {
//  	glob = new(sync.RWMutex)
//  }

/*
-> PKG:
COBS: 00
-> PKG:
Input(02 03 01 01 07 2d 01 37 a9 c8 20 01 02 54 09 42 b3 2f 01 37 a9 c8 20 01 06 55 01 ff 8f ef 01 01 07 2f 01 37 a9 c8 20 01 06 56 01 ff 8f 03 01 01 07 2f 01 37 a9 c8 20 01 06 57 01 ff 8f 9c 01 01 07 2f 01 37 a9 c8 20 01 06 58 01 ff 8f 40 01 01 07 2f 01 37 a9 c8 20 01 0
5 59 01 ff 8f 01 01 01 07 2f 01 37 a9 c8 20 01 06 5a 01 ff 8f 04 01 01 07 2f 01 37 a9 c8 20 01 06 5b 01 ff 8f 7c 01 01 07 2f 01 37 a9 c8 20 01 06 5c 01 ff 8f c3 01 01 07 31 01 37 a9 c8 20 01 02 5d 09 b2 a7 25 01 8e 91 c8 20 01 06 5e 03 dc b2 03 01 01 02 04 01 01 03 40 9c
 01 07 34 01 37 a9 c8 20 01 02 5f 09 d6 a5 36 01 37 a9 c8 20 01 06 60 01 c6 a5 ef 01 01 07 37 01 37 a9 c8 20 01 05 61 01 c6 a5 01 01 01 07 36 01 37 a9 c8 20 01 06 62 01 c6 a5 03 01 01 07 37 01 37 a9 c8 20 01 05 63 01 c6 a5 01 01 01 07 36 01 37 a9 c8 20 01 06 64 01 c6 a5
08 01 01 07 37 01 37 a9 c8 20 01 05 65 01 c6 a5 01 01 01 07 36 01 37 a9 c8 20 01 05 66 01 c6 a5 01 01 01 07 37 01 37 a9 c8 20 01 05 67 01 c6 a5 01 01 01 07 36 01 37 a9 c8 20 01 06 68 01 c6 a5 01 01 01 07 37 01 37 a9 c8 20 01 05 69 01 c6 a5 01 01 01 07 36 01 37 a9 c8 20 0
1 05 6a 01 c6 a5 01 01 01 07 37 01 37 a9 c8 20 01 05 6b 01 c6 a5 01 01 01 07 36 01 37 a9 c8 20 01 06 6c 01 c6 a5 04 01 01 07 37 01 37 a9 c8 20 01 05 6d 01 c6 a5 01 01 01 07 36 01 37 a9 c8 20 01 05 6e 01 c6 a5 01 01 01 07 37 01 37 a9 c8 20 01 05 6f 01 c6 a5 01 01 01 07 36
 01 37 a9 c8 20 01 06 70 01 c6 a5 02 01 01 07 37 01 37 a9 c8 20 01 05 71 01 c6 a5 01 01 01 07 36 01 37 a9 c8 20 01 05 72 01 c6 a5 01 01 01 07 37 01 37 a9 c8 20 01 05 73 01 c6 a5 01 01 01 07 36 01 37 a9 c8 20 01 06 74 01 c6 a5 ef 01 01 07 37 01 37 a9 c8 20 01 05 75 01 c6
a5 01 01 01 07 36 01 37 a9 c8 20 01 06 76 01 c6 a5 f7 01 01 07 37 01 37 a9 c8 20 01 05 77 01 c6 a5 01 01 01 07 36 01 37 a9 c8 20 01 06 78 01 c6 a5 b0 01 01 07 37 01 37 a9 c8 20 01 05 79 01 c6 a5 01 01 01 07 39 01 37 a9 c8 20 01 02 7a 03 b2 a7 00 00 00)
COBS: 02 03 01 01 07 2d 01 37 a9 c8 20 01 02 54 09 42 b3 2f 01 37 a9 c8 20 01 06 55 01 ff 8f ef 01 01 07 2f 01 37 a9 c8 20 01 06 56 01 ff 8f 03 01 01 07 2f 01 37 a9 c8 20 01 06 57 01 ff 8f 9c 01 01 07 2f 01 37 a9 c8 20 01 06 58 01 ff 8f 40 01 01 07 2f 01 37 a9 c8 20 01 0
5 59 01 ff 8f 01 01 01 07 2f 01 37 a9 c8 20 01 06 5a 01 ff 8f 04 01 01 07 2f 01 37 a9 c8 20 01 06 5b 01 ff 8f 7c 01 01 07 2f 01 37 a9 c8 20 01 06 5c 01 ff 8f c3 01 01 07 31 01 37 a9 c8 20 01 02 5d 09 b2 a7 25 01 8e 91 c8 20 01 06 5e 03 dc b2 03 01 01 02 04 01 01 03 40 9c
 01 07 34 01 37 a9 c8 20 01 02 5f 09 d6 a5 36 01 37 a9 c8 20 01 06 60 01 c6 a5 ef 01 01 07 37 01 37 a9 c8 20 01 05 61 01 c6 a5 01 01 01 07 36 01 37 a9 c8 20 01 06 62 01 c6 a5 03 01 01 07 37 01 37 a9 c8 20 01 05 63 01 c6 a5 01 01 01 07 36 01 37 a9 c8 20 01 06 64 01 c6 a5
08 01 01 07 37 01 37 a9 c8 20 01 05 65 01 c6 a5 01 01 01 07 36 01 37 a9 c8 20 01 05 66 01 c6 a5 01 01 01 07 37 01 37 a9 c8 20 01 05 67 01 c6 a5 01 01 01 07 36 01 37 a9 c8 20 01 06 68 01 c6 a5 01 01 01 07 37 01 37 a9 c8 20 01 05 69 01 c6 a5 01 01 01 07 36 01 37 a9 c8 20 0
1 05 6a 01 c6 a5 01 01 01 07 37 01 37 a9 c8 20 01 05 6b 01 c6 a5 01 01 01 07 36 01 37 a9 c8 20 01 06 6c 01 c6 a5 04 01 01 07 37 01 37 a9 c8 20 01 05 6d 01 c6 a5 01 01 01 07 36 01 37 a9 c8 20 01 05 6e 01 c6 a5 01 01 01 07 37 01 37 a9 c8 20 01 05 6f 01 c6 a5 01 01 01 07 36
 01 37 a9 c8 20 01 06 70 01 c6 a5 02 01 01 07 37 01 37 a9 c8 20 01 05 71 01 c6 a5 01 01 01 07 36 01 37 a9 c8 20 01 05 72 01 c6 a5 01 01 01 07 37 01 37 a9 c8 20 01 05 73 01 c6 a5 01 01 01 07 36 01 37 a9 c8 20 01 06 74 01 c6 a5 ef 01 01 07 37 01 37 a9 c8 20 01 05 75 01 c6
a5 01 01 01 07 36 01 37 a9 c8 20 01 06 76 01 c6 a5 f7 01 01 07 37 01 37 a9 c8 20 01 05 77 01 c6 a5 01 01 01 07 36 01 37 a9 c8 20 01 06 78 01 c6 a5 b0 01 01 07 37 01 37 a9 c8 20 01 05 79 01 c6 a5 01 01 01 07 39 01 37 a9 c8 20 01 02 7a 03 b2 a7 00
-> PKG:  03 00 00 00 2d 01 37 a9 c8 20 00 00 54 00 42 b3 2f 01 37 a9 c8 20 00 00 55 01 ff 8f ef 00 00 00 2f 01 37 a9 c8 20 00 00 56 01 ff 8f 03 00 00 00 2f 01 37 a9 c8 20 00 00 57 01 ff 8f 9c 00 00 00 2f 01 37 a9 c8 20 00 00 58 01 ff 8f 40 00 00 00 2f 01 37 a9 c8 20 00 0
0 59 01 ff 8f 00 00 00 00 2f 01 37 a9 c8 20 00 00 5a 01 ff 8f 04 00 00 00 2f 01 37 a9 c8 20 00 00 5b 01 ff 8f 7c 00 00 00 2f 01 37 a9 c8 20 00 00 5c 01 ff 8f c3 00 00 00 31 01 37 a9 c8 20 00 00 5d 00 b2 a7 25 01 8e 91 c8 20 00 00 5e 03 dc b2 03 00 00 00 04 00 00 00 40 9c
 00 00 34 01 37 a9 c8 20 00 00 5f 00 d6 a5 36 01 37 a9 c8 20 00 00 60 01 c6 a5 ef 00 00 00 37 01 37 a9 c8 20 00 00 61 01 c6 a5 00 00 00 00 36 01 37 a9 c8 20 00 00 62 01 c6 a5 03 00 00 00 37 01 37 a9 c8 20 00 00 63 01 c6 a5 00 00 00 00 36 01 37 a9 c8 20 00 00 64 01 c6 a5
08 00 00 00 37 01 37 a9 c8 20 00 00 65 01 c6 a5 00 00 00 00 36 01 37 a9 c8 20 00 00 66 01 c6 a5 00 00 00 00 37 01 37 a9 c8 20 00 00 67 01 c6 a5 00 00 00 00 36 01 37 a9 c8 20 00 00 68 01 c6 a5 01 00 00 00 37 01 37 a9 c8 20 00 00 69 01 c6 a5 00 00 00 00 36 01 37 a9 c8 20 0
0 00 6a 01 c6 a5 00 00 00 00 37 01 37 a9 c8 20 00 00 6b 01 c6 a5 00 00 00 00 36 01 37 a9 c8 20 00 00 6c 01 c6 a5 04 00 00 00 37 01 37 a9 c8 20 00 00 6d 01 c6 a5 00 00 00 00 36 01 37 a9 c8 20 00 00 6e 01 c6 a5 00 00 00 00 37 01 37 a9 c8 20 00 00 6f 01 c6 a5 00 00 00 00 36
 01 37 a9 c8 20 00 00 70 01 c6 a5 02 00 00 00 37 01 37 a9 c8 20 00 00 71 01 c6 a5 00 00 00 00 36 01 37 a9 c8 20 00 00 72 01 c6 a5 00 00 00 00 37 01 37 a9 c8 20 00 00 73 01 c6 a5 00 00 00 00 36 01 37 a9 c8 20 00 00 74 01 c6 a5 ef 00 00 00 37 01 37 a9 c8 20 00 00 75 01 c6
a5 00 00 00 00 36 01 37 a9 c8 20 00 00 76 01 c6 a5 f7 00 00 00 37 01 37 a9 c8 20 00 00 77 01 c6 a5 00 00 00 00 36 01 37 a9 c8 20 00 00 78 01 c6 a5 b0 00 00 00 37 01 37 a9 c8 20 00 00 79 01 c6 a5 00 00 00 00 39 01 37 a9 c8 20 00 00 7a 00 b2 a7
TRICE -> 54 00 42 b3
TRICE -> 55 01 ff 8f ef 00 00 00
TRICE -> 56 01 ff 8f 03 00 00 00
TRICE -> 57 01 ff 8f 9c 00 00 00
TRICE -> 58 01 ff 8f 40 00 00 00
TRICE -> 59 01 ff 8f 00 00 00 00
TRICE -> 5a 01 ff 8f 04 00 00 00
TRICE -> 5b 01 ff 8f 7c 00 00 00
TRICE -> 5c 01 ff 8f c3 00 00 00
TRICE -> 5d 00 b2 a7
Feb  4 16:03:26.380121  J-LINK:         Modbus.c: 301      8392 rx: ef 03 9c 40 00 04 7c c3
m
TRICE -> 5e 03 dc b2 03 00 00 00 04 00 00 00 40 9c 00 00
Feb  4 16:03:26.382627  J-LINK: ModbusFCHandler.c: 293      8392 FC3: Read 4 Holding Registers @40000
TRICE -> 5f 00 d6 a5
TRICE -> 60 01 c6 a5 ef 00 00 00
TRICE -> 61 01 c6 a5 00 00 00 00
TRICE -> 62 01 c6 a5 03 00 00 00
TRICE -> 63 01 c6 a5 00 00 00 00
TRICE -> 64 01 c6 a5 08 00 00 00
TRICE -> 65 01 c6 a5 00 00 00 00
TRICE -> 66 01 c6 a5 00 00 00 00
TRICE -> 67 01 c6 a5 00 00 00 00
TRICE -> 68 01 c6 a5 01 00 00 00
TRICE -> 69 01 c6 a5 00 00 00 00
TRICE -> 6a 01 c6 a5 00 00 00 00
TRICE -> 6b 01 c6 a5 00 00 00 00
TRICE -> 6c 01 c6 a5 04 00 00 00
TRICE -> 6d 01 c6 a5 00 00 00 00
TRICE -> 6e 01 c6 a5 00 00 00 00
TRICE -> 6f 01 c6 a5 00 00 00 00
TRICE -> 70 01 c6 a5 02 00 00 00
TRICE -> 71 01 c6 a5 00 00 00 00
TRICE -> 72 01 c6 a5 00 00 00 00
TRICE -> 73 01 c6 a5 00 00 00 00
TRICE -> 74 01 c6 a5 ef 00 00 00
TRICE -> 75 01 c6 a5 00 00 00 00
TRICE -> 76 01 c6 a5 f7 00 00 00
TRICE -> 77 01 c6 a5 00 00 00 00
TRICE -> 78 01 c6 a5 b0 00 00 00
TRICE -> 79 01 c6 a5 00 00 00 00
TRICE -> 7a 00 b2 a7
Feb  4 16:03:26.382627  J-LINK:         Modbus.c: 308      8392 tx: ef 00 03 00 08 00 00
38;5;101;40m00 01 00 00 00 04 00 00 00 02 00 00
m00 ef 00 f7 00 b0 00
COBS: 00
-> PKG:
COBS: 00
-> PKG:
*/
