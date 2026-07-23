// SPDX-License-Identifier: MIT

package vis

import (
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// TestRuleStringsCollectsRepeatedValues verifies the flag.Value behavior used by repeatable -vis options.
func TestRuleStringsCollectsRepeatedValues(t *testing.T) {
	var values RuleStrings
	require.NoError(t, values.Set(`a:printf("%d",v0)@a.csv`))
	require.NoError(t, values.Set(`b:printf("%d",v0)@b.csv`))
	assert.Equal(t, RuleStrings{
		`a:printf("%d",v0)@a.csv`,
		`b:printf("%d",v0)@b.csv`,
	}, values)
	assert.Contains(t, values.String(), "a:printf")
	assert.Contains(t, values.String(), "b:printf")
}

// TestParseRuleAcceptsTheMVPGrammar verifies selector, encoder, sink, expressions, and output policy separation.
func TestParseRuleAcceptsTheMVPGrammar(t *testing.T) {
	raw := `msg:printf("%8d,%0.3f,%%,%t\n",ts32/10,(v0),v1)@udp://127.0.0.1:7010;log=drop`
	rule, err := parseRule(raw)
	require.NoError(t, err)

	assert.Equal(t, raw, rule.raw)
	assert.Equal(t, "msg", rule.tag)
	assert.Equal(t, "%8d,%0.3f,%%,%t\n", rule.format)
	assert.Equal(t, "udp://127.0.0.1:7010", rule.sink)
	assert.Equal(t, outputDrop, rule.policy)
	require.Len(t, rule.expressions, 3)
	require.Len(t, rule.verbs, 3)
	assert.Equal(t, byte('d'), rule.verbs[0].verb)
	assert.Equal(t, byte('f'), rule.verbs[1].verb)
	assert.Equal(t, byte('t'), rule.verbs[2].verb)
	assert.True(t, rule.references.ts32)
	assert.True(t, rule.references.values[0])
	assert.True(t, rule.references.values[1])
}

// TestParseRuleKeepsLogByDefault verifies that omitting the policy has no effect on normal output.
func TestParseRuleKeepsLogByDefault(t *testing.T) {
	rule, err := parseRule(`msg:printf("{\"id\":%d,\"v\":%g}\n",id,v0)@file:out.jsonl`)
	require.NoError(t, err)
	assert.Equal(t, outputKeep, rule.policy)
	assert.Equal(t, `file:out.jsonl`, rule.sink)
}

// TestParseRuleHandlesQuotedDelimitersAndSinkSpellings verifies that delimiters inside the Go string are not structural.
func TestParseRuleHandlesQuotedDelimitersAndSinkSpellings(t *testing.T) {
	rule, err := parseRule(`tag:printf("literal : @ ; , ( ) \"quoted\" %d",v0)@/tmp/out.txt;log=keep`)
	require.NoError(t, err)
	assert.Equal(t, `literal : @ ; , ( ) "quoted" %d`, rule.format)
	assert.Equal(t, "/tmp/out.txt", rule.sink)
	assert.Equal(t, outputKeep, rule.policy)

	rule, err = parseRule(`tag:printf("%d",v0)@file:relative/out.txt`)
	require.NoError(t, err)
	assert.Equal(t, "file:relative/out.txt", rule.sink)

	for _, sink := range []string{"udp://127.0.0.1:7010", "udp://localhost:7010", "udp://[::1]:7010"} {
		rule, err = parseRule(`tag:printf("%d",v0)@` + sink)
		require.NoError(t, err)
		assert.Equal(t, sink, rule.sink)
	}
}

// TestParseRuleRejectsUnsupportedSyntax covers the intentionally small and future-safe MVP grammar.
func TestParseRuleRejectsUnsupportedSyntax(t *testing.T) {
	tests := []struct {
		name string
		rule string
		want string
	}{
		{name: "missing tag separator", rule: `printf("%d",v0)@out`, want: "missing selector separator"},
		{name: "empty tag", rule: `:printf("%d",v0)@out`, want: "must not be empty"},
		{name: "tag whitespace", rule: `bad tag:printf("%d",v0)@out`, want: "unsupported character"},
		{name: "other encoder", rule: `msg:json("%d",v0)@out`, want: "only the printf"},
		{name: "missing at", rule: `msg:printf("%d",v0)out`, want: "followed immediately"},
		{name: "missing closing parenthesis", rule: `msg:printf("%d",v0@out`, want: "unterminated printf"},
		{name: "unterminated quote", rule: `msg:printf("%d,v0)@out`, want: "unterminated string"},
		{name: "empty sink", rule: `msg:printf("%d",v0)@`, want: "sink must not be empty"},
		{name: "relative file URI", rule: `msg:printf("%d",v0)@file://out`, want: "plain path or file:<path>"},
		{name: "unsupported sink scheme", rule: `msg:printf("%d",v0)@tcp://localhost:7`, want: `scheme "tcp" is not supported`},
		{name: "websocket scheme", rule: `msg:printf("%d",v0)@ws://localhost:7`, want: `scheme "ws" is not supported`},
		{name: "pipe scheme", rule: `msg:printf("%d",v0)@pipe://plot`, want: `scheme "pipe" is not supported`},
		{name: "empty UDP address", rule: `msg:printf("%d",v0)@udp://`, want: "address must not be empty"},
		{name: "UDP missing port", rule: `msg:printf("%d",v0)@udp://127.0.0.1`, want: "udp://host:port"},
		{name: "UDP invalid port", rule: `msg:printf("%d",v0)@udp://127.0.0.1:port`, want: "port must be a number"},
		{name: "empty explicit file path", rule: `msg:printf("%d",v0)@file:`, want: "path must not be empty"},
		{name: "unknown option", rule: `msg:printf("%d",v0)@out;queue=1`, want: "unsupported -vis option"},
		{name: "duplicate option", rule: `msg:printf("%d",v0)@out;log=keep;log=drop`, want: "duplicate log"},
		{name: "bad log value", rule: `msg:printf("%d",v0)@out;log=hide`, want: "keep or drop"},
		{name: "raw format string", rule: "msg:printf(`%d`,v0)@out", want: "double-quoted"},
		{name: "unsupported output u", rule: `msg:printf("%u",v0)@out`, want: "%u is not supported"},
		{name: "unsupported output string", rule: `msg:printf("%s",v0)@out`, want: "%s is not supported"},
		{name: "dynamic width", rule: `msg:printf("%*d",2,v0)@out`, want: "dynamic widths"},
		{name: "explicit index", rule: `msg:printf("%[1]d",v0)@out`, want: "argument indexes"},
		{name: "missing expression", rule: `msg:printf("%d")@out`, want: "consumes 1 values"},
		{name: "extra expression", rule: `msg:printf("literal",v0)@out`, want: "consumes 0 values"},
		{name: "unknown field", rule: `msg:printf("%d",rpm)@out`, want: "unknown field"},
		{name: "value above MVP range", rule: `msg:printf("%d",v12)@out`, want: "v0 through v11"},
		{name: "leading zero value", rule: `msg:printf("%d",v01)@out`, want: "only v0 through v11"},
		{name: "function call", rule: `msg:printf("%f",float64(v0))@out`, want: "expression form"},
		{name: "selector expression", rule: `msg:printf("%f",x.v0)@out`, want: "expression form"},
		{name: "index expression", rule: `msg:printf("%f",v0[0])@out`, want: "expression form"},
		{name: "remainder operator", rule: `msg:printf("%f",v0%2)@out`, want: "operator"},
		{name: "unary plus", rule: `msg:printf("%f",+v0)@out`, want: "only unary minus"},
		{name: "mixed explicit stamps", rule: `msg:printf("%d,%d",ts16,ts32)@out`, want: "both ts16 and ts32"},
		{name: "bool from id", rule: `msg:printf("%t",id)@out`, want: "not Boolean"},
		{name: "bool from literal", rule: `msg:printf("%t",1)@out`, want: "direct Boolean"},
		{name: "bool arithmetic", rule: `msg:printf("%t",v0+1)@out`, want: "direct Boolean"},
	}

	for _, test := range tests {
		t.Run(test.name, func(t *testing.T) {
			_, err := parseRule(test.rule)
			require.Error(t, err)
			assert.Contains(t, err.Error(), test.want)
		})
	}
}

// TestParseOutputFormatSupportsPromisedVerbs verifies the complete MVP conversion set and literal percent handling.
func TestParseOutputFormatSupportsPromisedVerbs(t *testing.T) {
	verbs, err := parseOutputFormat(`%d %b %o %x %X %f %e %E %g %G %v %t %%`)
	require.NoError(t, err)
	require.Len(t, verbs, 12)
	assert.Equal(t, byte('d'), verbs[0].verb)
	assert.Equal(t, byte('t'), verbs[len(verbs)-1].verb)
}
