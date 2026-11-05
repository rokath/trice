// SPDX-License-Identifier: MIT

package decoder

import (
	"bytes"
	"fmt"
	"strconv"
	"strings"
	"sync"
	"testing"
	"time"
	"unicode/utf8"

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

// TestStatisticsLocationColumns checks report alignment, width caps and retained
// path suffixes, independently of the map's row order and terminal color codes.
func TestStatisticsLocationColumns(t *testing.T) {
	for _, tc := range []struct {
		name, format, path, expected string
		width, maxDirs               int
	}{
		{"short_paths_shrink", "%32s%4d", "src/main.c", "src/main.c", 20, 3},
		{"long_paths_are_capped", "%32s%4d", "examples/exampleData/triceLogDiagData.c", "…/exampleData/triceLogDiagData.c", 32, 3},
		{"directory_suffix_retained", "%12s%4d", "one/two/verylong.c", "…/verylong.c", 12, 3},
		{"left_aligned_format", "%-12s%4d", "one/two/verylong.c", "…/verylong.c", 12, 3},
		{"index_after_width", "%12[1]s%4[2]d", "one/two/verylong.c", "…/verylong.c", 12, 3},
		{"escaped_percent_ignored", "%%64s:%12s%4d", "one/two/verylong.c", "…/verylong.c", 12, 3},
		{"no_width_keeps_full_path", "%s:%d", "examples/exampleData/triceLogDiagData.c", "examples/exampleData/triceLogDiagData.c", 39, 3},
		{"unbounded_first_string", "%s %%32s %d", "examples/exampleData/triceLogDiagData.c", "examples/exampleData/triceLogDiagData.c", 39, 3},
		{"unicode_suffix", "%11s%4d", "src/überlang.c", "…überlang.c", 11, 3},
		{"one_character_limit", "%1s%4d", "src/main.c", "…", 1, 3},
		{"liMaxDirs_applies_first", "%32s%4d", "examples/exampleData/triceLogDiagData.c", "triceLogDiagData.c", 20, 0},
		{"missing_location", "%32s%4d", "", ".", 20, 0},
	} {
		t.Run(tc.name, func(t *testing.T) {
			assert.NotContains(t, fmt.Sprintf(tc.format, tc.path, 23), "%!", "the width rule must match a usable Go location format")
			// Restore every shared input so report sizing cannot leak to other tests.
			oldEnabled, oldAll, oldCounts := TriceStatistics, emitter.AllStatistics, IDStat
			oldTIL, oldLI, oldFormat := IDLUT, LILUT, LocationInformationFormatString
			oldMaxDirs, oldPalette := id.LIMaxDirs, emitter.ColorPalette
			t.Cleanup(func() {
				TriceStatistics, emitter.AllStatistics, IDStat = oldEnabled, oldAll, oldCounts
				IDLUT, LILUT, LocationInformationFormatString = oldTIL, oldLI, oldFormat
				id.LIMaxDirs, emitter.ColorPalette = oldMaxDirs, oldPalette
			})
			TriceStatistics, emitter.AllStatistics = true, false
			IDStat = map[id.TriceID]int{7: 2, 8: 1}
			IDLUT = id.TriceIDLookUp{7: {Type: "TRICE", Strg: "hello\\n"}, 8: {Type: "trice", Strg: "another message\\n"}}
			LILUT = id.TriceIDLookUpLI{
				7: {File: tc.path, Line: 23},
				8: {File: "src/z.c", Line: 4},
				// Locations without recorded events must not widen the report.
				9: {File: "unseen/very-very-very-long-file-without-any-recorded-events.c", Line: 1},
			}
			LocationInformationFormatString = tc.format
			id.LIMaxDirs, emitter.ColorPalette = tc.maxDirs, "none"
			var output bytes.Buffer
			PrintTriceStatistics(&output)
			// Every separator must occupy the heading's column in both rules and rows.
			var boundaries []int
			for _, line := range strings.Split(output.String(), "\n") {
				if !strings.Contains(line, "|") {
					continue
				}
				var positions []int
				for position, character := range []rune(line) {
					if character == '|' {
						positions = append(positions, position)
					}
				}
				if boundaries == nil {
					boundaries = positions
				}
				assert.Equal(t, boundaries, positions, line)
				fields := strings.Split(line, "|")
				assert.Equal(t, tc.width+2, utf8.RuneCountInString(fields[1]), line)
				if _, err := strconv.Atoi(strings.TrimSpace(fields[0])); err == nil {
					expectedPath := tc.expected
					if strings.TrimSpace(fields[3]) == "8" {
						expectedPath = "src/z.c"
						if tc.maxDirs == 0 {
							expectedPath = "z.c"
						}
						if tc.width == 1 {
							expectedPath = "…"
						}
					}
					assert.Equal(t, expectedPath, strings.TrimSpace(fields[1]))
					assert.True(t, utf8.ValidString(fields[1]))
				}
			}
			assert.Contains(t, output.String(), "3 Trice messsges")
		})
	}
}

// TestStatisticsTypeColumns checks that only printed type names determine the
// column width and that headings, rules and rows retain identical boundaries.
func TestStatisticsTypeColumns(t *testing.T) {
	for _, tc := range []struct {
		name  string
		types []string
		width int
	}{
		{"empty_report_keeps_heading", nil, 4},
		{"missing_type_keeps_heading", []string{""}, 4},
		{"short_names_shrink", []string{"TRICE", "trice"}, 5},
		{"heading_is_minimum", []string{"T", "X"}, 4},
		{"ten_character_name", []string{"trice16_12", "TRICE"}, 10},
		{"assert_name_expands", []string{"triceAssert", "TRICE"}, 11},
		{"long_assert_name_expands", []string{"TRice32_1", "TriceAssertOrReturnValue"}, 24},
	} {
		t.Run(tc.name, func(t *testing.T) {
			// Keep report configuration and event counts local to each case.
			oldEnabled, oldAll, oldCounts := TriceStatistics, emitter.AllStatistics, IDStat
			oldTIL, oldLI, oldPalette := IDLUT, LILUT, emitter.ColorPalette
			t.Cleanup(func() {
				TriceStatistics, emitter.AllStatistics, IDStat = oldEnabled, oldAll, oldCounts
				IDLUT, LILUT, emitter.ColorPalette = oldTIL, oldLI, oldPalette
			})
			TriceStatistics, emitter.AllStatistics, emitter.ColorPalette = true, false, "none"
			IDStat, IDLUT, LILUT = map[id.TriceID]int{}, id.TriceIDLookUp{}, id.TriceIDLookUpLI{}
			for i, name := range tc.types {
				tid := id.TriceID(i + 1)
				IDStat[tid] = 1
				IDLUT[tid] = id.TriceFmt{Type: name, Strg: "message\\n"}
			}
			// Dictionary entries without recorded events must not widen the column.
			IDLUT[99] = id.TriceFmt{Type: "UnrecordedTypeLongerThanEveryPrintedName"}
			var output bytes.Buffer
			PrintTriceStatistics(&output)
			var boundaries []int
			rows := 0
			for _, line := range strings.Split(output.String(), "\n") {
				if !strings.Contains(line, "|") {
					continue
				}
				var positions []int
				for position, character := range []rune(line) {
					if character == '|' {
						positions = append(positions, position)
					}
				}
				if boundaries == nil {
					boundaries = positions
				}
				assert.Equal(t, boundaries, positions, line)
				fields := strings.Split(line, "|")
				assert.Equal(t, tc.width+2, utf8.RuneCountInString(fields[4]), line)
				if _, err := strconv.Atoi(strings.TrimSpace(fields[0])); err == nil {
					tid, err := strconv.Atoi(strings.TrimSpace(fields[3]))
					assert.NoError(t, err)
					assert.Equal(t, IDLUT[id.TriceID(tid)].Type, strings.TrimSpace(fields[4]))
					rows++
				}
			}
			assert.NotEmpty(t, boundaries, "the report must include its column headings")
			assert.Equal(t, len(tc.types), rows, "each recorded type must remain visible")
		})
	}
}

// TestStatisticsWaitsForDictionaryReload checks that a report cannot inspect
// maps while a reload is replacing them. Once released, it uses the new entry
// and location together; the per-ID event count is independent of the reload.
func TestStatisticsWaitsForDictionaryReload(t *testing.T) {
	oldEnabled, oldCounts := TriceStatistics, IDStat
	oldTIL, oldLI, oldMutex := IDLUT, LILUT, LUTMutex
	t.Cleanup(func() {
		TriceStatistics, IDStat = oldEnabled, oldCounts
		IDLUT, LILUT, LUTMutex = oldTIL, oldLI, oldMutex
	})
	TriceStatistics = true
	IDStat = map[id.TriceID]int{7: 3}
	IDLUT, LILUT = id.TriceIDLookUp{}, id.TriceIDLookUpLI{}
	LUTMutex = new(sync.RWMutex)
	LUTMutex.Lock()
	var out bytes.Buffer
	started, finished := make(chan struct{}), make(chan struct{})
	go func() {
		close(started)
		PrintTriceStatistics(&out)
		close(finished)
	}()
	<-started
	select {
	case <-finished:
		t.Error("statistics accessed dictionaries while their writer still held the lock")
	case <-time.After(20 * time.Millisecond):
	}
	IDLUT[7] = id.TriceFmt{Type: "TRICE", Strg: "reloaded message"}
	LILUT[7] = id.TriceLI{File: "reloaded.c", Line: 42}
	LUTMutex.Unlock()
	<-finished
	assert.Contains(t, out.String(), "reloaded message")
	assert.Contains(t, out.String(), "reloaded.c")
	assert.Contains(t, out.String(), "3 Trice messsges")
}

// TestHandleTypeX0 verifies the initial selector-0 handling modes and shorthand rules.
func TestHandleTypeX0(t *testing.T) {
	oldTypeX0 := TypeX0
	t.Cleanup(func() { TypeX0 = oldTypeX0 })

	tests := []struct {
		name           string
		option         string
		record         []byte
		endian         bool
		noneFraming    bool
		wantText       string
		wantContains   string
		wantConsumed   int
		wantBlankMeta  bool
		wantDiagnostic bool
		wantTag        string
	}{
		{name: "default error", option: "error", record: []byte{0x02, 0x00, 'O', 'K'}, endian: LittleEndian, wantContains: "typeX0 packet ignored", wantConsumed: 4, wantBlankMeta: true, wantDiagnostic: true},
		{name: "counted string shorthand", option: "%s", record: []byte{0x02, 0x00, 'O', 'K'}, endian: LittleEndian, wantText: "OK", wantConsumed: 4, wantBlankMeta: true},
		{name: "counted explicit with colon format", option: "counted:sig:%s", record: []byte{0x02, 0x00, 'O', 'K'}, endian: LittleEndian, wantText: "sig:OK", wantConsumed: 4, wantBlankMeta: true, wantTag: "sig"},
		{name: "colon shorthand rejected", option: "sig:%s", record: []byte{0x02, 0x00, 'O', 'K'}, endian: LittleEndian, wantContains: `unsupported typeX0 mode "sig"`, wantConsumed: 4, wantBlankMeta: true, wantDiagnostic: true},
		{name: "counted ignore", option: "ignore", record: []byte{0x02, 0x00, 'O', 'K'}, endian: LittleEndian, wantConsumed: 4},
		{name: "all ignore consumes package", option: "all:ignore", record: []byte{0x02, 0x00, 'O', 'K', 0x01, 0x40}, endian: LittleEndian, wantConsumed: 6},
		{name: "big endian counted", option: "%s", record: []byte{0x00, 0x02, 'O', 'K'}, endian: BigEndian, wantText: "OK", wantConsumed: 4, wantBlankMeta: true},
		{name: "none framing skips alignment", option: "%s", record: []byte{0x01, 0x00, 'A', 0x00}, endian: LittleEndian, noneFraming: true, wantText: "A", wantConsumed: 4, wantBlankMeta: true},
		{name: "none framing allows compact empty payload", option: "%s", record: []byte{0x00, 0x00}, endian: LittleEndian, noneFraming: true, wantText: "", wantConsumed: 2, wantBlankMeta: true},
		{name: "none framing keeps compact next record", option: "%s", record: []byte{0x01, 0x00, 'A', 0x01, 0x40}, endian: LittleEndian, noneFraming: true, wantText: "A", wantConsumed: 3, wantBlankMeta: true},
		{name: "framed rejects non-zero alignment padding", option: "%s", record: []byte{0x01, 0x00, 'A', 'B'}, endian: LittleEndian, wantContains: "non-zero alignment padding", wantConsumed: 4, wantBlankMeta: true, wantDiagnostic: true},
		{name: "malformed ignore still errors", option: "ignore", record: []byte{0x04, 0x00, 'A'}, endian: LittleEndian, wantContains: "malformed counted typeX0 packet", wantConsumed: 3, wantBlankMeta: true, wantDiagnostic: true},
	}
	for _, tc := range tests {
		t.Run(tc.name, func(t *testing.T) {
			TypeX0 = tc.option
			got := HandleTypeX0(tc.record, tc.endian, tc.noneFraming)
			assert.Equal(t, tc.wantConsumed, got.Consumed)
			assert.Equal(t, tc.wantBlankMeta, got.BlankMetadata)
			assert.Equal(t, tc.wantDiagnostic, got.Diagnostic)
			assert.Equal(t, tc.wantTag, got.Tag)
			if tc.wantContains != "" {
				assert.Contains(t, got.Text, tc.wantContains)
				return
			}
			assert.Equal(t, tc.wantText, got.Text)
		})
	}
}

// TestLocationInformationWidth exercises the regex-based width calculator to ensure consistent output lengths.
func TestLocationInformationWidth(t *testing.T) {
	oldFmt := LocationInformationFormatString
	t.Cleanup(func() { LocationInformationFormatString = oldFmt })

	LocationInformationFormatString = "info:%21s%6d "
	assert.Equal(t, 28, locationInformationWidth())
}
