// SPDX-License-Identifier: MIT

// Package fmtspec provides a tiny shared parser for the subset of C printf format
// specifiers that Trice needs during source analysis and host-side decoding.
//
// The parser exists to keep internal/id and internal/decoder in sync. Issue #649
// showed that the previous duplicated regex-based scanners drifted apart and did
// not recognize valid C length modifiers such as %zu, %lu, %lX or %llX.
package fmtspec

import "strings"

// Kind classifies a single format specifier for later decoder-side handling.
type Kind int

const (
	KindUnsigned Kind = iota
	KindSigned
	KindBasedInteger
	KindFloat
	KindBool
	KindPointer
	KindString
)

// Spec describes one parsed and normalized format specifier.
type Spec struct {
	Kind Kind
}

// Normalize rewrites valid C length-modified format specifiers to a length-free
// form that Go fmt understands while preserving flags, width and precision.
//
// Examples:
//
//	%zu   -> %u
//	%8lu  -> %8u
//	%li   -> %d
//	%016llX -> %016X
//	%04Lf -> %04f
//
// Unsupported or invalid combinations are intentionally left untouched and are
// not counted as recognized Trice format specifiers.
func Normalize(format string) (string, []Spec) {
	var out strings.Builder
	out.Grow(len(format))
	specs := make([]Spec, 0, strings.Count(format, "%"))

	for i := 0; i < len(format); i++ {
		if format[i] != '%' {
			out.WriteByte(format[i])
			continue
		}
		if i+1 < len(format) && format[i+1] == '%' {
			out.WriteString("%%")
			i++
			continue
		}

		normalized, spec, width, ok := parseSpecifier(format[i:])
		if !ok {
			// Keep the original percent sign and continue scanning. This preserves
			// the original text while ensuring invalid constructs such as %LX are
			// not misdetected as valid Trice format specifiers.
			out.WriteByte(format[i])
			continue
		}

		out.WriteString(normalized)
		specs = append(specs, spec)
		i += width - 1
	}

	return out.String(), specs
}

func parseSpecifier(s string) (normalized string, spec Spec, width int, ok bool) {
	if len(s) < 2 || s[0] != '%' {
		return "", Spec{}, 0, false
	}

	i := 1
	for i < len(s) && strings.ContainsRune(" +-0#'", rune(s[i])) {
		i++
	}
	for i < len(s) && isDigit(s[i]) {
		i++
	}
	if i < len(s) && s[i] == '.' {
		i++
		for i < len(s) && isDigit(s[i]) {
			i++
		}
	}

	length, lengthWidth := parseLengthModifier(s[i:])
	i += lengthWidth
	if i >= len(s) {
		return "", Spec{}, 0, false
	}

	conv := s[i]
	spec, ok = classify(length, conv)
	if !ok {
		return "", Spec{}, 0, false
	}
	goConv := normalizedConversion(conv)

	var b strings.Builder
	b.Grow(i + 1)
	b.WriteString(s[:i-lengthWidth])
	b.WriteByte(goConv)
	return b.String(), spec, i + 1, true
}

func parseLengthModifier(s string) (string, int) {
	if len(s) >= 2 {
		switch s[:2] {
		case "hh", "ll":
			return s[:2], 2
		}
	}
	if len(s) >= 1 {
		switch s[0] {
		case 'h', 'l', 'j', 'z', 't', 'L':
			// Distinguish true length modifiers from bare conversion specifiers.
			// Issue #649 triggered the shared parser work; while doing so we must
			// keep `%t` as the existing Trice bool specifier and only treat `%td`
			// or `%zu`-style constructs as C length-modified conversions.
			if len(s) >= 2 && isASCIIAlpha(s[1]) {
				return s[:1], 1
			}
		}
	}
	return "", 0
}

func classify(length string, conv byte) (Spec, bool) {
	switch conv {
	case 'd', 'i':
		return Spec{Kind: KindSigned}, isIntegerLength(length)
	case 'u':
		return Spec{Kind: KindUnsigned}, isIntegerLength(length)
	case 'x', 'X', 'o', 'O', 'b', 'q', 'U':
		return Spec{Kind: KindBasedInteger}, isIntegerLength(length)
	case 'f', 'F', 'e', 'E', 'g', 'G':
		return Spec{Kind: KindFloat}, length == "" || length == "l" || length == "L"
	case 't':
		return Spec{Kind: KindBool}, length == ""
	case 'p':
		return Spec{Kind: KindPointer}, length == ""
	case 's':
		return Spec{Kind: KindString}, length == ""
	case 'c':
		return Spec{Kind: KindSigned}, length == ""
	default:
		return Spec{}, false
	}
}

func normalizedConversion(conv byte) byte {
	// Go fmt uses %d for signed decimal integers and does not accept C's %i.
	// Normalize %i together with length-modified forms such as %hi, %li or %lli
	// so decoder output stays valid while preserving the signed-integer semantics.
	if conv == 'i' {
		return 'd'
	}
	return conv
}

func isIntegerLength(length string) bool {
	switch length {
	case "", "hh", "h", "l", "ll", "j", "z", "t":
		return true
	default:
		return false
	}
}

func isDigit(b byte) bool {
	return '0' <= b && b <= '9'
}

func isASCIIAlpha(b byte) bool {
	return ('a' <= b && b <= 'z') || ('A' <= b && b <= 'Z')
}
