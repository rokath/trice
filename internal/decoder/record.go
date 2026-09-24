// SPDX-License-Identifier: MIT

package decoder

import (
	"fmt"
	"math"
	"unicode/utf8"

	"github.com/rokath/trice/internal/fmtspec"
	"github.com/rokath/trice/internal/id"
)

// LogFormat selects presentation only; templates are decoded in every mode.
var LogFormat = "text"

// FieldValue preserves a typed user value before presentation rounding,
// clipping, padding, or the legacy -unsigned option can alter its meaning.
type FieldValue struct {
	Name    string
	Value   any
	Address bool // Addresses use JSON strings but unquoted KV hexadecimal values.
}

// ApplicationRecord is one target call, independent of its newline count.
// The fixed host facts are separate from Fields so user names never collide.
type ApplicationRecord struct {
	Tag       string
	Message   string
	Fields    []FieldValue
	ID        id.TriceID
	HasID     bool
	Stamp     uint64
	StampBits int
}

// RecordProvider exposes a successfully decoded event even when its text is
// empty. A false result prevents diagnostics and malformed packets becoming logs.
type RecordProvider interface {
	ApplicationRecord() (ApplicationRecord, bool)
}

// ScalarField interprets bits according to the shared format contract, rather
// than re-parsing formatted text or relying on the user's -unsigned setting.
func ScalarField(field fmtspec.Field, bits int, raw uint64) (FieldValue, error) {
	value := FieldValue{Name: field.Name}
	if bits != 8 && bits != 16 && bits != 32 && bits != 64 {
		return value, fmt.Errorf("field %q has unsupported bit width %d", field.Name, bits)
	}
	if bits < 64 {
		raw &= (uint64(1) << bits) - 1
	}
	switch field.Spec.Verb {
	case 'c', 'q', 'U':
		character := rune(raw)
		if raw > utf8.MaxRune || !utf8.ValidRune(character) {
			character = utf8.RuneError
		}
		value.Value = string(character)
	default:
		switch field.Spec.Kind {
		case fmtspec.KindSigned:
			value.Value = int64(raw<<(64-bits)) >> (64 - bits)
		case fmtspec.KindUnsigned, fmtspec.KindBasedInteger:
			value.Value = raw
		case fmtspec.KindFloat:
			if bits == 32 {
				value.Value = float64(math.Float32frombits(uint32(raw)))
			} else if bits == 64 {
				value.Value = math.Float64frombits(raw)
			} else {
				return value, fmt.Errorf("floating field %q requires 32 or 64 bits", field.Name)
			}
		case fmtspec.KindBool:
			value.Value = raw != 0
		case fmtspec.KindPointer:
			value.Value = fmt.Sprintf("0x%x", raw)
			value.Address = true
		default:
			return value, fmt.Errorf("field %q is not a numeric scalar", field.Name)
		}
	}
	return value, nil
}
