// SPDX-License-Identifier: MIT

package decoder

import (
	"bytes"
	"fmt"
	"io"
	"os"
	"strings"
	"testing"
	"time"

	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
	"github.com/stretchr/testify/assert"
)

// Test_UReplaceN verifies that unsigned specifiers are replaced while keeping surrounding tokens intact.
func Test_UReplaceN(t *testing.T) {
	tt := []struct {
		fmt string // fmt is the Trice format string written by the user in the target source code.
		exp string // exp is the modified fmt.
		u   []int  // u is the format specifiers modification information.
	}{
		{"a %s b", "a %s b", []int{5}},
		{"%x", "%x", []int{1}},
		{"% x", "% x", []int{1}},
		{"%u", "%d", []int{0}},
		{"%d", "%d", []int{1}},
		{"%i", "%d", []int{1}},
		{"%d %u %f", "%d %d %f", []int{1, 0, 2}},
	}
	for i, x := range tt {
		fmt.Println(i, x)
		act, uct := UReplaceN(x.fmt)
		assert.Equal(t, x.exp, act)
		assert.Equal(t, x.u, uct)
	}
}

// Test_UReplaceNLengthModifiers verifies normalization of valid C length modifiers
// into Go-compatible format strings for non-%u cases. Unsigned decimal handling is
// covered separately because the %u -> %d rewrite and type conversion are a different concern.
func Test_UReplaceNLengthModifiers(t *testing.T) {
	tt := []struct {
		name string
		fmt  string
		exp  string
		u    []int
	}{
		{name: "long signed", fmt: "count=%ld", exp: "count=%d", u: []int{SignedFormatSpecifier}},
		{name: "long signed i", fmt: "count=%li", exp: "count=%d", u: []int{SignedFormatSpecifier}},
		{name: "size_t signed", fmt: "count=%zd", exp: "count=%d", u: []int{SignedFormatSpecifier}},
		{name: "long long hex upper", fmt: "mask=%#016llX", exp: "mask=%#016X", u: []int{SignedFormatSpecifier}},
		{name: "size_t hex lower", fmt: "mask=%zx", exp: "mask=%x", u: []int{SignedFormatSpecifier}},
		{name: "long double float", fmt: "value=%04Lf", exp: "value=%04f", u: []int{FloatFormatSpecifier}},
	}
	for _, tc := range tt {
		t.Run(tc.name, func(t *testing.T) {
			act, uct := UReplaceN(tc.fmt)
			assert.Equal(t, tc.exp, act)
			assert.Equal(t, tc.u, uct)
		})
	}
}

// TestUReplaceNDoesNotUnsignedifySignedDecimal protects the historic behavior
// that the global Unsigned switch only affects base-changing verbs such as %x.
// Decimal signed verbs like %d must stay signed, otherwise PC target tests emit
// 4294967295 where "-1" is expected.
func TestUReplaceNDoesNotUnsignedifySignedDecimal(t *testing.T) {
	oldUnsigned := Unsigned
	t.Cleanup(func() { Unsigned = oldUnsigned })

	Unsigned = true
	act, uct := UReplaceN("value=%d %x")
	assert.Equal(t, "value=%d %x", act)
	assert.Equal(t, []int{SignedFormatSpecifier, UnsignedFormatSpecifier}, uct)
}

// TestNewDecoderDataWithoutBuffers ensures the base decoder initializes helper buffers only when requested.
func TestNewDecoderDataWithoutBuffers(t *testing.T) {
	d := NewDecoderData(Config{})
	assert.NotNil(t, d.W)
	assert.NotNil(t, d.LutMutex)
	assert.Equal(t, DefaultSize, cap(d.IBuf))
	assert.Nil(t, d.B)
	assert.Nil(t, d.B0)
	assert.Nil(t, d.InnerBuffer)
}

// TestSetInput confirms SetInput swaps the underlying reader instance.
func TestSetInput(t *testing.T) {
	d := NewDecoderData(Config{})
	r := strings.NewReader("abc")
	d.SetInput(r)
	assert.Same(t, r, d.In)
}

// TestReadUXEndianess exercises U16/U32/U64 reading in both endiannesses.
func TestReadUXEndianess(t *testing.T) {
	dLittle := NewDecoderData(Config{Endian: LittleEndian})
	dBig := NewDecoderData(Config{Endian: BigEndian})

	assert.EqualValues(t, 0x3412, dLittle.ReadU16([]byte{0x12, 0x34}))
	assert.EqualValues(t, 0x1234, dBig.ReadU16([]byte{0x12, 0x34}))

	assert.EqualValues(t, 0x78563412, dLittle.ReadU32([]byte{0x12, 0x34, 0x56, 0x78}))
	assert.EqualValues(t, 0x12345678, dBig.ReadU32([]byte{0x12, 0x34, 0x56, 0x78}))

	assert.EqualValues(t, 0x0807060504030201, dLittle.ReadU64([]byte{1, 2, 3, 4, 5, 6, 7, 8}))
	assert.EqualValues(t, 0x0102030405060708, dBig.ReadU64([]byte{1, 2, 3, 4, 5, 6, 7, 8}))
}

// TestSetInputAffectsRead proves that changing the reader affects subsequent reads.
func TestSetInputAffectsRead(t *testing.T) {
	d := NewDecoderData(Config{In: strings.NewReader("old")})
	fd := fakeDecoder{data: &d}

	fd.SetInput(bytes.NewBufferString("new"))
	b := make([]byte, 8)
	n, err := fd.Read(b)
	assert.NoError(t, err)
	assert.Equal(t, "new", string(b[:n]))
}

// TestDump asserts the hex formatting outputs space-separated hex bytes.
func TestDump(t *testing.T) {
	var out bytes.Buffer
	Dump(&out, []byte{0x00, 0x1f, 0xa0})
	assert.Equal(t, "00 1f a0 \n", out.String())
}

// TestRecordForStatisticsDisabled checks that recorded statistics stay empty when globally disabled.
func TestRecordForStatisticsDisabled(t *testing.T) {
	oldTriceStatistics := TriceStatistics
	oldAllStatistics := emitter.AllStatistics
	oldIDStat := IDStat
	t.Cleanup(func() {
		TriceStatistics = oldTriceStatistics
		emitter.AllStatistics = oldAllStatistics
		IDStat = oldIDStat
	})

	TriceStatistics = false
	emitter.AllStatistics = false
	IDStat = map[id.TriceID]int{}
	RecordForStatistics(7)
	assert.Equal(t, 0, IDStat[7])
}

// TestRecordAndPrintTriceStatistics ensures counter increments and the printed report contains expected data.
func TestRecordAndPrintTriceStatistics(t *testing.T) {
	oldTriceStatistics := TriceStatistics
	oldAllStatistics := emitter.AllStatistics
	oldIDStat := IDStat
	oldIDLUT := IDLUT
	oldLILUT := LILUT
	t.Cleanup(func() {
		TriceStatistics = oldTriceStatistics
		emitter.AllStatistics = oldAllStatistics
		IDStat = oldIDStat
		IDLUT = oldIDLUT
		LILUT = oldLILUT
	})

	TriceStatistics = true
	emitter.AllStatistics = false
	IDStat = map[id.TriceID]int{}
	IDLUT = id.TriceIDLookUp{
		7: {Type: "TRICE", Strg: "hello\\n"},
	}
	LILUT = id.TriceIDLookUpLI{
		7: {File: "a/b/c.c", Line: 23},
	}

	RecordForStatistics(7)
	RecordForStatistics(7)
	assert.Equal(t, 2, IDStat[7])

	var out bytes.Buffer
	PrintTriceStatistics(&out)
	s := out.String()
	assert.Contains(t, s, "Trice Statistics")
	assert.Contains(t, s, "c.c")
	assert.Contains(t, s, "hello")
	assert.Contains(t, s, "2 Trice messsges")
}

// TestHandleTypeX0 verifies the initial selector-0 handling modes and shorthand rules.
func TestHandleTypeX0(t *testing.T) {
	oldTypeX0 := TypeX0
	t.Cleanup(func() { TypeX0 = oldTypeX0 })

	tests := []struct {
		name          string
		option        string
		record        []byte
		endian        bool
		noneFraming   bool
		wantText      string
		wantContains  string
		wantConsumed  int
		wantBlankMeta bool
	}{
		{name: "default error", option: "error", record: []byte{0x02, 0x00, 'O', 'K'}, endian: LittleEndian, wantContains: "typeX0 packet ignored", wantConsumed: 4, wantBlankMeta: true},
		{name: "counted string shorthand", option: "%s", record: []byte{0x02, 0x00, 'O', 'K'}, endian: LittleEndian, wantText: "OK", wantConsumed: 4, wantBlankMeta: true},
		{name: "counted explicit with colon format", option: "counted:sig:%s", record: []byte{0x02, 0x00, 'O', 'K'}, endian: LittleEndian, wantText: "sig:OK", wantConsumed: 4, wantBlankMeta: true},
		{name: "colon shorthand rejected", option: "sig:%s", record: []byte{0x02, 0x00, 'O', 'K'}, endian: LittleEndian, wantContains: `unsupported typeX0 mode "sig"`, wantConsumed: 4, wantBlankMeta: true},
		{name: "counted ignore", option: "ignore", record: []byte{0x02, 0x00, 'O', 'K'}, endian: LittleEndian, wantConsumed: 4},
		{name: "all ignore consumes package", option: "all:ignore", record: []byte{0x02, 0x00, 'O', 'K', 0x01, 0x40}, endian: LittleEndian, wantConsumed: 6},
		{name: "big endian counted", option: "%s", record: []byte{0x00, 0x02, 'O', 'K'}, endian: BigEndian, wantText: "OK", wantConsumed: 4, wantBlankMeta: true},
		{name: "none framing skips alignment", option: "%s", record: []byte{0x01, 0x00, 'A', 0x00}, endian: LittleEndian, noneFraming: true, wantText: "A", wantConsumed: 4, wantBlankMeta: true},
		{name: "none framing allows compact empty payload", option: "%s", record: []byte{0x00, 0x00}, endian: LittleEndian, noneFraming: true, wantText: "", wantConsumed: 2, wantBlankMeta: true},
		{name: "none framing keeps compact next record", option: "%s", record: []byte{0x01, 0x00, 'A', 0x01, 0x40}, endian: LittleEndian, noneFraming: true, wantText: "A", wantConsumed: 3, wantBlankMeta: true},
		{name: "framed rejects non-zero alignment padding", option: "%s", record: []byte{0x01, 0x00, 'A', 'B'}, endian: LittleEndian, wantContains: "non-zero alignment padding", wantConsumed: 4, wantBlankMeta: true},
		{name: "malformed ignore still errors", option: "ignore", record: []byte{0x04, 0x00, 'A'}, endian: LittleEndian, wantContains: "malformed counted typeX0 packet", wantConsumed: 3, wantBlankMeta: true},
	}
	for _, tc := range tests {
		t.Run(tc.name, func(t *testing.T) {
			TypeX0 = tc.option
			got := HandleTypeX0(tc.record, tc.endian, tc.noneFraming)
			assert.Equal(t, tc.wantConsumed, got.Consumed)
			assert.Equal(t, tc.wantBlankMeta, got.BlankMetadata)
			if tc.wantContains != "" {
				assert.Contains(t, got.Text, tc.wantContains)
				return
			}
			assert.Equal(t, tc.wantText, got.Text)
		})
	}
}

// TestLocationInformation validates location string formatting with and without verbose/lookup data.
func TestLocationInformation(t *testing.T) {
	oldFmt := LocationInformationFormatString
	oldVerbose := Verbose
	t.Cleanup(func() {
		LocationInformationFormatString = oldFmt
		Verbose = oldVerbose
	})

	li := id.TriceIDLookUpLI{
		12: {File: "/tmp/demo/main.c", Line: 77},
	}

	LocationInformationFormatString = "%s:%d "
	assert.Equal(t, "main.c:77 ", LocationInformation(12, li))
	assert.Equal(t, ":0 ", LocationInformation(13, li))

	LocationInformationFormatString = "off"
	Verbose = false
	assert.Equal(t, "", LocationInformation(12, li))

	Verbose = true
	assert.Equal(t, "no li", LocationInformation(12, li))
}

// TestLocationInformationWidth exercises the regex-based width calculator to ensure consistent output lengths.
func TestLocationInformationWidth(t *testing.T) {
	oldFmt := LocationInformationFormatString
	t.Cleanup(func() { LocationInformationFormatString = oldFmt })

	LocationInformationFormatString = "info:%21s%6d "
	assert.Equal(t, 28, locationInformationWidth())
}

// TestCorrectWrappedTimestamp verifies timestamp wrap handling returns future-corrected times inside reasonablen range.
func TestCorrectWrappedTimestamp(t *testing.T) {
	// Valid range: unchanged.
	valid := uint32(time.Date(2005, 1, 1, 0, 0, 0, 0, time.UTC).Unix())
	tv := CorrectWrappedTimestamp(valid)
	assert.Equal(t, 2005, tv.Year())

	// Out-of-range low value: wrapped to plausible year.
	tw := CorrectWrappedTimestamp(0)
	assert.Equal(t, 2106, tw.Year())
}

// TestCorrectWrappedTimestampFallback ensures fallback branch logs a warning and returns transformed time for invalid input.
func TestCorrectWrappedTimestampFallback(t *testing.T) {
	// Silence warning output from fallback path.
	oldStdout := os.Stdout
	r, w, err := os.Pipe()
	assert.NoError(t, err)
	os.Stdout = w
	t.Cleanup(func() {
		_ = w.Close()
		os.Stdout = oldStdout
		_, _ = io.ReadAll(r)
		_ = r.Close()
	})

	// Max uint32 is outside accepted range and wrapped value is too far in the future.
	tf := CorrectWrappedTimestamp(^uint32(0))
	assert.Equal(t, 2106, tf.Year())
}
