package decoder

import (
	"bytes"
	"fmt"
	"io"
	"io/ioutil"
	"strings"
	"testing"

	"github.com/tj/assert"
)

// doTableTest is the universal decoder test sequence.
func doDUMPtableTest(t *testing.T, out io.Writer, f newDecoder, endianness bool, teTa testTable) {
	for _, x := range teTa {
		buf := make([]byte, defaultSize)
		dec := f(out, nil, nil, nil, endianness) // a new decoder instance
		in := ioutil.NopCloser(bytes.NewBuffer(x.in))
		dec.setInput(in)
		var err error
		var n int
		var act string
		for err == nil {
			n, err = dec.Read(buf)
			if n == 0 {
				break // end of test table
			}
			act += fmt.Sprint(string(buf[:n]))
		}
		act = strings.TrimSuffix(act, "\\n")
		act = strings.TrimSuffix(act, "\n")
		assert.Equal(t, x.exp, act)
	}
}

func TestDUMP(t *testing.T) {
	DumpLineByteCount = 8
	tt := testTable{ // little endian
		{[]byte{'A', 'B', 'C', '0', '1', '2'}, `41 42 43 30 31 32 `},
		{[]byte{0, 0, 0, 4, 0, 0, 0, 8, 0, 0, 0, 12, 0, 0, 0, 16, 0, 0, 0, 20, 0, 0, 0, 24, 0, 0, 0, 28, 0, 0, 0, 32, 33, 34}, "00 00 00 04 00 00 00 08 \\n00 00 00 0c 00 00 00 10 \\n00 00 00 14 00 00 00 18 \\n00 00 00 1c 00 00 00 20 \\n21 22 "},
	}
	var out bytes.Buffer
	doDUMPtableTest(t, &out, newDUMPDecoder, littleEndian, tt)
}
