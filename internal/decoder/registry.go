package decoder

import (
	"fmt"
	"io"
	"strings"
	"sync"

	"github.com/rokath/trice/internal/id"
)

var (
	registryMu sync.RWMutex
	registry   = map[string]New{}
)

func normalizeEncoding(name string) string {
	return strings.ToUpper(strings.TrimSpace(name))
}

// Register associates an encoding name with a decoder constructor.
//
// Names are matched case-insensitively.
func Register(name string, ctor New) {
	key := normalizeEncoding(name)
	if key == "" {
		panic("decoder: empty encoding name")
	}
	if ctor == nil {
		panic("decoder: nil constructor")
	}
	registryMu.Lock()
	registry[key] = ctor
	registryMu.Unlock()
}

// NewForEncoding creates a decoder instance for the requested encoding.
func NewForEncoding(encoding string, out io.Writer, lut id.TriceIDLookUp, m *sync.RWMutex, li id.TriceIDLookUpLI, in io.Reader, endian bool) (Decoder, error) {
	key := normalizeEncoding(encoding)
	registryMu.RLock()
	ctor, ok := registry[key]
	registryMu.RUnlock()
	if !ok {
		return nil, fmt.Errorf("unknown encoding %q", encoding)
	}
	return ctor(out, lut, m, li, in, endian), nil
}
