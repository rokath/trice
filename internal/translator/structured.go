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

// canonicalLogLevel uses immutable built-in severity aliases. Weight and color
// overrides, and newly registered user tags, cannot redefine severity semantics.
func canonicalLogLevel(candidate string) string {
	groups := []struct{ level, aliases string }{
		{"FATAL", "fatal"}, {"CRITICAL", "critical crit"}, {"EMERGENCY", "emergency em"},
		{"ERROR", "error err e"}, {"WARNING", "warning warn wrn"}, {"ATTENTION", "attention att"},
		{"INFO", "info inf informal i"}, {"DEBUG", "debug dbg deb db d"}, {"TRACE", "trace tr"},
		{"NOTICE", "notice note"}, {"ALERT", "alert"}, {"ASSERT", "assert"}, {"ALARM", "alarm a"},
		{"VERBOSE", "verbose v"},
	}
	for _, group := range groups {
		for _, alias := range strings.Fields(group.aliases) {
			if strings.EqualFold(candidate, alias) {
				return group.level
			}
		}
	}
	return ""
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

// renderStructuredRecord serializes one event without the text line composer.
// Newlines in messages are escaped data and never split or merge records.
func renderStructuredRecord(record decoder.ApplicationRecord, li id.TriceIDLookUpLI, now time.Time) ([]byte, error) {
	tag := record.Tag
	message := record.Message
	level := canonicalLogLevel(tag)
	if _, err := emitter.FindTagName(tag); err == nil {
		message = strings.TrimPrefix(message, tag+":")
	} else {
		tag = "untagged"
	}
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
			if file := id.LocationFile(location); file != "" {
				members = append(members, recordMember{"file", file, false})
			}
			if location.Line != 0 {
				members = append(members, recordMember{"line", location.Line, true})
			}
		}
	}
	stampFormat, _ := targetStampFormats(record.StampBits / 8)
	if record.StampBits != 0 && stampFormat != "" && stampFormat != "off" && stampFormat != "none" {
		members = append(members, recordMember{"ts", record.Stamp, true})
	}
	if stamp := structuredHostStamp(now); stamp != "" {
		members = append(members, recordMember{"hs", stamp, false})
	}
	fields := make([]recordMember, 0, len(record.Fields))
	for _, field := range record.Fields {
		if value, ok := field.Value.(float64); ok && decoder.LogFormat == "json" && (math.IsNaN(value) || math.IsInf(value, 0)) {
			continue
		}
		_, isString := field.Value.(string)
		fields = append(fields, recordMember{field.Name, field.Value, field.Address || !isString})
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
