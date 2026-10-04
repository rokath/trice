// SPDX-License-Identifier: MIT

package fmtspec

import (
	"strings"
	"testing"

	"github.com/stretchr/testify/assert"
)

// TestTemplateCanonicalization documents the source-to-dictionary contract.
// Each successful example is also reparsed without source expressions, proving
// that the unchanged TIL schema contains everything required by the decoder.
func TestTemplateCanonicalization(t *testing.T) {
	tests := []struct {
		name, source    string
		args            []string
		canonical, text string
		names           []string
		positions       []int
	}{
		{"plain printf keeps identity", "count=%08u %%", []string{"count"}, "count=%08u %%", "count=%08u %%", nil, nil},
		{"named integers use decimal", "info:Motor {motor_id}", []string{"motor"}, "info:Motor {motor_id}", "info:Motor %d", []string{"motor_id"}, []int{0}},
		{"automatic name and transparent float wrapper", "{}: {}", []string{"motor_id", "aFloat(temperature_c)"}, "{motor_id}: {temperature_c:%f}", "%d: %f", []string{"motor_id", "temperature_c"}, []int{0, 1}},
		{"double wrapper and explicit complex expression", "{temperature}", []string{"aDouble(getTemperature())"}, "{temperature:%f}", "%f", []string{"temperature"}, []int{0}},
		{"wrapper must cover the entire expression", "{bits}", []string{"aFloat(x) + aFloat(y)"}, "{bits}", "%d", []string{"bits"}, []int{0}},
		{"parenthesis inside quoted wrapper argument", "{temperature}", []string{`aFloat(readSensor("("))`}, "{temperature:%f}", "%f", []string{"temperature"}, []int{0}},
		{"member arrows and spacing normalize", "{}", []string{"controller -> motor.temperature_c"}, "{controller.motor.temperature_c}", "%d", []string{"controller.motor.temperature_c"}, []int{0}},
		{"explicit prefix with inferred member chain", "{plant.}", []string{"motor->temperature"}, "{plant.motor.temperature}", "%d", []string{"plant.motor.temperature"}, []int{0}},
		{"mixed positional conversions", "%d,{:%.1f},%08x,{last}", []string{"a", "aFloat(x)", "bits", "z"}, "%d,{x:%.1f},%08x,{last}", "%d,%.1f,%08x,%d", []string{"x", "last"}, []int{1, 3}},
		{"display text and colon belong to field", "Motor{motor_id:: %d, }Temperature{: = %.1f C}", []string{"id", "aFloat(temp)"}, "Motor{motor_id:: %d, }Temperature{temp: = %.1f C}", "Motor: %d, Temperature = %.1f C", []string{"motor_id", "temp"}, []int{0, 1}},
		{"literal braces and escaped percent do not consume arguments", "set={{1,2,3}} %% {value: %d%%}", []string{"value"}, "set={{1,2,3}} %% {value: %d%%}", "set={1,2,3} %%  %d%%", []string{"value"}, []int{0}},
		{"literal braces can surround a field", "{{{x}}}", []string{"x"}, "{{{x}}}", "{%d}", []string{"x"}, []int{0}},
		{"explicit default has same schema", "{x:%d}", []string{"x"}, "{x}", "%d", []string{"x"}, []int{0}},
		{"parent and member are different flat keys", "{motor} {motor.temperature}", []string{"id", "temp"}, "{motor} {motor.temperature}", "%d %d", []string{"motor", "motor.temperature"}, []int{0, 1}},
		{"C escapes stay separate", `path=\\tmp\n{x}\t`, []string{"x"}, `path=\\tmp\n{x}\t`, `path=\\tmp\n%d\t`, []string{"x"}, []int{0}},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got, err := ParseTemplate(tt.source, tt.args)
			if !assert.NoError(t, err) {
				return
			}
			assert.Equal(t, tt.canonical, got.Canonical, "dictionary schema identity")
			assert.Equal(t, tt.text, got.Format, "derived classic format")
			var names []string
			var positions []int
			for _, field := range got.Fields {
				names = append(names, field.Name)
				positions = append(positions, field.Argument)
			}
			assert.Equal(t, tt.names, names)
			assert.Equal(t, tt.positions, positions)
			reparsed, err := ParseTemplate(got.Canonical, nil)
			assert.NoError(t, err)
			assert.Equal(t, got, reparsed, "host decoding must not need the original C expressions")
		})
	}
}

// TestTemplateRejectsAmbiguousFields prevents source mistakes from silently
// changing a record's schema, consuming an extra argument, or losing a value.
func TestTemplateRejectsAmbiguousFields(t *testing.T) {
	for _, tt := range []struct{ name, source, expression, errorText string }{
		{"unclosed", "{x", "x", "unclosed"},
		{"unexpected closing brace", "x}", "x", "unmatched closing"},
		{"nested field", "{x{y}", "x", "nested brace"},
		{"empty name in dictionary", "{}", "", "explicit field name"},
		{"array expression requires name", "{}", "values[0]", "explicit field name"},
		{"call requires name", "{}", "getValue()", "explicit field name"},
		{"arithmetic requires name", "{}", "a+b", "explicit field name"},
		{"dereference requires name", "{}", "*value", "explicit field name"},
		{"duplicate canonical name", "{motor->id}{motor.id}", "x", "duplicate structured field"},
		{"invalid key", "{bad key}", "x", "invalid structured field"},
		{"numeric key", "{123}", "x", "invalid structured field"},
		{"empty display", "{x:}", "x", "exactly one"},
		{"display without value", "{x:hello}", "x", "exactly one"},
		{"two values in one field", "{x:%d %u}", "x", "exactly one"},
		{"dynamic width unsupported", "{x:%*d}", "x", "exactly one"},
		{"invalid extra conversion", "{x:%d %Z}", "x", "exactly one"},
		{"escaped percent is not a value", "{x:%%}", "x", "exactly one"},
		{"backslash is not brace escaping", `\{`, "x", "unclosed"},
	} {
		t.Run(tt.name, func(t *testing.T) {
			_, err := ParseTemplate(tt.source, []string{tt.expression})
			assert.ErrorContains(t, err, tt.errorText)
		})
	}
	longName := strings.Repeat("long_name_", 1000)
	template, err := ParseTemplate("{"+longName+"}", nil)
	assert.NoError(t, err, "field names have no artificial length limit")
	assert.Equal(t, longName, template.Fields[0].Name)
}
