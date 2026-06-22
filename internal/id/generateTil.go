// SPDX-License-Identifier: MIT

// Package id List is responsible for id List managing
package id

// List management

import (
	"fmt"
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
	text := []byte(`// File: ` + filename + `
// Trice generated code - do not edit.
//
// triceLog stores compact receive-side facts derived from til.json.
// bitWidth is generated because it is not present in the binary record.
// paramCount is exact for scalar Trices; TRICE_LOG_PARAM_COUNT_DYNAMIC means
// the record byte count controls the number of string, buffer, or ABC values.

#include "triceRx.h"

const triceLog_t triceLog[] = {
	/* Trice type (  extended  ) */  //     id, bitWidth, paramCount, format-string
`)
	tail := []byte(`};

// triceLogElements is used by the RX resolver to bound the generated table.
const unsigned triceLogElements = sizeof(triceLog) / sizeof(triceLog[0]);
`)
	defaultBitWidth, err := strconv.Atoi(DefaultTriceBitWidth)
	msg.FatalOnErr(err)

	for id, t := range ilu {
		extType, bitWidth, paramCount := computeLogValues(t, defaultBitWidth)
		// strconv.Quote emits a valid C-compatible double-quoted string literal
		// for the generated format text. This keeps embedded quotes, backslashes,
		// tabs, and newlines safe when arbitrary Trice format strings enter til.c.
		quotedFormat := strconv.Quote(t.Strg)
		text = append(text, []byte(fmt.Sprintf(`	/* %10s ( %10s ) */ { %5du, %3du, %s, %s },`+"\n", t.Type, extType, id, bitWidth, paramCount, quotedFormat))...)
	}

	text = append(text, tail...)
	return text, nil
}

// computeLogValues prepares the compact C RX table. The uint8_t paramCount
// cannot use -1, so variable-size payload families use the public sentinel.
func computeLogValues(t TriceFmt, defaultBitWidth int) (extType string, bitWidth int, paramCount string) {
	DefaultTriceBitWidth = strconv.Itoa(defaultBitWidth)
	if info := abcTypeInfo(t.Type); info.isABC {
		if info.bitWidth == 0 {
			return t.Type, 0, "0u"
		}
		return t.Type, info.bitWidth, "TRICE_LOG_PARAM_COUNT_DYNAMIC"
	}
	switch triceTypeCategory(t.Type) {
	case "S", "N":
		return t.Type, 8, "TRICE_LOG_PARAM_COUNT_DYNAMIC"
	case "B":
		extType, bitWidth = logDynamicTypeWidth(t.Type, 1, defaultBitWidth)
		return extType, bitWidth, "TRICE_LOG_PARAM_COUNT_DYNAMIC"
	case "F":
		extType, bitWidth = logDynamicTypeWidth(t.Type, 0, defaultBitWidth)
		return extType, bitWidth, "TRICE_LOG_PARAM_COUNT_DYNAMIC"
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
