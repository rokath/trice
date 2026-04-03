// SPDX-License-Identifier: MIT

package dumpDecoder

import (
	"bytes"
	"fmt"
	"io"
	"io/ioutil"
	"strings"
	"testing"

	"github.com/rokath/trice/internal/decoder"
	"github.com/stretchr/testify/assert"
)

// doTableTest is the universal decoder test sequence.
func doDUMPtableTest(t *testing.T, out io.Writer, f decoder.New, endianness bool, teTa decoder.TestTable) {
	for _, x := range teTa {
		buf := make([]byte, decoder.DefaultSize)
		dec := f(out, nil, nil, nil, nil, endianness) // a new decoder instance
		in := ioutil.NopCloser(bytes.NewBuffer(x.In))
		dec.SetInput(in)
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
		assert.Equal(t, x.Exp, act)
	}
}

// TestDUMP verifies the expected behavior.
func TestDUMP(t *testing.T) {
	decoder.DumpLineByteCount = 8
	tt := decoder.TestTable{ // little endian
		{[]byte{'A', 'B', 'C', '0', '1', '2'}, `41 42 43 30 31 32 `},
		{[]byte{0, 0, 0, 4, 0, 0, 0, 8, 0, 0, 0, 12, 0, 0, 0, 16, 0, 0, 0, 20, 0, 0, 0, 24, 0, 0, 0, 28, 0, 0, 0, 32, 33, 34}, "00 00 00 04 00 00 00 08 \\n00 00 00 0c 00 00 00 10 \\n00 00 00 14 00 00 00 18 \\n00 00 00 1c 00 00 00 20 \\n21 22 "},
	}
	var out bytes.Buffer
	doDUMPtableTest(t, &out, New, decoder.LittleEndian, tt)
}

// TestReadWithoutInputReturnsEOF verifies the expected behavior.
func TestReadWithoutInputReturnsEOF(t *testing.T) {
	dec := New(io.Discard, nil, nil, nil, nil, decoder.LittleEndian)
	buf := make([]byte, 16)
	n, err := dec.Read(buf)
	assert.Equal(t, 0, n)
	assert.Equal(t, io.EOF, err)
}

// TestReadWithTooSmallBufferFails verifies the expected behavior.
func TestReadWithTooSmallBufferFails(t *testing.T) {
	dec := New(io.Discard, nil, nil, nil, bytes.NewBufferString("x"), decoder.LittleEndian)
	buf := make([]byte, 3)
	n, err := dec.Read(buf)
	assert.Equal(t, 0, n)
	assert.NotNil(t, err)
}

// TestReadKeepsLineCounterAcrossReads verifies the expected behavior.
func TestReadKeepsLineCounterAcrossReads(t *testing.T) {
	decoder.DumpLineByteCount = 4
	in := bytes.NewBuffer([]byte{0x01, 0x02, 0x03, 0x04, 0x05})
	dec := New(io.Discard, nil, nil, nil, in, decoder.LittleEndian)

	buf := make([]byte, 8)
	n1, err1 := dec.Read(buf)
	assert.NoError(t, err1)
	assert.Equal(t, "01 02 ", string(buf[:n1]))

	n2, err2 := dec.Read(buf)
	assert.NoError(t, err2)
	assert.Equal(t, "03 04 \\n", string(buf[:n2]))

	n3, err3 := dec.Read(buf)
	assert.True(t, err3 == nil || err3 == io.EOF)
	assert.Equal(t, "05 ", string(buf[:n3]))
}
