// SPDX-License-Identifier: MIT

// Package vis transforms selected typed TREX records into record-oriented visualization outputs.
package vis

import (
	"fmt"
	"net"
	"strconv"
	"strings"
	"unicode"

	"github.com/rokath/trice/internal/printf"
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

// parsedRule separates the selector, encoder, sink, and output policy before runtime state is added.
type parsedRule struct {
	raw         string
	tag         string
	format      string
	expressions []printf.Expression
	verbs       []printf.Verb
	references  printf.References
	sink        string
	policy      outputPolicy
	header      string
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
	sink, policy, header, err := parseSinkAndOptions(sinkAndOptions)
	if err != nil {
		return parsedRule{}, err
	}
	if err := validateSinkSpecification(sink); err != nil {
		return parsedRule{}, err
	}

	program, err := printf.Parse(callText)
	if err != nil {
		return parsedRule{}, err
	}

	return parsedRule{
		raw:         raw,
		tag:         tag,
		format:      program.Format,
		expressions: program.Expressions,
		verbs:       program.Verbs,
		references:  program.References,
		sink:        sink,
		policy:      policy,
		header:      header,
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

// parseSinkAndOptions separates the destination, output policy, and optional one-time header.
func parseSinkAndOptions(input string) (sink string, policy outputPolicy, header string, err error) {
	parts := strings.Split(input, ";")
	if parts[0] == "" {
		return "", outputKeep, "", fmt.Errorf("visualization sink must not be empty")
	}
	sink = parts[0]
	policy = outputKeep
	logSeen := false
	headerSeen := false
	for _, option := range parts[1:] {
		if option == "" {
			return "", outputKeep, "", fmt.Errorf("empty -vis option")
		}
		key, value, found := strings.Cut(option, "=")
		if !found {
			return "", outputKeep, "", fmt.Errorf("unsupported -vis option %q", option)
		}
		switch key {
		case "log":
			if logSeen {
				return "", outputKeep, "", fmt.Errorf("duplicate log option")
			}
			logSeen = true
			switch value {
			case "keep":
				policy = outputKeep
			case "drop":
				policy = outputDrop
			default:
				return "", outputKeep, "", fmt.Errorf("log option must be keep or drop, got %q", value)
			}
		case "header":
			if headerSeen {
				return "", outputKeep, "", fmt.Errorf("duplicate header option")
			}
			if len(value) < 2 || value[0] != '"' || value[len(value)-1] != '"' {
				return "", outputKeep, "", fmt.Errorf("header option must be a quoted Go string")
			}
			decoded, decodeErr := strconv.Unquote(value)
			if decodeErr != nil {
				return "", outputKeep, "", fmt.Errorf("invalid header string: %w", decodeErr)
			}
			header = decoded
			headerSeen = true
		default:
			return "", outputKeep, "", fmt.Errorf("unsupported -vis option %q", option)
		}
	}
	return sink, policy, header, nil
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
