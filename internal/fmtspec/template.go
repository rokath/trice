// SPDX-License-Identifier: MIT

package fmtspec

import (
	"fmt"
	"strings"
	"unicode"
)

// Field associates a canonical user name with one argument in the complete
// printf argument sequence. Ordinary printf arguments do not create fields.
type Field struct {
	Name     string
	Argument int
	Spec     Spec
}

// Template retains both dictionary identity and the derived presentation.
// Fields and Specs preserve source order; no map iteration determines output.
type Template struct {
	Canonical string
	Format    string
	Fields    []Field
	Specs     []Spec
}

// ParseTemplate handles the shared source/dictionary template language. Source
// callers supply C argument expressions for inferred names and float defaults;
// dictionary callers pass nil because all inference must already be resolved.
// The original C escapes stay intact for the existing presentation pipeline.
func ParseTemplate(format string, args []string) (Template, error) {
	var result Template
	var canonical, text strings.Builder
	seen := make(map[string]bool)
	for i := 0; i < len(format); {
		switch format[i] {
		case '{':
			if i+1 < len(format) && format[i+1] == '{' {
				canonical.WriteString("{{")
				text.WriteByte('{')
				i += 2
				continue
			}
			end := strings.IndexByte(format[i+1:], '}')
			if end < 0 {
				return Template{}, fmt.Errorf("unclosed structured placeholder at byte %d; use {{ for a literal brace", i)
			}
			end += i + 1
			body := format[i+1 : end]
			if strings.ContainsRune(body, '{') {
				return Template{}, fmt.Errorf("nested brace in structured placeholder at byte %d", i)
			}
			name, display, explicitDisplay := strings.Cut(body, ":")
			name = strings.TrimSpace(name)
			argument := len(result.Specs)
			var expression string
			if argument < len(args) {
				expression = args[argument]
			}
			unwrapped, floating := unwrapTemplateArgument(expression)
			if name == "" || strings.HasSuffix(name, ".") {
				inferred, ok := canonicalFieldName(unwrapped)
				if !ok {
					return Template{}, fmt.Errorf("placeholder %q needs an explicit field name for argument %d (%s)", body, argument+1, expression)
				}
				name += inferred
			}
			var valid bool
			name, valid = canonicalFieldName(name)
			if !valid {
				return Template{}, fmt.Errorf("invalid structured field name in {%s}", body)
			}
			if seen[name] {
				return Template{}, fmt.Errorf("duplicate structured field %q", name)
			}
			seen[name] = true
			if !explicitDisplay {
				display = "%d"
				if floating {
					display = "%f"
				}
			}
			_, specs := Normalize(display)
			if len(specs) != 1 || !validFieldDisplay(display) {
				return Template{}, fmt.Errorf("field %q requires exactly one supported printf conversion in its display", name)
			}
			result.Fields = append(result.Fields, Field{Name: name, Argument: argument, Spec: specs[0]})
			result.Specs = append(result.Specs, specs[0])
			canonical.WriteByte('{')
			canonical.WriteString(name)
			if display != "%d" {
				canonical.WriteByte(':')
				canonical.WriteString(display)
			}
			canonical.WriteByte('}')
			text.WriteString(display)
			i = end + 1
		case '}':
			if i+1 >= len(format) || format[i+1] != '}' {
				return Template{}, fmt.Errorf("unmatched closing brace at byte %d; use }} for a literal brace", i)
			}
			canonical.WriteString("}}")
			text.WriteByte('}')
			i += 2
		case '%':
			if i+1 < len(format) && format[i+1] == '%' {
				canonical.WriteString("%%")
				text.WriteString("%%")
				i += 2
				continue
			}
			_, spec, width, ok := parseSpecifier(format[i:])
			if ok {
				result.Specs = append(result.Specs, spec)
				canonical.WriteString(format[i : i+width])
				text.WriteString(format[i : i+width])
				i += width
			} else {
				// Preserve the established treatment of ordinary text containing
				// percent signs. Structured displays themselves are strict.
				canonical.WriteByte('%')
				text.WriteByte('%')
				i++
			}
		default:
			canonical.WriteByte(format[i])
			text.WriteByte(format[i])
			i++
		}
	}
	result.Canonical, result.Format = canonical.String(), text.String()
	return result, nil
}

// validFieldDisplay rejects unsupported conversions instead of allowing a
// misleading field schema alongside fmt's runtime error markers.
func validFieldDisplay(display string) bool {
	for i := 0; i < len(display); i++ {
		if display[i] != '%' {
			continue
		}
		if i+1 < len(display) && display[i+1] == '%' {
			i++
			continue
		}
		_, _, width, ok := parseSpecifier(display[i:])
		if !ok {
			return false
		}
		i += width - 1
	}
	return true
}

// canonicalFieldName accepts identifiers and member chains only. This keeps
// names usable as unquoted KV keys and refuses to infer names from expressions.
func canonicalFieldName(name string) (string, bool) {
	name = strings.ReplaceAll(strings.TrimSpace(name), "->", ".")
	parts := strings.Split(name, ".")
	for i, part := range parts {
		part = strings.TrimSpace(part)
		if part == "" {
			return "", false
		}
		for j, r := range part {
			if r != '_' && !unicode.IsLetter(r) && (j == 0 || !unicode.IsDigit(r)) {
				return "", false
			}
		}
		parts[i] = part
	}
	return strings.Join(parts, "."), true
}

// unwrapTemplateArgument recognizes only the two documented transparent float
// wrappers. General calls stay expressions and require an explicit field name.
func unwrapTemplateArgument(expression string) (string, bool) {
	expression = strings.TrimSpace(expression)
	for _, wrapper := range []string{"aFloat", "aDouble"} {
		rest, ok := strings.CutPrefix(expression, wrapper)
		rest = strings.TrimSpace(rest)
		if ok && strings.HasPrefix(rest, "(") && strings.HasSuffix(rest, ")") {
			// The wrapper must cover the whole expression. Parentheses inside
			// quoted C literals do not close the wrapper's argument list.
			depth, quote, escaped := 0, rune(0), false
			for i, r := range rest {
				if quote != 0 {
					if escaped {
						escaped = false
					} else if r == '\\' {
						escaped = true
					} else if r == quote {
						quote = 0
					}
					continue
				}
				switch r {
				case '\'', '"':
					quote = r
				case '(':
					depth++
				case ')':
					depth--
					if depth == 0 {
						if i == len(rest)-1 {
							return strings.TrimSpace(rest[1:i]), true
						}
						return expression, false
					}
				}
			}
		}
	}
	return expression, false
}
