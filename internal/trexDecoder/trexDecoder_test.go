// SPDX-License-Identifier: MIT

package trexDecoder

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"io"
	"math"
	"os"
	"strings"
	"sync"
	"testing"

	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
	"github.com/tj/assert"
)

// doTableTest is the universal decoder test sequence.
func doTableTest(t *testing.T, out io.Writer, f decoder.New, endianness bool, teTa decoder.TestTable) {
	var (
		// til is the trace id list content for test
		idl = `{
			"3713": {
				"Type": "TRICE16",
				"Strg": "MSG: 💚 START select = %d\\n"
			},
			"935": {
				"Type": "TRICE",
				"Strg": "MSG:triceFifoDepthMax = %d of max %d, triceStreamBufferDepthMax = %d of max %d\\n"
			}
		}
	`
	)
	ilu := make(id.TriceIDLookUp)  // empty
	li := make(id.TriceIDLookUpLI) // empty
	luM := new(sync.RWMutex)
	assert.Nil(t, ilu.FromJSON([]byte(idl)))
	ilu.AddFmtCount(os.Stdout)
	buf := make([]byte, decoder.DefaultSize)
	dec := f(out, ilu, luM, li, nil, endianness) // a new decoder instance
	for _, x := range teTa {
		in := io.NopCloser(bytes.NewBuffer(x.In))
		dec.SetInput(in)
		lineStart := true
		var err error
		var n int
		var act string
		for nil == err {
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

// TestTREX takes byte sequence in tt as TCOBSv1 framed trice and converts it to the appropriate sting in the tt.
// For translation is uses the idl inside doTableTest. The cycle counter starts with 0xC0 to avoid cycle error messages.
func TestTREX(t *testing.T) {
	tt := decoder.TestTable{ // little endian
		//tcobs:idLo  idHi  tsLo  tsHi  cycle count vLo   vHi   sigil delim
		{[]byte{0x81, 0x8e, 0x09, 0x23, 0xc0, 0x02, 0xb8, 0x01, 0xa4, 0x00}, `MSG: 💚 START select = 440`},
		{[]byte{0xa7, 0x83, 0x1b, 0x23, 0xc1, 0x10, 0x5c, 0x63, 0x80, 0x61, 0x50, 0x05, 0x62, 0x08, 0x41, 0x00}, `MSG:triceFifoDepthMax = 92 of max 128, triceStreamBufferDepthMax = 1360 of max 2048`},
	}
	decoder.PackageFraming = "TCOBSv1"
	var out bytes.Buffer
	doTableTest(t, &out, New, decoder.LittleEndian, tt)
	assert.Equal(t, "", out.String())
}

func TestNewSelectsPackageFraming(t *testing.T) {
	tt := []struct {
		name    string
		framing string
		want    int
	}{
		{name: "cobs", framing: "cobs", want: packageFramingCOBS},
		{name: "tcobs alias", framing: "tcobs", want: packageFramingTCOBS},
		{name: "tcobsv1 alias", framing: "tcobsv1", want: packageFramingTCOBS},
		{name: "none", framing: "none", want: packageFramingNone},
	}
	for _, tc := range tt {
		t.Run(tc.name, func(t *testing.T) {
			decoder.PackageFraming = tc.framing
			dec := New(io.Discard, nil, nil, nil, nil, decoder.LittleEndian)
			td, ok := dec.(*trexDec)
			if !ok {
				t.Fatalf("unexpected decoder type %T", dec)
			}
			assert.Equal(t, tc.want, td.packageFraming)
		})
	}
}

func TestIsZero(t *testing.T) {
	assert.True(t, isZero([]byte{0, 0, 0}))
	assert.False(t, isZero([]byte{0, 1, 0}))
}

func TestRemoveZeroHiByte(t *testing.T) {
	d := &trexDec{DecoderData: decoder.NewDecoderData(decoder.Config{Endian: decoder.BigEndian})}
	assert.Equal(t, []byte{0x11, 0x22}, d.removeZeroHiByte([]byte{0x00, 0x11, 0x22}))

	d.Endian = decoder.LittleEndian
	assert.Equal(t, []byte{0x11, 0x22}, d.removeZeroHiByte([]byte{0x11, 0x00, 0x22}))
}

func TestNextDataAppendsToBuffer(t *testing.T) {
	p := &trexDec{
		DecoderData: decoder.NewDecoderData(decoder.Config{
			In:          bytes.NewBuffer([]byte{1, 2, 3}),
			NeedBuffers: true,
		}),
	}
	p.B = []byte{9}
	p.nextData()
	assert.Equal(t, []byte{9, 1, 2, 3}, p.B)
}

func TestNextPackageCOBS(t *testing.T) {
	p := &trexDec{
		DecoderData: decoder.NewDecoderData(decoder.Config{
			NeedBuffers: true,
		}),
		packageFraming: packageFramingCOBS,
	}
	// COBS frame "01" decodes to an empty payload.
	p.IBuf = []byte{0x01, 0x00}

	p.nextPackage()
	assert.Equal(t, 0, len(p.B))
	assert.Equal(t, 0, len(p.IBuf))
}

func TestTriceStringAndBufferConverters(t *testing.T) {
	p := &trexDec{DecoderData: decoder.NewDecoderData(decoder.Config{Endian: decoder.LittleEndian})}
	b := make([]byte, 256)

	p.Trice.Strg = "msg:%s"
	p.B = []byte("abc")
	p.ParamSpace = 3
	n := p.triceN(b, 0, 0)
	assert.Equal(t, "msg:abc", string(b[:n]))

	p.Trice.Strg = "str:%s"
	n = p.triceS(b, 0, 0)
	assert.Equal(t, "str:abc", string(b[:n]))

	p.Trice.Strg = "CH:%d\n"
	p.B = []byte{1, 2}
	p.ParamSpace = 2
	n = p.trice8B(b, 0, 0)
	assert.Equal(t, "CH:1\n2\n", string(b[:n]))

	p.B = []byte{0x34, 0x12}
	p.ParamSpace = 2
	n = p.trice16B(b, 0, 0)
	assert.Equal(t, "CH:4660\n", string(b[:n]))

	p.B = []byte{0x78, 0x56, 0x34, 0x12}
	p.ParamSpace = 4
	n = p.trice32B(b, 0, 0)
	assert.Equal(t, "CH:305419896\n", string(b[:n]))

	p.B = []byte{1, 0, 0, 0, 0, 0, 0, 0}
	p.ParamSpace = 8
	n = p.trice64B(b, 0, 0)
	assert.Equal(t, "CH:1\n", string(b[:n]))
}

func TestTriceFunctionStyleConvertersAndTrice0(t *testing.T) {
	p := &trexDec{DecoderData: decoder.NewDecoderData(decoder.Config{})}
	b := make([]byte, 256)

	p.Trice.Strg = "F"

	p.B = []byte{0x01, 0x02}
	p.ParamSpace = 2
	n := p.trice8F(b, 0, 0)
	assert.Equal(t, "F(01)(02)\n", string(b[:n]))

	p.B = []byte{0x34, 0x12}
	p.ParamSpace = 2
	n = p.trice16F(b, 0, 0)
	assert.Equal(t, "F(1234)\n", string(b[:n]))

	p.B = []byte{0x78, 0x56, 0x34, 0x12}
	p.ParamSpace = 4
	n = p.trice32F(b, 0, 0)
	assert.Equal(t, "F(12345678)\n", string(b[:n]))

	p.B = []byte{1, 0, 0, 0, 0, 0, 0, 0}
	p.ParamSpace = 8
	n = p.trice64F(b, 0, 0)
	assert.Equal(t, "F(0000000000000001)\n", string(b[:n]))

	p.pFmt = "plain"
	n = p.trice0(b, 0, 0)
	assert.Equal(t, "plain", string(b[:n]))
}

func TestUnsignedOrSignedOut(t *testing.T) {
	p := &trexDec{DecoderData: decoder.NewDecoderData(decoder.Config{Endian: decoder.LittleEndian})}
	b := make([]byte, 256)

	p.Trice.Type = "TRICE8_4"
	p.Trice.Strg = "%u %d %t %x"
	p.pFmt = "%d %d %t %x"
	p.u = []int{
		decoder.UnsignedFormatSpecifier,
		decoder.SignedFormatSpecifier,
		decoder.BooleanFormatSpecifier,
		decoder.PointerFormatSpecifier,
	}
	p.B = []byte{1, 0xff, 1, 0xaa}
	n := p.unSignedOrSignedOut(b, 8, 4)
	assert.Equal(t, "1 -1 true aa", string(b[:n]))

	p.Trice.Type = "TRICE32_2"
	p.Trice.Strg = "%f %d"
	p.pFmt = "%0.1f %d"
	p.u = []int{decoder.FloatFormatSpecifier, decoder.SignedFormatSpecifier}
	p.B = make([]byte, 8)
	binary.LittleEndian.PutUint32(p.B[0:], math.Float32bits(1.5))
	binary.LittleEndian.PutUint32(p.B[4:], 0xfffffffe)
	n = p.unSignedOrSignedOut(b, 32, 2)
	assert.Equal(t, "1.5 -2", string(b[:n]))

	p.u = []int{decoder.StringFormatSpecifier}
	p.B = []byte{1}
	n = p.unSignedOrSignedOut(b, 8, 1)
	assert.Contains(t, string(b[:n]), "Invalid format specifier")

	p.u = []int{decoder.UnsignedFormatSpecifier}
	p.B = []byte{1}
	n = p.unSignedOrSignedOut(b, 8, 2)
	assert.Contains(t, string(b[:n]), "Invalid format specifier count")
}

func TestPrintTestTableLine(t *testing.T) {
	oldStdout := os.Stdout
	oldNextLine := emitter.NextLine
	oldVirgin := testTableVirgin
	r, w, err := os.Pipe()
	assert.NoError(t, err)
	os.Stdout = w
	t.Cleanup(func() {
		_ = w.Close()
		os.Stdout = oldStdout
		emitter.NextLine = oldNextLine
		testTableVirgin = oldVirgin
	})

	p := &trexDec{}
	p.IBuf = []byte{1, 2, 3}
	emitter.NextLine = true
	testTableVirgin = true
	p.printTestTableLine(2)

	_ = w.Close()
	out, _ := io.ReadAll(r)
	assert.Contains(t, string(out), "{ []byte{")
	assert.Contains(t, string(out), "  1,")
	assert.Contains(t, string(out), "  2,")
}
