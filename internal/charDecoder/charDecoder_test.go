package charDecoder

import (
	"bytes"
	"fmt"
	"io"
	"io/ioutil"
	"strings"
	"sync"
	"testing"

	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/id"
	"github.com/tj/assert"
)

// new abstracts the function type for a new decoder.
type new func(out io.Writer, lut id.TriceIDLookUp, m *sync.RWMutex, li id.TriceIDLookUpLI, in io.Reader, endian bool) decoder.Decoder

// doTableTest is the universal decoder test sequence.
func doCHARableTest(t *testing.T, out io.Writer, f new, endianness bool, teTa decoder.TestTable) {
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
