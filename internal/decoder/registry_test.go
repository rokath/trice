// SPDX-License-Identifier: MIT

package decoder

import (
	"bytes"
	"io"
	"strings"
	"sync"
	"testing"

	"github.com/rokath/trice/internal/id"
)

type fakeDecoder struct {
	DecoderData
}

func (f *fakeDecoder) Read(p []byte) (int, error) {
	if f.In == nil {
		return 0, io.EOF
	}
	return f.In.Read(p)
}

func TestNewDecoderDataDefaults(t *testing.T) {
	d := NewDecoderData(Config{
		Out:         nil,
		LUTMutex:    nil,
		NeedBuffers: true,
	})
	if d.W == nil {
		t.Fatal("expected non-nil writer")
	}
	if d.LutMutex == nil {
		t.Fatal("expected non-nil LUT mutex")
	}
	if cap(d.IBuf) != DefaultSize {
		t.Fatalf("unexpected IBuf cap: %d", cap(d.IBuf))
	}
	if cap(d.B) != DefaultSize || len(d.B0) != DefaultSize || len(d.InnerBuffer) != DefaultSize {
		t.Fatal("expected packet buffers to be allocated")
	}
}

func TestNewForEncodingUsesRegistry(t *testing.T) {
	const enc = "UNIT_TEST_DECODER"
	Register(enc, func(out io.Writer, lut id.TriceIDLookUp, m *sync.RWMutex, li id.TriceIDLookUpLI, in io.Reader, endian bool) Decoder {
		return &fakeDecoder{
			DecoderData: NewDecoderData(Config{
				Out:      out,
				LUT:      lut,
				LUTMutex: m,
				LI:       li,
				In:       in,
				Endian:   endian,
			}),
		}
	})

	in := strings.NewReader("ok")
	var out bytes.Buffer
	dec, err := NewForEncoding(strings.ToLower(enc), &out, nil, nil, nil, in, LittleEndian)
	if err != nil {
		t.Fatalf("unexpected error: %v", err)
	}
	buf := make([]byte, 8)
	n, rerr := dec.Read(buf)
	if rerr != nil && rerr != io.EOF {
		t.Fatalf("unexpected read error: %v", rerr)
	}
	if string(buf[:n]) != "ok" {
		t.Fatalf("unexpected decoder output: %q", string(buf[:n]))
	}
}

func TestNewForEncodingUnknown(t *testing.T) {
	if _, err := NewForEncoding("does-not-exist", io.Discard, nil, nil, nil, nil, LittleEndian); err == nil {
		t.Fatal("expected error for unknown encoding")
	}
}

func TestRegisterPanicsForInvalidInput(t *testing.T) {
	assertPanic := func(name string, fn func()) {
		t.Helper()
		defer func() {
			if recover() == nil {
				t.Fatalf("expected panic for %s", name)
			}
		}()
		fn()
	}

	assertPanic("empty name", func() {
		Register("  ", func(out io.Writer, lut id.TriceIDLookUp, m *sync.RWMutex, li id.TriceIDLookUpLI, in io.Reader, endian bool) Decoder {
			return &fakeDecoder{}
		})
	})
	assertPanic("nil constructor", func() { Register("X", nil) })
}

func TestNewForEncodingTrimsAndNormalizesCase(t *testing.T) {
	const enc = "UNIT_TEST_DECODER_TRIM"
	Register(enc, func(out io.Writer, lut id.TriceIDLookUp, m *sync.RWMutex, li id.TriceIDLookUpLI, in io.Reader, endian bool) Decoder {
		return &fakeDecoder{
			DecoderData: NewDecoderData(Config{
				Out:      out,
				LUT:      lut,
				LUTMutex: m,
				LI:       li,
				In:       in,
				Endian:   endian,
			}),
		}
	})

	in := strings.NewReader("ok")
	var out bytes.Buffer
	dec, err := NewForEncoding("  unit_test_decoder_trim  ", &out, nil, nil, nil, in, LittleEndian)
	if err != nil {
		t.Fatalf("unexpected error: %v", err)
	}
	buf := make([]byte, 8)
	n, rerr := dec.Read(buf)
	if rerr != nil && rerr != io.EOF {
		t.Fatalf("unexpected read error: %v", rerr)
	}
	if string(buf[:n]) != "ok" {
		t.Fatalf("unexpected decoder output: %q", string(buf[:n]))
	}
}
