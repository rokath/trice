package charDecoder

import (
	"bytes"
	"fmt"
	"io"
	"io/ioutil"
	"strings"
	"testing"

	"github.com/rokath/trice/internal/decoder"
	"github.com/tj/assert"
)

// doTableTest is the universal decoder test sequence.
func doCHARableTest(t *testing.T, out io.Writer, f decoder.New, endianness bool, teTa decoder.TestTable) {
	//var (
	//	// til is the trace id list content for test
	//	idl = ``
	//)
	//lu := make(id.TriceIDLookUp) // empty
	//luM := new(sync.RWMutex)
	//assert.Nil(t, ilu.FromJSON([]byte(idl)))
	//lu.AddFmtCount(os.Stdout)
	buf := make([]byte, decoder.DefaultSize)
	dec := f(out, nil, nil, nil, nil, endianness) // a new decoder instance
	for _, x := range teTa {
		in := ioutil.NopCloser(bytes.NewBuffer(x.In))
		dec.SetInput(in)
		lineStart := true
		var err error
		var n int
		var act string
		for err == nil {
			n, err = dec.Read(buf)
			if n == 0 {
				break
			}
			if decoder.ShowID != "" && lineStart {
				act += fmt.Sprintf(decoder.ShowID, decoder.LastTriceID)
			}
			act += fmt.Sprint(string(buf[:n]))
			lineStart = false
		}
		act = strings.TrimSuffix(act, "\\n")
		act = strings.TrimSuffix(act, "\n")
		assert.Equal(t, x.Exp, act)
	}
}

func TestCHAR(t *testing.T) {
	tt := decoder.TestTable{ // little endian
		{[]byte{'A', 'B', 'C', '0', '1', '2'}, `ABC012`},
		{[]byte{'a', 'b', '3', '4'}, `ab34`},
	}
	var out bytes.Buffer
	doCHARableTest(t, &out, New, decoder.LittleEndian, tt)
}
