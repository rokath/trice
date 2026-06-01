// SPDX-License-Identifier: MIT

package decoder

import (
	"encoding/binary"
	"fmt"
	"strings"
)

// TypeX0Result describes how a decoder shall continue after a selector-0 record.
type TypeX0Result struct {
	Text          string // Text is the optional host output generated for this X0 record.
	Consumed      int    // Consumed is the byte count to remove from the current decoded record buffer.
	BlankMetadata bool   // BlankMetadata requests aligned, value-less metadata for Text.
}

// typeX0Mode identifies the initial selector-0 handling variants.
type typeX0Mode int

const (
	// typeX0ModeError reports every selector-0 packet and drops the current decoded package.
	typeX0ModeError typeX0Mode = iota

	// typeX0ModeCountedIgnore validates counted X0 records but emits no text.
	typeX0ModeCountedIgnore

	// typeX0ModeAllIgnore drops the complete decoded selector-0 package without length checks.
	typeX0ModeAllIgnore

	// typeX0ModeCountedFormat validates counted X0 records and formats the payload with Go fmt.
	typeX0ModeCountedFormat

	// typeX0ModeInvalid reports unsupported or malformed -typeX0 mode prefixes.
	typeX0ModeInvalid
)

// typeX0Config is the parsed form of the -typeX0 CLI value.
type typeX0Config struct {
	mode            typeX0Mode // mode selects the handling path.
	format          string     // format is the Go fmt string for counted payloads.
	unsupportedMode string     // unsupportedMode is reported for future or mistyped mode prefixes.
}

// HandleTypeX0 interprets one selector-0 record at the start of record.
func HandleTypeX0(record []byte, endian bool, noneFraming bool) TypeX0Result {
	cfg := parseTypeX0Config(TypeX0)
	switch cfg.mode {
	case typeX0ModeAllIgnore:
		return TypeX0Result{Consumed: len(record)}
	case typeX0ModeError:
		return typeX0ErrorResult(len(record), "typeX0 packet ignored; configure -typeX0 to handle selector-0 records")
	case typeX0ModeInvalid:
		return typeX0ErrorResult(len(record), fmt.Sprintf("unsupported typeX0 mode %q", cfg.unsupportedMode))
	}

	payload, consumed, err := countedTypeX0Payload(record, endian, noneFraming)
	if err != nil {
		return typeX0ErrorResult(len(record), fmt.Sprintf("malformed counted typeX0 packet: %v", err))
	}
	if cfg.mode == typeX0ModeCountedIgnore {
		return TypeX0Result{Consumed: consumed}
	}
	return TypeX0Result{
		Text:          fmt.Sprintf(cfg.format, payload),
		Consumed:      consumed,
		BlankMetadata: true,
	}
}

// parseTypeX0Config parses the public -typeX0 grammar without consuming record bytes.
func parseTypeX0Config(raw string) typeX0Config {
	value := strings.TrimSpace(raw)
	if value == "" || strings.EqualFold(value, "error") {
		return typeX0Config{mode: typeX0ModeError}
	}
	lower := strings.ToLower(value)
	switch lower {
	case "ignore", "counted:ignore":
		return typeX0Config{mode: typeX0ModeCountedIgnore}
	case "all:ignore":
		return typeX0Config{mode: typeX0ModeAllIgnore}
	}
	const countedPrefix = "counted:"
	if strings.HasPrefix(lower, countedPrefix) {
		format := value[len(countedPrefix):]
		if format == "" {
			return typeX0Config{mode: typeX0ModeInvalid, unsupportedMode: "counted"}
		}
		return typeX0Config{mode: typeX0ModeCountedFormat, format: format}
	}
	mode, _, hasModePrefix := strings.Cut(value, ":")
	if hasModePrefix {
		return typeX0Config{mode: typeX0ModeInvalid, unsupportedMode: mode}
	}
	return typeX0Config{mode: typeX0ModeCountedFormat, format: value}
}

// countedTypeX0Payload returns the payload and byte consumption for the counted X0 layout.
func countedTypeX0Payload(record []byte, endian bool, noneFraming bool) ([]byte, int, error) {
	if len(record) < 2 {
		return nil, len(record), fmt.Errorf("available bytes %d < X0 header size 2", len(record))
	}

	var first uint16
	if endian == LittleEndian {
		first = binary.LittleEndian.Uint16(record[:2])
	} else {
		first = binary.BigEndian.Uint16(record[:2])
	}
	count := int(first & 0x3fff)
	logicalLen := 2 + count
	if len(record) < logicalLen {
		return nil, len(record), fmt.Errorf("available bytes %d < counted length %d", len(record), logicalLen)
	}

	consumed := logicalLen
	padding := align4(logicalLen) - logicalLen
	if padding > 0 {
		paddingEnd := logicalLen + padding
		if len(record) >= paddingEnd && isZeroBytes(record[logicalLen:paddingEnd]) {
			// NONE streams can be direct and padded or deferred and compacted.
			// Consume alignment bytes only when they are physically present.
			consumed = paddingEnd
		} else if !noneFraming && len(record) == paddingEnd {
			return nil, len(record), fmt.Errorf("non-zero alignment padding")
		}
	}
	return record[2:logicalLen], consumed, nil
}

// align4 returns n rounded up to the next 32-bit boundary.
func align4(n int) int {
	return (n + 3) &^ 3
}

// isZeroBytes reports whether each byte in b is zero.
func isZeroBytes(b []byte) bool {
	for _, v := range b {
		if v != 0 {
			return false
		}
	}
	return true
}

// typeX0ErrorResult formats diagnostics as normal decoder output and consumes the current package.
func typeX0ErrorResult(consumed int, text string) TypeX0Result {
	return TypeX0Result{
		Text:          fmt.Sprintln("ERROR:\a" + text),
		Consumed:      consumed,
		BlankMetadata: true,
	}
}
