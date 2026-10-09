// SPDX-License-Identifier: MIT

package translator

import (
	"bytes"
	"encoding/json"
	"fmt"
	"math"
	"strconv"
	"strings"
	"time"
	"unicode"

	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
)

// recordMember preserves the presentation order of host facts and user fields.
// Unquoted applies only to KV; JSON always uses the actual Go value's type.
type recordMember struct {
	name     string
	value    any
	unquoted bool
}

// structuredHostStamp follows the existing host-stamp option without the text
// column's trailing padding. The supplied clock makes metadata tests exact.
func structuredHostStamp(now time.Time) string {
	switch emitter.HostStamp {
	case "", "off", "none":
		return ""
	case "LOCmicro":
		return now.Format(time.StampMicro)
	case "UTCmicro":
		return "UTC " + now.UTC().Format(time.StampMicro)
	case "zero":
		return "2006-01-02_1504-05"
	default:
		return emitter.HostStamp
	}
}

// structuredTargetStamp removes a display tag such as "time:" or "dt:" while
// retaining the CLI-selected representation and any configured unit or suffix.
func structuredTargetStamp(rendered string) string {
	rendered = strings.TrimSpace(rendered)
	tag, value, found := strings.Cut(rendered, ":")
	if found && tag != "" {
		validTag := true
		for i, r := range tag {
			letter := (r >= 'a' && r <= 'z') || (r >= 'A' && r <= 'Z')
			if i == 0 && !letter {
				validTag = false
				break
			}
			if !letter && (r < '0' || r > '9') && r != '-' && r != '_' {
				validTag = false
				break
			}
		}
		if validTag {
			rendered = value
		}
	}
	return strings.TrimSpace(rendered)
}

// structuredTargetMembers keeps 16- and 32-bit stamp histories independent.
// A first delta has no comparison value and is therefore omitted entirely.
func structuredTargetMembers(record decoder.ApplicationRecord, state *targetStampState) []recordMember {
	if record.StampBits != 16 && record.StampBits != 32 {
		return nil
	}
	size := record.StampBits / 8
	absoluteFormat, deltaFormat := targetStampFormats(size)
	var members []recordMember
	if absoluteFormat != "" && absoluteFormat != "off" && absoluteFormat != "none" {
		name := "ts16"
		if size == 4 {
			name = "ts32"
		}
		members = append(members, recordMember{name, structuredTargetStamp(renderTargetStamp(size, record.Stamp)), false})
	}
	if deltaFormat != "" && deltaFormat != "off" && deltaFormat != "none" {
		hadPrevious := state.hasPrev16
		name := "ts16Delta"
		if size == 4 {
			hadPrevious = state.hasPrev32
			name = "ts32Delta"
		}
		delta := formatTargetDelta(size, deltaFormat, record.Stamp, state)
		if hadPrevious {
			members = append(members, recordMember{name, structuredTargetStamp(delta), false})
		}
	}
	return members
}

// structuredTagAndMessage classifies the original application message without
// adding a visible tag. Only an explicit lowercase format tag is removed under
// palettes that strip such tags in text mode.
func structuredTagAndMessage(record decoder.ApplicationRecord) (string, string) {
	canonical, err := emitter.FindTagName(record.Tag)
	if err != nil {
		return "untagged", emitter.DecodeDisplayEscapes(record.Message)
	}
	message := record.Message
	if emitter.ColorPalette != "off" {
		lowercase := true
		for _, r := range record.Tag {
			if unicode.IsLetter(r) && !unicode.IsLower(r) {
				lowercase = false
				break
			}
		}
		if lowercase {
			message = strings.TrimPrefix(message, record.Tag+":")
		}
	}
	return canonical, emitter.DecodeDisplayEscapes(message)
}

// renderStructuredRecord serializes one event without the text line composer.
// Newlines in messages are escaped data and never split or merge records.
func renderStructuredRecord(record decoder.ApplicationRecord, li id.TriceIDLookUpLI, now time.Time, state *targetStampState) ([]byte, error) {
	tag, message := structuredTagAndMessage(record)
	level := emitter.TagLevel(tag)
	members := []recordMember{{"tag", tag, true}}
	if level != "" {
		members = append(members, recordMember{"level", level, true})
	}
	members = append(members, recordMember{"message", message, false})
	if record.HasID && decoder.ShowID != "" {
		members = append(members, recordMember{"id", record.ID, true})
	}
	if record.HasID && id.LIFnJSON != "off" && id.LIFnJSON != "none" && decoder.LocationInformationFormatString != "off" && decoder.LocationInformationFormatString != "none" && decoder.LocationInformationFormatString != "" {
		if location, ok := li[record.ID]; ok {
			if file := strings.TrimSpace(id.LocationFile(location)); file != "" {
				members = append(members, recordMember{"file", file, false})
			}
			if location.Line != 0 {
				members = append(members, recordMember{"line", location.Line, true})
			}
		}
	}
	members = append(members, structuredTargetMembers(record, state)...)
	if stamp := strings.TrimSpace(structuredHostStamp(now)); stamp != "" {
		members = append(members, recordMember{"hs", stamp, false})
	}
	fields := make([]recordMember, 0, len(record.Fields))
	for _, field := range record.Fields {
		if value, ok := field.Value.(float64); ok && decoder.LogFormat == "json" && (math.IsNaN(value) || math.IsInf(value, 0)) {
			continue
		}
		value := field.Value
		if text, ok := value.(string); ok && !field.Address {
			value = strings.TrimSpace(text)
		}
		_, isString := value.(string)
		fields = append(fields, recordMember{field.Name, value, field.Address || !isString})
	}
	var out bytes.Buffer
	if decoder.LogFormat == "json" {
		out.WriteByte('{')
		if err := appendJSONMembers(&out, members); err != nil {
			return nil, err
		}
		if len(fields) != 0 {
			out.WriteString(",\"fields\":{")
			if err := appendJSONMembers(&out, fields); err != nil {
				return nil, err
			}
			out.WriteByte('}')
		}
		out.WriteString("}\n")
	} else {
		for _, field := range fields {
			members = append(members, recordMember{"field." + field.name, field.value, field.unquoted})
		}
		for i, member := range members {
			if i > 0 {
				out.WriteByte(' ')
			}
			out.WriteString(member.name)
			out.WriteByte('=')
			if text, ok := member.value.(string); ok {
				if member.unquoted && !strings.ContainsAny(text, " \t\r\n\"\\=") {
					out.WriteString(text)
				} else {
					out.WriteString(strconv.Quote(text))
				}
			} else {
				fmt.Fprint(&out, member.value)
			}
		}
		out.WriteByte('\n')
	}
	return out.Bytes(), nil
}

// appendJSONMembers avoids map serialization so field order follows the source.
// json.Marshal also preserves integer precision and escapes control characters.
func appendJSONMembers(out *bytes.Buffer, members []recordMember) error {
	for i, member := range members {
		if i > 0 {
			out.WriteByte(',')
		}
		name, _ := json.Marshal(member.name)
		value, err := json.Marshal(member.value)
		if err != nil {
			return fmt.Errorf("cannot encode structured field %q: %w", member.name, err)
		}
		out.Write(name)
		out.WriteByte(':')
		out.Write(value)
	}
	return nil
}
