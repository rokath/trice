// SPDX-License-Identifier: MIT

// Package id List is responsible for id List managing
package id

// List management

import (
	"fmt"
	"sort"
	"strconv"
	"strings"

	"github.com/rokath/trice/pkg/msg"
	"github.com/spf13/afero"
)

// ToFileTilC generates a C receive-side log metadata source.
func (ilu TriceIDLookUp) ToFileTilC(fSys afero.Fs, fn string) (err error) {
	fh, err := fSys.Create(fn)
	msg.FatalOnErr(err)
	defer func() {
		err = fh.Close()
		msg.FatalOnErr(err)
	}()

	text, err := ilu.toListTilC(fn)
	msg.FatalOnErr(err)
	_, err = fh.Write(text)
	msg.FatalOnErr(err)
	return
}

// toListTilC converts ilu into a human-readable C-source byte slice.
func (ilu TriceIDLookUp) toListTilC(filename string) ([]byte, error) {
	text := []byte(`// SPDX-License-Identifier: MIT

// File: ` + filename + `

// Trice generated code - do not edit.

// triceLog stores compact receive-side facts derived from til.json.
// bitWidth is generated because it is not present in the binary record.
// paramCount is exact for scalar Trices; distinct dynamic family markers keep
// strings, buffers, functions, and ABC records unambiguous at runtime.
//
// Feature guards use the target's triceConfig.h through triceRx.h. A disabled
// row either retains only ID/shape metadata for a useful runtime diagnostic or
// disappears completely when TRICE_LOCAL_LOG_KEEP_DISABLED_IDS is zero.

#include "triceRx.h"

const triceLog_t triceLog[] = {
	/* Trice type ( extended ) */  /*   id, bitWidth, paramCount, format-string */
`)
	defaultBitWidth, err := strconv.Atoi(DefaultTriceBitWidth)
	if err != nil {
		return nil, fmt.Errorf("invalid default Trice bit width %q: %w", DefaultTriceBitWidth, err)
	}

	ids := make([]int, 0, len(ilu))
	for id := range ilu {
		ids = append(ids, int(id))
	}
	sort.Ints(ids)

	for _, n := range ids {
		id := TriceID(n)
		t := ilu[id]
		extType, bitWidth, paramCount := computeLogValues(t, defaultBitWidth)
		quotedFormat := tilCFormatLiteral(t.Strg)
		row := fmt.Sprintf(`	/* %-10s ( %-10s ) */ { %5du, %3du, %s, %s },`+"\n", t.Type, extType, id, bitWidth, paramCount, quotedFormat)
		condition := logFeatureCondition(t, bitWidth)
		if condition == "1" {
			text = append(text, []byte(row)...)
			continue
		}
		text = append(text, []byte("#if "+condition+"\n")...)
		text = append(text, []byte(row)...)
		text = append(text, []byte("#elif TRICE_LOCAL_LOG_KEEP_DISABLED_IDS == 1\n")...)
		text = append(text, []byte(fmt.Sprintf(`	/* %-10s ( %-10s ) */ { %5du, %3du, %s, 0 }, // Known ID; local feature disabled.`+"\n", t.Type, extType, id, bitWidth, paramCount))...)
		text = append(text, []byte("#endif\n")...)
	}
	// A permanent trailing sentinel makes the declaration valid even when all
	// real rows are compiled out. It is excluded from triceLogElements.
	text = append(text, []byte("\t{ 0u, 0u, 0u, 0 }, // Portable generated-table sentinel.\n")...)
	text = append(text, []byte(`};

// triceLogElements is used by the RX resolver to bound the generated table.
`)...)
	text = append(text, []byte("const unsigned triceLogElements = sizeof(triceLog) / sizeof(triceLog[0]) - 1u;\n")...)
	return text, nil
}

// logFeatureCondition maps a TIL entry to the smallest set of target-side
// configuration switches needed to retain its format string. It consumes the
// shared fmtspec parser result instead of growing a second printf parser in the
// generator. Generated and runtime checks remain deliberately redundant: the
// former saves flash while the latter protects hand-written/stale tables.
func logFeatureCondition(t TriceFmt, bitWidth int) string {
	category := triceTypeCategory(t.Type)
	specs := formatSpecifierSpecs(t.Strg)
	conditions := make([]string, 0, 8)
	add := func(condition string) {
		for _, existing := range conditions {
			if existing == condition {
				return
			}
		}
		conditions = append(conditions, condition)
	}

	if info := abcTypeInfo(t.Type); info.isABC {
		return "0"
	}
	switch category {
	case "S", "N":
		add("TRICE_LOCAL_LOG_USE_DYNAMIC_STRING_TRICES == 1")
		if len(specs) != 1 || (specs[0].Verb != 's' && specs[0].Verb != 'q') {
			return "0"
		}
		if strings.Trim(specs[0].Flags, "-") != "" {
			return "0"
		}
	case "B":
		add("TRICE_LOCAL_LOG_USE_BUFFER_TRICES == 1")
		if len(specs) != 1 {
			return "0"
		}
	case "F":
		return "0"
	}
	if bitWidth == 64 && (category == "B" || (category != "S" && category != "N" && len(specs) != 0)) {
		add("TRICE_LOCAL_LOG_USE_64_BIT_VALUES == 1")
	}

	hasMinimalScalar := false
	needsHook := false
	for _, spec := range specs {
		if spec.HasFieldWidth {
			add("TRICE_LOCAL_LOG_USE_FIELD_WIDTH_FORMAT_SPECIFIERS == 1")
		}
		if spec.HasPrecision {
			add("TRICE_LOCAL_LOG_USE_PRECISION_FORMAT_SPECIFIERS == 1")
		}
		if spec.HasAltForm {
			add("TRICE_LOCAL_LOG_USE_ALT_FORM_FLAG == 1")
		}
		switch spec.Verb {
		case 'd', 'x':
			if spec.HasFlags || spec.HasFieldWidth || spec.HasPrecision {
				needsHook = true
			} else {
				hasMinimalScalar = true
			}
		case 'b':
			if strings.Trim(spec.Flags, "-0#") != "" {
				return "0"
			}
			add("TRICE_LOCAL_LOG_USE_BINARY_FORMAT_SPECIFIERS == 1")
		case 'O', 'p':
			if strings.Trim(spec.Flags, "-0#") != "" {
				return "0"
			}
			add("TRICE_LOCAL_LOG_USE_EXTENDED_FORMAT_SPECIFIERS == 1")
		case 't', 'q':
			if strings.Trim(spec.Flags, "-") != "" {
				return "0"
			}
			if spec.HasPrecision && (spec.Verb == 't' || (category != "S" && category != "N")) {
				return "0"
			}
			add("TRICE_LOCAL_LOG_USE_EXTENDED_FORMAT_SPECIFIERS == 1")
		case 's':
			if category != "S" && category != "N" {
				return "0"
			}
		case 'e', 'E', 'f', 'F', 'g', 'G':
			add("TRICE_LOCAL_LOG_USE_FLOAT_FORMAT_SPECIFIERS == 1")
			needsHook = true
		case 'i', 'u', 'o', 'X', 'c':
			needsHook = true
		default:
			return "0"
		}
	}
	if needsHook {
		add("TRICE_LOCAL_LOG_USE_PRINTF_HOOK == 1")
	} else if hasMinimalScalar {
		add("(TRICE_LOCAL_LOG_USE_PRINTF_HOOK == 1 || TRICE_LOCAL_LOG_USE_MINIMAL_FORMATTER == 1)")
	}
	if len(conditions) == 0 {
		return "1"
	}
	return strings.Join(conditions, " && ")
}

// tilCFormatLiteral converts the TIL-stored source spelling into a generated C
// string literal. JSON unmarshalling already consumed JSON escapes, but Strg may
// still contain C string escapes copied from the original Trice call, such as
// \n, \r, \" or \\. Those escapes must be interpreted once so rx->pFmt has the
// same runtime text as the target-side C string literal.
func tilCFormatLiteral(format string) string {
	return strconv.Quote(decodeCStringEscapes(format))
}

// decodeCStringEscapes implements the C string escapes used in Trice format
// strings. Unknown escape pairs are kept byte-for-byte to avoid silently losing
// literal backslashes from existing TIL files.
func decodeCStringEscapes(s string) string {
	var b strings.Builder
	b.Grow(len(s))

	for i := 0; i < len(s); i++ {
		if s[i] != '\\' || i+1 >= len(s) {
			b.WriteByte(s[i])
			continue
		}

		i++
		switch s[i] {
		case 'a':
			b.WriteByte('\a')
		case 'b':
			b.WriteByte('\b')
		case 'f':
			b.WriteByte('\f')
		case 'n':
			b.WriteByte('\n')
		case 'r':
			b.WriteByte('\r')
		case 't':
			b.WriteByte('\t')
		case 'v':
			b.WriteByte('\v')
		case '\\':
			b.WriteByte('\\')
		case '\'', '"', '?':
			b.WriteByte(s[i])
		case '0', '1', '2', '3', '4', '5', '6', '7':
			value := int(s[i] - '0')
			for digits := 1; digits < 3 && i+1 < len(s) && isOctalDigit(s[i+1]); digits++ {
				i++
				value = value*8 + int(s[i]-'0')
			}
			b.WriteByte(byte(value))
		case 'x':
			value := 0
			digits := 0
			for i+1 < len(s) {
				digit, ok := hexDigitValue(s[i+1])
				if !ok {
					break
				}
				i++
				digits++
				value = value*16 + digit
			}
			if digits == 0 {
				b.WriteString(`\x`)
			} else {
				b.WriteByte(byte(value))
			}
		default:
			b.WriteByte('\\')
			b.WriteByte(s[i])
		}
	}

	return b.String()
}

func isOctalDigit(c byte) bool {
	return '0' <= c && c <= '7'
}

func hexDigitValue(c byte) (int, bool) {
	switch {
	case '0' <= c && c <= '9':
		return int(c - '0'), true
	case 'a' <= c && c <= 'f':
		return int(c-'a') + 10, true
	case 'A' <= c && c <= 'F':
		return int(c-'A') + 10, true
	default:
		return 0, false
	}
}

// computeLogValues prepares the compact C RX table. The uint8_t paramCount
// cannot use -1, so variable-size payload families use the public sentinel.
func computeLogValues(t TriceFmt, defaultBitWidth int) (extType string, bitWidth int, paramCount string) {
	DefaultTriceBitWidth = strconv.Itoa(defaultBitWidth)
	if info := abcTypeInfo(t.Type); info.isABC {
		if info.bitWidth == 0 {
			return t.Type, 0, "0u"
		}
		return t.Type, info.bitWidth, "TRICE_LOG_PARAM_COUNT_DYNAMIC_ABC"
	}
	switch triceTypeCategory(t.Type) {
	case "S", "N":
		return t.Type, 8, "TRICE_LOG_PARAM_COUNT_DYNAMIC_STRING"
	case "B":
		extType, bitWidth = logDynamicTypeWidth(t.Type, 1, defaultBitWidth)
		return extType, bitWidth, "TRICE_LOG_PARAM_COUNT_DYNAMIC_BUFFER"
	case "F":
		extType, bitWidth = logDynamicTypeWidth(t.Type, 0, defaultBitWidth)
		return extType, bitWidth, "TRICE_LOG_PARAM_COUNT_DYNAMIC_FUNCTION"
	default:
		count := formatSpecifierCount(t.Strg)
		extType, _ = ConstructFullTriceInfo(t.Type, count)
		bitWidth = logBitWidthFromType(extType, defaultBitWidth)
		return extType, bitWidth, fmt.Sprintf("%du", count)
	}
}

// logDynamicTypeWidth uses existing macro normalization for buffer-like Trices.
// If normalization fails, the original type still keeps generation diagnostic
// value in the comment while the default width preserves historic behavior.
func logDynamicTypeWidth(typeName string, paramCount int, defaultBitWidth int) (extType string, bitWidth int) {
	extType, _ = ConstructFullTriceInfo(typeName, paramCount)
	if extType == "" {
		extType = typeName
	}
	return extType, logBitWidthFromType(extType, defaultBitWidth)
}

// logBitWidthFromType extracts explicit 8/16/32/64 widths. Types without an
// explicit width keep the configured default, except ABC no-payload C entries,
// which are handled before this helper is called.
func logBitWidthFromType(typeName string, defaultBitWidth int) int {
	upper := strings.ToUpper(typeName)
	for _, width := range []int{64, 32, 16, 8} {
		if strings.Contains(upper, strconv.Itoa(width)) {
			return width
		}
	}
	return defaultBitWidth
}
