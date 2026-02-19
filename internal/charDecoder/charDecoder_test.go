package charDecoder

import (
	"bytes"
	"io"
	"strings"
	"sync"
	"testing"

	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/id"
	"github.com/stretchr/testify/assert"
)

func readAllViaDecoder(t *testing.T, dec decoder.Decoder, chunkSize int) string {
	t.Helper()
	buf := make([]byte, chunkSize)
	var out strings.Builder
	for {
		n, err := dec.Read(buf)
		if n > 0 {
			out.Write(buf[:n])
		}
		if err == io.EOF {
			return out.String()
		}
		assert.NoError(t, err)
	}
}

func TestNewInitializesDecoderData(t *testing.T) {
	var out bytes.Buffer
	in := strings.NewReader("abc")
	lut := id.TriceIDLookUp{1: id.TriceFmt{Type: "trice", Strg: "hello"}}
	li := id.TriceIDLookUpLI{1: id.TriceLI{File: "f.c", Line: 3}}
	m := &sync.RWMutex{}

	dec := New(&out, lut, m, li, in, decoder.BigEndian)
	p, ok := dec.(*char)
	if !ok {
		t.Fatalf("unexpected decoder type %T", dec)
	}

	assert.Same(t, &out, p.W)
	assert.Same(t, in, p.In)
	assert.Equal(t, decoder.DefaultSize, cap(p.IBuf))
	assert.Equal(t, lut, p.Lut)
	assert.Same(t, m, p.LutMutex)
	assert.Equal(t, decoder.BigEndian, p.Endian)
}

func TestReadPassThrough(t *testing.T) {
	var out bytes.Buffer
	in := strings.NewReader("ABC012")
	dec := New(&out, nil, nil, nil, in, decoder.LittleEndian)

	assert.Equal(t, "ABC012", readAllViaDecoder(t, dec, 2))
}

func TestSetInputSwitchesReader(t *testing.T) {
	var out bytes.Buffer
	dec := New(&out, nil, nil, nil, strings.NewReader("first"), decoder.LittleEndian)

	assert.Equal(t, "first", readAllViaDecoder(t, dec, 8))

	dec.SetInput(strings.NewReader("second"))
	assert.Equal(t, "second", readAllViaDecoder(t, dec, 8))
}

func TestReadWithoutInputReturnsEOF(t *testing.T) {
	var out bytes.Buffer
	dec := New(&out, nil, nil, nil, nil, decoder.LittleEndian)

	b := make([]byte, 8)
	n, err := dec.Read(b)
	assert.Equal(t, 0, n)
	assert.Equal(t, io.EOF, err)
}
