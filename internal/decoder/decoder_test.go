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
		{"%d %u %f", "%d %d %f", []int{1, 0, 2}},
	}
	for i, x := range tt {
		fmt.Println(i, x)
		act, uct := UReplaceN(x.fmt)
		assert.Equal(t, x.exp, act)
		assert.Equal(t, x.u, uct)
	}
}

func TestNewDecoderDataWithoutBuffers(t *testing.T) {
	d := NewDecoderData(Config{})
	assert.NotNil(t, d.W)
	assert.NotNil(t, d.LutMutex)
	assert.Equal(t, DefaultSize, cap(d.IBuf))
	assert.Nil(t, d.B)
	assert.Nil(t, d.B0)
	assert.Nil(t, d.InnerBuffer)
}

func TestSetInput(t *testing.T) {
	d := NewDecoderData(Config{})
	r := strings.NewReader("abc")
	d.SetInput(r)
	assert.Same(t, r, d.In)
}

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

func TestSetInputAffectsRead(t *testing.T) {
	d := NewDecoderData(Config{In: strings.NewReader("old")})
	fd := fakeDecoder{DecoderData: d}

	fd.SetInput(bytes.NewBufferString("new"))
	b := make([]byte, 8)
	n, err := fd.Read(b)
	assert.NoError(t, err)
	assert.Equal(t, "new", string(b[:n]))
}

func TestDump(t *testing.T) {
	var out bytes.Buffer
	Dump(&out, []byte{0x00, 0x1f, 0xa0})
	assert.Equal(t, "00 1f a0 \n", out.String())
}

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

func TestLocationInformationWidth(t *testing.T) {
	oldFmt := LocationInformationFormatString
	t.Cleanup(func() { LocationInformationFormatString = oldFmt })

	LocationInformationFormatString = "info:%21s%6d "
	assert.Equal(t, 28, locationInformationWidth())
}

func TestCorrectWrappedTimestamp(t *testing.T) {
	// Valid range: unchanged.
	valid := uint32(time.Date(2005, 1, 1, 0, 0, 0, 0, time.UTC).Unix())
	tv := CorrectWrappedTimestamp(valid)
	assert.Equal(t, 2005, tv.Year())

	// Out-of-range low value: wrapped to plausible year.
	tw := CorrectWrappedTimestamp(0)
	assert.Equal(t, 2106, tw.Year())
}

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
