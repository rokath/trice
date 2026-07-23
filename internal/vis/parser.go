// SPDX-License-Identifier: MIT

// Package vis transforms selected typed TREX records into record-oriented visualization outputs.
package vis

import (
	"fmt"
	"go/ast"
	"go/parser"
	"go/token"
	"net"
	"strconv"
	"strings"
	"unicode"
)

// RuleStrings collects repeatable -vis command-line values in their original order.
type RuleStrings []string

// String implements flag.Value and provides a readable representation for command-line help.
func (values *RuleStrings) String() string {
	if values == nil {
		return ""
	}
	return strings.Join(*values, ", ")
}

// Set implements flag.Value by appending one independently parsed rule specification.
func (values *RuleStrings) Set(value string) error {
	*values = append(*values, value)
	return nil
}

// outputPolicy determines whether a successful rule keeps or suppresses the normal log record.
type outputPolicy uint8

const (
	// outputKeep is the default and leaves normal tlog output unchanged.
	outputKeep outputPolicy = iota
	// outputDrop suppresses normal output only after the rule writes successfully.
	outputDrop
)

// verbClass groups formatter verbs by their required argument conversion.
type verbClass uint8

const (
	// verbInteger accepts signed, unsigned, or safely converted floating values.
	verbInteger verbClass = iota
	// verbFloat converts numeric values to float64.
	verbFloat
	// verbGeneric preserves the evaluator result type.
	verbGeneric
	// verbBool accepts only a direct Boolean field.
	verbBool
)

// formatVerb describes one consuming conversion in a validated Go format string.
type formatVerb struct {
	verb  byte
	class verbClass
}

// parsedRule separates the selector, encoder, sink, and output policy before runtime state is added.
type parsedRule struct {
	raw         string
	tag         string
	format      string
	expressions []compiledExpression
	verbs       []formatVerb
	references  expressionReferences
	sink        string
	policy      outputPolicy
}

// parseRule validates one complete first-implementation -vis specification.
func parseRule(raw string) (parsedRule, error) {
	tag, encoderAndSink, found := strings.Cut(raw, ":")
	if !found {
		return parsedRule{}, fmt.Errorf("missing selector separator ':'")
	}
	if err := validateTag(tag); err != nil {
		return parsedRule{}, err
	}

	callText, sinkAndOptions, err := splitEncoderCall(encoderAndSink)
	if err != nil {
		return parsedRule{}, err
	}
	sink, policy, err := parseSinkAndOptions(sinkAndOptions)
	if err != nil {
		return parsedRule{}, err
	}
	if err := validateSinkSpecification(sink); err != nil {
		return parsedRule{}, err
	}

	expression, err := parser.ParseExpr(callText)
	if err != nil {
		return parsedRule{}, fmt.Errorf("invalid printf encoder: %w", err)
	}
	call, ok := expression.(*ast.CallExpr)
	if !ok {
		return parsedRule{}, fmt.Errorf("encoder must be a printf(...) call")
	}
	function, ok := call.Fun.(*ast.Ident)
	if !ok || function.Name != "printf" {
		return parsedRule{}, fmt.Errorf("only the printf encoder is supported")
	}
	if call.Ellipsis.IsValid() {
		return parsedRule{}, fmt.Errorf("variadic expansion is not supported")
	}
	if len(call.Args) == 0 {
		return parsedRule{}, fmt.Errorf("printf requires a double-quoted Go format string")
	}
	formatLiteral, ok := call.Args[0].(*ast.BasicLit)
	if !ok || formatLiteral.Kind != token.STRING ||
		len(formatLiteral.Value) < 2 || formatLiteral.Value[0] != '"' {
		return parsedRule{}, fmt.Errorf("printf requires a double-quoted Go format string")
	}
	outputFormat, err := strconv.Unquote(formatLiteral.Value)
	if err != nil {
		return parsedRule{}, fmt.Errorf("invalid printf format string: %w", err)
	}
	verbs, err := parseOutputFormat(outputFormat)
	if err != nil {
		return parsedRule{}, err
	}
	if len(call.Args)-1 != len(verbs) {
		return parsedRule{}, fmt.Errorf(
			"printf format consumes %d values but %d expressions were supplied",
			len(verbs), len(call.Args)-1,
		)
	}

	compiled := make([]compiledExpression, 0, len(verbs))
	var references expressionReferences
	for i, argument := range call.Args[1:] {
		source := expressionSource(callText, argument)
		item, err := compileExpression(argument, source)
		if err != nil {
			return parsedRule{}, fmt.Errorf("expression %d (%s): %w", i+1, source, err)
		}
		if err := validateExpressionVerb(item, verbs[i], nil); err != nil {
			return parsedRule{}, fmt.Errorf("expression %d (%s): %w", i+1, source, err)
		}
		references.merge(item.references)
		compiled = append(compiled, item)
	}
	if references.ts16 && references.ts32 {
		return parsedRule{}, fmt.Errorf("one rule cannot reference both ts16 and ts32")
	}

	return parsedRule{
		raw:         raw,
		tag:         tag,
		format:      outputFormat,
		expressions: compiled,
		verbs:       verbs,
		references:  references,
		sink:        sink,
		policy:      policy,
	}, nil
}

// validateTag keeps the MVP selector unambiguous without imposing semantics on tag names.
func validateTag(tag string) error {
	if tag == "" {
		return fmt.Errorf("the selector tag must not be empty")
	}
	for _, r := range tag {
		if unicode.IsSpace(r) || unicode.IsControl(r) || strings.ContainsRune(":@;", r) {
			return fmt.Errorf("selector tag %q contains unsupported character %q", tag, r)
		}
	}
	return nil
}

// splitEncoderCall locates the quote-aware closing parenthesis and the required sink separator.
func splitEncoderCall(input string) (call string, sinkAndOptions string, err error) {
	if !strings.HasPrefix(input, "printf(") {
		return "", "", fmt.Errorf("only the printf(...) encoder is supported")
	}
	depth := 0
	inString := false
	escaped := false
	for i := 0; i < len(input); i++ {
		ch := input[i]
		if inString {
			if escaped {
				escaped = false
				continue
			}
			switch ch {
			case '\\':
				escaped = true
			case '"':
				inString = false
			}
			continue
		}
		switch ch {
		case '"':
			inString = true
		case '(':
			depth++
		case ')':
			depth--
			if depth < 0 {
				return "", "", fmt.Errorf("unexpected ')' in printf encoder")
			}
			if depth == 0 {
				if i+1 >= len(input) || input[i+1] != '@' {
					return "", "", fmt.Errorf("printf(...) must be followed immediately by '@<sink>'")
				}
				return input[:i+1], input[i+2:], nil
			}
		}
	}
	if inString {
		return "", "", fmt.Errorf("unterminated string in printf encoder")
	}
	return "", "", fmt.Errorf("unterminated printf(...) encoder")
}

// parseSinkAndOptions separates the destination from the only MVP option, log=keep|drop.
func parseSinkAndOptions(input string) (sink string, policy outputPolicy, err error) {
	parts := strings.Split(input, ";")
	if parts[0] == "" {
		return "", outputKeep, fmt.Errorf("visualization sink must not be empty")
	}
	sink = parts[0]
	policy = outputKeep
	optionSeen := false
	for _, option := range parts[1:] {
		if option == "" {
			return "", outputKeep, fmt.Errorf("empty -vis option")
		}
		key, value, found := strings.Cut(option, "=")
		if !found || key != "log" {
			return "", outputKeep, fmt.Errorf("unsupported -vis option %q", option)
		}
		if optionSeen {
			return "", outputKeep, fmt.Errorf("duplicate log option")
		}
		optionSeen = true
		switch value {
		case "keep":
			policy = outputKeep
		case "drop":
			policy = outputDrop
		default:
			return "", outputKeep, fmt.Errorf("log option must be keep or drop, got %q", value)
		}
	}
	return sink, policy, nil
}

// validateSinkSpecification rejects reserved or ambiguous schemes even when LUT classification later disables a rule.
func validateSinkSpecification(specification string) error {
	switch {
	case strings.HasPrefix(specification, "udp://"):
		address := strings.TrimPrefix(specification, "udp://")
		if address == "" {
			return fmt.Errorf("UDP sink address must not be empty")
		}
		host, port, err := net.SplitHostPort(address)
		if err != nil {
			return fmt.Errorf("UDP sink must use udp://host:port: %w", err)
		}
		if host == "" {
			return fmt.Errorf("UDP sink host must not be empty")
		}
		portNumber, err := strconv.Atoi(port)
		if err != nil || portNumber < 1 || portNumber > 65535 {
			return fmt.Errorf("UDP sink port must be a number from 1 through 65535")
		}
		return nil
	case strings.HasPrefix(specification, "file://"):
		return fmt.Errorf(
			"%q is not supported; use a plain path or file:<path>", specification,
		)
	case strings.Contains(specification, "://"):
		scheme, _, _ := strings.Cut(specification, "://")
		return fmt.Errorf("sink scheme %q is not supported", scheme)
	}
	path := specification
	if strings.HasPrefix(path, "file:") {
		path = strings.TrimPrefix(path, "file:")
	}
	if path == "" {
		return fmt.Errorf("file sink path must not be empty")
	}
	return nil
}

// parseOutputFormat accepts only literal-width Go conversions promised by the MVP.
func parseOutputFormat(format string) ([]formatVerb, error) {
	var verbs []formatVerb
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
		item := formatVerb{verb: verb}
		switch verb {
		case 'd', 'b', 'o', 'x', 'X':
			item.class = verbInteger
		case 'f', 'e', 'E', 'g', 'G':
			item.class = verbFloat
		case 'v':
			item.class = verbGeneric
		case 't':
			item.class = verbBool
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
