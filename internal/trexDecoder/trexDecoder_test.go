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
	"github.com/rokath/trice/pkg/cipher"
	"github.com/stretchr/testify/assert"
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

// TestNewSelectsPackageFraming verifies the expected behavior.
func TestNewSelectsPackageFraming(t *testing.T) {
	tt := []struct {
		name    string
		framing string
		want    int
	}{
		{name: "cobs", framing: "cobs", want: packageFramingCOBS},
		{name: "tcobs alias", framing: "tcobs", want: packageFramingTCOBS},
		{name: "tcobsv1 alias", framing: "tcobsv1", want: packageFramingTCOBS},
		{name: "tcobsv2", framing: "tcobsv2", want: packageFramingTCOBSv2},
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

// TestIsZero verifies the expected behavior.
func TestIsZero(t *testing.T) {
	assert.True(t, isZero([]byte{0, 0, 0}))
	assert.False(t, isZero([]byte{0, 1, 0}))
}

// TestRemoveZeroHiByte verifies the expected behavior.
func TestRemoveZeroHiByte(t *testing.T) {
	d := &trexDec{DecoderData: decoder.NewDecoderData(decoder.Config{Endian: decoder.BigEndian})}
	assert.Equal(t, []byte{0x11, 0x22}, d.removeZeroHiByte([]byte{0x00, 0x11, 0x22}))

	d.Endian = decoder.LittleEndian
	assert.Equal(t, []byte{0x11, 0x22}, d.removeZeroHiByte([]byte{0x11, 0x00, 0x22}))
}

// TestNextDataAppendsToBuffer verifies the expected behavior.
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

// TestNextPackageCOBS verifies the expected behavior.
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

// TestNextPackageTCOBSPaths verifies the expected behavior.
func TestNextPackageTCOBSPaths(t *testing.T) {
	t.Run("success empty payload", func(t *testing.T) {
		p := &trexDec{
			DecoderData: decoder.NewDecoderData(decoder.Config{
				NeedBuffers: true,
			}),
			packageFraming: packageFramingTCOBS,
		}
		// TCOBS N sigil with offset 0 -> empty payload.
		p.IBuf = []byte{0xA0, 0x00}
		p.nextPackage()
		assert.Equal(t, 0, len(p.B))
		assert.Equal(t, 0, len(p.IBuf))
	})

	t.Run("error path", func(t *testing.T) {
		p := &trexDec{
			DecoderData: decoder.NewDecoderData(decoder.Config{
				NeedBuffers: true,
			}),
			packageFraming: packageFramingTCOBS,
		}
		// R2 with offset 0 but without preceding data -> decode error.
		p.IBuf = []byte{0x08, 0x00}
		p.nextPackage()
		assert.Equal(t, 0, len(p.B))
		assert.Equal(t, 0, len(p.IBuf))
	})

	t.Run("read from input when ibuf incomplete", func(t *testing.T) {
		p := &trexDec{
			DecoderData: decoder.NewDecoderData(decoder.Config{
				NeedBuffers: true,
				In:          bytes.NewBuffer([]byte{0xA0, 0x00}),
			}),
			packageFraming: packageFramingTCOBS,
		}
		p.nextPackage()
		assert.Equal(t, 0, len(p.B))
		assert.Equal(t, 0, len(p.IBuf))
	})
}

// TestNextPackageWithPasswordShowsDecryptDebugStage verifies the expected behavior.
func TestNextPackageWithPasswordShowsDecryptDebugStage(t *testing.T) {
	oldPassword := cipher.Password
	oldDebug := decoder.DebugOut
	t.Cleanup(func() {
		cipher.Password = oldPassword
		decoder.DebugOut = oldDebug
	})

	cipher.Password = "pw"
	decoder.DebugOut = true

	var out bytes.Buffer
	p := &trexDec{
		DecoderData: decoder.NewDecoderData(decoder.Config{
			Out:         &out,
			NeedBuffers: true,
		}),
		packageFraming: packageFramingCOBS,
	}
	// Empty COBS payload.
	p.IBuf = []byte{0x01, 0x00}
	p.nextPackage()

	assert.Contains(t, out.String(), "->TRICE:")
	assert.Contains(t, out.String(), "-> DEC:")
}

// TestNextPackageDebugOutWithoutPassword verifies the expected behavior.
func TestNextPackageDebugOutWithoutPassword(t *testing.T) {
	oldPassword := cipher.Password
	oldDebug := decoder.DebugOut
	oldFraming := decoder.PackageFraming
	t.Cleanup(func() {
		cipher.Password = oldPassword
		decoder.DebugOut = oldDebug
		decoder.PackageFraming = oldFraming
	})

	cipher.Password = ""
	decoder.DebugOut = true
	decoder.PackageFraming = "cobs"

	var out bytes.Buffer
	p := &trexDec{
		DecoderData: decoder.NewDecoderData(decoder.Config{
			Out:         &out,
			NeedBuffers: true,
		}),
		packageFraming: packageFramingCOBS,
	}
	p.IBuf = []byte{0x01, 0x00}
	p.nextPackage()

	assert.Contains(t, out.String(), "cobs:")
	assert.Contains(t, out.String(), "->TRICE:")
	assert.NotContains(t, out.String(), "-> DEC:")
}

// TestTriceStringAndBufferConverters verifies the expected behavior.
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

	p.Trice.Strg = "%d"
	p.B = []byte{7}
	p.ParamSpace = 1
	n = p.trice8B(b, 0, 0)
	assert.Equal(t, "7", string(b[:n]))
}

// TestTriceConvertersDebugOut verifies the expected behavior.
func TestTriceConvertersDebugOut(t *testing.T) {
	oldDebug := decoder.DebugOut
	t.Cleanup(func() { decoder.DebugOut = oldDebug })
	decoder.DebugOut = true

	var dbg bytes.Buffer
	p := &trexDec{
		DecoderData: decoder.NewDecoderData(decoder.Config{
			Out: &dbg,
		}),
	}
	b := make([]byte, 256)

	p.Trice.Strg = "C:%d\\n"
	p.B = []byte{1, 2}
	p.ParamSpace = 2
	n := p.trice8B(b, 0, 0)
	assert.Equal(t, "C:12\n", string(b[:n]))
	assert.NotEqual(t, "", dbg.String())

	p.Trice.Strg = "F"
	p.B = []byte{0x01}
	p.ParamSpace = 1
	n = p.trice8F(b, 0, 0)
	assert.Equal(t, "F(01)\n", string(b[:n]))
	assert.NotEqual(t, "", dbg.String())
}

// TestTriceFunctionStyleConvertersAndTrice0 verifies the expected behavior.
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

// TestUnsignedOrSignedOut verifies the expected behavior.
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

	p.Trice.Type = "TRICE16_2"
	p.Trice.Strg = "%d %t"
	p.pFmt = "%d %t"
	p.u = []int{decoder.SignedFormatSpecifier, decoder.BooleanFormatSpecifier}
	p.B = []byte{0xfe, 0xff, 0x01, 0x00} // -2, true
	n = p.unSignedOrSignedOut(b, 16, 2)
	assert.Equal(t, "-2 true", string(b[:n]))

	p.Trice.Type = "TRICE64_2"
	p.Trice.Strg = "%f %d"
	p.pFmt = "%.1f %d"
	p.u = []int{decoder.FloatFormatSpecifier, decoder.SignedFormatSpecifier}
	p.B = make([]byte, 16)
	binary.LittleEndian.PutUint64(p.B[0:], math.Float64bits(2.5))
	binary.LittleEndian.PutUint64(p.B[8:], uint64(^uint64(1))) // -2 as int64
	n = p.unSignedOrSignedOut(b, 64, 2)
	assert.Equal(t, "2.5 -2", string(b[:n]))
}

// TestPrintTestTableLine verifies the expected behavior.
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

// TestSpecialCaseHelpers verifies the expected behavior.
func TestSpecialCaseHelpers(t *testing.T) {
	assert.True(t, isSpecialCaseTriceType("trice8_b"))
	assert.False(t, isSpecialCaseTriceType("trice16_2"))

	prefix, itemFmt, addNL := splitChannelFormat("CH:%d\\n")
	assert.Equal(t, "CH:", prefix)
	assert.Equal(t, "%d", itemFmt)
	assert.True(t, addNL)

	prefix, itemFmt, addNL = splitChannelFormat("%x")
	assert.Equal(t, "", prefix)
	assert.Equal(t, "%x", itemFmt)
	assert.False(t, addNL)
}

// TestApplyMultilineIndent verifies the expected behavior.
func TestApplyMultilineIndent(t *testing.T) {
	oldIndent := decoder.NewlineIndent
	oldShowID := decoder.ShowID
	oldLIFnJSON := id.LIFnJSON
	t.Cleanup(func() {
		decoder.NewlineIndent = oldIndent
		decoder.ShowID = oldShowID
		id.LIFnJSON = oldLIFnJSON
	})

	decoder.NewlineIndent = -1
	decoder.ShowID = ""
	id.LIFnJSON = "off"

	got := applyMultilineIndent("a\\nb\\nc")
	assert.Equal(t, "a\\n             b\\n             c", got)
	assert.Equal(t, 13, decoder.NewlineIndent)

	// Two segments only: unchanged.
	assert.Equal(t, "a\\nb", applyMultilineIndent("a\\nb"))
}

// TestApplyMultilineIndentIncludesLocationAndIDColumns verifies the expected behavior.
func TestApplyMultilineIndentIncludesLocationAndIDColumns(t *testing.T) {
	oldIndent := decoder.NewlineIndent
	oldShowID := decoder.ShowID
	oldLIFnJSON := id.LIFnJSON
	t.Cleanup(func() {
		decoder.NewlineIndent = oldIndent
		decoder.ShowID = oldShowID
		id.LIFnJSON = oldLIFnJSON
	})

	decoder.NewlineIndent = -1
	decoder.ShowID = "%5d"
	id.LIFnJSON = "demoLI.json"

	got := applyMultilineIndent("a\\nb\\nc")
	assert.Equal(t, "a\\n                                              b\\n                                              c", got)
	assert.Equal(t, 46, decoder.NewlineIndent)
}

// TestNextPackageConsumesInputUntilDelimiter verifies the expected behavior.
func TestNextPackageConsumesInputUntilDelimiter(t *testing.T) {
	p := &trexDec{
		DecoderData: decoder.NewDecoderData(decoder.Config{
			NeedBuffers: true,
			In:          bytes.NewBuffer([]byte{0x01, 0x00, 0xff}),
		}),
		packageFraming: packageFramingCOBS,
	}

	p.nextPackage()
	assert.Empty(t, p.B)
	assert.Equal(t, []byte{0xff}, p.IBuf)
}

// TestSprintTriceErrorPaths verifies the expected behavior.
func TestSprintTriceErrorPaths(t *testing.T) {
	p := &trexDec{DecoderData: decoder.NewDecoderData(decoder.Config{Endian: decoder.LittleEndian})}
	b := make([]byte, 256)

	p.Trice = id.TriceFmt{Type: "BADTYPE", Strg: "%d"}
	p.B = []byte{1}
	p.ParamSpace = 1
	n := p.sprintTrice(b)
	assert.Contains(t, string(b[:n]), "ConstructFullTriceInfo failed")

	p.Trice = id.TriceFmt{Type: "TRICE8_2", Strg: "%d %d"}
	p.B = []byte{1}
	p.ParamSpace = 2
	n = p.sprintTrice(b)
	assert.Contains(t, string(b[:n]), "len(p.B) = 1 < p.ParamSpace =  2")

	p.Trice = id.TriceFmt{Type: "TRICE16_1", Strg: "%d"}
	p.B = []byte{1}
	p.ParamSpace = 1
	n = p.sprintTrice(b)
	assert.Contains(t, string(b[:n]), "not matching with bitWidth")
}

// TestSprintTriceSAliasPath verifies the expected behavior.
func TestSprintTriceSAliasPath(t *testing.T) {
	p := &trexDec{DecoderData: decoder.NewDecoderData(decoder.Config{Endian: decoder.LittleEndian})}
	b := make([]byte, 256)

	p.Trice = id.TriceFmt{
		Type: "TRICE_S",
		Strg: id.SAliasStrgPrefix + `false, "ignored"` + id.SAliasStrgSuffix,
	}
	p.B = []byte("abc")
	p.ParamSpace = 3

	n := p.sprintTrice(b)
	assert.Equal(t, "abc", string(b[:n]))
}

// TestSprintTriceAssertTypeNormalization verifies the expected behavior.
func TestSprintTriceAssertTypeNormalization(t *testing.T) {
	p := &trexDec{DecoderData: decoder.NewDecoderData(decoder.Config{Endian: decoder.LittleEndian})}
	b := make([]byte, 256)

	p.Trice = id.TriceFmt{
		Type: "TriceAssertTrue",
		Strg: "%d",
	}
	p.B = []byte{7, 0, 0, 0}
	p.ParamSpace = 4

	n := p.sprintTrice(b)
	assert.Equal(t, "7", string(b[:n]))
	assert.Equal(t, "Trice", p.Trice.Type)
}

// TestReadNoneFramingKnownTrice verifies the expected behavior.
func TestReadNoneFramingKnownTrice(t *testing.T) {
	oldFraming := decoder.PackageFraming
	oldInitial := decoder.InitialCycle
	oldDisable := DisableCycleErrors
	t.Cleanup(func() {
		decoder.PackageFraming = oldFraming
		decoder.InitialCycle = oldInitial
		DisableCycleErrors = oldDisable
	})

	decoder.PackageFraming = "none"
	decoder.InitialCycle = true
	DisableCycleErrors = false

	// typeS0/id=1 (0x4001), nc=0x01c0 (count=1, cycle=0xc0), payload=42
	in := bytes.NewBuffer([]byte{0x01, 0x40, 0xc0, 0x01, 0x2a, 0x00, 0x00, 0x00})
	lut := id.TriceIDLookUp{
		1: {Type: "TRICE8_1", Strg: "v=%d"},
	}
	dec := New(io.Discard, lut, new(sync.RWMutex), nil, in, decoder.LittleEndian)

	buf := make([]byte, 128)
	n, err := dec.Read(buf)
	assert.NoError(t, err)
	assert.Equal(t, "v=42", string(buf[:n]))
}

// TestReadCOBSFramingUnknownID verifies the expected behavior.
func TestReadCOBSFramingUnknownID(t *testing.T) {
	oldFraming := decoder.PackageFraming
	t.Cleanup(func() { decoder.PackageFraming = oldFraming })

	decoder.PackageFraming = "cobs"
	// COBS frame without zero bytes: [06 payload(5 bytes)] 00
	in := bytes.NewBuffer([]byte{0x06, 0x01, 0x40, 0xc0, 0x01, 0x2a, 0x00})
	dec := New(io.Discard, id.TriceIDLookUp{}, new(sync.RWMutex), nil, in, decoder.LittleEndian)

	buf := make([]byte, 2048)
	n, err := dec.Read(buf)
	assert.NoError(t, err)
	assert.Contains(t, string(buf[:n]), "unknown ID")
}

// TestReadNoneFramingTypeX0Resync verifies the expected behavior.
func TestReadNoneFramingTypeX0Resync(t *testing.T) {
	oldFraming := decoder.PackageFraming
	oldVerbose := decoder.Verbose
	t.Cleanup(func() {
		decoder.PackageFraming = oldFraming
		decoder.Verbose = oldVerbose
	})

	decoder.PackageFraming = "none"
	decoder.Verbose = false

	in := bytes.NewBuffer([]byte{0x01, 0x00, 0xaa})
	decI := New(io.Discard, nil, new(sync.RWMutex), nil, in, decoder.LittleEndian)
	dec, ok := decI.(*trexDec)
	if !ok {
		t.Fatalf("unexpected decoder type %T", decI)
	}

	buf := make([]byte, 64)
	n, err := dec.Read(buf)
	assert.NoError(t, err)
	assert.Equal(t, 0, n)
	assert.Equal(t, []byte{0x01, 0xaa}, dec.B)
}

// TestReadTypeX0NotImplementedForFramedData verifies the expected behavior.
func TestReadTypeX0NotImplementedForFramedData(t *testing.T) {
	oldTargetTS := decoder.TargetTimestampSize
	t.Cleanup(func() { decoder.TargetTimestampSize = oldTargetTS })
	decoder.TargetTimestampSize = 0

	p := &trexDec{
		DecoderData:    decoder.NewDecoderData(decoder.Config{Endian: decoder.LittleEndian}),
		packageFraming: packageFramingCOBS,
	}
	// typeX0 + enough bytes for header/count check.
	p.B = []byte{0x01, 0x00, 0x00, 0x00}
	buf := make([]byte, 128)
	n, err := p.Read(buf)
	assert.NoError(t, err)
	assert.Contains(t, string(buf[:n]), "typeX0 not implemented")
}

// TestReadSingleFramingAndCycleError verifies the expected behavior.
func TestReadSingleFramingAndCycleError(t *testing.T) {
	oldFraming := decoder.PackageFraming
	oldInitial := decoder.InitialCycle
	oldVerbose := decoder.Verbose
	oldSingle := SingleFraming
	oldDisable := DisableCycleErrors
	t.Cleanup(func() {
		decoder.PackageFraming = oldFraming
		decoder.InitialCycle = oldInitial
		decoder.Verbose = oldVerbose
		SingleFraming = oldSingle
		DisableCycleErrors = oldDisable
	})

	decoder.PackageFraming = "none"
	decoder.InitialCycle = false
	decoder.Verbose = true
	SingleFraming = true
	DisableCycleErrors = false

	in := bytes.NewBuffer([]byte{0x01, 0x40, 0x10, 0x01, 0x2a, 0x00, 0x00, 0x00})
	lut := id.TriceIDLookUp{
		1: {Type: "TRICE8_1", Strg: "v=%d"},
	}
	decI := New(io.Discard, lut, new(sync.RWMutex), nil, in, decoder.LittleEndian)
	dec, ok := decI.(*trexDec)
	if !ok {
		t.Fatalf("unexpected decoder type %T", decI)
	}
	dec.cycle = 0x11 // force mismatch against cycle 0x10

	buf := make([]byte, 256)
	n, err := dec.Read(buf)
	assert.NoError(t, err)
	out := string(buf[:n])
	assert.Contains(t, out, "single framed package size")
}

// TestReadCycleErrorMessage verifies the expected behavior.
func TestReadCycleErrorMessage(t *testing.T) {
	oldFraming := decoder.PackageFraming
	oldInitial := decoder.InitialCycle
	oldVerbose := decoder.Verbose
	oldSingle := SingleFraming
	oldDisable := DisableCycleErrors
	t.Cleanup(func() {
		decoder.PackageFraming = oldFraming
		decoder.InitialCycle = oldInitial
		decoder.Verbose = oldVerbose
		SingleFraming = oldSingle
		DisableCycleErrors = oldDisable
	})

	decoder.PackageFraming = "none"
	decoder.InitialCycle = false
	decoder.Verbose = false
	SingleFraming = false
	DisableCycleErrors = false

	// exact-size package: tyId(2) + nc(2) + payload(1)
	in := bytes.NewBuffer([]byte{0x01, 0x40, 0x10, 0x01, 0x2a})
	lut := id.TriceIDLookUp{
		1: {Type: "TRICE8_1", Strg: "v=%d"},
	}
	decI := New(io.Discard, lut, new(sync.RWMutex), nil, in, decoder.LittleEndian)
	dec, ok := decI.(*trexDec)
	if !ok {
		t.Fatalf("unexpected decoder type %T", decI)
	}
	dec.cycle = 0x11 // force mismatch

	buf := make([]byte, 1024)
	n, err := dec.Read(buf)
	assert.NoError(t, err)
	assert.Contains(t, string(buf[:n]), "CYCLE_ERROR")
}

// TestReadFramedDiscardTrailingEncryptedZeroes verifies the expected behavior.
func TestReadFramedDiscardTrailingEncryptedZeroes(t *testing.T) {
	oldPassword := cipher.Password
	t.Cleanup(func() { cipher.Password = oldPassword })
	cipher.Password = "x"

	p := &trexDec{
		DecoderData:    decoder.NewDecoderData(decoder.Config{In: bytes.NewBuffer(nil), NeedBuffers: true}),
		packageFraming: packageFramingCOBS,
	}
	p.B = []byte{0, 0, 0, 0}

	buf := make([]byte, 64)
	n, err := p.Read(buf)
	assert.NoError(t, err)
	assert.Equal(t, 0, n)
	assert.Equal(t, 0, len(p.B))
}

// TestReadFramedDiscardSingleLeftoverByte verifies the expected behavior.
func TestReadFramedDiscardSingleLeftoverByte(t *testing.T) {
	oldVerbose := decoder.Verbose
	t.Cleanup(func() { decoder.Verbose = oldVerbose })
	decoder.Verbose = true

	p := &trexDec{
		DecoderData:    decoder.NewDecoderData(decoder.Config{In: bytes.NewBuffer(nil), NeedBuffers: true}),
		packageFraming: packageFramingCOBS,
	}
	p.B = []byte{0xaa}

	buf := make([]byte, 64)
	n, err := p.Read(buf)
	assert.NoError(t, err)
	assert.Equal(t, 0, n)
	assert.Equal(t, 0, len(p.B))
}

// TestReadTypeS2DoubledIDIncomplete verifies the expected behavior.
func TestReadTypeS2DoubledIDIncomplete(t *testing.T) {
	oldDouble := Doubled16BitID
	t.Cleanup(func() { Doubled16BitID = oldDouble })
	Doubled16BitID = true

	p := &trexDec{
		DecoderData:    decoder.NewDecoderData(decoder.Config{Endian: decoder.LittleEndian}),
		packageFraming: packageFramingCOBS,
	}
	// typeS2 + id1 only.
	p.B = []byte{0x01, 0x80}

	buf := make([]byte, 64)
	n, err := p.Read(buf)
	assert.NoError(t, err)
	assert.Equal(t, 0, n)
	assert.Equal(t, 0, len(p.B))
}

// TestReadTypeS2DoubledIDConsumesSecondID verifies the expected behavior.
func TestReadTypeS2DoubledIDConsumesSecondID(t *testing.T) {
	oldDouble := Doubled16BitID
	t.Cleanup(func() { Doubled16BitID = oldDouble })
	Doubled16BitID = true

	p := &trexDec{
		DecoderData:    decoder.NewDecoderData(decoder.Config{Endian: decoder.LittleEndian}),
		packageFraming: packageFramingCOBS,
	}
	// typeS2 + second id copy + minimal bytes.
	p.B = []byte{0x01, 0x80, 0x01, 0x80, 0x00, 0x00}

	buf := make([]byte, 64)
	n, err := p.Read(buf)
	assert.NoError(t, err)
	assert.Equal(t, 0, n)
	assert.True(t, len(p.B) <= 2)
}

// TestReadNoneFramingUnknownIDResync verifies the expected behavior.
func TestReadNoneFramingUnknownIDResync(t *testing.T) {
	oldFraming := decoder.PackageFraming
	oldVerbose := decoder.Verbose
	t.Cleanup(func() {
		decoder.PackageFraming = oldFraming
		decoder.Verbose = oldVerbose
	})
	decoder.PackageFraming = "none"
	decoder.Verbose = false

	in := bytes.NewBuffer([]byte{0x01, 0x40, 0xc0, 0x01, 0x2a, 0x00, 0x00, 0x00})
	decI := New(io.Discard, id.TriceIDLookUp{}, new(sync.RWMutex), nil, in, decoder.LittleEndian)
	dec, ok := decI.(*trexDec)
	if !ok {
		t.Fatalf("unexpected decoder type %T", decI)
	}

	buf := make([]byte, 64)
	n, err := dec.Read(buf)
	assert.NoError(t, err)
	assert.Equal(t, 0, n)
	assert.True(t, len(dec.B) < 8)
}

// TestReadNoneFramingTooShortForHeader verifies the expected behavior.
func TestReadNoneFramingTooShortForHeader(t *testing.T) {
	oldFraming := decoder.PackageFraming
	t.Cleanup(func() { decoder.PackageFraming = oldFraming })
	decoder.PackageFraming = "none"

	in := bytes.NewBuffer([]byte{0x01}) // shorter than tyIdSize
	decI := New(io.Discard, nil, new(sync.RWMutex), nil, in, decoder.LittleEndian)
	dec, ok := decI.(*trexDec)
	if !ok {
		t.Fatalf("unexpected decoder type %T", decI)
	}

	buf := make([]byte, 32)
	n, err := dec.Read(buf)
	assert.NoError(t, err)
	assert.Equal(t, 0, n)
	assert.Equal(t, 1, len(dec.B))
}

// TestReadNoneFramingResyncWhenTriceSizeExceedsPackage verifies the expected behavior.
func TestReadNoneFramingResyncWhenTriceSizeExceedsPackage(t *testing.T) {
	oldFraming := decoder.PackageFraming
	oldVerbose := decoder.Verbose
	oldInitial := decoder.InitialCycle
	t.Cleanup(func() {
		decoder.PackageFraming = oldFraming
		decoder.Verbose = oldVerbose
		decoder.InitialCycle = oldInitial
	})
	decoder.PackageFraming = "none"
	decoder.Verbose = false
	decoder.InitialCycle = false

	// typeS0 id=1 + nc=0x7fff (ParamSpace=127), no payload => TriceSize > packageSize
	in := bytes.NewBuffer([]byte{0x01, 0x40, 0xff, 0x7f})
	lut := id.TriceIDLookUp{1: {Type: "TRICE8_1", Strg: "v=%d"}}
	decI := New(io.Discard, lut, new(sync.RWMutex), nil, in, decoder.LittleEndian)
	dec, ok := decI.(*trexDec)
	if !ok {
		t.Fatalf("unexpected decoder type %T", decI)
	}

	buf := make([]byte, 128)
	n, err := dec.Read(buf)
	assert.NoError(t, err)
	assert.Equal(t, 0, n)
	// none-mode resync drops first byte from the preserved buffer.
	assert.Equal(t, 3, len(dec.B))
}
