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
func doCHARableTest(t *testing.T, out io.Writer, f newDecoder, endianness bool, teTa testTable) {
	//var (
	//	// til is the trace id list content for test
	//	idl = ``
	//)
	//lu := make(id.TriceIDLookUp) // empty
	//luM := new(sync.RWMutex)
	//assert.Nil(t, lu.FromJSON([]byte(idl)))
	//lu.AddFmtCount(os.Stdout)
	buf := make([]byte, defaultSize)
	dec := f(out, nil, nil, nil, endianness) // a new decoder instance
	for _, x := range teTa {
		in := ioutil.NopCloser(bytes.NewBuffer(x.in))
		dec.setInput(in)
		lineStart := true
		var err error
		var n int
		var act string
		for err == nil {
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

func TestCHAR(t *testing.T) {
	tt := testTable{ // little endian
		{[]byte{'A', 'B', 'C', '0', '1', '2'}, `ABC012`},
		{[]byte{'a', 'b', '3', '4'}, `ab34`},
	}
	var out bytes.Buffer
	doCHARableTest(t, &out, newCHARDecoder, littleEndian, tt)
}
