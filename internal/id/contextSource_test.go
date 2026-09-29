// SPDX-License-Identifier: MIT

package id

import (
	"bytes"
	"io"
	"path/filepath"
	"strings"
	"testing"

	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// prepareSourceContextTest keeps generated fields inside the isolated project.
func prepareSourceContextTest(t *testing.T, sources map[string]string, rules ...string) func() {
	t.Helper()
	teardown := prepareBindTest(t, sources)
	FieldsDir = BindDir
	ContextEnrichment = append(ArrayFlag{}, rules...)
	return teardown
}

// TestInsertCleanContextRoundTrip asserts persisted output, repeatability,
// exact source restoration and stable reuse of the final enriched schema.
func TestInsertCleanContextRoundTrip(t *testing.T) {
	for _, tc := range []struct {
		name, call, rule, physical string
		want                       TriceFmt
	}{
		{"clock example retains selector only in source", `trice("msg:ctx7:hi\n");`, `ctx7:", clock={}", clock`, `"msg:ctx7:hi, clock={}\n", clock)`, TriceFmt{Type: "trice", Strg: `msg:hi, clock={clock}\n`}},
		{"existing fields and whitespace survive cleaning", "trice( \"MSG:ctx:value={}\\n\", value );", `ctx:", count={count}", counter`, `"MSG:ctx:value={}, count={count}\n", value , counter)`, TriceFmt{Type: "trice", Strg: `MSG:value={value}, count={count}\n`}},
		{"fixed arity grows and then returns", `TRICE16_1(Id(0), "msg:ctx:{v}", v);`, `ctx:", x={}", x`, `TRICE16_2`, TriceFmt{Type: "TRICE16_2", Strg: "msg:{v}, x={x}"}},
		{"fixed zero becomes one", `trice0("msg:ctx:ready");`, `ctx:" {x}", x`, `trice_1`, TriceFmt{Type: "trice_1", Strg: "msg:ready {x}"}},
		{"literal extension works on string records", `triceS("msg:ctx:{device:%s}", "pump");`, `ctx:" online"`, `"msg:ctx:{device:%s} online", "pump")`, TriceFmt{Type: "triceS", Strg: "msg:{device:%s} online"}},
		{"float transport remains explicit", `trice32("msg:ctx:speed");`, `ctx:", m/s=%f", aFloat(velocity)`, `aFloat(velocity))`, TriceFmt{Type: "trice32", Strg: "msg:speed, m/s=%f"}},
		{"double field inference survives restoration", `trice64("msg:ctx:speed");`, `ctx:", speed={}", aDouble(velocity)`, `aDouble(velocity))`, TriceFmt{Type: "trice64", Strg: "msg:speed, speed={velocity:%f}"}},
		{"opening parenthesis character is data", `trice("msg:ctx:ready");`, `ctx:" {character}", '('`, `, '(')`, TriceFmt{Type: "trice", Strg: "msg:ready {character}"}},
		{"closing parenthesis character is data", `trice("msg:ctx:ready");`, `ctx:" {character}", ')'`, `, ')')`, TriceFmt{Type: "trice", Strg: "msg:ready {character}"}},
		{"escaped backslash n stays literal", `trice("msg:ctx:ready\\n");`, `ctx:" {x}", x`, `"msg:ctx:ready\\n {x}"`, TriceFmt{Type: "trice", Strg: `msg:ready\\n {x}`}},
		{"mixed-case selector remains visible", `trice("MSG:Ctx:ready");`, `ctx:" {x}", x`, `"MSG:Ctx:ready {x}"`, TriceFmt{Type: "trice", Strg: "MSG:Ctx:ready {x}"}},
		{"known alias retains normal tag policy", `trice("warn:ready");`, `WARNING:" {x}", x`, `"warn:ready {x}"`, TriceFmt{Type: "trice", Strg: "warn:ready {x}"}},
		{"literal context needs no arguments", `trice("msg:ctx:ready\n");`, `ctx:" online"`, `"msg:ctx:ready online\n")`, TriceFmt{Type: "trice", Strg: `msg:ready online\n`}},
		{"comments cannot close the call", `trice("msg:ctx:{x}", x /* ) */);`, `ctx:" {y}", y`, `x /* ) */, y)`, TriceFmt{Type: "trice", Strg: "msg:{x} {y}"}},
		{"CE expression comments survive safely", `trice("msg:ctx:ready");`, `ctx:" {x}", x /* ) */`, `, x /* ) */)`, TriceFmt{Type: "trice", Strg: "msg:ready {x}"}},
		{"empty extension still applies selector policy", `trice("msg:ctx:ready");`, `ctx:""`, `"msg:ctx:ready")`, TriceFmt{Type: "trice", Strg: "msg:ready"}},
	} {
		t.Run(tc.name, func(t *testing.T) {
			source := "#include \"trice.h\"\nvoid f(void) {\n    " + tc.call + "\n}\n"
			defer prepareSourceContextTest(t, map[string]string{"main.c": source}, tc.rule)()
			var output bytes.Buffer
			err := SubCmdIdInsert(&output, FSys)
			require.NoError(t, err, output.String())
			inserted, err := FSys.ReadFile(Srcs[0])
			require.NoError(t, err)
			assert.Contains(t, string(inserted), tc.physical)
			assert.NotContains(t, string(inserted), "/* trice-ce:", "CE must leave the source readable without provenance comments")
			assert.Equal(t, TriceIDLookUp{100: tc.want}, contextTestTIL(t))
			first := contextTestSnapshot(t)
			require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
			assert.Equal(t, first, contextTestSnapshot(t), "second insert must not append anything or change an ID")
			selected, err := selectCurrentLogEntries(io.Discard, FSys, contextTestTIL(t))
			require.NoError(t, err)
			assert.Equal(t, contextTestTIL(t), selected, "generate uses the explicit ID and final TIL schema without provenance")
			require.NoError(t, SubCmdIdClean(io.Discard, FSys))
			cleaned, err := FSys.ReadFile(Srcs[0])
			require.NoError(t, err)
			assert.Equal(t, source, string(cleaned), "clean restores field spelling, arity, whitespace and newline")
			afterClean := contextTestSnapshot(t)
			require.NoError(t, SubCmdIdClean(io.Discard, FSys))
			assert.Equal(t, afterClean, contextTestSnapshot(t), "second clean is harmless")
			require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
			assert.Equal(t, first, contextTestSnapshot(t), "reinsertion reuses the enriched schema's ID")
		})
	}
}

// TestSourceContextSelectionAndManualMatches checks source/CLI group ordering
// and proves that hand-written and tool-inserted matching suffixes are equal.
func TestSourceContextSelectionAndManualMatches(t *testing.T) {
	source := "trice(\"msg:ctxb:ctxa:CTXA:ready\\n\");\ntrice(\"msg:manual, x={x}\", x);\n"
	defer prepareSourceContextTest(t, map[string]string{"main.c": source}, `ctxa:", a={first}", a`, `ctxb:", b={second}", b`, `ctxa:", c={third}", c`)()
	var output bytes.Buffer
	require.NoError(t, SubCmdIdInsert(&output, FSys))
	assert.Contains(t, output.String(), "duplicate CE selector")
	assert.Equal(t, TriceFmt{Type: "trice", Strg: `msg:CTXA:ready, b={second}, a={first}, c={third}\n`}, contextTestTIL(t)[100])
	require.NoError(t, SubCmdIdClean(io.Discard, FSys))
	cleaned, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	assert.Equal(t, source, string(cleaned))
	// The complete suffix, not its origin, controls both commands.
	manual := `trice("msg:ctx:manual, x={x}", x);`
	require.NoError(t, FSys.WriteFile(Srcs[0], []byte(manual), 0o644))
	ContextEnrichment = ArrayFlag{`ctx:", x={x}", x`}
	require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
	cleaned, err = FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	assert.Equal(t, 1, strings.Count(string(cleaned), ", x={x}"), "insert recognizes a complete manual suffix")
	require.NoError(t, SubCmdIdClean(io.Discard, FSys))
	cleaned, err = FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	assert.Equal(t, `trice("msg:ctx:manual");`, string(cleaned), "clean removes a complete suffix regardless of origin")
}

// TestSourceContextRejectsBeforePublishing covers complete-project preflight,
// including invalid unselected rules and a later file's unsupported record.
func TestSourceContextRejectsBeforePublishing(t *testing.T) {
	for _, tc := range []struct{ name, rule, badCall, diagnostic string }{
		{"malformed unused rule", `other:"%d"`, `trice("msg:plain");`, "format conversion"},
		{"duplicate field", `ctx:" {v}", extra`, `trice("msg:ctx:{v}", value);`, "duplicate structured field"},
		{"too many scalar values", `ctx:"%d%d%d%d%d%d%d%d%d%d%d%d", x,x,x,x,x,x,x,x,x,x,x,x`, `trice("msg:ctx:%d", x);`, "no more than 12"},
		{"buffer cannot gain arguments", `ctx:"%d", x`, `triceB("msg:ctx:%x", buffer, size);`, "cannot append runtime"},
		{"string cannot gain arguments", `ctx:"%d", x`, `triceS("msg:ctx:%s", "device");`, "cannot append runtime"},
		{"wrong float transport", `ctx:"%f", aFloat(velocity)`, `trice64("msg:ctx:ready");`, "requires aDouble"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			defer prepareSourceContextTest(t, map[string]string{"a.c": `trice("msg:ctx:valid");`, "z.c": tc.badCall}, tc.rule)()
			before := contextTestSnapshot(t)
			assert.ErrorContains(t, SubCmdIdInsert(io.Discard, FSys), tc.diagnostic)
			assert.Equal(t, before, contextTestSnapshot(t), "no file changes on preflight failure")
		})
	}
}

// TestSourceContextCompleteSuffixContract checks the observable insert/clean
// contract on fresh source, including manual and partial extensions. Every case
// asserts exact source text and repeats insert to catch positional duplicates.
func TestSourceContextCompleteSuffixContract(t *testing.T) {
	for _, tc := range []struct {
		name, source, inserted, cleaned string
	}{
		{"absent suffix is appended before newline", `trice("msg:ctx7:hi\n");`, `trice("msg:ctx7:hi, clock=%d\n", clock);`, `trice("msg:ctx7:hi\n");`},
		{"manual complete suffix is already present", `trice("msg:ctx7:hi, clock=%d\n", clock);`, `trice("msg:ctx7:hi, clock=%d\n", clock);`, `trice("msg:ctx7:hi\n");`},
		{"complete suffix without newline", `trice("msg:ctx7:hi, clock=%d", clock);`, `trice("msg:ctx7:hi, clock=%d", clock);`, `trice("msg:ctx7:hi");`},
		{"format alone is a nonmatch", `trice("msg:ctx7:hi, clock=%d\n", other);`, `trice("msg:ctx7:hi, clock=%d, clock=%d\n", other, clock);`, `trice("msg:ctx7:hi, clock=%d\n", other);`},
		{"argument alone is a nonmatch", `trice("msg:ctx7:value=%d\n", clock);`, `trice("msg:ctx7:value=%d, clock=%d\n", clock, clock);`, `trice("msg:ctx7:value=%d\n", clock);`},
		{"matching text in middle is a nonmatch", `trice("msg:ctx7:hi, clock=%d then=%d\n", clock, other);`, `trice("msg:ctx7:hi, clock=%d then=%d, clock=%d\n", clock, other, clock);`, `trice("msg:ctx7:hi, clock=%d then=%d\n", clock, other);`},
		{"arguments must match at the same suffix position", `trice("msg:ctx7:first=%d, clock=%d\n", clock, other);`, `trice("msg:ctx7:first=%d, clock=%d, clock=%d\n", clock, other, clock);`, `trice("msg:ctx7:first=%d, clock=%d\n", clock, other);`},
		{"expression substring is a nonmatch", `trice("msg:ctx7:hi, clock=%d\n", my_clock);`, `trice("msg:ctx7:hi, clock=%d, clock=%d\n", my_clock, clock);`, `trice("msg:ctx7:hi, clock=%d\n", my_clock);`},
		{"trailing message space prevents a suffix match", `trice("msg:ctx7:hi, clock=%d \n", clock);`, `trice("msg:ctx7:hi, clock=%d , clock=%d\n", clock, clock);`, `trice("msg:ctx7:hi, clock=%d \n", clock);`},
		{"outer argument whitespace and comments are harmless", `trice("msg:ctx7:hi, clock=%d\n",  clock /* , ) */ );`, `trice("msg:ctx7:hi, clock=%d\n",  clock /* , ) */ );`, `trice("msg:ctx7:hi\n");`},
		{"nested original commas do not move CE boundary", `trice("msg:ctx7:value=%d\n", choose(a, b));`, `trice("msg:ctx7:value=%d, clock=%d\n", choose(a, b), clock);`, `trice("msg:ctx7:value=%d\n", choose(a, b));`},
		{"quoted comma is part of original argument", `trice("msg:ctx7:value=%d\n", ',');`, `trice("msg:ctx7:value=%d, clock=%d\n", ',', clock);`, `trice("msg:ctx7:value=%d\n", ',');`},
		{"escaped backslash n is ordinary message text", `trice("msg:ctx7:hi\\n");`, `trice("msg:ctx7:hi\\n, clock=%d", clock);`, `trice("msg:ctx7:hi\\n");`},
		{"selector must not be an identifier substring", `trice("msg:ctx7x:hi, clock=%d\n", clock);`, `trice("msg:ctx7x:hi, clock=%d\n", clock);`, `trice("msg:ctx7x:hi, clock=%d\n", clock);`},
		{"selector in message body does not select", `trice("msg:hello ctx7:hi, clock=%d\n", clock);`, `trice("msg:hello ctx7:hi, clock=%d\n", clock);`, `trice("msg:hello ctx7:hi, clock=%d\n", clock);`},
	} {
		t.Run(tc.name, func(t *testing.T) {
			defer prepareSourceContextTest(t, map[string]string{"main.c": tc.source}, `ctx7:", clock=%d", clock`)()
			// Clean sees freshly hand-written source; no earlier insert or TIL
			// entry may be required to recognize an existing full suffix.
			wantFreshClean := tc.source
			if tc.source == tc.inserted {
				wantFreshClean = tc.cleaned
			}
			require.NoError(t, SubCmdIdClean(io.Discard, FSys))
			freshClean, err := FSys.ReadFile(Srcs[0])
			require.NoError(t, err)
			assert.Equal(t, wantFreshClean, string(freshClean), "clean ignores partial matches completely")
			require.NoError(t, FSys.WriteFile(Srcs[0], []byte(tc.source), 0o644))
			require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
			inserted, err := FSys.ReadFile(Srcs[0])
			require.NoError(t, err)
			assert.Equal(t, strings.Replace(tc.inserted, "trice(", "trice(iD(100), ", 1), string(inserted))
			first := contextTestSnapshot(t)
			require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
			assert.Equal(t, first, contextTestSnapshot(t), "insert is idempotent after complete or partial input")
			require.NoError(t, SubCmdIdClean(io.Discard, FSys))
			cleaned, err := FSys.ReadFile(Srcs[0])
			require.NoError(t, err)
			assert.Equal(t, tc.cleaned, string(cleaned))
		})
	}
}

// TestSourceContextMatchesWholeRuleGroup prevents independently matched pieces
// from being mistaken for the complete, correctly ordered extension.
func TestSourceContextMatchesWholeRuleGroup(t *testing.T) {
	for _, tc := range []struct{ name, source, inserted, cleaned string }{
		{"complete group", `trice("ctx:ready a=%d b=%d", a, b);`, `trice("ctx:ready a=%d b=%d", a, b);`, `trice("ctx:ready");`},
		{"first rule alone", `trice("ctx:ready a=%d", a);`, `trice("ctx:ready a=%d a=%d b=%d", a, a, b);`, `trice("ctx:ready a=%d", a);`},
		{"last rule alone", `trice("ctx:ready b=%d", b);`, `trice("ctx:ready b=%d a=%d b=%d", b, a, b);`, `trice("ctx:ready b=%d", b);`},
		{"reversed format group", `trice("ctx:ready b=%d a=%d", b, a);`, `trice("ctx:ready b=%d a=%d a=%d b=%d", b, a, a, b);`, `trice("ctx:ready b=%d a=%d", b, a);`},
		{"reversed argument group", `trice("ctx:ready a=%d b=%d", b, a);`, `trice("ctx:ready a=%d b=%d a=%d b=%d", b, a, a, b);`, `trice("ctx:ready a=%d b=%d", b, a);`},
	} {
		t.Run(tc.name, func(t *testing.T) {
			defer Setup(t)()
			rules, err := parseContextRules([]string{`ctx:" a=%d", a`, `ctx:" b=%d", b`})
			require.NoError(t, err)
			inserted, err := transformSourceContext(io.Discard, "main.c", tc.source, rules, false, false)
			require.NoError(t, err)
			assert.Equal(t, tc.inserted, inserted)
			repeated, err := transformSourceContext(io.Discard, "main.c", inserted, rules, false, false)
			require.NoError(t, err)
			assert.Equal(t, inserted, repeated)
			cleaned, err := transformSourceContext(io.Discard, "main.c", inserted, rules, true, true)
			require.NoError(t, err)
			assert.Equal(t, tc.cleaned, cleaned)
		})
	}
}

// TestSourceContextLiteralBoundaries covers argument-free matching, including
// rule-owned newlines and text that must not consume the selecting prefix.
func TestSourceContextLiteralBoundaries(t *testing.T) {
	for _, tc := range []struct{ name, source, rule, inserted, cleaned string }{
		{"rule adds its own newline", `trice("ctx:hi");`, `ctx:" online\n"`, `trice("ctx:hi online\n");`, `trice("ctx:hi");`},
		{"original newline and rule newline", `trice("ctx:hi\n");`, `ctx:" online\n"`, `trice("ctx:hi online\n\n");`, `trice("ctx:hi\n");`},
		{"empty rule is already present", `trice("ctx:hi\n");`, `ctx:""`, `trice("ctx:hi\n");`, `trice("ctx:hi\n");`},
		{"suffix must preserve its selector", `trice("ctx:");`, `ctx:"ctx:"`, `trice("ctx:ctx:");`, `trice("ctx:");`},
		{"literal manual suffix is removable", `trice("ctx:hi online");`, `ctx:" online"`, `trice("ctx:hi online");`, `trice("ctx:hi");`},
		{"literal suffix must not cut a printf conversion", `trice("ctx:value=%d", x);`, `ctx:"d"`, `trice("ctx:value=%dd", x);`, `trice("ctx:value=%d", x);`},
		{"escaped percent does not own the last value", `trice("ctx:value=%d %%d", clock);`, `ctx:"%d", clock`, `trice("ctx:value=%d %%d%d", clock, clock);`, `trice("ctx:value=%d %%d", clock);`},
	} {
		t.Run(tc.name, func(t *testing.T) {
			defer Setup(t)()
			rules, err := parseContextRules([]string{tc.rule})
			require.NoError(t, err)
			freshClean, err := transformSourceContext(io.Discard, "main.c", tc.source, rules, true, true)
			require.NoError(t, err)
			wantFreshClean := tc.source
			if tc.source == tc.inserted {
				wantFreshClean = tc.cleaned
			}
			assert.Equal(t, wantFreshClean, freshClean, "literal text inside a conversion is not a complete CE format")
			inserted, err := transformSourceContext(io.Discard, "main.c", tc.source, rules, false, false)
			require.NoError(t, err)
			assert.Equal(t, tc.inserted, inserted)
			repeated, err := transformSourceContext(io.Discard, "main.c", inserted, rules, false, false)
			require.NoError(t, err)
			assert.Equal(t, inserted, repeated)
			cleaned, err := transformSourceContext(io.Discard, "main.c", inserted, rules, true, true)
			require.NoError(t, err)
			assert.Equal(t, tc.cleaned, cleaned)
		})
	}
}

// TestSourceContextCurrentRulesAndEditedMessage makes provenance irrelevant:
// editing ordinary text or replacing the CLI rule does not cause an ownership
// error. Only a currently matching end group can be removed.
func TestSourceContextCurrentRulesAndEditedMessage(t *testing.T) {
	defer prepareSourceContextTest(t, map[string]string{"main.c": `trice("ctx:ready");`}, `ctx:" a=%d", a`)()
	require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
	content, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	require.NoError(t, FSys.WriteFile(Srcs[0], bytes.Replace(content, []byte("ready"), []byte("edited"), 1), 0o644))
	ContextEnrichment = ArrayFlag{`ctx:" b=%d", b`}
	require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
	content, err = FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	assert.Contains(t, string(content), `"ctx:edited a=%d b=%d", a, b)`)
	ContextEnrichment = ArrayFlag{`ctx:" a=%d", a`}
	require.NoError(t, SubCmdIdClean(io.Discard, FSys))
	content, err = FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	assert.Equal(t, `trice("ctx:edited a=%d b=%d", a, b);`, string(content), "a matching group in the middle is not removed")
	ContextEnrichment = ArrayFlag{`ctx:" b=%d", b`}
	require.NoError(t, SubCmdIdClean(io.Discard, FSys))
	content, err = FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	assert.Equal(t, `trice("ctx:edited a=%d", a);`, string(content))
	ContextEnrichment = ArrayFlag{`ctx:" a=%d", a`}
	require.NoError(t, SubCmdIdClean(io.Discard, FSys))
	content, err = FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	assert.Equal(t, `trice("ctx:edited");`, string(content))
}

// TestSourceContextCleanRemovesOneGroupPerInvocation documents the deliberate
// absence of history: repeated manual suffixes can be removed one at a time.
func TestSourceContextCleanRemovesOneGroupPerInvocation(t *testing.T) {
	defer prepareSourceContextTest(t, map[string]string{"main.c": `trice("ctx:ready x=%d x=%d", x, x);`}, `ctx:" x=%d", x`)()
	for _, want := range []string{`trice("ctx:ready x=%d", x);`, `trice("ctx:ready");`, `trice("ctx:ready");`} {
		require.NoError(t, SubCmdIdClean(io.Discard, FSys))
		content, err := FSys.ReadFile(Srcs[0])
		require.NoError(t, err)
		assert.Equal(t, want, string(content))
	}
}

// TestSourceContextAliasWithUnderscoreKeepsItsName ensures that user aliases
// are not mistaken for the fixed-arity suffix of a built-in Trice macro.
func TestSourceContextAliasWithUnderscoreKeepsItsName(t *testing.T) {
	const source = `my_trace("ctx:ready");`
	defer prepareSourceContextTest(t, map[string]string{"main.c": source}, `ctx:" x=%d", x`)()
	TriceAliases = ArrayFlag{"my_trace"}
	ProcessAliases()
	t.Cleanup(ProcessAliases)
	require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
	inserted, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	assert.Contains(t, string(inserted), `my_trace(iD(100), "ctx:ready x=%d", x)`)
	require.NoError(t, SubCmdIdClean(io.Discard, FSys))
	cleaned, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	assert.Equal(t, source, string(cleaned))
}

// TestSourceContextMovesWithItsCall does not rely on source paths, line numbers,
// or build files to recognize its own output. Legacy ID-only clean is harmless.
func TestSourceContextMovesWithItsCall(t *testing.T) {
	defer prepareSourceContextTest(t, map[string]string{"main.c": `trice("msg:ctx:ready\n");`}, `ctx:" {x}", x`)()
	TriceCacheEnabled = true
	require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
	inserted, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	moved := filepath.Join(Proj, t.Name(), "moved.c")
	require.NoError(t, FSys.WriteFile(moved, append([]byte("// Moved call.\n\n"), inserted...), 0o644))
	require.NoError(t, FSys.Remove(Srcs[0]))
	require.NoError(t, FSys.RemoveAll(FieldsDir))
	Srcs = ArrayFlag{moved}
	rules := ContextEnrichment
	ContextEnrichment = nil
	require.NoError(t, SubCmdIdClean(io.Discard, FSys))
	partiallyCleaned, err := FSys.ReadFile(moved)
	require.NoError(t, err)
	assert.NotContains(t, string(partiallyCleaned), "iD(")
	assert.Contains(t, string(partiallyCleaned), `ready {x}\n", x)`, "ID-only clean must retain the CE suffix")
	ContextEnrichment = rules
	require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
	assert.Len(t, contextTestTIL(t), 1, "moving a call and deleting build output do not create a new schema")
	require.NoError(t, SubCmdIdClean(io.Discard, FSys))
	cleaned, err := FSys.ReadFile(moved)
	require.NoError(t, err)
	assert.Equal(t, "// Moved call.\n\ntrice(\"msg:ctx:ready\\n\");", string(cleaned))
}

// TestSourceContextRespectsSelection checks commented and excluded regions,
// multiline calls, macro definitions, repeated rules, and current field counts.
func TestSourceContextRespectsSelection(t *testing.T) {
	source := "// trice(\"msg:ctx:comment\");\n/* trice(\"msg:ctx:block comment\"); */\n" +
		"// TRICE_INSERT_OFF\ntrice(\"msg:ctx:excluded region\");\n// TRICE_INSERT_ON\n" +
		"#define WRAP() do { trice(\"msg:ctx:one\"); trice(\"msg:ctx:two\"); } while (0)\n" +
		"trice(\n  \"msg:ctx:multiline\\n\"\n);\nWRAP();\n"
	defer prepareSourceContextTest(t, map[string]string{"main.c": source, "excluded.c": `trice("msg:ctx:excluded file");`}, `ctx:" {x}", x`)()
	excluded := filepath.Join(Proj, t.Name(), "excluded.c")
	ExcludeSrcs = ArrayFlag{excluded}
	require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
	path := filepath.Join(Proj, t.Name(), "main.c")
	inserted, err := FSys.ReadFile(path)
	require.NoError(t, err)
	assert.Equal(t, 3, strings.Count(string(inserted), "{x}"))
	assert.NotContains(t, string(inserted), "/* trice-ce:")
	assert.Contains(t, string(inserted), `trice("msg:ctx:excluded region");`)
	registry, err := FSys.ReadFile(filepath.Join(FieldsDir, "trice-fields.txt"))
	require.NoError(t, err)
	assert.Equal(t, "       3 x\n", string(registry))
	excludedContent, err := FSys.ReadFile(excluded)
	require.NoError(t, err)
	assert.Equal(t, `trice("msg:ctx:excluded file");`, string(excludedContent))
	require.NoError(t, SubCmdIdClean(io.Discard, FSys))
	cleaned, err := FSys.ReadFile(path)
	require.NoError(t, err)
	assert.Equal(t, source, string(cleaned), "commented calls retain ordinary ID behavior and are not enriched")
}

// TestSourceContextAtomicPublish exercises real rollback after one replacement
// has succeeded, and dry-run isolation for both insertion and cleanup.
func TestSourceContextAtomicPublish(t *testing.T) {
	defer prepareSourceContextTest(t, map[string]string{"a.c": `trice("msg:ctx:one");`, "b.c": `trice("msg:ctx:two");`}, `ctx:" {x}", x`)()
	for _, clean := range []bool{false, true} {
		command := SubCmdIdInsert
		if clean {
			require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
			command = SubCmdIdClean
		}
		before := contextTestSnapshot(t)
		DryRun = true
		require.NoError(t, command(io.Discard, FSys))
		assert.Equal(t, before, contextTestSnapshot(t), "dry run must not publish any source or metadata")
		DryRun = false
		failing := &bindFailOnceRenameFs{Fs: FSys.Fs, destination: filepath.Join(Proj, t.Name(), "b.c")}
		assert.ErrorContains(t, command(io.Discard, &afero.Afero{Fs: failing}), "injected rename failure")
		assert.True(t, failing.failed, "the failure must reach the publishing stage")
		assert.Equal(t, before, contextTestSnapshot(t), "rollback must restore every already published file")
	}
}
