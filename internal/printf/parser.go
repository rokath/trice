// SPDX-License-Identifier: MIT

// Package printf provides the limited numeric expression and formatting language
// shared by visualization and target-stamp presentation. It has no output sinks.
package printf

import (
	"fmt"
	"go/ast"
	"go/parser"
	"go/token"
	"strconv"
	"strings"
)

// verbClass groups formatter verbs by their required argument conversion.
type VerbClass uint8

const (
	// verbInteger accepts signed, unsigned, or safely converted floating values.
	IntegerVerb VerbClass = iota
	// verbFloat converts numeric values to float64.
	FloatVerb
	// verbGeneric preserves the evaluator result type.
	GenericVerb
	// verbBool accepts only a direct Boolean field.
	BoolVerb
)

// formatVerb describes one consuming conversion in a validated Go format string.
type Verb struct {
	Verb  byte
	Class VerbClass
	start int // Byte offsets permit placeholders without evaluating expressions.
	end   int
}

// Program is a validated printf call, independent of routing or transport.
type Program struct {
	Format      string
	Expressions []Expression
	Verbs       []Verb
	References  References
	unwraps     []*unwrapNode
	deltas      []*deltaNode
}

// Parse compiles the complete printf call using the visualization context.
func Parse(callText string) (Program, error) {
	return parseContext(callText, &compiler{})
}

// ParseStamp enables only the matching raw stamp and the documented functions.
// Each compiled program owns one counter stream; compile again for a new source.
func ParseStamp(callText string, bits int, delta bool) (Program, error) {
	if bits != 16 && bits != 32 {
		return Program{}, fmt.Errorf("stamp width must be 16 or 32")
	}
	return parseContext(callText, &compiler{bits: bits, allowDelta: delta})
}

// parseContext shares syntax, format validation, and lowering across consumers.
func parseContext(callText string, context *compiler) (Program, error) {
	expression, err := parser.ParseExpr(callText)
	if err != nil {
		return Program{}, fmt.Errorf("invalid printf encoder: %w", err)
	}
	call, ok := expression.(*ast.CallExpr)
	if !ok {
		return Program{}, fmt.Errorf("encoder must be a printf(...) call")
	}
	function, ok := call.Fun.(*ast.Ident)
	if !ok || function.Name != "printf" {
		return Program{}, fmt.Errorf("only the printf encoder is supported")
	}
	if call.Ellipsis.IsValid() {
		return Program{}, fmt.Errorf("variadic expansion is not supported")
	}
	if len(call.Args) == 0 {
		return Program{}, fmt.Errorf("printf requires a double-quoted Go format string")
	}
	formatLiteral, ok := call.Args[0].(*ast.BasicLit)
	if !ok || formatLiteral.Kind != token.STRING ||
		len(formatLiteral.Value) < 2 || formatLiteral.Value[0] != '"' {
		return Program{}, fmt.Errorf("printf requires a double-quoted Go format string")
	}
	outputFormat, err := strconv.Unquote(formatLiteral.Value)
	if err != nil {
		return Program{}, fmt.Errorf("invalid printf format string: %w", err)
	}
	verbs, err := ParseOutputFormat(outputFormat)
	if err != nil {
		return Program{}, err
	}
	if len(call.Args)-1 != len(verbs) {
		return Program{}, fmt.Errorf(
			"printf format consumes %d values but %d expressions were supplied",
			len(verbs), len(call.Args)-1,
		)
	}

	compiled := make([]Expression, 0, len(verbs))
	var references References
	for i, argument := range call.Args[1:] {
		source := expressionSource(callText, argument)
		item, err := compileInContext(argument, source, context)
		if err != nil {
			return Program{}, fmt.Errorf("expression %d (%s): %w", i+1, source, err)
		}
		if err := ValidateExpressionVerb(item, verbs[i], nil); err != nil {
			return Program{}, fmt.Errorf("expression %d (%s): %w", i+1, source, err)
		}
		references.Merge(item.References)
		compiled = append(compiled, item)
	}
	if references.TS16 && references.TS32 {
		return Program{}, fmt.Errorf("one rule cannot reference both ts16 and ts32")
	}

	return Program{Format: outputFormat, Expressions: compiled, Verbs: verbs, References: references,
		unwraps: context.unwraps, deltas: context.deltas}, nil
}

// parseOutputFormat accepts only literal-width Go conversions promised by the MVP.
func ParseOutputFormat(format string) ([]Verb, error) {
	var verbs []Verb
	for i := 0; i < len(format); i++ {
		if format[i] != '%' {
			continue
		}
		start := i
		i++
		if i >= len(format) {
			return nil, fmt.Errorf("printf format ends with an incomplete conversion")
		}
		if format[i] == '%' {
			continue
		}

		for i < len(format) && strings.ContainsRune("+#- 0", rune(format[i])) {
			i++
		}
		if i < len(format) && (format[i] == '[' || format[i] == '*') {
			return nil, fmt.Errorf("dynamic widths and explicit argument indexes are not supported at byte %d", start)
		}
		for i < len(format) && format[i] >= '0' && format[i] <= '9' {
			i++
		}
		if i < len(format) && format[i] == '.' {
			i++
			precisionStart := i
			if i < len(format) && (format[i] == '[' || format[i] == '*') {
				return nil, fmt.Errorf("dynamic precisions and explicit argument indexes are not supported at byte %d", start)
			}
			for i < len(format) && format[i] >= '0' && format[i] <= '9' {
				i++
			}
			if i == precisionStart {
				return nil, fmt.Errorf("precision must be a literal decimal number at byte %d", start)
			}
		}
		if i >= len(format) {
			return nil, fmt.Errorf("printf format ends with an incomplete conversion")
		}
		if format[i] == '[' || format[i] == '*' {
			return nil, fmt.Errorf("dynamic widths and explicit argument indexes are not supported at byte %d", start)
		}

		verb := format[i]
		item := Verb{Verb: verb, start: start, end: i + 1}
		switch verb {
		case 'd', 'b', 'o', 'x', 'X':
			item.Class = IntegerVerb
		case 'f', 'e', 'E', 'g', 'G':
			item.Class = FloatVerb
		case 'v':
			item.Class = GenericVerb
		case 't':
			item.Class = BoolVerb
		default:
			if verb == 'u' {
				return nil, fmt.Errorf("%%u is not supported; use %%d for unsigned decimal output")
			}
			return nil, fmt.Errorf("formatter verb %%%c is not supported", verb)
		}
		verbs = append(verbs, item)
	}
	return verbs, nil
}

// expressionSource extracts the original CLI spelling for actionable diagnostics.
func expressionSource(callText string, expression ast.Expr) string {
	start := int(expression.Pos()) - 1
	end := int(expression.End()) - 1
	if start < 0 || end < start || end > len(callText) {
		return "<expression>"
	}
	return callText[start:end]
}
