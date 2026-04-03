// SPDX-License-Identifier: MIT

package decoder

import (
	"bytes"
	"io"
	"sync"
	"testing"

	"github.com/rokath/trice/internal/id"
)

// fakeDecoder satisfies Decoder for tests and delegates to stored DecoderData if set.
type fakeDecoder struct {
	data *DecoderData
}

// Read proxies input read requests to its DecoderData to simulate a real decoder read path.
func (f *fakeDecoder) Read(p []byte) (int, error) {
	if f.data == nil || f.data.In == nil {
		return 0, io.EOF
	}
	return f.data.In.Read(p)
}

// SetInput transparently forwards input replacement to the underlying DecoderData.
func (f *fakeDecoder) SetInput(r io.Reader) {
	if f.data != nil {
		f.data.SetInput(r)
	}
}

// TestNormalizeEncoding ensures casing/whitespace normalization keeps keys predictable.
func TestNormalizeEncoding(t *testing.T) {
	got := normalizeEncoding("  tRICE  ")
	if got != "TRICE" {
		t.Fatalf("expected TRICE normalized name, got %q", got)
	}
}

// TestRegisterAndNewForEncoding covers the happy path for decoder registration.
func TestRegisterAndNewForEncoding(t *testing.T) {
	defer func() {
		registryMu.Lock()
		delete(registry, "TEST")
		registryMu.Unlock()
	}()

	Register("test", func(out io.Writer, lut id.TriceIDLookUp, m *sync.RWMutex, li id.TriceIDLookUpLI, in io.Reader, endian bool) Decoder {
		d := NewDecoderData(Config{Out: out, In: in, LUT: lut, LUTMutex: m, LI: li, Endian: endian})
		return &fakeDecoder{data: &d}
	})

	d, err := NewForEncoding(" TEST ", io.Discard, nil, nil, nil, bytes.NewReader(nil), LittleEndian)
	if err != nil {
		t.Fatalf("unexpected error: %v", err)
	}
	if _, ok := d.(*fakeDecoder); !ok {
		t.Fatalf("expected fakeDecoder instance, got %T", d)
	}
}

// TestRegisterPanics validates that invalid registration data triggers panics.
func TestRegisterPanics(t *testing.T) {
	defer func() {
		if r := recover(); r == nil {
			t.Fatal("expected panic for empty name")
		}
	}()
	Register("", nil)
}

// TestRegisterPanicsOnNilConstructor ensures panic when registering a nil constructor.
func TestRegisterPanicsOnNilConstructor(t *testing.T) {
	defer func() {
		if r := recover(); r == nil {
			t.Fatal("expected panic for nil constructor")
		}
	}()
	Register("ok", nil)
}
